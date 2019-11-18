/*
                             *******************
******************************* C SOURCE FILE ********************************
**                           *******************                            **
**                                                                          **
**  Project   : KSC_LDC                                                     **
**  Filename  : Asw_FaultDetection.c                                         **
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

#define LDCFAULTDETECTION_C_SRC

/****************************************************************************/
/**                                                                        **/
/**                      MODULES USED                                      **/
/**                                                                        **/
/****************************************************************************/
#include "Asw_FaultDetection.h"

/****************************************************************************/
/**                                                                        **/
/**                      DEFINITIONS AND MACROS                            **/
/**                                                                        **/
/****************************************************************************/
#define INPUT_V_ERR_CNT					( 30U )
#define OUTPUT_V_ERR_CNT				( 30U )
#define BATT_ERR_CNT						( 30U )
#define TEMPER_ERR_CNT					( 50U )
#define OUTPUT_I_ERR_CNT				( 30U )
#define SENSOR_ERR_CNT					( 50U )
#define BATT_DIS_ERR_CNT				( 50U )
#define CAN_BUS_OFF_ERR_CNT			( 50U )

/****************************************************************************/
/**                                                                        **/
/**                      TYPEDEFS AND STRUCTURES                           **/
/**                                                                        **/
/****************************************************************************/

/****************************************************************************/
/**                                                                        **/
/**                      PROTOTYPES OF LOCAL FUNCTIONS                     **/
/**                                                                        **/
/****************************************************************************/
static void Asw_Diagnostic_InputV ( uint16_t u16_realVin, Diagnostic *pDiag );
static void Asw_Diagnostic_OutputV ( uint16_t u16_realVout, Diagnostic *pDiag );
static void Asw_Diagnostic_BattV ( uint16_t u16_realBplus, Diagnostic *pDiag );
static void Asw_Diagnostic_OuputI ( uint16_t u16_realIout, Diagnostic *pDiag );
static void Asw_Diagnostic_Temper ( uint16_t u16_realTemp, Diagnostic_Temp *pDiag );

static void Asw_Diagnostic_Temp_UnderMode( uint16_t u16_realTemp, Diagnostic_Temp *pDiag );
static void Asw_Diagnostic_Temp_NormalMode( uint16_t u16_realTemp, Diagnostic_Temp *pDiag );
static void Asw_Diagnostic_Temp_DeratingMode( uint16_t u16_realTemp, Diagnostic_Temp *pDiag );
static void Asw_Diagnostic_Temp_OverMode( uint16_t u16_realTemp, Diagnostic_Temp *pDiag );

static void Asw_Diagnostic_IoutSensor( uint16_t u16_Averadc_Iout );
static void Asw_Diagnostic_TempSensor ( uint16_t u16_Averadc_Temp );
static void Asw_Diagnostic_Batt_DisConnection ( uint16_t u16_realVout );

static void Asw_Diagnostic_CAN1_Timeout ( uint8_t u8_canTimeoutStatus, Diagnostic_CAN *pDiag );
static void Asw_Diagnostic_CAN2_Timeout ( uint8_t u8_canTimeoutStatus, Diagnostic_CAN *pDiag );
static void Asw_Diagnostic_CAN1_Busoff ( uint8_t u8_busOffStatus, Diagnostic_CAN *pDiag );
static void Asw_Diagnostic_CAN2_Busoff( uint8_t u8_busOffStatus, Diagnostic_CAN *pDiag );

static void Asw_Diagnostic_OverMode ( uint16_t u16_real, Diagnostic *pDiag );
static void Asw_Diagnostic_NormalMode ( uint16_t u16_real, Diagnostic *pDiag );
static void Asw_Diagnostic_UnderMode ( uint16_t u16_real, Diagnostic *pDiag );

static uint8_t Asw_Diagnostic_IsIoutOver ( uint16_t u16_real, Diagnostic *pDiag );
static uint8_t Asw_Diagnostic_IsIoutOver_ShutDwn ( uint16_t u16_real, Diagnostic *pDiag );

static uint8_t Asw_Diagnostic_IsSensorShortCircuit ( uint16_t u16_adc, Diagnostic_Sensor *pDiag );
static uint8_t Asw_Diagnostic_IsSensorOpenCircuit ( uint16_t u16_adc, Diagnostic_Sensor *pDiag );

