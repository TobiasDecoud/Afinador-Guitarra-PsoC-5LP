/*******************************************************************************
* File Name: CtrlReg_Filtro_Out_PM.c
* Version 1.80
*
* Description:
*  This file contains the setup, control, and status commands to support 
*  the component operation in the low power mode. 
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "CtrlReg_Filtro_Out.h"

/* Check for removal by optimization */
#if !defined(CtrlReg_Filtro_Out_Sync_ctrl_reg__REMOVED)

static CtrlReg_Filtro_Out_BACKUP_STRUCT  CtrlReg_Filtro_Out_backup = {0u};

    
/*******************************************************************************
* Function Name: CtrlReg_Filtro_Out_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void CtrlReg_Filtro_Out_SaveConfig(void) 
{
    CtrlReg_Filtro_Out_backup.controlState = CtrlReg_Filtro_Out_Control;
}


/*******************************************************************************
* Function Name: CtrlReg_Filtro_Out_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*
*******************************************************************************/
void CtrlReg_Filtro_Out_RestoreConfig(void) 
{
     CtrlReg_Filtro_Out_Control = CtrlReg_Filtro_Out_backup.controlState;
}


/*******************************************************************************
* Function Name: CtrlReg_Filtro_Out_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component for entering the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void CtrlReg_Filtro_Out_Sleep(void) 
{
    CtrlReg_Filtro_Out_SaveConfig();
}


/*******************************************************************************
* Function Name: CtrlReg_Filtro_Out_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component after waking up from the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void CtrlReg_Filtro_Out_Wakeup(void)  
{
    CtrlReg_Filtro_Out_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
