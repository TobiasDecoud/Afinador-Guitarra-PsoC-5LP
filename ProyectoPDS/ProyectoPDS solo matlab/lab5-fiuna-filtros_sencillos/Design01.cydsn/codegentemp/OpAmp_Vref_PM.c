/*******************************************************************************
* File Name: OpAmp_Vref_PM.c
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

#include "OpAmp_Vref.h"

static OpAmp_Vref_BACKUP_STRUCT  OpAmp_Vref_backup;


/*******************************************************************************  
* Function Name: OpAmp_Vref_SaveConfig
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
void OpAmp_Vref_SaveConfig(void) 
{
    /* Nothing to save as registers are System reset on retention flops */
}


/*******************************************************************************  
* Function Name: OpAmp_Vref_RestoreConfig
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
void OpAmp_Vref_RestoreConfig(void) 
{
    /* Nothing to restore */
}


/*******************************************************************************   
* Function Name: OpAmp_Vref_Sleep
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
*  OpAmp_Vref_backup: The structure field 'enableState' is modified 
*  depending on the enable state of the block before entering to sleep mode.
*
*******************************************************************************/
void OpAmp_Vref_Sleep(void) 
{
    /* Save OpAmp enable state */
    if((OpAmp_Vref_PM_ACT_CFG_REG & OpAmp_Vref_ACT_PWR_EN) != 0u)
    {
        /* Component is enabled */
        OpAmp_Vref_backup.enableState = 1u;
         /* Stops the component */
         OpAmp_Vref_Stop();
    }
    else
    {
        /* Component is disabled */
        OpAmp_Vref_backup.enableState = 0u;
    }
    /* Saves the configuration */
    OpAmp_Vref_SaveConfig();
}


/*******************************************************************************  
* Function Name: OpAmp_Vref_Wakeup
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
*  OpAmp_Vref_backup: The structure field 'enableState' is used to 
*  restore the enable state of block after wakeup from sleep mode.
*
*******************************************************************************/
void OpAmp_Vref_Wakeup(void) 
{
    /* Restore the user configuration */
    OpAmp_Vref_RestoreConfig();

    /* Enables the component operation */
    if(OpAmp_Vref_backup.enableState == 1u)
    {
        OpAmp_Vref_Enable();
    } /* Do nothing if component was disable before */
}


/* [] END OF FILE */
