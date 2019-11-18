/*
                             *******************
******************************* C SOURCE FILE ********************************
**                           *******************                            **
**                                                                          **
**  Project   : LDC_7kW                                                     **
**  Filename  : Bsw_Adc.c                                                   **
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

#define BSW_ADC_C_SRC

/****************************************************************************/
/**                                                                        **/
/**                      MODULES USED                                      **/
/**                                                                        **/
/****************************************************************************/
#include "Bsw_Adc.h"

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
static int32_t Bsw_ADC2REAL_TEMP ( uint16_t AverAdc, const uint16_t adcTable[19], const uint16_t realTable[ 19 ] );

/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED VARIABLES                                **/
/**                                                                        **/
/****************************************************************************/
uint16_t u16_Vout[ 19 ] = { 110,120,130,140,150,160,170,180,190,200,210,220,230,250,270,290,310,330,350 };
uint16_t u16_AdcVout[ 19 ] = { 1185,1292,1401,1508,1616,1723,1832,1940,2045,2156,2264,2371,2484,2692,2907,3118,3386,3584,3768 };
uint16_t u16_AdcIout[ 19 ] = { 415,576,737,901,1066,1230,1393,1557,1726,1887,2243,2386,2551,2717,2883,3050,3203,3353,3503 };
uint16_t u16_Iout[ 19 ] = { 0,100,200,300,400,500,600,700,800,900,1100,1200,1300,1400,1500,1600,1700,1800,1900 };

uint16_t u16g_Bsw_realVout = 0U;
uint16_t u16g_Bsw_realIout = 0U;
uint16_t u16g_Bsw_realVin= 0U;
uint16_t u16g_Bsw_realTemp= 0U;
uint16_t u16g_Bsw_realBatt= 0U;

volatile uint32_t u32g_Bsw_AdcVout_Q15 = 0U;
volatile uint32_t u32g_Bsw_AdcIout_Q15 = 0U;

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
void Bsw_AdcInit( void )
{
	Hal_AdcInit();
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
void Bsw_ADC2REAL ( void )
{
#if TEST_VARIABLE
	u16g_Bsw_realBatt = 280;
	u16g_Bsw_realIout = 0;
	u16g_Bsw_realTemp = 60;
	u16g_Bsw_realVin = 365;
//	u16g_Bsw_realVout = u16g_Hal_AverAdc_Vout / 20;
#else
	u16g_Bsw_realBatt = u16g_Hal_AverAdc_Batt / 20;
	u16g_Bsw_realIout = u16g_Hal_AverAdc_Iout / 20;
	u16g_Bsw_realTemp = u16g_Hal_AverAdc_Temp / 40;
	u16g_Bsw_realVin = u16g_Hal_AverAdc_Vin / 20;
	u16g_Bsw_realVout = u16g_Hal_AverAdc_Vout / 20;
#endif

}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static int32_t Bsw_ADC2REAL_TEMP ( uint16_t AverAdc, const uint16_t adcTable[19], const uint16_t realTable[ 19 ] )
{
	uint8_t u8_Idx;
	int32_t tmp_1;
	int32_t tmp_2;
	int32_t tmp_3;
	int32_t temp = 0;

	if( AverAdc < adcTable[ 0 ] )
	{
		temp = ( int32_t )realTable[ 0 ];
	}
	else
	{
		if( ( AverAdc >= adcTable[ 0 ]) && ( AverAdc < adcTable[ 1 ] ) ){ u8_Idx = 0U; }
		else if( ( AverAdc >= adcTable[ 1 ] ) && ( AverAdc < adcTable[ 2 ] ) ){ u8_Idx = 1U; }
		else if( ( AverAdc >= adcTable[ 2 ] ) && ( AverAdc < adcTable[ 3 ] ) ){ u8_Idx = 2U; }
		else if( ( AverAdc >= adcTable[ 3 ] ) && ( AverAdc < adcTable[ 4 ] ) ){ u8_Idx = 3U; }
		else if( ( AverAdc >= adcTable[ 4 ] ) && ( AverAdc < adcTable[ 5 ] ) ){ u8_Idx = 4U; }
		else if( ( AverAdc >= adcTable[ 5 ] ) && ( AverAdc < adcTable[ 6 ] ) ){ u8_Idx = 5U; }
		else if( ( AverAdc >= adcTable[ 6 ] ) && ( AverAdc < adcTable[ 7 ] ) ){ u8_Idx = 6U; }
		else if( ( AverAdc >= adcTable[ 7 ] ) && ( AverAdc < adcTable[ 8 ] ) ){ u8_Idx = 7U; }
		else if( ( AverAdc >= adcTable[ 8 ] ) && ( AverAdc < adcTable[ 9 ] ) ){ u8_Idx = 8U; }
		else if( ( AverAdc >= adcTable[ 9 ] ) && ( AverAdc < adcTable[ 10 ] ) ){ u8_Idx = 9U; }
		else if( ( AverAdc >= adcTable[ 10 ] ) && ( AverAdc < adcTable[ 11 ] ) ){ u8_Idx = 10U;	}
		else if( ( AverAdc >= adcTable[ 11 ] ) && ( AverAdc < adcTable[ 12 ] ) ){ u8_Idx = 11U;	}
		else if( ( AverAdc >= adcTable[ 12 ] ) && ( AverAdc < adcTable[ 13 ] ) ){ u8_Idx = 12U;	}
		else if( ( AverAdc >= adcTable[ 13 ] ) && ( AverAdc < adcTable[ 14 ] ) ){ u8_Idx = 13U;	}
		else if( ( AverAdc >= adcTable[ 14 ] ) && ( AverAdc < adcTable[ 15 ] ) ){ u8_Idx = 14U;	}
		else if( ( AverAdc >= adcTable[ 15 ] ) && ( AverAdc < adcTable[ 16 ] ) ){ u8_Idx = 15U; }
		else if( ( AverAdc >= adcTable[ 16 ] ) && ( AverAdc < adcTable[ 17 ] ) ){ u8_Idx = 16U;	}
		else if( ( AverAdc >= adcTable[ 17 ] ) && ( AverAdc < adcTable[ 18 ] ) ){ u8_Idx = 17U; }
		else{ u8_Idx = 18U; }

		if( u8_Idx <= 17U )
		{
			tmp_1 = ( ( int32_t )realTable[ u8_Idx + 1U ] - ( int32_t )realTable[ u8_Idx ] ) * ( ( int32_t )AverAdc - ( int32_t )adcTable[ u8_Idx ] );
			tmp_2 = ( int32_t )( ( int32_t )adcTable[ u8_Idx + 1U ] - ( int32_t )adcTable[ u8_Idx ] );
			tmp_3 = ( int32_t )( tmp_1 / tmp_2 );
			temp = ( int32_t )( ( int32_t )realTable[ u8_Idx ] + tmp_3 );
		}
		else
		{
			temp =  ( int32_t )realTable[ 18 ];
		}
	}

	return ( temp );
}
/****************************************************************************/
/**                                                                        **/
/**                      EOF                                               **/
/**                                                                        **/
/****************************************************************************/
