/*
                             *******************
******************************* C HEADER FILE ********************************
**                           *******************                            **
**                                                                          **
**  Project   : LDC_7kW                                                     **
**  Filename  : Bsw_Adc.h                                                   **
**  Version   : -.- (PCB : )                                                **
**  Date      : 2019.11.08                                                  **
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

#ifndef BSW_ADC_INCLUDED
#define BSW_ADC_INCLUDED

/****************************************************************************/
/**                                                                        **/
/**                      MODULES USED                                      **/
/**                                                                        **/
/****************************************************************************/
#include "LdcConfig.h"

/****************************************************************************/
/**                                                                        **/
/**                      DEFINITIONS AND MACROS                            **/
/**                                                                        **/
/****************************************************************************/
#define ADC_BUFFER_SIZE		( 7U )

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

# ifndef BSW_ADC_C_SRC
# endif
extern uint16_t u16_AdcVout[ 19 ];
extern uint16_t u16_Vout[ 19 ];
extern uint16_t u16_AdcIout[ 19 ];
extern uint16_t u16_Iout[ 19 ];

extern uint16_t u16g_Bsw_realVout;
extern uint16_t u16g_Bsw_realIout;
extern uint16_t u16g_Bsw_realVin;
extern uint16_t u16g_Bsw_realTemp;
extern uint16_t u16g_Bsw_realBatt;

extern volatile uint32_t u32g_Bsw_AdcVout_Q15;
extern volatile uint32_t u32g_Bsw_AdcIout_Q15;

/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED FUNCTIONS                                **/
/**                                                                        **/
/****************************************************************************/
extern void Bsw_AdcInit( void );
extern void Bsw_ADC2REAL ( void );
#endif

/****************************************************************************/
/**                                                                        **/
/**                      EOF                                               **/
/**                                                                        **/
/****************************************************************************/
