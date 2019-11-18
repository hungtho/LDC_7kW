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
 * @file    eeprom.h
 * @brief   EEPROM emulation driver header.
 *
 * @addtogroup EEPROM
 * @{
 */

#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "eeprom_lld.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/
/**
 * @name    EEPROM Emulation Driver state definitions
 * @{
 */
#define EEPROM_NOT_INITIALIZED           0x00U
#define EEPROM_INITIALIZED               0x01U
#define EEPROM_NO_BLOCK                  0xFFU
/** @} */

/**
 * @name    EEPROM Emulation Driver errors definitions
 * @{
 */
#define EEPROM_OK                        0x00000000UL
#define EEPROM_FIRST_INIT_OK             0x80000000UL
#define EEPROM_ERROR_FLASH_INIT          0x00000001UL
#define EEPROM_ERROR_FLASH_LOCK          0x00000002UL
#define EEPROM_ERROR_FLASH_ERASE         0x00000004UL
#define EEPROM_ERROR_FLASH_PROGRAM       0x00000008UL
#define EEPROM_ERROR_NOT_INITIALIZED     0x00000010UL
#define EEPROM_ERROR_ID_NOT_FOUND        0x00000020UL
#define EEPROM_ERROR_NO_SPACE            0x00000040UL
#define EEPROM_ERROR_SWAP                0x00000020UL
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

/**
 * @brief   Block configuration structure.
 */
typedef struct
{
  /**
   * @brief  Block start address
   */
  uint32_t            startAddr;         
  /**
   * @brief  Block size
   */
  uint32_t            size;              
  /**
   * @brief  The space (low, middle or high) for the block
   */
  uint8_t             space;
   /**
   * @brief  Block number in the given space
   */ 
  uint8_t             number;            
}BLOCKConfig;

/**
 * @brief   EEPROM configuration structure.
 */
typedef struct {
  /**
   * @brief  Number of Blocks used for EEPROM emulation
   */
  uint8_t             numberOfBlock;
  /**
   * @brief  Block configuration array pointer
   */
  BLOCKConfig*        blockConfig;       
} EEPROMConfig;

/**
 * @brief  EEPROM driver structure.
 */ 
typedef struct {
  /**
   * @brief EEPROM configuration
   */
  EEPROMConfig        *config;
  /**
   * @brief  Current active block
   */
  uint8_t             currentBlock;
  /**
   * @brief  Flag indicating if the EEPROM is initialized
   */
  uint8_t             isInitialized;      /*eeprom is initialized */
  /**
   * @brief  Free space in the active block
   */
  uint32_t            freespace;          
} EEPROMDriver;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/
/**
 * @name EEPROM Emulation Driver macros
 * @{
 */
#define EEPROM_ROUND16(x)             ((uint32_t)x%16UL==0UL? (uint32_t)x:(((uint32_t)x/16UL)+1UL)*16UL)
#define EEPROM_READ8(x)               ((vuint8_t)(*(vuint8_t*)(x)))
#define EEPROM_WRITE8(address, value) (*(vuint8_t*)(address) = (value))
#define EEPROM_WRITE64(address, value) (*(vuint64_t*)(address) = (value))
/** @} */
/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

extern EEPROMDriver EEPROMD;

#ifdef __cplusplus
extern "C" {
#endif
  void     eeprom_init  (void);
  uint32_t eeprom_start (EEPROMDriver* eepromp, EEPROMConfig* config);
  uint32_t eeprom_remove(EEPROMDriver* eepromp);
  uint32_t eeprom_write (EEPROMDriver* eepromp, uint16_t id, uint32_t size, uint32_t source);
  uint32_t eeprom_read  (EEPROMDriver* eepromp,uint16_t id, uint32_t* size, uint32_t source);
  uint32_t eeprom_delete(EEPROMDriver* eepromp, uint16_t id);

#ifdef __cplusplus
}
#endif

#endif /* _EEPROM_H_ */

/** @} */
