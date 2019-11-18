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
 * @file    eeprom.c
 * @brief   EEPROM emulation driver code.
 *
 * @addtogroup EEPROM
 * @{
 */

#include "eeprom.h"

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/
/**
 * @brief  EEPROM driver identifier
 */ 
EEPROMDriver EEPROMD;

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/
uint32_t eeprom_searchID(EEPROMDriver* eepromp, uint16_t id, uint32_t* address, uint16_t* size);
uint32_t eeprom_blockswap(EEPROMDriver* eepromp);

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   eeprom initialization.
 * @details Initialize EEPROMD driver structure by setting fields
 *          to its default status
 * @api
 */
void eeprom_init(void) {

  /* initialize driver global variables*/
  EEPROMD.config = NULL;
  EEPROMD.isInitialized = EEPROM_NOT_INITIALIZED;
  EEPROMD.currentBlock = EEPROM_NO_BLOCK;
  EEPROMD.freespace = 0UL;
}

/**
 * @brief    Start the eeprom emulation driver.
 * @details  Initialize flash driver and involved data flash blocks. then, if an eeprom
 *           is already configured, find the active block and scan it to retrieve valid
 *           records, otherwise erase all blocks involved and set the first to active state.
 *
 * @param[in] eepromp      pointer to a @p EEPROMDriver object
 * @param[in] config       pointer to eeprom configuration structure
 *
 * @return  EEPROM_OK or error code if operation fails
 *
 * @api
 */
uint32_t eeprom_start(EEPROMDriver* eepromp, EEPROMConfig* config) {
  uint32_t returnvalue;
  uint8_t counter;
  uint64_t blockstatus;
  uint64_t buffer;
  uint32_t address;
  uint32_t idsize;
  uint32_t actualsize;
  uint64_t idtype;
  uint32_t data;
  uint32_t blockaddress;
  uint32_t sizeaddress;

  /* Associate configuration to eeprom driver */
  eepromp->config = config;

  /* Flash initialization */
  returnvalue = eeprom_lld_flashinit();
  if (returnvalue != FLASH_OK) {
    return (EEPROM_ERROR_FLASH_INIT);
  }

  for (counter = 0; counter < eepromp->config->numberOfBlock; counter++) {
    /* Unlock flash blocks */
    returnvalue = eeprom_lld_unlock(eepromp->config->blockConfig[counter].number, eepromp->config->blockConfig[counter].space);
    if (returnvalue != FLASH_OK) {
      return (EEPROM_ERROR_FLASH_LOCK);
    }
    /* check if this block is active */
    blockstatus = FLASHREAD64(eepromp->config->blockConfig[counter].startAddr);
    if (blockstatus == EEPROM_BLOCK_ACTIVE) {
      eepromp->currentBlock = counter;
    }
  }

  /* check if an active block exist */
  if (eepromp->currentBlock == EEPROM_NO_BLOCK) {
    /* No Active block exist, delete all block used by eeprom driver
     * and init the first available block to be active */
    for (counter = 0; counter < eepromp->config->numberOfBlock; counter++) {
      returnvalue = eeprom_lld_flasherase(eepromp->config->blockConfig[counter].number, eepromp->config->blockConfig[counter].space);
      if (returnvalue != FLASH_OK) {
        return EEPROM_ERROR_FLASH_ERASE;
      }
    }
    /* init the first block by writing active status flag*/
    address = eepromp->config->blockConfig[0].startAddr;
    buffer = EEPROM_BLOCK_ACTIVE;
    returnvalue = eeprom_lld_flashprogram64(address, buffer);
    if (returnvalue != FLASH_OK) {
      return EEPROM_ERROR_FLASH_PROGRAM;
    }
    /* update eeprom active block to first block and update freespace*/
    eepromp->currentBlock = 0U;
    /* freespace when block is empty is block size - 16byte of metadata written (8byte block status + 8byte TDB) */
    eepromp->freespace = eepromp->config->blockConfig[0].size - 16UL;
    returnvalue = EEPROM_FIRST_INIT_OK;
  }
  /*
   * If block exist scan it to check
   */
  else
  {
   // address = eepromp->config->blockConfig[eepromp->currentBlock].startAddr;
    eepromp->freespace = eepromp->config->blockConfig[eepromp->currentBlock].size - 16UL;
    blockaddress = eepromp->config->blockConfig[eepromp->currentBlock].startAddr + 16U; /* start scan after blocks metadata */
    while (blockaddress < eepromp->config->blockConfig[eepromp->currentBlock].startAddr + eepromp->config->blockConfig[eepromp->currentBlock].size) {
      idtype = FLASHREAD64(blockaddress);
      sizeaddress = blockaddress + 8UL;
      data = FLASHREAD32(sizeaddress);
      idsize = data & 0x0000FFFFUL;
      actualsize = EEPROM_ROUND16((uint16_t )idsize);
      actualsize += 16U;
      switch (idtype) {
      case EEPROM_PROGRAM_COMPLETE:
      case EEPROM_ID_INVALID:
        eepromp->freespace -= actualsize;
        blockaddress = blockaddress + actualsize;
        break;
      case EEPROM_PROGRAM_START: /* power loss during write set sector as BAD */
    	returnvalue = eeprom_lld_flashprogram64(blockaddress, EEPROM_ID_BAD);
    	if (returnvalue != FLASH_OK) {
    	  return EEPROM_ERROR_FLASH_PROGRAM;
    	}
    	eepromp->freespace -= 16UL;
        blockaddress += 16UL;
    	break;
      case 0xFFFFFFFFFFFFFFFFUL: /* freespace*/
        blockaddress += 16UL;
        break;
      default: /* remnants of interrupted write operations*/
    	eepromp->freespace -= 16UL;
        blockaddress += 16UL;
        break;
      }
    }
    returnvalue = EEPROM_OK;
  }
  eepromp->isInitialized = EEPROM_INITIALIZED;
  return returnvalue;

}

/**
 * @brief    Remove emulated eeprom.
 * @details  Erase all blocks involved in the emulation and set driver structure
 *           field to its default values (no eeprom configured).
 * @note     All records written will be lost.
 *
 * @param[in] eepromp  pointer to a @p EEPROMDriver object
 *
 * @return  EEPROM_OK or error code if operation fails
 *
 * @api
 */
uint32_t eeprom_remove(EEPROMDriver* eepromp) {
  uint8_t counter;
  uint32_t returnvalue;

  /* check if eeprom driver is initialized */
  if (eepromp->isInitialized != EEPROM_INITIALIZED) {
    return EEPROM_ERROR_NOT_INITIALIZED;
  }

  /* delete all block used by eeprom driver */
  for (counter = 0; counter < eepromp->config->numberOfBlock; counter++) {
    returnvalue = eeprom_lld_flasherase(eepromp->config->blockConfig[counter].number, eepromp->config->blockConfig[counter].space);
    if (returnvalue != FLASH_OK) {
      return EEPROM_ERROR_FLASH_ERASE;
    }
  }
  EEPROMD.config = NULL;
  EEPROMD.isInitialized = EEPROM_NOT_INITIALIZED;
  EEPROMD.currentBlock = EEPROM_NO_BLOCK;
  return EEPROM_OK;
}

/**
 * @brief    Write a record in the emulated eeprom
 * @details  Write a record with a given ID of a specific length. If ID is already stored
 *           it will be marked as invalid and new record one will be written , while if no space
 *           available in the block, swap function will be called to free space from invalid ID
 *           and then record will be written.
 *
 * @param[in] eepromp      Pointer to a @p EEPROMDriver object
 * @param[in] id           ID of the record to write
 * @param[in] size         Record lenght
 * @param[in] source       Record data
 *
 * @return  EEPROM_OK or error code if operation fails
 *
 * @api
 */
uint32_t eeprom_write(EEPROMDriver* eepromp, uint16_t id, uint32_t size, uint32_t source) {
  uint32_t actualsize;
  uint16_t idsize;
  uint64_t buffer;
  uint32_t buffaddress;
  uint8_t counter;
  uint8_t i;
  uint32_t returnvalue;
  uint32_t address;
  uint32_t idaddress;
  uint32_t alignedbytes;
  uint32_t notalignedbytes;
  uint32_t nuomofcycles;
  uint32_t tempsource;
  uint8_t invalidateid;

  /* iF block is not present or eeprom is not initialized return error  */
  if (eepromp->currentBlock == EEPROM_NO_BLOCK || eepromp->isInitialized == EEPROM_NOT_INITIALIZED) {
    return EEPROM_ERROR_NOT_INITIALIZED;
  }

  tempsource = source;

  /* calculate space needed for this operation.it is 16byte of metadata and data will be rounded to 16byte */
  actualsize = EEPROM_ROUND16(size);
  actualsize += 16U;

  /* check if there is enough space in the current block */
  if (eepromp->freespace < actualsize) {
    returnvalue = eeprom_blockswap(eepromp);
    if (returnvalue != EEPROM_OK) {
      return EEPROM_ERROR_SWAP;
    }
    /* After the swap, check if there is space. If not return error */
    if (eepromp->freespace < actualsize) {
      return EEPROM_ERROR_NO_SPACE;
    }
  }

  /* check if ID is already written in flash. if yes will
   * be invalidated after the write operation */
  invalidateid = FALSE;
  if (eeprom_searchID(eepromp, id, &idaddress, &idsize) == EEPROM_OK) {
    invalidateid = TRUE;
  }

  /* operations performed will be the following:
   * 1- write program start tag       (offset 0x00)
   * 2- write ID and size             (offset 0x08)
   * 3- write datas                   (offset 0x10)
   * 4- write program complete tag    (offset 0x00)
   */

  /* program start (offset 0x00)*/
  address = eepromp->config->blockConfig[eepromp->currentBlock].startAddr + eepromp->config->blockConfig[eepromp->currentBlock].size
      - eepromp->freespace;
  returnvalue = eeprom_lld_flashprogram64(address, EEPROM_PROGRAM_START);
  if (returnvalue != FLASH_OK) {
    return EEPROM_ERROR_FLASH_PROGRAM;
  }

  /* write ID and SIZE (offset 0x08) */
  address += 8U;
  buffer = 0x00000000FFFFFFFFUL;
  buffer = buffer | ((uint64_t)id << 48);
  buffer = buffer | ((uint64_t)size << 32);
  returnvalue = eeprom_lld_flashprogram64(address, buffer);
  if (returnvalue != FLASH_OK) {
    return EEPROM_ERROR_FLASH_PROGRAM;
  }

  /* write data (offset 0x10)*/
  alignedbytes = (size / 8U) * 8U;
  nuomofcycles = alignedbytes / 8U;
  notalignedbytes = size % 8U;
  for (counter = 0; counter < nuomofcycles; counter++) {
    address += 8U;
    buffer = 0xFFFFFFFFFFFFFFFFUL;
    for (i = 0; i < 8U; i++) {
      // buffer |= (uint64_t)(*(uint8_t*)source++)<<(i*8);
      buffaddress = ((uint32_t)(&buffer) + (uint32_t)i);
      EEPROM_WRITE8(buffaddress, EEPROM_READ8(source++));
    }
    returnvalue = eeprom_lld_flashprogram64(address, buffer);
    if (returnvalue != FLASH_OK) {
      return EEPROM_ERROR_FLASH_PROGRAM;
    }
  }
  if (notalignedbytes != 0UL) {
    /* write not aligned to 8bytes data */
    address += 8U;
    buffer = 0xFFFFFFFFFFFFFFFFUL;
    for (i = 0; i < notalignedbytes; i++) {
      buffaddress =  ((uint32_t)(&buffer) + (uint32_t)i);
      EEPROM_WRITE8(buffaddress, EEPROM_READ8(source++));
    }
    returnvalue = eeprom_lld_flashprogram64(address, buffer);
    if (returnvalue != FLASH_OK) {
      return EEPROM_ERROR_FLASH_PROGRAM;
    }
  }
  /* Verify written data */
  address = eepromp->config->blockConfig[eepromp->currentBlock].startAddr + eepromp->config->blockConfig[eepromp->currentBlock].size
      - eepromp->freespace;
  address += 16U; /*point to data */
  for (counter = 0; counter < size; counter++) {
    if (FLASHREAD8(address) != EEPROM_READ8(tempsource)) {
      return EEPROM_ERROR_FLASH_PROGRAM;
    }
    tempsource++;
    address++;
  }

  /* program complete  (offset 0x00)*/
  address = eepromp->config->blockConfig[eepromp->currentBlock].startAddr + eepromp->config->blockConfig[eepromp->currentBlock].size
      - eepromp->freespace;
  returnvalue = eeprom_lld_flashprogram64(address, EEPROM_PROGRAM_COMPLETE);
  if (returnvalue != FLASH_OK) {
    return EEPROM_ERROR_FLASH_PROGRAM;
  }

  /*A new id is written. Invalidate the old one */
  if (invalidateid == TRUE) {
    returnvalue = eeprom_lld_flashprogram64(idaddress, (uint64_t)EEPROM_ID_INVALID);
    if (returnvalue != FLASH_OK) {
      return EEPROM_ERROR_FLASH_PROGRAM;
    }
  }
  /* update info */
  eepromp->freespace -= actualsize;
  return EEPROM_OK;

}

/**
 * @brief    Search an ID inside emulated eeprom
 * @details  Check if a valid record with a given ID is stored in the emulated eeprom
 *
 * @param[in]  eepromp    Pointer to a @p EEPROMDriver object
 * @param[in]  id         ID of the record to search
 * @param[out] address    Phisical address in the data flash where ID is stored
 * @param[out] size       Size of the stored record
 *
 * @return  EEPROM_OK if record ID is found or EEPROM_ERROR_ID_NOT_FOUND otherwise
 *
 * @notapi
 */
uint32_t eeprom_searchID(EEPROMDriver* eepromp, uint16_t id, uint32_t* address, uint16_t* size) {
  uint16_t idnum;
  uint32_t idsize;
  uint32_t actualsize;
  uint64_t idtype;
  uint32_t data;
  uint32_t blockaddress;
  uint32_t sizeaddress;

  blockaddress = eepromp->config->blockConfig[eepromp->currentBlock].startAddr + 16U; /* start scan for ID after blocks metadata */

  /* scan block to search for ID */
  while (blockaddress < eepromp->config->blockConfig[eepromp->currentBlock].startAddr + eepromp->config->blockConfig[eepromp->currentBlock].size) {
    idtype = FLASHREAD64(blockaddress);
    sizeaddress = blockaddress + 8UL;
    data = FLASHREAD32(sizeaddress);
    idnum = (uint16_t)(data >> 16);
    idsize = data & 0x0000FFFFUL;
    actualsize = EEPROM_ROUND16(idsize);
    actualsize += 16U;

    if (idtype == EEPROM_PROGRAM_COMPLETE) {
      if (idnum == id) {
        *address = blockaddress;
        *size = (uint16_t)idsize;
        return EEPROM_OK;
      }
      else {
        blockaddress = blockaddress + actualsize;
      }
    }
    else if (idtype == EEPROM_ID_INVALID) {
      blockaddress = blockaddress + actualsize;
    }
    else {
      blockaddress += 16UL;
    }
  }

  return EEPROM_ERROR_ID_NOT_FOUND;
}

/**
 * @brief    Read a record in the emulated eeprom
 * @details  Search a given ID inside the emulated eeprom and, if found, read record data
 *
 * @param[in] eepromp      Pointer to a @p EEPROMDriver object
 * @param[in] id           ID of the record to read
 * @param[out] size        Record lenght
 * @param[out] source      address of record data buffer
 *
 * @return  EEPROM_OK or EEPROM_ERROR_ID_NOT_FOUND if ID doesn't exist
 *
 * @api
 */
uint32_t eeprom_read(EEPROMDriver* eepromp, uint16_t id, uint32_t* size, uint32_t source) {
  uint16_t idsize;
  uint16_t counter;
  uint32_t idaddress;
  uint32_t address;
  uint32_t buffaddress;


  /* iF block is not present or eeprom is not initialized return error  */
  if (eepromp->currentBlock == EEPROM_NO_BLOCK || eepromp->isInitialized == EEPROM_NOT_INITIALIZED) {
    return EEPROM_ERROR_NOT_INITIALIZED;
  }

  if (eeprom_searchID(eepromp, id, &idaddress, &idsize) != EEPROM_OK) {
    return EEPROM_ERROR_ID_NOT_FOUND;
  }

  address = idaddress + 16U;
  *size = idsize;
  for (counter = 0; counter < idsize; counter++) {
	buffaddress = (source + counter);
    EEPROM_WRITE8(buffaddress, FLASHREAD8(address++));
  }

  return EEPROM_OK;
}

/**
 * @brief    Swap blocks.
 * @details  when there is not enough space inside a physical block, swap operation allows to
 *           free space by deleting invalid records. this operation is achieved in this way:
 *           - search valid ID in the active block and move to  swap block
 *           - invalidate active block
 *           - set swap block as the new active block
 *
 * @param[in] eepromp      pointer to a @p EEPROMDriver object
 *
 * @return  EEPROM_OK or error code if operation fails
 *
 * @notapi
 */
uint32_t eeprom_blockswap(EEPROMDriver* eepromp) {
  uint8_t block;
  uint32_t returnvalue;
  uint8_t eraseblock;
  uint32_t address;
  uint32_t daddress;
  uint32_t idsize;
  uint32_t actualsize;
  uint64_t idtype;
  uint32_t data;
  uint32_t counter;
  uint32_t numcycles;
  uint64_t buffer;
  uint32_t i;
  uint32_t sizeaddress;
  uint32_t buffaddress;

  /* Select new block in the configuration structure */
  block = (eepromp->currentBlock + 1U) % (eepromp->config->numberOfBlock);

  /* check if new block is blank otherwise erase it */
  eraseblock = FALSE;
  returnvalue = eeprom_lld_blankcheck(eepromp->config->blockConfig[block].startAddr, eepromp->config->blockConfig[block].size);
  if (returnvalue != FLASH_OK) {
    eraseblock = TRUE;
  }
  if (eraseblock == TRUE) {
    returnvalue = eeprom_lld_flasherase(eepromp->config->blockConfig[block].number, eepromp->config->blockConfig[block].space);
    if (returnvalue != FLASH_OK) {
      return EEPROM_ERROR_FLASH_ERASE;
    }
  }

  /*
   * Start swap operation
   */
  /* write copy start flag to new block */
  returnvalue = eeprom_lld_flashprogram64(eepromp->config->blockConfig[block].startAddr, (uint64_t)EEPROM_SWAP_START);
  if (returnvalue != FLASH_OK) {
    return EEPROM_ERROR_FLASH_PROGRAM;
  }
  /* search and copy valid id */
  address = eepromp->config->blockConfig[eepromp->currentBlock].startAddr + 16UL;
  daddress = eepromp->config->blockConfig[block].startAddr + 16UL;
  while (address < eepromp->config->blockConfig[eepromp->currentBlock].startAddr + eepromp->config->blockConfig[eepromp->currentBlock].size) {
    idtype = FLASHREAD64(address);
    sizeaddress = address + 8UL;
    data = FLASHREAD32(sizeaddress);
    idsize = data & 0x0000FFFFUL;
    actualsize = EEPROM_ROUND16(idsize);
    actualsize += 16U;
    switch (idtype) {
    case EEPROM_PROGRAM_COMPLETE:
      //idnum = (uint16_t)(data >> 16);
      numcycles = actualsize / 8U;
      /* move valid data to new block */
      for (counter = 0; counter < numcycles; counter++) {
        buffer = 0xFFFFFFFFFFFFFFFFUL;
        for (i = 0; i < 8U; i++) {
          // buffer |= (uint64_t)(*(uint8_t*)source++)<<(i*8);
          buffaddress = ((uint32_t)(&buffer) + i);
          EEPROM_WRITE8(buffaddress, EEPROM_READ8(address++));
        }
        returnvalue = eeprom_lld_flashprogram64(daddress, buffer);
        if (returnvalue != FLASH_OK) {
          return EEPROM_ERROR_FLASH_PROGRAM;
        }
        daddress = daddress + 8U;
      }
      break;
    case EEPROM_ID_INVALID:
      address = address + actualsize;
      break;
    case EEPROM_ID_BAD:
      address += 16UL;
      break;
    default:
      address += 16UL;
      break;
    }
  }

  /*invalidate old block */
  returnvalue = eeprom_lld_flashprogram64(eepromp->config->blockConfig[eepromp->currentBlock].startAddr, (uint64_t)EEPROM_BLOCK_INVALID);
  if (returnvalue != FLASH_OK) {
    return EEPROM_ERROR_FLASH_PROGRAM;
  }

  /* set new block as active */
  returnvalue = eeprom_lld_flashprogram64(eepromp->config->blockConfig[block].startAddr, (uint64_t)EEPROM_BLOCK_ACTIVE);
  if (returnvalue != FLASH_OK) {
    return EEPROM_ERROR_FLASH_PROGRAM;
  }

  /* update information */
  eepromp->currentBlock = block;
  eepromp->freespace = eepromp->config->blockConfig[eepromp->currentBlock].size
      - (daddress - eepromp->config->blockConfig[eepromp->currentBlock].startAddr);
  return EEPROM_OK;

}

/**
 * @brief    Delete a record
 * @details  Check if record with a given ID is stored in the emulated eeprom
 *           and mark as invalid
 *
 * @param[in]  eepromp    Pointer to a @p EEPROMDriver object
 * @param[in]  id         ID of the record to delete
 *
 * @return  EEPROM_OK if record ID is correctly deleted or error code otherwise
 *
 * @notapi
 */
uint32_t eeprom_delete(EEPROMDriver* eepromp, uint16_t id) {
  uint32_t returnvalue;
  uint32_t idaddress;
  uint16_t idsize;

  /* if block is not present or eeprom is not initialized return error  */
  if (eepromp->currentBlock == EEPROM_NO_BLOCK || eepromp->isInitialized == EEPROM_NOT_INITIALIZED) {
    return EEPROM_ERROR_NOT_INITIALIZED;
  }

  /* Check if record is present */
  if (eeprom_searchID(eepromp, id, &idaddress, &idsize) != EEPROM_OK) {
    return EEPROM_ERROR_ID_NOT_FOUND;
  }

  /* invalidate record */
  returnvalue = eeprom_lld_flashprogram64(idaddress, (uint64_t)EEPROM_ID_INVALID);
  if (returnvalue != FLASH_OK) {
    return EEPROM_ERROR_FLASH_PROGRAM;
  }

  return EEPROM_OK;
}

/** @} */
