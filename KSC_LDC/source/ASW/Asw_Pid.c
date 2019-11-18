/*
                             *******************
******************************* C SOURCE FILE ********************************
**                           *******************                            **
**                                                                          **
**  Project   : KSC_LDC                                                     **
**  Filename  : pid_ctrl.c                                                  **
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

#define PID_CTRL_C_SRC

/****************************************************************************/
/**                                                                        **/
/**                      MODULES USED                                      **/
/**                                                                        **/
/****************************************************************************/
#include "Asw_Pid.h"

/****************************************************************************/
/**                                                                        **/
/**                      DEFINITIONS AND MACROS                            **/
/**                                                                        **/
/****************************************************************************/
#define CV			( 0U )
#define CC 			( 0U )
#define CC_CV		( 1U )

/*------------------------------------------------------------------------------
 	PID Macro Definition
------------------------------------------------------------------------------*/
#define PI_MACRO( v )																							\
	/* Compute the error */																					\
  v.err = v.Ref - v.Fbk; 			    																\
	/* proportional term */ 																				\
	v.up = IQmpy( v.Kp, v.err );																	  \
																																	\
	/* integral term */ 																						\
	v.ui = v.ui + IQmpy( v.Ki, v.up ) + IQmpy( v.Kc, v.satErr );	  \
	v.preSatErr = v.up + v.ui;																			\
																																	\
	/* control output */ 																						\
	v.Out = SATURATION( v.preSatErr, v.Umax, v.Umin );							\
	v.satErr = v.Out - v.preSatErr;


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
static int32_t SATURATION ( int32_t s32_in, int32_t s32_Max, int32_t s32_Min );

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
static PI pi_V = PID_DEFAULTS;
static PI pi_I = PID_DEFAULTS;

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
static int32_t SATURATION ( int32_t s32_in, int32_t s32_Max, int32_t s32_Min )
{
	int32_t s32_sat;

	if( s32_in >= s32_Max )
	{
		s32_sat = s32_Max;
	}
	else if ( s32_in <= s32_Min )
	{
		s32_sat = s32_Min;
	}
	else
	{
		s32_sat = s32_in;
	}

	return ( int32_t )( s32_sat );
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
void Asw_PI_Init ( void )
{
# if CV
	pi_V.Kp= IQ13( 0.50 );
	pi_V.Ki= IQ13( 0.022 );
	pi_V.Kc= IQ13( 0.10 );
# endif

# if CC_CV
	pi_V.Kp= IQ13( 0.90 );
	pi_V.Ki= IQ13( 0.5 );
	pi_V.Kc= IQ13( 0.50 );
# endif

	pi_V.err = 0;
	pi_V.up = 0;
	pi_V.ui = 0;
	pi_V.satErr = 0;
	pi_V.preSatErr = 0;
	pi_V.Out = 0;

	pi_I.Kp= IQ13( 0.250 );
	pi_I.Ki= IQ13( 0.011 );
	pi_I.Kc= IQ13( 0.10 );
	pi_I.Umax = IQ17( 0.95 );
	pi_I.Umin = IQ17( 0.001 );

	pi_I.err = 0;
	pi_I.up = 0;
	pi_I.ui = 0;
	pi_I.satErr = 0;
	pi_I.preSatErr = 0;
	pi_I.Out = 0;
}

/*--------------------------------------------------------------------------*/
/* Function  |                                                              */
/* Name      |                                                              */
/*--------------------------------------------------------------------------*/
/* Parameter | Nothing                                                      */
/* Return    | Nothing                                                      */
/*--------------------------------------------------------------------------*/
uint16_t Asw_PI_Controller ( uint32_t u32_IRef, uint32_t u32_VRef, uint32_t u32_Ifbk, uint32_t u32_Vfbk )
{
	int32_t s32_shift = 0;

#if CV
	pi_V.Ref = ( int32_t )( u32_VRef );
	pi_V.Fbk = ( int32_t )( u32_Vfbk );
	pi_V.Umax = IQ17( 0.95 );
	pi_V.Umin = 0;

	PI_MACRO( pi_V );

	s32_shift = ( uint16_t )( ( pi_V.Out * 430U ) >> 17U );
#endif

#if CC
	pi_I.Ref = ( int32_t )( u32_IRef );
	pi_I.Fbk = ( int32_t )( u32_Ifbk );
	PI_MACRO( pi_I );

	s32_shift = ( uint32_t )( ( pi_I.Out * 400U ) >> 17U );
#endif

#if CC_CV
	pi_V.Ref = ( int32_t )( u32_VRef );
	pi_V.Fbk = ( int32_t )( u32_Vfbk );
	pi_V.Umax = ( int32_t )( u32_IRef );
	pi_V.Umin = ( int32_t )( 0 );

	PI_MACRO( pi_V );

	pi_I.Ref = ( int32_t )( pi_V.Out );
	pi_I.Fbk = ( int32_t )( u32_Ifbk );
	PI_MACRO( pi_I );

	s32_shift = ( int32_t )( ( pi_I.Out * ( ETIMER_3_PERIOD / 2 ) ) / 131072 );

#endif

	return ( ( uint16_t )s32_shift );
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
uint32_t Asw_Ramp ( uint32_t u32_setpoint, uint32_t u32_ref )
{
	uint32_t u32_out = 0;
	uint32_t u32_Ref_tmp;

	u32_Ref_tmp = u32_ref;

	if( u32_setpoint >= u32_Ref_tmp )  /*ramp-up*/
	{
		u32_Ref_tmp += ( u32_setpoint / 1000U ); /* 1ms * 1000 = 1sec(Soft-Start time) */

		if( u32_Ref_tmp > u32_setpoint )
		{
			u32_Ref_tmp = u32_setpoint;
		}
	}
	else	/*ramp_down*/
	{
		u32_Ref_tmp -= ( u32_setpoint / 100U );

		if( u32_Ref_tmp < u32_setpoint )
		{
			u32_Ref_tmp = u32_setpoint;
		}
	}

	u32_out = u32_Ref_tmp;

	return ( u32_out );
}

/****************************************************************************/
/**                                                                        **/
/**                      EOF                                               **/
/**                                                                        **/
/****************************************************************************/
