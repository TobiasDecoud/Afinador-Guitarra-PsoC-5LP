% Cargar el archivo de audio
[x, fs] = audioread('465490__timidity__savarez-esaite.flac');  % Cargar archivo de audio

% Si el archivo tiene más de un canal (estéreo), tomamos uno solo
if size(x, 2) > 1
    x = x(:, 1);  % Tomamos solo el canal izquierdo
end

% Normalizamos la señal
x = x / max(abs(x));

% Aplicamos un ventaneo de Hamming para reducir efectos de borde
x_windowed = x .* hamming(length(x));

% Realizamos la autocorrelación de la señal
[acor, lag] = xcorr(x_windowed, 'coeff');  % 'coeff' normaliza la autocorrelación

% Consideramos solo las partes positivas de lag (relevante para periodicidad)
acor_pos = acor(lag > 0);
lag_pos = lag(lag > 0);

% Buscar el primer pico significativo en la autocorrelación
% Establecemos un umbral para ignorar pequeños picos
threshold = 0.1;  % Ajusta este valor según el caso
acor_pos(acor_pos < threshold) = 0;

% Encontrar el primer pico relevante en la autocorrelación
[~, idx] = max(acor_pos);  % Encuentra el índice del pico máximo

% El periodo fundamental es el valor de 'lag' correspondiente al primer pico
periodo_fundamental = lag_pos(idx);

% La frecuencia fundamental es el inverso del periodo
frecuencia_fundamental = fs / periodo_fundamental;

% Mostrar el resultado
disp(['La frecuencia fundamental es: ', num2str(frecuencia_fundamental), ' Hz']);
