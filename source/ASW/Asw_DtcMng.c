/*
                             *******************
******************************* C SOURCE FILE ********************************
**                           *******************                            **
**                                                                          **
**  Project   : KSC_LDC                                                     **
**  Filename  : Asw_DtcMng.c                                                **
**  Version   : -.- (PCB : )                                                **
**  Date      : 2019.08.01                                                  **
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
/****************************************************************************/
/**                                                                        **/
/**                      MODULES USED                                      **/
/**                                                                        **/
/****************************************************************************/
#include "Asw_DtcMng.h"

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

static void DtcMng_UpdateC1203( LdcDtcDef *dtcData,  Diagnostic* pDiag );
static void DtcMng_UpdateC1204( LdcDtcDef *dtcData,  Diagnostic* pDiag );
static void DtcMng_UpdateC1213( LdcDtcDef *dtcData,  Diagnostic* pDiag );
static void DtcMng_UpdateC1214( LdcDtcDef *dtcData,  Diagnostic* pDiag );
static void DtcMng_UpdateC1223( LdcDtcDef *dtcData,  Diagnostic* pDiag );
static void DtcMng_UpdateC1224( LdcDtcDef *dtcData,  Diagnostic* pDiag );
static void DtcMng_UpdateC1233( LdcDtcDef *dtcData,  Diagnostic* pDiag );
static void DtcMng_UpdateC1234( LdcDtcDef *dtcData,  Diagnostic* pDiag );
static void DtcMng_UpdateC1240( LdcDtcDef *dtcData,  Diagnostic_Temp* pDiag );
static void DtcMng_UpdateC1241( LdcDtcDef *dtcData,  Diagnostic_Temp* pDiag );
static void DtcMng_UpdateC124E( LdcDtcDef *dtcData,  Diagnostic_Temp* pDiag );

static void DtcMng_UpdateC1235( LdcDtcDef *dtcData,  Diagnostic_Sensor* pDiag );
static void DtcMng_UpdateC1236( LdcDtcDef *dtcData,  Diagnostic_Sensor* pDiag );
static void DtcMng_UpdateC1245( LdcDtcDef *dtcData,  Diagnostic_Sensor* pDiag );
static void DtcMng_UpdateC1246( LdcDtcDef *dtcData,  Diagnostic_Sensor* pDiag );

static void DtcMng_UpdateC1252( LdcDtcDef *dtcData,  Diagnostic_CAN* pDiag );
static void DtcMng_UpdateC1262( LdcDtcDef *dtcData,  Diagnostic_CAN* pDiag );
static void DtcMng_UpdateC1256( LdcDtcDef *dtcData,  Diagnostic_CAN* pDiag );
static void DtcMng_UpdateC1266( LdcDtcDef *dtcData,  Diagnostic_CAN* pDiag );

static void DtcSetNormalFromError(uint8_t* val);
static void DtcSetErrorFromNormal(uint8_t* val);

/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED VARIABLES                                **/
/**                                                                        **/
/****************************************************************************/

