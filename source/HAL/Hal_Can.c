/*
                             *******************
******************************* C SOURCE FILE ********************************
**                           *******************                            **
**                                                                          **
**  Project   : SPC570S_FBL                                                 **
**  Filename  : Hal_Can.c                                                   **
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

#define CAN_DRV_C_SRC

/****************************************************************************/
/**                                                                        **/
/**                      MODULES USED                                      **/
/**                                                                        **/
/****************************************************************************/
#include "Hal_Can.h"

/****************************************************************************/
/**                                                                        **/
/**                      DEFINITIONS AND MACROS                            **/
/**                                                                        **/
/****************************************************************************/
#define ID_Table0_CAN1 ( *(vuint32_t* )( 0xFBEC00E0UL ) )
#define ID_Table1_CAN1 ( *(vuint32_t* )( 0xFBEC00E4UL ) )
#define ID_Table2_CAN1 ( *(vuint32_t* )( 0xFBEC00E8UL ) )
#define ID_Table3_CAN1 ( *(vuint32_t* )( 0xFBEC00ECUL ) )
#define ID_Table4_CAN1 ( *(vuint32_t* )( 0xFBEC00F0UL ) )
#define ID_Table5_CAN1 ( *(vuint32_t* )( 0xFBEC00F4UL ) )
#define ID_Table6_CAN1 ( *(vuint32_t* )( 0xFBEC00F8UL ) )
#define ID_Table7_CAN1 ( *(vuint32_t* )( 0xFBEC00FCUL ) )

#define ID_Table0_CAN0 ( *(vuint32_t* )( 0xFFEC00E0UL ) )
#define ID_Table1_CAN0 ( *(vuint32_t* )( 0xFFEC00E4UL ) )
#define ID_Table2_CAN0 ( *(vuint32_t* )( 0xFFEC00E8UL ) )
#define ID_Table3_CAN0 ( *(vuint32_t* )( 0xFFEC00ECUL ) )
#define ID_Table4_CAN0 ( *(vuint32_t* )( 0xFFEC00F0UL ) )
#define ID_Table5_CAN0 ( *(vuint32_t* )( 0xFFEC00F4UL ) )
#define ID_Table6_CAN0 ( *(vuint32_t* )( 0xFFEC00F8UL ) )
#define ID_Table7_CAN0 ( *(vuint32_t* )( 0xFFEC00FCUL ) )

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
CAN_MESSAGE_OBJECT_t g_can_RxData_18FF31EF;
CAN_MESSAGE_OBJECT_t g_can_RxData_REQUEST;

uint8_t u8g_Hal_RecvNewDataforDiag;
uint32_t u32g_Hal_TimeoutCan1Cnt = 0U;
uint32_t u32g_Hal_TimeoutCan2Cnt = 0U;
uint8_t u8g_can1BusOffFlag = 0;
uint8_t u8g_can2BusOffFlag = 0;

