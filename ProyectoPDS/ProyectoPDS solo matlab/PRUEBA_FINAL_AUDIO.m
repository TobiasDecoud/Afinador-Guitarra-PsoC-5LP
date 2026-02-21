clear all
close all
clc

%% 1. Configuración de Parámetros y Audio
N = 1000;
fs = 2000;
nombre_archivo = 'cuerdalucas.mp4'; 
fs_objetivo = fs; 

fprintf('--- Iniciando Procesamiento de Audio ---\n');

if ~exist(nombre_archivo, 'file')
    error('El archivo de audio no se encontró.');
end

[y_original, fs_original] = audioread(nombre_archivo);

if size(y_original, 2) > 1
    y_original = mean(y_original, 2);
end

%% 2. Remuestreo y Recorte de Señal
y_procesada = resample(y_original, fs_objetivo, fs_original);

x = y_procesada(4500:5499);
x = round(x, 2);
m = length(x);

if m ~= N
    error('Error: La longitud de la señal debe ser igual a N.');
end

%% 3. Configuración del Puerto Serial
delete(instrfind);
SerialPort = 'COM4'; 
fincad = 'CR/LF';
baudios = 115200;

s = serial(SerialPort);
set(s,'BaudRate',baudios,'DataBits', 8, 'Parity', 'none','StopBits', 1,'FlowControl', 'none','Timeout',1);
set(s,'Terminator',fincad);
flushinput(s);
s.OutputBufferSize = 10000;
s.InputBufferSize = 2048;
flushinput(s);
s.BytesAvailableFcnCount = m;
s.BytesAvailableFcnMode = 'byte';

fopen(s);
disp(['Puerto ' SerialPort ' abierto.']);

%% 4. Envío de Datos al PSoC
disp('Enviando señal al PSoC...');
fwrite(s, x, 'float');
pause(5);

%% 5. Recepción de Eco (Comando P)
disp('Solicitando eco (P)...');
datos_eco = 0;
flushinput(s);
fwrite(s,'P');

longitud = m+1;
count = 1;
k = 0;

while ~isequal(count, longitud)
    k = k + 1;
    if k == longitud
        disp('Timeout alcanzado (ECO).');
        break;
    end
    datos_eco(count) = str2double(fscanf(s));
    count = count + 1;
end
disp('Eco recibido.');
pause(2);

%% 6. Recepción de Frecuencia Calculada (Comando F)
disp('Solicitando frecuencia calculada (F)...');
datos_procesados = 0;
flushinput(s);
fwrite(s,'F');

longitud = 2;
count = 1;
k = 0;

while ~isequal(count, longitud)
    k = k + 1;
    if k == longitud
        disp('Timeout alcanzado (FRECUENCIA).');
        break;
    end
    datos_procesados(count) = str2double(fscanf(s));
    count = count + 1;
end
disp('Frecuencia recibida.');

%% 7. Cierre del Puerto Serial
flushinput(s);
fclose(s);
delete(s);
clear s;
disp("Puerto serial cerrado.");

%% 8. Visualización de Resultados
if isequal(datos_procesados, 0) || length(datos_procesados) < 1
    frecuencia_recibida = 0;
else
    frecuencia_recibida = datos_procesados(1); 
end

fprintf('\n==================== RESULTADO ====================\n');
fprintf('Frecuencia calculada por el PSoC: %.2f Hz\n', frecuencia_recibida);
fprintf('===================================================\n');