function [magnitud_cuadrada] = goertzel_mag(y, targetFreq, Fs)
%
% Esta función implementa el algoritmo de Goertzel.
% Es la traducción directa de la función en C.
%
% y:           vector de señal de entrada
% targetFreq:  la frecuencia que buscamos (ej. 110.0)
% Fs:          frecuencia de muestreo (ej. 5000)
%

% N se toma de la longitud de la señal de entrada
N = length(y);

%% Paso 1: Pre-calcular las Constantes
% (Variables con los mismos nombres que en el código C)
k = N * (targetFreq / Fs);
omega = (2 * pi * k) / N;
coeff = 2 * cos(omega);

%% Paso 2: El Bucle Recursivo
% (Variables con los mismos nombres que en el código C)
q0 = 0;
q1 = 0;
q2 = 0;

% MATLAB usa índices desde 1 (en C usamos desde 0)
for n = 1:N
    q0 = y(n) + coeff * q1 - q2;
    q2 = q1;
    q1 = q0;
end

%% Paso 3: Calcular el Resultado
magnitud_cuadrada = (q1^2) + (q2^2) - (q1 * q2 * coeff);

end