/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED VARIABLES                                **/
/**                                                                        **/
/****************************************************************************/
uint8_t u8g_Asw_LDC_SYS_ERR_BATTERY_DIS = 0U;
uint32_t u32g_LDC_ERR_Sts;

/****************************************************************************/
/**                                                                        **/
/**                      LOCAL VARIABLES                                   **/
/**                                                                        **/
/****************************************************************************/
Diagnostic Dia_Vin;
Diagnostic Dia_Vout;
Diagnostic Dia_Batt;
Diagnostic Dia_Iout;

Diagnostic_Temp Dia_Temp;

Diagnostic_Sensor Dia_Temp_Sens;
Diagnostic_Sensor Dia_Iout_Sens;
Diagnostic_CAN Dia_CAN;

/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED FUNCTIONS                                **/
/**                                                                        **/
/****************************************************************************/
/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void Asw_Diagnostic_Init ( void )
{
	Dia_Vin.Err_Cnt_Max = INPUT_V_ERR_CNT;
	Dia_Vin.Fault_Max = VIN_FAULT_MAX;
	Dia_Vin.Fault_Min = VIN_FAULT_MIN;
	Dia_Vin.Warning_Max = VIN_WARNING_MAX;
	Dia_Vin.Warning_Min = VIN_WARNING_MIN;

	Dia_Vout.Err_Cnt_Max = OUTPUT_V_ERR_CNT;
	Dia_Vout.Fault_Max = VOUT_FAULT_MAX;
	Dia_Vout.Fault_Min = VOUT_FAULT_MIN;
	Dia_Vout.Warning_Max = VOUT_WARNING_MAX;
	Dia_Vout.Warning_Min = VOUT_WARNING_MIN;

	Dia_Batt.Err_Cnt_Max = BATT_ERR_CNT;
	Dia_Batt.Fault_Max = BATT_FAULT_MAX;
	Dia_Batt.Fault_Min = BATT_FAULT_MIN;
	Dia_Batt.Warning_Max = BATT_WARNING_MAX;
	Dia_Batt.Warning_Min = BATT_WARNING_MIN;

	Dia_Iout.Err_Cnt_Max = BATT_ERR_CNT;
	Dia_Iout.Fault_Max = IOUT_FLT_SHUTDWN;
	Dia_Iout.Fault_Min = IOUT_FAULT_MIN;

	Dia_Temp.Over_Level = TEMP_FAULT_MAX;
	Dia_Temp.Derating_Level = TEMP_NORMAL;
	Dia_Temp.Under_Level = TEMP_FAULT_MIN;
	Dia_Temp.Err_Cnt_Max = TEMPER_ERR_CNT;

	Dia_Temp_Sens.Open_Limit = SENSOR_OPEN_LIMIT;
	Dia_Temp_Sens.Short_Limit = SENSOR_SHORT_LIMIT;
	Dia_Temp_Sens.Err_Cnt_Max = SENSOR_ERR_CNT;

	Dia_Iout_Sens.Open_Limit = SENSOR_OPEN_LIMIT;
	Dia_Iout_Sens.Short_Limit = SENSOR_SHORT_LIMIT;
	Dia_Iout_Sens.Err_Cnt_Max = SENSOR_ERR_CNT;
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void Asw_Diagnostic_Fault ( void )
{
#if Vin_PROTECT
	if( ( eng_LDC_Sts == en_LDC_RunCheck )
   || ( eng_LDC_Sts == en_LDC_Run )
	 || ( eng_LDC_Sts == en_LDC_RecoveryFault ) )
	{
		Asw_Diagnostic_InputV( u16g_Bsw_realVin, &Dia_Vin  );
	}
#endif

#if Vout_PROTECT
	Asw_Diagnostic_OutputV( u16g_Bsw_realVout, &Dia_Vout );
#endif

#if Bplus_PROTECT
	Asw_Diagnostic_BattV( u16g_Bsw_realBatt, &Dia_Batt);
#endif

#if Temp_PROTECT
	Asw_Diagnostic_Temper( u16g_Bsw_realTemp, &Dia_Temp );
#endif

#if Iout_PROTECT
	Asw_Diagnostic_OuputI( u16g_Bsw_realIout, &Dia_Iout );
#endif

#if Sens_PROTECT
	Asw_Diagnostic_IoutSensor( u16g_Hal_AverAdc_Iout, &Dia_Iout_Sens );
	Asw_Diagnostic_TempSensor( u16g_Hal_AverAdc_Temp, &Dia_Temp_Sens );
	Asw_Diagnostic_Batt_DisConnection( u16g_Bsw_realVout );
#endif

#if CAN_PROTECT
	Asw_Diagnostic_CAN1_Busoff( u8g_can1BusOffFlag, &Dia_CAN );
	Asw_Diagnostic_CAN1_Timeout( u8g_Bsw_can1ErrStatus, &Dia_CAN );
	Asw_Diagnostic_CAN2_Busoff( u8g_can2BusOffFlag, &Dia_CAN );
	Asw_Diagnostic_CAN2_Timeout( u8g_Bsw_can2ErrStatus, &Dia_CAN );
#endif
}

/****************************************************************************/
/**                                                                        **/
/**                      LOCAL FUNCTIONS                                   **/
/**                                                                        **/
/****************************************************************************/
/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Asw_Diagnostic_InputV ( uint16_t u16_realVin, Diagnostic *pDiag )
{
	switch ( pDiag->Mode )
	{
	case UNDER:
		Asw_Diagnostic_UnderMode( u16_realVin, pDiag );

		if( pDiag->Err_Under == 1U ){
			clrSetReg32Bits( ( u32g_LDC_ERR_Sts ), ( IN_V_OVER_ERR ) , ( IN_V_UNDER_ERR ) );
		}
		break;
	case NORMAL:
		Asw_Diagnostic_NormalMode( u16_realVin, pDiag);

		clrReg32Bits( ( u32g_LDC_ERR_Sts ), ( IN_V_OVER_ERR | IN_V_UNDER_ERR ) );
		break;
	case OVER:
		Asw_Diagnostic_OverMode( u16_realVin, pDiag );

		if( pDiag->Err_Over == 1U ){
			clrSetReg32Bits( ( u32g_LDC_ERR_Sts ), ( IN_V_OVER_ERR ) , ( IN_V_UNDER_ERR ) );
		}
		break;
	default:
		break;
	}
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Asw_Diagnostic_OutputV( uint16_t u16_realVout, Diagnostic *pDiag )
{
	switch ( pDiag->Mode )
	{
	case UNDER:
		Asw_Diagnostic_UnderMode( u16_realVout, pDiag );

		if( pDiag->Err_Under == 1U ){
			clrSetReg32Bits( ( u32g_LDC_ERR_Sts ), ( OUT_V_OVER_ERR ) , ( OUT_V_UNDER_ERR ) );
		}
		break;
	case NORMAL:
		Asw_Diagnostic_NormalMode( u16_realVout, pDiag);

		clrReg32Bits( ( u32g_LDC_ERR_Sts ), ( OUT_V_OVER_ERR | OUT_V_UNDER_ERR ) );
		break;
	case OVER:
		Asw_Diagnostic_OverMode( u16_realVout, pDiag );

		if( pDiag->Err_Over == 1U ){
			clrSetReg32Bits( ( u32g_LDC_ERR_Sts ), ( OUT_V_UNDER_ERR ) , ( OUT_V_OVER_ERR ) );
		}
		break;
	default:
		break;
	}
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Asw_Diagnostic_BattV( uint16_t u16_realBatt, Diagnostic *pDiag )
{
	switch ( pDiag->Mode )
	{
	case UNDER:
		Asw_Diagnostic_UnderMode( u16_realBatt, pDiag );

		if( pDiag->Err_Under == 1U ){
			clrSetReg32Bits( ( u32g_LDC_ERR_Sts ), ( BATT_V_OVER_ERR ) , ( BATT_V_UNDER_ERR ) );
		}
		break;
	case NORMAL:
		Asw_Diagnostic_NormalMode( u16_realBatt, pDiag);

		clrReg32Bits( ( u32g_LDC_ERR_Sts ), ( BATT_V_OVER_ERR | BATT_V_UNDER_ERR ) );
		break;
	case OVER:
		Asw_Diagnostic_OverMode( u16_realBatt, pDiag );

		if( pDiag->Err_Over == 1U ){
			clrSetReg32Bits( ( u32g_LDC_ERR_Sts ), ( BATT_V_UNDER_ERR ) , ( BATT_V_OVER_ERR ) );
		}
		break;
	default:
		break;
	}
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Asw_Diagnostic_Temper( uint16_t u16_realTemp, Diagnostic_Temp *pDiag )
{
	switch( pDiag->Mode )
	{
	case UNDER:
		Asw_Diagnostic_Temp_UnderMode( u16_realTemp, pDiag );
		break;
	case NORMAL:
		Asw_Diagnostic_Temp_NormalMode( u16_realTemp, pDiag );
		break;
	case OVER:
		Asw_Diagnostic_Temp_OverMode( u16_realTemp, pDiag);
		break;
	case DERATING:
		Asw_Diagnostic_Temp_DeratingMode( u16_realTemp, pDiag);
		break;
	default:
		break;
	}
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Asw_Diagnostic_OuputI(  uint16_t u16_realIout, Diagnostic *pDiag )
{
	if( Asw_Diagnostic_IsIoutOver( u16_realIout, pDiag ) == uTRUE )
	{
		setReg32Bits( ( u32g_LDC_ERR_Sts ), ( OUT_I_OVER_ERR ) );
	}

	if( Asw_Diagnostic_IsIoutOver_ShutDwn( u16_realIout, pDiag ) == uTRUE )
	{
		setReg32Bits( ( u32g_LDC_ERR_Sts ), ( OUT_I_OVER_SHUTDOWN_ERR ) );
	}
}
/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Asw_Diagnostic_Temp_UnderMode( uint16_t u16_realTemp, Diagnostic_Temp *pDiag )
{
	if( ( u16_realTemp >= pDiag->Under_Level ) && ( u16_realTemp < pDiag->Derating_Level ) )
	{
		pDiag->Mode = NORMAL;
	}
	else if( ( u16_realTemp >= pDiag->Derating_Level ) && ( u16_realTemp < pDiag->Over_Level ) )
	{
		pDiag->Mode = DERATING;
	}
	else if( u16_realTemp >= pDiag->Over_Level )
	{
		pDiag->Mode = OVER;
	}
	else
	{
		if( pDiag->Err_Under_Cnt < pDiag->Err_Cnt_Max )
		{
			pDiag->Err_Under_Cnt++;

			pDiag->Derating = 0U;
			pDiag->Derating_Cnt = 0U;
			pDiag->Err_Over = 0U;
			pDiag->Err_Over_Cnt = 0U;
		}
		else
		{
			pDiag->Err_Under = 1U;
			clrSetReg32Bits( ( u32g_LDC_ERR_Sts ), ( TEMP_OVER_ERR | TEMP_OVER_85_ERR ), TEMP_UNDER_ERR );
		}
	}
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Asw_Diagnostic_Temp_NormalMode( uint16_t u16_realTemp, Diagnostic_Temp *pDiag )
{
	static uint8_t u8_nrm_cnt = 0;

	if( ( u16_realTemp < pDiag->Under_Level ))
	{
		pDiag->Mode = UNDER;
		u8_nrm_cnt = 0;
	}
	else if( ( u16_realTemp >= pDiag->Derating_Level ) && ( u16_realTemp < pDiag->Over_Level ) )
	{
		pDiag->Mode = DERATING;
		u8_nrm_cnt = 0;
	}
	else if( u16_realTemp >= pDiag->Over_Level )
	{
		pDiag->Mode = OVER;
		u8_nrm_cnt = 0;
	}
	else
	{
		if( u8_nrm_cnt < 10U )
		{
			u8_nrm_cnt++;

			pDiag->Err_Under = 0U;
			pDiag->Err_Under_Cnt = 0;
			pDiag->Derating = 0U;
			pDiag->Derating_Cnt = 0U;
			pDiag->Err_Over = 0U;
			pDiag->Err_Over_Cnt = 0U;

			clrReg32Bits( ( u32g_LDC_ERR_Sts ), ( TEMP_OVER_ERR | TEMP_UNDER_ERR | TEMP_OVER_85_ERR ) );
		}
	}
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Asw_Diagnostic_Temp_DeratingMode( uint16_t u16_realTemp, Diagnostic_Temp *pDiag )
{
	static uint8_t u8_drt_cnt = 0;

	if( ( u16_realTemp < pDiag->Under_Level ))
	{
		pDiag->Mode = UNDER;
		u8_drt_cnt = 0;
	}
	else if( ( u16_realTemp >= pDiag->Under_Level ) && ( u16_realTemp < pDiag->Derating_Level ) )
	{
		pDiag->Mode = NORMAL;
		u8_drt_cnt = 0;
	}
	else if( u16_realTemp >= pDiag->Over_Level )
	{
		pDiag->Mode = OVER;
		u8_drt_cnt = 0;
	}
	else
	{
		if( u8_drt_cnt < 10U )
		{
			if( pDiag->Derating_Cnt < pDiag->Err_Cnt_Max )
			{
				pDiag->Derating_Cnt++;

				pDiag->Err_Under = 0U;
				pDiag->Err_Under_Cnt = 0;
				pDiag->Err_Over = 0U;
				pDiag->Err_Over_Cnt = 0U;
			}
			else
			{
				pDiag->Derating = 1U;
				clrSetReg32Bits( ( u32g_LDC_ERR_Sts ), ( TEMP_OVER_ERR | TEMP_UNDER_ERR ), TEMP_OVER_85_ERR );
			}
		}
	}
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Asw_Diagnostic_Temp_OverMode( uint16_t u16_realTemp, Diagnostic_Temp *pDiag )
{
	if( ( u16_realTemp < pDiag->Under_Level ))
	{
		pDiag->Mode = UNDER;
	}
	else if( ( u16_realTemp >= pDiag->Under_Level ) && ( u16_realTemp < pDiag->Derating_Level ) )
	{
		pDiag->Mode = NORMAL;
	}
	else if( ( u16_realTemp >= pDiag->Derating_Level ) && ( u16_realTemp < pDiag->Over_Level ) )
	{
		pDiag->Mode = DERATING;
	}
	else
	{
		pDiag->Err_Under = 0U;
		pDiag->Err_Under_Cnt = 0U;
		pDiag->Derating = 0U;
		pDiag->Derating_Cnt = 0U;

		clrSetReg32Bits( ( u32g_LDC_ERR_Sts ), ( TEMP_UNDER_ERR ), ( TEMP_OVER_ERR | TEMP_OVER_85_ERR ) );
	}
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Asw_Diagnostic_IoutSensor(  uint16_t u16_Averadc_Iout )
{
	/* Check open circuit fault */
	if( Asw_Diagnostic_IsSensorOpenCircuit( u16_Averadc_Iout, &Dia_Iout_Sens ) )
	{
		setReg32Bits( ( u32g_LDC_ERR_Sts ), ( CUR_SENSOR_OPEN_ERR ) );
	}
	else
	{
		/* Check short circuit fault */
		if( Asw_Diagnostic_IsSensorShortCircuit( u16_Averadc_Iout, &Dia_Iout_Sens ) )
		{
			setReg32Bits( ( u32g_LDC_ERR_Sts ), ( CUR_SENSOR_SHORT_ERR ) );
		}
	}
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Asw_Diagnostic_TempSensor( uint16_t u16_Averadc_Temp )
{
	/* Check open circuit fault */
	if( Asw_Diagnostic_IsSensorOpenCircuit( u16_Averadc_Temp, &Dia_Temp_Sens ) )
	{
		setReg32Bits( ( u32g_LDC_ERR_Sts ), ( TEMP_SENSOR_OPEN_ERR ) );
	}
	else
	{
		/* Check short circuit fault */
		if( Asw_Diagnostic_IsSensorShortCircuit( u16_Averadc_Temp, &Dia_Temp_Sens ) )
		{
			setReg32Bits( ( u32g_LDC_ERR_Sts ), ( TEMP_SENSOR_SHORT_ERR ) );
		}
	}
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Asw_Diagnostic_Batt_DisConnection( uint16_t u16_realVout )
{
	static uint16_t u16s_BattDisConnect_Count = 0U;

	if( u16_realVout < 30U )
	{
		if( u16s_BattDisConnect_Count >= BATT_DIS_ERR_CNT )
		{
			u8g_Asw_LDC_SYS_ERR_BATTERY_DIS = 1U;
		}
		else
		{
			u16s_BattDisConnect_Count++;
		}
	}
	else
	{
		u16s_BattDisConnect_Count = 0U;
		u8g_Asw_LDC_SYS_ERR_BATTERY_DIS = 0U;
	}
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Asw_Diagnostic_CAN1_Timeout( uint8_t u8_canTimeoutStatus, Diagnostic_CAN *pDiag )
{
	if( u8_canTimeoutStatus == 1U )
	{
		setReg32Bits( ( u32g_LDC_ERR_Sts ), ( CAN1_TIME_OUT_ERR ) );
		pDiag->CAN1_Timeout =  1U;
	}
	else
	{
		clrReg32Bits( ( u32g_LDC_ERR_Sts ), ( CAN1_TIME_OUT_ERR ) );
		pDiag->CAN1_Timeout =  0U;
	}
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Asw_Diagnostic_CAN2_Timeout( uint8_t u8_canTimeoutStatus, Diagnostic_CAN *pDiag )
{
	if( u8_canTimeoutStatus == 1U )
	{
		setReg32Bits( ( u32g_LDC_ERR_Sts ), ( CAN2_TIME_OUT_ERR ) );
		pDiag->CAN2_Timeout =  1U;
	}
	else
	{
		clrReg32Bits( ( u32g_LDC_ERR_Sts ), ( CAN2_TIME_OUT_ERR ) );
		pDiag->CAN2_Timeout =  0U;
	}
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Asw_Diagnostic_CAN1_Busoff( uint8_t u8_busOffStatus, Diagnostic_CAN *pDiag  )
{
	static uint16_t u16s_can1BusOffCnt = 0;

	if( u8_busOffStatus == 1U )
	{
		if( u16s_can1BusOffCnt >= CAN_BUS_OFF_ERR_CNT )
		{
			setReg32Bits( ( u32g_LDC_ERR_Sts ), ( CAN1_BUS_OFF_ERR ) );
			pDiag->CAN1_Busoff =  1U;
		}
		else
		{
			u16s_can1BusOffCnt++;
		}
	}
	else
	{
		clrReg32Bits( ( u32g_LDC_ERR_Sts ), ( CAN1_BUS_OFF_ERR ) );
		u16s_can1BusOffCnt = 0U;
		pDiag->CAN1_Busoff =  0U;
	}
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Asw_Diagnostic_CAN2_Busoff( uint8_t u8_busOffStatus, Diagnostic_CAN *pDiag )
{
	static uint16_t u16s_can2BusOffCnt = 0U;

	if( u8_busOffStatus == 1U )
	{
		if( u16s_can2BusOffCnt >= CAN_BUS_OFF_ERR_CNT )
		{
			setReg32Bits( ( u32g_LDC_ERR_Sts ), ( CAN2_BUS_OFF_ERR ) );
			pDiag->CAN2_Busoff =  1U;
		}
		else
		{
			u16s_can2BusOffCnt++;
		}
	}
	else
	{
		clrReg32Bits( ( u32g_LDC_ERR_Sts ), ( CAN2_BUS_OFF_ERR ) );
		u16s_can2BusOffCnt = 0U;
		pDiag->CAN2_Busoff =  0U;
	}
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Asw_Diagnostic_OverMode ( uint16_t u16_real, Diagnostic *pDiag )
{
	if ( u16_real < pDiag->Fault_Min )
	{
		pDiag->Mode = UNDER;
	}
	else if ( ( u16_real >= pDiag->Fault_Min ) && ( u16_real <= pDiag->Warning_Max ) )
	{
		pDiag->Mode = NORMAL;
	}
	else if ( ( u16_real > pDiag->Warning_Max ) && ( u16_real <= pDiag->Fault_Max ) )
	{
		if( pDiag->Err_Over == 0U ){
			pDiag->Mode = NORMAL;
		}
	}
	else
	{
		if( pDiag->Err_Over_Cnt < pDiag->Err_Cnt_Max )
		{
			pDiag->Err_Over_Cnt++;

			pDiag->Err_Under = 0U;
			pDiag->Err_Under_Cnt = 0U;
		}
		else
		{
			pDiag->Err_Over = 1U;
		}
	}
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Asw_Diagnostic_NormalMode ( uint16_t u16_real, Diagnostic *pDiag )
{
	if ( ( u16_real < pDiag->Fault_Min ) )
	{
		pDiag->Mode = UNDER;
		pDiag->Normal_Cnt = 0U;
	}
	else if( u16_real > pDiag->Fault_Max )
	{
		pDiag->Mode = OVER;
		pDiag->Normal_Cnt = 0U;
	}
	else
	{
		if( pDiag->Normal_Cnt < 50U )
		{
			pDiag->Normal_Cnt++;
			pDiag->Err_Over = 0U;
			pDiag->Err_Over_Cnt = 0U;
			pDiag->Err_Under = 0U;
			pDiag->Err_Under_Cnt = 0U;
		}
	}
}
/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Asw_Diagnostic_UnderMode ( uint16_t u16_real, Diagnostic *pDiag )
{
	if ( ( u16_real >= pDiag->Fault_Min ) && ( u16_real < pDiag->Warning_Min ) )
	{
		if( pDiag->Err_Under == 0U )
		{
			pDiag->Mode = NORMAL;
		}
	}
	else if ( ( u16_real >= pDiag->Warning_Min ) && ( u16_real <= pDiag->Fault_Max ) )
	{
		pDiag->Mode = NORMAL;
	}
	else if( u16_real > pDiag->Fault_Max )
	{
		pDiag->Mode = OVER;
	}
	else
	{
		if( pDiag->Err_Under_Cnt < pDiag->Err_Cnt_Max )
		{
			pDiag->Err_Under_Cnt++;

			pDiag->Err_Over = 0U;
			pDiag->Err_Over_Cnt = 0U;
		}
		else
		{
			pDiag->Err_Under = 1U;
		}
	}
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static uint8_t Asw_Diagnostic_IsIoutOver ( uint16_t u16_real, Diagnostic *pDiag )
{
	if( ( u16_real > pDiag->Fault_Min ) && ( u16_real < pDiag->Fault_Max ) )
	{
		if( pDiag->Err_Over_Cnt < pDiag->Err_Cnt_Max )
		{
			pDiag->Err_Over_Cnt++;
		}
		else
		{
			pDiag->Err_Over =  1U;
		}
	}

	return ( pDiag->Err_Over );
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static uint8_t Asw_Diagnostic_IsIoutOver_ShutDwn ( uint16_t u16_real, Diagnostic *pDiag )
{
	if( u16_real > pDiag->Fault_Max )
	{
		if( pDiag->Err_Over_ShutdwnCnt < pDiag->Err_Cnt_Max )
		{
			pDiag->Err_Over_ShutdwnCnt++;
		}
		else
		{
			pDiag->Err_Shutdown =  1U;
		}
	}

	return ( pDiag->Err_Shutdown );
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static uint8_t Asw_Diagnostic_IsSensorShortCircuit ( uint16_t u16_adc, Diagnostic_Sensor *pDiag )
{
	if( u16_adc < pDiag->Short_Limit )
	{
		if( pDiag->Err_Short_Cnt < pDiag->Err_Cnt_Max ){
			pDiag->Err_Short_Cnt++;
		}
		else{
			pDiag->Err_Short = 1U;
		}
	}

	return ( pDiag->Err_Short );
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static uint8_t Asw_Diagnostic_IsSensorOpenCircuit ( uint16_t u16_adc, Diagnostic_Sensor *pDiag )
{
	if( u16_adc > pDiag->Open_Limit )
	{
		if( pDiag->Err_Open_Cnt < pDiag->Err_Cnt_Max ){
			pDiag->Err_Open_Cnt++;
		}
		else{
			pDiag->Err_Open = 1U;
		}
	}

	return ( pDiag->Err_Open );
}

/****************************************************************************/
/**                                                                        **/
/**                      EOF                                               **/
/**                                                                        **/
/****************************************************************************/
