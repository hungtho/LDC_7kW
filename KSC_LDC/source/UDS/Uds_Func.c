/*
                             *******************
******************************* C SOURCE FILE ********************************
**                           *******************                            **
**                                                                          **
**  Project   : SPC570S_FBL                                                 **
**  Filename  : Uds_Func.c                                                  **
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

#define UDS_FUNC_C_SRC

/****************************************************************************/
/**                                                                        **/
/**                      MODULES USED                                      **/
/**                                                                        **/
/****************************************************************************/
#include "Uds_Func.h"
#include "Hal_Can.h"
#include "Tp_Func.h"
#include "Asw_Utility.h"
#include <stdlib.h>

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
static void UdsSendResponse ( const ST_DescMsgContext *stp_MsgContext );
static void DescSetNegResponse ( uint8_t u8_mContext, uint8_t u8_ErrorCode );
static void DescSetResponsePending ( void );

static void ApplDescProcessDiagnosticSessionControl ( ST_DescMsgContext *stp_MsgContext );
static void ApplDescProcessEcuReset ( ST_DescMsgContext *stp_MsgContext );
static void ApplDescProcessSecurityAccess ( ST_DescMsgContext *stp_MsgContext );
static void ApplDescProcessCommunicatonControl ( ST_DescMsgContext *stp_MsgContext );
static void ApplDescProcessWriteDataByIdentifier ( ST_DescMsgContext *stp_MsgContext );
static void ApplDescProcessTesterPresent ( ST_DescMsgContext *stp_MsgContext );
static void ApplDescProcessControlDTCSetting ( ST_DescMsgContext *stp_MsgContext );
static void ApplDescProcessReadDTCInformation ( ST_DescMsgContext *stp_MsgContext );
static void ApplDescProcessClearDiagnosticInformation ( ST_DescMsgContext *stp_MsgContext );
static void ApplDescProcessReadDataByIdentifier ( ST_DescMsgContext *stp_MsgContext );

/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED VARIABLES                                **/
/**                                                                        **/
/****************************************************************************/
uint8_t u8g_ReqReset;
uint8_t u8g_CAN_TX_Ctrl;
uint8_t u8g_CAN_RX_Ctrl;
uint8_t u8_DtcControl;

uint8_t u8g_Eeprom_WriteData[ 32 ];

