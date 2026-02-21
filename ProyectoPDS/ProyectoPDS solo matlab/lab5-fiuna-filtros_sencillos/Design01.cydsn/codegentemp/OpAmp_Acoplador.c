/*******************************************************************************
* File Name: OpAmp_Acoplador.c
* Version 1.90
*
* Description:
*  This file provides the source code to the API for OpAmp (Analog Buffer) 
*  Component.
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

uint8 OpAmp_Acoplador_initVar = 0u;


/*******************************************************************************   
* Function Name: OpAmp_Acoplador_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the 
*  customizer of the component placed onto schematic. Usually called in 
*  OpAmp_Acoplador_Start().
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void OpAmp_Acoplador_Init(void) 
{
    OpAmp_Acoplador_SetPower(OpAmp_Acoplador_DEFAULT_POWER);
}


/*******************************************************************************   
* Function Name: OpAmp_Acoplador_Enable
********************************************************************************
*
* Summary:
*  Enables the OpAmp block operation
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void OpAmp_Acoplador_Enable(void) 
{
    /* Enable negative charge pumps in ANIF */
    OpAmp_Acoplador_PUMP_CR1_REG  |= (OpAmp_Acoplador_PUMP_CR1_CLKSEL | OpAmp_Acoplador_PUMP_CR1_FORCE);

    /* Enable power to buffer in active mode */
    OpAmp_Acoplador_PM_ACT_CFG_REG |= OpAmp_Acoplador_ACT_PWR_EN;

    /* Enable power to buffer in alternative active mode */
    OpAmp_Acoplador_PM_STBY_CFG_REG |= OpAmp_Acoplador_STBY_PWR_EN;
}


/*******************************************************************************
* Function Name:   OpAmp_Acoplador_Start
********************************************************************************
*
* Summary:
*  The start function initializes the Analog Buffer with the default values and 
*  sets the power to the given level. A power level of 0, is same as 
*  executing the stop function.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  OpAmp_Acoplador_initVar: Used to check the initial configuration, modified 
*  when this function is called for the first time.
*
*******************************************************************************/
void OpAmp_Acoplador_Start(void) 
{
    if(OpAmp_Acoplador_initVar == 0u)
    {
        OpAmp_Acoplador_initVar = 1u;
        OpAmp_Acoplador_Init();
    }

    OpAmp_Acoplador_Enable();
}


/*******************************************************************************
* Function Name: OpAmp_Acoplador_Stop
********************************************************************************
*
* Summary:
*  Powers down amplifier to lowest power state.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void OpAmp_Acoplador_Stop(void) 
{
    /* Disable power to buffer in active mode template */
    OpAmp_Acoplador_PM_ACT_CFG_REG &= (uint8)(~OpAmp_Acoplador_ACT_PWR_EN);

    /* Disable power to buffer in alternative active mode template */
    OpAmp_Acoplador_PM_STBY_CFG_REG &= (uint8)(~OpAmp_Acoplador_STBY_PWR_EN);
    
    /* Disable negative charge pumps for ANIF only if all ABuf is turned OFF */
    if(OpAmp_Acoplador_PM_ACT_CFG_REG == 0u)
    {
        OpAmp_Acoplador_PUMP_CR1_REG &= (uint8)(~(OpAmp_Acoplador_PUMP_CR1_CLKSEL | OpAmp_Acoplador_PUMP_CR1_FORCE));
    }
}


/*******************************************************************************
* Function Name: OpAmp_Acoplador_SetPower
********************************************************************************
*
* Summary:
*  Sets power level of Analog buffer.
*
* Parameters: 
*  power: PSoC3: Sets power level between low (1) and high power (3).
*         PSoC5: Sets power level High (0)
*
* Return:
*  void
*
**********************************************************************************/
void OpAmp_Acoplador_SetPower(uint8 power) 
{
    #if (CY_PSOC3 || CY_PSOC5LP)
        OpAmp_Acoplador_CR_REG &= (uint8)(~OpAmp_Acoplador_PWR_MASK);
        OpAmp_Acoplador_CR_REG |= power & OpAmp_Acoplador_PWR_MASK;      /* Set device power */
    #else
        CYASSERT(OpAmp_Acoplador_HIGHPOWER == power);
    #endif /* CY_PSOC3 || CY_PSOC5LP */
}


/* [] END OF FILE */
