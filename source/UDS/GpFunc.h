/*
                             *******************
******************************* C HEADER FILE ********************************
**                           *******************                            **
**                                                                          **
**  Project   : KSC_FBL                                                     **
**  Filename  : GpFunc.h                                                    **
**  Version   : -.- (PCB : )                                                **
**  Date      : 2019.04.11                                                  **
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
Revised by  : Im, Hong Kyun
Description : Original version.

*/

#ifndef GPFUNC_INCLUDED
#define GPFUNC_INCLUDED

/****************************************************************************/
/**                                                                        **/
/**                      MODULES USED                                      **/
/**                                                                        **/
/****************************************************************************/
#include "LdcConfig.h"
#include "Defines.h"

/****************************************************************************/
/**                                                                        **/
/**                      DEFINITIONS AND MACROS                            **/
/**                                                                        **/
/****************************************************************************/
#define FLASH_LOW_SPACE			( 0x00U ) /* Code flash */
#define FLASH_128K_SPACE		( 0x01U )
#define FLASH_256K_SPACE		( 0x02U )
//#define FLASH_HIGH_SPACE		( 0x03U ) /* Data flash */
//
//#define FLASH_OK                ( 0x00U )
//#define FLASH_ERROR_INIT        ( 0x01U )
//#define FLASH_ERROR_LOCK        ( 0x02U )
//#define FLASH_ERROR_ERASE       ( 0x04U )
//#define FLASH_ERROR_PROGRAM     ( 0x08U )
//#define FLASH_ERROR_NOT_BLANK		( 0x10U )

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
extern uint32_t u32g_Asw_present_addr;
# ifndef GPFUNC_C_SRC
# endif

/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED FUNCTIONS                                **/
/**                                                                        **/
/****************************************************************************/
extern void Gp_Func_Init ( void );
extern void SetBootMode ( uint32_t u32_BootMode );
extern void UDS_WDT_PulseOut ( void );
extern uint32_t UDS_EEP_ReadDataByIdentifier ( uint32_t u32_Addr);
extern void UDS_EEP_WriteDataByIdentifier ( uint32_t *u32_dtcbuffer );

#endif

/****************************************************************************/
/**                                                                        **/
/**                      EOF                                               **/
/**                                                                        **/
/****************************************************************************/
