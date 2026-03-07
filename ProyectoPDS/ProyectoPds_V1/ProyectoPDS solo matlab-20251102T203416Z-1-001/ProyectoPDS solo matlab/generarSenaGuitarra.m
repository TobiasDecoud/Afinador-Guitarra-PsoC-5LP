function x = generarSenaGuitarra(f_fundamental, snr_db)
% generarSenaGuitarra: Genera una señal de guitarra de 1000 puntos para un sistema de 4 kHz,
% garantizando que no se generen frecuencias IGUALES O SUPERIORES a 1000 Hz.
%
% ENTRADAS:
%   f_fundamental - La frecuencia fundamental de la nota (ej. 110 para La grave).
%   snr_db        - (Opcional) La Relación Señal a Ruido en dB. 
%                   Por defecto es 25 dB. Usar Inf para no añadir ruido.
%
% SALIDA:
%   x             - El vector de la señal de audio generada (siempre 1000 puntos).

    %% 1. Configuración de Parámetros del Sistema
    
    if nargin < 2
        snr_db = 25;
    end

    Fs = 2000;              % Frecuencia de muestreo del sistema.
    N = 1000;               % Número de puntos del buffer.
    f_limite = 1000;        % <-- CAMBIO CRÍTICO: Límite de frecuencia estricto.
    
    t = (0:N-1) / Fs;

    %% 2. Generación de la Señal de Armónicos (Limitada en Banda a < 1 kHz)
    
    amplitudes = [1.0, 0.75, 0.5, 0.3, 0.15, 0.1, 0.08, 0.05, 0.03, 0.02, 0.01];
    harmonics  = 1:length(amplitudes);

    signal_harmonics = zeros(1, N);
    
    % Bucle para construir la señal sumando solo los armónicos permitidos
    for i = 1:length(harmonics)
        f_harmonic = f_fundamental * harmonics(i);
        
        % Solo se añade el armónico si su frecuencia es ESTRICTAMENTE MENOR que el límite.
        if f_harmonic < f_limite
            signal_harmonics = signal_harmonics + amplitudes(i) * sin(2 * pi * f_harmonic * t);
        else
            % Si un armónico ya es muy alto, detenemos el bucle.
            break; 
        end
    end
    
    % Normalizar la señal para que su amplitud esté entre -1 y 1.
    if max(abs(signal_harmonics)) > 0
        signal_harmonics = signal_harmonics / max(abs(signal_harmonics));
    end

    % Crear y aplicar la envolvente de decaimiento.
    decay_constant = 0.3;
    envelope = exp(-t / decay_constant);
    x_limpia = signal_harmonics .* envelope;

    %% 3. Adición de Ruido Opcional
    
    if isinf(snr_db)
        x = x_limpia;
        return;
    end

    potencia_senal = mean(x_limpia.^2);
    snr_lineal = 10^(snr_db / 10);
    potencia_ruido = potencia_senal / snr_lineal;
    ruido = sqrt(potencia_ruido) * randn(1, N);
    x = x_limpia + ruido;

end