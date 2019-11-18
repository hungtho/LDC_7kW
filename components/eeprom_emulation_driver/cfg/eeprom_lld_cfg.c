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
 * @file    eeprom_lld_cfg.c
 * @brief   EEPROM Driver configuration code.
 *
 * @addtogroup EEPROM
 * @{
 */

#include "eeprom_lld_cfg.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/
SSD_CONFIG ssdConfig = {
  C55_REG_BASE, /* C55 control register base */
  MAIN_ARRAY_BASE, /* base of main array */
  {0, 0, 0, 0}, /* blocks info of low address space */
  {0, 0, 0, 0}, /* blocks info of mid address space */
  {0, 0, 0, 0}, /* blocks info of high address space */
  0, /* number of blocks in 256K address space */
  UTEST_ARRAY_BASE, /* base of UTEST array */
  TRUE, /* interface flag indicate main or alternate interface */
  C55_PROGRAMMABLE_SIZE,/* programmable size */
  FALSE /* debug mode selection */
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
