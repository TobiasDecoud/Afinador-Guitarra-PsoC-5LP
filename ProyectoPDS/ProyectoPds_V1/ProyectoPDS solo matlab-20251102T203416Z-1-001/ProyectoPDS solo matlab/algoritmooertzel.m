clear all;
close all;
clc;

%% Algoritmo de Goertzel

%% 1. Parámetros (Iguales que en el PSoC)
Fs = 5000;  % Frecuencia de muestreo (5 kHz)
N = 100;    % Número de muestras
t = (0:N-1) / Fs; % Vector de tiempo (100 puntos)

%% 2. Señal de Prueba
% Vamos a simular que tocamos la cuerda 'La' (A2)
f_prueba = 90.0; % 110 Hz

% (Opcional) Prueba cambiar f_prueba a 146.83 (Re) y corre de nuevo
% f_prueba = 146.83; 

% Generamos la señal y añadimos un poco de ruido
y = cos(2*pi*f_prueba*t) 

% Graficamos la señal de entrada
figure(1);
plot(t, y, '.-');
title(['Señal de prueba: ' num2str(f_prueba) ' Hz (con ruido)']);
xlabel('Tiempo (s)');
ylabel('Amplitud');
grid on;

%% 3. Frecuencias Objetivo (Las 6 cuerdas de la guitarra)
% Nombres de las notas
labels = {'Mi (E2)', 'La (A2)', 'Re (D3)', 'Sol (G3)', 'Si (B3)', 'Mi (E4)'};
% Frecuencias
freq_targets = [82.41, 110.0, 146.83, 196.00, 246.94, 329.63];

%% 4. ¡Correr el Algoritmo!
% Llamamos a nuestra función Goertzel 6 veces, una para cada cuerda
magnitudes = zeros(1, length(freq_targets));

for i = 1:length(freq_targets)
    % Pasamos la señal, la frecuencia objetivo y el Fs
    magnitudes(i) = goertzel_mag(y, freq_targets(i), Fs);
end

%% 5. Mostrar Resultados
disp('Energía detectada en cada frecuencia:');
for i = 1:length(labels)
    % Imprimimos la magnitud (energía)
    fprintf('%s (%.2f Hz): \t%.4f\n', labels{i}, freq_targets(i), magnitudes(i));
end

% Graficamos los resultados en un gráfico de barras
figure(2);
bar(magnitudes);
set(gca, 'xticklabel', labels);
title('Energía Detectada por Goertzel (Afinador)');
ylabel('Magnitud al Cuadrado (Energía)');