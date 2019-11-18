/****************************************************************************
*
* Copyright © 2017-2019 STMicroelectronics - All Rights Reserved
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
 * @file    fccu_lld_cfg.h
 * @brief   FCCU Driver configuration macros and structures.
 *
 * @addtogroup FCCU
 * @{
 */

#ifndef _FCCU_LLD_CFG_H_
#define _FCCU_LLD_CFG_H_

#include "spc5_lld.h"
#include "fccu_lld.h"

#if (LLD_USE_FCCU == TRUE) || defined(__DOXYGEN__)

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

/* List of the FCCUChannelConfig structures defined in fccu_lld_cfg.c.*/

/* List of the FCCUConfig structures defined in fccu_lld_cfg.c.*/

#ifdef __cplusplus
extern "C" {
#endif
  /* List of the callback functions referenced from the FCCUChannelConfig
     structures in fccu_lld_cfg.c.*/

  /* List of the callback functions referenced from the FCCUConfig
     structures in fccu_lld_cfg.c.*/
#ifdef __cplusplus
}
#endif

#endif /* LLD_USE_FCCU */

#endif /* _FCCU_LLD_CFG_H_ */

/** @} */
