/*
                             *******************
******************************* C HEADER FILE ********************************
**                           *******************                            **
**                                                                          **
**  Project   : KSC_LDC                                                     **
**  Filename  : Asw_FaultDetection.h                                         **
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

#ifndef LDCFAULTDETECTION_INCLUDED
#define LDCFAULTDETECTION_INCLUDED

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
//#define Sens_PROTECT			( 1U )
//#define CAN_PROTECT				( 1U )
#define Vout_PROTECT			( 1U )
#define Temp_PROTECT			( 1U )
#define Iout_PROTECT			( 1U )
#define Vin_PROTECT		  	( 1U )
#define Bplus_PROTECT			( 1U )

#define CAN2_TIME_OUT_ERR					( ( uint32_t )( 0x00080000 ) )
#define CAN1_TIME_OUT_ERR					( ( uint32_t )( 0x00040000 ) )
#define OUTPUT_FAIL_ERR						( ( uint32_t )( 0x00020000 ) )
#define CAN2_BUS_OFF_ERR					( ( uint32_t )( 0x00010000 ) )
#define CAN1_BUS_OFF_ERR  				( ( uint32_t )( 0x00008000 ) )
#define CUR_SENSOR_SHORT_ERR			( ( uint32_t )( 0x00004000 ) )
#define CUR_SENSOR_OPEN_ERR				( ( uint32_t )( 0x00002000 ) )
#define TEMP_SENSOR_SHORT_ERR			( ( uint32_t )( 0x00001000 ) )
#define TEMP_SENSOR_OPEN_ERR			( ( uint32_t )( 0x00000800 ) )
#define TEMP_UNDER_ERR						( ( uint32_t )( 0x00000400 ) )
#define TEMP_OVER_ERR							( ( uint32_t )( 0x00000200 ) )
#define TEMP_OVER_85_ERR					( ( uint32_t )( 0x00000100 ) )
#define OUT_I_OVER_SHUTDOWN_ERR		( ( uint32_t )( 0x00000080 ) )
#define OUT_I_OVER_ERR						( ( uint32_t )( 0x00000040 ) )
#define OUT_V_UNDER_ERR						( ( uint32_t )( 0x00000020 ) )
#define OUT_V_OVER_ERR						( ( uint32_t )( 0x00000010 ) )
#define IN_V_UNDER_ERR						( ( uint32_t )( 0x00000008 ) )
#define IN_V_OVER_ERR							( ( uint32_t )( 0x00000004 ) )
#define BATT_V_UNDER_ERR					( ( uint32_t )( 0x00000002 ) )
#define BATT_V_OVER_ERR						( ( uint32_t )( 0x00000001 ) )

/****************************************************************************/
/**                                                                        **/
/**                      TYPEDEFS AND STRUCTURES                           **/
/**                                                                        **/
/****************************************************************************/
typedef struct
{
	uint32_t Fault_Max;
	uint32_t Fault_Min;
	uint32_t Warning_Max;
	uint32_t Warning_Min;
	uint16_t Err_Over_ShutdwnCnt;
	uint16_t Err_Over_Cnt;
	uint16_t Err_Under_Cnt;
	uint16_t Normal_Cnt;
	uint16_t Err_Cnt_Max;
	uint8_t Err_Over;
	uint8_t Err_Under;
	uint8_t Err_Shutdown;
	uint8_t Mode;
}Diagnostic;

typedef struct
{
	uint16_t Over_Level;
	uint16_t Under_Level;
	uint16_t Derating_Level;
	uint16_t Err_Cnt_Max;
	uint16_t Err_Over_Cnt;
	uint16_t Err_Under_Cnt;
	uint16_t Derating_Cnt;
	uint8_t Err_Over;
	uint8_t Err_Under;
	uint8_t Derating;
	uint8_t Mode;
}Diagnostic_Temp;

typedef struct
{
	uint16_t Short_Limit;
	uint16_t Open_Limit;
	uint16_t Err_Open_Cnt;
	uint16_t Err_Short_Cnt;
	uint16_t Err_Cnt_Max;
	uint8_t Err_Short;
	uint8_t Err_Open;
}Diagnostic_Sensor;

typedef struct
{
	uint16_t CAN1_Timeout;
	uint16_t CAN1_Busoff;
	uint16_t CAN2_Timeout;
	uint16_t CAN2_Busoff;
}Diagnostic_CAN;

typedef enum
{
	UNDER = 0U,
	NORMAL = 1U,
	OVER = 2U,
	DERATING = 3U,
	SHUTDOWN = 4
}Diagnostic_Level;
/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED VARIABLES                                **/
/**                                                                        **/
/****************************************************************************/

# ifndef LDCFAULTDETECTION_C_SRC
# endif

extern uint8_t u8g_Asw_LDC_SYS_ERR_BATTERY_DIS;
extern uint32_t u32g_LDC_ERR_Sts;

extern Diagnostic Dia_Vin;
extern Diagnostic Dia_Vout;
extern Diagnostic Dia_Batt;
extern Diagnostic Dia_Iout;

extern Diagnostic_Temp Dia_Temp;

extern Diagnostic_Sensor Dia_Temp_Sens;
extern Diagnostic_Sensor Dia_Iout_Sens;
extern Diagnostic_CAN Dia_CAN;

/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED FUNCTIONS                                **/
/**                                                                        **/
/****************************************************************************/
extern void Asw_Diagnostic_Fault( void );
extern void Asw_Diagnostic_Init ( void );
#endif

/****************************************************************************/
/**                                                                        **/
/**                      EOF                                               **/
/**                                                                        **/
/****************************************************************************/
