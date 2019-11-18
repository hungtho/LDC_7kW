/****************************************************************************
*
* Copyright © 2015-2019 STMicroelectronics - All Rights Reserved
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
 * @file    eeprom_lld.c
 * @brief   Platform specific EEPROM emulation driver code.
 *
 * @addtogroup EEPROM
 * @{
 */

#include "eeprom_lld.h"

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief    Initialize flash driver.
 *
 * @return  FLASH_OK or FLASH_ERROR_INIT if operation fails
 *
 * @notapi
 */
uint32_t eeprom_lld_flashinit(void) {

  if (FlashInit(&ssdConfig) != C55_OK) {
    return FLASH_ERROR_INIT;
  }

  return FLASH_OK;
}

/**
 * @brief    Block Unlock.
 * @details  unlock block involved in eeprom emulation. after this operation will be
 *           possible to perform erase and program operation
 *
 * @param[in] block        Number of block to unlock
 * @param[in] flashspace   Block belonging space (LOW/MID/LARGE/HIGH)
 *
 * @return  FLASH_OK or FLASH_ERROR_LOCK if operation fails
 *
 * @notapi
 */

uint32_t eeprom_lld_unlock(uint8_t block, uint8_t flashspace) {
  /* current implementation will unlock all blocks in dataflash space
   * other options will be implemented in the future
   */
  (void)block;
  (void)flashspace;

  if (SetLock(&ssdConfig, C55_BLOCK_HIGH, UNLOCK_ALL_BLOCKS) != C55_OK) {
    return FLASH_ERROR_LOCK;
  }

  return FLASH_OK;
}

/**
 * @brief    Block Erase.
 * @details  Call flash driver block erase function to erase a block
 *
 * @param[in] block        Number of block to erase
 * @param[in] flashspace   Block belonging space (LOW/MID/LARGE/HIGH)
 *
 * @return  FLASH_OK or FLASH_ERROR_ERASE if operation fails
 *
 * @notapi
 */
uint32_t eeprom_lld_flasherase(uint8_t block, uint8_t flashspace) {

  uint32_t lowBlockSelect;
  uint32_t midBlockSelect;
  uint32_t highBlockSelect;
  uint32_t returnvalue;
  uint32_t result;
  NLARGE_BLOCK_SEL nLargeBlockSelect;
  CONTEXT_DATA dummyCtxData;

  /* current implementation will manage only blocks in dataflash space
   * other options will be implemented in the future
   */
  (void)flashspace;

  lowBlockSelect = 0UL;
  midBlockSelect = 0UL;
  highBlockSelect = (1UL << block);
  nLargeBlockSelect.firstLargeBlockSelect = 0UL;
  nLargeBlockSelect.secondLargeBlockSelect = 0UL;

  /* Erase the block */
  returnvalue = FlashErase(&ssdConfig, C55_ERASE_MAIN, lowBlockSelect, midBlockSelect, highBlockSelect, nLargeBlockSelect);
  if ( C55_OK != returnvalue) {
    return FLASH_ERROR_ERASE;
  }

  /* Call FlashCheckStatus() to check status of the progress */
  while (C55_INPROGRESS == FlashCheckStatus(&ssdConfig, C55_MODE_OP_ERASE, &result, &dummyCtxData)) {
  }
  if ( C55_OK != result) {
    return FLASH_ERROR_ERASE;
  }

  return FLASH_OK;

}

/**
 * @brief    Data Program.
 * @details  Call flash driver program function to write 64bit data
 *
 * @param[in] address    Address to program
 * @param[in] buffer     Data to program
 *
 * @return  FLASH_OK or FLASH_ERROR_PROGRAM if operation fails
 *
 * @notapi
 */
uint32_t eeprom_lld_flashprogram64(uint32_t address, uint64_t buffer) {
  uint32_t returnvalue;
  uint32_t bufferpointer;
  uint32_t result;
  CONTEXT_DATA pgmCtxData;

  bufferpointer = (uint32_t)(&buffer);

  returnvalue = FlashProgram(&ssdConfig, FALSE, address, 8U, bufferpointer, &pgmCtxData);
  if (C55_OK == returnvalue) {
    /* Call FlashCheckStatus() to check status of the progress */
    while (C55_INPROGRESS == FlashCheckStatus(&ssdConfig, C55_MODE_OP_PROGRAM, &result, &pgmCtxData)) {
    }

    if (C55_OK != result) {
      return FLASH_ERROR_PROGRAM;
    }

  }
  else {
    return FLASH_ERROR_PROGRAM;
  }

  (void)buffer;
  return FLASH_OK;
}

/**
 * @brief    Blank Check
 * @details  Check if a given address its not programmed for a given size
 *
 * @param[in] address    Address to check
 * @param[in] size     size to blank check
 *
 * @return  FLASH_OK or FLASH_ERROR_NOT_BLANK if space is not blank
 *
 * @notapi
 */
uint32_t eeprom_lld_blankcheck(uint32_t address, uint32_t size) {
  uint32_t returnvalue;
  uint32_t failAddress;
  uint32_t failData;
  CONTEXT_DATA bcCtxData;

  returnvalue = BlankCheck(&ssdConfig, address, size, &failAddress, &failData, &bcCtxData);

  if (C55_OK != returnvalue) {
    return FLASH_ERROR_NOT_BLANK;
  }

  return FLASH_OK;
}
/** @} */
