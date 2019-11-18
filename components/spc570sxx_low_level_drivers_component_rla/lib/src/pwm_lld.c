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
 * @file    pwm_lld.c
 * @brief   SPC5xx PWM low level driver code.
 *
 * @addtogroup PWM
 * @{
 */

#include "pwm_lld.h"

#if (LLD_USE_PWM == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   PWMD1 driver identifier.
 */
#if (SPC5_PWM_USE_ETIMER0_CH0 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD1;
#endif

/**
 * @brief   PWMD2 driver identifier.
 */
#if (SPC5_PWM_USE_ETIMER0_CH1 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD2;
#endif

/**
 * @brief   PWMD3 driver identifier.
 */
#if (SPC5_PWM_USE_ETIMER0_CH2 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD3;
#endif

/**
 * @brief   PWMD4 driver identifier.
 */
#if (SPC5_PWM_USE_ETIMER0_CH3 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD4;
#endif

/**
 * @brief   PWMD5 driver identifier.
 */
#if (SPC5_PWM_USE_ETIMER0_CH4 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD5;
#endif

/**
 * @brief   PWMD6 driver identifier.
 */
#if (SPC5_PWM_USE_ETIMER0_CH5 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD6;
#endif

/**
 * @brief   PWMD7 driver identifier.
 */
#if (SPC5_PWM_USE_ETIMER1_CH0 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD7;
#endif

/**
 * @brief   PWMD8 driver identifier.
 */
#if (SPC5_PWM_USE_ETIMER1_CH1 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD8;
#endif

/**
 * @brief   PWMD9 driver identifier.
 */
#if (SPC5_PWM_USE_ETIMER1_CH2 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD9;
#endif

/**
 * @brief   PWMD10 driver identifier.
 */
#if (SPC5_PWM_USE_ETIMER1_CH3 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD10;
#endif

/**
 * @brief   PWMD11 driver identifier.
 */
#if (SPC5_PWM_USE_ETIMER1_CH4 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD11;
#endif

/**
 * @brief   PWMD12 driver identifier.
 */
#if (SPC5_PWM_USE_ETIMER1_CH5 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD12;
#endif

/**
 * @brief   PWMD13 driver identifier.
 */
#if (SPC5_PWM_USE_ETIMER2_CH0 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD13;
#endif

/**
 * @brief   PWMD14 driver identifier.
 */
#if (SPC5_PWM_USE_ETIMER2_CH1 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD14;
#endif

/**
 * @brief   PWMD15 driver identifier.
 */
#if (SPC5_PWM_USE_ETIMER2_CH2 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD15;
#endif

/**
 * @brief   PWMD16 driver identifier.
 */
#if (SPC5_PWM_USE_ETIMER2_CH3 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD16;
#endif

/**
 * @brief   PWMD17 driver identifier.
 */
#if (SPC5_PWM_USE_ETIMER2_CH4 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD17;
#endif

/**
 * @brief   PWMD18 driver identifier.
 */
#if (SPC5_PWM_USE_ETIMER2_CH5 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD18;
#endif

/**
 * @brief   PWMD19 driver identifier.
 */
#if (SPC5_PWM_USE_ETIMER3_CH0 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD19;
#endif

/**
 * @brief   PWMD20 driver identifier.
 */
#if (SPC5_PWM_USE_ETIMER3_CH1 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD20;
#endif

/**
 * @brief   PWMD21 driver identifier.
 */
#if (SPC5_PWM_USE_ETIMER3_CH2 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD21;
#endif

/**
 * @brief   PWMD22 driver identifier.
 */
#if (SPC5_PWM_USE_ETIMER3_CH3 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD22;
#endif

/**
 * @brief   PWMD23 driver identifier.
 */
#if (SPC5_PWM_USE_ETIMER3_CH4 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD23;
#endif

/**
 * @brief   PWMD24 driver identifier.
 */
#if (SPC5_PWM_USE_ETIMER3_CH5 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD24;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Shared IRQ handler.
 *
 * @param[in] pwmp      pointer to the @p PWMDriver object
 */
static void pwm_lld_serve_interrupt(PWMDriver *pwmp) {

  uint16_t intdma;
  uint16_t sr;

  intdma = pwmp->etimerp->CH[pwmp->channel].INTDMA.R;
  sr = pwmp->etimerp->CH[pwmp->channel].STS.R & intdma;

  /* TCF1 (Timer Compare 1 Flag).*/
  if ((sr & 0x0002U) != 0U) {
    /* Reset TCF1 bit.*/
    pwmp->etimerp->CH[pwmp->channel].STS.R = 0x0002U;
    if (pwmp->config->ch_config->channel_callback != NULL) {
      pwmp->config->ch_config->channel_callback(pwmp);
    }
  }
  /* TCF2 (Timer Compare 2 Flag).*/
  else if ((sr & 0x0004U) != 0U) {
    pwmp->etimerp->CH[pwmp->channel].STS.R = 0x0004U;
    /* Reset TCF2 bit.*/
    if (pwmp->config->period_callback != NULL) {
      pwmp->config->period_callback(pwmp);
    }
  } else {
    ; /*close if...else if as per MISRA rules*/
  }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if (SPC5_PWM_USE_ETIMER0_CH0 == TRUE)
#if !defined(SPC5_ETIMER0_STS0_HANDLER)
#error "SPC5_ETIMER0_STS0_HANDLER not defined"
#endif
/**
 * @brief   eTimer0 Channel 0 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER0_STS0_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD1);

  IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_ETIMER0_CH0 */

#if (SPC5_PWM_USE_ETIMER0_CH1 == TRUE)
#if !defined(SPC5_ETIMER0_STS1_HANDLER)
#error "SPC5_ETIMER0_STS1_HANDLER not defined"
#endif
/**
 * @brief   eTimer0 Channel 1 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER0_STS1_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD2);

  IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_ETIMER0_CH1 */

#if (SPC5_PWM_USE_ETIMER0_CH2 == TRUE)
#if !defined(SPC5_ETIMER0_STS2_HANDLER)
#error "SPC5_ETIMER0_STS2_HANDLER not defined"
#endif
/**
 * @brief   eTimer0 Channel 2 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER0_STS2_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD3);

  IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_ETIMER0_CH2 */

#if (SPC5_PWM_USE_ETIMER0_CH3 == TRUE)
#if !defined(SPC5_ETIMER0_STS3_HANDLER)
#error "SPC5_ETIMER0_STS3_HANDLER not defined"
#endif
/**
 * @brief   eTimer0 Channel 3 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER0_STS3_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD4);

  IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_ETIMER0_CH3 */

#if (SPC5_PWM_USE_ETIMER0_CH4 == TRUE)
#if !defined(SPC5_ETIMER0_STS4_HANDLER)
#error "SPC5_ETIMER0_STS4_HANDLER not defined"
#endif
/**
 * @brief   eTimer0 Channel 4 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER0_STS4_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD5);

  IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_ETIMER0_CH4 */

#if (SPC5_PWM_USE_ETIMER0_CH5 == TRUE)
#if !defined(SPC5_ETIMER0_STS5_HANDLER)
#error "SPC5_ETIMER0_STS5_HANDLER not defined"
#endif
/**
 * @brief   eTimer0 Channel 5 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER0_STS5_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD6);

  IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_ETIMER0_CH5 */

#if (SPC5_PWM_USE_ETIMER1_CH0 == TRUE)
#if !defined(SPC5_ETIMER1_STS0_HANDLER)
#error "SPC5_ETIMER1_STS0_HANDLER not defined"
#endif
/**
 * @brief   eTimer1 Channel 0 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER1_STS0_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD7);

  IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_ETIMER1_CH0 */

#if (SPC5_PWM_USE_ETIMER1_CH1 == TRUE)
#if !defined(SPC5_ETIMER1_STS1_HANDLER)
#error "SPC5_ETIMER1_STS1_HANDLER not defined"
#endif
/**
 * @brief   eTimer1 Channel 1 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER1_STS1_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD8);

  IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_ETIMER1_CH1 */

#if (SPC5_PWM_USE_ETIMER1_CH2 == TRUE)
#if !defined(SPC5_ETIMER1_STS2_HANDLER)
#error "SPC5_ETIMER1_STS2_HANDLER not defined"
#endif
/**
 * @brief   eTimer1 Channel 2 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER1_STS2_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD9);

  IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_ETIMER1_CH2 */

#if (SPC5_PWM_USE_ETIMER1_CH3 == TRUE)
#if !defined(SPC5_ETIMER1_STS3_HANDLER)
#error "SPC5_ETIMER1_STS3_HANDLER not defined"
#endif
/**
 * @brief   eTimer1 Channel 3 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER1_STS3_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD10);

  IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_ETIMER1_CH3 */

#if (SPC5_PWM_USE_ETIMER1_CH4 == TRUE)
#if !defined(SPC5_ETIMER1_STS4_HANDLER)
#error "SPC5_ETIMER1_STS4_HANDLER not defined"
#endif
/**
 * @brief   eTimer1 Channel 4 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER1_STS4_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD11);

  IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_ETIMER1_CH4 */

#if (SPC5_PWM_USE_ETIMER1_CH5 == TRUE)
#if !defined(SPC5_ETIMER1_STS5_HANDLER)
#error "SPC5_ETIMER1_STS5_HANDLER not defined"
#endif
/**
 * @brief   eTimer1 Channel 5 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER1_STS5_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD12);

  IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_ETIMER1_CH5 */

#if (SPC5_PWM_USE_ETIMER2_CH0 == TRUE)
#if !defined(SPC5_ETIMER2_STS0_HANDLER)
#error "SPC5_ETIMER2_STS0_HANDLER not defined"
#endif
/**
 * @brief   eTimer2 Channel 0 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER2_STS0_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD13);

  IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_ETIMER2_CH0 */

#if (SPC5_PWM_USE_ETIMER2_CH1 == TRUE)
#if !defined(SPC5_ETIMER2_STS1_HANDLER)
#error "SPC5_ETIMER2_STS1_HANDLER not defined"
#endif
/**
 * @brief   eTimer2 Channel 1 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER2_STS1_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD14);

  IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_ETIMER2_CH1 */

#if (SPC5_PWM_USE_ETIMER2_CH2 == TRUE)
#if !defined(SPC5_ETIMER2_STS2_HANDLER)
#error "SPC5_ETIMER2_STS2_HANDLER not defined"
#endif
/**
 * @brief   eTimer2 Channel 2 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER2_STS2_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD15);

  IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_ETIMER2_CH2 */

#if (SPC5_PWM_USE_ETIMER2_CH3 == TRUE)
#if !defined(SPC5_ETIMER2_STS3_HANDLER)
#error "SPC5_ETIMER2_STS3_HANDLER not defined"
#endif
/**
 * @brief   eTimer2 Channel 3 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER2_STS3_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD16);

  IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_ETIMER2_CH3 */

#if (SPC5_PWM_USE_ETIMER2_CH4 == TRUE)
#if !defined(SPC5_ETIMER2_STS4_HANDLER)
#error "SPC5_ETIMER2_STS4_HANDLER not defined"
#endif
/**
 * @brief   eTimer2 Channel 4 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER2_STS4_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD17);

  IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_ETIMER2_CH4 */

#if (SPC5_PWM_USE_ETIMER2_CH5 == TRUE)
#if !defined(SPC5_ETIMER2_STS5_HANDLER)
#error "SPC5_ETIMER2_STS5_HANDLER not defined"
#endif
/**
 * @brief   eTimer2 Channel 5 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER2_STS5_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD18);

  IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_ETIMER2_CH5 */

#if (SPC5_PWM_USE_ETIMER3_CH0 == TRUE)
#if !defined(SPC5_ETIMER3_STS0_HANDLER)
#error "SPC5_ETIMER3_STS0_HANDLER not defined"
#endif
/**
 * @brief   eTimer3 Channel 0 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER3_STS0_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD19);

  IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_ETIMER3_CH0 */

#if (SPC5_PWM_USE_ETIMER3_CH1 == TRUE)
#if !defined(SPC5_ETIMER3_STS1_HANDLER)
#error "SPC5_ETIMER3_STS1_HANDLER not defined"
#endif
/**
 * @brief   eTimer3 Channel 1 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER3_STS1_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD20);

  IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_ETIMER3_CH1 */

#if (SPC5_PWM_USE_ETIMER3_CH2 == TRUE)
#if !defined(SPC5_ETIMER3_STS2_HANDLER)
#error "SPC5_ETIMER3_STS2_HANDLER not defined"
#endif
/**
 * @brief   eTimer3 Channel 2 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER3_STS2_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD21);

  IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_ETIMER3_CH2 */

#if (SPC5_PWM_USE_ETIMER3_CH3 == TRUE)
#if !defined(SPC5_ETIMER3_STS3_HANDLER)
#error "SPC5_ETIMER3_STS3_HANDLER not defined"
#endif
/**
 * @brief   eTimer3 Channel 3 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER3_STS3_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD22);

  IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_ETIMER3_CH3 */

#if (SPC5_PWM_USE_ETIMER3_CH4 == TRUE)
#if !defined(SPC5_ETIMER3_STS4_HANDLER)
#error "SPC5_ETIMER3_STS4_HANDLER not defined"
#endif
/**
 * @brief   eTimer3 Channel 4 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER3_STS4_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD23);

  IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_ETIMER3_CH4 */

#if (SPC5_PWM_USE_ETIMER3_CH5 == TRUE)
#if !defined(SPC5_ETIMER3_STS5_HANDLER)
#error "SPC5_ETIMER3_STS5_HANDLER not defined"
#endif
/**
 * @brief   eTimer3 Channel 5 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_ETIMER3_STS5_HANDLER) {

  IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD24);

  IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_ETIMER3_CH5 */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level PWM driver initialization.
 *
 * @init
 */
void pwm_lld_init(void) {

  /* eTIMERx data initialization. Initially all channels are not in use.*/
#if (SPC5_HAS_ETIMER0 == TRUE)
  ETIMER0D.etimer_active_channels = 0U;
  ETIMER0D.etimerp = &SPC5_ETIMER_0;
#endif /* SPC5_HAS_ETIMER0 */

#if (SPC5_HAS_ETIMER1 == TRUE)
  ETIMER1D.etimer_active_channels = 0U;
  ETIMER1D.etimerp = &SPC5_ETIMER_1;
#endif /* SPC5_HAS_ETIMER1 */

#if (SPC5_HAS_ETIMER2 == TRUE)
  ETIMER2D.etimer_active_channels = 0U;
  ETIMER2D.etimerp = &SPC5_ETIMER_2;
#endif /* SPC5_HAS_ETIMER2 */

#if (SPC5_HAS_ETIMER3 == TRUE)
  ETIMER3D.etimer_active_channels = 0U;
  ETIMER3D.etimerp = &SPC5_ETIMER_3;
#endif /* SPC5_HAS_ETIMER3*/

#if (SPC5_PWM_USE_ETIMER0_CH0 == TRUE)
  /* Driver initialization.*/
  PWMD1.channel = 0U;
  PWMD1.config = NULL;
  PWMD1.etimerp = &SPC5_ETIMER_0;
#endif /* SPC5_PWM_USE_ETIMER0_CH0 */

#if (SPC5_PWM_USE_ETIMER0_CH1 == TRUE)
  /* Driver initialization.*/
  PWMD2.channel = 1U;
  PWMD2.config = NULL;
  PWMD2.etimerp = &SPC5_ETIMER_0;
#endif /* SPC5_PWM_USE_ETIMER0_CH1 */

#if (SPC5_PWM_USE_ETIMER0_CH2 == TRUE)
  /* Driver initialization.*/
  PWMD3.channel = 2U;
  PWMD3.config = NULL;
  PWMD3.etimerp = &SPC5_ETIMER_0;
#endif /* SPC5_PWM_USE_ETIMER0_CH2 */

#if (SPC5_PWM_USE_ETIMER0_CH3 == TRUE)
  /* Driver initialization.*/
  PWMD4.channel = 3U;
  PWMD4.config = NULL;
  PWMD4.etimerp = &SPC5_ETIMER_0;
#endif /* SPC5_PWM_USE_ETIMER0_CH3 */

#if (SPC5_PWM_USE_ETIMER0_CH4 == TRUE)
  /* Driver initialization.*/
  PWMD5.channel = 4U;
  PWMD5.config = NULL;
  PWMD5.etimerp = &SPC5_ETIMER_0;
#endif /* SPC5_PWM_USE_ETIMER0_CH4 */

#if (SPC5_PWM_USE_ETIMER0_CH5 == TRUE)
  /* Driver initialization.*/
  PWMD6.channel = 5U;
  PWMD6.config = NULL;
  PWMD6.etimerp = &SPC5_ETIMER_0;
#endif /* SPC5_PWM_USE_ETIMER0_CH5 */

#if (SPC5_PWM_USE_ETIMER1_CH0 == TRUE)
  /* Driver initialization.*/
  PWMD7.channel = 0U;
  PWMD7.config = NULL;
  PWMD7.etimerp = &SPC5_ETIMER_1;
#endif /* SPC5_PWM_USE_ETIMER1_CH0 */

#if (SPC5_PWM_USE_ETIMER1_CH1 == TRUE)
  /* Driver initialization.*/
  PWMD8.channel = 1U;
  PWMD8.config = NULL;
  PWMD8.etimerp = &SPC5_ETIMER_1;
#endif /* SPC5_PWM_USE_ETIMER1_CH1 */

#if (SPC5_PWM_USE_ETIMER1_CH2 == TRUE)
  /* Driver initialization.*/
  PWMD9.channel = 2U;
  PWMD9.config = NULL;
  PWMD9.etimerp = &SPC5_ETIMER_1;
#endif /* SPC5_PWM_USE_ETIMER1_CH2 */

#if (SPC5_PWM_USE_ETIMER1_CH3 == TRUE)
  /* Driver initialization.*/
  PWMD10.channel = 3U;
  PWMD10.config = NULL;
  PWMD10.etimerp = &SPC5_ETIMER_1;
#endif /* SPC5_PWM_USE_ETIMER1_CH3 */

#if (SPC5_PWM_USE_ETIMER1_CH4 == TRUE)
  /* Driver initialization.*/
  PWMD11.channel = 4U;
  PWMD11.config = NULL;
  PWMD11.etimerp = &SPC5_ETIMER_1;
#endif /* SPC5_PWM_USE_ETIMER1_CH4 */

#if (SPC5_PWM_USE_ETIMER1_CH5 == TRUE)
  /* Driver initialization.*/
  PWMD12.channel = 5U;
  PWMD12.config = NULL;
  PWMD12.etimerp = &SPC5_ETIMER_1;
#endif /* SPC5_PWM_USE_ETIMER1_CH5 */

#if (SPC5_PWM_USE_ETIMER2_CH0 == TRUE)
  /* Driver initialization.*/
  PWMD13.channel = 0U;
  PWMD13.config = NULL;
  PWMD13.etimerp = &SPC5_ETIMER_2;
#endif /* SPC5_PWM_USE_ETIMER2_CH0 */

#if (SPC5_PWM_USE_ETIMER2_CH1 == TRUE)
  /* Driver initialization.*/
  PWMD14.channel = 1U;
  PWMD14.config = NULL;
  PWMD14.etimerp = &SPC5_ETIMER_2;
#endif /* SPC5_PWM_USE_ETIMER2_CH1 */

#if (SPC5_PWM_USE_ETIMER2_CH2 == TRUE)
  /* Driver initialization.*/
  PWMD15.channel = 2U;
  PWMD15.config = NULL;
  PWMD15.etimerp = &SPC5_ETIMER_2;
#endif /* SPC5_PWM_USE_ETIMER2_CH2 */

#if (SPC5_PWM_USE_ETIMER2_CH3 == TRUE)
  /* Driver initialization.*/
  PWMD16.channel = 3U;
  PWMD16.config = NULL;
  PWMD16.etimerp = &SPC5_ETIMER_2;
#endif /* SPC5_PWM_USE_ETIMER2_CH3 */

#if (SPC5_PWM_USE_ETIMER2_CH4 == TRUE)
  /* Driver initialization.*/
  PWMD17.channel = 4U;
  PWMD17.config = NULL;
  PWMD17.etimerp = &SPC5_ETIMER_2;
#endif /* SPC5_PWM_USE_ETIMER2_CH4 */

#if (SPC5_PWM_USE_ETIMER2_CH5 == TRUE)
  /* Driver initialization.*/
  PWMD18.channel = 5U;
  PWMD18.config = NULL;
  PWMD18.etimerp = &SPC5_ETIMER_2;
#endif /* SPC5_PWM_USE_ETIMER2_CH5 */

#if (SPC5_PWM_USE_ETIMER3_CH0 == TRUE)
  /* Driver initialization.*/
  PWMD19.channel = 0U;
  PWMD19.config = NULL;
  PWMD19.etimerp = &SPC5_ETIMER_3;
#endif /* SPC5_PWM_USE_ETIMER3_CH0 */

#if (SPC5_PWM_USE_ETIMER3_CH1 == TRUE)
  /* Driver initialization.*/
  PWMD20.channel = 1U;
  PWMD20.config = NULL;
  PWMD20.etimerp = &SPC5_ETIMER_3;
#endif /* SPC5_PWM_USE_ETIMER3_CH1 */

#if (SPC5_PWM_USE_ETIMER3_CH2 == TRUE)
  /* Driver initialization.*/
  PWMD21.channel = 2U;
  PWMD21.config = NULL;
  PWMD21.etimerp = &SPC5_ETIMER_3;
#endif /* SPC5_PWM_USE_ETIMER3_CH2 */

#if (SPC5_PWM_USE_ETIMER3_CH3 == TRUE)
  /* Driver initialization.*/
  PWMD22.channel = 3U;
  PWMD22.config = NULL;
  PWMD22.etimerp = &SPC5_ETIMER_3;
#endif /* SPC5_PWM_USE_ETIMER3_CH3 */

#if (SPC5_PWM_USE_ETIMER3_CH4 == TRUE)
  /* Driver initialization.*/
  PWMD23.channel = 4U;
  PWMD23.config = NULL;
  PWMD23.etimerp = &SPC5_ETIMER_3;
#endif /* SPC5_PWM_USE_ETIMER3_CH4 */

#if (SPC5_PWM_USE_ETIMER3_CH5 == TRUE)
  /* Driver initialization.*/
  PWMD24.channel = 5U;
  PWMD24.config = NULL;
  PWMD24.etimerp = &SPC5_ETIMER_3;
#endif /* SPC5_PWM_USE_ETIMER3_CH5 */

#if (SPC5_PWM_USE_ETIMER0_CH0 == TRUE)
  INTC_PSR(SPC5_ETIMER0_STS0_NUMBER) = SPC5_ETIMER0_CH0_PRIORITY;
#endif

#if (SPC5_PWM_USE_ETIMER0_CH1 == TRUE)
  INTC_PSR(SPC5_ETIMER0_STS1_NUMBER) = SPC5_ETIMER0_CH1_PRIORITY;
#endif

#if (SPC5_PWM_USE_ETIMER0_CH2 == TRUE)
  INTC_PSR(SPC5_ETIMER0_STS2_NUMBER) = SPC5_ETIMER0_CH2_PRIORITY;
#endif

#if (SPC5_PWM_USE_ETIMER0_CH3 == TRUE)
  INTC_PSR(SPC5_ETIMER0_STS3_NUMBER) = SPC5_ETIMER0_CH3_PRIORITY;
#endif

#if (SPC5_PWM_USE_ETIMER0_CH4 == TRUE)
  INTC_PSR(SPC5_ETIMER0_STS4_NUMBER) = SPC5_ETIMER0_CH4_PRIORITY;
#endif

#if (SPC5_PWM_USE_ETIMER0_CH5 == TRUE)
  INTC_PSR(SPC5_ETIMER0_STS5_NUMBER) = SPC5_ETIMER0_CH5_PRIORITY;
#endif

#if (SPC5_PWM_USE_ETIMER1_CH0 == TRUE)
  INTC_PSR(SPC5_ETIMER1_STS0_NUMBER) = SPC5_ETIMER1_CH0_PRIORITY;
#endif

#if (SPC5_PWM_USE_ETIMER1_CH1 == TRUE)
  INTC_PSR(SPC5_ETIMER1_STS1_NUMBER) = SPC5_ETIMER1_CH1_PRIORITY;
#endif

#if (SPC5_PWM_USE_ETIMER1_CH2 == TRUE)
  INTC_PSR(SPC5_ETIMER1_STS2_NUMBER) = SPC5_ETIMER1_CH2_PRIORITY;
#endif

#if (SPC5_PWM_USE_ETIMER1_CH3 == TRUE)
  INTC_PSR(SPC5_ETIMER1_STS3_NUMBER) = SPC5_ETIMER1_CH3_PRIORITY;
#endif

#if (SPC5_PWM_USE_ETIMER1_CH4 == TRUE)
  INTC_PSR(SPC5_ETIMER1_STS4_NUMBER) = SPC5_ETIMER1_CH4_PRIORITY;
#endif

#if (SPC5_PWM_USE_ETIMER1_CH5 == TRUE)
  INTC_PSR(SPC5_ETIMER1_STS5_NUMBER) = SPC5_ETIMER1_CH5_PRIORITY;
#endif

#if (SPC5_PWM_USE_ETIMER2_CH0 == TRUE)
  INTC_PSR(SPC5_ETIMER2_STS0_NUMBER) = SPC5_ETIMER2_CH0_PRIORITY;
#endif

#if (SPC5_PWM_USE_ETIMER2_CH1 == TRUE)
  INTC_PSR(SPC5_ETIMER2_STS1_NUMBER) = SPC5_ETIMER2_CH1_PRIORITY;
#endif

#if (SPC5_PWM_USE_ETIMER2_CH2 == TRUE)
  INTC_PSR(SPC5_ETIMER2_STS2_NUMBER) = SPC5_ETIMER2_CH2_PRIORITY;
#endif

#if (SPC5_PWM_USE_ETIMER2_CH3 == TRUE)
  INTC_PSR(SPC5_ETIMER2_STS3_NUMBER) = SPC5_ETIMER2_CH3_PRIORITY;
#endif

#if (SPC5_PWM_USE_ETIMER2_CH4 == TRUE)
  INTC_PSR(SPC5_ETIMER2_STS4_NUMBER) = SPC5_ETIMER2_CH4_PRIORITY;
#endif

#if (SPC5_PWM_USE_ETIMER2_CH5 == TRUE)
  INTC_PSR(SPC5_ETIMER2_STS5_NUMBER) = SPC5_ETIMER2_CH5_PRIORITY;
#endif

#if (SPC5_PWM_USE_ETIMER3_CH0 == TRUE)
  INTC_PSR(SPC5_ETIMER3_STS0_NUMBER) = SPC5_ETIMER3_CH0_PRIORITY;
#endif

#if (SPC5_PWM_USE_ETIMER3_CH1 == TRUE)
  INTC_PSR(SPC5_ETIMER3_STS1_NUMBER) = SPC5_ETIMER3_CH1_PRIORITY;
#endif

#if (SPC5_PWM_USE_ETIMER3_CH2 == TRUE)
  INTC_PSR(SPC5_ETIMER3_STS2_NUMBER) = SPC5_ETIMER3_CH2_PRIORITY;
#endif

#if (SPC5_PWM_USE_ETIMER3_CH3 == TRUE)
  INTC_PSR(SPC5_ETIMER3_STS3_NUMBER) = SPC5_ETIMER3_CH3_PRIORITY;
#endif

#if (SPC5_PWM_USE_ETIMER3_CH4 == TRUE)
  INTC_PSR(SPC5_ETIMER3_STS4_NUMBER) = SPC5_ETIMER3_CH4_PRIORITY;
#endif

#if (SPC5_PWM_USE_ETIMER3_CH5 == TRUE)
  INTC_PSR(SPC5_ETIMER3_STS5_NUMBER) = SPC5_ETIMER3_CH5_PRIORITY;
#endif
}

/**
 * @brief   Configures and activates the PWM peripheral.
 *
 * @param[in] pwmp      pointer to the @p PWMDriver object
 * @param[in] config    pointer to a @p PWMConfig object
 *
 * @api
 */
void pwm_lld_start(PWMDriver *pwmp, const PWMConfig *config) {

  osalEnterCritical();
  
  pwmp->config = config;
  pwmp->period = config->period;
  
  /* Update the number of active PWM channels.*/
#if (SPC5_PWM_USE_ETIMER0_CH0 == TRUE)
  if (&PWMD1 == pwmp) {
    ETIMER0D.etimer_active_channels++;
  }
#endif /* SPC5_PWM_USE_ETIMER0_CH0 */

#if (SPC5_PWM_USE_ETIMER0_CH1 == TRUE)
  if (&PWMD2 == pwmp) {
    ETIMER0D.etimer_active_channels++;
  }
#endif /* SPC5_PWM_USE_ETIMER0_CH1 */

#if (SPC5_PWM_USE_ETIMER0_CH2 == TRUE)
  if (&PWMD3 == pwmp) {
    ETIMER0D.etimer_active_channels++;
  }
#endif /* SPC5_PWM_USE_ETIMER0_CH2 */

#if (SPC5_PWM_USE_ETIMER0_CH3 == TRUE)
  if (&PWMD4 == pwmp) {
    ETIMER0D.etimer_active_channels++;
  }
#endif /* SPC5_PWM_USE_ETIMER0_CH3 */

#if (SPC5_PWM_USE_ETIMER0_CH4 == TRUE)
  if (&PWMD5 == pwmp) {
    ETIMER0D.etimer_active_channels++;
  }
#endif /* SPC5_PWM_USE_ETIMER0_CH4 */

#if (SPC5_PWM_USE_ETIMER0_CH5 == TRUE)
  if (&PWMD6 == pwmp) {
    ETIMER0D.etimer_active_channels++;
  }
#endif /* SPC5_PWM_USE_ETIMER0_CH5 */

#if (SPC5_PWM_USE_ETIMER1_CH0 == TRUE)
  if (&PWMD7 == pwmp) {
    ETIMER1D.etimer_active_channels++;
  }
#endif /* SPC5_PWM_USE_ETIMER1_CH0 */

#if (SPC5_PWM_USE_ETIMER1_CH1 == TRUE)
  if (&PWMD8 == pwmp) {
    ETIMER1D.etimer_active_channels++;
  }
#endif /* SPC5_PWM_USE_ETIMER1_CH1 */

#if (SPC5_PWM_USE_ETIMER1_CH2 == TRUE)
  if (&PWMD9 == pwmp) {
    ETIMER1D.etimer_active_channels++;
  }
#endif /* SPC5_PWM_USE_ETIMER1_CH2 */

#if (SPC5_PWM_USE_ETIMER1_CH3 == TRUE)
  if (&PWMD10 == pwmp) {
    ETIMER1D.etimer_active_channels++;
  }
#endif /* SPC5_PWM_USE_ETIMER1_CH3 */

#if (SPC5_PWM_USE_ETIMER1_CH4 == TRUE)
  if (&PWMD11 == pwmp) {
    ETIMER1D.etimer_active_channels++;
  }
#endif /* SPC5_PWM_USE_ETIMER1_CH4 */

#if (SPC5_PWM_USE_ETIMER1_CH5 == TRUE)
  if (&PWMD12 == pwmp) {
    ETIMER1D.etimer_active_channels++;
  }
#endif /* SPC5_PWM_USE_ETIMER1_CH5 */

#if (SPC5_PWM_USE_ETIMER2_CH0 == TRUE)
  if (&PWMD13 == pwmp) {
    ETIMER2D.etimer_active_channels++;
  }
#endif /* SPC5_PWM_USE_ETIMER2_CH0 */

#if (SPC5_PWM_USE_ETIMER2_CH1 == TRUE)
  if (&PWMD14 == pwmp) {
    ETIMER2D.etimer_active_channels++;
  }
#endif /* SPC5_PWM_USE_ETIMER2_CH1 */

#if (SPC5_PWM_USE_ETIMER2_CH2 == TRUE)
  if (&PWMD15 == pwmp) {
    ETIMER2D.etimer_active_channels++;
  }
#endif /* SPC5_PWM_USE_ETIMER2_CH2 */

#if (SPC5_PWM_USE_ETIMER2_CH3 == TRUE)
  if (&PWMD16 == pwmp) {
    ETIMER2D.etimer_active_channels++;
  }
#endif /* SPC5_PWM_USE_ETIMER2_CH3 */

#if (SPC5_PWM_USE_ETIMER2_CH4 == TRUE)
  if (&PWMD17 == pwmp) {
    ETIMER2D.etimer_active_channels++;
  }
#endif /* SPC5_PWM_USE_ETIMER2_CH4 */

#if (SPC5_PWM_USE_ETIMER2_CH5 == TRUE)
  if (&PWMD18 == pwmp) {
    ETIMER2D.etimer_active_channels++;
  }
#endif /* SPC5_PWM_USE_ETIMER2_CH5 */

#if (SPC5_PWM_USE_ETIMER3_CH0 == TRUE)
  if (&PWMD19 == pwmp) {
    ETIMER3D.etimer_active_channels++;
  }
#endif /* SPC5_PWM_USE_ETIMER3_CH0 */

#if (SPC5_PWM_USE_ETIMER3_CH1 == TRUE)
  if (&PWMD20 == pwmp) {
    ETIMER3D.etimer_active_channels++;
  }
#endif /* SPC5_PWM_USE_ETIMER3_CH1 */

#if (SPC5_PWM_USE_ETIMER3_CH2 == TRUE)
  if (&PWMD21 == pwmp) {
    ETIMER3D.etimer_active_channels++;
  }
#endif /* SPC5_PWM_USE_ETIMER3_CH2 */

#if (SPC5_PWM_USE_ETIMER3_CH3 == TRUE)
  if (&PWMD22 == pwmp) {
    ETIMER3D.etimer_active_channels++;
  }
#endif /* SPC5_PWM_USE_ETIMER3_CH3 */

#if (SPC5_PWM_USE_ETIMER3_CH4 == TRUE)
  if (&PWMD23 == pwmp) {
    ETIMER3D.etimer_active_channels++;
  }
#endif /* SPC5_PWM_USE_ETIMER3_CH4 */

#if (SPC5_PWM_USE_ETIMER3_CH5 == TRUE)
  if (&PWMD24 == pwmp) {
    ETIMER3D.etimer_active_channels++;
  }
#endif /* SPC5_PWM_USE_ETIMER3_CH5 */

  /* Enable ETIMER0 Clock.*/
#if (SPC5_HAS_ETIMER0 == TRUE)
  if (pwmp->etimerp == &SPC5_ETIMER_0) {
    etimer_enable_clock(&ETIMER0D);
  }
#endif /* SPC5_HAS_ETIMER0 */

  /* Enable ETIMER1 Clock.*/
#if (SPC5_HAS_ETIMER1 == TRUE)
  if (pwmp->etimerp == &SPC5_ETIMER_1) {
    etimer_enable_clock(&ETIMER1D);
  }
#endif /* SPC5_HAS_ETIMER1 */

  /* Enable ETIMER2 Clock.*/
#if (SPC5_HAS_ETIMER2 == TRUE)
  if (pwmp->etimerp == &SPC5_ETIMER_2) {
    etimer_enable_clock(&ETIMER2D);
  }
#endif /* SPC5_HAS_ETIMER2 */

  /* Enable ETIMER3 Clock.*/
#if (SPC5_HAS_ETIMER3 == TRUE)
  if (pwmp->etimerp == &SPC5_ETIMER_3) {
    etimer_enable_clock(&ETIMER3D);
  }
#endif /* SPC5_HAS_ETIMER3 */
   
  osalExitCritical();
}

/**
 * @brief   Deactivates the PWM peripheral.
 *
 * @param[in] pwmp      pointer to the @p PWMDriver object
 *
 * @api
 */
void pwm_lld_stop(PWMDriver *pwmp) {

  osalEnterCritical();

  /* Update the number of active PWM channels.*/
#if (SPC5_PWM_USE_ETIMER0_CH0 == TRUE)
  if (&PWMD1 == pwmp) {
    ETIMER0D.etimer_active_channels--;
  }
#endif /* SPC5_PWM_USE_ETIMER0_CH0 */

#if (SPC5_PWM_USE_ETIMER0_CH1 == TRUE)
  if (&PWMD2 == pwmp) {
    ETIMER0D.etimer_active_channels--;
  }
#endif /* SPC5_PWM_USE_ETIMER0_CH1 */

#if (SPC5_PWM_USE_ETIMER0_CH2 == TRUE)
  if (&PWMD3 == pwmp) {
    ETIMER0D.etimer_active_channels--;
  }
#endif /* SPC5_PWM_USE_ETIMER0_CH2 */

#if (SPC5_PWM_USE_ETIMER0_CH3 == TRUE)
  if (&PWMD4 == pwmp) {
    ETIMER0D.etimer_active_channels--;
  }
#endif /* SPC5_PWM_USE_ETIMER0_CH3 */

#if (SPC5_PWM_USE_ETIMER0_CH4 == TRUE)
  if (&PWMD5 == pwmp) {
    ETIMER0D.etimer_active_channels--;
  }
#endif /* SPC5_PWM_USE_ETIMER0_CH4 */

#if (SPC5_PWM_USE_ETIMER0_CH5 == TRUE)
  if (&PWMD6 == pwmp) {
    ETIMER0D.etimer_active_channels--;
  }
#endif /* SPC5_PWM_USE_ETIMER0_CH5 */

#if (SPC5_PWM_USE_ETIMER1_CH0 == TRUE)
  if (&PWMD7 == pwmp) {
    ETIMER1D.etimer_active_channels--;
  }
#endif /* SPC5_PWM_USE_ETIMER1_CH0 */

#if (SPC5_PWM_USE_ETIMER1_CH1 == TRUE)
  if (&PWMD8 == pwmp) {
    ETIMER1D.etimer_active_channels--;
  }
#endif /* SPC5_PWM_USE_ETIMER1_CH1 */

#if (SPC5_PWM_USE_ETIMER1_CH2 == TRUE)
  if (&PWMD9 == pwmp) {
    ETIMER1D.etimer_active_channels--;
  }
#endif /* SPC5_PWM_USE_ETIMER1_CH2 */

#if (SPC5_PWM_USE_ETIMER1_CH3 == TRUE)
  if (&PWMD10 == pwmp) {
    ETIMER1D.etimer_active_channels--;
  }
#endif /* SPC5_PWM_USE_ETIMER1_CH3 */

#if (SPC5_PWM_USE_ETIMER1_CH4 == TRUE)
  if (&PWMD11 == pwmp) {
    ETIMER1D.etimer_active_channels--;
  }
#endif /* SPC5_PWM_USE_ETIMER1_CH4 */

#if (SPC5_PWM_USE_ETIMER1_CH5 == TRUE)
  if (&PWMD12 == pwmp) {
    ETIMER1D.etimer_active_channels--;
  }
#endif /* SPC5_PWM_USE_ETIMER1_CH5 */

#if (SPC5_PWM_USE_ETIMER2_CH0 == TRUE)
  if (&PWMD13 == pwmp) {
    ETIMER2D.etimer_active_channels--;
  }
#endif /* SPC5_PWM_USE_ETIMER2_CH0 */

#if (SPC5_PWM_USE_ETIMER2_CH1 == TRUE)
  if (&PWMD14 == pwmp) {
    ETIMER2D.etimer_active_channels--;
  }
#endif /* SPC5_PWM_USE_ETIMER2_CH1 */

#if (SPC5_PWM_USE_ETIMER2_CH2 == TRUE)
  if (&PWMD15 == pwmp) {
    ETIMER2D.etimer_active_channels--;
  }
#endif /* SPC5_PWM_USE_ETIMER2_CH2 */

#if (SPC5_PWM_USE_ETIMER2_CH3 == TRUE)
  if (&PWMD16 == pwmp) {
    ETIMER2D.etimer_active_channels--;
  }
#endif /* SPC5_PWM_USE_ETIMER2_CH3 */

#if (SPC5_PWM_USE_ETIMER2_CH4 == TRUE)
  if (&PWMD17 == pwmp) {
    ETIMER2D.etimer_active_channels--;
  }
#endif /* SPC5_PWM_USE_ETIMER2_CH4 */

#if (SPC5_PWM_USE_ETIMER2_CH5 == TRUE)
  if (&PWMD18 == pwmp) {
    ETIMER2D.etimer_active_channels--;
  }
#endif /* SPC5_PWM_USE_ETIMER2_CH5 */

#if (SPC5_PWM_USE_ETIMER3_CH0 == TRUE)
  if (&PWMD19 == pwmp) {
    ETIMER3D.etimer_active_channels--;
  }
#endif /* SPC5_PWM_USE_ETIMER3_CH0 */

#if (SPC5_PWM_USE_ETIMER3_CH1 == TRUE)
  if (&PWMD20 == pwmp) {
    ETIMER3D.etimer_active_channels--;
  }
#endif /* SPC5_PWM_USE_ETIMER3_CH1 */

#if (SPC5_PWM_USE_ETIMER3_CH2 == TRUE)
  if (&PWMD21 == pwmp) {
    ETIMER3D.etimer_active_channels--;
  }
#endif /* SPC5_PWM_USE_ETIMER3_CH2 */

#if (SPC5_PWM_USE_ETIMER3_CH3 == TRUE)
  if (&PWMD22 == pwmp) {
    ETIMER3D.etimer_active_channels--;
  }
#endif /* SPC5_PWM_USE_ETIMER3_CH3 */

#if (SPC5_PWM_USE_ETIMER3_CH4 == TRUE)
  if (&PWMD23 == pwmp) {
    ETIMER3D.etimer_active_channels--;
  }
#endif /* SPC5_PWM_USE_ETIMER3_CH4 */

#if (SPC5_PWM_USE_ETIMER3_CH5 == TRUE)
  if (&PWMD24 == pwmp) {
    ETIMER3D.etimer_active_channels--;
  }
#endif /* SPC5_PWM_USE_ETIMER3_CH5 */

  /* Disable ETIMER0 Clock.*/
#if (SPC5_HAS_ETIMER0 == TRUE)
  if (pwmp->etimerp == &SPC5_ETIMER_0) {
    etimer_disable_clock(&ETIMER0D);
  }
#endif /* SPC5_HAS_ETIMER0 */

  /* Disable ETIMER1 Clock.*/
#if (SPC5_HAS_ETIMER1 == TRUE)
  if (pwmp->etimerp == &SPC5_ETIMER_1) {
    etimer_disable_clock(&ETIMER1D);
  }
#endif /* SPC5_HAS_ETIMER1 */

  /* Disable ETIMER2 Clock.*/
#if (SPC5_HAS_ETIMER2 == TRUE)
  if (pwmp->etimerp == &SPC5_ETIMER_2) {
    etimer_disable_clock(&ETIMER2D);
  }
#endif /* SPC5_HAS_ETIMER2 */

  /* Disable ETIMER3 Clock.*/
#if (SPC5_HAS_ETIMER3 == TRUE)
  if (pwmp->etimerp == &SPC5_ETIMER_3) {
    etimer_disable_clock(&ETIMER3D);
  }
#endif /* SPC5_HAS_ETIMER3 */

  osalExitCritical();
}

/**
 * @brief   Changes the period of the PWM peripheral.
 * @details This function changes the period of a PWM channels that has already
 *          been activated using @p pwm_lld_start().
 * @pre     The PWM channel must have been activated using @p pwm_lld_start().
 * @post    The PWM channel period is changed to the new value.
 * @note    The function has effect at the next cycle start.
 * @note    If a period is specified that is shorter than the pulse width
 *          programmed in one of the channels then the behavior is not
 *          guaranteed.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] period    new period in ticks
 *
 * @api
 */
void pwm_lld_change_period(PWMDriver *pwmp, pwmcnt_t period) {

  pwmp->period = period;

#if (SPC5_PWM_USE_ETIMER0_CH0 == TRUE)
  if (&PWMD1 == pwmp) {
    pwmp->etimerp->CH[pwmp->channel].COMP2.R = pwmp->period - pwmp->width;
  }
#endif /* SPC5_PWM_USE_ETIMER0_CH0 */

#if (SPC5_PWM_USE_ETIMER0_CH1 == TRUE)
  if (&PWMD2 == pwmp) {
    pwmp->etimerp->CH[pwmp->channel].COMP2.R = pwmp->period - pwmp->width;
  }
#endif /* SPC5_PWM_USE_ETIMER0_CH1 */

#if (SPC5_PWM_USE_ETIMER0_CH2 == TRUE)
  if (&PWMD3 == pwmp) {
    pwmp->etimerp->CH[pwmp->channel].COMP2.R = pwmp->period - pwmp->width;
  }
#endif /* SPC5_PWM_USE_ETIMER0_CH2 */

#if (SPC5_PWM_USE_ETIMER0_CH3 == TRUE)
  if (&PWMD4 == pwmp) {
    pwmp->etimerp->CH[pwmp->channel].COMP2.R = pwmp->period - pwmp->width;
  }
#endif /* SPC5_PWM_USE_ETIMER0_CH3 */

#if (SPC5_PWM_USE_ETIMER0_CH4 == TRUE)
  if (&PWMD5 == pwmp) {
    pwmp->etimerp->CH[pwmp->channel].COMP2.R = pwmp->period - pwmp->width;
  }
#endif /* SPC5_PWM_USE_ETIMER0_CH4 */

#if (SPC5_PWM_USE_ETIMER0_CH5 == TRUE)
  if (&PWMD6 == pwmp) {
    pwmp->etimerp->CH[pwmp->channel].COMP2.R = pwmp->period - pwmp->width;
  }
#endif /* SPC5_PWM_USE_ETIMER0_CH5 */

#if (SPC5_PWM_USE_ETIMER1_CH0 == TRUE)
  if (&PWMD7 == pwmp) {
    pwmp->etimerp->CH[pwmp->channel].COMP2.R = pwmp->period - pwmp->width;
  }
#endif /* SPC5_PWM_USE_ETIMER1_CH0 */

#if (SPC5_PWM_USE_ETIMER1_CH1 == TRUE)
  if (&PWMD8 == pwmp) {
    pwmp->etimerp->CH[pwmp->channel].COMP2.R = pwmp->period - pwmp->width;
  }
#endif /* SPC5_PWM_USE_ETIMER1_CH1 */

#if (SPC5_PWM_USE_ETIMER1_CH2 == TRUE)
  if (&PWMD9 == pwmp) {
    pwmp->etimerp->CH[pwmp->channel].COMP2.R = pwmp->period - pwmp->width;
  }
#endif /* SPC5_PWM_USE_ETIMER1_CH2 */

#if (SPC5_PWM_USE_ETIMER1_CH3 == TRUE)
  if (&PWMD10 == pwmp) {
    pwmp->etimerp->CH[pwmp->channel].COMP2.R = pwmp->period - pwmp->width;
  }
#endif /* SPC5_PWM_USE_ETIMER1_CH3 */

#if (SPC5_PWM_USE_ETIMER1_CH4 == TRUE)
  if (&PWMD11 == pwmp) {
    pwmp->etimerp->CH[pwmp->channel].COMP2.R = pwmp->period - pwmp->width;
  }
#endif /* SPC5_PWM_USE_ETIMER1_CH4 */

#if (SPC5_PWM_USE_ETIMER1_CH5 == TRUE)
  if (&PWMD12 == pwmp) {
    pwmp->etimerp->CH[pwmp->channel].COMP2.R = pwmp->period - pwmp->width;
  }
#endif /* SPC5_PWM_USE_ETIMER1_CH5 */

#if (SPC5_PWM_USE_ETIMER2_CH0 == TRUE)
  if (&PWMD13 == pwmp) {
    pwmp->etimerp->CH[pwmp->channel].COMP2.R = pwmp->period - pwmp->width;
  }
#endif /* SPC5_PWM_USE_ETIMER2_CH0 */

#if (SPC5_PWM_USE_ETIMER2_CH1 == TRUE)
  if (&PWMD14 == pwmp) {
    pwmp->etimerp->CH[pwmp->channel].COMP2.R = pwmp->period - pwmp->width;
  }
#endif /* SPC5_PWM_USE_ETIMER2_CH1 */

#if (SPC5_PWM_USE_ETIMER2_CH2 == TRUE)
  if (&PWMD15 == pwmp) {
    pwmp->etimerp->CH[pwmp->channel].COMP2.R = pwmp->period - pwmp->width;
  }
#endif /* SPC5_PWM_USE_ETIMER2_CH2 */

#if (SPC5_PWM_USE_ETIMER2_CH3 == TRUE)
  if (&PWMD16 == pwmp) {
    pwmp->etimerp->CH[pwmp->channel].COMP2.R = pwmp->period - pwmp->width;
  }
#endif /* SPC5_PWM_USE_ETIMER2_CH3 */

#if (SPC5_PWM_USE_ETIMER2_CH4 == TRUE)
  if (&PWMD17 == pwmp) {
    pwmp->etimerp->CH[pwmp->channel].COMP2.R = pwmp->period - pwmp->width;
  }
#endif /* SPC5_PWM_USE_ETIMER2_CH4 */

#if (SPC5_PWM_USE_ETIMER2_CH5 == TRUE)
  if (&PWMD18 == pwmp) {
    pwmp->etimerp->CH[pwmp->channel].COMP2.R = pwmp->period - pwmp->width;
  }
#endif /* SPC5_PWM_USE_ETIMER2_CH5 */

#if (SPC5_PWM_USE_ETIMER3_CH0 == TRUE)
  if (&PWMD19 == pwmp) {
    pwmp->etimerp->CH[pwmp->channel].COMP2.R = pwmp->period - pwmp->width;
  }
#endif /* SPC5_PWM_USE_ETIMER3_CH0 */

#if (SPC5_PWM_USE_ETIMER3_CH1 == TRUE)
  if (&PWMD20 == pwmp) {
    pwmp->etimerp->CH[pwmp->channel].COMP2.R = pwmp->period - pwmp->width;
  }
#endif /* SPC5_PWM_USE_ETIMER3_CH1 */

#if (SPC5_PWM_USE_ETIMER3_CH2 == TRUE)
  if (&PWMD21 == pwmp) {
    pwmp->etimerp->CH[pwmp->channel].COMP2.R = pwmp->period - pwmp->width;
  }
#endif /* SPC5_PWM_USE_ETIMER3_CH2 */

#if (SPC5_PWM_USE_ETIMER3_CH3 == TRUE)
  if (&PWMD22 == pwmp) {
    pwmp->etimerp->CH[pwmp->channel].COMP2.R = pwmp->period - pwmp->width;
  }
#endif /* SPC5_PWM_USE_ETIMER3_CH3 */

#if (SPC5_PWM_USE_ETIMER3_CH4 == TRUE)
  if (&PWMD23 == pwmp) {
    pwmp->etimerp->CH[pwmp->channel].COMP2.R = pwmp->period - pwmp->width;
  }
#endif /* SPC5_PWM_USE_ETIMER3_CH4 */

#if (SPC5_PWM_USE_ETIMER3_CH5 == TRUE)
  if (&PWMD24 == pwmp) {
    pwmp->etimerp->CH[pwmp->channel].COMP2.R = pwmp->period - pwmp->width;
  }
#endif /* SPC5_PWM_USE_ETIMER3_CH5 */
}

/**
 * @brief   Enables a PWM channel.
 * @pre     The PWM unit must have been activated using @p pwm_lld_start().
 * @post    The channel is active using the specified configuration.
 * @note    Depending on the hardware implementation this function has
 *          effect starting on the next cycle (recommended implementation)
 *          or immediately (fallback implementation).
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] channel   PWM channel identifier (0...PWM_CHANNELS-1). This
 *                      parameter is not relevant in this implementation
 *                      because each PWM driver has only one channel.
 * @param[in] width     PWM pulse width as clock pulses number
 *
 * @api
 */
void pwm_lld_enable_channel(PWMDriver *pwmp,
                            pwmchannel_t channel,
                            pwmcnt_t width) {

  (void)channel;
  
  osalEnterCritical();

  pwmp->width = width;

  /* Clear pending IRQs (if any).*/
  pwmp->etimerp->CH[pwmp->channel].STS.R = 0xFFFFU;

  /* Enable channel.*/
  pwmp->etimerp->ENBL.R |= (uint16_t)(1UL << (pwmp->channel));

  /* Set count till compare.*/
  pwmp->etimerp->CH[pwmp->channel].CTRL1.B.LENGTH = 1U;

  /* Set continuous count.*/
  pwmp->etimerp->CH[pwmp->channel].CTRL1.B.ONCE = 0U;

  /* Set Output Enable (OFLAG output signal will be driven on the external pin).*/
  pwmp->etimerp->CH[pwmp->channel].CTRL2.B.OEN = 1U;

  /* Set Output Mode (Clear OFLAG output on successful compare (COMP1 or COMP2)).*/
  pwmp->etimerp->CH[pwmp->channel].CTRL2.B.OUTMODE = SPC5_ETIMER_OUTMODE_TOAC;

  /* Set counter prescaler.*/
  pwmp->etimerp->CH[pwmp->channel].CTRL1.B.PRISRC = pwmp->config->psc;

  /* Set PWM polarity.*/
  if (pwmp->config->ch_config->mode == PWM_OUTPUT_ACTIVE_HIGH) {
    pwmp->etimerp->CH[pwmp->channel].CTRL2.B.OPS = 1U;
  } else {
    pwmp->etimerp->CH[pwmp->channel].CTRL2.B.OPS = 0U;
  }

  /* Set PWM period and width.*/
  pwmp->etimerp->CH[pwmp->channel].COMP1.R = width;
  pwmp->etimerp->CH[pwmp->channel].COMP2.R = pwmp->period - width;

  /* Active period and channel interrupts.*/
  if (pwmp->config->ch_config->channel_callback != NULL) {
    pwmp->etimerp->CH[pwmp->channel].INTDMA.B.TCF1IE = 1U;
  }
  if (pwmp->config->period_callback != NULL) {
    pwmp->etimerp->CH[pwmp->channel].INTDMA.B.TCF2IE = 1U;
  }

  /* Enable counter.*/
  pwmp->etimerp->CH[pwmp->channel].CTRL1.B.CNTMODE = SPC5_ETIMER_CNTMODE_RE;

  osalExitCritical();
}

/**
 * @brief   Disables a PWM channel.
 * @pre     The PWM unit must have been activated using @p pwm_lld_start().
 * @post    The channel is disabled and its output line returned to the
 *          idle state.
 * @note    Depending on the hardware implementation this function has
 *          effect starting on the next cycle (recommended implementation)
 *          or immediately (fallback implementation).
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] channel   PWM channel identifier (0...PWM_CHANNELS-1). This
 *                      parameter is not relevant in this implementation
 *                      because each driver has only one channel.
 *
 * @api
 */
void pwm_lld_disable_channel(PWMDriver *pwmp, pwmchannel_t channel) {

  (void)channel;
  
  osalEnterCritical();

  /* Disable counter.*/
  pwmp->etimerp->CH[pwmp->channel].CTRL1.B.CNTMODE = SPC5_ETIMER_CNTMODE_NO_OPERATION;

  /* Clear pending IRQs (if any).*/
  pwmp->etimerp->CH[pwmp->channel].STS.R = 0xFFFFU;

  /* De-active period and channel interrupts.*/
  if (pwmp->config->ch_config->channel_callback != NULL) {
    pwmp->etimerp->CH[pwmp->channel].INTDMA.B.TCF1IE = 0U;
  }
  if (pwmp->config->period_callback != NULL) {
    pwmp->etimerp->CH[pwmp->channel].INTDMA.B.TCF2IE = 0U;
  }

  /* Disable channel.*/
  pwmp->etimerp->ENBL.R &= ~(uint16_t)(1UL << (pwmp->channel));

  osalExitCritical();
}

#endif /* LLD_USE_PWM */

/** @} */
