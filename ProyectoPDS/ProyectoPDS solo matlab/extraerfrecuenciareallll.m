%% Método 2: Autocorrelación
clear all; clc;
[y, fs] = audioread('cuerdalucas.mp4');
if size(y,2)>1, y = mean(y,2); end

% Calcular autocorrelación
[acor, lag] = xcorr(y, y, 'coeff');

% Solo nos interesa la mitad positiva de los lags
mid_point = length(lag)/2;
acor = acor(ceil(mid_point):end);
lag = lag(ceil(mid_point):end);

% --- IMPORTANTE ---
% El máximo siempre será en lag=0 (la señal comparada consigo misma sin moverse).
% Debemos ignorar el inicio y buscar el SIGUIENTE pico más alto.

% Definir rango de búsqueda para f0 humana/instrumentos (ej. 50Hz a 1000Hz)
min_lag = round(fs / 1000); % Correspondiente a 1000 Hz
max_lag = round(fs / 50);   % Correspondiente a 50 Hz

% Recortar la autocorrelación a esa ventana
[pico_val, pico_idx] = max(acor(min_lag:max_lag));

% Ajustar índice real
lag_real = pico_idx + min_lag - 1;

% Calcular frecuencia
f0_corr = fs / lag_real;

fprintf('Frecuencia (Autocorrelación): %.2f Hz\n', f0_corr);

figure; plot(lag(1:max_lag), acor(1:max_lag)); 
title('Autocorrelación (El 2do pico indica el periodo)');