%% 0. Limpieza del Entorno
clear all;
close all;
clc;

%% 1. Parámetros GLOBALES del Sistema
f_objetivo = 82.41; % Frecuencia de la cuerda E2 en Hz
fs = 1000; % Frecuencia de muestreo (Hz)
tolerancia_hz = 1.0; % Margen para considerar la cuerda "afinada" (+/- 1 Hz)
N = 1500;   % Número de muestras a procesar.




%% 2. Carga y Preparación de la Señal REAL
% Nombre del archivo de audio.
% Asegúrate de que este archivo esté en la misma carpeta que el script
% o proporciona la ruta completa.
archivo_flac =  '1ra_MI_330.ogg';


% Leer el archivo de audio para obtener la señal y su frecuencia de muestreo original
[y_original, fs_original] = audioread(archivo_flac);
fprintf('El archivo original tiene una fs de %d Hz.\n', fs_original);

%% 3. Remuestreo de la Señal (si es necesario)
% Comprobar si la frecuencia de muestreo original es diferente a la deseada
if fs_original ~= fs
    fprintf('Remuestreando de %d Hz a %d Hz...\n', fs_original, fs);
    % Remuestrear la señal a la nueva frecuencia 'fs'
    y_procesada = resample(y_original, fs, fs_original);
else
    fprintf('La frecuencia de muestreo deseada (%d Hz) coincide con la original. No se requiere remuestreo.\n', fs);
    % Si las frecuencias son iguales, se utiliza la señal original directamente
    y_procesada = y_original;
end

%% 4. Conversión a Monoaural
% Comprobar si la señal tiene más de un canal (es estéreo)
if size(y_procesada, 2) > 1
    % Convertir a mono promediando los canales
    y_procesada = mean(y_procesada, 2);
end

%% 5. Selección de Muestras y Creación de Vectores Finales
% Asegurarse de que la señal procesada tiene suficientes muestras
if length(y_procesada) < N
    error('La señal procesada es más corta que el N especificado de %d muestras.', N);
end

% Seleccionar las primeras N muestras de la señal procesada
% y asegurarse de que sea un vector fila con el operador (').
x = y_procesada(1000:N+1000)'; 

% Crear el vector de tiempo 't' correspondiente a las N muestras
t = (0:N-1) / fs;

%% 3. Diseño del Filtro Pasa-Banda para la Cuerda Mi (82.4 Hz)
fprintf('Diseñando el filtro FIR Pasa-Banda para %.2f Hz...\n', f_objetivo);
Fpass1 = 310;
Fpass2 = 350;
orden_filtro = 80;

filtro_bp_E2 = designfilt('bandpassfir', 'FilterOrder', orden_filtro, ...
    'CutoffFrequency1', Fpass1, 'CutoffFrequency2', Fpass2, ...
    'SampleRate', fs);
coeficientes_E2 = filtro_bp_E2.Coefficients;
fprintf('\n--- Coeficientes del Filtro para la CUERDA MI (E2) ---\n');
fprintf('float num[] = {');
fprintf('%.6f, ', coeficientes_E2(1:end-1));
fprintf('%.6f};\n', coeficientes_E2(end));

retardo_muestras = orden_filtro / 2;
fprintf('El filtro tiene un retardo de grupo de %d muestras (%.1f ms).\n\n', retardo_muestras, (retardo_muestras/fs) * 1000);

%% 4. Aplicación del Filtro
y_filtrada = filter(filtro_bp_E2, x);

%% 5. Compensación del Retardo (Para análisis)
x_alineada = x(retardo_muestras + 1 : end);
y_alineada = y_filtrada(retardo_muestras + 1 : end);
t_alineado = (0 : length(x_alineada) - 1) / fs;

% =========================================================================
%% 6. Simulación del Hardware PSoC (Cruce por Cero y Medición de Frecuencia)
% =========================================================================
fprintf('Simulando el hardware del PSoC...\n');

% --- Simulación del Comparador Analógico ---
% Se convierte la señal sinusoidal en una onda cuadrada.
% La salida es 1 si la señal > 0, y 0 si es < 0.
onda_cuadrada = (y_alineada > 0);

% --- Simulación del Timer en Modo Captura ---
% Buscamos los puntos exactos donde la señal pasa de 0 a 1 (flanco de subida).
% 'diff' calcula la diferencia entre puntos adyacentes. Un flanco de subida es +1.
indices_cruces = find(diff(onda_cuadrada) == 1);

frecuencia_medida = 0; % Valor por defecto
resultado_afinacion = "Señal Débil o Nula"; % Mensaje por defecto

% Necesitamos al menos dos cruces para medir un período
if length(indices_cruces) > 1
    % Medimos la distancia (en muestras) entre cada cruce consecutivo
    periodos_en_muestras = diff(indices_cruces);
    
    % Calculamos el período promedio para mayor estabilidad
    periodo_promedio_muestras = mean(periodos_en_muestras);
    
    % Convertimos el período de muestras a segundos
    periodo_promedio_segundos = periodo_promedio_muestras / fs;
    
    % Calculamos la frecuencia medida
    frecuencia_medida = 1 / periodo_promedio_segundos;
    
    % --- Simulación de la Lógica de la CPU ---
    % Comparamos la frecuencia medida con el objetivo
    if frecuencia_medida < (f_objetivo - tolerancia_hz)
        resultado_afinacion = "Subir Tono (Muy Bajo)";
    elseif frecuencia_medida > (f_objetivo + tolerancia_hz)
        resultado_afinacion = "Bajar Tono (Muy Alto)";
    else
        resultado_afinacion = "¡Afinado!";
    end
end

fprintf('Frecuencia Medida: %.2f Hz\n', frecuencia_medida);
fprintf('Veredicto: %s\n', resultado_afinacion);

% =========================================================================
%% 7. Visualización Final
% =========================================================================
figure('Name', 'Simulación Completa del Afinador');

% Creamos un título general para toda la figura con el resultado final
fig_title = sprintf('Análisis de Afinación: Frec. Medida = %.2f Hz  -->  %s', frecuencia_medida, resultado_afinacion);
sgtitle(fig_title, 'FontSize', 14, 'FontWeight', 'bold');

% Gráfico 1: Señal Original
subplot(3, 1, 1);
plot(t_alineado, x_alineada, 'b');
title('1. Señal Real de Guitarra (Original)');
xlabel('Tiempo (s)'); ylabel('Amplitud'); grid on; xlim([0, 0.2]);

% Gráfico 2: Señal Filtrada
subplot(3, 1, 2);
plot(t_alineado, y_alineada, 'r');
title('2. Señal Procesada por el Filtro (DFB)');
xlabel('Tiempo (s)'); ylabel('Amplitud'); grid on; xlim([0, 0.2]);

% Gráfico 3: Salida del Comparador y Medición
subplot(3, 1, 3);
plot(t_alineado, onda_cuadrada, 'k', 'LineWidth', 1.5);
hold on;
% Marcamos con círculos rojos los puntos exactos que el 'Timer' capturaría
plot(t_alineado(indices_cruces), 1, 'ro', 'MarkerFaceColor', 'r', 'MarkerSize', 8);
hold off;
title('3. Salida del Comparador y Puntos de Medición del Timer');
xlabel('Tiempo (s)'); ylabel('Digital'); grid on; xlim([0, 0.2]);
ylim([-0.1 1.1]); % Ajustamos el eje Y para que parezca una señal digital
legend('Salida Comparador', 'Capturas del Timer');