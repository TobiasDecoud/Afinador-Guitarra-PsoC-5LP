%% 0. Limpieza del Entorno
clear all;
close all;
clc;

%% 1. Parámetros Fundamentales
% Nombre del archivo de audio. Asegúrate de que esté en la misma carpeta.
nombre_archivo = '465490__timidity__savarez-esaite.flac'; 

% Frecuencia de muestreo objetivo a la que queremos la señal final.
fs_objetivo = 2000; % 1000 Hz

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
fprintf('Señal procesada y lista para el análisis.\n\n');


%% 4. Cálculo de la Transformada Rápida de Fourier (FFT)
fprintf('--- Calculando la FFT de la señal procesada ---\n');

% Longitud de la señal procesada
L = length(y_procesada);

% Calcular la FFT
Y = fft(y_procesada);

% Calcular el espectro de doble lado P2
P2 = abs(Y/L);

% Calcular el espectro de un solo lado P1.
% Solo nos interesa hasta la frecuencia de Nyquist (fs_objetivo / 2)
P1 = P2(1:floor(L/2)+1);
P1(2:end-1) = 2*P1(2:end-1);

% Crear el vector de frecuencias correspondiente al espectro P1
f = fs_objetivo * (0:floor(L/2)) / L;

fprintf('Cálculo de la FFT finalizado.\n');

%% 5. Visualización del Espectro de Frecuencia (FFT)
% Graficamos el resultado para ver el contenido de frecuencia de la señal.
figure('Name', 'Análisis de Frecuencia (FFT)');
plot(f, P1);
title('Espectro de Frecuencia de la Señal de Audio Procesada');
xlabel('Frecuencia (Hz)');
ylabel('Amplitud');
grid on;

% Opcional: Limitar el eje X para ver mejor la zona de interés
% xlim([0 200]); % Por ejemplo, para ver en detalle de 0 a 200 Hz