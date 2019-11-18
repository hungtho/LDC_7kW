/*
                             *******************
******************************* C HEADER FILE ********************************
**                           *******************                            **
**                                                                          **
**  Project   : SPC570S_FBL                                                 **
**  Filename  : Hal_Can.h                                                   **
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

#ifndef CAN_DRV_INCLUDED
#define CAN_DRV_INCLUDED

/****************************************************************************/
/**                                                                        **/
/**                      MODULES USED                                      **/
/**                                                                        **/
/****************************************************************************/
#include "LdcConfig.h"

/****************************************************************************/
/**                                                                        **/
/**                      DEFINITIONS AND MACROS                            **/
/**                                                                        **/
/****************************************************************************/
#define FLEXCAN1_MAX_MB		( 5U )
#define FLEXCAN1_TX_MB		( 3U )
#define FLEXCAN1_RX_MB		( 3U )
#define TX_MB8						( 8U )
#define TX_MB9						( 9U )
#define TX_MB10						( 10U )

#define CAN_RX_MB0				( 0U )		/* CAN CH0 RX mailbox index */
#define CAN_RX_MB1 		  	( 1U )

#define CAN_TX_MB2				( 2U )		/* CAN CH0 TX mailbox index */
#define CAN_TX_MB3				( 3U )		/* CAN CH0 TX mailbox index */
#define CAN_TX_MB4				( 4U )		/* CAN CH0 TX mailbox index */

#define RX_BUF_MASK				( 0x00000003U )

/* Shared CAN Message RAM */
#define CAN_SHARED_RAM_ADD_START    ( 0xFFEC0080UL )
#define CAN_SHARED_RAM_ADD_END      ( 0xFFEC027FUL )

#define CAN_MCR_WRN_EN              ( 1UL << 21 )
#define CAN_CTRL_RWRN_MSK           ( 1UL<< 10 )
#define CAN_CTRL_TWRN_MSK           ( 1UL<< 11 )
#define CAN_CTRL_ERR_MSK            ( 1UL<< 14 )
#define CAN_CTRL_BOFF_MSK           ( 1UL<< 15 )

#define CAN_ESR_ERR_INT             ( 1UL << 1 )
#define CAN_ESR_BOFF_INT            ( 1UL << 2 )
#define CAN_ESR_TWRN_INT            ( 1UL << 17 )
#define CAN_ESR_RWRN_INT            ( 1UL << 16 )

/****************************************************************************/
/**                                                                        **/
/**                      TYPEDEFS AND STRUCTURES                           **/
/**                                                                        **/
/****************************************************************************/
typedef struct
{
	uint8_t  IDE;
	uint8_t  LENGTH;
	uint16_t TIME_STAMP;
	uint32_t ID;
	uint8_t  Data[ 8 ];
}CAN_MESSAGE_OBJECT_t;

/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED VARIABLES                                **/
/**                                                                        **/
/****************************************************************************/

# ifndef CAN_DRV_C_SRC
# endif

extern CAN_MESSAGE_OBJECT_t g_can_RxData_18FF31EF;
extern CAN_MESSAGE_OBJECT_t g_can_RxData_REQUEST;

extern uint8_t u8g_Hal_RecvNewDataforDiag;
extern uint32_t u32g_Hal_TimeoutCan1Cnt;
extern uint32_t u32g_Hal_TimeoutCan2Cnt;
extern uint8_t u8g_can1BusOffFlag;
extern uint8_t u8g_can2BusOffFlag;

/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED FUNCTIONS                                **/
/**                                                                        **/
/****************************************************************************/
extern void Hal_CAN1_Init ( void );
extern void Hal_CAN2_Init ( void );
extern void CAN1_Drv_SendFrame ( uint8_t mailbox, CAN_MESSAGE_OBJECT_t g_can_TxData );
extern void CAN2_Drv_SendFrame ( uint8_t mailbox, CAN_MESSAGE_OBJECT_t g_can_TxData );
extern void SPC5_FLEXCAN1_BUF_04_07_HANDLER ( void );
extern void SPC5_FLEXCAN0_BUF_04_07_HANDLER ( void );
extern void SPC5_FLEXCAN1_ESR_BOFF_HANDLER ( void );
extern void SPC5_FLEXCAN0_ESR_BOFF_HANDLER ( void );

#endif

/****************************************************************************/
/**                                                                        **/
/**                      EOF                                               **/
/**                                                                        **/
/****************************************************************************/
