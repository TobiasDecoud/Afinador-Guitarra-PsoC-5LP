clear all
close all
clc

%% 1. Señal de prueba
N = 1000; % Longitud de la señal
fs = 2000; % Frecuencia de muestreo (Hz)
t = 0:1/fs:(N-1)/fs; % Vector de tiempo para 1000 puntos



freq_pasa = 82.5;
freq_no_pasa = 160;
x_original = 0.5*sin(2*pi*freq_pasa*t) + 0.7*sin(2*pi*freq_no_pasa*t);
x = x_original / 1.5; 



%%
Fs    = 2000;  
archivo_flac = '465490__timidity__savarez-esaite.flac';
[audio_orig, Fs_orig] = audioread(archivo_flac);

% Si es estéreo, tomar solo un canal
if size(audio_orig, 2) > 1
    audio_orig = audio_orig(:, 1);
end

%% --- Resamplear a Fs=2000 ---
audio_rs = resample(audio_orig, Fs, Fs_orig);
x = audio_rs(1000:1999)

%%
%x = 0.8*sin(2*pi*freq_pasa*t);

%x = 1+ 0.9*sin(2*pi*freq_pasa*t) ;
%x = zeros(1,N) ;
%x = generarSenaGuitarra(82.4);
x = round(x,2);

m = length(t); % m = 100
disp(['Longitud de la señal: ', num2str(m)]);
if m ~= N
    error('Error: La longitud (m) debe ser 100 para coincidir con N_RECEPCION del PSoC.');
end






%% Se configura el puerto serial y se abre el canal
delete(instrfind);
SerialPort='COM4'; %serial port
fincad = 'CR/LF';
baudios = 115200;
s = serial(SerialPort);
set(s,'BaudRate',baudios,'DataBits', 8, 'Parity', 'none','StopBits', 1,'FlowControl', 'none','Timeout',1);
set(s,'Terminator',fincad);


s.OutputBufferSize = 10000;
s.InputBufferSize = 2048;  % Tamaño del buffer en bytes
flushinput(s);
s.BytesAvailableFcnCount = m;
s.BytesAvailableFcnMode = 'byte';
%Se abre el puerto de comunicación
fopen(s);

fwrite(s, x, 'float');% Enviamos la senal creada al psoc
pause(5) %esperamos 5 segundos para darle tiempo al psoc de procesar todos los datos

            
%% 4. Recibir el Eco (Comando 'P')
disp('Solicitando eco de la señal (comando P)...');
datos_eco = 0; % Variable para guardar datos recibidos
flushinput(s);
fwrite(s,'P'); % Solicitamos los datos originales (el eco)

longitud = m+1;
count = 1;
k=0;

disp(['Iniciando bucle de recepción (ECO) para ' num2str(m) ' puntos...']);
while ~isequal(count,longitud)
 %%Re creating Serial port before timeout
    k=k+1;  
    if k==longitud
        disp('K-Timeout alcanzado (ECO). Reiniciando puerto...');
        fclose(s);
        delete(s);
        clear s;        
        s = serial(SerialPort);
        set(s,'Terminator',fincad);
        set(s,'BaudRate',baudios,'Parity','none','Timeout',1);
        fopen(s);
        k=0;
    end
    datos_eco(count) = str2double(fscanf(s));
    count = count+1;
end
disp('Recepción (ECO) finalizada.');


pause(2);
%% 5. Recibir Datos PROCESADOS (Comando 'F')
% AHORA PEDIMOS LOS DATOS PROCESADOS (y_procesado)
disp('Solicitando datos PROCESADOS (comando F)...');
datos_procesados = 0; % Nueva variable
flushinput(s);
fwrite(s,'F'); % Solicitamos los datos procesados

longitud = m+1;
count = 1;
k=0;

disp(['Iniciando bucle de recepción (PROCESADOS) para ' num2str(m) ' puntos...']);
while ~isequal(count,longitud)
 %%Re creating Serial port before timeout
    k=k+1;  
    if k==longitud
        disp('K-Timeout alcanzado (PROCESADOS). Reiniciando puerto...');
        fclose(s);
        delete(s);
        clear s;        
        s = serial(SerialPort);
        set(s,'Terminator',fincad);
        set(s,'BaudRate',baudios,'Parity','none','Timeout',1);
        fopen(s);
        k=0;
    end
    datos_procesados(count) = str2double(fscanf(s));
    count = count+1;
end
disp('Recepción (PROCESADOS) finalizada.');


%% 6. Graficar para comparar
figure
subplot(3,1,1)
plot(t, x)
title('Senal Original Enviada (MATLAB)');
ylabel('Amplitud');
grid on;

subplot(3,1,2)
plot(t, datos_eco) % Graficamos el eco
title('Senal Recibida (Eco del PSoC)');
ylabel('Amplitud');
grid on;




subplot(3,1,3)
plot(t, datos_procesados) % Graficamos los datos procesados
title('Senal PROCESADA (Recibida del PSoC)');
ylabel('Amplitud');
xlabel('Muestras');
grid on;
    
%% 7. Clean up the serial port
flushinput(s);
fclose(s);
delete(s);
clear s;
disp("Puerto serial cerrado")


%%
% 1. Configurar la Frecuencia de Muestreo (Fs)
Fs = fs; % Asegurate de que este valor sea correcto.

% 2. Centrar la señal en cero (Restar la media o offset de DC)
% Si la señal no esta centrada, los cruces por cero seran incorrectos.
datos_centrados = datos_procesados - mean(datos_procesados);

% 3. Calcular la Tasa de Cruce por Cero (ZCR)
ZCR = zerocrossrate(datos_centrados);

% 4. Estimar la Frecuencia Fundamental (F0)
% La frecuencia fundamental (F0) se estima como: 
% F0 = (ZCR * Fs) / 2.
% Se divide por 2 porque hay dos cruces por cero por ciclo de onda.
F_estimada = (ZCR * Fs) / 2;

% 5. Mostrar el resultado
fprintf('La frecuencia estimada F0 es: %.2f Hz\n', F_estimada);


%% ANALISIS ESPECTRAL (FFT)
% Fs es la Frecuencia de Muestreo (1000 Hz)
Fs = fs; 
N_muestras = length(datos_procesados);
L = N_muestras;

% 1. Calcular la Transformada de Fourier
Y = fft(datos_procesados);

% 2. Calcular el espectro de doble lado y pasarlo a un solo lado (hasta Nyquist)
P2 = abs(Y/L);
P1 = P2(1:floor(L/2)+1);
P1(2:end-1) = 2*P1(2:end-1);

% 3. Crear el vector de frecuencias
f = Fs*(0:floor(L/2))/L;

% 4. Graficar el espectro
figure
plot(f, P1) 
title('Espectro de Frecuencia de la Senal PROCESADA')
xlabel('Frecuencia (Hz)')
ylabel('|P1(f)|')
grid on

% 5. Encontrar la Frecuencia Pico
[Amplitud_Pico, Indice_Pico] = max(P1);
Frecuencia_Pico_FFT = f(Indice_Pico);

fprintf('La Frecuencia Pico (FFT) es: %.2f Hz\n', Frecuencia_Pico_FFT);