/*******************************************************************************
* File Name: BIT3.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_BIT3_ALIASES_H) /* Pins BIT3_ALIASES_H */
#define CY_PINS_BIT3_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants        
***************************************/
#define BIT3_0			(BIT3__0__PC)
#define BIT3_0_INTR	((uint16)((uint16)0x0001u << BIT3__0__SHIFT))

#define BIT3_INTR_ALL	 ((uint16)(BIT3_0_INTR))

#endif /* End Pins BIT3_ALIASES_H */


/* [] END OF FILE */
