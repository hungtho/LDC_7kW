/*
                             *******************
******************************* C SOURCE FILE ********************************
**                           *******************                            **
**                                                                          **
**  Project   : LDC_7kW                                                     **
**  Filename  : Bsw_Can.c                                                   **
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

#define BSW_CAN_C_SRC

/****************************************************************************/
/**                                                                        **/
/**                      MODULES USED                                      **/
/**                                                                        **/
/****************************************************************************/
#include "Bsw_Can.h"

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
static void Bsw_StoreDM1Buffer ( uint8_t u8_buffer[], uint8_t u8_dtcnumber );
static void TP_Frame ( uint8_t u8_tp_frame_count );
static void Bsw_DM1Buffer_Updating ( void );
static uint8_t Bsw_Total_DTC ( void );
static void Bsw_SortArray( uint8_t arr[], uint16_t N );

/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED VARIABLES                                **/
/**                                                                        **/
/****************************************************************************/
uint8_t u8g_Bsw_can1ErrStatus = 0U;
uint8_t u8g_Bsw_can2ErrStatus = 0U;

/****************************************************************************/
/**                                                                        **/
/**                      LOCAL VARIABLES                                   **/
/**                                                                        **/
/****************************************************************************/
static uint8_t u8_FFCount = 0;
static uint8_t u16s_dm1_meg_cnt	= 0U;
static uint8_t u8s_TpDt_cnt = 0U;
static uint8_t CAN_Buffer[ DM1_FRAME_MAX ];
static uint8_t DM1_Array[ DTC_CODE_NUMBER ];

