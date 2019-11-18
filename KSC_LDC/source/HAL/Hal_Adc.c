/*
                             *******************
******************************* C SOURCE FILE ********************************
**                           *******************                            **
**                                                                          **
**  Project   : KSC_LDC                                                     **
**  Filename  : HalADC.c                                                    **
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

#define HALADC_C_SRC

/****************************************************************************/
/**                                                                        **/
/**                      MODULES USED                                      **/
/**                                                                        **/
/****************************************************************************/
#include "Hal_Adc.h"

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
static void Hal_AdcBuffer ( void );
static uint16_t Hal_MovingAvg ( const uint16_t ptrArrNumbers[] );

/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED VARIABLES                                **/
/**                                                                        **/
/****************************************************************************/
uint16_t u16g_Hal_AverAdc_Batt = 0U;
uint16_t u16g_Hal_AverAdc_Vout = 0U;
uint16_t u16g_Hal_AverAdc_Vin = 0U;

uint16_t u16g_Hal_AverAdc_Iout = 0U;
uint16_t u16g_Hal_AverAdc_Temp = 0U;
uint16_t u16g_Hal_AverAdc_KeyIn = 0U;

/****************************************************************************/
/**                                                                        **/
/**                      GLOBAL VARIABLES                                  **/
/**                                                                        **/
/****************************************************************************/
static volatile struct SARADC_tag *stsp_SAR_ADC_12BIT_0 = &SAR_ADC_12BIT_0;

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
void Hal_AdcInit( void )
{
	SPCSetPeripheralClockMode( SPC5_SARADC12_0_PCTL, SPC5_ME_PCTL_RUN( 2 ) | SPC5_ME_PCTL_LP( 2 ) );

	stsp_SAR_ADC_12BIT_0->MCR.B.NTRGEN = 1; /* Normal trigger enabled to start a
	                                      normal conversion */
	stsp_SAR_ADC_12BIT_0->MCR.B.MODE = 0x0; /* Sets conversion mode on SARADCB,
	                                      single conversion */
	stsp_SAR_ADC_12BIT_0->MCR.B.OWREN = 1; /* SAR ADC B overwrite enable set */
	stsp_SAR_ADC_12BIT_0->ICIMR[0].R = 0x000002ECU; /* Interrupt of ANP[2],ANP[3],ANP[5],ANP[6],ANP[7],ANP[9] enabled */
	stsp_SAR_ADC_12BIT_0->IMR.B.MSKNECH = 1U; /* NEOC interrupt is enabled */
	stsp_SAR_ADC_12BIT_0->CTR[0].B.CRES = 0U;
	stsp_SAR_ADC_12BIT_0->CTR[0].B.INPSAMP = 5U;
	stsp_SAR_ADC_12BIT_0->ICNCMR[0].R = 0x000002ECU; /* Sets channel ANP[2],ANP[3],ANP[5],ANP[6],ANP[7],ANP[9] to be
	                                           converted in SARADCB 0 (PB[0])*/
	stsp_SAR_ADC_12BIT_0->ICDR[0].R = 0x0U; /* Setup internal channel  for SARADCB,
	                                      no pre-charge */

	/* Register interrupt handler */
	/* Register interrupt handler, priority 10 */
	INTC_PSR( SPC5_SARADC12_0_INT_NUMBER ) = INTC_PSR_ENABLE( INTC_PSR_CORE0, 11 );

	stsp_SAR_ADC_12BIT_0->MCR.B.PWDN = 0; /* Power up SAR ADC B */
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void Hal_Adc_Start ( void )
{
	stsp_SAR_ADC_12BIT_0->MCR.B.NSTART = 0x1;         /* Starts CSCAN onSARADCB */
}
/*--------------------------------------------------------------------------*/
/* Function  |            																					        */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void SPC5_SARADC12_0_HANDLER ( void )
{
	IRQ_PROLOGUE();

	Hal_AdcBuffer ( );
	stsp_SAR_ADC_12BIT_0->ISR.B.NECH = 1U; /* Clear ANP[0] interrupt flag */

	IRQ_EPILOGUE ();
}
/****************************************************************************/
/**                                                                        **/
/**                      LOCAL FUNCTIONS                                   **/
/**                                                                        **/
/****************************************************************************/
/*--------------------------------------------------------------------------*/
/* Function  |            																					        */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Hal_AdcBuffer ( void )
{
	static uint16_t Index;
	static uint16_t Hist_KeyIn[ ADC_BUFFER_SIZE ];
	static uint16_t Hist_14VVCC[ ADC_BUFFER_SIZE ];
	static uint16_t Hist_Vout[ ADC_BUFFER_SIZE ];
	static uint16_t Hist_Iout[ ADC_BUFFER_SIZE ];
	static uint16_t Hist_Vin[ ADC_BUFFER_SIZE ];
	static uint16_t Hist_Temp[ ADC_BUFFER_SIZE ];

	Hist_KeyIn[ Index ] =  stsp_SAR_ADC_12BIT_0->ICDR[ 2 ].B.CDATA & 0x0FFFU ;	/* read AD conversion result from AN2 */;
	Hist_14VVCC[ Index ] =  stsp_SAR_ADC_12BIT_0->ICDR[ 3 ].B.CDATA & 0x0FFFU ;	/* read AD conversion result from AN3 */;
	Hist_Vout[ Index ] =  stsp_SAR_ADC_12BIT_0->ICDR[ 5 ].B.CDATA & 0x0FFFU;	/* read AD conversion result from AN5 */;
	Hist_Iout[ Index ] =  stsp_SAR_ADC_12BIT_0->ICDR[ 6 ].B.CDATA & 0x0FFFU;	/* read AD conversion result from AN6 */;
	Hist_Vin[ Index ] =  stsp_SAR_ADC_12BIT_0->ICDR[ 7 ].B.CDATA & 0x0FFFU;	/* read AD conversion result from AN7 */;
	Hist_Temp[ Index ] =  stsp_SAR_ADC_12BIT_0->ICDR[ 9 ].B.CDATA & 0x0FFFU;	/* read AD conversion result from AN9 */;

	Index++;
	if( Index >= ADC_BUFFER_SIZE ){ Index = 0U; }

	u16g_Hal_AverAdc_KeyIn = 2000;//Bsw_MovingAvg( Hist_KeyIn );
	u16g_Hal_AverAdc_Batt = Hal_MovingAvg( Hist_14VVCC );
	u16g_Hal_AverAdc_Vout = Hal_MovingAvg( Hist_Vout );
	u16g_Hal_AverAdc_Iout = Hal_MovingAvg( Hist_Iout );
	u16g_Hal_AverAdc_Vin = Hal_MovingAvg( Hist_Vin );
	u16g_Hal_AverAdc_Temp = Hal_MovingAvg( Hist_Temp );

	if( u16g_Hal_AverAdc_Iout >= ( IOUT_ADC_OFFSET + 1U ) )
	{
		u16g_Hal_AverAdc_Iout = ( u16g_Hal_AverAdc_Iout - IOUT_ADC_OFFSET ) ; /* Transform Q12 to Q17 */
	}
	else
	{
		u32g_Bsw_AdcIout_Q15 = 0U;
	}
}


/*--------------------------------------------------------------------------*/
/* Function  |            																						      */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static uint16_t Hal_MovingAvg ( const uint16_t ptrArrNumbers[] )
{
  uint8_t u8_cnt;
  uint16_t u16_AvgRet;
  uint32_t u32_Sum = 0;

  for( u8_cnt = 0; u8_cnt < ADC_BUFFER_SIZE; u8_cnt++ )
  {
  	u32_Sum += ptrArrNumbers[ u8_cnt ];
  }

  u16_AvgRet = ( uint16_t )( u32_Sum / ( ADC_BUFFER_SIZE ) );

  /*return the average*/
  return ( u16_AvgRet );
}
/****************************************************************************/
/**                                                                        **/
/**                      EOF                                               **/
/**                                                                        **/
/****************************************************************************/
