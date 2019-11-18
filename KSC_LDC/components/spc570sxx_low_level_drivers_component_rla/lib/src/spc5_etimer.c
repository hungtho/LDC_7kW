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
 * @file    spc5_etimer.c
 * @brief   SPC5xx ICU-PWM low level drivers common code.
 *
 * @addtogroup SPC5xx_eTIMER
 * @{
 */

#include "spc5_etimer.h"
#include "pwm_lld.h"
#include "icu_lld.h"

#if (LLD_USE_ICU == TRUE) || (LLD_USE_PWM == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   ETIMER0 data identifier.
 */
#if (SPC5_HAS_ETIMER0 == TRUE) && !defined(__DOXYGEN__)
ETIMERData ETIMER0D;
#endif

/**
 * @brief   ETIMER1 data identifier.
 */
#if (SPC5_HAS_ETIMER1 == TRUE) && !defined(__DOXYGEN__)
ETIMERData ETIMER1D;
#endif

/**
 * @brief   ETIMER2 data identifier.
 */
#if (SPC5_HAS_ETIMER2 == TRUE) && !defined(__DOXYGEN__)
ETIMERData ETIMER2D;
#endif

/**
 * @brief   ETIMER3 data identifier.
 */
#if (SPC5_HAS_ETIMER3 == TRUE) && !defined(__DOXYGEN__)
ETIMERData ETIMER3D;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Enables the eTIMERx clock.
 *
 * @param[in] etimerdp   pointer to the eTIMERx registers block.
 *
 * @notapi
 */
void etimer_enable_clock(ETIMERData *etimerdp) {

  /* If this is the first eTIMERx channel activated, 
     then the eTIMERx module is enabled.*/
  if (etimerdp->etimer_active_channels == 1U) {
#if (SPC5_HAS_ETIMER0 == TRUE)
    if (etimerdp == &ETIMER0D) {
      SPCSetPeripheralClockMode(SPC5_ETIMER0_PCTL,
                                   SPC5_ETIMER0_START_PCTL);
    }
#endif /* SPC5_HAS_ETIMER0 */
#if (SPC5_HAS_ETIMER1 == TRUE)
    if (etimerdp == &ETIMER1D) {
      SPCSetPeripheralClockMode(SPC5_ETIMER1_PCTL,
                                   SPC5_ETIMER1_START_PCTL);
    }
#endif /* SPC5_HAS_ETIMER1 */
#if (SPC5_HAS_ETIMER2 == TRUE)
    if (etimerdp == &ETIMER2D) {
      SPCSetPeripheralClockMode(SPC5_ETIMER2_PCTL,
                                   SPC5_ETIMER2_START_PCTL);
    }
#endif /* SPC5_HAS_ETIMER2 */
#if (SPC5_HAS_ETIMER3 == TRUE)
    if (etimerdp == &ETIMER3D) {
      SPCSetPeripheralClockMode(SPC5_ETIMER3_PCTL,
                                   SPC5_ETIMER3_START_PCTL);
    }
#endif /* SPC5_HAS_ETIMER3 */
  }
}

/**
 * @brief   Disables the eTIMERx clock.
 *
 * @param[in] etimerdp   pointer to the eTIMERx registers block.
 *
 * @notapi
 */
void etimer_disable_clock(ETIMERData *etimerdp) {

  /* If this is the last eTIMERx active channel,
     then the eTIMERx module is disabled.*/
  if (etimerdp->etimer_active_channels == 0U) {
#if (SPC5_HAS_ETIMER0 == TRUE)
    if (etimerdp == &ETIMER0D) {
      SPCSetPeripheralClockMode(SPC5_ETIMER0_PCTL,
                                   SPC5_ETIMER0_STOP_PCTL);
    }
#endif /* SPC5_HAS_ETIMER0 */
#if (SPC5_HAS_ETIMER1 == TRUE)
    if (etimerdp == &ETIMER1D) {
      SPCSetPeripheralClockMode(SPC5_ETIMER1_PCTL,
                                   SPC5_ETIMER1_STOP_PCTL);
    }
#endif /* SPC5_HAS_ETIMER1 */
#if (SPC5_HAS_ETIMER2 == TRUE)
    if (etimerdp == &ETIMER2D) {
      SPCSetPeripheralClockMode(SPC5_ETIMER2_PCTL,
                                   SPC5_ETIMER2_STOP_PCTL);
    }
#endif /* SPC5_HAS_ETIMER2 */
#if (SPC5_HAS_ETIMER3 == TRUE)
    if (etimerdp == &ETIMER3D) {
      SPCSetPeripheralClockMode(SPC5_ETIMER3_PCTL,
                                   SPC5_ETIMER3_STOP_PCTL);
    }
#endif /* SPC5_HAS_ETIMER3 */
  }
}

#endif /* LLD_USE_ICU || LLD_USE_PWM */

/** @} */
