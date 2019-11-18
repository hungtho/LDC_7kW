/*
                             *******************
******************************* C SOURCE FILE ********************************
**                           *******************                            **
**                                                                          **
**  Project   : KSC_LDC                                                     **
**  Filename  : Asw_Utility.c                                               **
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

#define ASW_UTILITY_C_SRC

/****************************************************************************/
/**                                                                        **/
/**                      MODULES USED                                      **/
/**                                                                        **/
/****************************************************************************/
#include "Asw_Utility.h"

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
const uint8_t u8g_Asw_DTC_CODE_Array[ DTC_CODE_NUMBER ] =
{
	0x23, 0x24, 0x03, 0x04, 0x13, 0x14, 0x34, 0x33,
	0x4E, 0x40, 0x41, 0x45, 0x46, 0x35, 0x36, 0x52,
	0x62, 0x12, 0x56, 0x66
};

uint32_t u32g_Asw_EEPROM_LAST_BUFFER_DTC[ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
uint32_t u32g_Asw_EEPROM_BUFFER_DTC[ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };

uint32_t u32g_Asw_EEPROM_BUFFER_DID[ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
uint32_t u32g_Asw_EEPROM_LAST_BUFFER_DID[ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };

uint32_t u32g_Asw_present_addr;

uint8_t u8g_Asw_Ldc_State_Ready = 0U;
uint8_t u8g_Asw_Ldc_State_Run = 0U;
uint8_t u8g_Asw_LDC_ENB = 0U;

uint32_t u32g_Asw_IoReference = 0U;
uint32_t u32g_Asw_VoReference = 0U;
uint8_t u8g_Asw_WriteEEPROM_Flag = 0U;
#if OPEN_LOOP
uint16_t Vcu_PhaseShift = 0;
#endif
#if 0
LdcDiagKeyIn DiagKeyIn = {0,};
#endif

/****************************************************************************/
/**                                                                        **/
/**                      GLOBAL VARIABLES                                  **/
/**                                                                        **/
/****************************************************************************/

/****************************************************************************/
/**                                                                        **/
/**                      PROTOTYPES OF LOCAL FUNCTIONS                     **/
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
uint8_t Asw_GetIgnSts ( void )
{
	uint8_t u8_Ret = 0U;

//	if( u16g_B_AverAdc_KeyIn >= 1400U )
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
void Asw_FindCurrentAdd ( void )
{
	uint32_t u32_add = BLOCK0_ADDR;
	uint8_t u8_foundFresentAdd_Flag = 0U;

	uint32_t u32_eep_tmp;
	uint32_t u32_eep_tmp1;
	uint32_t u32_eep_tmp2;
	uint32_t u32_eep_tmp3;

	if( UDS_EEP_ReadDataByIdentifier( BLOCK2_ADDR - 0x40U ) != 0xFFFFFFFFU )
	{
		u32_add = BLOCK0_ADDR;
	}
	else
	{
		if( UDS_EEP_ReadDataByIdentifier( BLOCK1_ADDR ) == 0xFFFFFFFFU )
		{
			u32_add = BLOCK0_ADDR;
		}
		else
		{
			u32_add = BLOCK1_ADDR;
		}
	}

	while( u8_foundFresentAdd_Flag != 1U )
	{
		u32_eep_tmp = UDS_EEP_ReadDataByIdentifier( u32_add );
		u32_eep_tmp1 = UDS_EEP_ReadDataByIdentifier( u32_add + 0x4U );
		u32_eep_tmp2 = UDS_EEP_ReadDataByIdentifier( u32_add + 0x8U );
		u32_eep_tmp3 = UDS_EEP_ReadDataByIdentifier( u32_add + 0xCU );

		if( ( u32_eep_tmp & u32_eep_tmp1 & u32_eep_tmp2 & u32_eep_tmp3 ) != 0xFFFFFFFFU )
		{
			u32_add += 0x40U;
		}
		else
		{
			u8_foundFresentAdd_Flag = 1U;
			u32g_Asw_present_addr = u32_add;
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
void Asw_DTC_Eeprom_Read ( uint32_t addr )
{
	uint8_t ee_i;
	uint32_t ee_blk_addr;
	uint32_t ee_addr;
	uint32_t ee_read;

	if( addr == BLOCK0_ADDR ){ ee_blk_addr = BLOCK2_ADDR - 0x40U; }
	else{ ee_blk_addr = addr - 0x40U; }

	for( ee_i = 0U; ee_i < 8U; ee_i++ )
	{
		ee_addr = ee_blk_addr + ( ( uint32_t )ee_i * 0x04U );

		if( ( ee_addr >= BLOCK0_ADDR ) && ( addr < ( BLOCK2_ADDR - 0x20U ) ) )
		{
			ee_read = UDS_EEP_ReadDataByIdentifier( ee_addr );

			if( ee_read != 0xFFFFFFFFU )
			{
				u32g_Asw_EEPROM_BUFFER_DTC[ ee_i ] = ee_read;
				u32g_Asw_EEPROM_LAST_BUFFER_DTC[ ee_i ] = ee_read;
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
void Asw_DTC_Eeprom_Write ( void )
{
	static uint32_t u32s_LDC_ERR_Sts_bku = 0;

	uint8_t u8s_i = 0U;
	uint8_t u8s_j = 0U;
	uint32_t u32_tmp = 0;
	uint8_t u8_BufIdx;
	uint8_t u8_BufPtr;
	uint32_t u32_BufSts;

	if ( u32s_LDC_ERR_Sts_bku != u32g_LDC_ERR_Sts )
	{
		for( u8s_i = 0U; u8s_i < DTC_CODE_NUMBER; u8s_i++ )
		{
			u32_tmp = 0x00000001U << u8s_i;
			u8_BufIdx = ( uint8_t )( u8s_i / 4U );
			u8_BufPtr = ( uint8_t )( u8s_i % 4U );

			/* Save DTC_Code into DTC array buffer */
			if( ( u32_tmp & u32g_LDC_ERR_Sts ) != 0U )
			{
				u32_BufSts = ( uint32_t )0x09 << ( u8_BufPtr * 8U );
				u32g_Asw_EEPROM_BUFFER_DTC[ u8_BufIdx ] |= u32_BufSts;
			}
			else
			{
				u32_BufSts = ( uint32_t )0x01 << ( u8_BufPtr * 8U );
				u32g_Asw_EEPROM_BUFFER_DTC[ u8_BufIdx ] &= ( ~u32_BufSts );
			}
		}

		for( u8s_j = 0; u8s_j < 8U; u8s_j++ )
		{
			if( u32g_Asw_EEPROM_BUFFER_DTC [ u8s_j ] != u32g_Asw_EEPROM_LAST_BUFFER_DTC [ u8s_j ] )
			{
				u32g_Asw_EEPROM_LAST_BUFFER_DTC [ u8s_j ] = u32g_Asw_EEPROM_BUFFER_DTC [ u8s_j ];
				u8g_Asw_WriteEEPROM_Flag = 1U;
			}
		}

		u32s_LDC_ERR_Sts_bku = u32g_LDC_ERR_Sts;
	}
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void Asw_DID_Eeprom_Read ( uint32_t addr )
{
	uint8_t ee_i;
	uint32_t ee_blk_addr;
	uint32_t ee_addr;
	uint32_t ee_read;

	if( addr == BLOCK0_ADDR ){ ee_blk_addr = BLOCK2_ADDR - 0x20U; }
	else{ ee_blk_addr = addr - 0x20U; }

	for( ee_i = 0U; ee_i < 8U; ee_i++ )
	{
		ee_addr = ee_blk_addr + ( ( uint32_t )ee_i * 0x04U );

		if( ( ee_addr >= BLOCK0_ADDR ) && ( addr < BLOCK2_ADDR ) )
		{
			ee_read = UDS_EEP_ReadDataByIdentifier( ee_addr );

			if( ee_read != 0xFFFFFFFFU )
			{
				u32g_Asw_EEPROM_BUFFER_DID[ ee_i ] = ee_read;
				u32g_Asw_EEPROM_LAST_BUFFER_DID[ ee_i ] = ee_read;
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
void Asw_DID_Eeprom_Write ( void )
{
	uint8_t ee_j;

	/* Store new DID EEPROM buffer */
	for( ee_j = 0U; ee_j< 8U; ee_j++ )
	{
		u32g_Asw_EEPROM_BUFFER_DID[ ee_j ] = ( uint32_t )( ( ( uint32_t )u8g_Eeprom_WriteData[ 3U + ( 4U * ee_j ) ] << 24U )
																						| ( ( uint32_t )u8g_Eeprom_WriteData[ 2U + ( 4U * ee_j ) ] << 16U )
																						| ( ( uint32_t )u8g_Eeprom_WriteData[ 1U + ( 4U * ee_j ) ] << 8U )
																						| ( ( uint32_t )u8g_Eeprom_WriteData[ 0U + ( 4U * ee_j ) ] ) );

		if( u32g_Asw_EEPROM_BUFFER_DID[ ee_j ] != u32g_Asw_EEPROM_LAST_BUFFER_DID[ ee_j ] )
		{
			u32g_Asw_EEPROM_LAST_BUFFER_DID[ ee_j ] = u32g_Asw_EEPROM_BUFFER_DID[ ee_j ];
			u8g_Asw_WriteEEPROM_Flag = 1U;
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
uint32_t UTILS_REF_INV ( uint16_t Ref, const uint16_t AdcValue[ 19 ], const uint16_t realValue[ 19 ] )
{
	uint8_t u8_Idx;
	uint16_t tmp_1;
	uint16_t tmp_2;
	int16_t tmp_3;
	uint32_t temp = 0;

	if( Ref < realValue[ 1 ] ){ u8_Idx = 0U; }
	else if( ( Ref >= realValue[ 1 ] ) && ( Ref < realValue[ 2 ] ) ){ u8_Idx = 1U; }
	else if( ( Ref >= realValue[ 2 ] ) && ( Ref < realValue[ 3 ] ) ){ u8_Idx = 2U; }
	else if( ( Ref >= realValue[ 3 ] ) && ( Ref < realValue[ 4 ] ) ){ u8_Idx = 3U; }
	else if( ( Ref >= realValue[ 4 ] ) && ( Ref < realValue[ 5 ] ) ){ u8_Idx = 4U; }
	else if( ( Ref >= realValue[ 5 ] ) && ( Ref < realValue[ 6 ] ) ){ u8_Idx = 5U; }
	else if( ( Ref >= realValue[ 6 ] ) && ( Ref < realValue[ 7 ] ) ){ u8_Idx = 6U;}
	else if( ( Ref >= realValue[ 7 ] ) && ( Ref < realValue[ 8 ] ) ){ u8_Idx = 7U; }
	else if( ( Ref >= realValue[ 8 ] ) && ( Ref < realValue[ 9 ] ) ){ u8_Idx = 8U; }
	else if( ( Ref >= realValue[ 9 ] ) && ( Ref < realValue[ 10 ] ) ){ u8_Idx = 9U; }
	else if( ( Ref >= realValue[ 10 ] ) && ( Ref < realValue[ 11 ] ) ){ u8_Idx = 10U; }
	else if( ( Ref >= realValue[ 11 ] ) && ( Ref < realValue[ 12 ] ) ){ u8_Idx = 11U; }
	else if( ( Ref >= realValue[ 12 ] ) && ( Ref < realValue[ 13 ] ) ){ u8_Idx = 12U; }
	else if( ( Ref >= realValue[ 13 ] ) && ( Ref < realValue[ 14 ] ) ){ u8_Idx = 13U;	}
	else if( ( Ref >= realValue[ 14 ] ) && ( Ref < realValue[ 15 ] ) ){ u8_Idx = 14U;	}
	else if( ( Ref >= realValue[ 15 ] ) && ( Ref < realValue[ 16 ] ) ){ u8_Idx = 15U;	}
	else if( ( Ref >= realValue[ 16 ] ) && ( Ref < realValue[ 17 ] ) ){ u8_Idx = 16U;	}
	else if( ( Ref >= realValue[ 17 ] ) && ( Ref < realValue[ 18 ] ) ){ u8_Idx = 17U;	}
	else{ u8_Idx = 18U; }

	if( u8_Idx <= 17U )
	{
		tmp_1 = ( AdcValue[ u8_Idx + 1U ] - AdcValue[ u8_Idx ] ) * ( Ref - realValue[ u8_Idx ] );
		tmp_2 = realValue[ u8_Idx + 1U ] - realValue[ u8_Idx ];
		tmp_3 = ( int16_t )( tmp_1 / tmp_2 );
		temp = ( ( uint32_t )AdcValue[ u8_Idx ] + ( uint32_t )tmp_3 );
	}
	else
	{
		temp = AdcValue[ 18 ];
	}

	return ( temp );
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void Asw_IoRef_TemperDerating ( uint16_t realTemper )
{
	static uint8_t overTempFlag = 0U;

	if( overTempFlag == 0U )
	{
		if( realTemper < 135U ) /* 95 degree */
		{
//			u32g_Asw_IoReference = 84512U; //( UTILS_REF_INV ( CUR_REF, u16_AdcIout, u16_Iout ) - IOUT_ADC_OFFSET ) << 5U;
		}
		else if ( ( realTemper < 145U ) && ( realTemper >= 135U ) ) /* 95 - 105 degree */
		{
			u32g_Asw_IoReference = 68000U;//( UTILS_REF_INV ( CUR_REF_95, u16_AdcIout, u16_Iout ) - IOUT_ADC_OFFSET ) << 5U;
		}
		else if ( ( realTemper < 155U ) && ( realTemper >= 145U ) ) /* 105 - 115 degree */
		{
			u32g_Asw_IoReference = 42144U;//( UTILS_REF_INV ( CUR_REF_105, u16_AdcIout, u16_Iout ) - IOUT_ADC_OFFSET ) << 5U;
		}
		else /* over 115 degree */
		{
			u32g_Asw_IoReference = 0U;//( UTILS_REF_INV ( CUR_REF_115, u16_AdcIout, u16_Iout ) - IOUT_ADC_OFFSET ) << 5U;
			overTempFlag = 1U;
		}
	}

	if ( overTempFlag == 1U )
	{
		/* 85 degree */
		if( realTemper < 125U )
		{
			overTempFlag = 0U;
			u32g_Asw_IoReference = 84512U; //( UTILS_REF_INV ( CUR_REF, u16_AdcIout, u16_Iout ) - IOUT_ADC_OFFSET ) << 5U;
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
uint16_t Asw_LDC_OFF ( uint16_t u16_pwmshift )
{
	uint16_t u16_temp = 0U;

	u16_temp = u16_pwmshift;

	if( u16_temp > 3U )
	{
		u16_temp = u16_temp - 3U;
	}
	else
	{
		u16_temp = 0;
		Bsw_PwmDisable();
	}

	return ( u16_temp );
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



/****************************************************************************/
/**                                                                        **/
/**                      EOF                                               **/
/**                                                                        **/
/****************************************************************************/
