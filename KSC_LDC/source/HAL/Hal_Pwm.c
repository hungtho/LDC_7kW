/*
                             *******************
******************************* C SOURCE FILE ********************************
**                           *******************                            **
**                                                                          **
**  Project   : KSC_LDC                                                     **
**  Filename  : Hal_Pwm.c                                                    **
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

#define HALPWM_C_SRC

/****************************************************************************/
/**                                                                        **/
/**                      MODULES USED                                      **/
/**                                                                        **/
/****************************************************************************/
#include "Hal_Pwm.h"

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
typedef struct
{
	uint16_t u16_phaseShift;
	uint16_t u16_phaseShiftActive;
} tPwmProp;

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

/****************************************************************************/
/**                                                                        **/
/**                      GLOBAL VARIABLES                                  **/
/**                                                                        **/
/****************************************************************************/
static volatile struct ETIMER_tag *stsp_ETIMER_3 = &ETIMER_3;
static tPwmProp pwmB;

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
void Hal_PwmInit ( void )
{
	SPCSetPeripheralClockMode( SPC5_ETIMER3_PCTL, SPC5_ME_PCTL_RUN( 2 ) | SPC5_ME_PCTL_LP( 2 ) );

	stsp_ETIMER_3->ENBL.R = 0x00U; /* enable etimer channel 0,1,2,3 */

	stsp_ETIMER_3->CH[ 0 ].CTRL1.B.CNTMODE = 0x01U; /* Counter mode = count rising edges */
	stsp_ETIMER_3->CH[ 0 ].CTRL1.B.PRISRC = 0x18U; /* Primary count source = IBBus clock devide by prescaler 1 */
	stsp_ETIMER_3->CH[ 0 ].CTRL1.B.LENGTH = 1U; /* Count until compare */
	stsp_ETIMER_3->CH[ 0 ].CTRL1.B.ONCE = 0U; /* Continuous count */

	stsp_ETIMER_3->CH[ 0 ].CTRL2.B.OEN = 0U; /* Output disable */
	stsp_ETIMER_3->CH[ 0 ].CTRL2.B.OPS = 0U; /* True polarity */
	stsp_ETIMER_3->CH[ 0 ].CTRL2.B.OUTMODE = 0x08U;	/* Set on successful compare on COMP1, clear on successful compare on COMP2 */

	stsp_ETIMER_3->CH[ 0 ].CCCTRL.B.CLC1 = 0x03U; /* Load COMP1 with CMPLD1 upon successful compare with the value in COMP2 */
	stsp_ETIMER_3->CH[ 0 ].CCCTRL.B.CLC2 = 0x07U; /* Load CNTR with CMPLD2 upon successful compare with the value in COMP2 */

	stsp_ETIMER_3->CH[ 0 ].COMP1.R = ETIMER_3_DUTY/* + ETIMER_3_DEAD_TIME*/; /* Compare value for counter */
	stsp_ETIMER_3->CH[ 0 ].COMP2.R = ETIMER_3_PERIOD; /* Compare value for counter */
	stsp_ETIMER_3->CH[ 0 ].CMPLD1.R = ETIMER_3_DUTY + ETIMER_3_DEAD_TIME;
	stsp_ETIMER_3->CH[ 0 ].CMPLD2.R = 0;

	stsp_ETIMER_3->CH[ 0 ].LOAD.R = ETIMER_3_PERIOD; /* Reload value used to initialize the counter */
	stsp_ETIMER_3->CH[ 0 ].CNTR.R = ETIMER_3_PERIOD; /* Initial value */

	/*---------------------------------------------------------------------------/
		@ eTimer 3 channel 1
	----------------------------------------------------------------------------*/
	stsp_ETIMER_3->CH[ 1 ].CTRL1.B.CNTMODE = 0x01U; /* Counter mode = count rising edges */
	stsp_ETIMER_3->CH[ 1 ].CTRL1.B.PRISRC = 0x18U; /* Primary count source = IBBus clock devide by prescaler 1 */
	stsp_ETIMER_3->CH[ 1 ].CTRL1.B.LENGTH = 1U; /* Count until compare */
	stsp_ETIMER_3->CH[ 1 ].CTRL1.B.ONCE = 0U; /* Continuous count */

	stsp_ETIMER_3->CH[ 1 ].CTRL2.B.OEN = 0U; /* Output disable */
	stsp_ETIMER_3->CH[ 1 ].CTRL2.B.OPS = 1U; /* Inverted polarity */
	stsp_ETIMER_3->CH[ 1 ].CTRL2.B.OUTMODE = 0x08U;	/* Set on successful compare on COMP1, clear on successful compare on COMP2 */

	stsp_ETIMER_3->CH[ 1 ].CCCTRL.B.CLC1 = 0x03U; /* Load COMP1 with CMPLD1 upon successful compare with the value in COMP2 */
	stsp_ETIMER_3->CH[ 1 ].CCCTRL.B.CLC2 = 0x07U; /* Load CNTR with CMPLD2 upon successful compare with the value in COMP2 */

	stsp_ETIMER_3->CH[ 1 ].COMP1.R = ETIMER_3_DUTY; /* Compare value for counter */
	stsp_ETIMER_3->CH[ 1 ].COMP2.R = ETIMER_3_PERIOD + ETIMER_3_DEAD_TIME; /* Compare value for counter */
	stsp_ETIMER_3->CH[ 1 ].CMPLD1.R = ETIMER_3_DUTY;
	stsp_ETIMER_3->CH[ 1 ].CMPLD2.R = ETIMER_3_DEAD_TIME;

	stsp_ETIMER_3->CH[ 1 ].LOAD.R = ETIMER_3_PERIOD; /* Reload value used to initialize the counter */
	stsp_ETIMER_3->CH[ 1 ].CNTR.R = ETIMER_3_PERIOD; /* Initial value */

	/*---------------------------------------------------------------------------/
		@ eTimer 3 channel 2
	----------------------------------------------------------------------------*/
	stsp_ETIMER_3->CH[ 2 ].CTRL1.B.CNTMODE = 0x01U; /* Counter mode = count rising edges */
	stsp_ETIMER_3->CH[ 2 ].CTRL1.B.PRISRC = 0x18U; /* Primary count source = IBBus clock devide by prescaler 1 */
	stsp_ETIMER_3->CH[ 2 ].CTRL1.B.LENGTH = 1U; /* Count until compare */
	stsp_ETIMER_3->CH[ 2 ].CTRL1.B.ONCE = 0U; /* Continuous count */

	stsp_ETIMER_3->CH[ 2 ].CTRL2.B.OEN = 0U; /* Output disable */
	stsp_ETIMER_3->CH[ 2 ].CTRL2.B.OPS = 1U; /* Inverted polarity */
	stsp_ETIMER_3->CH[ 2 ].CTRL2.B.OUTMODE = 0x08U;	/* Set on successful compare on COMP1, clear on successful compare on COMP2 */

	stsp_ETIMER_3->CH[ 2 ].CCCTRL.B.CLC1 = 0x03U; /* Load COMP1 with CMPLD1 upon successful compare with the value in COMP2 */
	stsp_ETIMER_3->CH[ 2 ].CCCTRL.B.CLC2 = 0x07U; /* Load CNTR with CMPLD2 upon successful compare with the value in COMP2 */

	stsp_ETIMER_3->CH[ 2 ].COMP1.R = ETIMER_3_DUTY; /* Compare value for counter */
	stsp_ETIMER_3->CH[ 2 ].COMP2.R = ETIMER_3_PERIOD + ETIMER_3_DEAD_TIME; /* Compare value for counter */
	stsp_ETIMER_3->CH[ 2 ].CMPLD1.R = ETIMER_3_DUTY;
	stsp_ETIMER_3->CH[ 2 ].CMPLD2.R = ETIMER_3_DEAD_TIME;

	stsp_ETIMER_3->CH[ 2 ].LOAD.R = 0; /* Reload value used to initialize the counter */
	stsp_ETIMER_3->CH[ 2 ].CNTR.R = 0; /* Initial value */

	/*---------------------------------------------------------------------------/
		@ eTimer 3 channel 3
	----------------------------------------------------------------------------*/
	stsp_ETIMER_3->CH[ 3 ].CTRL1.B.CNTMODE = 0x01U; /* Counter mode = count rising edges */
	stsp_ETIMER_3->CH[ 3 ].CTRL1.B.PRISRC = 0x18U; /* Primary count source = IBBus clock devide by prescaler 1 */
	stsp_ETIMER_3->CH[ 3 ].CTRL1.B.LENGTH = 1U; /* Count until compare */
	stsp_ETIMER_3->CH[ 3 ].CTRL1.B.ONCE = 0U; /* Continuous count */

	stsp_ETIMER_3->CH[ 3 ].CTRL2.B.OEN = 0U; /* Output disable */
	stsp_ETIMER_3->CH[ 3 ].CTRL2.B.OPS = 0U; /* True polarity */
	stsp_ETIMER_3->CH[ 3 ].CTRL2.B.OUTMODE = 0x08U;	/* Set on successful compare on COMP1, clear on successful compare on COMP2 */

	stsp_ETIMER_3->CH[ 3 ].CCCTRL.B.CLC1 = 0x03U; /* Load COMP1 with CMPLD1 upon successful compare with the value in COMP2 */
	stsp_ETIMER_3->CH[ 3 ].CCCTRL.B.CLC2 = 0x07U; /* Load CNTR with CMPLD2 upon successful compare with the value in COMP2 */

	stsp_ETIMER_3->CH[ 3 ].COMP1.R = ETIMER_3_DUTY + ETIMER_3_DEAD_TIME; /* Compare value for counter */
	stsp_ETIMER_3->CH[ 3 ].COMP2.R = ETIMER_3_PERIOD; /* Compare value for counter */
	stsp_ETIMER_3->CH[ 3 ].CMPLD1.R = ETIMER_3_DUTY + ETIMER_3_DEAD_TIME;
	stsp_ETIMER_3->CH[ 3 ].CMPLD2.R = 0;

	stsp_ETIMER_3->CH[ 3 ].LOAD.R = 0; /* Reload value used to initialize the counter */
	stsp_ETIMER_3->CH[ 3 ].CNTR.R = 0; /* Initial value */

	stsp_ETIMER_3->CH[ 3 ].STS.R = 0xFFFFU;
	stsp_ETIMER_3->CH[ 3 ].INTDMA.B.TCF2IE = 1U;
	INTC_PSR( SPC5_ETIMER3_STS3_NUMBER ) = INTC_PSR_ENABLE( INTC_PSR_CORE0, 15 );
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void Hal_PwmSetShift( uint16_t u16_shift )
{
	if( u16_shift < ( PWM_DUTY_MAX / 2U ) )
	{
		pwmB.u16_phaseShift = u16_shift;
	}
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void Hal_PwmEnable ( void )
{
	stsp_ETIMER_3->CH[ 0 ].CTRL2.B.OEN = 1U; /* output enable */
	stsp_ETIMER_3->CH[ 1 ].CTRL2.B.OEN = 1U; /* output enable */
	stsp_ETIMER_3->CH[ 2 ].CTRL2.B.OEN = 1U; /* output enable */
	stsp_ETIMER_3->CH[ 3 ].CTRL2.B.OEN = 1U; /* output enable */

	stsp_ETIMER_3->ENBL.R = 0x0FU; /* enable etimer channel 0,1,2,3 */
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void Hal_PwmDisable ( void )
{
	stsp_ETIMER_3->CH[ 0 ].CTRL2.B.OEN = 0U; /* output disable */
	stsp_ETIMER_3->CH[ 1 ].CTRL2.B.OEN = 0U; /* output disable */
	stsp_ETIMER_3->CH[ 2 ].CTRL2.B.OEN = 0U; /* output disable */
	stsp_ETIMER_3->CH[ 3 ].CTRL2.B.OEN = 0U; /* output disable */

	stsp_ETIMER_3->ENBL.R = 0x00U; /* disable etimer channel 0,1,2,3 */
}

/*--------------------------------------------------------------------------*/
/* Function  |    PWMIsrHandler()                                           */
/* Name      |                                                              */
/* Period time : 10.8 us(92kHz)																							*/
/* Execution time : 1.3us																										*/
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void SPC5_ETIMER3_STS3_HANDLER ( void )
{
	uint16_t u16_shiftB = 0;

	IRQ_PROLOGUE();

	u16_shiftB = ( uint16_t )( pwmB.u16_phaseShift - pwmB.u16_phaseShiftActive );
	pwmB.u16_phaseShiftActive = pwmB.u16_phaseShift;

	stsp_ETIMER_3->CH[ 2 ].CMPLD2.R = ( uint16_t )ETIMER_3_DEAD_TIME + ( uint16_t )u16_shiftB;
	stsp_ETIMER_3->CH[ 3 ].CMPLD2.R = ( uint16_t )( u16_shiftB );

	stsp_ETIMER_3->CH[ 0 ].STS.B.TCF2 = 0x1U;
	stsp_ETIMER_3->CH[ 1 ].STS.B.TCF2 = 0x1U;
	stsp_ETIMER_3->CH[ 2 ].STS.B.TCF2 = 0x1U;
	stsp_ETIMER_3->CH[ 3 ].STS.B.TCF2 = 0x1U;

	IRQ_EPILOGUE ();
}

/****************************************************************************/
/**                                                                        **/
/**                      EOF                                               **/
/**                                                                        **/
/****************************************************************************/
