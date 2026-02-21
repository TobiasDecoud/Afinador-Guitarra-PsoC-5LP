/*******************************************************************************
* File Name: OpAmp_Acoplador_PM.c
* Version 1.90
*
* Description:
*  This file provides the power management source code to the API for the 
*  OpAmp (Analog Buffer) component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "OpAmp_Acoplador.h"

static OpAmp_Acoplador_BACKUP_STRUCT  OpAmp_Acoplador_backup;


/*******************************************************************************  
* Function Name: OpAmp_Acoplador_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration registers.
* 
* Parameters:
*  void
* 
* Return:
*  void
*
*******************************************************************************/
void OpAmp_Acoplador_SaveConfig(void) 
{
    /* Nothing to save as registers are System reset on retention flops */
}


/*******************************************************************************  
* Function Name: OpAmp_Acoplador_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration registers.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void OpAmp_Acoplador_RestoreConfig(void) 
{
    /* Nothing to restore */
}


/*******************************************************************************   
* Function Name: OpAmp_Acoplador_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves its configuration. Should be called 
*  just prior to entering sleep.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  OpAmp_Acoplador_backup: The structure field 'enableState' is modified 
*  depending on the enable state of the block before entering to sleep mode.
*
*******************************************************************************/
void OpAmp_Acoplador_Sleep(void) 
{
    /* Save OpAmp enable state */
    if((OpAmp_Acoplador_PM_ACT_CFG_REG & OpAmp_Acoplador_ACT_PWR_EN) != 0u)
    {
        /* Component is enabled */
        OpAmp_Acoplador_backup.enableState = 1u;
         /* Stops the component */
         OpAmp_Acoplador_Stop();
    }
    else
    {
        /* Component is disabled */
        OpAmp_Acoplador_backup.enableState = 0u;
    }
    /* Saves the configuration */
    OpAmp_Acoplador_SaveConfig();
}


/*******************************************************************************  
* Function Name: OpAmp_Acoplador_Wakeup
********************************************************************************
*
* Summary:
*  Enables block's operation and restores its configuration. Should be called
*  just after awaking from sleep.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  OpAmp_Acoplador_backup: The structure field 'enableState' is used to 
*  restore the enable state of block after wakeup from sleep mode.
*
*******************************************************************************/
void OpAmp_Acoplador_Wakeup(void) 
{
    /* Restore the user configuration */
    OpAmp_Acoplador_RestoreConfig();

    /* Enables the component operation */
    if(OpAmp_Acoplador_backup.enableState == 1u)
    {
        OpAmp_Acoplador_Enable();
    } /* Do nothing if component was disable before */
}


/* [] END OF FILE */
