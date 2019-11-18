/****************************************************************************
*
* Copyright © 2015-2019 STMicroelectronics - All Rights Reserved
*
* License terms: STMicroelectronics Proprietary in accordance with licensing
* terms SLA0089 at www.st.com.
* 
* THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
* INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
* EVALUATION ONLY - NOT FOR USE IN PRODUCTION
*****************************************************************************/
/**
 * @file    pit_lld_cfg.h
 * @brief   PIT Driver configuration macros and strupitres.
 *
 * @addtogroup PIT
 * @{
 */

#ifndef _PIT_LLD_CFG_H_
#define _PIT_LLD_CFG_H_

#include "spc5_lld.h"
#include "pit_lld.h"

#if (LLD_USE_PIT == TRUE) || defined(__DOXYGEN__)

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
/* Driver data strupitres and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* List of the PITConfig strucutres defined in pit_lld_cfg.c.*/
extern PIT_CH_Config pit_config[SPC5_PIT_CHANNELS];

/* To make applications interface fully compatible. */
#define PIT1 PITD

#ifdef __cplusplus
extern "C" {
#endif
/*Callback Prototypes*/
#ifdef __cplusplus
}
#endif

#endif /* LLD_USE_PIT */

#endif /* _PIT_LLD_CFG_H_ */

/** @} */
