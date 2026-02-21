clear all
close all
clc

%% 1. Señal de prueba
f = 112; % Frecuencia del coseno (Hz)
fs = 1000; % Frecuencia de muestreo (Hz)
t = 0:1/fs:(1-1/fs); % Vector de tiempo (100 puntos)

% float freq_E2 = 82.41;
% float freq_A2 = 110.0;
% float freq_D3 = 146.83;
% float freq_G3 = 196.00;
% float freq_B3 = 246.94;
% float freq_E4 = 329.63;

x = generarSenaGuitarra(f);

x = round(x, 2);

m = length(t); % m = 100
disp(['Longitud de la señal: ', num2str(m)]);
if m ~= fs
    error('Error: La longitud (m) debe ser 100 para coincidir con N_RECEPCION del PSoC.');
end

%% Se configura el puerto serial y se abre el canal
delete(instrfind);
SerialPort='COM4'; %serial port
fincad = 'CR/LF';
baudios = 115200;
s = serial(SerialPort);
set(s,'BaudRate',baudios,'DataBits', 8, 'Parity', 'none','StopBits', 1,'FlowControl', 'none','Timeout',1);
set(s,'Terminator',fincad);


s.OutputBufferSize = 4098;
s.InputBufferSize = 2048;  % Tamaño del buffer en bytes
flushinput(s);
s.BytesAvailableFcnCount = m;
s.BytesAvailableFcnMode = 'byte';
%Se abre el puerto de comunicación
fopen(s);

fwrite(s, x, 'float');% Enviamos la senal creada al psoc
pause(5) %esperamos 5 segundos para darle tiempo al psoc de procesar todos los datos


%% 4. Recibir el Eco (Comando 'P')
disp('Solicitando eco de la señal (comando P)...');
datos_eco = 0; % Variable para guardar datos recibidos
flushinput(s);
fwrite(s,'P'); % Solicitamos los datos originales (el eco)

longitud = m+1;
count = 1;
k=0;

disp(['Iniciando bucle de recepción (ECO) para ' num2str(m) ' puntos...']);
while ~isequal(count,longitud)
 %%Re creating Serial port before timeout
    k=k+1;  
    if k==longitud
        disp('K-Timeout alcanzado (ECO). Reiniciando puerto...');
        fclose(s);
        delete(s);
        clear s;        
        s = serial(SerialPort);
        set(s,'Terminator',fincad);
        set(s,'BaudRate',baudios,'Parity','none','Timeout',1);
        fopen(s);
        k=0;
    end
    datos_eco(count) = str2double(fscanf(s));
    count = count+1;
end
disp('Recepción (ECO) finalizada.');

%% 5. Recibir Datos PROCESADOS (Comando 'F')
% AHORA PEDIMOS LOS DATOS PROCESADOS (y_procesado)
disp('Solicitando datos PROCESADOS (comando F)...');
datos_procesados = 0; % Nueva variable
flushinput(s);
fwrite(s,'F'); % Solicitamos los datos procesados

longitud = m+1;
count = 1;
k=0;

disp(['Iniciando bucle de recepción (PROCESADOS) para ' num2str(m) ' puntos...']);
while ~isequal(count,longitud)
 %%Re creating Serial port before timeout
    k=k+1;  
    if k==longitud
        disp('K-Timeout alcanzado (PROCESADOS). Reiniciando puerto...');
        fclose(s);
        delete(s);
        clear s;        
        s = serial(SerialPort);
        set(s,'Terminator',fincad);
        set(s,'BaudRate',baudios,'Parity','none','Timeout',1);
        fopen(s);
        k=0;
    end
    datos_procesados(count) = str2double(fscanf(s));
    count = count+1;
end
disp('Recepción (PROCESADOS) finalizada.');
%% 6. Graficar para comparar (Con mejoras de robustez)
figure('Name', 'Resultados de Comunicación con PSoC', 'NumberTitle', 'off');
subplot(3,1,1)
plot(t, x, 'b-o', 'LineWidth', 1.5)
title(['Señal Original Enviada (MATLAB)']);
ylabel('Amplitud');
grid on;
legend('Enviada');

subplot(3,1,2)
% Graficamos solo los puntos que se recibieron correctamente
plot(t, datos_eco, 'r-x') 
title('Señal Recibida (Eco del PSoC)');
ylabel('Amplitud');
grid on;
legend('Eco Recibido');

% --- GRÁFICO DEL AFINADOR ---
subplot(3,1,3)
% Asegurarnos de que tenemos al menos 6 datos antes de intentar graficarlos
if length(datos_procesados) >= 6
    magnitudes_afinador = datos_procesados(1:6);
    labels = {'Mi (E2)', 'La (A2)', 'Re (D3)', 'Sol (G3)', 'Si (B3)', 'Mi (E4)'};
    bar(magnitudes_afinador, 'g')
    set(gca, 'xtick', 1:6, 'xticklabel', labels);
    title('Magnitudes Goertzel (Recibidas del PSoC)');
    ylabel('Energía');
    grid on;
else
    title('No se recibieron suficientes datos de Goertzel para graficar');
end
    
%% 7. Clean up the serial port (Sin cambios)
disp("Cerrando puerto serial...")
fclose(s);
delete(s);
clear s;
disp("Puerto serial cerrado.")