/****************************************************************************/
/**                                                                        **/
/**                      GLOBAL VARIABLES                                  **/
/**                                                                        **/
/****************************************************************************/
#define TIME_STAMP 1U

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
void DtcMng_DtcCodeUpdate( LdcDtcDef* strDtc )
{
	DtcMng_UpdateC1203( strDtc, &Dia_Vin);
	DtcMng_UpdateC1204( strDtc, &Dia_Vin );
	DtcMng_UpdateC1213( strDtc, &Dia_Vout);
	DtcMng_UpdateC1214( strDtc, &Dia_Vout );
	DtcMng_UpdateC1223( strDtc, &Dia_Batt );
	DtcMng_UpdateC1224( strDtc, &Dia_Batt );
	DtcMng_UpdateC1233( strDtc, &Dia_Iout);
	DtcMng_UpdateC1234( strDtc, &Dia_Iout );
	DtcMng_UpdateC1240( strDtc, &Dia_Temp );
	DtcMng_UpdateC1241( strDtc, &Dia_Temp );
	DtcMng_UpdateC124E( strDtc, &Dia_Temp );

	DtcMng_UpdateC1235( strDtc, &Dia_Iout_Sens );
	DtcMng_UpdateC1236( strDtc, &Dia_Iout_Sens );
	DtcMng_UpdateC1245( strDtc, &Dia_Temp_Sens);
	DtcMng_UpdateC1246( strDtc, &Dia_Temp_Sens );

	DtcMng_UpdateC1252( strDtc, &Dia_CAN );
	DtcMng_UpdateC1262( strDtc, &Dia_CAN );
	DtcMng_UpdateC1256( strDtc, &Dia_CAN);
	DtcMng_UpdateC1266( strDtc, &Dia_CAN );
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void DtcMng_UpdateC1223( LdcDtcDef *dtcData, Diagnostic* pDiag )
{

	if( pDiag->Err_Over == uTRUE )
	{
		DtcSetErrorFromNormal( &dtcData->dtcC1223 );
	}
	else
	{
		if( dtcData->dtcC1223 == 0x9U ){
			DtcSetNormalFromError( &dtcData->dtcC1223 );
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
static void DtcMng_UpdateC1224( LdcDtcDef *dtcData, Diagnostic* pDiag )
{
	if( pDiag->Err_Under == uTRUE )
	{
		DtcSetErrorFromNormal( &dtcData->dtcC1224 );
	}
	else
	{
		if( dtcData->dtcC1224 == 0x9U ){
			DtcSetNormalFromError( &dtcData->dtcC1224 );
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
static void DtcMng_UpdateC1203(LdcDtcDef *dtcData,  Diagnostic* pDiag )
{
	if( pDiag->Err_Over == uTRUE )
	{
		DtcSetErrorFromNormal( &dtcData->dtcC1203 );
	}
	else
	{
		if( dtcData->dtcC1203 == 0x9U ){
			DtcSetNormalFromError( &dtcData->dtcC1203 );
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
static void DtcMng_UpdateC1204(LdcDtcDef *dtcData,  Diagnostic* pDiag )
{
	if( pDiag->Err_Under == uTRUE )
	{
		DtcSetErrorFromNormal( &dtcData->dtcC1204 );
	}
	else
	{
		if( dtcData->dtcC1204 == 0x9U ){
			DtcSetNormalFromError( &dtcData->dtcC1204 );
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
static void DtcMng_UpdateC1213(LdcDtcDef *dtcData,  Diagnostic* pDiag )
{
	if( pDiag->Err_Over == uTRUE )
	{
		DtcSetErrorFromNormal( &dtcData->dtcC1213 );
	}
	else
	{
		if( dtcData->dtcC1213 == 0x9U ){
			DtcSetNormalFromError( &dtcData->dtcC1213 );
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
static void DtcMng_UpdateC1214(LdcDtcDef *dtcData,  Diagnostic* pDiag )
{
	if( pDiag->Err_Under == uTRUE )
	{
		DtcSetErrorFromNormal( &dtcData->dtcC1214 );
	}
	else
	{
		if( dtcData->dtcC1214 == 0x9U ){
			DtcSetNormalFromError( &dtcData->dtcC1214 );
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
static void DtcMng_UpdateC1233(LdcDtcDef *dtcData,  Diagnostic* pDiag )
{
	if( pDiag->Err_Shutdown == uTRUE )
	{
		DtcSetErrorFromNormal( &dtcData->dtcC1233 );
	}
	else
	{
		if( dtcData->dtcC1233 == 0x9U ){
			DtcSetNormalFromError( &dtcData->dtcC1233 );
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
static void DtcMng_UpdateC1234(LdcDtcDef *dtcData,  Diagnostic* pDiag )
{
	if( pDiag->Err_Over == uTRUE )
	{
		DtcSetErrorFromNormal( &dtcData->dtcC1234 );
	}
	else
	{
		if( dtcData->dtcC1234 == 0x9U ){
			DtcSetNormalFromError( &dtcData->dtcC1234 );
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
static void DtcMng_UpdateC1240(LdcDtcDef *dtcData,  Diagnostic_Temp* pDiag )
{
	if( pDiag->Err_Over == uTRUE )
	{
		DtcSetErrorFromNormal( &dtcData->dtcC1240 );
	}
	else
	{
		if( dtcData->dtcC1240 == 0x9U ){
			DtcSetNormalFromError( &dtcData->dtcC1240 );
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
static void DtcMng_UpdateC1241(LdcDtcDef *dtcData,  Diagnostic_Temp* pDiag )
{
	if( pDiag->Err_Under == uTRUE )
	{
		DtcSetErrorFromNormal( &dtcData->dtcC1241 );
	}
	else
	{
		if( dtcData->dtcC1241 == 0x9U ){
			DtcSetNormalFromError( &dtcData->dtcC1241 );
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
static void DtcMng_UpdateC124E(LdcDtcDef *dtcData,  Diagnostic_Temp* pDiag )
{
	if( pDiag->Derating == uTRUE )
	{
		DtcSetErrorFromNormal( &dtcData->dtcC124E );
	}
	else
	{
		if( dtcData->dtcC124E == 0x9U ){
			DtcSetNormalFromError( &dtcData->dtcC124E );
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
static void DtcMng_UpdateC1235( LdcDtcDef *dtcData,  Diagnostic_Sensor* pDiag )
{
	if( pDiag->Err_Open == uTRUE )
	{
		DtcSetErrorFromNormal( &dtcData->dtcC1235 );
	}
	else
	{
		if( dtcData->dtcC1235 == 0x9U ){
			DtcSetNormalFromError( &dtcData->dtcC1235 );
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
static void DtcMng_UpdateC1236( LdcDtcDef *dtcData,  Diagnostic_Sensor* pDiag )
{
	if( pDiag->Err_Short == uTRUE )
	{
		DtcSetErrorFromNormal( &dtcData->dtcC1236 );
	}
	else
	{
		if( dtcData->dtcC1236 == 0x9U ){
			DtcSetNormalFromError( &dtcData->dtcC1236 );
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
static void DtcMng_UpdateC1245( LdcDtcDef *dtcData,  Diagnostic_Sensor* pDiag )
{
	if( pDiag->Err_Open == uTRUE )
	{
		DtcSetErrorFromNormal( &dtcData->dtcC1245 );
	}
	else
	{
		if( dtcData->dtcC1245 == 0x9U ){
			DtcSetNormalFromError( &dtcData->dtcC1245 );
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
static void DtcMng_UpdateC1246( LdcDtcDef *dtcData,  Diagnostic_Sensor* pDiag )
{
	if( pDiag->Err_Short == uTRUE )
	{
		DtcSetErrorFromNormal( &dtcData->dtcC1246 );
	}
	else
	{
		if( dtcData->dtcC1246 == 0x9U ){
			DtcSetNormalFromError( &dtcData->dtcC1246 );
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
static void DtcMng_UpdateC1252( LdcDtcDef *dtcData,  Diagnostic_CAN* pDiag )
{
	if( pDiag->CAN1_Busoff == uTRUE )
	{
		DtcSetErrorFromNormal( &dtcData->dtcC1252 );
	}
	else
	{
		if( dtcData->dtcC1252 == 0x9U ){
			DtcSetNormalFromError( &dtcData->dtcC1252 );
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
static void DtcMng_UpdateC1262( LdcDtcDef *dtcData,  Diagnostic_CAN* pDiag )
{
	if( pDiag->CAN2_Busoff == uTRUE )
	{
		DtcSetErrorFromNormal( &dtcData->dtcC1262 );
	}
	else
	{
		if( dtcData->dtcC1262 == 0x9U ){
			DtcSetNormalFromError( &dtcData->dtcC1262 );
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
static void DtcMng_UpdateC1256( LdcDtcDef *dtcData,  Diagnostic_CAN* pDiag )
{
	if( pDiag->CAN1_Timeout == uTRUE )
	{
		DtcSetErrorFromNormal( &dtcData->dtcC1256 );
	}
	else
	{
		if( dtcData->dtcC1256 == 0x9U ){
			DtcSetNormalFromError( &dtcData->dtcC1256 );
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
static void DtcMng_UpdateC1266( LdcDtcDef *dtcData,  Diagnostic_CAN* pDiag )
{
	if( pDiag->CAN2_Timeout == uTRUE )
	{
		DtcSetErrorFromNormal( &dtcData->dtcC1266 );
	}
	else
	{
		if( dtcData->dtcC1266 == 0x9U ){
			DtcSetNormalFromError( &dtcData->dtcC1266 );
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
static void DtcSetNormalFromError(uint8_t* val)
{
	if(*(val) == 0x9){
		*(val) = 0x8;
	}
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
static void DtcSetErrorFromNormal(uint8_t* val)
{
	if(*(val) != 0x9 || *(val) != 0x8){
		*(val) = 0x9;
	}
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
