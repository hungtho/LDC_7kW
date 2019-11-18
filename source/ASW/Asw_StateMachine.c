/*
                             *******************
******************************* C SOURCE FILE ********************************
**                           *******************                            **
**                                                                          **
**  Project   : KSC_LDC                                                     **
**  Filename  : Asw_StateMachine.c                                              **
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

#define STATEMACHINE_C_SRC

/****************************************************************************/
/**                                                                        **/
/**                      MODULES USED                                      **/
/**                                                                        **/
/****************************************************************************/
#include "Asw_StateMachine.h"

/****************************************************************************/
/**                                                                        **/
/**                      DEFINITIONS AND MACROS                            **/
/**                                                                        **/
/****************************************************************************/
#define POWER_LATCH_TM 	( 5000U )
#define VOUT_REF				( 280U )


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
static void Sts_Init ( void );
static void Sts_ReadyCheck ( void );
static void Sts_Ready ( void );
static void Sts_RunCheck ( void );
static void Sts_Run ( void );
static void Sts_RecoveryFault ( void );
static void Sts_Fault ( void );
static void Sts_PowerLatch ( void );

static uint8_t GetRecoveryError ( void );
static uint8_t GetRecovery ( void );
static uint8_t GetNonRecoveryError ( void );

/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED VARIABLES                                **/
/**                                                                        **/
/****************************************************************************/
EN_LDC_Sts eng_LDC_Sts;
uint32_t u32_IoRef_SS = 0;

