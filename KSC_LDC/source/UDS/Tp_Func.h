/*
                             *******************
******************************* C HEADER FILE ********************************
**                           *******************                            **
**                                                                          **
**  Project   : SPC570S_FBL                                                 **
**  Filename  : Tp_Func.h                                                   **
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

#ifndef TP_FUNC_INCLUDED
#define TP_FUNC_INCLUDED

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
#define TP_FUNC_MISRA

#define FT_SF		( 0U ) /* frame type: single frame */
#define FT_FF		( 1U ) /* frame type: first frame */
#define FT_CF		( 2U ) /* frame type: consecutive frame */
#define FT_FC		( 3U ) /* frame type, flow control frame */

#define MAX_TX_FRAME_SIZE		( 150U )
#define MAX_RX_FRAME_SIZE		( 150U )

/****************************************************************************/
/**                                                                        **/
/**                      TYPEDEFS AND STRUCTURES                           **/
/**                                                                        **/
/****************************************************************************/

/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED VARIABLES                                **/
/**                                                                        **/
/****************************************************************************/

# ifndef TP_FUNC_C_SRC
# endif

extern uint8_t u8g_TpTxBuf[ MAX_TX_FRAME_SIZE ];
extern uint8_t u8g_TpRxBuf[ MAX_RX_FRAME_SIZE ];
extern uint8_t u8g_SingleRxBuf[ 8 ];
extern uint8_t u8g_RxFrameType;

/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED FUNCTIONS                                **/
/**                                                                        **/
/****************************************************************************/
extern void Tp_Func_Init ( void );
extern void Tp_Func ( void );
extern uint8_t TpRecvProc ( uint8_t *u8p_IsEndFrame, uint8_t *u8p_RecvLen, uint8_t *u8p_ReqType );
extern void TpSingleFrame ( uint8_t u8_DataLen );
extern void TpFirstFrame ( uint8_t u8_DataLen );

#endif

/****************************************************************************/
/**                                                                        **/
/**                      EOF                                               **/
/**                                                                        **/
/****************************************************************************/
