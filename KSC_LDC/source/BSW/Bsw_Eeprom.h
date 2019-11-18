/*
                             *******************
******************************* C HEADER FILE ********************************
**                           *******************                            **
**                                                                          **
**  Project   : LDC_7kW                                                     **
**  Filename  : Bsw_Eeprom.h                                                **
**  Version   : -.- (PCB : )                                                **
**  Date      : 2019.11.15                                                  **
**                                                                          **
******************************************************************************
**                                                                          **
**  (c) 2019 YOUNG HWA TECH Co., Ltd. All Rights Reserved                   **
**                                                                          **
**                       YOUNG HWA TECH Co., Ltd.                           **
******************************************************************************

VERSION HISTORY:
----------------

Version     : -.-
Date        : ----.--.--
Revised by  : Tran Duc Hung
Description : Original version.

*/

#ifndef BSW_EEPROM_INCLUDED
#define BSW_EEPROM_INCLUDED

/****************************************************************************/
/**                                                                        **/
/**                      MODULES USED                                      **/
/**                                                                        **/
/****************************************************************************/
#include "LdcConfig.h"
#include "eeprom.h"
#include "eeprom_cfg.h"

/****************************************************************************/
/**                                                                        **/
/**                      DEFINITIONS AND MACROS                            **/
/**                                                                        **/
/****************************************************************************/

/****************************************************************************/
/**                                                                        **/
/**                      TYPEDEFS AND STRUCTURES                           **/
/**                                                                        **/
/****************************************************************************/

/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED VARIABLES                                **/
/**                                                                        **/
/****************************************************************************/

# ifndef BSW_EEPROM_C_SRC
# endif

/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED FUNCTIONS                                **/
/**                                                                        **/
/****************************************************************************/
extern void EEP_EEPROM_INIT(void);
extern void EEP_EEPROM_READ(void);

extern void EEP_EEPROM_WRITE( LdcDtcDef* eepData, uint32_t length);
extern void EEP_DTC_CLEAR(void);
extern uint8_t EEP_CMP_BUFFER(void* desBuffer, void* srcBuffer, uint32_t length);
extern uint8_t* EEP_GET_WRITE_BUFFER(void);

extern void EEP_UPDATE_EEPROM(LdcDtcDef* eepData, uint32_t length, uint8_t ldcStat);
extern void utils_memcpy( void *des, const void *src, uint32_t num );

#endif

/****************************************************************************/
/**                                                                        **/
/**                      EOF                                               **/
/**                                                                        **/
/****************************************************************************/
