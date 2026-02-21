#ifndef PROCESAMIENTO_H
#define PROCESAMIENTO_H

#include "project.h"
#include "configuracion.h"



void convertToQ32(const float* input, uint32_t* output, size_t size);


float Calcular_Frecuencia_Regresion(const uint8_t* datos, uint16 size);

#endif