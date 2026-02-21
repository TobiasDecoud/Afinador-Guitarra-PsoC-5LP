clear all;
close all;
clc;

%% Configuración
Fs = 44100;        % Frecuencia estándar (compatible con todas las Notebooks)
duracion = 2;      % Duración de la grabación en segundos
nBits = 16;        % Resolución
canales = 1;       % 1 = Mono, 2 = Stereo

%% 1. Crear el objeto de grabación
% Si esto falla, es problema de drivers o permisos de Windows
try
    recObj = audiorecorder(Fs, nBits, canales);
    disp('Objeto de grabación creado correctamente.');
catch
    error('No se pudo acceder al micrófono. Revisa Privacidad en Windows.');
end

%% 2. Grabar
fprintf('\n--- GRABANDO POR %d SEGUNDOS ---\n', duracion);
fprintf('¡Habla o toca la guitarra ahora!\n');

% recordblocking detiene el código hasta que termina de grabar
recordblocking(recObj, duracion); 

disp('--- Fin de la grabación ---');

%% 3. Obtener los datos y Graficar
y = getaudiodata(recObj);

% Crear vector de tiempo para el eje X
t = (0:length(y)-1) / Fs;

% Graficar
figure;
plot(t, y);
title('Prueba de Micrófono');
xlabel('Tiempo (segundos)');
ylabel('Amplitud');
grid on;
ylim([-1 1]);

% Reproducir lo grabado para confirmar que se escucha bien
disp('Reproduciendo lo grabado...');
play(recObj);