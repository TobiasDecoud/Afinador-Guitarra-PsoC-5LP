#include "project.h"
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define N 1000
#define Q_FACTOR 8388608.0f

/* --- Buffers --- */
static uint8 x[N*4]; 
float y_entrada[N] = {0}; 
float y_salida[N] = {0}; // Inicializar a 0 es más seguro
static uint32_t y_entrada_q32[N]; 
static uint32_t y_salida_q32[N]; 

/* --- Variables de Control --- */
static uint16 n = 0;
volatile uint8 estado = 0u;
volatile uint8 dma_transfer_complete_flag = 0;
uint8 DMA_In_Chan, DMA_Out_Chan;
uint8 DMA_In_TD[1], DMA_Out_TD[1];

/* --- Prototipos --- */
CY_ISR_PROTO(RxIsr);
CY_ISR_PROTO(DMA_Done_ISR);
void my_Start();
void convertToIntFloat(const uint8_t* input, float* output, size_t inputSize);
void convertToQ32(const float* input, uint32_t* output, size_t size);
void convertFromQ32(const uint32_t* input, float* output, size_t size);


#if defined (__GNUC__)
    asm(".global _printf_float");
#endif

int main(void)
{
    my_Start();
    isr_rx_StartEx(RxIsr);
    isr_dma_done_StartEx(DMA_Done_ISR);
    CyGlobalIntEnable;

    DMA_In_Chan = DMA_In_DmaInitialize(4, 1, HI16(CYDEV_SRAM_BASE), HI16(CYDEV_PERIPH_BASE));
    DMA_In_TD[0] = CyDmaTdAllocate();
    DMA_Out_Chan = DMA_Out_DmaInitialize(4, 1, HI16(CYDEV_PERIPH_BASE), HI16(CYDEV_SRAM_BASE));
    DMA_Out_TD[0] = CyDmaTdAllocate();
 
    for(;;)
    {
        switch(estado) {
            case '0': break;
            case 'I': estado = '0'; break;
            
            case 'X': 
                dma_transfer_complete_flag = 0;
                
                convertToQ32(y_entrada, y_entrada_q32, N);
                
                // Configurar el DMA de Entrada
                CyDmaTdSetConfiguration(DMA_In_TD[0], N * 4, DMA_In_TD[0], CY_DMA_TD_INC_SRC_ADR);
                CyDmaTdSetAddress(DMA_In_TD[0], LO16((uint32)y_entrada_q32), LO16((uint32)Filter_1_STAGEA_PTR));
                
                // Configurar el DMA de Salida (la interrupción se genera al final de este)
                CyDmaTdSetConfiguration(DMA_Out_TD[0], N * 4, DMA_Out_TD[0], CY_DMA_TD_INC_DST_ADR | DMA_Out__TD_TERMOUT_EN);
                CyDmaTdSetAddress(DMA_Out_TD[0], LO16((uint32)Filter_1_HOLDA_PTR), LO16((uint32)y_salida_q32));
                
                CyDmaChSetInitialTd(DMA_In_Chan, DMA_In_TD[0]);
                CyDmaChSetInitialTd(DMA_Out_Chan, DMA_Out_TD[0]);
                
                CyDmaChEnable(DMA_In_Chan, 1);
                CyDmaChEnable(DMA_Out_Chan, 1);
                
                // <-- LA SOLUCIÓN DEFINITIVA BASADA EN EL DATASHEET
                // Forzamos la PRIMERA transferencia del DMA por software.
                // Esto envía el primer dato al Filtro, rompiendo el bloqueo.
                // El resto de las transferencias se sincronizarán por hardware (drq).
                CyDmaChSetRequest(DMA_In_Chan, 1);
                
                estado = 'W';
                break;

            case 'W': 
                if (dma_transfer_complete_flag == 1)
                {
                    convertFromQ32(y_salida_q32, y_salida, N);
                    estado = '0';
                }
                break;

            case 'P': 
                for(int j = 0; j < N; j++) { 
                    Chart_1_Plot(y_entrada[j]);
                }
                estado = '0';
                break;
                
            case 'F': 
                for(int j = 0; j < N; j++) {
                    Chart_1_Plot(y_salida[j]);
                }
                estado = '0';
            break;
        }
    }
}

/* --- ISRs y Funciones de Utilidad (SIN CAMBIOS) --- */

CY_ISR(DMA_Done_ISR)
{
    // Esta ISR se dispara cuando el DMA_Out termina,
    // lo que significa que todo el bloque ha sido procesado.
    dma_transfer_complete_flag = 1;
    isr_dma_done_ClearPending();
}

CY_ISR(RxIsr)
{
    uint8 rxData;
    while((UART_1_GetRxBufferSize() > 0))
    {
        rxData = UART_1_ReadRxData();
        if (rxData == 'I' || rxData == 'P' || rxData == 'F') {
            estado = rxData;
            n = 0;
        } else {
            if (n < (N * 4)) x[n++] = rxData;
            if (n == (N * 4)) { 
                convertToIntFloat(x, y_entrada, N*4);
                estado = 'X'; 
                n = 0;
            }
        }
    }
    isr_rx_ClearPending(); 
}

void convertToIntFloat(const uint8_t* input, float* output, size_t inputSize) {
    for (size_t i = 0; i < inputSize; i += 4) {
        // Usar memcpy es más seguro y portable que el type-punning
        uint32_t temp;
        memcpy(&temp, input + i, sizeof(uint32_t));
        memcpy(output + (i / 4), &temp, sizeof(float));
    }
}

void convertToQ32(const float* input, uint32_t* output, size_t size) {
    for (size_t i = 0; i < size; i++) {
        output[i] = (int32_t)(input[i] * Q_FACTOR); 
    }
}

void convertFromQ32(const uint32_t* input, float* output, size_t size) {
    for (size_t i = 0; i < size; i++) {
        output[i] = (float)((int32_t)input[i] / Q_FACTOR);
    }
}

void my_Start(){    UART_1_Start();
    Filter_1_Start(); 
}