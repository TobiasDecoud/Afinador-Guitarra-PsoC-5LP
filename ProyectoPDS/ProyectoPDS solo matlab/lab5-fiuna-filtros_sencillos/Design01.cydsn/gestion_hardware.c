#include "gestion_hardware.h"

// --- Buffers Reales ---
uint32_t y_entrada_q32[N];
uint8_t  y_salida_pulsos_hw[N];
volatile uint8 dma_transfer_complete = 0;

// --- Variables Privadas (Handles DMA) ---
static uint8 DMA_In_Chan, DMA_Out_Chan, DMA_a_Comp_Chan;
static uint8 DMA_In_TD[2], DMA_Out_TD[1], DMA_a_Comp_TD[1];

// Prototipo ISR
CY_ISR_PROTO(DMA_Done_ISR);

// Implementación ISR
CY_ISR(DMA_Done_ISR) {
    CyDmaChDisable(DMA_In_Chan);
    CyDmaChDisable(DMA_Out_Chan);
    CyDmaChDisable(DMA_a_Comp_Chan);
    dma_transfer_complete = 1;
    isr_dma_done_ClearPending();
}

void Hardware_Iniciar(void) {
    // 1. Iniciar Componentes
    Filter_1_Start();
    
    // 2. Inicializar Canales DMA
    DMA_In_Chan = DMA_In_DmaInitialize(4, 1, HI16(CYDEV_SRAM_BASE), HI16(CYDEV_PERIPH_BASE));
    DMA_Out_Chan = DMA_Out_DmaInitialize(1, 1, HI16(CYDEV_PERIPH_BASE), HI16(CYDEV_SRAM_BASE));
    DMA_a_Comp_Chan = DMA_a_Comp_DmaInitialize(1, 1, HI16(CYDEV_PERIPH_BASE), HI16(CYDEV_PERIPH_BASE));

    // 3. Reservar TDs
    DMA_In_TD[0] = CyDmaTdAllocate();
    DMA_In_TD[1] = CyDmaTdAllocate(); 
    DMA_Out_TD[0] = CyDmaTdAllocate();
    DMA_a_Comp_TD[0] = CyDmaTdAllocate();
    
    // 4. Configurar ISR
    isr_dma_done_StartEx(DMA_Done_ISR);
}

void Hardware_Configurar_Disparar_DMA(void) {
    dma_transfer_complete = 0;

    // 1. DMA al Comparador/Control
    CyDmaTdSetConfiguration(DMA_a_Comp_TD[0], N, DMA_a_Comp_TD[0], 0u);
    CyDmaTdSetAddress(DMA_a_Comp_TD[0], LO16((uint32)Filter_1_HOLDAH_PTR), LO16((uint32)CtrlReg_Filtro_Out_Control_PTR));
    CyDmaChSetInitialTd(DMA_a_Comp_Chan, DMA_a_Comp_TD[0]);
    CyDmaChEnable(DMA_a_Comp_Chan, 1);

    // 2. DMA Entrada (RAM -> Filtro) - Usando el buffer global y_entrada_q32
    CyDmaTdSetConfiguration(DMA_In_TD[0], (N * 4) / 2, DMA_In_TD[1], CY_DMA_TD_INC_SRC_ADR);
    CyDmaTdSetAddress(DMA_In_TD[0], LO16((uint32)y_entrada_q32), LO16((uint32)Filter_1_STAGEA_PTR));
    
    CyDmaTdSetConfiguration(DMA_In_TD[1], (N * 4) / 2, CY_DMA_DISABLE_TD, CY_DMA_TD_INC_SRC_ADR);
    CyDmaTdSetAddress(DMA_In_TD[1], LO16((uint32)&y_entrada_q32[N/2]), LO16((uint32)Filter_1_STAGEA_PTR));

    // 3. DMA Salida (Filtro -> RAM) - Usando el buffer global y_salida_pulsos_hw
    CyDmaTdSetConfiguration(DMA_Out_TD[0], N, DMA_Out_TD[0], CY_DMA_TD_INC_DST_ADR | DMA_Out__TD_TERMOUT_EN);
    CyDmaTdSetAddress(DMA_Out_TD[0], LO16((uint32)Status_Reg_1_Status_PTR), LO16((uint32)y_salida_pulsos_hw));

    // Activar y Disparar
    CyDmaChSetInitialTd(DMA_Out_Chan, DMA_Out_TD[0]);
    CyDmaChSetInitialTd(DMA_In_Chan, DMA_In_TD[0]);
    CyDmaChEnable(DMA_Out_Chan, 1);
    CyDmaChEnable(DMA_In_Chan, 1);
    CyDmaChSetRequest(DMA_In_Chan, 1);
}

void Hardware_Cargar_Coeficientes(const uint8* new_coeffs, uint16 num_bytes) {
    Filter_1_RAM_DIR_REG = Filter_1_RAM_DIR_BUS;
    void* dest_ptr = (void*) Filter_1_DB_RAM; // O DA_RAM
    const void* src_ptr = (const void*) new_coeffs;
    cymemcpy(dest_ptr, src_ptr, num_bytes);
    Filter_1_RAM_DIR_REG = Filter_1_RAM_DIR_DFB;
}



void Hardware_Reiniciar_Proceso(void) {
    dma_transfer_complete = 0;
    
  
    memset(y_entrada_q32, 0, sizeof(y_entrada_q32));
    memset(y_salida_pulsos_hw, 0, sizeof(y_salida_pulsos_hw));
}

