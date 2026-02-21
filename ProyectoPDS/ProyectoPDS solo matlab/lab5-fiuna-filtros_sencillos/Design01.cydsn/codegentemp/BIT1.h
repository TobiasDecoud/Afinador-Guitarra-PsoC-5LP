/*******************************************************************************
* File Name: BIT1.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_BIT1_H) /* Pins BIT1_H */
#define CY_PINS_BIT1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "BIT1_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 BIT1__PORT == 15 && ((BIT1__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    BIT1_Write(uint8 value);
void    BIT1_SetDriveMode(uint8 mode);
uint8   BIT1_ReadDataReg(void);
uint8   BIT1_Read(void);
void    BIT1_SetInterruptMode(uint16 position, uint16 mode);
uint8   BIT1_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the BIT1_SetDriveMode() function.
     *  @{
     */
        #define BIT1_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define BIT1_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define BIT1_DM_RES_UP          PIN_DM_RES_UP
        #define BIT1_DM_RES_DWN         PIN_DM_RES_DWN
        #define BIT1_DM_OD_LO           PIN_DM_OD_LO
        #define BIT1_DM_OD_HI           PIN_DM_OD_HI
        #define BIT1_DM_STRONG          PIN_DM_STRONG
        #define BIT1_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define BIT1_MASK               BIT1__MASK
#define BIT1_SHIFT              BIT1__SHIFT
#define BIT1_WIDTH              1u

/* Interrupt constants */
#if defined(BIT1__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in BIT1_SetInterruptMode() function.
     *  @{
     */
        #define BIT1_INTR_NONE      (uint16)(0x0000u)
        #define BIT1_INTR_RISING    (uint16)(0x0001u)
        #define BIT1_INTR_FALLING   (uint16)(0x0002u)
        #define BIT1_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define BIT1_INTR_MASK      (0x01u) 
#endif /* (BIT1__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define BIT1_PS                     (* (reg8 *) BIT1__PS)
/* Data Register */
#define BIT1_DR                     (* (reg8 *) BIT1__DR)
/* Port Number */
#define BIT1_PRT_NUM                (* (reg8 *) BIT1__PRT) 
/* Connect to Analog Globals */                                                  
#define BIT1_AG                     (* (reg8 *) BIT1__AG)                       
/* Analog MUX bux enable */
#define BIT1_AMUX                   (* (reg8 *) BIT1__AMUX) 
/* Bidirectional Enable */                                                        
#define BIT1_BIE                    (* (reg8 *) BIT1__BIE)
/* Bit-mask for Aliased Register Access */
#define BIT1_BIT_MASK               (* (reg8 *) BIT1__BIT_MASK)
/* Bypass Enable */
#define BIT1_BYP                    (* (reg8 *) BIT1__BYP)
/* Port wide control signals */                                                   
#define BIT1_CTL                    (* (reg8 *) BIT1__CTL)
/* Drive Modes */
#define BIT1_DM0                    (* (reg8 *) BIT1__DM0) 
#define BIT1_DM1                    (* (reg8 *) BIT1__DM1)
#define BIT1_DM2                    (* (reg8 *) BIT1__DM2) 
/* Input Buffer Disable Override */
#define BIT1_INP_DIS                (* (reg8 *) BIT1__INP_DIS)
/* LCD Common or Segment Drive */
#define BIT1_LCD_COM_SEG            (* (reg8 *) BIT1__LCD_COM_SEG)
/* Enable Segment LCD */
#define BIT1_LCD_EN                 (* (reg8 *) BIT1__LCD_EN)
/* Slew Rate Control */
#define BIT1_SLW                    (* (reg8 *) BIT1__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define BIT1_PRTDSI__CAPS_SEL       (* (reg8 *) BIT1__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define BIT1_PRTDSI__DBL_SYNC_IN    (* (reg8 *) BIT1__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define BIT1_PRTDSI__OE_SEL0        (* (reg8 *) BIT1__PRTDSI__OE_SEL0) 
#define BIT1_PRTDSI__OE_SEL1        (* (reg8 *) BIT1__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define BIT1_PRTDSI__OUT_SEL0       (* (reg8 *) BIT1__PRTDSI__OUT_SEL0) 
#define BIT1_PRTDSI__OUT_SEL1       (* (reg8 *) BIT1__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define BIT1_PRTDSI__SYNC_OUT       (* (reg8 *) BIT1__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(BIT1__SIO_CFG)
    #define BIT1_SIO_HYST_EN        (* (reg8 *) BIT1__SIO_HYST_EN)
    #define BIT1_SIO_REG_HIFREQ     (* (reg8 *) BIT1__SIO_REG_HIFREQ)
    #define BIT1_SIO_CFG            (* (reg8 *) BIT1__SIO_CFG)
    #define BIT1_SIO_DIFF           (* (reg8 *) BIT1__SIO_DIFF)
#endif /* (BIT1__SIO_CFG) */

/* Interrupt Registers */
#if defined(BIT1__INTSTAT)
    #define BIT1_INTSTAT            (* (reg8 *) BIT1__INTSTAT)
    #define BIT1_SNAP               (* (reg8 *) BIT1__SNAP)
    
	#define BIT1_0_INTTYPE_REG 		(* (reg8 *) BIT1__0__INTTYPE)
#endif /* (BIT1__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_BIT1_H */


/* [] END OF FILE */
