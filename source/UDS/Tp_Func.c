/*
                             *******************
******************************* C SOURCE FILE ********************************
**                           *******************                            **
**                                                                          **
**  Project   : SPC570S_FBL                                                 **
**  Filename  : Tp_Func.c                                                   **
**  Version   : -.- (PCB : )                                                **
**  Date      : 2019.05.10                                                  **
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
Revised by  : Im, Hong Kyun
Description : Original version.

*/

#define TP_FUNC_C_SRC

/****************************************************************************/
/**                                                                        **/
/**                      MODULES USED                                      **/
/**                                                                        **/
/****************************************************************************/
#include "Tp_Func.h"
#include "Hal_Can.h"
#include "Uds_Func.h"

/****************************************************************************/
/**                                                                        **/
/**                      DEFINITIONS AND MACROS                            **/
/**                                                                        **/
/****************************************************************************/
#define TP_ST_NORMAL		( 0U )
#define TP_ST_WAIT_FC		( 1U ) /* wait flow control */
#define TP_ST_SEND_CF		( 2U ) /* send consecutive frame */

#define ST_MIN_LOW			( 1U ) /* 1ms */
#define ST_MIN_VALUE    ( 20U ) /* 20ms */
#define ST_MIN_HIGH			( 127U ) /* 127ms */
#define TP_BLOCK_SIZE		( 8U )

/****************************************************************************/
/**                                                                        **/
/**                      TYPEDEFS AND STRUCTURES                           **/
/**                                                                        **/
/****************************************************************************/
#if !defined ( TP_FUNC_MISRA ) /* MISRA_C_2012_11_03	*/
typedef struct
{
	uint8_t u4_mTPI_Type :4;	/* MSB */
	uint8_t u4_mTPI_Dl   :4;	/* LSB */
	uint8_t u8_mData[ 7 ];
}ST_TP;
#endif

/****************************************************************************/
/**                                                                        **/
/**                      PROTOTYPES OF LOCAL FUNCTIONS                     **/
/**                                                                        **/
/****************************************************************************/
static void TpFlowControlFrame ( void );
static void TpSendFrame ( const uint8_t *p_Data, uint8_t u8_Size );

/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED VARIABLES                                **/
/**                                                                        **/
/****************************************************************************/
uint8_t u8g_TpTxBuf[ MAX_TX_FRAME_SIZE ];
uint8_t u8g_TpRxBuf[ MAX_RX_FRAME_SIZE ];
uint8_t u8g_SingleRxBuf[ 8 ];
uint8_t u8g_RxFrameType;

/****************************************************************************/
/**                                                                        **/
/**                      GLOBAL VARIABLES                                  **/
/**                                                                        **/
/****************************************************************************/
static uint8_t u8s_TpTxBufIdx;
static uint8_t u8s_TpTxSeqNum;
static uint8_t u8s_TpTxDataCnt;
static uint8_t u8s_TpTxState;

static uint8_t u8s_TpRxBufIdx;
static uint8_t u8s_TpRxSeqNum;
static uint8_t u8s_TpRxDataCnt;
static uint8_t u8s_TpRxDataSize;

