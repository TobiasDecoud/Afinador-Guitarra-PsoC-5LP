clear all
close all
clc

Fs    = 2000;       % frecuencia de muestreo (cámbialo según tu caso)
Fc    = 82.41;       % frecuencia central
Bw    = 50;         % ancho de banda deseado

fnyq  = Fs/2;
fc1   = (Fc - Bw/2) / fnyq;
fc2   = (Fc + Bw/2) / fnyq;

orden = 80;
beta  = 7;

b = fir1(orden, [fc1 fc2], 'bandpass', kaiser(orden+1, beta));

% Visualizar
figure('Color','w');
freqz(b, 1, 8192, Fs);
xline([Fc-Bw/2 Fc+Bw/2], 'r--', 'Banda de paso');
title(sprintf('Pasabanda Kaiser - orden %d, β=%.1f, Fc=%.1f Hz', orden, beta, Fc));
grid on; ylim([-100 5]);


fprintf('--- INICIO DE COEFICIENTES ---\n');
fprintf('%.15f\n', b);
fprintf('--- FIN DE COEFICIENTES ---\n');