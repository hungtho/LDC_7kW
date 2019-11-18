/*
                             *******************
******************************* C HEADER FILE ********************************
**                           *******************                            **
**                                                                          **
**  Project   : KSC_LDC                                                     **
**  Filename  : LdConfig.h 					                                        **
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

#ifndef LDCCONFIG_INCLUDED
#define LDCCONFIG_INCLUDED

/****************************************************************************/
/**                                                                        **/
/**                      MODULES USED                                      **/
/**                                                                        **/
/****************************************************************************/
/* ASW */
#include "Rte.h"
#include "Aw_MatLib.h"
#include "Asw_Utility.h"
#include "Asw_FaultDetection.h"
#include "Asw_Pid.h"
#include "Asw_StateMachine.h"
#include "Asw_DtcMng.h"

/*BSW*/
#include "Bsw_Adc.h"
#include "Bsw_Can.h"
#include "Bsw_Pwm.h"
#include "Bsw_PIT.h"

/* RTE */
#include "Hal_PIT.h"
#include "Hal_Adc.h"
#include "Hal_Can.h"
#include "Hal_Pwm.h"
#include "components.h"

/* UDS */
#include "GpFunc.h"
#include "Tp_Func.h"
#include "Uds_Func.h"
#include "Defines.h"

/****************************************************************************/
/**                                                                        **/
/**                      DEFINITIONS AND MACROS                            **/
/**                                                                        **/
/****************************************************************************/
/*---------------------------------------------------------------------------/
  @ Configuration
----------------------------------------------------------------------------*/
#define ENABLE_NEW_V_IN_SENSOR
#define DISABLE_NEW_MOVE_AVERAGE

#define LDC_18FECA1A_ID		( 0x18FECA1A )
#define LDC_18FF041A_ID		( 0x18FF041A )
#define LDC_18FF31EF_ID		( 0x18FF31EF )
#define LDC_18FF0411_ID		( 0x18FF0411 )

#define LDC_STS_READY			( ( uint8_t )( u8g_Asw_Ldc_State_Ready & 0x01U ) )
#define LDC_STS_RUN				( ( uint8_t )( ( u8g_Asw_Ldc_State_Run & 0x01U ) << 1U ) )

#define MaxOutputVoltage	( 214.1 )	// This is scaled to 5V
#define MaxOutputCurrent	( 2492.0 )		// This is scaled to 5V
#define IoRef  						( CUR_REF / MaxOutputCurrent )  	// Base output voltage is taken as 14V and reference is scaled it down to 12V
#define IoRef_5  					( CUR_REF_95 / MaxOutputCurrent )  	// Base output voltage is taken as 14V and reference is scaled it down to 12V
#define IoRef_95  				( CUR_REF_95 / MaxOutputCurrent )  	// Base output voltage is taken as 14V and reference is scaled it down to 12V
#define IoRef_105  				( CUR_REF_105 / MaxOutputCurrent )  	// Base output voltage is taken as 14V and reference is scaled it down to 12V
#define IoRef_115  				( CUR_REF_115 / MaxOutputCurrent )  	// Base output voltage is taken as 14V and reference is scaled it down to 12V

/* DUTY  limit */
#define MaxPhase					(uint16_t)( ( ETIMER_CLOCK_FREQUENCY/PWM_FREQUENCY / 2U ) )

/****************** PI Controller defines - end *******************************/

#define ETIMER_CLOCK_FREQUENCY		( 80000000UL ) /* 80MHz */
#define DEADTIME_NS_TO_DUTY(NS)		(uint32_t)(( NS )*(((( float )PWM_DUTY_MAX)*(( float )(PWM_FREQUENCY)) / 1000000000.0 ) ) )

#define PWM_FREQUENCY							( 150000UL ) /* 150kHz */
#define PWM_DUTY_MAX							( ETIMER_CLOCK_FREQUENCY / PWM_FREQUENCY )
#define DEADTIME 									( 150U ) /* 200ns */

/*----------------------------------------------------*/
#define VOUT_FAULT_MAX		( 320U )
#define VOUT_FAULT_MIN	  ( 160U )
#define VOUT_WARNING_MAX	( 300U )
#define VOUT_WARNING_MIN	( 180U )

#define BATT_FAULT_MAX		( 320U )
#define BATT_FAULT_MIN	  ( 160U )
#define BATT_WARNING_MAX	( 300U )
#define BATT_WARNING_MIN	( 180U )

#define IOUT_ADC_OFFSET		( 409U )
#define IOUT_FLT_SHUTDWN	( 2100U )
#define IOUT_FAULT_MAX		( 1750U )
#define IOUT_FAULT_MIN 		( 30U )

#define VIN_FAULT_MAX	 		( 4210U )
#define VIN_FAULT_MIN			( 2450U )
#define VIN_WARNING_MAX	 	( 4150U )
#define VIN_WARNING_MIN		( 2700U )

#define TEMP_FAULT_MAX 		( 155U )		/* realTemp + 40 offset */
#define TEMP_NORMAL 			( 125U )		/* realTemp + 40 offset */
#define TEMP_FAULT_MIN  	( 10U )		/* realTemp + 40 offset */

#define CUR_REF		   			( 1600U )		/* Output Current Reference (A)*10 */
#define CUR_REF_95	 			( 1290U )			/* Output Current Reference (A)*10 */
#define CUR_REF_105	 			( 800U	)	/* Output Current Reference (A)*10 */
#define CUR_REF_115	 			( 00U )		/* Output Current Reference (A)*10 */

#define SENSOR_OPEN_LIMIT  ( 4000U	)	/* Output Current Reference (A)*10 */
#define SENSOR_SHORT_LIMIT ( 10U	)	/* Output Current Reference (A)*10 */

#define NEW_EEPROM 				( 1 )
#define TEST_VARIABLE			( 1 )

/****************************************************************************/
/**                                                                        **/
/**                      TYPEDEFS AND STRUCTURES                           **/
/**                                                                        **/
/****************************************************************************/
enum
{
	uFALSE = 0U,
	uTRUE = 1U,
};
/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED VARIABLES                                **/
/**                                                                        **/
/****************************************************************************/

/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED FUNCTIONS                                **/
/**                                                                        **/
/****************************************************************************/

#endif

/****************************************************************************/
/**                                                                        **/
/**                      EOF                                               **/
/**                                                                        **/
/****************************************************************************/
