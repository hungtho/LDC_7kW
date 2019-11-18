/*
                             *******************
******************************* C SOURCE FILE ********************************
**                           *******************                            **
**                                                                          **
**  Project   : KSC_LDC                                                     **
**  Filename  : Hal_PIT.c                                                    **
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

#define HALPIT_C_SRC

/****************************************************************************/
/**                                                                        **/
/**                      MODULES USED                                      **/
/**                                                                        **/
/****************************************************************************/
#include "Hal_PIT.h"

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

/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED VARIABLES                                **/
/**                                                                        **/
/****************************************************************************/
volatile uint8_t u8g_Hal_task_flag_1m = TRUE;
uint8_t u8g_Hal_task_10m_cnt = 10U;
uint8_t u8g_Hal_task_100m_cnt = 105U;

/****************************************************************************/
/**                                                                        **/
/**                      GLOBAL VARIABLES                                  **/
/**                                                                        **/
/****************************************************************************/
static volatile struct PIT_tag *stsp_PIT_0 = &PIT_0;

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
void Hal_PIT_Init ( void )
{
  SPCSetPeripheralClockMode( SPC5_PIT_PCTL, SPC5_ME_PCTL_RUN( 2 ) | SPC5_ME_PCTL_LP( 2 ) );

  stsp_PIT_0->MCR.R = 1U; /* PIT clock enabled, stop while debugging. */
  stsp_PIT_0->CH[ 3 ].LDVAL.R = 15999U; /* PIT clock 16Mhz, period = 1ms => counter =  16MHz/1kHz -1*/
  stsp_PIT_0->CH[ 3 ].CVAL.R  = 15999U;
  stsp_PIT_0->CH[ 3 ].TFLG.R  = 1U; /* Interrupt flag cleared. */
  stsp_PIT_0->CH[ 3 ].TCTRL.R = 3U; /* Timer active, interrupt enabled. */


  INTC_PSR( SPC_PIT_CH3_INT_NUMBER ) = INTC_PSR_ENABLE( INTC_PSR_CORE0, 13 );

	return;
}

/*--------------------------------------------------------------------------*/
/* Function  |             Period time : 100us, Execution time : 12.5us     */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
uint32_t Get_PIT_CH3_CVAL ( void )
{
	uint32_t u32_Ret;

	u32_Ret = stsp_PIT_0->CH[ 3 ].CVAL.R;

	return ( u32_Ret );
}

/*--------------------------------------------------------------------------*/
/* Function  |             Period time : 100us, Execution time : 12.5us     */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void SPC_PIT_CH3_HANDLER ( void )
{
	IRQ_PROLOGUE();

	u8g_Hal_task_flag_1m = TRUE;

  Hal_Adc_Start();
	u32g_Bsw_AdcIout_Q15 = ( u16g_Hal_AverAdc_Iout - IOUT_ADC_OFFSET ) << 5U ; /* Transform Q12 to Q17 */
	u32g_Bsw_AdcVout_Q15 = u16g_Hal_AverAdc_Vout << 5U; /* Transform Q12 to Q17 */

	u32_IoRef_SS = Asw_Ramp( u32g_Asw_IoReference, u32_IoRef_SS );

#if OPEN_LOOP
	u16g_Rte_pwmShift = Vcu_PhaseShift;
#else
	u16g_Rte_pwmShift = Asw_PI_Controller( u32_IoRef_SS, u32g_Asw_VoReference, u32g_Bsw_AdcIout_Q15, u32g_Bsw_AdcVout_Q15 );
#endif
	Bsw_PwmSetShift( u16g_Rte_pwmShift );

	stsp_PIT_0->CH[ 3 ].TFLG.R = 0x01U; /* clear flag*/

	IRQ_EPILOGUE();
}

/****************************************************************************/
/**                                                                        **/
/**                      LOCAL FUNCTIONS                                   **/
/**                                                                        **/
/****************************************************************************/

/****************************************************************************/
/**                                                                        **/
/**                      EOF                                               **/
/**                                                                        **/
/****************************************************************************/
