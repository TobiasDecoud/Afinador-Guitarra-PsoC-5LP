/*******************************************************************************
* File Name: OpAmp_Vref.c
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

#include "OpAmp_Vref.h"

uint8 OpAmp_Vref_initVar = 0u;


/*******************************************************************************   
* Function Name: OpAmp_Vref_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the 
*  customizer of the component placed onto schematic. Usually called in 
*  OpAmp_Vref_Start().
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void OpAmp_Vref_Init(void) 
{
    OpAmp_Vref_SetPower(OpAmp_Vref_DEFAULT_POWER);
}


/*******************************************************************************   
* Function Name: OpAmp_Vref_Enable
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
void OpAmp_Vref_Enable(void) 
{
    /* Enable negative charge pumps in ANIF */
    OpAmp_Vref_PUMP_CR1_REG  |= (OpAmp_Vref_PUMP_CR1_CLKSEL | OpAmp_Vref_PUMP_CR1_FORCE);

    /* Enable power to buffer in active mode */
    OpAmp_Vref_PM_ACT_CFG_REG |= OpAmp_Vref_ACT_PWR_EN;

    /* Enable power to buffer in alternative active mode */
    OpAmp_Vref_PM_STBY_CFG_REG |= OpAmp_Vref_STBY_PWR_EN;
}


/*******************************************************************************
* Function Name:   OpAmp_Vref_Start
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
*  OpAmp_Vref_initVar: Used to check the initial configuration, modified 
*  when this function is called for the first time.
*
*******************************************************************************/
void OpAmp_Vref_Start(void) 
{
    if(OpAmp_Vref_initVar == 0u)
    {
        OpAmp_Vref_initVar = 1u;
        OpAmp_Vref_Init();
    }

    OpAmp_Vref_Enable();
}


/*******************************************************************************
* Function Name: OpAmp_Vref_Stop
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
void OpAmp_Vref_Stop(void) 
{
    /* Disable power to buffer in active mode template */
    OpAmp_Vref_PM_ACT_CFG_REG &= (uint8)(~OpAmp_Vref_ACT_PWR_EN);

    /* Disable power to buffer in alternative active mode template */
    OpAmp_Vref_PM_STBY_CFG_REG &= (uint8)(~OpAmp_Vref_STBY_PWR_EN);
    
    /* Disable negative charge pumps for ANIF only if all ABuf is turned OFF */
    if(OpAmp_Vref_PM_ACT_CFG_REG == 0u)
    {
        OpAmp_Vref_PUMP_CR1_REG &= (uint8)(~(OpAmp_Vref_PUMP_CR1_CLKSEL | OpAmp_Vref_PUMP_CR1_FORCE));
    }
}


/*******************************************************************************
* Function Name: OpAmp_Vref_SetPower
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
void OpAmp_Vref_SetPower(uint8 power) 
{
    #if (CY_PSOC3 || CY_PSOC5LP)
        OpAmp_Vref_CR_REG &= (uint8)(~OpAmp_Vref_PWR_MASK);
        OpAmp_Vref_CR_REG |= power & OpAmp_Vref_PWR_MASK;      /* Set device power */
    #else
        CYASSERT(OpAmp_Vref_HIGHPOWER == power);
    #endif /* CY_PSOC3 || CY_PSOC5LP */
}


/* [] END OF FILE */