static CAN_MESSAGE_OBJECT_t g_can_TxData_18FF041A;
static CAN_MESSAGE_OBJECT_t g_can_TxData_18FECA1A;
static CAN_MESSAGE_OBJECT_t g_can_TxData_18ECFF1A; /* BAM Message */
static CAN_MESSAGE_OBJECT_t g_can_TxData_18EBFF1A; /* TP_DP Message */

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
void Bsw_CAN1_Init ( void )
{
	Hal_CAN1_Init();
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void Bsw_CAN2_Init ( void )
{
	Hal_CAN2_Init();
}
/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void Bsw_Can_Tx ( void )
{
	/* Transmitted message */
	g_can_TxData_18FF041A.ID = 0x18FF041A;
	g_can_TxData_18FF041A.LENGTH = 8U;
	g_can_TxData_18FF041A.IDE = 1U;

	g_can_TxData_18FF041A.Data[ 0 ] = ( LDC_STS_READY | LDC_STS_RUN );

	g_can_TxData_18FF041A.Data[ 1 ] = ( uint8_t )( u16g_Bsw_realVin & 0x00FFU );
	g_can_TxData_18FF041A.Data[ 2 ] = ( uint8_t )( ( u16g_Bsw_realVin & 0xFF00U ) >> 8U );

	g_can_TxData_18FF041A.Data[ 3 ] = ( uint8_t )( ( u16g_Bsw_realVout * 10U ) & 0x00FFU );
	g_can_TxData_18FF041A.Data[ 4 ] = ( uint8_t )( ( u16g_Bsw_realVout * 10U ) >> 8U );

	g_can_TxData_18FF041A.Data[ 5 ] = ( uint8_t )( u16g_Bsw_realIout & 0x00FFU );
	g_can_TxData_18FF041A.Data[ 6 ] = ( uint8_t )( ( u16g_Bsw_realIout & 0xFF00U ) >> 8U );

	g_can_TxData_18FF041A.Data[ 7 ] = ( uint8_t )u16g_Bsw_realTemp;

	if( u8g_CAN_TX_Ctrl == CAN_CTRL_ENABLE )
	{
		CAN1_Drv_SendFrame ( TX_MB8, g_can_TxData_18FF041A );
		CAN2_Drv_SendFrame ( TX_MB8, g_can_TxData_18FF041A );
	}
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void Bsw_Can_Rx ( void )
{
	if( u8g_CAN_RX_Ctrl == CAN_CTRL_ENABLE )
	{
		u8g_Asw_LDC_ENB = 0U;

		if( Asw_GetIgnSts() == 1U )
		{
			if( ( g_can_RxData_18FF31EF.Data[ 0 ] & 0x02U ) != 0x00U )
			{
				u8g_Asw_LDC_ENB = 1U;
#if OPEN_LOOP
				Vcu_PhaseShift = g_can_RxData_18FF31EF.Data[ 3 ] << 8U;
				Vcu_PhaseShift = Vcu_PhaseShift + g_can_RxData_18FF31EF.Data[ 2 ];
#endif
			}
		}
	}

	if( u32g_Hal_TimeoutCan1Cnt >= 50U )
	{
		u8g_Bsw_can1ErrStatus = 1U;
	}
	else
	{
		u8g_Bsw_can1ErrStatus = 0U;
		u32g_Hal_TimeoutCan1Cnt++;
	}

	if( u32g_Hal_TimeoutCan2Cnt >= 50U )
	{
		u8g_Bsw_can2ErrStatus = 1U;
	}
	else
	{
		u8g_Bsw_can2ErrStatus = 0U;
		u32g_Hal_TimeoutCan2Cnt++;
	}
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void Bsw_Can_DM1_Init ( void )
{
	g_can_TxData_18FECA1A.ID = 0x18FECA1A; /* DM1 Message */
	g_can_TxData_18FECA1A.LENGTH = 8U;
	g_can_TxData_18FECA1A.IDE = 1U;

	g_can_TxData_18ECFF1A.ID = 0x18ECFF1A; /* BAM Message */
	g_can_TxData_18ECFF1A.LENGTH = 8U;
	g_can_TxData_18ECFF1A.IDE = 1U;

	g_can_TxData_18EBFF1A.ID = 0x18EBFF1A; /* TP_DP Message */
	g_can_TxData_18EBFF1A.LENGTH = 8U;
	g_can_TxData_18EBFF1A.IDE = 1U;
}


/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void Bsw_Can_DM1_Transmitter ( void )
{
	static uint8_t dtc_number = 0U;
	static uint8_t dtc_number_bak = 0U;
	static uint16_t u16_1scnt = 0U;
	static uint32_t u32s_LDC_ERR_Sts_bak = 0;
	static uint8_t u8_numberofbytes = 0U;
	static uint8_t u8_numberofpackets = 0U;
	uint8_t u8_TpDt_Req = 0U;

	dtc_number = Bsw_Total_DTC ();

	if ( u32s_LDC_ERR_Sts_bak != u32g_LDC_ERR_Sts )
	{
		Bsw_DM1Buffer_Updating();
		Bsw_StoreDM1Buffer ( CAN_Buffer, dtc_number );

		u32s_LDC_ERR_Sts_bak = u32g_LDC_ERR_Sts;
	}

	if( dtc_number_bak != dtc_number )
	{
		if( dtc_number == 2U )
		{
			u8_numberofbytes = 0x0AU;
		}
		else if( dtc_number == 3U )
		{
			u8_numberofbytes = 0x0E;
		}
		else
		{
			u8_numberofbytes = ( dtc_number * 4U ) + 2U;
		}

		u8_numberofpackets = ( uint8_t)( ( u8_FFCount / 7U ) + 1U );

		dtc_number_bak = dtc_number;
	}

	u8s_TpDt_cnt++;
	if( u8s_TpDt_cnt >= 6U )
	{
		u8s_TpDt_cnt = 0U;
		u8_TpDt_Req = 1U;
	}

	switch( dtc_number )
	{
		case 0:
			if( u16s_dm1_meg_cnt == 0U ) /* DM1 Message */
			{
				g_can_TxData_18FECA1A.Data[ 0U ] = 0x00U;
				g_can_TxData_18FECA1A.Data[ 1U ] = 0xFFU;
				g_can_TxData_18FECA1A.Data[ 2U ] = 0x00U;
				g_can_TxData_18FECA1A.Data[ 3U ] = 0x00U;
				g_can_TxData_18FECA1A.Data[ 4U ] = 0x00U;
				g_can_TxData_18FECA1A.Data[ 5U ] = 0xFFU;
				g_can_TxData_18FECA1A.Data[ 6U ] = 0xFFU;
				g_can_TxData_18FECA1A.Data[ 7U ] = 0xFFU;

				CAN1_Drv_SendFrame ( TX_MB9, g_can_TxData_18FECA1A );
				CAN2_Drv_SendFrame ( TX_MB9, g_can_TxData_18FECA1A );

				u16s_dm1_meg_cnt++;
			}
			break;
		case 1:
			if( u16s_dm1_meg_cnt == 0U ) /* DM1 Message */
			{
				g_can_TxData_18FECA1A.Data[ 0U ] = 0x04U; /* Warning Lamp */
				g_can_TxData_18FECA1A.Data[ 1U ] = 0xFFU;
				g_can_TxData_18FECA1A.Data[ 2U ] = DTC_HIGH;
				g_can_TxData_18FECA1A.Data[ 3U ] = DTC_MIDDLE;
				g_can_TxData_18FECA1A.Data[ 4U ] = DM1_Array[ 0 ];
				g_can_TxData_18FECA1A.Data[ 5U ] = 0xFFU;
				g_can_TxData_18FECA1A.Data[ 6U ] = 0xFFU;
				g_can_TxData_18FECA1A.Data[ 7U ] = 0xFFU;

				CAN1_Drv_SendFrame ( TX_MB9, g_can_TxData_18FECA1A );
				CAN2_Drv_SendFrame ( TX_MB9, g_can_TxData_18FECA1A );

				u16s_dm1_meg_cnt++;
			}
			break;
		default:
			if( u16s_dm1_meg_cnt == 0U ) /* BAM Message */
			{
				g_can_TxData_18ECFF1A.Data[ 0U ] = 0x20U; /* Broadcast Announce Message */
				g_can_TxData_18ECFF1A.Data[ 1U ] = 0x00U; /* Total message size, number of byte */
				g_can_TxData_18ECFF1A.Data[ 2U ] = u8_numberofbytes; /* Total message size, number of byte */
				g_can_TxData_18ECFF1A.Data[ 3U ] = u8_numberofpackets; /* Total number of packets */
				g_can_TxData_18ECFF1A.Data[ 4U ] = 0xFFU; /* Reserved for assignment by SAE, filled with FF */
				g_can_TxData_18ECFF1A.Data[ 5U ] = 0x00U;
				g_can_TxData_18ECFF1A.Data[ 6U ] = 0xFEU;
				g_can_TxData_18ECFF1A.Data[ 7U ] = 0xCAU;

				CAN1_Drv_SendFrame ( TX_MB9, g_can_TxData_18ECFF1A );
				CAN2_Drv_SendFrame ( TX_MB9, g_can_TxData_18ECFF1A );

				u16s_dm1_meg_cnt++;
				u8s_TpDt_cnt = 0U;
			}
			else if( ( u16s_dm1_meg_cnt > 0U ) && ( u16s_dm1_meg_cnt <= ( ( u8_FFCount / 7U ) + 1U ) ) && ( u8_TpDt_Req == 1U ) ) /* TP_DT Message */
			{
				TP_Frame ( u16s_dm1_meg_cnt );
				u16s_dm1_meg_cnt++;
			}
			else{
			}

			break;
	}

	u16_1scnt++;
	if( u16_1scnt >= 100U )
	{
		u16_1scnt = 0U;
		u16s_dm1_meg_cnt = 0U;
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
static void Bsw_StoreDM1Buffer ( uint8_t u8_buffer[], uint8_t u8_dtcnumber )
{
	static uint8_t u8_zerocount = 0U;
	uint8_t u8_k = 0U;
	uint8_t u8_j = 0U;
	uint8_t u8_dtcMax;

	u8_dtcMax = u8_dtcnumber;

	if( u8_dtcMax > 1U )
	{
		u8_buffer[ 0U ] =  0x04U;
		u8_buffer[ 1U ] =  0xFFU;

		for( u8_k = 0U; u8_k < u8_dtcMax; u8_k++ )
		{
			u8_buffer[ ( u8_k * 4U ) + 2U ] = DTC_HIGH;
			u8_buffer[ ( u8_k * 4U ) + 3U ] = DTC_MIDDLE;
			u8_buffer[ ( u8_k * 4U ) + 4U ] = DM1_Array[ u8_k ];
			u8_buffer[ ( u8_k * 4U ) + 5U ] = 0xFFU;

			u8_j += 4U;
		}

		u8_FFCount = u8_j;

		while( ( ( u8_FFCount + 2U ) % 7U ) != 0U )
		{
			u8_buffer[ u8_FFCount + 2U ] = 0xFFU;
			u8_FFCount++;
		}

		u8_zerocount = u8_FFCount;

		while( u8_zerocount < DM1_FRAME_MAX )
		{
			u8_buffer[ u8_zerocount + 2U ] = 0x00U;
			u8_zerocount++;
		}
	}
	else
	{
		for( u8_j = 0U; u8_j < 21U; u8_j++ )
		{
			u8_buffer[ u8_j ] = 0U;
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
static void TP_Frame ( uint8_t u8_tp_frame_count )
{
	uint8_t tp_frame_Idx;

	tp_frame_Idx = ( u8_tp_frame_count - 1U ) * 7U;

	g_can_TxData_18EBFF1A.Data[ 0 ] = u8_tp_frame_count;
	g_can_TxData_18EBFF1A.Data[ 1 ] = CAN_Buffer[ tp_frame_Idx ];
	g_can_TxData_18EBFF1A.Data[ 2 ] = CAN_Buffer[ tp_frame_Idx + 1U ];
	g_can_TxData_18EBFF1A.Data[ 3 ] = CAN_Buffer[ tp_frame_Idx + 2U ]; /* 0xFF */
	g_can_TxData_18EBFF1A.Data[ 4 ] = CAN_Buffer[ tp_frame_Idx + 3U ]; /* 0xFF */
	g_can_TxData_18EBFF1A.Data[ 5 ] = CAN_Buffer[ tp_frame_Idx + 4U ]; /* 0xFF */
	g_can_TxData_18EBFF1A.Data[ 6 ] = CAN_Buffer[ tp_frame_Idx + 5U ]; /* 0xFF */
	g_can_TxData_18EBFF1A.Data[ 7 ] = CAN_Buffer[ tp_frame_Idx + 6U ]; /* 0xFF */

	CAN1_Drv_SendFrame ( TX_MB9, g_can_TxData_18EBFF1A );
	CAN2_Drv_SendFrame ( TX_MB9, g_can_TxData_18EBFF1A );
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Bsw_DM1Buffer_Updating ( void )
{
	uint8_t u8s_i = 0U;
	uint32_t u32_tmp = 0;

	for( u8s_i = 0; u8s_i < DTC_CODE_NUMBER; u8s_i++ )
	{
		u32_tmp = 0x00000001U << u8s_i;

		/* Save DTC_Code into DTC array buffer */
		if( ( u32_tmp & u32g_LDC_ERR_Sts ) != 0U )
		{
			DM1_Array[ u8s_i ] = u8g_Asw_DTC_CODE_Array[ u8s_i ];
		}
		else
		{

			DM1_Array[ u8s_i ] = 0x00U;
		}
	}

	Bsw_SortArray( DM1_Array, DTC_CODE_NUMBER );
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static uint8_t Bsw_Total_DTC ( void )
{
	uint8_t temp_total_dtc = 0U;

	temp_total_dtc  = Dia_Batt.Err_Over + Dia_Batt.Err_Under + Dia_Vin.Err_Over + Dia_Vin.Err_Under +
										Dia_Vout.Err_Over + Dia_Vout.Err_Under + Dia_Iout_Sens.Err_Open + Dia_Iout_Sens.Err_Short +
										Dia_Temp_Sens.Err_Open + Dia_Temp_Sens.Err_Short + Dia_CAN.CAN1_Busoff + Dia_CAN.CAN1_Timeout +
										Dia_CAN.CAN2_Busoff + Dia_CAN.CAN2_Timeout + Dia_Iout.Err_Over + Dia_Iout.Err_Shutdown + Dia_Temp.Err_Over +
										Dia_Temp.Err_Under + Dia_Temp.Derating;

	return ( temp_total_dtc );
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void Bsw_SortArray( uint8_t arr[], uint16_t N )
{
	uint16_t t = 0U;
	uint16_t i = 0U;
	uint16_t j = 0U;

	for( i = 0; i< ( N-1 ); i++ )
	{
		for( j = ( i+1 ); j < N; j++ )
		{
			if( arr[ i ] < arr[ j ] )
			{
				 t = arr[ i ];
				 arr[ i ] = arr[ j ];
				 arr[ j ]= t;
			}
		}
	}
}

/****************************************************************************/
/**                                                                        **/
/**                      EOF                                               **/
/**                                                                        **/
/****************************************************************************/
