% = a=======================================================================
% SCRIPT: Filtrar audio en tiempo real y visualizar la señal ORIGINAL
% Objetivo: Aplicar un filtro pasa-alto pero seguir graficando la señal
%           cruda del micrófono.
% =========================================================================

clear all;
close all;
clc;

%% 1. Parámetros y Diseño del Filtro
fs = 2000; % Frecuencia de muestreo en Hz
N = 1000;  % Número de puntos a capturar en cada bloque

fprintf('--- Visualizador y Filtro de Audio en Tiempo Real ---\n');

% Creamos el vector de tiempo para el eje X del gráfico.
t = (0:N-1)/fs;

% --- DISEÑO DEL FILTRO PASA-ALTO ---
% Usamos un objeto dsp.HighpassFilter, optimizado para bucles en tiempo real.
% Lo diseñamos UNA VEZ aquí, y luego lo reutilizamos en cada ciclo.
% Frecuencia de corte: 80 Hz (buen valor para eliminar ruido de DC y hum de 50/60Hz).
% Puedes ajustar 'PassbandFrequency' a la frecuencia que necesites.
try
    filtroPasaAlto = dsp.HighpassFilter('SampleRate', fs, ...
                                        'FilterType', 'IIR', ...
                                        'PassbandFrequency', 80, ...
                                        'StopbandFrequency', 40);
    fprintf('Filtro Pasa-Alto IIR (corte ~80 Hz) creado correctamente.\n');
catch e
    fprintf('\n========== ERROR ==========\n');
    fprintf('No se pudo crear el filtro. ¿Tienes el DSP System Toolbox instalado?\n');
    fprintf('Mensaje de error de MATLAB: %s\n', e.message);
    return;
end


%% 2. Configuración del Dispositivo de Audio
try
    audioReader = audioDeviceReader('SampleRate', fs, 'SamplesPerFrame', N);
    fprintf('Dispositivo de audio configurado correctamente.\n');
catch e
    fprintf('\n========== ERROR ==========\n');
    fprintf('No se pudo configurar el dispositivo de audio.\n');
    fprintf('Verifica los permisos de micrófono en la configuración de Windows.\n');
    fprintf('Mensaje de error de MATLAB: %s\n', e.message);
    return;
end

%% 3. Creación de la Figura
hFig = figure;
hFig.Name = 'Audio Original en Tiempo Real';
hPlot = plot(t, zeros(1, N)); % Graficamos datos vacíos para crear el objeto 'plot'

title('Señal de Audio ORIGINAL (sin filtrar)');
xlabel('Tiempo (s)');
ylabel('Amplitud');
grid on;
ylim([-1, 1]);

fprintf('\nIniciando visualización...\n');
fprintf('IMPORTANTE: Cierra la ventana de la gráfica para detener el programa.\n');
frecuencia_recibida = 0;


%% 4. Bucle Principal en Tiempo Real
while ishandle(hFig)
    %% 2. Configuración del Dispositivo de Audio
    pause(3);
    try
        audioReader = audioDeviceReader('SampleRate', 44100, 'SamplesPerFrame', N*23);
        fprintf('Dispositivo de audio configurado correctamente.\n');
    catch e
        fprintf('\n========== ERROR ==========\n');
        fprintf('No se pudo configurar el dispositivo de audio.\n');
        fprintf('Verifica los permisos de micrófono en la configuración de Windows.\n');
        fprintf('Mensaje de error de MATLAB: %s\n', e.message);
        return;
    end
    % PASO 1: Capturar audio original.
    % x contendrá la señal cruda del micrófono (1x1000).
    x_real = audioReader()';

     % PASO 2: Filtrar la señal
    x_real = filtroPasaAlto(x_real);


    x = resample(x_real, 2000, 44100);
    x = x(1:1000);
   

    %% ==========================================================
    %  AQUÍ ESTÁ LA LÍNEA PARA ELIMINAR EL RUIDO DE BAJA AMPLITUD
    %  ==========================================================
    umbral_ruido = 0.02;
    x(abs(x) < umbral_ruido) = 0;
    %  ==========================================================

    % Redondeamos la señal ya limpia para enviarla al PSoC
    x = round(x,2);
   
   
    frecuencia_prueba = 330;

    t = 0:1/fs:(N-1)/fs;

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
    
    pause(1);
    
    
   
    
    %% 6. Cierre del puerto serial
    flushinput(s);
    fclose(s);
    delete(s);
    clear s;
    disp("Puerto serial cerrado.");
    
   



    
end

%% 5. Limpieza Final
release(audioReader);       % Liberamos el dispositivo de audio.
release(filtroPasaAlto);    % Liberamos el objeto del filtro.
fprintf('\nVisualización detenida. Recursos de audio y filtro liberados.\n');
disp('Script finalizado.');