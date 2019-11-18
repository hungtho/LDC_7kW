/*
                             *******************





******************************* C HEADER FILE ********************************
**                           *******************                            **
**                                                                          **
**  Project   : KSC_LDC                                                     **
**  Filename  : pid_ctrl.h                                                  **
**  Version   : -.- (PCB : )                                                **
**  Date      : 2019.09.16                                                  **
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

#ifndef PID_CTRL_INCLUDED
#define PID_CTRL_INCLUDED

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

typedef struct
{
	int32_t  Ref;   		/* Input: reference set-point*/
	int32_t  Fbk;   	  /* Input: feedback*/
	int32_t  Out;   		/* Output: controller output*/

	int32_t  Kp;				/* Parameter: proportional loop gain*/
	int32_t  Ki;			  /* Parameter: integral gain*/
	int32_t  Kc;		     	/* Parameter: Integral correction gain*/
	int32_t  Umax;			/* Parameter: upper saturation limit*/
	int32_t  Umin;			/* Parameter: lower saturation limit*/

	int32_t  err;				/* Variable: Error*/
	int32_t  satErr;		/* Variable: Saturated difference*/
	int32_t  preSatErr;	/* Variable: Saturated difference*/
	int32_t  up;				/* Data: proportional term*/
	int32_t  ui;				/* Data: integral term*/

}PI;

/*-----------------------------------------------------------------------------
Default initialization values for the PID objects
-----------------------------------------------------------------------------*/
#define PID_DEFAULTS {   \
					 0, 					 \
					 0, 			     \
					 0, 			     \
										     \
				  IQ13( 1.0 ), 	 \
				  IQ13( 1.0 ),	 \
				  IQ13( 1.0 ),	 \
				  IQ17( 1.0 ),	 \
				  IQ17( 0.0 ),   \
										     \
				  IQ17( 0.0 ),	 \
				  IQ17( 0.0 ),   \
				  IQ17( 0.0 ),	 \
				  IQ17( 0.0 ), 	 \
				  IQ17( 0.0 ) 	 \
				}


/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED VARIABLES                                **/
/**                                                                        **/
/****************************************************************************/
extern void Asw_PI_Init ( void );
extern uint16_t Asw_PI_Controller ( uint32_t u32_IRef, uint32_t u32_VRef, uint32_t u32_Ifbk, uint32_t u32_Vfbk );
extern uint32_t Asw_Ramp ( uint32_t u32_setpoint, uint32_t u32_ref );

# ifndef PID_CTRL_C_SRC
# endif

/****************************************************************************/
/**                                                                        **/
/**                      EXPORTED FUNCTIONS                                **/
/**                                                                        **/
/****************************************************************************/
#endif

/****************************************************************************/
/**                                                                        **/
/**                      EOF                                               **/
/**                                                                        **/
/****************************************************************************/
