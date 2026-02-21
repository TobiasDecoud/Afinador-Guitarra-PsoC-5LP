#include "project.h"
#include "configuracion.h"
#include "coeficientes.h"
#include "lcd_i2c.h"         
#include "procesamiento.h"
#include "gestion_hardware.h"
#include <stdio.h> 
#include <stdlib.h>

// Solución para printf de floats en GCC
#if defined (__GNUC__)
    asm(".global _printf_float");
#endif

// --- Buffers de Procesamiento ---
// Nota: 'N' debe estar definido en configuracion.h (ej. 1024 o 2048)
float y_entrada[N] = {0};             

// --- Variables Lógica Afinador ---
int cuerda_actual = 0; // 0 = Ninguna cuerda seleccionada (Idle)
float freq_objetivo = 0.0f;
float freq_calculada = 0.0f;
char buffer_LCD[20];


// Función auxiliar para actualizar pantalla
void Actualizar_LCD_Resultado(float freq_medida, float freq_target, int num_cuerda);

int main(void)
{
    // 1. Inicialización de Periféricos
    Hardware_Iniciar(); // Inicia Filtro, DMA e Interrupciones HW
    
    // --- Eliminado UART Start ---
    
    I2C_1_Start();
    LCD_Init();
    
    // Cadena de acondicionamiento analógico
    OpAmp_Vref_Start();
    PGA_1_Start();
    PGA_1_SetGain(PGA_1_GAIN_24); // Ajustar ganancia según el nivel de tu guitarra
    OpAmp_Filtro1_Start();
    OpAmp_Filtro2_Start();
    OpAmp_Acoplador_Start();
    
    ADC_DelSig_1_Start();
    ADC_DelSig_1_StartConvert(); // El ADC comienza a convertir continuamente
    
    CyGlobalIntEnable;

    // Mensaje Bienvenida
    LCD_SetCursor(0, 0); 
    LCD_Print("Afinador");
    LCD_SetCursor(1, 0); 
    LCD_Print("Listo...");
    CyDelay(1000);
    LCD_Clear();

    // Variables botones
    uint8_t sw_estado_ant = 255;
    uint8_t sw_lectura;
    
    int i; // Índice para bucles

    for(;;)
    {
        /* -----------------------------------------------------------
           1. Lectura de Botones (Selección de Cuerda)
           ----------------------------------------------------------- */
        sw_lectura = (BIT1_Read() << 2) | (BIT2_Read() << 1) | BIT3_Read(); 

        // Si hay cambio en los botones
        if (sw_lectura != sw_estado_ant)
        {
            CyDelay(20); // Debounce simple
            
            // Selección de Cuerda y Carga de Coeficientes del Filtro
            switch (sw_lectura)
            {
                case 0b001: // Cuerda 1 (Mi grave / E2)
                    Hardware_Cargar_Coeficientes(COEFFS_82_4, sizeof(COEFFS_82_4));
                    cuerda_actual = 1; freq_objetivo = 82.4f;
                    break;
                case 0b010: // Cuerda 2 (La / A2)
                    Hardware_Cargar_Coeficientes(COEFFS_110, sizeof(COEFFS_110));
                    cuerda_actual = 2; freq_objetivo = 110.0f;
                    break;
                case 0b011: // Cuerda 3 (Re / D3)
                    Hardware_Cargar_Coeficientes(COEFFS_146_83, sizeof(COEFFS_146_83));
                    cuerda_actual = 3; freq_objetivo = 146.8f;
                    break;
                case 0b100: // Cuerda 4 (Sol / G3)
                    Hardware_Cargar_Coeficientes(COEFFS_196, sizeof(COEFFS_196));
                    cuerda_actual = 4; freq_objetivo = 196.0f;
                    break;
                case 0b101: // Cuerda 5 (Si / B3)
                    Hardware_Cargar_Coeficientes(COEFFS_246_94, sizeof(COEFFS_246_94));
                    cuerda_actual = 5; freq_objetivo = 246.9f;
                    break;
                case 0b110: // Cuerda 6 (Mi agudo / E4)
                    Hardware_Cargar_Coeficientes(COEFFS_330, sizeof(COEFFS_330));
                    cuerda_actual = 6; freq_objetivo = 329.6f;
                    break;
                default: 
                    // Si se sueltan los botones o combinación inválida, mantenemos la última
                    break;
            }
            
            if(cuerda_actual > 0) {
                LCD_Clear();
                LCD_SetCursor(0,0);
                sprintf(buffer_LCD, "Cuerda %d...", cuerda_actual);
                LCD_Print(buffer_LCD);
                
                // Reiniciamos el proceso de hardware por si estaba a medias
                Hardware_Reiniciar_Proceso();
            }
            sw_estado_ant = sw_lectura;
        }
        
        /* -----------------------------------------------------------
           2. Lógica de Adquisición y Procesamiento
           Solo se ejecuta si hay una cuerda seleccionada
           ----------------------------------------------------------- */
        if (cuerda_actual > 0)
        {
            // A. ADQUISICIÓN DE DATOS (Bloqueante por N muestras)
            for(i = 0; i < N; i++)
            {
                
                
                
                
                ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_WAIT_FOR_RESULT);
    
                // Leer resultado (int16 o uint16 dependiendo de config)
                uint16 lectura_adc = ADC_DelSig_1_GetResult16();
                
                
                // LOGICA DE FILTRADO (ZONA MUERTA)
                // Calculamos la diferencia absoluta respecto al centro (2.5V)
                uint16 diferencia = lectura_adc - OFFSET_DC;
                
                if(abs((int)diferencia) < UMBRAL_RUIDO)
                {
                    // Si la amplitud es menor a 0.0125V, forzamos al centro.
                    // Así eliminamos la señal pequeña y dejamos una linea plana limpia.
                    lectura_adc = OFFSET_DC;
                }
                // Si es mayor, dejamos pasar el valor original (lectura_adc se mantiene)

                // Guardar en el buffer casteado a float
                y_entrada[i] = (float)lectura_adc;

            }

            // B. PRE-PROCESAMIENTO (Float -> Q31 para el DFB/Filtro HW)
            // Usamos el buffer global y_entrada_q32 definido en hardware.c/h
            convertToQ32(y_entrada, y_entrada_q32, N);
            
            // C. FILTRADO POR HARDWARE
            dma_transfer_complete = 0; // Resetear bandera
            Hardware_Configurar_Disparar_DMA();
            
            // Esperar a que el DMA/Filtro termine (bandera activada en ISR del DMA)
            while(dma_transfer_complete == 0)
            {
                // Podríamos chequear botones aquí si quisiéramos salir rápido
            }
            
            // D. CÁLCULO DE FRECUENCIA
            // Usamos buffer global de salida y_salida_pulsos_hw
            freq_calculada = Calcular_Frecuencia_Regresion(y_salida_pulsos_hw, N);
            
            // E. MOSTRAR RESULTADOS
            Actualizar_LCD_Resultado(freq_calculada, freq_objetivo, cuerda_actual);
            
            // Pequeña pausa visual para no parpadear la pantalla demasiado rápido
            // CyDelay(50); 
        }
    }
}

