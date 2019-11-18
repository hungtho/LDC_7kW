/*
                             *******************
******************************* C HEADER FILE ********************************
**                           *******************                            **
**                                                                          **
**  Project   : KSC_LDC                                                     **
**  Filename  : Asw_Utility.h                                               **
**  Version   : -.- (PCB : )                                                **
**  Date      : 2019.07.29                                                  **
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

#ifndef ASW_UTILITY_INCLUDED
#define ASW_UTILITY_INCLUDED

/****************************************************************************/
/**                                                                        **/
/**                      MODULES USED                                      **/
/**                                                                        **/
/****************************************************************************/
#include "LdcConfig.h"
#include "components.h"

/****************************************************************************/
/**                                                                        **/
/**                      DEFINITIONS AND MACROS                            **/
/**                                                                        **/
/****************************************************************************/
#define setReg32Bits( RegName, SetMask )		( RegName |= ( uint32_t )( SetMask ) )
#define clrReg32Bits( RegName, ClrMask )    ( RegName &= ( uint32_t )( ~ClrMask & 0xFFFFFFFFU ) )
#define clrSetReg32Bits( RegName, ClrMask, SetMask )    ( RegName = ( RegName & ( ( uint32_t )( ~ClrMask & 0xFFFFFFFFU ) ) ) | ( uint32_t )( SetMask ) )

#define DTC_HIGH 					( 0x59U )
#define DTC_MIDDLE				( 0x12U )
#define DTC_CODE_NUMBER		( 20U )

#define OPEN_LOOP					( 0U )

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

# ifndef ASW_UTILITY_C_SRC
# endif

extern const uint8_t u8g_Asw_DTC_CODE_Array[ DTC_CODE_NUMBER ];
extern uint32_t u32g_Asw_EEPROM_LAST_BUFFER_DTC[ 8 ];
extern uint32_t u32g_Asw_EEPROM_BUFFER_DTC[ 8 ];

extern uint32_t u32g_Asw_EEPROM_BUFFER_DID[ 8 ];
extern uint32_t u32g_Asw_EEPROM_LAST_BUFFER_DID[ 8 ];

extern uint8_t u8g_Asw_Ldc_State_Ready;
extern uint8_t u8g_Asw_Ldc_State_Run;
extern uint8_t u8g_Asw_LDC_ENB;

extern uint32_t u32g_Asw_IoReference;
extern uint32_t u32g_Asw_VoReference ;

extern uint8_t u8g_Asw_WriteEEPROM_Flag;
#if OPEN_LOOP
extern uint16_t Vcu_PhaseShift;
#endif

#if 0
extern LdcDiagKeyIn DiagKeyIn;
#endif

/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED FUNCTIONS                                **/
/**                                                                        **/
/****************************************************************************/
extern uint8_t Asw_GetIgnSts ( void );
extern void Asw_FindCurrentAdd ( void );
extern void Asw_DID_Eeprom_Read ( uint32_t addr );
extern void Asw_DID_Eeprom_Write ( void );
extern void Asw_DTC_Eeprom_Read ( uint32_t addr );
extern void Asw_DTC_Eeprom_Write ( void );
extern uint32_t UTILS_REF_INV ( uint16_t Ref, const uint16_t AdcValue[ 19 ], const uint16_t realValue[ 19 ] );

extern void Asw_IoRef_TemperDerating ( uint16_t realTemper );
extern uint16_t Asw_LDC_OFF ( uint16_t u16_pwmshift );

#endif

/****************************************************************************/
/**                                                                        **/
/**                      EOF                                               **/
/**                                                                        **/
/****************************************************************************/