/****************************************************************************/
/**                                                                        **/
/**                      GLOBAL VARIABLES                                  **/
/**                                                                        **/
/****************************************************************************/
static volatile struct FLEXCAN_tag *stsp_CAN_Main = &FLEXCAN_1;
static volatile struct FLEXCAN_tag *stsp_CAN_Sub = &FLEXCAN_0;
static const uint32_t RX_Id_Table[ FLEXCAN1_RX_MB ] =
{
	0x18FF31EFU,
	CANID_DIAG_PHYS_REQUEST,
	CANID_DIAG_FUNC_REQUEST
};

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
void Hal_CAN1_Init ( void )
{
	uint8_t u8_MsgBufIdx;

	SPCSetPeripheralClockMode( SPC5_FLEXCAN1_PCTL, SPC5_ME_PCTL_RUN( 2 ) | SPC5_ME_PCTL_LP( 2 ) );

	stsp_CAN_Main->MCR.B.HALT = 1U; /* Freeze Mode request */
  stsp_CAN_Main->MCR.B.MDIS = 0U; /* Enable the FlexCAN module */

  /* Initialize the Module configuration register */
  stsp_CAN_Main->MCR.B.FEN = 1U; /* FIFO enabled */
  stsp_CAN_Main->MCR.B.IDAM = 0U; /* FIFO, One full ID (standard or extended) per filter element */
  stsp_CAN_Main->MCR.B.BCC = 1U; /* Individual Rx masking and queue feature are enabled */

  stsp_CAN_Main->CTRL.B.PRESDIV = 1U; /* 20MHz/2(PRESDIV + 1 ) = 10MHz */
  stsp_CAN_Main->CTRL.B.RJW = 0U; /* 1(Resync Jump Width) = RJW + 1 */
  stsp_CAN_Main->CTRL.B.PROPSEG = 7U; /* 8(Propagation segment) = PROPSEG + 1 */
  stsp_CAN_Main->CTRL.B.PSEG1 = 6U; /* 7(Phase Buffer segment 1) = PSEG1 + 1 */
  stsp_CAN_Main->CTRL.B.PSEG2 = 3U; /* 4(Phase Buffer segment 2) = PSEG2 + 1 */

	stsp_CAN_Main->CTRL.B.LPB = 0U; /* Loop back disabled */
	stsp_CAN_Main->MCR.B.SRX_DIS = 1U; /* Self reception disabled */

  stsp_CAN_Main->IMASK1.B.BUF5M = 1U; /* MB5 completed reception or frames available in the FIFO. Interrupt is enabled */

	stsp_CAN_Main->MB[0].MB_CS.R = 0x04000000U; /* MB CODE: 0100b EMPTY: MB is active and empty */

	stsp_CAN_Main->MCR.B.MDIS = 0U; /* Enable the FlexCAN module */

	/* RX MB initialization.*/
	for( u8_MsgBufIdx = 0U; u8_MsgBufIdx < FLEXCAN1_RX_MB; u8_MsgBufIdx++ )
	{
		stsp_CAN_Main->MB[ u8_MsgBufIdx ].MB_CS.B.IDE = 1U; /* Frame format is extended */
		stsp_CAN_Main->MB[ u8_MsgBufIdx ].MB_ID.B.ID = RX_Id_Table[ u8_MsgBufIdx ];
		stsp_CAN_Main->MB[ u8_MsgBufIdx ].MB_CS.B.CODE = 4U;
	}

  ID_Table0_CAN1 = ( RX_Id_Table[ 0 ] << 1 ) | ( 0x40000000 ); /* ID(0x00000000)<<1 + EXT */
  ID_Table1_CAN1 = ( RX_Id_Table[ 1 ] << 1 ) | ( 0x40000000 ); /* ID(0x00000000)<<1 + EXT */
  ID_Table2_CAN1 = ( RX_Id_Table[ 2 ] << 1 ) | ( 0x40000000 ); /* ID(0x00000000)<<1 + EXT */
  ID_Table3_CAN1 = ( 0x00000000U << 1U ) | ( 0x40000000U ); /* ID(0x00000000)<<1 + EXT */
  ID_Table4_CAN1 = ( 0x00000000U << 1U ) | ( 0x40000000U ); /* ID(0x00000000)<<1 + EXT */
  ID_Table5_CAN1 = ( 0x00000000U << 1U ) | ( 0x40000000U ); /* ID(0x00000000)<<1 + EXT */
  ID_Table6_CAN1 = ( 0x00000000U << 1U ) | ( 0x40000000U ); /* ID(0x00000000)<<1 + EXT */
  ID_Table7_CAN1 = ( 0x00000000U << 1U ) | ( 0x40000000U ); /* ID(0x00000000)<<1 + EXT */

  /* Set individual mask register for ID filtering for FIFO,  all ID bits must match*/
  stsp_CAN_Main->RXIMR[ 0 ].R = 0x18FFFFFFU;
  stsp_CAN_Main->RXIMR[ 1 ].R = 0x18FFFFFFU;
  stsp_CAN_Main->RXIMR[ 2 ].R = 0x18FFFFFFU;
  stsp_CAN_Main->RXIMR[ 3 ].R = 0x18FFFFFFU;
  stsp_CAN_Main->RXIMR[ 4 ].R = 0x18FFFFFFU;
  stsp_CAN_Main->RXIMR[ 5 ].R = 0x18FFFFFFU;
  stsp_CAN_Main->RXIMR[ 6 ].R = 0x18FFFFFFU;
  stsp_CAN_Main->RXIMR[ 7 ].R = 0x18FFFFFFU;

	/* TX MB initialization.*/
	for( u8_MsgBufIdx = 0U; u8_MsgBufIdx < FLEXCAN1_TX_MB; u8_MsgBufIdx++ )
	{
		stsp_CAN_Main->MB[ u8_MsgBufIdx + 8U ].MB_CS.B.CODE = 8U;
	}

	( void )stsp_CAN_Main->TIMER.R;

  /* Interrupt sources initialization.*/
  stsp_CAN_Main->CTRL.R |= CAN_CTRL_BOFF_MSK;

  INTC_PSR( SPC5_FLEXCAN1_ESR_BOFF_NUMBER ) = INTC_PSR_ENABLE( INTC_PSR_CORE0, 7 );
  INTC_PSR( SPC5_FLEXCAN1_BUF_04_07_NUMBER ) = INTC_PSR_ENABLE( INTC_PSR_CORE0, 10 );

	stsp_CAN_Main->MCR.B.HALT = 0U;  /* No Freeze Mode request */
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void Hal_CAN2_Init ( void )
{
	uint8_t u8_MsgBufIdx;

	SPCSetPeripheralClockMode( SPC5_FLEXCAN0_PCTL, SPC5_ME_PCTL_RUN( 2 ) | SPC5_ME_PCTL_LP( 2 ) );

	stsp_CAN_Sub->MCR.B.HALT = 1U; /* Freeze Mode request */
  stsp_CAN_Sub->MCR.B.MDIS = 0U; /* Enable the FlexCAN module */

  /* Initialize the Module configuration register */
  stsp_CAN_Sub->MCR.B.FEN = 1U; /* FIFO enabled */
  stsp_CAN_Sub->MCR.B.IDAM = 0U; /* FIFO, One full ID (standard or extended) per filter element*/
  stsp_CAN_Sub->MCR.B.BCC = 1; /* Individual Rx masking and queue feature are enabled*/

  stsp_CAN_Sub->CTRL.B.PRESDIV = 1U; /* 20MHz/2(PRESDIV + 1 ) = 10MHz */
  stsp_CAN_Sub->CTRL.B.RJW = 0U; /* 1(Resync Jump Width) = RJW + 1 */
  stsp_CAN_Sub->CTRL.B.PROPSEG = 7U; /* 8(Propagation segment) = PROPSEG + 1 */
  stsp_CAN_Sub->CTRL.B.PSEG1 = 6U; /* 7(Phase Buffer segment 1) = PSEG1 + 1 */
  stsp_CAN_Sub->CTRL.B.PSEG2 = 3U; /* 4(Phase Buffer segment 2) = PSEG2 + 1 */

	stsp_CAN_Sub->CTRL.B.LPB = 0U; /* Loop back disabled */
	stsp_CAN_Sub->MCR.B.SRX_DIS = 1U; /* Self reception disabled */

  stsp_CAN_Sub->IMASK1.B.BUF5M = 1U; /* MB5 completed reception or frames available in the FIFO. Interrupt	is enabled */

  stsp_CAN_Sub->MB[0].MB_CS.R = 0x04000000U; /* MB CODE: 0100b EMPTY: MB is active and empty */

  stsp_CAN_Sub->MCR.B.MDIS = 0U; /* Enable the FlexCAN module */

	/* RX MB initialization.*/
	for( u8_MsgBufIdx = 0U; u8_MsgBufIdx < FLEXCAN1_RX_MB; u8_MsgBufIdx++ )
	{
		stsp_CAN_Sub->MB[ u8_MsgBufIdx ].MB_CS.B.IDE = 1U; /* Frame format is extended */
		stsp_CAN_Sub->MB[ u8_MsgBufIdx ].MB_ID.B.ID = RX_Id_Table[ u8_MsgBufIdx ];
		stsp_CAN_Sub->MB[ u8_MsgBufIdx ].MB_CS.B.CODE = 4U;
	}

  ID_Table0_CAN0 = ( RX_Id_Table[ 0 ] << 1 )|( 0x40000000 ); /* ID(0x00000000)<<1 + EXT */
  ID_Table1_CAN0 = ( RX_Id_Table[ 1 ] << 1 )|( 0x40000000 ); /* ID(0x00000000)<<1 + EXT */
  ID_Table2_CAN0 = ( RX_Id_Table[ 2 ] << 1 )|( 0x40000000 ); /* ID(0x00000000)<<1 + EXT */
  ID_Table3_CAN0 = ( 0x00000000U << 1U ) | ( 0x40000000U ); /* ID(0x00000000)<<1 + EXT */
  ID_Table4_CAN0 = ( 0x00000000U << 1U ) | ( 0x40000000U ); /* ID(0x00000000)<<1 + EXT */
  ID_Table5_CAN0 = ( 0x00000000U << 1U ) | ( 0x40000000U ); /* ID(0x00000000)<<1 + EXT */
  ID_Table6_CAN0 = ( 0x00000000U << 1U ) | ( 0x40000000U ); /* ID(0x00000000)<<1 + EXT */
  ID_Table7_CAN0 = ( 0x00000000U << 1U ) | ( 0x40000000U ); /* ID(0x00000000)<<1 + EXT */

  /* Set individual mask register for ID filtering for FIFO,  all ID bits must match*/
  stsp_CAN_Sub->RXIMR[ 0 ].R = 0x18FFFFFFU;
  stsp_CAN_Sub->RXIMR[ 1 ].R = 0x18FFFFFFU;
  stsp_CAN_Sub->RXIMR[ 2 ].R = 0x18FFFFFFU;
  stsp_CAN_Sub->RXIMR[ 3 ].R = 0x18FFFFFFU;
  stsp_CAN_Sub->RXIMR[ 4 ].R = 0x18FFFFFFU;
  stsp_CAN_Sub->RXIMR[ 5 ].R = 0x18FFFFFFU;
  stsp_CAN_Sub->RXIMR[ 6 ].R = 0x18FFFFFFU;
  stsp_CAN_Sub->RXIMR[ 7 ].R = 0x18FFFFFFU;

	/* TX MB initialization.*/
	for( u8_MsgBufIdx = 0U; u8_MsgBufIdx < FLEXCAN1_TX_MB; u8_MsgBufIdx++ )
	{
		stsp_CAN_Sub->MB[ u8_MsgBufIdx + 8U ].MB_CS.B.CODE = 8U;
	}

	( void )stsp_CAN_Sub->TIMER.R;

  /* Interrupt sources initialization.*/
  stsp_CAN_Sub->CTRL.R |= CAN_CTRL_BOFF_MSK;

  INTC_PSR( SPC5_FLEXCAN0_ESR_BOFF_NUMBER ) = INTC_PSR_ENABLE( INTC_PSR_CORE0, 7 );
  INTC_PSR( SPC5_FLEXCAN0_BUF_04_07_NUMBER ) = INTC_PSR_ENABLE( INTC_PSR_CORE0, 10 );

	stsp_CAN_Sub->MCR.B.HALT = 0U; /* No Freeze Mode request */
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void CAN1_Drv_SendFrame ( uint8_t mailbox, CAN_MESSAGE_OBJECT_t g_can_TxData )
{
	stsp_CAN_Main->MB[ mailbox ].MB_CS.B.IDE = 1U;
	stsp_CAN_Main->MB[ mailbox ].MB_CS.B.RTR = 0U;

	stsp_CAN_Main->MB[ mailbox ].MB_CS.B.CODE = 0x08U;

	stsp_CAN_Main->MB[ mailbox ].MB_ID.B.ID = g_can_TxData.ID;

	stsp_CAN_Main->MB[ mailbox ].MB_DATAL.B.DATA_BYTE_0 = g_can_TxData.Data[ 0 ];
	stsp_CAN_Main->MB[ mailbox ].MB_DATAL.B.DATA_BYTE_1 = g_can_TxData.Data[ 1 ];
	stsp_CAN_Main->MB[ mailbox ].MB_DATAL.B.DATA_BYTE_2 = g_can_TxData.Data[ 2 ];
	stsp_CAN_Main->MB[ mailbox ].MB_DATAL.B.DATA_BYTE_3 = g_can_TxData.Data[ 3 ];
	stsp_CAN_Main->MB[ mailbox ].MB_DATAH.B.DATA_BYTE_4 = g_can_TxData.Data[ 4 ];
	stsp_CAN_Main->MB[ mailbox ].MB_DATAH.B.DATA_BYTE_5 = g_can_TxData.Data[ 5 ];
	stsp_CAN_Main->MB[ mailbox ].MB_DATAH.B.DATA_BYTE_6 = g_can_TxData.Data[ 6 ];
	stsp_CAN_Main->MB[ mailbox ].MB_DATAH.B.DATA_BYTE_7 = g_can_TxData.Data[ 7 ];

	stsp_CAN_Main->MB[ mailbox ].MB_CS.B.LENGTH = 8U;

	stsp_CAN_Main->MB[ mailbox ].MB_CS.B.CODE = 0x0CU;

	return;
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void CAN2_Drv_SendFrame ( uint8_t mailbox, CAN_MESSAGE_OBJECT_t g_can_TxData )
{
	stsp_CAN_Sub->MB[ mailbox ].MB_CS.B.IDE = 1U;
	stsp_CAN_Sub->MB[ mailbox ].MB_CS.B.RTR = 0U;
	stsp_CAN_Sub->MB[ mailbox ].MB_CS.B.CODE = 0x08U;

	stsp_CAN_Sub->MB[ mailbox ].MB_ID.B.ID = g_can_TxData.ID;

	stsp_CAN_Sub->MB[ mailbox ].MB_DATAL.B.DATA_BYTE_0 = g_can_TxData.Data[ 0 ];
	stsp_CAN_Sub->MB[ mailbox ].MB_DATAL.B.DATA_BYTE_1 = g_can_TxData.Data[ 1 ];
	stsp_CAN_Sub->MB[ mailbox ].MB_DATAL.B.DATA_BYTE_2 = g_can_TxData.Data[ 2 ];
	stsp_CAN_Sub->MB[ mailbox ].MB_DATAL.B.DATA_BYTE_3 = g_can_TxData.Data[ 3 ];
	stsp_CAN_Sub->MB[ mailbox ].MB_DATAH.B.DATA_BYTE_4 = g_can_TxData.Data[ 4 ];
	stsp_CAN_Sub->MB[ mailbox ].MB_DATAH.B.DATA_BYTE_5 = g_can_TxData.Data[ 5 ];
	stsp_CAN_Sub->MB[ mailbox ].MB_DATAH.B.DATA_BYTE_6 = g_can_TxData.Data[ 6 ];
	stsp_CAN_Sub->MB[ mailbox ].MB_DATAH.B.DATA_BYTE_7 = g_can_TxData.Data[ 7 ];

	stsp_CAN_Sub->MB[ mailbox ].MB_CS.B.LENGTH = 8U;

	stsp_CAN_Sub->MB[ mailbox ].MB_CS.B.CODE = 0x0CU;

	return;
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/* Period time : 100ms		   																								*/
/* Execution time : 4.5us																										*/
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void SPC5_FLEXCAN1_BUF_04_07_HANDLER ( void )
{
	uint32_t RxId;

	IRQ_PROLOGUE();

	/* Frames available in FIFO */
	if( stsp_CAN_Main->IFLAG1.B.BUF5I == 1U )
	{
		/* Length */
		u32g_Hal_TimeoutCan1Cnt = 0U;
		u8g_can1BusOffFlag = 0U;

		RxId = stsp_CAN_Main->MB[ 0 ].MB_ID.B.ID;

		switch( RxId )
		{
			case 0x18FF31EFU:
				g_can_RxData_18FF31EF.LENGTH = (uint8_t)( stsp_CAN_Main->MB[ 0 ].MB_CS.B.LENGTH );
				/* Time stamp */
				g_can_RxData_18FF31EF.TIME_STAMP = (uint16_t)( stsp_CAN_Main->MB[ 0 ].MB_CS.B.TIME_STAMP );
				/* ID */
				g_can_RxData_18FF31EF.ID = stsp_CAN_Main->MB[ 0 ].MB_ID.B.ID;

				g_can_RxData_18FF31EF.Data[ 0 ] = ( uint8_t )stsp_CAN_Main->MB[ 0 ].MB_DATAL.B.DATA_BYTE_0;
				g_can_RxData_18FF31EF.Data[ 1 ] = ( uint8_t )stsp_CAN_Main->MB[ 0 ].MB_DATAL.B.DATA_BYTE_1;
				g_can_RxData_18FF31EF.Data[ 2 ] = ( uint8_t )stsp_CAN_Main->MB[ 0 ].MB_DATAL.B.DATA_BYTE_2;
				g_can_RxData_18FF31EF.Data[ 3 ] = ( uint8_t )stsp_CAN_Main->MB[ 0 ].MB_DATAL.B.DATA_BYTE_3;
				g_can_RxData_18FF31EF.Data[ 4 ] = ( uint8_t )stsp_CAN_Main->MB[ 0 ].MB_DATAH.B.DATA_BYTE_4;
				g_can_RxData_18FF31EF.Data[ 5 ] = ( uint8_t )stsp_CAN_Main->MB[ 0 ].MB_DATAH.B.DATA_BYTE_5;
				g_can_RxData_18FF31EF.Data[ 6 ] = ( uint8_t )stsp_CAN_Main->MB[ 0 ].MB_DATAH.B.DATA_BYTE_6;
				g_can_RxData_18FF31EF.Data[ 7 ] = ( uint8_t )stsp_CAN_Main->MB[ 0 ].MB_DATAH.B.DATA_BYTE_7;
				break;
			case CANID_DIAG_PHYS_REQUEST:
				g_can_RxData_REQUEST.LENGTH = (uint8_t)( stsp_CAN_Main->MB[ 0 ].MB_CS.B.LENGTH );
				/* Time stamp */
				g_can_RxData_REQUEST.TIME_STAMP = (uint16_t)( stsp_CAN_Main->MB[ 0 ].MB_CS.B.TIME_STAMP );
				/* ID */
				g_can_RxData_REQUEST.ID = stsp_CAN_Main->MB[ 0 ].MB_ID.B.ID;

				g_can_RxData_REQUEST.Data[ 0 ] = ( uint8_t )stsp_CAN_Main->MB[ 0 ].MB_DATAL.B.DATA_BYTE_0;
				g_can_RxData_REQUEST.Data[ 1 ] = ( uint8_t )stsp_CAN_Main->MB[ 0 ].MB_DATAL.B.DATA_BYTE_1;
				g_can_RxData_REQUEST.Data[ 2 ] = ( uint8_t )stsp_CAN_Main->MB[ 0 ].MB_DATAL.B.DATA_BYTE_2;
				g_can_RxData_REQUEST.Data[ 3 ] = ( uint8_t )stsp_CAN_Main->MB[ 0 ].MB_DATAL.B.DATA_BYTE_3;
				g_can_RxData_REQUEST.Data[ 4 ] = ( uint8_t )stsp_CAN_Main->MB[ 0 ].MB_DATAH.B.DATA_BYTE_4;
				g_can_RxData_REQUEST.Data[ 5 ] = ( uint8_t )stsp_CAN_Main->MB[ 0 ].MB_DATAH.B.DATA_BYTE_5;
				g_can_RxData_REQUEST.Data[ 6 ] = ( uint8_t )stsp_CAN_Main->MB[ 0 ].MB_DATAH.B.DATA_BYTE_6;
				g_can_RxData_REQUEST.Data[ 7 ] = ( uint8_t )stsp_CAN_Main->MB[ 0 ].MB_DATAH.B.DATA_BYTE_7;
				u8g_Hal_RecvNewDataforDiag = 1U;
				break;
			case CANID_DIAG_FUNC_REQUEST:
				g_can_RxData_REQUEST.LENGTH = (uint8_t)( stsp_CAN_Main->MB[ 0 ].MB_CS.B.LENGTH );
				/* Time stamp */
				g_can_RxData_REQUEST.TIME_STAMP = (uint16_t)( stsp_CAN_Main->MB[ 0 ].MB_CS.B.TIME_STAMP );
				/* ID */
				g_can_RxData_REQUEST.ID = stsp_CAN_Main->MB[ 0 ].MB_ID.B.ID;

				g_can_RxData_REQUEST.Data[ 0 ] = ( uint8_t )stsp_CAN_Main->MB[ 0 ].MB_DATAL.B.DATA_BYTE_0;
				g_can_RxData_REQUEST.Data[ 1 ] = ( uint8_t )stsp_CAN_Main->MB[ 0 ].MB_DATAL.B.DATA_BYTE_1;
				g_can_RxData_REQUEST.Data[ 2 ] = ( uint8_t )stsp_CAN_Main->MB[ 0 ].MB_DATAL.B.DATA_BYTE_2;
				g_can_RxData_REQUEST.Data[ 3 ] = ( uint8_t )stsp_CAN_Main->MB[ 0 ].MB_DATAL.B.DATA_BYTE_3;
				g_can_RxData_REQUEST.Data[ 4 ] = ( uint8_t )stsp_CAN_Main->MB[ 0 ].MB_DATAH.B.DATA_BYTE_4;
				g_can_RxData_REQUEST.Data[ 5 ] = ( uint8_t )stsp_CAN_Main->MB[ 0 ].MB_DATAH.B.DATA_BYTE_5;
				g_can_RxData_REQUEST.Data[ 6 ] = ( uint8_t )stsp_CAN_Main->MB[ 0 ].MB_DATAH.B.DATA_BYTE_6;
				g_can_RxData_REQUEST.Data[ 7 ] = ( uint8_t )stsp_CAN_Main->MB[ 0 ].MB_DATAH.B.DATA_BYTE_7;
				u8g_Hal_RecvNewDataforDiag = 1U;
				break;
			default:
				break;
		}

		/* Clear INT flag, */
		stsp_CAN_Main->IFLAG1.B.BUF5I = 0x1U;
	}

	IRQ_EPILOGUE();
}

/*--------------------------------------------------------------------------*/
/* Function  |    CanIsrHandler()                                           */
/* Name      |                                                              */
/* Period time : 100ms		   																								*/
/* Execution time : 4.5us																										*/
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void SPC5_FLEXCAN0_BUF_04_07_HANDLER ( void )
{
	uint32_t RxId;

	IRQ_PROLOGUE();

	/* Frames available in FIFO */
	if( stsp_CAN_Sub->IFLAG1.B.BUF5I == 1U )
	{
		/* Length */
		u32g_Hal_TimeoutCan2Cnt = 0;
		u8g_can2BusOffFlag = 0U;

		RxId = stsp_CAN_Sub->MB[ 0 ].MB_ID.B.ID;

		switch( RxId )
		{
			case 0x18FF31EFU:
				g_can_RxData_18FF31EF.LENGTH = (uint8_t)( stsp_CAN_Sub->MB[ 0 ].MB_CS.B.LENGTH );
				/* Time stamp */
				g_can_RxData_18FF31EF.TIME_STAMP = (uint16_t)( stsp_CAN_Sub->MB[ 0 ].MB_CS.B.TIME_STAMP );
				/* ID */
				g_can_RxData_18FF31EF.ID = stsp_CAN_Sub->MB[ 0 ].MB_ID.B.ID;

				g_can_RxData_18FF31EF.Data[ 0 ] = ( uint8_t )stsp_CAN_Sub->MB[ 0 ].MB_DATAL.B.DATA_BYTE_0;
				g_can_RxData_18FF31EF.Data[ 1 ] = ( uint8_t )stsp_CAN_Sub->MB[ 0 ].MB_DATAL.B.DATA_BYTE_1;
				g_can_RxData_18FF31EF.Data[ 2 ] = ( uint8_t )stsp_CAN_Sub->MB[ 0 ].MB_DATAL.B.DATA_BYTE_2;
				g_can_RxData_18FF31EF.Data[ 3 ] = ( uint8_t )stsp_CAN_Sub->MB[ 0 ].MB_DATAL.B.DATA_BYTE_3;
				g_can_RxData_18FF31EF.Data[ 4 ] = ( uint8_t )stsp_CAN_Sub->MB[ 0 ].MB_DATAH.B.DATA_BYTE_4;
				g_can_RxData_18FF31EF.Data[ 5 ] = ( uint8_t )stsp_CAN_Sub->MB[ 0 ].MB_DATAH.B.DATA_BYTE_5;
				g_can_RxData_18FF31EF.Data[ 6 ] = ( uint8_t )stsp_CAN_Sub->MB[ 0 ].MB_DATAH.B.DATA_BYTE_6;
				g_can_RxData_18FF31EF.Data[ 7 ] = ( uint8_t )stsp_CAN_Sub->MB[ 0 ].MB_DATAH.B.DATA_BYTE_7;
				break;
			case CANID_DIAG_PHYS_REQUEST:
				g_can_RxData_REQUEST.LENGTH = (uint8_t)( stsp_CAN_Sub->MB[ 0 ].MB_CS.B.LENGTH );
				/* Time stamp */
				g_can_RxData_REQUEST.TIME_STAMP = (uint16_t)( stsp_CAN_Sub->MB[ 0 ].MB_CS.B.TIME_STAMP );
				/* ID */
				g_can_RxData_REQUEST.ID = stsp_CAN_Sub->MB[ 0 ].MB_ID.B.ID;

				g_can_RxData_REQUEST.Data[ 0 ] = ( uint8_t )stsp_CAN_Sub->MB[ 0 ].MB_DATAL.B.DATA_BYTE_0;
				g_can_RxData_REQUEST.Data[ 1 ] = ( uint8_t )stsp_CAN_Sub->MB[ 0 ].MB_DATAL.B.DATA_BYTE_1;
				g_can_RxData_REQUEST.Data[ 2 ] = ( uint8_t )stsp_CAN_Sub->MB[ 0 ].MB_DATAL.B.DATA_BYTE_2;
				g_can_RxData_REQUEST.Data[ 3 ] = ( uint8_t )stsp_CAN_Sub->MB[ 0 ].MB_DATAL.B.DATA_BYTE_3;
				g_can_RxData_REQUEST.Data[ 4 ] = ( uint8_t )stsp_CAN_Sub->MB[ 0 ].MB_DATAH.B.DATA_BYTE_4;
				g_can_RxData_REQUEST.Data[ 5 ] = ( uint8_t )stsp_CAN_Sub->MB[ 0 ].MB_DATAH.B.DATA_BYTE_5;
				g_can_RxData_REQUEST.Data[ 6 ] = ( uint8_t )stsp_CAN_Sub->MB[ 0 ].MB_DATAH.B.DATA_BYTE_6;
				g_can_RxData_REQUEST.Data[ 7 ] = ( uint8_t )stsp_CAN_Sub->MB[ 0 ].MB_DATAH.B.DATA_BYTE_7;
				u8g_Hal_RecvNewDataforDiag = 1U;
				break;
			case CANID_DIAG_FUNC_REQUEST:
				g_can_RxData_REQUEST.LENGTH = (uint8_t)( stsp_CAN_Sub->MB[ 0 ].MB_CS.B.LENGTH );
				/* Time stamp */
				g_can_RxData_REQUEST.TIME_STAMP = (uint16_t)( stsp_CAN_Sub->MB[ 0 ].MB_CS.B.TIME_STAMP );
				/* ID */
				g_can_RxData_REQUEST.ID = stsp_CAN_Sub->MB[ 0 ].MB_ID.B.ID;

				g_can_RxData_REQUEST.Data[ 0 ] = ( uint8_t )stsp_CAN_Sub->MB[ 0 ].MB_DATAL.B.DATA_BYTE_0;
				g_can_RxData_REQUEST.Data[ 1 ] = ( uint8_t )stsp_CAN_Sub->MB[ 0 ].MB_DATAL.B.DATA_BYTE_1;
				g_can_RxData_REQUEST.Data[ 2 ] = ( uint8_t )stsp_CAN_Sub->MB[ 0 ].MB_DATAL.B.DATA_BYTE_2;
				g_can_RxData_REQUEST.Data[ 3 ] = ( uint8_t )stsp_CAN_Sub->MB[ 0 ].MB_DATAL.B.DATA_BYTE_3;
				g_can_RxData_REQUEST.Data[ 4 ] = ( uint8_t )stsp_CAN_Sub->MB[ 0 ].MB_DATAH.B.DATA_BYTE_4;
				g_can_RxData_REQUEST.Data[ 5 ] = ( uint8_t )stsp_CAN_Sub->MB[ 0 ].MB_DATAH.B.DATA_BYTE_5;
				g_can_RxData_REQUEST.Data[ 6 ] = ( uint8_t )stsp_CAN_Sub->MB[ 0 ].MB_DATAH.B.DATA_BYTE_6;
				g_can_RxData_REQUEST.Data[ 7 ] = ( uint8_t )stsp_CAN_Sub->MB[ 0 ].MB_DATAH.B.DATA_BYTE_7;
				u8g_Hal_RecvNewDataforDiag = 1U;
				break;
			default:
				break;
		}

		/* Clear INT flag, */
		stsp_CAN_Sub->IFLAG1.B.BUF5I = 0x1U;
	}

	IRQ_EPILOGUE();
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void SPC5_FLEXCAN1_ESR_BOFF_HANDLER ( void )
{
	IRQ_PROLOGUE();

	if( stsp_CAN_Main->ESR.B.BOFF_INT != 0U )
	{
		stsp_CAN_Main->ESR.B.BOFF_INT = 1U;
		u8g_can1BusOffFlag = 1U;
	}

	IRQ_EPILOGUE();
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void SPC5_FLEXCAN0_ESR_BOFF_HANDLER (void )
{
	IRQ_PROLOGUE();

	if( stsp_CAN_Sub->ESR.B.BOFF_INT != 0U )
	{
		stsp_CAN_Sub->ESR.B.BOFF_INT = 1U;
		u8g_can2BusOffFlag = 1U;
	}

	IRQ_EPILOGUE();
}

/****************************************************************************/
/**                                                                        **/
/**                      LOCAL FUNCTIONS                                   **/
/**                                                                        **/
/****************************************************************************/

/****************************************************************************/
/**                                                                        **/
/**                      EOF                                               **/
/**                                                                        **/
/****************************************************************************/