/****************************************************************************/
/**                                                                        **/
/**                      GLOBAL VARIABLES                                  **/
/**                                                                        **/
/****************************************************************************/
static uint16_t u16s_Check_cnt = 0U;
static uint16_t u16s_PowerLatch_cnt = 0;

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
void Asw_LDC_Operation ( void )
{
	switch( eng_LDC_Sts )
	{
		case en_LDC_Init:
			Sts_Init ();
			break;
		case en_LDC_ReadyCheck:
			Sts_ReadyCheck ();
			break;
		case en_LDC_Ready:
			Sts_Ready ();
			break;
		case en_LDC_RunCheck:
			Sts_RunCheck ();
			break;
		case en_LDC_Run:
			Sts_Run ();
			break;
		case en_LDC_RecoveryFault:
			Sts_RecoveryFault ();
			break;
		case en_LDC_Fault:
			Sts_Fault ();
			break;
		case en_LDC_PowerLatch:
			Sts_PowerLatch ();
			break;
		default:
			eng_LDC_Sts = en_LDC_Init;
			break;
	}
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
static void Sts_Init ( void )
{
	u8g_Asw_Ldc_State_Ready = 0U;
	u8g_Asw_Ldc_State_Run = 0U;

	u16s_Check_cnt = 0U;

	eng_LDC_Sts = en_LDC_ReadyCheck;

	return;
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Sts_ReadyCheck ( void )
{
	u8g_Asw_Ldc_State_Ready = 0U;
	u8g_Asw_Ldc_State_Run = 0U;

	if( u16s_Check_cnt >= 70U )
	{
		if( GetNonRecoveryError () == 1U )
		{
			eng_LDC_Sts = en_LDC_Fault;
		}
		else
		{
			if( GetRecoveryError () == 0U )
			{
				u8g_Asw_Ldc_State_Ready = 1U;

				eng_LDC_Sts = en_LDC_Ready;
			}
		}
	}
	else
	{
		u16s_Check_cnt++;
	}

	if( Asw_GetIgnSts() != 1U )
	{
		u16s_PowerLatch_cnt = 0U;

		eng_LDC_Sts = en_LDC_PowerLatch;
	}

	return;
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Sts_Ready ( void )
{
	u8g_Asw_Ldc_State_Ready = 1U;
	u8g_Asw_Ldc_State_Run = 0U;

	if( Asw_GetIgnSts() != 1U )
	{
		u8g_Asw_Ldc_State_Ready = 0U;
		u16s_PowerLatch_cnt = 0U;

		eng_LDC_Sts = en_LDC_PowerLatch;
	}
	else if( GetNonRecoveryError () == 1U )
	{
		u8g_Asw_Ldc_State_Ready = 0U;

		eng_LDC_Sts = en_LDC_Fault;
	}
	else if( GetRecoveryError () == 1U )
	{
		u8g_Asw_Ldc_State_Ready = 0U;

		eng_LDC_Sts = en_LDC_Init;
	}
	else
	{
		if( u8g_Asw_LDC_ENB == 1U )
		{
			Asw_PI_Init();
			u32_IoRef_SS = 0;
			u32g_Asw_IoReference = 84512U; // (160 A)
			u32g_Asw_VoReference = 96384U; //UTILS_REF_INV ( VOUT_REF, u16_AdcVout, u16_Vout ) << 5U;
			u16s_Check_cnt = 0U;

			eng_LDC_Sts = en_LDC_RunCheck;
		}
	}

	return;
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Sts_RunCheck ( void )
{
	u8g_Asw_Ldc_State_Ready = 1U;
	u8g_Asw_Ldc_State_Run = 0U;

	if( u16s_Check_cnt >= 30U )
	{
		if( GetNonRecoveryError () == 1U )
		{
			u8g_Asw_Ldc_State_Ready = 0U;

			eng_LDC_Sts = en_LDC_Fault;
		}
		else if( GetRecoveryError () == 1U )
		{
			u8g_Asw_Ldc_State_Ready = 0U;

			eng_LDC_Sts = en_LDC_RecoveryFault;
		}
		else
		{
			u8g_Asw_Ldc_State_Run = 1U;
			Bsw_PwmEnable ();

			eng_LDC_Sts = en_LDC_Run;
		}
	}
	else
	{
		u16s_Check_cnt++;
	}

	if( Asw_GetIgnSts() != 1U )
	{
		u8g_Asw_Ldc_State_Ready = 0U;
		u16s_PowerLatch_cnt = 0U;

		eng_LDC_Sts = en_LDC_PowerLatch;
	}
	else
	{
		if( u8g_Asw_LDC_ENB != 1U )
		{
			eng_LDC_Sts = en_LDC_Ready;
		}
	}

	return;
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Sts_Run ( void )
{

	u8g_Asw_Ldc_State_Ready = 1U;
	u8g_Asw_Ldc_State_Run = 1U;

	/*---------------IoRef --------------------*/
	Asw_IoRef_TemperDerating ( u16g_Bsw_realTemp );

	if( Asw_GetIgnSts() != 1U )
	{
		u8g_Asw_Ldc_State_Ready = 0U;
		u8g_Asw_Ldc_State_Run = 0U;
		u16s_PowerLatch_cnt = 0U;

		eng_LDC_Sts = en_LDC_PowerLatch;
	}
	else if( GetNonRecoveryError () == 1U )
	{
		u8g_Asw_Ldc_State_Ready = 0U;
		u8g_Asw_Ldc_State_Run = 0U;

		eng_LDC_Sts = en_LDC_Fault;
	}
	else if( GetRecoveryError () == 1U )
	{
		u8g_Asw_Ldc_State_Ready = 0U;
		u8g_Asw_Ldc_State_Run = 0U;

		eng_LDC_Sts = en_LDC_RecoveryFault;
	}
	else
	{
		if( u8g_Asw_LDC_ENB != 1U )
		{
			u8g_Asw_Ldc_State_Run = 0U;

			eng_LDC_Sts = en_LDC_Ready;
		}
	}

	return;
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Sts_RecoveryFault ( void )
{
	u8g_Asw_Ldc_State_Ready = 0U;
	u8g_Asw_Ldc_State_Run = 0U;

	if( Asw_GetIgnSts() != 1U )
	{
		u16s_PowerLatch_cnt = 0U;

		eng_LDC_Sts = en_LDC_PowerLatch;
	}
	else
	{
		if( GetRecovery () != 1U )
		{
			eng_LDC_Sts = en_LDC_Init;
		}
	}

	return;
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Sts_Fault ( void )
{
	u8g_Asw_Ldc_State_Ready = 0U;
	u8g_Asw_Ldc_State_Run = 0U;

	if( Asw_GetIgnSts() != 1U )
	{
		u16s_PowerLatch_cnt = 0U;

		eng_LDC_Sts = en_LDC_PowerLatch;
	}

	return;
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Sts_PowerLatch ( void )
{
	u8g_Asw_Ldc_State_Ready = 0U;
	u8g_Asw_Ldc_State_Run = 0U;

	if( u16s_PowerLatch_cnt >= POWER_LATCH_TM )
	{
		pal_clearpad ( PORT_A, POWER_LATCH ); /* Power latch off */
	}
	else
	{
		u16s_PowerLatch_cnt++;

		if( Asw_GetIgnSts() == 1U )
		{
			clrReg32Bits( ( u32g_LDC_ERR_Sts ),
										( OUT_I_OVER_SHUTDOWN_ERR
										| OUT_I_OVER_ERR
										| CUR_SENSOR_OPEN_ERR
										| CUR_SENSOR_SHORT_ERR
										| TEMP_SENSOR_OPEN_ERR
										| TEMP_SENSOR_SHORT_ERR ) );

			eng_LDC_Sts = en_LDC_Init;
		}
	}

	return;
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static uint8_t GetRecoveryError ( void )
{
	uint8_t u8_Ret = 0U;
	uint32_t u32_ErrSts;
	uint32_t u32_CanBusOffSts;
	uint32_t u32_CanTimeOutSts;

	u32_CanBusOffSts = CAN1_BUS_OFF_ERR | CAN2_BUS_OFF_ERR;
	u32_CanTimeOutSts = CAN1_TIME_OUT_ERR | CAN2_TIME_OUT_ERR;
	u32_ErrSts = OUT_V_OVER_ERR
						 | OUT_V_UNDER_ERR
						 | BATT_V_OVER_ERR
						 | BATT_V_UNDER_ERR
						 | TEMP_OVER_ERR
						 | TEMP_UNDER_ERR
						 | IN_V_OVER_ERR
						 | IN_V_UNDER_ERR;

	if( ( ( u32g_LDC_ERR_Sts & u32_ErrSts ) != 0U )
	 || ( ( u32g_LDC_ERR_Sts & u32_CanBusOffSts ) == u32_CanBusOffSts )
	 ||	( ( u32g_LDC_ERR_Sts & u32_CanTimeOutSts ) == u32_CanTimeOutSts )
	 || ( u8g_Asw_LDC_SYS_ERR_BATTERY_DIS == 1U ) )
	{
		u8_Ret = 1U;
	}

	return ( u8_Ret );
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static uint8_t GetRecovery ( void )
{
	uint8_t u8_Ret = 0U;
	uint32_t u32_ErrSts;
	uint32_t u32_CanBusOffSts;
	uint32_t u32_CanTimeOutSts;

	u32_CanBusOffSts = CAN1_BUS_OFF_ERR | CAN2_BUS_OFF_ERR;
	u32_CanTimeOutSts = CAN1_TIME_OUT_ERR | CAN2_TIME_OUT_ERR;
	u32_ErrSts = OUT_V_OVER_ERR
						 | OUT_V_UNDER_ERR
						 | BATT_V_OVER_ERR
						 | BATT_V_UNDER_ERR
						 | TEMP_OVER_ERR
						 | TEMP_OVER_85_ERR
						 | TEMP_UNDER_ERR
						 | IN_V_OVER_ERR
						 | IN_V_UNDER_ERR;

	if( ( ( u32g_LDC_ERR_Sts & u32_ErrSts ) != 0U )
	 || ( ( u32g_LDC_ERR_Sts & u32_CanBusOffSts ) == u32_CanBusOffSts )
	 ||	( ( u32g_LDC_ERR_Sts & u32_CanTimeOutSts ) == u32_CanTimeOutSts )
	 || ( u8g_Asw_LDC_SYS_ERR_BATTERY_DIS == 1U ) )
	{
		u8_Ret = 1U;
	}

	return ( u8_Ret );
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static uint8_t GetNonRecoveryError ( void )
{
	uint8_t u8_Ret = 0U;
	uint32_t u32_ErrSts;

	u32_ErrSts = OUT_I_OVER_SHUTDOWN_ERR
			       | OUT_I_OVER_ERR
						 | CUR_SENSOR_OPEN_ERR
						 | CUR_SENSOR_SHORT_ERR
						 | TEMP_SENSOR_OPEN_ERR
						 | TEMP_SENSOR_SHORT_ERR;

	if( ( u32g_LDC_ERR_Sts & u32_ErrSts ) != 0U )
	{
		u8_Ret = 1U;
	}

	return ( u8_Ret );
}

/****************************************************************************/
/**                                                                        **/
/**                      EOF                                               **/
/**                                                                        **/
/****************************************************************************/
