/*
                             *******************
******************************* C HEADER FILE ********************************
**                           *******************                            **
**                                                                          **
**  Project   : KSC_LDC                                                     **
**  Filename  : HalPwm.h                                                    **
**  Version   : -.- (PCB : )                                                **
**  Date      : 2019.08.01                                                  **
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

#ifndef HALPWM_INCLUDED
#define HALPWM_INCLUDED

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
#define PWM_DUTY							( 50U )
#define PWM_DEAD_TIME					( 200U )	/* 200 ns */
#define ETIMER_3_PERIOD				( ( ( uint32_t )SPC5_ETIMER_CLK / PWM_FREQUENCY ) - 1U )
#define ETIMER_3_DUTY					( ( uint16_t )( ETIMER_3_PERIOD - ( ( ETIMER_3_PERIOD * PWM_DUTY ) / 100U ) ) )
#define ETIMER_3_DEAD_TIME		( ( uint16_t )( ( 2U * PWM_DEAD_TIME ) / 25U ) ) /* 1 Tick = 25ns */

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

# ifndef HALPWM_C_SRC
# endif

/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED FUNCTIONS                                **/
/**                                                                        **/
/****************************************************************************/
extern void Hal_PwmInit ( void );
extern void Hal_PwmSetShift( uint16_t u16_shift );
extern void Hal_PwmEnable ( void );
extern void Hal_PwmDisable ( void );
extern void SPC5_ETIMER3_STS3_HANDLER ( void );

#endif

/****************************************************************************/
/**                                                                        **/
/**                      EOF                                               **/
/**                                                                        **/
/****************************************************************************/
