/****************************************************************************
*
* Copyright © 2018-2019 STMicroelectronics - All Rights Reserved
*
* License terms: STMicroelectronics Proprietary in accordance with licensing
* terms SLA0089 at www.st.com.
* 
* THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED, 
* INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
* EVALUATION ONLY – NOT FOR USE IN PRODUCTION
*****************************************************************************/


/**
 * @file    eeprom_lld_cfg.h
 * @brief   EEPROM Driver configuration macros and structures.
 *
 * @addtogroup EEPROM
 * @{
 */

#ifndef _EEPROM_LLD_CFG_H_
#define _EEPROM_LLD_CFG_H_

#include "flashdriver.h"
#include "spc5_lld.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    EEPROM Emulation flag definitions
 * @{
 */
#define EEPROM_BLOCK_ACTIVE              0x000000000000FFFFUL
#define EEPROM_BLOCK_INVALID             0x0000000000000000UL
#define EEPROM_SWAP_START                0x00000000FFFFFFFFUL

#define EEPROM_PROGRAM_START             0xFFFFFFFFFFFF0000UL
#define EEPROM_PROGRAM_COMPLETE          0xFFFF000000000000UL
#define EEPROM_ID_INVALID                0x0000000000000000UL
#define EEPROM_ID_BAD					 0x0000FFFF00000000UL

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* SSD_CONFIG structure defined in eeprom_lld_cfg.c.*/
extern SSD_CONFIG ssdConfig;

#endif /* _EEPROM_LLD_CFG_H_ */

/** @} */
