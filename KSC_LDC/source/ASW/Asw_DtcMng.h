/*
                             *******************
******************************* C SOURCE FILE ********************************
**                           *******************                            **
**                                                                          **
**  Project   : KSC_LDC                                                     **
**  Filename  : Asw_DtcMng.h                                                **
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

#ifndef SOURCE_ASW_ASW_DTCMNG_H_
#define SOURCE_ASW_ASW_DTCMNG_H_

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

/****************************************************************************/
/**                                                                        **/
/**                      TYPEDEFS AND STRUCTURES                           **/
/**                                                                        **/
/****************************************************************************/
typedef struct LdcDiagnosis{
	uint8_t Diag_12VBat; 			/* B+ - 12VBat (6V ~ 16V)*/
	uint8_t Diag_14V_VCC; 			/* 14V VCC (10V ~ 16V)*/
	uint8_t Diag_RegulatorOut5V;	/* 5V Regulator out - 5V_VDD (4.9V ~ 5.1V) */
	uint8_t Diag_IgnKey;			/* IGN Key value (6V ~ 16V) */

	uint8_t Diag_LdcTemp;			/* Temperature (-40 ~ 105)*/
	uint8_t Diag_LdcTempSub;			/* Temperature (-40 ~ 105)*/
	uint8_t Diag_HV_V;				/* HV V - 입력전압	(250V ~ 420V) */
	uint8_t Diag_HV_I;				/* HV I - 입력전류 (0A ~ 20A) */
	uint8_t Diag_LV_V;				/* LV V(Battery)- 출력전압 (6V ~ 16V) */
	uint8_t Diag_LV_I;				/* LV I(Battery)- 출력전류 (0A ~ 128A) */

	uint8_t Diag_LV_PeakCurr;		/* LV Peak Current (ON/OFF) */
	uint8_t Diag_LdcEnable;			/* LDC enable signal(Enable/Disable) */
	uint8_t Diag_InputLitmitCurr;	/* Input limit Current (I) (0A ~ 30A) */
	uint8_t Diag_TargetV;			/* Target V (V) (0V ~ 16V) */
	uint8_t Diag_VcuState;			/* VCU State (NR/R/EM) */
}LdcDiagnosisType;

typedef struct LdcDtc
{
	uint8_t dtcC1223;
	uint8_t dtcC1224;
	uint8_t dtcC1203;
	uint8_t dtcC1204;
	uint8_t dtcC1213;
	uint8_t dtcC1214;
	uint8_t dtcC1233;
	uint8_t dtcC1234;
	uint8_t dtcC1240;
	uint8_t dtcC1241;
	uint8_t dtcC124E;

	uint8_t dtcC1235;
	uint8_t dtcC1236;
	uint8_t dtcC1245;
	uint8_t dtcC1246;

	uint8_t dtcC1252;
	uint8_t dtcC1262;
	uint8_t dtcC1212;
	uint8_t dtcC1256;
	uint8_t dtcC1266;

	uint8_t tcC12314;
	uint8_t tcC1236;
	uint8_t tcC1245;
	uint8_t tcC1246;

	uint8_t tcC1252;
	uint8_t tcC1262;
	uint8_t tcC1212;
	uint8_t tcC1256;
	uint8_t tcC1266;

}LdcDtcDef;

typedef struct ADC_results_real_t
{
    int32_t realInputCurrent;
    int32_t realInputVoltage;
    int32_t realOutputCurrent;
    int32_t realOutputVoltage;
    int16_t realTemperature;
    int16_t realTemperatureSub;
    int16_t realSupplyVoltage;
    int32_t realBatteryVoltage;
    int32_t realKeyInVoltage;
    int32_t realVDDVoltage;
}AdcRealValuesType;

enum MCU_MODE_STATE{
	MCU_RESET = 0
	,MCU_INITIALIZE
	,MCU_READY
};

typedef enum LDC_MODE_STATE {
	LDC_MODE_READY = 0
	,LDC_MODE_NORMAL
	,LDC_MODE_WARNING
	,LDC_MODE_FAULT
	,LDC_MODE_DERATING
	,LDC_MODE_FAILURE
}LDC_MODE_DEF;

typedef enum LDC_RUN_MODE{
	LDC_RUN_STANDBY = 0
	,LDC_RUN_POWERON
}LDC_RUN_DEF;


/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED VARIABLES                                **/
/**                                                                        **/
/****************************************************************************/

# ifndef ADFA_C_SRC
# endif

/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED FUNCTIONS                                **/
/**                                                                        **/
/****************************************************************************/
extern void DtcMng_DtcCodeUpdate(LdcDtcDef* strDtc );

/****************************************************************************/
/**                                                                        **/
/**                      EOF                                               **/
/**                                                                        **/
/****************************************************************************/
#endif /* SOURCE_ASW_ASW_DTCMNG_H_ */
