% 1. Especificaciones del Filtro
Fs      = 44100;    % Frecuencia de muestreo en Hz
f_edges = [2000 4000 6000 8000]; % Frecuencias de flanco [Fstop1 Fpass1 Fpass2 Fstop2]
mags    = [0 1 0];  % Magnitudes deseadas para las bandas [stop, pass, stop]
Rp_dB   = 0.15;     % Rizado en banda de paso en dB
Rs_dB   = 55;       % Atenuación en banda de parada en dB

% 2. Conversión de dB a escala lineal para las desviaciones
delta_p = (10^(Rp_dB/20) - 1);
delta_s = 10^(-Rs_dB/20);
devs    = [delta_s, delta_p, delta_s];

% 3. Estimar el orden del filtro y los parámetros para firpm
[n, fo, ao, w] = firpmord(f_edges, mags, devs, Fs);

% 4. Diseñar el filtro FIR con el algoritmo Parks-McClellan
b = firpm(n, fo, ao, w);

% 5. Mostrar los coeficientes en la consola
fprintf('Filtro diseñado con orden N = %d\n', n);
fprintf('Se generaron %d coeficientes guardados en la variable "b".\n\n', length(b));
fprintf('--- INICIO DE COEFICIENTES ---\n');
fprintf('%.15f,\n', b);
fprintf('--- FIN DE COEFICIENTES ---\n');

% (Opcional) Para analizar visualmente la respuesta del filtro, descomenta la siguiente línea:
fvtool(b, 1, 'Fs', Fs)