clear all
close all
clc

%% 1. Señal de prueba
% --- CAMBIO: Ajustado para N=1000 y para probar el filtro pasa-banda ---
N = 1000; % Longitud de la señal, DEBE coincidir con el PSoC
fs = 1000; % Frecuencia de muestreo (Hz)
t = 0:1/fs:(N-1)/fs; % Vector de tiempo para 1000 puntos

% Señal para probar el filtro Pasa-Banda (100 Hz - 200 Hz)
% - Un tono a 150 Hz (debería pasar)
% - Un tono a 300 Hz (debería ser eliminado)
freq_pasa = 150;
freq_no_pasa = 300;
x = 0.8*sin(2*pi*freq_pasa*t) + 0.5*sin(2*pi*freq_no_pasa*t);

x = round(x,2);


m = length(x); % m = 1000
disp(['Longitud de la señal: ', num2str(m)]);
if m ~= N
    error(['Error: La longitud (m) debe ser ' num2str(N) ' para coincidir con N del PSoC.']);
end

%% Se configura el puerto serial y se abre el canal
delete(instrfind);
SerialPort='COM4'; 
fincad = 'CR/LF';
baudios = 115200;
s = serial(SerialPort);
set(s,'BaudRate',baudios,'DataBits', 8, 'Parity', 'none','StopBits', 1,'FlowControl', 'none','Timeout',2); % Timeout de 2 segundos
set(s,'Terminator',fincad);

s.OutputBufferSize = 4096;
s.InputBufferSize = 8192; % Suficiente para recibir 1000 paquetes de 6 bytes
flushinput(s);
% Se abre el puerto de comunicación
fopen(s);

% --- CAMBIO: Se asegura de enviar los datos como 'single' (float de 4 bytes) ---
fwrite(s, single(x), 'float'); % Enviamos la senal creada al psoc
disp('Señal enviada. Esperando para procesamiento...');
pause(2); % Pausa para que el PSoC procese

%% 4. Recibir el Eco (Comando 'P')
disp('Solicitando eco de la señal (comando P)...');
datos_eco = zeros(1, m); % Variable para guardar datos recibidos
flushinput(s);
fwrite(s,'P'); % Solicitamos los datos originales (el eco)

longitud = m; % <-- CORREGIDO: El bucle debe ser hasta m, no m+1
count = 1;
k=0;

disp(['Iniciando bucle de recepción (ECO) para ' num2str(m) ' puntos...']);
while ~isequal(count,longitud + 1)
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
    
    % --- INICIO DEL ÚNICO CAMBIO IMPORTANTE ---
    % Leemos los 6 bytes que envía Chart_1_Plot para cada punto
    packet = fread(s, 6, 'uint8');
    if length(packet) == 6
        % Si recibimos los 6 bytes, extraemos los 4 del dato y los convertimos
        datos_eco(count) = typecast(uint8(packet(3:6)), 'single');
    else
        % Si no, hubo un timeout, asignamos NaN (Not a Number)
        datos_eco(count) = NaN;
    end
    % --- FIN DEL ÚNICO CAMBIO IMPORTANTE ---
    
    count = count+1;
end
disp('Recepción (ECO) finalizada.');
           
%% 5. Recibir Datos PROCESADOS (Comando 'F')
disp('Solicitando datos PROCESADOS (comando F)...');
datos_procesados = zeros(1, m); % Nueva variable
flushinput(s);
fwrite(s,'F'); % Solicitamos los datos procesados

longitud = m; % <-- CORREGIDO: El bucle debe ser hasta m, no m+1
count = 1;
k=0;

disp(['Iniciando bucle de recepción (PROCESADOS) para ' num2str(m) ' puntos...']);
while ~isequal(count,longitud + 1)
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
    
    % --- INICIO DEL ÚNICO CAMBIO IMPORTANTE ---
    % Leemos los 6 bytes que envía Chart_1_Plot para cada punto
    packet = fread(s, 6, 'uint8');
    if length(packet) == 6
        % Si recibimos los 6 bytes, extraemos los 4 del dato y los convertimos
        datos_procesados(count) = typecast(uint8(packet(3:6)), 'single');
    else
        % Si no, hubo un timeout, asignamos NaN (Not a Number)
        datos_procesados(count) = NaN;
    end
    % --- FIN DEL ÚNICO CAMBIO IMPORTANTE ---
    
    count = count+1;
end
disp('Recepción (PROCESADOS) finalizada.');


%% 6. Graficar para comparar
figure
subplot(3,1,1)
plot(t, x, 'b-')
title('Senal Original Enviada (MATLAB)');
ylabel('Amplitud');
grid on;

subplot(3,1,2)
plot(t, datos_eco, 'r-')
title('Senal Recibida (Eco del PSoC)');
ylabel('Amplitud');
grid on;

subplot(3,1,3)
plot(t, datos_procesados, 'g-')
title('Senal PROCESADA (Recibida del PSoC)');
ylabel('Amplitud');
xlabel('Tiempo (s)');
grid on;
    
%% 7. Clean up the serial port
flushinput(s);
fclose(s);
delete(s);
clear s;
disp("Puerto serial cerrado")