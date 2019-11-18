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
 * @file    eeprom_lld.h
 * @brief   Platform specific EEPROM emulation driver header.
 *
 * @addtogroup EEPROM
 * @{
 */

#ifndef _EEPROM_LLDH_
#define _EEPROM_LLDH_

#include "eeprom_lld_cfg.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/
/**
 * @name EEPROM emulation Driver low level driver macros
 * @{
 */
#define FLASH_HIGH_SPACE                0x01
/** @} */

/**
 * @name    flash errors definitions
 * @{
 */
#define FLASH_OK                        0x00000000UL
#define FLASH_ERROR_INIT                0x00000001UL
#define FLASH_ERROR_LOCK                0x00000002UL
#define FLASH_ERROR_ERASE               0x00000004UL
#define FLASH_ERROR_PROGRAM             0x00000008UL
#define FLASH_ERROR_NOT_BLANK           0x00000010UL
/** @} */

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
/**
 * @name EEPROM Emulation Driver macros
 * @{
 */
#define FLASHREAD64(x)  ((uint64_t)(*(vuint64_t*)(x)))
#define FLASHREAD32(x)  ((uint32_t)(*(vuint32_t*)(x)))
#define FLASHREAD16(x)  ((uint16_t)(*(vuint16_t*)(x)))
#define FLASHREAD8(x)   ((uint8_t)(*(vuint8_t*)(x)))
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  uint32_t eeprom_lld_flashinit(void);
  uint32_t eeprom_lld_unlock(uint8_t block, uint8_t flashspace);
  uint32_t eeprom_lld_flasherase(uint8_t block, uint8_t flashspace);
  uint32_t eeprom_lld_flashprogram64(uint32_t address, uint64_t buffer);
  uint32_t eeprom_lld_blankcheck(uint32_t address, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif /* _EEPROM_LLDH_ */

/** @} */
