clear all
close all
clc

%% 1. Señal de prueba
N = 1500;
fs = 8000;
t = 0:1/fs:(N-1)/fs;
frecuencia_prueba = 82.41;
% Nombre del archivo de audio. Asegúrate de que esté en la misma carpeta.
%nombre_archivo = '6ta_MI_81.4.ogg'; 
nombre_archivo = '465490__timidity__savarez-esaite.flac'; 
nombre_archivo = 'cuerdalucas.mp4'; 

%nombre_archivo = '1ra_MI_330.ogg'; 

% Frecuencia de muestreo objetivo a la que queremos la señal final.
fs_objetivo = fs; % 1000 Hz

fprintf('--- Iniciando Procesamiento de Audio ---\n');

%% 2. Carga y Preparación del Audio
% Comprobamos si el archivo existe antes de intentar leerlo.
if ~exist(nombre_archivo, 'file')
    error('El archivo de audio "%s" no se encontró. Asegúrate de que esté en la misma carpeta que este script.', nombre_archivo);
end

% Leer el archivo de audio. 'y_original' es la señal, 'fs_original' es su frecuencia de muestreo.
[y_original, fs_original] = audioread(nombre_archivo);
fprintf('Archivo "%s" cargado. Frecuencia de muestreo original: %d Hz.\n', nombre_archivo, fs_original);

% Convertir a monoaural (un solo canal) si es estéreo.
if size(y_original, 2) > 1
    fprintf('La señal es estéreo. Convirtiendo a mono...\n');
    y_original = mean(y_original, 2);
end

%% 3. Remuestreo y Filtrado Anti-Aliasing
% Esta es la parte clave. La función 'resample' de MATLAB es muy potente.
% Antes de bajar la frecuencia de muestreo, automáticamente diseña y aplica
% un filtro pasa-bajos (filtro anti-aliasing) para eliminar todas las
% frecuencias por encima de la nueva frecuencia de Nyquist (fs_objetivo / 2),
% que en este caso es 500 Hz.
%
% ESTE ES EL PASO QUE "ELIMINA LOS ARMÓNICOS A PARTIR DE 1K" de forma correcta
% para evitar el aliasing en una señal de 1000 Hz.

fprintf('Remuestreando la señal a %d Hz. Esto incluye el filtro anti-aliasing...\n', fs_objetivo);
y_procesada = resample(y_original, fs_objetivo, fs_original);

x = y_procesada(4500:5999)*10;

x = round(x,2);
m = length(t);
disp(['Longitud de la señal: ', num2str(m)]);
if m ~= N
    error('Error: La longitud de la señal debe ser igual a N.');
end

%% 2. Configuración y apertura del puerto serial
delete(instrfind);
SerialPort='COM4'; % Asegúrate de que este es tu puerto COM
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

%% 3. Envío de la señal al PSoC
disp(['Enviando señal de prueba de ' num2str(frecuencia_prueba) ' Hz...']);
fwrite(s, x, 'float');
disp('Esperando a que el PSoC procese los datos...');
pause(5);

%% 4. Recibir el Eco (Comando 'P')
disp('Solicitando eco de la señal (comando P)...');
datos_eco = 0;
flushinput(s);
fwrite(s,'P');

longitud = m+1;
count = 1;
k=0;

while ~isequal(count,longitud)
    k=k+1;
    if k==longitud
        disp('K-Timeout alcanzado (ECO).');
        break;
    end
    datos_eco(count) = str2double(fscanf(s));
    count = count+1;
end
disp('Recepción (ECO) finalizada.');
pause(2);

%% 5. Recibir la FRECUENCIA CALCULADA (Comando 'F')
disp('Solicitando la FRECUENCIA calculada (comando F)...');
datos_procesados = 0;
flushinput(s);
fwrite(s,'F');

longitud = 2;
count = 1;
k=0;

while ~isequal(count,longitud)
    k=k+1;
    if k==longitud
        disp('K-Timeout alcanzado (FRECUENCIA).');
        break;
    end
    datos_procesados(count) = str2double(fscanf(s));
    count = count+1;
end
disp('Recepción (FRECUENCIA) finalizada.');

%% 6. Cierre del puerto serial
flushinput(s);
fclose(s);
delete(s);
clear s;
disp("Puerto serial cerrado.");

%% 7. Análisis del Resultado Final
if isequal(datos_procesados, 0) || length(datos_procesados) < 1
    frecuencia_recibida = 0;
else
    frecuencia_recibida = datos_procesados(1); % Tomamos el primer valor, todos son iguales
end

fprintf('\n\n==================== RESULTADO FINAL ====================\n');
fprintf('La frecuencia de prueba enviada fue: %.2f Hz\n', frecuencia_prueba);
fprintf('La frecuencia calculada por el PSoC es: %.2f Hz\n', frecuencia_recibida);
fprintf('=========================================================\n\n');

% Graficar las señales
figure
subplot(2,1,1)
plot(t, x)
title('Señal Original Enviada (MATLAB)');
ylabel('Amplitud');
grid on;

subplot(2,1,2)
plot(t, datos_eco)
title('Señal Recibida (Eco del PSoC)');
ylabel('Amplitud');
xlabel('Tiempo (s)');
grid on;