static uint16_t u16s_STminTickCnt;
static uint16_t u16s_STMin_tm;

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
void Tp_Func_Init ( void )
{
	u8s_TpTxBufIdx = 0U;
	u8s_TpTxSeqNum = 0U;
	u8s_TpTxDataCnt = 0U;
	u8s_TpTxState = TP_ST_NORMAL;

	u8s_TpRxBufIdx = 0U;
	u8s_TpRxSeqNum = 0U;
	u8s_TpRxDataCnt = 0U;
	u8s_TpRxDataSize = 0U;

	u16s_STminTickCnt = ST_MIN_HIGH; /* need to check */
	u16s_STMin_tm = 0U;

	return;
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void Tp_Func ( void )
{
	uint8_t u8_TpIdx;
	uint8_t u8_DataLen;

#if defined ( TP_FUNC_MISRA ) /* MISRA_C_2012_11_03	*/
	uint8_t u8_TpFrame[ 8 ];
#else
	ST_TP st_TpFrame;
#endif

	u16s_STMin_tm++;

	switch( u8s_TpTxState )
	{
		case TP_ST_NORMAL:
			break;
		case TP_ST_WAIT_FC:
			break;
		case TP_ST_SEND_CF:
			if( u16s_STMin_tm >= u16s_STminTickCnt )
			{
				u16s_STMin_tm = 0U;
				u8s_TpTxSeqNum++;

				if( u8s_TpTxSeqNum > 0x0FU ){ u8s_TpTxSeqNum = 0U; }

#if defined ( TP_FUNC_MISRA ) /* MISRA_C_2012_11_03	*/
				u8_TpFrame[ 0 ] = ( uint8_t )( FT_CF << 4U );
				u8_TpFrame[ 0 ] |= ( uint8_t )( u8s_TpTxSeqNum & 0x0FU );
#else
				st_TpFrame.u4_mTPI_Type = FT_CF;
				st_TpFrame.u4_mTPI_Dl = u8s_TpTxSeqNum;
#endif
				if( u8s_TpTxDataCnt > 7U ){ u8_DataLen = 7U; }
				else{ u8_DataLen = u8s_TpTxDataCnt; }

				for( u8_TpIdx = 0U; u8_TpIdx < u8_DataLen; u8_TpIdx++ )
				{
#if defined ( TP_FUNC_MISRA ) /* MISRA_C_2012_11_03	*/
					u8_TpFrame[ u8_TpIdx + 1U ] = u8g_TpTxBuf[ u8_TpIdx + u8s_TpTxBufIdx ];
#else
					st_TpFrame.u8_mData[ u8_TpIdx ] = u8g_TpTxBuf[ u8_TpIdx + u8s_TpTxBufIdx ];
#endif
				}
#if defined ( TP_FUNC_MISRA ) /* MISRA_C_2012_11_03	*/
				TpSendFrame ( u8_TpFrame, u8_DataLen + 1U );
#else
				TpSendFrame ( ( const uint8_t * )&st_TpFrame, u8_DataLen + 1U );
#endif
				u8s_TpTxBufIdx += u8_DataLen;
				u8s_TpTxDataCnt -= u8_DataLen;

				if( u8s_TpTxDataCnt == 0U ){ u8s_TpTxState = TP_ST_NORMAL; }
			}
			break;
		default:
			break;
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
uint8_t TpRecvProc ( uint8_t *u8p_IsEndFrame, uint8_t *u8p_RecvLen, uint8_t *u8p_ReqType )
{
	uint8_t u8_TpIdx;
	static uint8_t u8_FrameType;
	uint8_t u8_Dlc;
	uint8_t u8_DataLen;
	uint8_t u8_SeqNum;
	uint8_t u8_ErrNo = ERR_OK;
	static uint8_t u8_CANFrameData[ 8 ];
  uint8_t u8_STmin;
  uint32_t u32_CANMsgID;

  u32_CANMsgID = g_can_RxData_REQUEST.ID;
	for( u8_TpIdx = 0U; u8_TpIdx < 8U; u8_TpIdx++ )
	{
		u8_CANFrameData[ u8_TpIdx ] = g_can_RxData_REQUEST.Data[ u8_TpIdx ];
	}

	if( u32_CANMsgID == CANID_DIAG_PHYS_REQUEST ){ *u8p_ReqType = PHYS; }
	else if( u32_CANMsgID == CANID_DIAG_FUNC_REQUEST ){ *u8p_ReqType = FUNC; }
	else{ *u8p_ReqType = 0x00U; }

	if( *u8p_ReqType != 0x00U )
	{
		u8_FrameType = Get8BitHiNibble( ( u8_CANFrameData[ 0 ] ) );
		u8_Dlc = Get8BitLoNibble( ( u8_CANFrameData[ 0 ] ) );

		u8g_RxFrameType = u8_FrameType;

		switch( u8_FrameType )
		{
			case FT_SF: /* single frame */
				for( u8_TpIdx = 0U; u8_TpIdx < u8_Dlc; u8_TpIdx++ )
				{
					u8g_SingleRxBuf[ u8_TpIdx ] = u8_CANFrameData[ u8_TpIdx + 1U ];
				}

				*u8p_IsEndFrame = TRUE;
				*u8p_RecvLen = u8_Dlc;
				break;
			case FT_FF: /* first frame */
				u8s_TpRxDataSize = u8_CANFrameData[ 1 ];
				u8s_TpRxDataCnt  = u8s_TpRxDataSize;

				for( u8_TpIdx = 0U; u8_TpIdx < 6U; u8_TpIdx++ )
				{
					u8g_TpRxBuf[ u8_TpIdx ] = u8_CANFrameData[ u8_TpIdx + 2U ];
				}

				u8s_TpRxBufIdx = 6U;
				u8s_TpRxDataCnt -= 6U;
				u8s_TpRxSeqNum = 0U;

				/* send flow control frame */
				TpFlowControlFrame ();
				break;
			case FT_CF: /* consecutive frame */
				u8s_TpRxSeqNum++;
				u8_SeqNum = u8_Dlc;

				if( u8s_TpRxSeqNum > 0x0FU ){ u8s_TpRxSeqNum = 0U; }

				if( u8s_TpRxSeqNum != u8_SeqNum )
				{
					u8s_TpRxBufIdx = 0U;
					u8_ErrNo = NrcRequestSequenceError;
				}
				else
				{
					if( u8s_TpRxDataCnt > 7U ){ u8_DataLen = 7U; }
					else{ u8_DataLen = u8s_TpRxDataCnt;	}

					if( ( u8s_TpRxBufIdx + u8_DataLen ) >= MAX_RX_FRAME_SIZE )
					{
						u8s_TpRxBufIdx = 0U;
						u8_ErrNo = NrcRequestOutOfRange;
					}
					else
					{
						for( u8_TpIdx = 0U; u8_TpIdx < u8_DataLen; u8_TpIdx++ )
						{
							u8g_TpRxBuf[ u8_TpIdx + u8s_TpRxBufIdx ] = u8_CANFrameData[ u8_TpIdx + 1U ];
						}

						u8s_TpRxBufIdx += u8_DataLen;
						u8s_TpRxDataCnt -= u8_DataLen;

						if( u8s_TpRxDataCnt == 0U )
						{
							*u8p_IsEndFrame = TRUE;
							*u8p_RecvLen = u8s_TpRxDataSize;
							u8s_TpRxBufIdx = 0U;
						}
						else
						{
							if( ( u8s_TpRxSeqNum % TP_BLOCK_SIZE ) == 0U )
							{
								/* send flow control frame */
								TpFlowControlFrame ();
							}
						}
					}
				}
				break;
			case FT_FC: /* flow control frame */
				u8_STmin = u8_CANFrameData[ 2 ];

				if( u8_STmin < ST_MIN_LOW ){ u16s_STminTickCnt = ( uint16_t )( ST_MIN_LOW / BASE_TICK ); }
				else if( u8_STmin > ST_MIN_HIGH ){ u16s_STminTickCnt = ( uint16_t )( ST_MIN_HIGH / BASE_TICK ); }
				else{ u16s_STminTickCnt = ( uint16_t )( u8_STmin / BASE_TICK ); }

				u16s_STMin_tm = 0U;
				u8s_TpTxState = TP_ST_SEND_CF;
				break;
			default:
				u8_ErrNo = ERR_NOTAVAIL;
				break;
		}
	}
	else
	{
		u8_ErrNo = ERR_NOTAVAIL;
	}

	return ( u8_ErrNo )	;
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void TpSingleFrame ( uint8_t u8_DataLen )
{
	uint8_t u8_TpIdx;

#if defined ( TP_FUNC_MISRA ) /* MISRA_C_2012_11_03	*/
	uint8_t u8_TpFrame[ 8 ];

	u8_TpFrame[ 0 ] = ( uint8_t )( FT_SF << 4U );
	u8_TpFrame[ 0 ] |= ( uint8_t )( u8_DataLen & 0x0FU );

	for( u8_TpIdx = 0; u8_TpIdx < u8_DataLen; u8_TpIdx++ )
	{
		u8_TpFrame[ u8_TpIdx + 1U ] = u8g_TpTxBuf[ u8_TpIdx ];
	}

	TpSendFrame ( u8_TpFrame, u8_DataLen + 1U );
#else
		ST_TP st_TpFrame;

		st_TpFrame.u4_mTPI_Type = FT_SF;
		st_TpFrame.u4_mTPI_Dl = u8_DataLen;

		for( u8_TpIdx = 0; u8_TpIdx < u8_DataLen; u8_TpIdx++ )
		{
			st_TpFrame.u8_mData[ u8_TpIdx ] = u8g_TpTxBuf[ u8_TpIdx ];
		}

		TpSendFrame ( ( const uint8_t * )&st_TpFrame, u8_DataLen + 1U );
#endif

	u8s_TpTxState = TP_ST_NORMAL;

	return;
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void TpFirstFrame ( uint8_t u8_DataLen )
{
	uint8_t u8_TpIdx;

#if defined ( TP_FUNC_MISRA ) /* MISRA_C_2012_11_03	*/
	uint8_t u8_TpFrame[ 8 ];

	u8_TpFrame[ 0 ] = ( uint8_t )( FT_FF << 4U );
	u8_TpFrame[ 1 ] = u8_DataLen;

	for( u8_TpIdx = 0U; u8_TpIdx < 6U; u8_TpIdx++ )
	{
		u8_TpFrame[ u8_TpIdx + 2U ] = u8g_TpTxBuf[ u8_TpIdx ];
	}

	TpSendFrame ( u8_TpFrame, 8U );
#else
	ST_TP st_TpFrame;

	st_TpFrame.u4_mTPI_Type = FT_FF;
	st_TpFrame.u4_mTPI_Dl = 0;
	st_TpFrame.u8_mData[ 0 ] = u8_DataLen;

	for( u8_TpIdx = 0U; u8_TpIdx < 6U; u8_TpIdx++ )
	{
		st_TpFrame.u8_mData[ u8_TpIdx + 1U ] = u8g_TpTxBuf[ u8_TpIdx ];
	}

	TpSendFrame ( ( const uint8_t * )&st_TpFrame, 8U );
#endif

	u8s_TpTxDataCnt = u8_DataLen - 6U;
	u8s_TpTxBufIdx = 6U;
	u8s_TpTxSeqNum = 0U;

	u8s_TpTxState = TP_ST_WAIT_FC;

  return;
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
static void TpFlowControlFrame ( void )
{
#if defined ( TP_FUNC_MISRA ) /* MISRA_C_2012_11_03	*/
	uint8_t u8_TpFrame[ 8 ];
#else
	ST_TP st_TpFrame;
#endif

	/* send flow control frame */
#if defined ( TP_FUNC_MISRA ) /* MISRA_C_2012_11_03	*/
	u8_TpFrame[ 0 ] = ( uint8_t )( FT_FC << 4U );
	u8_TpFrame[ 1 ] = TP_BLOCK_SIZE;
	u8_TpFrame[ 2 ] = ST_MIN_VALUE;

	TpSendFrame ( u8_TpFrame, 3U );
#else
	st_TpFrame.u4_mTPI_Type = FT_FC;
	st_TpFrame.u4_mTPI_Dl = 0U;
	st_TpFrame.u8_mData[ 0 ] = TP_BLOCK_SIZE; /* Block Size: 8 frame */
	st_TpFrame.u8_mData[ 1 ] = ST_MIN_VALUE; /* STmin: 20ms */

	TpSendFrame ( ( const uint8_t * )&st_TpFrame, 3U );
#endif

  return;
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void TpSendFrame ( const uint8_t *p_Data, uint8_t u8_Size )
{
	CAN_MESSAGE_OBJECT_t g_can_TxData_18FEC91A;
	uint8_t u8_TpIdx;

	g_can_TxData_18FEC91A.ID = CANID_DIAG_PHYS_RESPONSE;
	g_can_TxData_18FEC91A.LENGTH = 8U;
	g_can_TxData_18FEC91A.IDE = 1U;

	g_can_TxData_18FEC91A.Data[ 0 ] = 0xAAU;
	g_can_TxData_18FEC91A.Data[ 1 ] = 0xAAU;
	g_can_TxData_18FEC91A.Data[ 2 ] = 0xAAU;
	g_can_TxData_18FEC91A.Data[ 3 ] = 0xAAU;
	g_can_TxData_18FEC91A.Data[ 4 ] = 0xAAU;
	g_can_TxData_18FEC91A.Data[ 5 ] = 0xAAU;
	g_can_TxData_18FEC91A.Data[ 6 ] = 0xAAU;
	g_can_TxData_18FEC91A.Data[ 7 ] = 0xAAU;

	for( u8_TpIdx = 0U; u8_TpIdx < u8_Size; u8_TpIdx++ )
	{
		g_can_TxData_18FEC91A.Data[ u8_TpIdx ] = p_Data[ u8_TpIdx ];
	}

	CAN1_Drv_SendFrame( TX_MB10, g_can_TxData_18FEC91A );
	CAN2_Drv_SendFrame( TX_MB10, g_can_TxData_18FEC91A );

	return;
}

/****************************************************************************/
/**                                                                        **/
/**                      EOF                                               **/
/**                                                                        **/
/****************************************************************************/
