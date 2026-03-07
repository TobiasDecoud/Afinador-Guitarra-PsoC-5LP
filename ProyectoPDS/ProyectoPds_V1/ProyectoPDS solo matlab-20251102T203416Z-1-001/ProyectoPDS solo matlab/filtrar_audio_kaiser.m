clear all
close all
clc

%% --- Filtro ---
Fs    = 2000;
Fc    = 82.41;
Bw    = 50;
fnyq  = Fs/2;
fc1   = (Fc - Bw/2) / fnyq;
fc2   = (Fc + Bw/2) / fnyq;
orden = 80;
beta  = 7;
b = fir1(orden, [fc1 fc2], 'bandpass', kaiser(orden+1, beta));

%% --- Cargar audio ---
archivo_flac = '465490__timidity__savarez-esaite.flac';
[audio_orig, Fs_orig] = audioread(archivo_flac);

% Si es estéreo, tomar solo un canal
if size(audio_orig, 2) > 1
    audio_orig = audio_orig(:, 1);
end

%% --- Resamplear a Fs=2000 ---
audio_rs = resample(audio_orig, Fs, Fs_orig);

%% --- Aplicar el filtro ---
audio_filtrado = filter(b, 1, audio_rs);

%% --- Visualizar ---
t = (0:length(audio_rs)-1) / Fs;

figure;
subplot(2,1,1);
plot(t, audio_rs);
title('Audio original (resampleado a 2000 Hz)');
xlabel('Tiempo (s)'); ylabel('Amplitud');

subplot(2,1,2);
plot(t, audio_filtrado);
title(sprintf('Audio filtrado (Fc=%.1f Hz, BW=%d Hz)', Fc, Bw));
xlabel('Tiempo (s)'); ylabel('Amplitud');

%% --- Espectro de frecuencias ---
N = length(audio_rs);
f = (0:N-1) * Fs / N;
mitad = 1:floor(N/2);

figure;
Y_orig     = fft(audio_rs);
Y_filtrado = fft(audio_filtrado);

plot(f(mitad), 20*log10(abs(Y_orig(mitad))), 'b'); hold on;
plot(f(mitad), 20*log10(abs(Y_filtrado(mitad))), 'r');
title('Espectro de frecuencias');
xlabel('Frecuencia (Hz)'); ylabel('Magnitud (dB)');
legend('Original', 'Filtrado');
xlim([0 fnyq]);
grid on;

%% --- Escuchar ---
disp('Reproduciendo audio ORIGINAL...');
sound(audio_rs, Fs);
pause(length(audio_rs)/Fs + 1);

disp('Reproduciendo audio FILTRADO...');
sound(audio_filtrado, Fs);