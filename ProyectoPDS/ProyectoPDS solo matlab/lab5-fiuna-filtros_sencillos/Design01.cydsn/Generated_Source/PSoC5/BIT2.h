/*******************************************************************************
* File Name: BIT2.h  
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

#if !defined(CY_PINS_BIT2_H) /* Pins BIT2_H */
#define CY_PINS_BIT2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "BIT2_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 BIT2__PORT == 15 && ((BIT2__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    BIT2_Write(uint8 value);
void    BIT2_SetDriveMode(uint8 mode);
uint8   BIT2_ReadDataReg(void);
uint8   BIT2_Read(void);
void    BIT2_SetInterruptMode(uint16 position, uint16 mode);
uint8   BIT2_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the BIT2_SetDriveMode() function.
     *  @{
     */
        #define BIT2_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define BIT2_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define BIT2_DM_RES_UP          PIN_DM_RES_UP
        #define BIT2_DM_RES_DWN         PIN_DM_RES_DWN
        #define BIT2_DM_OD_LO           PIN_DM_OD_LO
        #define BIT2_DM_OD_HI           PIN_DM_OD_HI
        #define BIT2_DM_STRONG          PIN_DM_STRONG
        #define BIT2_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define BIT2_MASK               BIT2__MASK
#define BIT2_SHIFT              BIT2__SHIFT
#define BIT2_WIDTH              1u

/* Interrupt constants */
#if defined(BIT2__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in BIT2_SetInterruptMode() function.
     *  @{
     */
        #define BIT2_INTR_NONE      (uint16)(0x0000u)
        #define BIT2_INTR_RISING    (uint16)(0x0001u)
        #define BIT2_INTR_FALLING   (uint16)(0x0002u)
        #define BIT2_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define BIT2_INTR_MASK      (0x01u) 
#endif /* (BIT2__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define BIT2_PS                     (* (reg8 *) BIT2__PS)
/* Data Register */
#define BIT2_DR                     (* (reg8 *) BIT2__DR)
/* Port Number */
#define BIT2_PRT_NUM                (* (reg8 *) BIT2__PRT) 
/* Connect to Analog Globals */                                                  
#define BIT2_AG                     (* (reg8 *) BIT2__AG)                       
/* Analog MUX bux enable */
#define BIT2_AMUX                   (* (reg8 *) BIT2__AMUX) 
/* Bidirectional Enable */                                                        
#define BIT2_BIE                    (* (reg8 *) BIT2__BIE)
/* Bit-mask for Aliased Register Access */
#define BIT2_BIT_MASK               (* (reg8 *) BIT2__BIT_MASK)
/* Bypass Enable */
#define BIT2_BYP                    (* (reg8 *) BIT2__BYP)
/* Port wide control signals */                                                   
#define BIT2_CTL                    (* (reg8 *) BIT2__CTL)
/* Drive Modes */
#define BIT2_DM0                    (* (reg8 *) BIT2__DM0) 
#define BIT2_DM1                    (* (reg8 *) BIT2__DM1)
#define BIT2_DM2                    (* (reg8 *) BIT2__DM2) 
/* Input Buffer Disable Override */
#define BIT2_INP_DIS                (* (reg8 *) BIT2__INP_DIS)
/* LCD Common or Segment Drive */
#define BIT2_LCD_COM_SEG            (* (reg8 *) BIT2__LCD_COM_SEG)
/* Enable Segment LCD */
#define BIT2_LCD_EN                 (* (reg8 *) BIT2__LCD_EN)
/* Slew Rate Control */
#define BIT2_SLW                    (* (reg8 *) BIT2__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define BIT2_PRTDSI__CAPS_SEL       (* (reg8 *) BIT2__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define BIT2_PRTDSI__DBL_SYNC_IN    (* (reg8 *) BIT2__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define BIT2_PRTDSI__OE_SEL0        (* (reg8 *) BIT2__PRTDSI__OE_SEL0) 
#define BIT2_PRTDSI__OE_SEL1        (* (reg8 *) BIT2__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define BIT2_PRTDSI__OUT_SEL0       (* (reg8 *) BIT2__PRTDSI__OUT_SEL0) 
#define BIT2_PRTDSI__OUT_SEL1       (* (reg8 *) BIT2__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define BIT2_PRTDSI__SYNC_OUT       (* (reg8 *) BIT2__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(BIT2__SIO_CFG)
    #define BIT2_SIO_HYST_EN        (* (reg8 *) BIT2__SIO_HYST_EN)
    #define BIT2_SIO_REG_HIFREQ     (* (reg8 *) BIT2__SIO_REG_HIFREQ)
    #define BIT2_SIO_CFG            (* (reg8 *) BIT2__SIO_CFG)
    #define BIT2_SIO_DIFF           (* (reg8 *) BIT2__SIO_DIFF)
#endif /* (BIT2__SIO_CFG) */

/* Interrupt Registers */
#if defined(BIT2__INTSTAT)
    #define BIT2_INTSTAT            (* (reg8 *) BIT2__INTSTAT)
    #define BIT2_SNAP               (* (reg8 *) BIT2__SNAP)
    
	#define BIT2_0_INTTYPE_REG 		(* (reg8 *) BIT2__0__INTTYPE)
#endif /* (BIT2__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_BIT2_H */


/* [] END OF FILE */
