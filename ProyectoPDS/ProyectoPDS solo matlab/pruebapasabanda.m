%% 0. Limpieza del Entorno
clear all;
close all;
clc;

%% 1. Parámetros GLOBALES del Sistema
f_fundamental = 110.0; % Frecuencia de la cuerda A2 (La) en Hz
snr_db = 25;           % Relación señal-ruido en dB (más bajo = más ruido)

% ¡IMPORTANTE! fs debe coincidir con la frecuencia de muestreo usada
% dentro de tu función generarSenaGuitarra y en tu diseño de filtro.
fs = 1000; % Frecuencia de muestreo (Hz)

fprintf('Sistema configurado para fs = %d Hz.\n', fs);

%% 2. Generación de la Señal de Guitarra Realista
fprintf('Generando señal de guitarra para la nota A2 a %.2f Hz con SNR de %d dB...\n', f_fundamental, snr_db);

% Llamada a TU nueva función para generar la señal
x = generarSenaGuitarra(f_fundamental, snr_db);

% La función ya crea una señal de 1 segundo. Generamos el vector de tiempo
% correspondiente para poder graficarla.
N = length(x);
t = (0:N-1) / fs;

%% 3. Diseño del Filtro Pasa-Banda FIR (para fs = 1000 Hz)
% El objetivo sigue siendo aislar la fundamental de 110 Hz.
% Los parámetros del filtro se ajustan a la nueva fs de 1000 Hz.

fprintf('Diseñando el filtro FIR Pasa-Banda para fs = 1000 Hz...\n');

Fpass1 = 95;    % Inicio de la banda de paso (Hz)
Fpass2 = 125;   % Fin de la banda de paso (Hz)
orden_filtro = 60; % Aumentamos un poco el orden para que sea más selectivo

% Usamos 'designfilt' para crear el objeto del filtro
filtro_bp = designfilt('bandpassfir', 'FilterOrder', orden_filtro, ...
    'CutoffFrequency1', Fpass1, 'CutoffFrequency2', Fpass2, ...
    'SampleRate', fs);

% --- EXTRACCIÓN DE COEFICIENTES PARA EL PSOC ---
% NOTA: Estos coeficientes son para un sistema que muestrea a 1000 Hz.
% Si tu PSoC muestrea a 2048 Hz, debes rediseñar el filtro con fs=2048.
coeficientes = filtro_bp.Coefficients;

% Mostramos los coeficientes en la consola
fprintf('\n--- Coeficientes del Filtro para un PSoC con fs = %d Hz ---\n', fs);
fprintf('float num[] = {');
fprintf('%.6f, ', coeficientes(1:end-1));
fprintf('%.6f};\n', coeficientes(end));
fprintf('-------------------------------------------------------------\n\n');


%% 4. Aplicación del Filtro a la Señal
fprintf('Filtrando la señal de guitarra...\n');
y = filter(filtro_bp, x);

%% 5. Visualización de Resultados

% --- GRÁFICO 1: RESPUESTA EN FRECUENCIA DEL FILTRO ---
fprintf('Mostrando la respuesta en frecuencia del filtro...\n');
figure('Name', 'Análisis del Filtro Diseñado para fs=1000Hz');
freqz(filtro_bp, 512, fs); % Visualizamos la respuesta del filtro

% --- GRÁFICO 2: SEÑAL ORIGINAL vs. SEÑAL FILTRADA ---
fprintf('Mostrando la comparación de las señales en el tiempo...\n');
figure('Name', 'Efecto del Filtro en la Señal de Guitarra Realista');

% Subplot para la señal original
subplot(2, 1, 1);
plot(t, x, 'b');
title('Señal Original (Generada por tu función)');
xlabel('Tiempo (s)');
ylabel('Amplitud');
grid on;
xlim([0, 0.1]); % Zoom en los primeros 100ms

% Subplot para la señal filtrada
subplot(2, 1, 2);
plot(t, y, 'r');
title('Señal Filtrada (Fundamental Aislada)');
xlabel('Tiempo (s)');
ylabel('Amplitud');
grid on;
xlim([0, 0.1]); % Mismo zoom para comparar