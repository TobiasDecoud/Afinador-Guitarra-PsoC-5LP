%% Limpieza
clear all
close all
clc


%% Parámetros de la Señal
amplitud = 1;      % Amplitud de la onda senoidal
frecuencia = 82.42;    % Frecuencia de la señal en Hertz
fase = pi/2;
fs = 2000;         % Frecuencia de muestreo en Hertz
N = 1000;           % Número de puntos

%% Crear un vector de tiempo
t = 0:1/fs:(N-1)/fs; 


%%  Generación de la Señal
y = amplitud*sin(2*pi*frecuencia*t + fase);


%% Detección de Cruces por Cero
cruces_por_cero_indices = find(sign(y(1:end-1)) .* sign(y(2:end)) < 0) + 1;

senal_cruces = zeros(size(y));
% En los índices donde ocurre un cruce, establecer el valor a 1.
senal_cruces(cruces_por_cero_indices) = 1;

%% Gráfica de Ambas Señales
figure();

% Graficar la señal senoidal original
plot(t, y, 'b-', 'LineWidth', 1.5);

hold on;

% Graficar la señal de cruces
stem(t, senal_cruces, 'r', 'Marker', 'none', 'LineWidth', 1.5);

titulo_str = sprintf('Señal Senoidal y Cruces por Cero (N = %d puntos, fs = %d Hz)', N, fs);
title(titulo_str);
xlabel('Tiempo (s)');
ylabel('Amplitud');
grid on;
legend('Señal Senoidal', 'Cruce por Cero (1)');
ylim([-1.2, 1.2]); 
hold off; 

indices_picos = find(senal_cruces);

% Obtener los valores de tiempo correspondientes a esos picos
tiempos_picos = t(indices_picos);

% Calcular la diferencia de tiempo entre cada pico consecutivo
% Esto nos da un vector con la duración de cada "banda" o intervalo
intervalos_tiempo = diff(tiempos_picos);

% Calcular el promedio de estos intervalos
promedio_intervalo = mean(intervalos_tiempo)*2;


%% Mostrar los resultados en la ventana de comandos
fprintf('\n--- Análisis de Cruces por Cero ---\n');
fprintf('Número de cruces por cero detectados: %d\n', length(indices_picos));
fprintf('El tiempo promedio entre cruces es: %.4f segundos\n', promedio_intervalo);
fprintf('La frecuencia estimada es: %.4f Hz\n', 1/promedio_intervalo);

