/****************************************************************************
*
* Copyright © 2017-2019 STMicroelectronics - All Rights Reserved
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
 * @file    eeprom_cfg.c
 * @brief   EEPROM Driver configuration code.
 *
 * @addtogroup EEPROM
 * @{
 */

#include "eeprom_cfg.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/
/**
 * @brief   Block configuration structure.
 */
BLOCKConfig blockconf[2] = {
  {
  /* block start address*/
  EEPROM_DATAFLASH_ADDR(0UL),
  /*block size*/
  EEPROM_DATAFLASH_BLOCKSIZE,
  /* space */
  FLASH_HIGH_SPACE,
  /* number */
  0U,
  },
  {
   /* block start address*/
  EEPROM_DATAFLASH_ADDR(1UL),
  /*block size*/
  EEPROM_DATAFLASH_BLOCKSIZE,
  /* space */
  FLASH_HIGH_SPACE,
  /* number */
  1U,
  },
};

/**
 * @brief   EEPROM configuration structure.
 */
EEPROMConfig eeprom_cfg = {
  /* the number of blocks used for emulation */
  2U,
  /* the block configuration array pointer   */
  blockconf,
};

/*===========================================================================*/
/* Driver local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/** @} */
