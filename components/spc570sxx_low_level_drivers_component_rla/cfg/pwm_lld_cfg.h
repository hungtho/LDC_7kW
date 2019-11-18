/****************************************************************************
*
* Copyright © 2018-2019 STMicroelectronics - All Rights Reserved
*
* License terms: STMicroelectronics Proprietary in accordance with licensing
* terms SLA0089 at www.st.com.
* 
* THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED, 
* INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
* EVALUATION ONLY – NOT FOR USE IN PRODUCTION
*****************************************************************************/
/**
 * @file    pwm_lld_cfg.h
 * @brief   PWM Driver configuration macros and structures.
 *
 * @addtogroup PWM
 * @{
 */

#ifndef _PWM_LLD_CFG_H_
#define _PWM_LLD_CFG_H_

#include "spc5_lld.h"
#include "pwm_lld.h"

#if (LLD_USE_PWM == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* List of the PWMConfig structures defined in pwm_lld_cfg.c.*/

#ifdef __cplusplus
extern "C" {
#endif
  /* List of the callback functions referenced from the PWMConfig
     structures in pwm_lld_cfg.c.*/
#ifdef __cplusplus
}
#endif

#endif /* LLD_USE_PWM */

#endif /* _PWM_LLD_CFG_H_ */

/** @} */
