/*
                             *******************
******************************* C HEADER FILE ********************************
**                           *******************                            **
**                                                                          **
**  Project   : SPC570S_FBL                                                 **
**  Filename  : Uds_Func.h                                                  **
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

#ifndef UDS_FUNC_INCLUDED
#define UDS_FUNC_INCLUDED

/****************************************************************************/
/**                                                                        **/
/**                      MODULES USED                                      **/
/**                                                                        **/
/****************************************************************************/
#include "Defines.h"

/****************************************************************************/
/**                                                                        **/
/**                      DEFINITIONS AND MACROS                            **/
/**                                                                        **/
/****************************************************************************/
#define PHYS																	( 0x01U )
#define FUNC    															( 0x02U )

#define NO																		( 0U )
#define YES   																( 1U )

#define DefaultSession        								( 0x01U )
#define ProgrammingSession    								( 0x02U )
#define ExtendedDiagSession										( 0x03U )

#define hardReset															( 0x01U )

#define RequestSeed														( 0x01U )
#define SendKey																( 0x02U )
#define SECURITY_LOCK													( 0x00U	)
#define SECURITY_UNLOCK												( 0x01U	)
#define SECURITY_SEQ_FALSE										( 0x00U	)
#define SECURITY_SEQ_TRUE											( 0x01U	)

#define enableRxAndTx													( 0x00U )
#define enableRxAndDisableTx									( 0x01U )
#define disableRxAndEnableTx									( 0x02U )
#define disableRxAndTx												( 0x03U )

#define CAN_CTRL_ENABLE												( 0x00U )
#define CAN_CTRL_DISABLE											( 0x01U )

#define ON																		( 0x01U )
#define OFF																		( 0x02U )
#define DTC_DISABLE														( 0x01U )
#define DTC_ENABLE														( 0x00U )

#define reportNumberOfDTCByStatusMask 				( 0x01U )
#define reportDTCByStatusMask									( 0x02U )

#define startRoutine													( 0x01U )

/*-----------------------------------------------------------------------------
    &&&~ Negative response codes
 -----------------------------------------------------------------------------*/
#define NrcPositiveResponse                         ( 0x00U )
#define NrcGeneralReject                            ( 0x10U )
#define NrcServiceNotSupported                      ( 0x11U )
#define NrcSubfunctionNotSupported                  ( 0x12U )
#define NrcInvalidFormat                            ( 0x13U )
#define NrcResponseTooLong                          ( 0x14U )
#define NrcBusyRepeatRequest                        ( 0x21U )
#define NrcConditionsNotCorrect                     ( 0x22U )
#define NrcRequestSequenceError                     ( 0x24U )
#define NrcRequestOutOfRange                        ( 0x31U )
#define NrcAccessDenied                             ( 0x33U )
#define NrcInvalidKey                               ( 0x35U )
#define NrcExceedNumOfAttempts                      ( 0x36U )
#define NrcTimeDelayNotExpired                      ( 0x37U )
#define NrcUploadDownloadNotAccepted                ( 0x70U )
#define NrcTransferDataSuspended                    ( 0x71U )
#define NrcGeneralProgrammingFailure                ( 0x72U )
#define NrcWrongBlockSequenceCounter                ( 0x73U )
#define NrcResponsePending                          ( 0x78U )
#define NrcSubfunctionNotSupportedInActiveSession		( 0x7EU )
#define NrcServiceNotSupportedInActiveSession       ( 0x7FU )
#define NrcRpmTooHigh                               ( 0x81U )
#define NrcRpmTooLow                                ( 0x82U )
#define NrcEngineIsRunning                          ( 0x83U )
#define NrcEngineIsNotRunning                       ( 0x84U )
#define NrcEngineRunTimeTooLow                      ( 0x85U )
#define NrcTemperatureTooHigh                       ( 0x86U )
#define NrcTemperatureTooLow                        ( 0x87U )
#define NrcVehicleSpeedTooHigh                      ( 0x88U )
#define NrcVehicleSpeedTooLow                       ( 0x89U )
#define NrcThrottleSpeedTooHigh                     ( 0x8AU )
#define NrcThrottleSpeedTooLow                      ( 0x8BU )
#define NrcTransmissionRangeInNeutral               ( 0x8CU )
#define NrcTransmissionRangeInGears                 ( 0x8DU )
#define NrcBrakeSwitchNotClosed                     ( 0x8FU )
#define NrcShifterLeverNotInPark                    ( 0x90U )
#define NrcTorqueConverterClutchLocked              ( 0x91U )
#define NrcVoltageTooHigh                           ( 0x92U )
#define NrcVoltageTooLow                            ( 0x93U )

/****************************************************************************/
/**                                                                        **/
/**                      TYPEDEFS AND STRUCTURES                           **/
/**                                                                        **/
/****************************************************************************/
typedef struct
{
	unsigned int u2_mReqType     :2; /* 0x01: Phys 0x02: Func */
	unsigned int u1_mSuppressRes :1; /* 0x00: No   0x01: Yes  */
	unsigned int u5_mDummy       :5;
}ST_DescMsgAddInfo;

/* Context information of a request */
typedef struct
{
	uint8_t *u8p_mReqData;
	uint16_t u16_mReqDataLen;
	uint8_t *u8p_mResData;
	uint16_t u16_mResDataLen;
	ST_DescMsgAddInfo st_mMsgAddInfo;
	uint8_t u8_mContext;
}ST_DescMsgContext;

typedef struct
{
	unsigned int u2_mIsValid     :2; /* 0x00: End, 0x01: Support, 0x02: Not Support */
	unsigned int u2_mReqType     :2; /* 0x01: Phys 0x02: Func */
	unsigned int u1_mHaveSubFunc :1; /* 0x00: No   0x01: Yes  */
	unsigned int u3_mDummy       :3;
	uint8_t u8_mServiceId;
	void ( * Func )( ST_DescMsgContext * );
}ST_DescService;

/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED VARIABLES                                **/
/**                                                                        **/
/****************************************************************************/

# ifndef UDS_FUNC_C_SRC
# endif

extern uint8_t u8g_ReqReset;
extern uint8_t u8g_CAN_TX_Ctrl;
extern uint8_t u8g_CAN_RX_Ctrl;
extern uint8_t u8_DtcControl;

extern uint8_t u8g_Eeprom_WriteData[ 32 ];

/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED FUNCTIONS                                **/
/**                                                                        **/
/****************************************************************************/
extern void Uds_Func_Init ( void );
extern void Uds_Func ( void );

#endif

/****************************************************************************/
/**                                                                        **/
/**                      EOF                                               **/
/**                                                                        **/
/****************************************************************************/