/****************************************************************************/
/**                                                                        **/
/**                      GLOBAL VARIABLES                                  **/
/**                                                                        **/
/****************************************************************************/
static uint8_t u8s_DescNegResCode;
static uint8_t u8s_Session;
static uint8_t u8s_Lockstate;
static uint8_t u8s_SecuritySequence;
static uint16_t u16s_TestPresentTimeout_cnt;

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
void Uds_Func_Init ( void )
{
	u8g_CAN_TX_Ctrl = CAN_CTRL_ENABLE;
	u8g_CAN_RX_Ctrl = CAN_CTRL_ENABLE;
	u8_DtcControl = DTC_ENABLE;
	u8s_DescNegResCode = NrcPositiveResponse;
	u8s_Session = DefaultSession;
	u8s_Lockstate = SECURITY_LOCK;
	u8s_SecuritySequence = SECURITY_SEQ_FALSE;
	u16s_TestPresentTimeout_cnt = 0U;

	return;
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void Uds_Func ( void )
{
	/* Diagnostic Service Id List */
	static const ST_DescService sts_DescService[] =
	{
/* u2_mIsValid, u2_mReqType, u1_mHaveSubFunc, u3_mDummy, u2_mServiceId, Func */
		{ YES, PHYS|FUNC, YES, 0U, 0x10U, ApplDescProcessDiagnosticSessionControl  	},
		{ YES, PHYS     , YES, 0U, 0x11U, ApplDescProcessEcuReset                  	},
		{ YES, PHYS     , YES, 0U, 0x27U, ApplDescProcessSecurityAccess            	},
		{ YES, PHYS|FUNC, YES, 0U, 0x28U, ApplDescProcessCommunicatonControl       	},
		{ YES, PHYS     , NO,  0U, 0x2EU, ApplDescProcessWriteDataByIdentifier     	},
		{ YES, PHYS|FUNC, YES, 0U, 0x3EU, ApplDescProcessTesterPresent            	},
		{ YES, PHYS|FUNC, YES, 0U, 0x85U, ApplDescProcessControlDTCSetting        	},
		{ YES, PHYS			, YES, 0U, 0x19U, ApplDescProcessReadDTCInformation       	},
		{ YES, PHYS			, NO,  0U, 0x14U, ApplDescProcessClearDiagnosticInformation	},
		{ YES, PHYS			, NO,  0U, 0x22U, ApplDescProcessReadDataByIdentifier		    },
		{ NO , 0x00U    , 0U , 0U, 0xFFU, NULL                                     	},
	};

	static ST_DescMsgContext st_MsgContext;
	uint8_t u8_idx;
	uint8_t u8_IsEndFrame = 0U;
	uint8_t u8_SvcId;
	uint8_t u8_RecvLen = 0U;
	uint8_t u8_RequestType = 0x00U;

	if( u8s_Session != DefaultSession )
	{
		if( u16s_TestPresentTimeout_cnt >= 5000U )
		{
			g_can_RxData_REQUEST.Data[ 0 ] = 0x02U;
			g_can_RxData_REQUEST.Data[ 1 ] = 0x10U;
			g_can_RxData_REQUEST.Data[ 2 ] = 0x01U;
			g_can_RxData_REQUEST.Data[ 3 ] = 0x55U;
			g_can_RxData_REQUEST.Data[ 4 ] = 0x55U;
			g_can_RxData_REQUEST.Data[ 5 ] = 0x55U;
			g_can_RxData_REQUEST.Data[ 6 ] = 0x55U;
			g_can_RxData_REQUEST.Data[ 7 ] = 0x55U;
			u8g_Hal_RecvNewDataforDiag = 1U;
		}
		else
		{
			u16s_TestPresentTimeout_cnt++;
		}
	}

	if( u8g_Hal_RecvNewDataforDiag == 1U )
	{
		u8g_Hal_RecvNewDataforDiag = 0U;
		u16s_TestPresentTimeout_cnt = 0U;

		if( TpRecvProc ( &u8_IsEndFrame, &u8_RecvLen, &u8_RequestType ) != ERR_OK )
		{
			u8s_DescNegResCode = NrcInvalidFormat;
			st_MsgContext.u8_mContext = 0U;
			UdsSendResponse ( &st_MsgContext ); /* send negative response */
		}
		else
		{
			if( ( u8_IsEndFrame == 1U ) && ( u8_RecvLen != 0U ) )
			{
				st_MsgContext.u16_mReqDataLen = ( uint16_t )u8_RecvLen - 1U;

				if( u8_RecvLen < 8U )
				{
					u8_SvcId = u8g_SingleRxBuf[ 0 ];
					st_MsgContext.u8p_mReqData = &u8g_SingleRxBuf[ 1 ];
				}
				else
				{
					u8_SvcId = u8g_TpRxBuf[ 0 ];
					st_MsgContext.u8p_mReqData = &u8g_TpRxBuf[ 1 ];
				}
				st_MsgContext.u8p_mResData = &u8g_TpTxBuf[ 1 ];
				st_MsgContext.st_mMsgAddInfo.u2_mReqType = u8_RequestType;
				st_MsgContext.st_mMsgAddInfo.u1_mSuppressRes = NO;

				u8s_DescNegResCode = NrcPositiveResponse;

				for( u8_idx = 0U; sts_DescService[ u8_idx ].u2_mIsValid != NO; u8_idx++ )
				{
					if( ( sts_DescService[ u8_idx ].u8_mServiceId == u8_SvcId )
					 && ( ( sts_DescService[ u8_idx ].u2_mReqType & u8_RequestType ) != 0x00U ) )
					{
						sts_DescService[ u8_idx ].Func ( &st_MsgContext );
						break;
					}
				}

				if( sts_DescService[ u8_idx ].u2_mIsValid == NO )
				{
					u8s_DescNegResCode = NrcServiceNotSupported;
				}
				if( ( st_MsgContext.st_mMsgAddInfo.u2_mReqType == FUNC )
				 && ( ( u8s_DescNegResCode == NrcServiceNotSupported )
					 || ( u8s_DescNegResCode == NrcSubfunctionNotSupported )
					 || ( u8s_DescNegResCode == NrcRequestOutOfRange ) ) )
				{
					st_MsgContext.st_mMsgAddInfo.u1_mSuppressRes = YES;
				}
				else
				{
					if( sts_DescService[ u8_idx ].u1_mHaveSubFunc == YES ) /* if service with subFunction is supported */
					{
						if( ( ( st_MsgContext.u8p_mReqData[ 0 ] & 0x80U ) == 0x80U )
						 && ( u8s_DescNegResCode == NrcPositiveResponse ) )
						{
							st_MsgContext.st_mMsgAddInfo.u1_mSuppressRes = YES;
						}
					}
				}

				if( st_MsgContext.st_mMsgAddInfo.u1_mSuppressRes == NO )
				{
					UdsSendResponse ( &st_MsgContext ); 		/* send response */
				}
			}
		}
	}

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
static void UdsSendResponse ( const ST_DescMsgContext *stp_MsgContext )
{
	uint8_t u8_Sid;
	uint8_t u8_ResDataLen;

	if( u8g_RxFrameType == FT_SF ){ u8_Sid = u8g_SingleRxBuf[ 0 ]; }
	else{ u8_Sid = u8g_TpRxBuf[ 0 ]; }

	if( u8s_DescNegResCode == NrcPositiveResponse ) /* positive response */
	{
		u8g_TpTxBuf[ 0 ] = u8_Sid + 0x40U;
		u8_ResDataLen = ( uint8_t )( stp_MsgContext->u16_mResDataLen + 1U );
	}
  else /* negative response */
	{
		u8g_TpTxBuf[ 0 ] = 0x7FU;
		u8g_TpTxBuf[ 1 ] = u8_Sid;
		u8g_TpTxBuf[ 2 ] = u8s_DescNegResCode;
		u8_ResDataLen = 3U;
	}

	if( u8_ResDataLen < 8U )
	{
		TpSingleFrame ( u8_ResDataLen );
	}
	else
	{
		TpFirstFrame ( u8_ResDataLen );
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
static void DescSetNegResponse ( uint8_t u8_mContext, uint8_t u8_ErrorCode )
{
	if( u8_mContext != 0U ){}
	u8s_DescNegResCode = u8_ErrorCode;

	return;
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void DescSetResponsePending ( void )
{
  uint8_t u8_Sid;
	uint8_t u8_ResDataLen;

	if( u8g_RxFrameType == FT_SF ){ u8_Sid = u8g_SingleRxBuf[ 0 ]; }
	else{ u8_Sid = u8g_TpRxBuf[ 0 ]; }

	u8g_TpTxBuf[ 0 ] = 0x7FU;
	u8g_TpTxBuf[ 1 ] = u8_Sid;
	u8g_TpTxBuf[ 2 ] = NrcResponsePending;
	u8_ResDataLen = 3U;

  TpSingleFrame ( u8_ResDataLen );

  return;
}
/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void ApplDescProcessDiagnosticSessionControl ( ST_DescMsgContext *stp_MsgContext )
{
  uint8_t u8_SubFunc;

	u8_SubFunc = stp_MsgContext->u8p_mReqData[ 0 ] & 0x7FU;

	switch( u8_SubFunc )
	{
	  case DefaultSession:
	  	if( stp_MsgContext->u16_mReqDataLen <= 0x02U )
	  	{
		  	stp_MsgContext->u8p_mResData[ 0 ] = DefaultSession;
				stp_MsgContext->u8p_mResData[ 1 ] = 0x00U;
				stp_MsgContext->u8p_mResData[ 2 ] = 0x32U;
				stp_MsgContext->u8p_mResData[ 3 ] = 0x00U;
				stp_MsgContext->u8p_mResData[ 4 ] = 0xC8U;
				stp_MsgContext->u16_mResDataLen = 5U;
				u8s_Session = DefaultSession;
				u8s_Lockstate = SECURITY_LOCK;
				u8s_SecuritySequence = SECURITY_SEQ_FALSE;
				u8g_CAN_TX_Ctrl = CAN_CTRL_ENABLE;
				u8g_CAN_RX_Ctrl = CAN_CTRL_ENABLE;
				u8_DtcControl = DTC_ENABLE;
	  	}
	  	else
	  	{
	  		DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcInvalidFormat );
	  	}
	  	break;
	  case ProgrammingSession:
	  	if( stp_MsgContext->u16_mReqDataLen <= 0x02U )
	  	{
	  		stp_MsgContext->u8p_mResData[ 0 ] = ProgrammingSession;
	  		stp_MsgContext->u8p_mResData[ 1 ] = 0x00U;
	  		stp_MsgContext->u8p_mResData[ 2 ] = 0x32U;
	  		stp_MsgContext->u8p_mResData[ 3 ] = 0x00U;
	  		stp_MsgContext->u8p_mResData[ 4 ] = 0xC8U;
	  		stp_MsgContext->u16_mResDataLen = 5U;
	  		u8s_Session = ProgrammingSession;
	  		SetBootMode(BOOT_MODE);
	  		u8g_ReqReset = 1U;
	  		u8s_SecuritySequence = SECURITY_SEQ_FALSE;
	  	}
	  	else
			{
				DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcInvalidFormat );
			}
	  	break;
    case ExtendedDiagSession:
	  	if( stp_MsgContext->u16_mReqDataLen <= 0x02U )
	  	{
	  		stp_MsgContext->u8p_mResData[ 0 ] = ExtendedDiagSession;
	  		stp_MsgContext->u8p_mResData[ 1 ] = 0x00U;
				stp_MsgContext->u8p_mResData[ 2 ] = 0x32U;
				stp_MsgContext->u8p_mResData[ 3 ] = 0x00U;
				stp_MsgContext->u8p_mResData[ 4 ] = 0xC8U;
	  		stp_MsgContext->u16_mResDataLen = 5U;
	  		u8s_Session = ExtendedDiagSession;
	  		u8s_SecuritySequence = SECURITY_SEQ_FALSE;
	  	}
	  	else
			{
				DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcInvalidFormat );
			}
	  	break;
	  default:
	    DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcSubfunctionNotSupported );
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
static void ApplDescProcessEcuReset ( ST_DescMsgContext *stp_MsgContext )
{
	uint8_t u8_SubFunc;

  u8_SubFunc = stp_MsgContext->u8p_mReqData[ 0 ] & 0x7FU;

	switch( u8_SubFunc )
	{
	  case hardReset:
	  	if( stp_MsgContext->u16_mReqDataLen <= 0x01U )
			{
				stp_MsgContext->u8p_mResData[ 0 ] = 0x01U;
				stp_MsgContext->u16_mResDataLen = 1U;
				u8g_ReqReset = 1U;
			}
			else
			{
				DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcInvalidFormat );
			}
	    break;
	  default:
	    DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcSubfunctionNotSupported );
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
static void ApplDescProcessSecurityAccess ( ST_DescMsgContext *stp_MsgContext )
{
	static uint8_t u8s_Seed[ 4 ];
  uint8_t u8_SubFunc;

  u8_SubFunc = stp_MsgContext->u8p_mReqData[ 0 ] & 0x7FU;

  if ( u8s_Session == ExtendedDiagSession )
  {
  	switch( u8_SubFunc )
  	{
  		case RequestSeed:
  	  	if( stp_MsgContext->u16_mReqDataLen <= 0x01U )
  			{
  	  		if( u8s_SecuritySequence == SECURITY_SEQ_FALSE )
  	  		{
  	  			u8s_SecuritySequence = SECURITY_SEQ_TRUE;

    	  		u8s_Seed[ 0 ] = 0U;
    	  		u8s_Seed[ 1 ] = 0U;
    	  		u8s_Seed[ 2 ] = 0U;
    	  		u8s_Seed[ 3 ] = 0U;

    	  		if( u8s_Lockstate == SECURITY_LOCK )
    	  		{
  						srand( Get_PIT_CH3_CVAL () );

  						u8s_Seed[ 0 ] = ( uint8_t )( ( ( uint8_t )rand() % 255U ) + 1U );
  						u8s_Seed[ 1 ] = ( uint8_t )( ( ( uint8_t )rand() % 255U ) + 1U );
  						u8s_Seed[ 2 ] = ( uint8_t )( ( ( uint8_t )rand() % 255U ) + 1U );
  						u8s_Seed[ 3 ] = ( uint8_t )( ( ( uint8_t )rand() % 255U ) + 1U );
  					}

    	  		stp_MsgContext->u8p_mResData[ 0 ] = RequestSeed;
    				stp_MsgContext->u8p_mResData[ 1 ] = u8s_Seed[ 0 ];
    				stp_MsgContext->u8p_mResData[ 2 ] = u8s_Seed[ 1 ];
    				stp_MsgContext->u8p_mResData[ 3 ] = u8s_Seed[ 2 ];
    				stp_MsgContext->u8p_mResData[ 4 ] = u8s_Seed[ 3 ];

    	  		stp_MsgContext->u16_mResDataLen = 5U;
  	  		}
  	  		else
  	  		{
  	  			DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcRequestSequenceError );
  	  		}
  			}
  			else
  			{
  				DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcInvalidFormat );
  			}
  			break;
  		case SendKey:
  	  	if( stp_MsgContext->u16_mReqDataLen <= 0x05U )
  			{
  	  		if( u8s_SecuritySequence == SECURITY_SEQ_TRUE )
  	  		{
  	  			u8s_SecuritySequence = 0U;

  	  			if( ( stp_MsgContext->u8p_mReqData[ 1 ] == ( u8s_Seed[ 0 ] ^ 0x55U ) )
    	  		 && ( stp_MsgContext->u8p_mReqData[ 2 ] == ( u8s_Seed[ 1 ] ^ 0xAAU ) )
  					 && ( stp_MsgContext->u8p_mReqData[ 3 ] == ( u8s_Seed[ 2 ] ^ 0x55U ) )
  					 && ( stp_MsgContext->u8p_mReqData[ 4 ] == ( u8s_Seed[ 3 ] ^ 0xAAU ) ) )
    	  		{
      	  		stp_MsgContext->u8p_mResData[ 0 ] = SendKey;
      	   		stp_MsgContext->u16_mResDataLen = 1U;
      	   		u8s_Lockstate = SECURITY_UNLOCK;
    	  		}
    	  		else
    	  		{
    	  			DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcInvalidKey );
    	  		}
  	  		}
  	  		else
  	  		{
  	  			DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcRequestSequenceError );
  	  		}
  			}
  			else
  			{
  				DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcInvalidFormat );
  			}
  			break;
  		default:
  			DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcSubfunctionNotSupported );
  			break;
  	}
  }
  else
  {
  	DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcConditionsNotCorrect );
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
static void ApplDescProcessCommunicatonControl ( ST_DescMsgContext *stp_MsgContext )
{
	uint8_t u8_SubFunc;

	if ( u8s_Session == ExtendedDiagSession )
	{
		u8_SubFunc = stp_MsgContext->u8p_mReqData[ 0 ] & 0x7FU;

		switch( u8_SubFunc )
		{
			case enableRxAndTx:
				if( stp_MsgContext->u16_mReqDataLen <= 0x02U )
				{
					stp_MsgContext->u8p_mResData[ 0 ] = enableRxAndTx;
					stp_MsgContext->u16_mResDataLen = 1U;

					u8g_CAN_TX_Ctrl = CAN_CTRL_ENABLE;
					u8g_CAN_RX_Ctrl = CAN_CTRL_ENABLE;
				}
				else
				{
					DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcInvalidFormat );
				}
				break;
			case enableRxAndDisableTx:
				if( stp_MsgContext->u16_mReqDataLen <= 0x02U )
				{
					stp_MsgContext->u8p_mResData[ 0 ] = enableRxAndTx;
					stp_MsgContext->u16_mResDataLen = 1U;

					u8g_CAN_TX_Ctrl = CAN_CTRL_DISABLE;
					u8g_CAN_RX_Ctrl = CAN_CTRL_ENABLE;
				}
				else
				{
					DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcInvalidFormat );
				}
				break;
			case disableRxAndEnableTx:
				if( stp_MsgContext->u16_mReqDataLen <= 0x02U )
				{
					stp_MsgContext->u8p_mResData[ 0 ] = enableRxAndTx;
					stp_MsgContext->u16_mResDataLen = 1U;

					u8g_CAN_TX_Ctrl = CAN_CTRL_ENABLE;
					u8g_CAN_RX_Ctrl = CAN_CTRL_DISABLE;
				}
				else
				{
					DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcInvalidFormat );
				}
				break;
			case disableRxAndTx:
				if( stp_MsgContext->u16_mReqDataLen <= 0x02U )
				{
					stp_MsgContext->u8p_mResData[ 0 ] = disableRxAndTx;
					stp_MsgContext->u16_mResDataLen = 1U;

					u8g_CAN_TX_Ctrl = CAN_CTRL_DISABLE;
					u8g_CAN_RX_Ctrl = CAN_CTRL_DISABLE;
				}
				else
				{
					DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcInvalidFormat );
				}
				break;
			default:
				DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcSubfunctionNotSupported );
				break;
		}
	}
	else
	{
		DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcConditionsNotCorrect );
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
static void ApplDescProcessTesterPresent ( ST_DescMsgContext *stp_MsgContext )
{
	uint8_t u8_SubFunc;

	u8_SubFunc = stp_MsgContext->u8p_mReqData[ 0 ] & 0x7FU;

	switch( u8_SubFunc )
	{
	  case 0x00:
	    if( stp_MsgContext->u16_mReqDataLen <= 0x01U )
	    {
	      stp_MsgContext->u8p_mResData[ 0 ] = 0x00U;
  		  stp_MsgContext->u16_mResDataLen = 1U;
	    }
	    else
	    {
	      DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcInvalidFormat );
	    }
	    break;
	  default:
	    DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcSubfunctionNotSupported );
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
static void ApplDescProcessControlDTCSetting ( ST_DescMsgContext *stp_MsgContext )
{
	uint8_t u8_SubFunc;

	if ( u8s_Session == ExtendedDiagSession )
	{
		u8_SubFunc = stp_MsgContext->u8p_mReqData[ 0 ] & 0x7FU;

		switch( u8_SubFunc )
		{
			case ON:
				if( stp_MsgContext->u16_mReqDataLen <= 0x01U )
				{
					stp_MsgContext->u8p_mResData[ 0 ] = ON;
					stp_MsgContext->u16_mResDataLen = 1U;

					u8_DtcControl = DTC_ENABLE;
				}
				else
				{
					DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcInvalidFormat );
				}
				break;
			case OFF:
				if( stp_MsgContext->u16_mReqDataLen <= 0x01U )
				{
					stp_MsgContext->u8p_mResData[ 0 ] = OFF;
					stp_MsgContext->u16_mResDataLen = 1U;

					u8_DtcControl = DTC_DISABLE;
				}
				else
				{
					DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcInvalidFormat );
				}
				break;
			default:
				DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcSubfunctionNotSupported );
				break;
		}
	}
	else
	{
		DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcConditionsNotCorrect );
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
static void ApplDescProcessReadDTCInformation ( ST_DescMsgContext *stp_MsgContext )
{
	uint8_t uds_u8Eeprom_ReadDTCDataByIdentifier[ 32 ];
	uint8_t u8_SubFunc = 0U;
	uint8_t u8_DTCStatusMask = 0U;
	uint8_t u8_i = 0U;
	uint16_t dtc_count = 0U;

	u8_SubFunc = stp_MsgContext->u8p_mReqData[ 0 ] & 0x7FU;
	u8_DTCStatusMask = stp_MsgContext->u8p_mReqData[ 1 ];

	for( u8_i = 0U; u8_i< 8U; u8_i++ )
	{
		uds_u8Eeprom_ReadDTCDataByIdentifier[ ( 4U * u8_i ) + 0U ] = ( uint8_t )( u32g_Asw_EEPROM_LAST_BUFFER_DTC[ u8_i ] & 0x000000FFU );
		uds_u8Eeprom_ReadDTCDataByIdentifier[ ( 4U * u8_i ) + 1U ] = ( uint8_t )( ( u32g_Asw_EEPROM_LAST_BUFFER_DTC[ u8_i ] & 0x0000FF00U ) >> 8U );
		uds_u8Eeprom_ReadDTCDataByIdentifier[ ( 4U * u8_i ) + 2U ] = ( uint8_t )( ( u32g_Asw_EEPROM_LAST_BUFFER_DTC[ u8_i ] & 0x00FF0000U ) >> 16U );
		uds_u8Eeprom_ReadDTCDataByIdentifier[ ( 4U * u8_i ) + 3U ] = ( uint8_t )( ( u32g_Asw_EEPROM_LAST_BUFFER_DTC[ u8_i ] & 0xFF000000U ) >> 24U );
	}

	switch( u8_SubFunc )
	{
	  case reportNumberOfDTCByStatusMask:
	    if( stp_MsgContext->u16_mReqDataLen <= 0x02U )
	    {
	    	for( u8_i = 0U; u8_i < DTC_CODE_NUMBER; u8_i++ )
	    	{
		  		if( ( uds_u8Eeprom_ReadDTCDataByIdentifier[u8_i] & u8_DTCStatusMask ) != 0x00U )
		  		{
		  			dtc_count++;
		  		}
	    	}

	    	stp_MsgContext->u8p_mResData[ 0 ] = reportNumberOfDTCByStatusMask;
				stp_MsgContext->u8p_mResData[ 1 ] = 0x09U; /* DTC Status Availability Mask*/
				stp_MsgContext->u8p_mResData[ 2 ] = 0x00U;
				stp_MsgContext->u8p_mResData[ 3 ] = ( uint8_t )( ( dtc_count & 0xFF00U ) >> 8U ); /* DTC_COUNT HIGHBYTE */
				stp_MsgContext->u8p_mResData[ 4 ] = ( uint8_t )( dtc_count & 0x00FFU ); /* DTC_COUNT LOWBYTE */

		  	stp_MsgContext->u16_mResDataLen = 5U;
	    }
	    else
	    {
	      DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcInvalidFormat );
	    }
	    break;
	  case reportDTCByStatusMask:
	    if( stp_MsgContext->u16_mReqDataLen <= 0x02U )
	    {
	    	stp_MsgContext->u8p_mResData[ 0 ] = reportDTCByStatusMask;
				stp_MsgContext->u8p_mResData[ 1 ] = 0x09U; /* DTC Status Availability Mask*/

				for( u8_i = 0U; u8_i < DTC_CODE_NUMBER; u8_i++ )
				{
					if( ( uds_u8Eeprom_ReadDTCDataByIdentifier[u8_i] & u8_DTCStatusMask ) != 0x00U )
					{
						stp_MsgContext->u8p_mResData[ ( dtc_count * 4U ) + 2U ] = DTC_HIGH;
						stp_MsgContext->u8p_mResData[ ( dtc_count * 4U ) + 3U ] = DTC_MIDDLE;
						stp_MsgContext->u8p_mResData[ ( dtc_count * 4U ) + 4U ] = u8g_Asw_DTC_CODE_Array[u8_i];
						stp_MsgContext->u8p_mResData[ ( dtc_count * 4U ) + 5U ] = uds_u8Eeprom_ReadDTCDataByIdentifier[u8_i];

						dtc_count++;
					}
				}

				stp_MsgContext->u16_mResDataLen = 2U + ( dtc_count * 4U );
	    }
	    else
	    {
	      DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcInvalidFormat );
	    }
	    break;
	  default:
	    DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcSubfunctionNotSupported );
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
static void ApplDescProcessClearDiagnosticInformation ( ST_DescMsgContext *stp_MsgContext )
{
	uint8_t ee_i;

  if( stp_MsgContext->u16_mReqDataLen <= 0x03U )
  {
  	if ( ( stp_MsgContext->u8p_mReqData[ 0 ] == 0xFFU )
  		&& ( stp_MsgContext->u8p_mReqData[ 1 ] == 0xFFU )
  		&& ( stp_MsgContext->u8p_mReqData[ 2 ] == 0xFFU ) )
  	{
  		/* Store new DTC in buffer */
  		for ( ee_i = 0U; ee_i< 8U; ee_i++ )
  		{
  			u32g_Asw_EEPROM_BUFFER_DTC[ ee_i ] = 0;
  			u32g_Asw_EEPROM_LAST_BUFFER_DTC[ ee_i ] = 0;
  		}

  		u8g_Asw_WriteEEPROM_Flag = 1U;
  		stp_MsgContext->u16_mResDataLen = 0U;
  	}
  	else
  	{
  		DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcRequestOutOfRange );
  	}
  }
  else
  {
  	DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcInvalidFormat );
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
static void ApplDescProcessReadDataByIdentifier ( ST_DescMsgContext *stp_MsgContext )
{
	uint8_t ee_i = 0U;
	uint8_t u8_Eeprom_ReadDIDData[ 32 ];
	uint16_t u16_Identifier = 0U;

	u16_Identifier = ( uint16_t )( ( ( uint16_t )stp_MsgContext->u8p_mReqData[ 0 ] & 0x00FFU ) << 8 );
	u16_Identifier |= ( uint16_t )stp_MsgContext->u8p_mReqData[ 1 ];

	/* Read the last eeprom DTC buffer */
	Asw_DID_Eeprom_Read ( u32g_Asw_present_addr );

	for( ee_i = 0U; ee_i< 8U; ee_i++ )
	{
		u8_Eeprom_ReadDIDData[ ( 4U * ee_i ) + 0U ] = ( uint8_t )( u32g_Asw_EEPROM_LAST_BUFFER_DID[ ee_i ] & 0x000000FFU );
		u8_Eeprom_ReadDIDData[ ( 4U * ee_i ) + 1U ] = ( uint8_t )( ( u32g_Asw_EEPROM_LAST_BUFFER_DID[ ee_i ] & 0x0000FF00U ) >> 8U );
		u8_Eeprom_ReadDIDData[ ( 4U * ee_i ) + 2U ] = ( uint8_t )( ( u32g_Asw_EEPROM_LAST_BUFFER_DID[ ee_i ] & 0x00FF0000U ) >> 16U );
		u8_Eeprom_ReadDIDData[ ( 4U * ee_i ) + 3U ] = ( uint8_t )( ( u32g_Asw_EEPROM_LAST_BUFFER_DID[ ee_i ] & 0xFF000000U ) >> 24U );
	}

	switch( u16_Identifier )
	{
	  case 0xF187U:
	    if( stp_MsgContext->u16_mReqDataLen <= 0x02U )
	    {
				stp_MsgContext->u8p_mResData[ 0 ] = 0xF1U;
				stp_MsgContext->u8p_mResData[ 1 ] = 0x87U;
				stp_MsgContext->u8p_mResData[ 2 ] = u8_Eeprom_ReadDIDData[ 0 ];
				stp_MsgContext->u8p_mResData[ 3 ] = u8_Eeprom_ReadDIDData[ 1 ];
				stp_MsgContext->u8p_mResData[ 4 ] = u8_Eeprom_ReadDIDData[ 2 ];
				stp_MsgContext->u8p_mResData[ 5 ] = u8_Eeprom_ReadDIDData[ 3 ];
				stp_MsgContext->u8p_mResData[ 6 ] = u8_Eeprom_ReadDIDData[ 4 ];
				stp_MsgContext->u8p_mResData[ 7 ] = u8_Eeprom_ReadDIDData[ 5 ];
				stp_MsgContext->u8p_mResData[ 8 ] = u8_Eeprom_ReadDIDData[ 6 ];
				stp_MsgContext->u8p_mResData[ 9 ] = u8_Eeprom_ReadDIDData[ 7 ];
				stp_MsgContext->u8p_mResData[ 10 ] = u8_Eeprom_ReadDIDData[ 8 ];
				stp_MsgContext->u8p_mResData[ 11 ] = u8_Eeprom_ReadDIDData[ 9 ];

				stp_MsgContext->u16_mResDataLen = 0xCU;
	    }
	    else
	    {
	      DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcInvalidFormat );
	    }
	    break;
	  case 0xF18BU:
	    if( stp_MsgContext->u16_mReqDataLen <= 0x02U )
	    {
				stp_MsgContext->u8p_mResData[ 0 ] = 0xF1U;
				stp_MsgContext->u8p_mResData[ 1 ] = 0x8BU;
				stp_MsgContext->u8p_mResData[ 2 ] = u8_Eeprom_ReadDIDData[ 12 ];
				stp_MsgContext->u8p_mResData[ 3 ] = u8_Eeprom_ReadDIDData[ 13 ];
				stp_MsgContext->u8p_mResData[ 4 ] = u8_Eeprom_ReadDIDData[ 14 ];
				stp_MsgContext->u8p_mResData[ 5 ] = u8_Eeprom_ReadDIDData[ 15 ];

				stp_MsgContext->u16_mResDataLen = 6U;
	    }
	    else
	    {
	      DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcInvalidFormat );
	    }
	    break;
	  case 0xF195U:
	    if( stp_MsgContext->u16_mReqDataLen <= 0x02U )
	    {
				stp_MsgContext->u8p_mResData[ 0 ] = 0xF1U;
				stp_MsgContext->u8p_mResData[ 1 ] = 0x95U;
				stp_MsgContext->u8p_mResData[ 2 ] = SW_VERSION_H;
				stp_MsgContext->u8p_mResData[ 3 ] = SW_VERSION_M;
				stp_MsgContext->u8p_mResData[ 4 ] = SW_VERSION_L;

				stp_MsgContext->u16_mResDataLen = 5U;
	    }
	    else
	    {
	      DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcInvalidFormat );
	    }
	    break;
	  case 0xF193U:
	    if( stp_MsgContext->u16_mReqDataLen <= 0x02U )
	    {
				stp_MsgContext->u8p_mResData[ 0 ] = 0xF1U;
				stp_MsgContext->u8p_mResData[ 1 ] = 0x93U;
				stp_MsgContext->u8p_mResData[ 2 ] = u8_Eeprom_ReadDIDData[ 16 ];
				stp_MsgContext->u8p_mResData[ 3 ] = u8_Eeprom_ReadDIDData[ 17 ];
				stp_MsgContext->u8p_mResData[ 4 ] = u8_Eeprom_ReadDIDData[ 18 ];

				stp_MsgContext->u16_mResDataLen = 5U;
	    }
	    else
	    {
	      DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcInvalidFormat );
	    }
	    break;
	  default:
	    DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcRequestOutOfRange );
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
static void ApplDescProcessWriteDataByIdentifier ( ST_DescMsgContext *stp_MsgContext )
{
	uint16_t u16_Identifier = 0U;

	if ( u8s_Session == ExtendedDiagSession )
	{
		if( u8s_Lockstate == SECURITY_UNLOCK )
		{
			u16_Identifier = ( uint16_t )( ( ( uint16_t )stp_MsgContext->u8p_mReqData[ 0 ] & 0x00FFU ) << 8 );
			u16_Identifier |= ( uint16_t )stp_MsgContext->u8p_mReqData[ 1 ];

			switch( u16_Identifier )
			{
				case 0xF187U:
					if( stp_MsgContext->u16_mReqDataLen <= 0xCU )
					{
						stp_MsgContext->u8p_mResData[ 0 ] = 0xF1U;
						stp_MsgContext->u8p_mResData[ 1 ] = 0x87U;		/* DTC Status Availability Mask*/

						stp_MsgContext->u16_mResDataLen = 0x02U;

						u8g_Eeprom_WriteData[ 0 ] = stp_MsgContext->u8p_mReqData[ 2 ] ;
						u8g_Eeprom_WriteData[ 1 ] = stp_MsgContext->u8p_mReqData[ 3 ] ;
						u8g_Eeprom_WriteData[ 2 ] = stp_MsgContext->u8p_mReqData[ 4 ] ;
						u8g_Eeprom_WriteData[ 3 ] = stp_MsgContext->u8p_mReqData[ 5 ] ;
						u8g_Eeprom_WriteData[ 4 ] = stp_MsgContext->u8p_mReqData[ 6 ] ;
						u8g_Eeprom_WriteData[ 5 ] = stp_MsgContext->u8p_mReqData[ 7 ] ;
						u8g_Eeprom_WriteData[ 6 ] = stp_MsgContext->u8p_mReqData[ 8 ] ;
						u8g_Eeprom_WriteData[ 7 ] = stp_MsgContext->u8p_mReqData[ 9 ] ;
						u8g_Eeprom_WriteData[ 8 ] = stp_MsgContext->u8p_mReqData[ 10 ];
						u8g_Eeprom_WriteData[ 9 ] = stp_MsgContext->u8p_mReqData[ 11 ];
					}
					else
					{
						DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcInvalidFormat );
					}
					break;
				case 0xF18BU:
					if( stp_MsgContext->u16_mReqDataLen <= 0x06U )
					{
						stp_MsgContext->u8p_mResData[ 0 ] = 0xF1U;
						stp_MsgContext->u8p_mResData[ 1 ] = 0x8BU;		/* DTC Status Availability Mask*/

						stp_MsgContext->u16_mResDataLen = 2U;

						u8g_Eeprom_WriteData[ 12 ] = stp_MsgContext->u8p_mReqData[ 2 ] ;
						u8g_Eeprom_WriteData[ 13 ] = stp_MsgContext->u8p_mReqData[ 3 ] ;
						u8g_Eeprom_WriteData[ 14 ] = stp_MsgContext->u8p_mReqData[ 4 ] ;
						u8g_Eeprom_WriteData[ 15 ] = stp_MsgContext->u8p_mReqData[ 5 ] ;
					}
					else
					{
						DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcInvalidFormat );
					}
					break;
				case 0xF193U:
					if( stp_MsgContext->u16_mReqDataLen <= 0x05U )
					{
						stp_MsgContext->u8p_mResData[ 0 ] = 0xF1U;
						stp_MsgContext->u8p_mResData[ 1 ] = 0x93U;

						stp_MsgContext->u16_mResDataLen = 2U;

						u8g_Eeprom_WriteData[ 16 ] = stp_MsgContext->u8p_mReqData[ 2 ] ;
						u8g_Eeprom_WriteData[ 17 ] = stp_MsgContext->u8p_mReqData[ 3 ] ;
						u8g_Eeprom_WriteData[ 18 ] = stp_MsgContext->u8p_mReqData[ 4 ] ;
					}
					else
					{
						DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcInvalidFormat );
					}
					break;
				default:
					DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcRequestOutOfRange );
					break;
			}

			Asw_DID_Eeprom_Write();
		}
		else
		{
			DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcAccessDenied );
		}
	}
	else
	{
		DescSetNegResponse ( stp_MsgContext->u8_mContext, NrcConditionsNotCorrect );
	}

  return;
}
/****************************************************************************/
/**                                                                        **/
/**                      EOF                                               **/
/**                                                                        **/
/****************************************************************************/
