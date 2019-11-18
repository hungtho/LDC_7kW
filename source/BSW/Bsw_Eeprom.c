/*
                             *******************
******************************* C SOURCE FILE ********************************
**                           *******************                            **
**                                                                          **
**  Project   : LDC_7kW                                                     **
**  Filename  : Bsw_Eeprom.c                                                **
**  Version   : -.- (PCB : )                                                **
**  Date      : 2019.11.15                                                  **
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

#define BSW_EEPROM_C_SRC

/****************************************************************************/
/**                                                                        **/
/**                      MODULES USED                                      **/
/**                                                                        **/
/****************************************************************************/
#include "Bsw_Eeprom.h"

/****************************************************************************/
/**                                                                        **/
/**                      DEFINITIONS AND MACROS                            **/
/**                                                                        **/
/****************************************************************************/
#define NUM_OF_ID     100U       	/* number of ID tested */
#define MAX_ID_SIZE   40U       	/* max size of an ID */

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
static void eep_memcpy(void *des, const void *src, uint32_t num);

/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED VARIABLES                                **/
/**                                                                        **/
/****************************************************************************/

/****************************************************************************/
/**                                                                        **/
/**                      LOCAL VARIABLES                                   **/
/**                                                                        **/
/****************************************************************************/
static uint32_t returnvalue;
static uint32_t size;
static uint8_t updateFlag = 0; /* 0 : don't need to update / 1: need to update */

static uint8_t readBuffer[ MAX_ID_SIZE ];
static uint8_t writeBuffer[ MAX_ID_SIZE ];

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
void EEP_UPDATE_EEPROM(LdcDtcDef* eepData, uint32_t length, uint8_t ldcStat)
{
	uint32_t idx = 0;

	char* pDes = (char*)&writeBuffer;
	char* pSrc = (char*)eepData;

	for(idx = 0; idx < length; idx++){
		if(pDes[idx] != pSrc[idx]){
			if( (pSrc[idx] == 0x9) || (pSrc[idx] == 0x8) ){
				pDes[idx] = pSrc[idx];
				updateFlag = 1;
			}
		}
	}

	if(ldcStat != en_LDC_Run && (updateFlag == 1) )
	{
		returnvalue = eeprom_write(&EEPROMD, (uint16_t) 0, length, (uint32_t) writeBuffer);
		if (returnvalue != EEPROM_OK) {
		}
		updateFlag = 0;
	}
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void EEP_EEPROM_INIT(void)
{
  /* Init eeprom */
  returnvalue = eeprom_start(&EEPROMD, &eeprom_cfg);
  if ((returnvalue != EEPROM_OK) && (returnvalue != EEPROM_FIRST_INIT_OK)) {
  }
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void EEP_EEPROM_READ(void)
{
  /*read*/
  returnvalue = eeprom_read(&EEPROMD, (uint16_t)0, &size, (uint32_t)readBuffer);
  if (returnvalue != EEPROM_OK){
  }

  eep_memcpy(writeBuffer, readBuffer, sizeof(readBuffer));
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void EEP_EEPROM_WRITE(LdcDtcDef* eepData, uint32_t length)
{
	/* write */
	size = length;
	if (eepData != 0) {
		eep_memcpy(&writeBuffer, eepData, size);
	}

	returnvalue = eeprom_write(&EEPROMD, (uint16_t) 0, size, (uint32_t) writeBuffer);
	if (returnvalue != EEPROM_OK) {
	}
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void EEP_DTC_CLEAR(void)
{
	uint32_t idx = 0;
	uint8_t clearBuffer[MAX_ID_SIZE] = {0, };

	uint32_t size = sizeof(LdcDtcDef);

	for(idx = 0; idx < NUM_OF_ID; idx++){
		returnvalue = eeprom_write(&EEPROMD, (uint16_t) idx, size, (uint32_t) clearBuffer);
		if (returnvalue != EEPROM_OK) {
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
uint8_t EEP_CMP_BUFFER(void* desBuffer, void* srcBuffer, uint32_t length)
{
    uint32_t idx = 0;
    uint8_t rtnVal = 1;

    char* pDes = (char*)desBuffer;
    char* pSrc = (char*)srcBuffer;


    for(idx = 0 ; idx < length ; idx++){
    	if(pDes[idx] != pSrc[idx]){
    		rtnVal = 0;
    	}
    }

    return rtnVal;
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
uint8_t* EEP_GET_WRITE_BUFFER(void)
{
	return &writeBuffer[0];
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void utils_memcpy( void *des, const void *src, uint32_t num )
{
	char* pDes = ( char* )des; /* [CS_IGNORE=MISRA_C_2012.MISRA_C_2012_11_05;char, signed char, unsigned char를 가리키는 포인터 간의 변환은 허용된다. 이 타입들이 독립된 byte단위로 접근할수 있게끔 표준이 보장하기 때문이다. (근거 - Rule 11.3)] */
	char* pSrc = ( char* )src; /* [CS_IGNORE=MISRA_C_2012.MISRA_C_2012_11_08;char, signed char, unsigned char를 가리키는 포인터 간의 변환은 허용된다. 이 타입들이 독립된 byte단위로 접근할수 있게끔 표준이 보장하기 때문이다. (근거 - Rule 11.3)] */ /* [CS_IGNORE=MISRA_C_2012.MISRA_C_2012_11_05;char, signed char, unsigned char를 가리키는 포인터 간의 변환은 허용된다. 이 타입들이 독립된 byte단위로 접근할수 있게끔 표준이 보장하기 때문이다. (근거 - Rule 11.3)] */

	uint32_t idx = 0;

	for( idx = 0 ; idx < num ; idx++ ){
		pDes[ idx ] = pSrc[ idx ];
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
static void eep_memcpy(void *des, const void *src, uint32_t num)
{
	uint32_t idx = 0;
	char* pDes = (char*)des;
	char* pSrc = (char*)src;

	for(idx = 0 ; idx < num ; idx++){
		pDes[idx] = pSrc[idx];
	}
}

/****************************************************************************/
/**                                                                        **/
/**                      EOF                                               **/
/**                                                                        **/
/****************************************************************************/
