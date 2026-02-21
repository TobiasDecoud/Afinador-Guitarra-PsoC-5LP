#ifndef GESTION_HARDWARE_H
#define GESTION_HARDWARE_H

#include "project.h"
#include "configuracion.h"

// Variables Globales
extern uint32_t y_entrada_q32[N];     // Buffer de entrada al filtro
extern uint8_t  y_salida_pulsos_hw[N];// Buffer de salida del hardware

// Bandera para saber si el DMA terminó
extern volatile uint8 dma_transfer_complete;

// Funciones de Control
void Hardware_Iniciar(void);
void Hardware_Configurar_Disparar_DMA(void);
void Hardware_Cargar_Coeficientes(const uint8* new_coeffs, uint16 num_bytes);
void Hardware_Reiniciar_Proceso(void);

#endif