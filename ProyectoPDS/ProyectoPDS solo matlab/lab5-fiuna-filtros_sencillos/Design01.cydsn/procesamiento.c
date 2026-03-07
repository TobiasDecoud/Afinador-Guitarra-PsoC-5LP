#include "procesamiento.h"
#include <string.h> // Para memcpy




void convertToQ32(const float* input, uint32_t* output, size_t size) {
    for (size_t i = 0; i < size; i++) {
        // Opción A: Si input viene del ADC (ej. 15000.0)
        // Convertimos a entero y desplazamos 8 bits a la izquierda.
        // Esto convierte 16 bits en 24 bits efectivos para el DFB.
        // Ejemplo: 0x7FFF (max ADC) -> 0x7FFF00 (Alto volumen para DFB)
        
        int32_t valor_temp = (int32_t)input[i];
        output[i] = (uint32_t)(valor_temp); 
    }
}

float Calcular_Frecuencia_Regresion(const uint8_t* datos, uint16 size) {
    double sum_x = 0, sum_y = 0, sum_xy = 0, sum_xx = 0;
    int pulse_count = 0;
    float frecuencia = 0.0f;
   
    for(int i = 0; i < size; i++) {
        if (datos[i] & 0x01) { // Si hay pulso (LSB = 1)
            sum_x += pulse_count;
            sum_y += i;
            sum_xy += (double)pulse_count * i;
            sum_xx += (double)pulse_count * pulse_count;
            pulse_count++;
        }
    }

    if (pulse_count > 1) {
        double n_dbl = (double)pulse_count;
        double numerator = (n_dbl * sum_xy) - (sum_x * sum_y);
        double denominator = (n_dbl * sum_xx) - (sum_x * sum_x);
        
        if (denominator != 0) {
            float avg_period_samples = (float)(numerator / denominator);
            if (avg_period_samples > 0.0001f) {
                // Frecuencia = (Fs / muestras_por_pulso) / 2
                frecuencia = ((SAMPLING_FREQ*Kcal) / avg_period_samples) / 2.0f;
            }
        }
    }
    return frecuencia;
}

