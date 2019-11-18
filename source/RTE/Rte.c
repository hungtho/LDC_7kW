/*
                             *******************
******************************* C SOURCE FILE ********************************
**                           *******************                            **
**                                                                          **
**  Project   : KSC_LDC                                                     **
**  Filename  : Rte.c                                                        **
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

#define RTE_C_SRC

/****************************************************************************/
/**                                                                        **/
/**                      MODULES USED                                      **/
/**                                                                        **/
/****************************************************************************/
#include "Rte.h"
#include "Bsw_Eeprom.h"

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
/**                      PROTOTYPES OF LOCAL FUNCTIONS                     **/
/**                                                                        **/
/****************************************************************************/
static void Rte_TimeTask_TT0 ( void );
static void Rte_TimeTask_TT1 ( void );
static void Rte_TimeTask_TT2 ( void );

/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED VARIABLES                                **/
/**                                                                        **/
/****************************************************************************/
uint16_t u16g_Rte_pwmShift = 0U;

/****************************************************************************/
/**                                                                        **/
/**                      LOCAL VARIABLES                                  **/
/**                                                                        **/
/****************************************************************************/
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
void Rte_Init ( void )
{
	/*------------------------------------
	 * Disable interrupts
	 * ----------------------------------*/
	irqIsrDisable();
	/*------------------------------------
	 * System initialization
	 * ----------------------------------*/
	componentsInit();

	Bsw_PIT_Init();

	/* Initializing of CAN communication module. */
	Bsw_CAN1_Init ();
	Bsw_CAN2_Init ();

	/* Initializing of PWM timer module. */
	Bsw_PwmInit();

	/* Initializing of ADC module. */
	Bsw_AdcInit();

	Gp_Func_Init();

	/* Initializing of Uds_function. */
	Uds_Func_Init();

	/* Initializing of Tp_function. */
	Tp_Func_Init();

	Bsw_Can_DM1_Init ();

	/* Read EEPROM Execution time : 55us */
	Asw_Diagnostic_Init();

#if 1
  EEP_EEPROM_INIT();
  EEP_EEPROM_READ();
#else
	Asw_FindCurrentAdd();
	Asw_DTC_Eeprom_Read ( u32g_Asw_present_addr );
	Asw_DID_Eeprom_Read ( u32g_Asw_present_addr );
#endif


	u8g_Asw_Ldc_State_Ready = 0U;
	u8g_Asw_Ldc_State_Run = 0U;

	irqIsrEnable();
	/*------------------------------------
	 * Reset sequence has finished
	 * ----------------------------------*/
	eng_LDC_Sts = en_LDC_Init;
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void Rte_TimeTaskCtrl ( void )
{
	/* -------------------------------------------------------------------*/
	/*   Time Task Management for TT1: 1ms																*/
	/* -------------------------------------------------------------------*/
	/* Execution periodic time task TT0 */
	Rte_TimeTask_TT0();

	/* -------------------------------------------------------------------*/
	/*   Time Task Management for TT1: 10ms																*/
	/* -------------------------------------------------------------------*/
	if( --u8g_Hal_task_10m_cnt == 0U )
	{
		u8g_Hal_task_10m_cnt = 10U;

		/* Execution periodic time task TT1 */
		Rte_TimeTask_TT1();
	}

	/* -------------------------------------------------------------------*/
	/*   Time Task Management for TT1: 100ms															*/
	/* -------------------------------------------------------------------*/
	if( --u8g_Hal_task_100m_cnt == 0U )
	{
		u8g_Hal_task_100m_cnt = 100U;

		/* Execution periodic time task TT2 */
		Rte_TimeTask_TT2();
	}

	return;
}

/****************************************************************************/
/**                                                                        **/
/**                      LOCAL FUNCTIONS                                   **/
/**                                                                        **/
/****************************************************************************/
/*--------------------------------------------------------------------------*/
/* Function  |               Period time : 1ms, Execution time : 379us	    */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Rte_TimeTask_TT0 ( void )
{
	/* unified diagnostic service functions */

	Uds_Func();
	/* Transfer protocol functions */
	Tp_Func();

	/* WD Toggle */
	if( u8g_ReqReset != 1U )
	{
		UDS_WDT_PulseOut ();
	}
}

/*--------------------------------------------------------------------------*/
/* Function  |        Period time : 10ms, Execution time : 100us	          */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Rte_TimeTask_TT1 ( void )
{
	if( u8g_CAN_TX_Ctrl == CAN_CTRL_ENABLE )
	{
		Bsw_Can_DM1_Transmitter();
	}

	Bsw_ADC2REAL();

	Asw_Diagnostic_Fault ();
	Asw_LDC_Operation ();

	if( eng_LDC_Sts != en_LDC_Run ){ u16g_Rte_pwmShift = Asw_LDC_OFF ( u16g_Rte_pwmShift ); }
}

/*--------------------------------------------------------------------------*/
/* Function  |      Period time : 100ms, Execution time : 118us	            */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Rte_TimeTask_TT2 ( void )
{
	static LdcDtcDef dtcBuffer = {0, };

	Bsw_Can_Tx();
	Bsw_Can_Rx ();

	if( u8_DtcControl == DTC_ENABLE ){ Asw_DTC_Eeprom_Write(); }

	if( eng_LDC_Sts != en_LDC_Run )
	{
#if NEW_EEPROM
		utils_memcpy(&dtcBuffer, EEP_GET_WRITE_BUFFER(), sizeof(LdcDtcDef));
		DtcMng_DtcCodeUpdate( &dtcBuffer );
		EEP_UPDATE_EEPROM(&dtcBuffer, sizeof(LdcDtcDef), eng_LDC_Sts);
#else
		if( u8g_Asw_WriteEEPROM_Flag == 1U )
		{
			/* Write dtc code to EEPROM during key off */
			UDS_EEP_WriteDataByIdentifier( &u32g_Asw_EEPROM_BUFFER_DTC[ 0 ] );

			/* Write DatabyIdentifier code to EEPROM during key off */
			UDS_EEP_WriteDataByIdentifier( &u32g_Asw_EEPROM_BUFFER_DID[ 0 ] );

			u8g_Asw_WriteEEPROM_Flag = 0U;
		}
#endif
	}
}

/****************************************************************************/
/**                                                                        **/
/**                      EOF                                               **/
/**                                                                        **/
/****************************************************************************/