// Función Lógica LCD
void Actualizar_LCD_Resultado(float freq_medida, float freq_target, int num_cuerda) {
    // Solo borramos parte de la pantalla o usamos lógica inteligente para evitar parpadeo
    // Pero por simplicidad, limpiamos si cambia drásticamente o sobrescribimos.
    
    LCD_SetCursor(0, 0);
    if (freq_medida > 10.0f) {
        sprintf(buffer_LCD, "C%d: %.1f Hz   ", num_cuerda, freq_medida); // Mostrar entero para lectura rápida
        LCD_Print(buffer_LCD);

        LCD_SetCursor(1, 0);
        float diferencia = freq_medida - freq_target;

       
        

        if (diferencia < -TOLERANCIA) {
            LCD_Print(">> TENSAR (+)   "); 
        } else if (diferencia > TOLERANCIA) {
            LCD_Print("<< AFLOJAR (-)  ");
        } else {
            LCD_Print("** AFINADO **   ");
            CyDelay(1000);
        }

    } else {
        // Si no detecta frecuencia clara (ruido o silencio)
        sprintf(buffer_LCD, "Cuerda %d (---)", num_cuerda);
        LCD_Print(buffer_LCD);
        LCD_SetCursor(1, 0);
        LCD_Print("Escuchando...   ");
    }
}