clear all
close all
clc

%% 1. Señal de prueba
N = 1000;
fs = 2000;
t = 0:1/fs:(N-1)/fs;

% Genera una señal de prueba, por ejemplo 110 Hz (nota LA)
frecuencia_prueba = 82.4;
x = generarSenaGuitarra(frecuencia_prueba);

%x = 0.5*sin(2*pi*frecuencia_prueba*t);


x = round(x,2);
m = length(t);
disp(['Longitud de la señal: ', num2str(m)]);
if m ~= N
    error('Error: La longitud de la señal debe ser igual a N.');
end

%% 2. Configuración y apertura del puerto serial
delete(instrfind);
SerialPort='COM4'; % Asegúrate de que este es tu puerto COM
fincad = 'CR/LF';
baudios = 115200;
s = serial(SerialPort);
set(s,'BaudRate',baudios,'DataBits', 8, 'Parity', 'none','StopBits', 1,'FlowControl', 'none','Timeout',1);
set(s,'Terminator',fincad);

s.OutputBufferSize = 10000;
s.InputBufferSize = 2048;
flushinput(s);
s.BytesAvailableFcnCount = m;
s.BytesAvailableFcnMode = 'byte';

fopen(s);
disp(['Puerto ' SerialPort ' abierto.']);

%% 3. Envío de la señal al PSoC
disp(['Enviando señal de prueba de ' num2str(frecuencia_prueba) ' Hz...']);
fwrite(s, x, 'float');
disp('Esperando a que el PSoC procese los datos...');
pause(1);


%% 6. Cierre del puerto serial
flushinput(s);
fclose(s);
delete(s);
clear s;
disp("Puerto serial cerrado.");

