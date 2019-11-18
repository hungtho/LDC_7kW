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
 * @file    icu_lld.c
 * @brief   SPC5xx ICU low level driver code.
 *
 * @addtogroup ICU
 * @{
 */

#include "icu_lld.h"

#if (LLD_USE_ICU == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   ICUD1 driver identifier.
 */
#if (SPC5_ICU_USE_ETIMER0_CH0 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD1;
#endif

/**
 * @brief   ICUD2 driver identifier.
 */
#if (SPC5_ICU_USE_ETIMER0_CH1 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD2;
#endif

/**
 * @brief   ICUD3 driver identifier.
 */
#if (SPC5_ICU_USE_ETIMER0_CH2 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD3;
#endif

/**
 * @brief   ICUD4 driver identifier.
 */
#if (SPC5_ICU_USE_ETIMER0_CH3 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD4;
#endif

/**
 * @brief   ICUD5 driver identifier.
 */
#if (SPC5_ICU_USE_ETIMER0_CH4 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD5;
#endif

/**
 * @brief   ICUD6 driver identifier.
 */
#if (SPC5_ICU_USE_ETIMER0_CH5 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD6;
#endif

/**
 * @brief   ICUD7 driver identifier.
 */
#if (SPC5_ICU_USE_ETIMER1_CH0 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD7;
#endif

/**
 * @brief   ICUD8 driver identifier.
 */
#if (SPC5_ICU_USE_ETIMER1_CH1 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD8;
#endif

/**
 * @brief   ICUD9 driver identifier.
 */
#if (SPC5_ICU_USE_ETIMER1_CH2 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD9;
#endif

/**
 * @brief   ICUD10 driver identifier.
 */
#if (SPC5_ICU_USE_ETIMER1_CH3 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD10;
#endif

/**
 * @brief   ICUD11 driver identifier.
 */
#if (SPC5_ICU_USE_ETIMER1_CH4 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD11;
#endif

/**
 * @brief   ICUD12 driver identifier.
 */
#if (SPC5_ICU_USE_ETIMER1_CH5 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD12;
#endif

/**
 * @brief   ICUD13 driver identifier.
 */
#if (SPC5_ICU_USE_ETIMER2_CH0 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD13;
#endif

/**
 * @brief   ICUD14 driver identifier.
 */
#if (SPC5_ICU_USE_ETIMER2_CH1 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD14;
#endif

/**
 * @brief   ICUD15 driver identifier.
 */
#if (SPC5_ICU_USE_ETIMER2_CH2 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD15;
#endif

/**
 * @brief   ICUD16 driver identifier.
 */
#if (SPC5_ICU_USE_ETIMER2_CH3 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD16;
#endif

/**
 * @brief   ICUD17 driver identifier.
 */
#if (SPC5_ICU_USE_ETIMER2_CH4 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD17;
#endif

/**
 * @brief   ICUD18 driver identifier.
 */
#if (SPC5_ICU_USE_ETIMER2_CH5 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD18;
#endif

/**
 * @brief   ICUD19 driver identifier.
 */
#if (SPC5_ICU_USE_ETIMER3_CH0 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD19;
#endif

/**
 * @brief   ICUD20 driver identifier.
 */
#if (SPC5_ICU_USE_ETIMER3_CH1 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD20;
#endif

/**
 * @brief   ICUD21 driver identifier.
 */
#if (SPC5_ICU_USE_ETIMER3_CH2 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD21;
#endif

/**
 * @brief   ICUD22 driver identifier.
 */
#if (SPC5_ICU_USE_ETIMER3_CH3 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD22;
#endif

/**
 * @brief   ICUD23 driver identifier.
 */
#if (SPC5_ICU_USE_ETIMER3_CH4 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD23;
#endif

/**
 * @brief   ICUD24 driver identifier.
 */
#if (SPC5_ICU_USE_ETIMER3_CH5 == TRUE) || defined(__DOXYGEN__)
ICUDriver ICUD24;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   Width and Period registers.
 */
static uint16_t width;
static uint16_t period;

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Shared IRQ handler.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 */
static void icu_lld_serve_interrupt(ICUDriver *icup) {
  uint16_t intdma;
  uint16_t sr;

  intdma = icup->etimerp->CH[icup->channel].INTDMA.R;
  sr = icup->etimerp->CH[icup->channel].STS.R & intdma;

#if (ICU_SKIP_FIRST_CAPTURE == TRUE)
  /* TOF (Timer Overflow Flag).*/
  if ((sr & 0x0008U) != 0U) {
    /* Reset TOF bit.*/
    icup->etimerp->CH[icup->channel].STS.R = 0x0008U;
    icup->config->overflow_callback(icup);
  }
  /* ICF1 (Input Capture 1 Flag).*/
  if ((sr & 0x0040U) != 0U) {
    if (icup->etimerp->CH[icup->channel].CTRL1.B.CNTMODE ==
        SPC5_ETIMER_CNTMODE_RFE_SIHA) {
      /* Reset ICF1 bit.*/
      icup->etimerp->CH[icup->channel].STS.R = 0x0040U;
      icup->etimerp->CH[icup->channel].CTRL1.B.CNTMODE =
          SPC5_ETIMER_CNTMODE_RE;
    }
    else {
      /* Reset ICF1 bit.*/
      icup->etimerp->CH[icup->channel].STS.R = 0x0040U;
      if (icup->etimerp->CH[icup->channel].CTRL3.B.C1FCNT == 2U) {
        period = icup->etimerp->CH[icup->channel].CAPT1.R;
        period = icup->etimerp->CH[icup->channel].CAPT1.R;
      } else {
        period = icup->etimerp->CH[icup->channel].CAPT1.R;
      }
      icup->config->period_callback(icup);
    }
  }
  /* ICF2 (Input Capture 2 Flag).*/
  else if ((sr & 0x0080U) != 0U) {
    if (icup->etimerp->CH[icup->channel].CTRL1.B.CNTMODE ==
        SPC5_ETIMER_CNTMODE_RFE_SIHA) {
      /* Reset ICF2 bit.*/
      icup->etimerp->CH[icup->channel].STS.R = 0x0080U;
      icup->etimerp->CH[icup->channel].CNTR.R = 0;
    }
    else {
      /* Reset ICF2 bit.*/
      icup->etimerp->CH[icup->channel].STS.R = 0x0080U;
      if (icup->etimerp->CH[icup->channel].CTRL3.B.C2FCNT == 2U) {
        width = icup->etimerp->CH[icup->channel].CAPT2.R;
        width = icup->etimerp->CH[icup->channel].CAPT2.R;
      } else {
        width = icup->etimerp->CH[icup->channel].CAPT2.R;
      }
      icup->config->width_callback(icup);
    }
  } else {
    ; /*close if...else if as per MISRA rules*/
  }
#else
  /* TOF (Timer Overflow Flag).*/
  if ((sr & 0x0008U) != 0U) {
    /* Reset TOF bit.*/
    icup->etimerp->CH[icup->channel].STS.R = 0x0008U;
    icup->config->overflow_callback(icup);
  }
  /* ICF1 (Input Capture 1 Flag).*/
  if ((sr & 0x0040U) != 0U) {
    /* Reset ICF1 bit.*/
    icup->etimerp->CH[icup->channel].STS.R = 0x0040U;
    if (icup->etimerp->CH[icup->channel].CTRL3.B.C1FCNT == 2U) {
      period = icup->etimerp->CH[icup->channel].CAPT1.R;
      period = icup->etimerp->CH[icup->channel].CAPT1.R;
    } else {
      period = icup->etimerp->CH[icup->channel].CAPT1.R;
    }
    icup->config->period_callback(icup);
  }
  /* ICF2 (Input Capture 2 Flag).*/
  else if ((sr & 0x0080U) != 0U) {
    /* Reset ICF2 bit.*/
    icup->etimerp->CH[icup->channel].STS.R = 0x0080U;
    if (icup->etimerp->CH[icup->channel].CTRL3.B.C2FCNT == 2U) {
      width = icup->etimerp->CH[icup->channel].CAPT2.R;
      width = icup->etimerp->CH[icup->channel].CAPT2.R;
    } else {
      width = icup->etimerp->CH[icup->channel].CAPT2.R;
    }
    icup->config->width_callback(icup);
  } else {
    ; /*close if...else if as per MISRA rules*/
  }
#endif /* ICU_SKIP_FIRST_CAPTURE */
}

/**
 * @brief   eTimer channel initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] icup      pointer to a @p ICUDriver object
 */
static void spc5_icu_channel_init(ICUDriver *icup) {

  /* Set primary source and clock prescaler.*/
  icup->etimerp->CH[icup->channel].CTRL1.B.PRISRC = icup->config->psc;

  /* Set control registers.*/
  icup->etimerp->CH[icup->channel].CTRL1.B.ONCE = 0U;
  icup->etimerp->CH[icup->channel].CTRL1.B.LENGTH = 0U;
  icup->etimerp->CH[icup->channel].CTRL1.B.DIR = 0U;
  icup->etimerp->CH[icup->channel].CTRL2.B.PIPS = 0U;

  /* Set secondary source.*/
  icup->etimerp->CH[icup->channel].CTRL1.B.SECSRC = icup->channel;

  /* Set secondary source polarity.*/
  if (icup->config->mode == ICU_INPUT_ACTIVE_HIGH) {
    icup->etimerp->CH[icup->channel].CTRL2.B.SIPS = 0U;
  }
  else {
    icup->etimerp->CH[icup->channel].CTRL2.B.SIPS = 1U;
  }

  /* Direct pointers to the capture registers in order to make reading
     data faster from within callbacks.*/
  icup->pccrp = &period;
  icup->wccrp = &width;

  /* Enable channel.*/
  icup->etimerp->ENBL.R |= (uint16_t)(1UL << (icup->channel));
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if (SPC5_ICU_USE_ETIMER0_CH0 == TRUE)
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

  icu_lld_serve_interrupt(&ICUD1);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_ETIMER0_CH0 */

#if (SPC5_ICU_USE_ETIMER0_CH1 == TRUE)
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

  icu_lld_serve_interrupt(&ICUD2);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_ETIMER0_CH1 */

#if (SPC5_ICU_USE_ETIMER0_CH2 == TRUE)
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

  icu_lld_serve_interrupt(&ICUD3);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_ETIMER0_CH2 */

#if (SPC5_ICU_USE_ETIMER0_CH3 == TRUE)
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

  icu_lld_serve_interrupt(&ICUD4);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_ETIMER0_CH3 */

#if (SPC5_ICU_USE_ETIMER0_CH4 == TRUE)
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

  icu_lld_serve_interrupt(&ICUD5);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_ETIMER0_CH4 */

#if (SPC5_ICU_USE_ETIMER0_CH5 == TRUE)
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

  icu_lld_serve_interrupt(&ICUD6);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_ETIMER0_CH5 */

#if (SPC5_ICU_USE_ETIMER1_CH0 == TRUE)
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

  icu_lld_serve_interrupt(&ICUD7);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_ETIMER1_CH0 */

#if (SPC5_ICU_USE_ETIMER1_CH1 == TRUE)
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

  icu_lld_serve_interrupt(&ICUD8);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_ETIMER1_CH1 */

#if (SPC5_ICU_USE_ETIMER1_CH2 == TRUE)
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

  icu_lld_serve_interrupt(&ICUD9);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_ETIMER1_CH2 */

#if (SPC5_ICU_USE_ETIMER1_CH3 == TRUE)
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

  icu_lld_serve_interrupt(&ICUD10);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_ETIMER1_CH3 */

#if (SPC5_ICU_USE_ETIMER1_CH4 == TRUE)
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

  icu_lld_serve_interrupt(&ICUD11);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_ETIMER1_CH4 */

#if (SPC5_ICU_USE_ETIMER1_CH5 == TRUE)
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

  icu_lld_serve_interrupt(&ICUD12);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_ETIMER1_CH5 */

#if (SPC5_ICU_USE_ETIMER2_CH0 == TRUE)
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

  icu_lld_serve_interrupt(&ICUD13);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_ETIMER2_CH0 */

#if (SPC5_ICU_USE_ETIMER2_CH1 == TRUE)
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

  icu_lld_serve_interrupt(&ICUD14);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_ETIMER2_CH1 */

#if (SPC5_ICU_USE_ETIMER2_CH2 == TRUE)
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

  icu_lld_serve_interrupt(&ICUD15);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_ETIMER2_CH2 */

#if (SPC5_ICU_USE_ETIMER2_CH3 == TRUE)
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

  icu_lld_serve_interrupt(&ICUD16);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_ETIMER2_CH3 */

#if (SPC5_ICU_USE_ETIMER2_CH4 == TRUE)
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

  icu_lld_serve_interrupt(&ICUD17);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_ETIMER2_CH4 */

#if (SPC5_ICU_USE_ETIMER2_CH5 == TRUE)
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

  icu_lld_serve_interrupt(&ICUD18);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_ETIMER2_CH5 */

#if (SPC5_ICU_USE_ETIMER3_CH0 == TRUE)
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

  icu_lld_serve_interrupt(&ICUD19);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_ETIMER3_CH0 */

#if (SPC5_ICU_USE_ETIMER3_CH1 == TRUE)
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

  icu_lld_serve_interrupt(&ICUD20);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_ETIMER3_CH1 */

#if (SPC5_ICU_USE_ETIMER3_CH2 == TRUE)
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

  icu_lld_serve_interrupt(&ICUD21);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_ETIMER3_CH2 */

#if (SPC5_ICU_USE_ETIMER3_CH3 == TRUE)
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

  icu_lld_serve_interrupt(&ICUD22);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_ETIMER3_CH3 */

#if (SPC5_ICU_USE_ETIMER3_CH4 == TRUE)
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

  icu_lld_serve_interrupt(&ICUD23);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_ETIMER3_CH4 */

#if (SPC5_ICU_USE_ETIMER3_CH5 == TRUE)
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

  icu_lld_serve_interrupt(&ICUD24);

  IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_ETIMER3_CH5 */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level ICU driver initialization.
 *
 * @init
 */
void icu_lld_init(void) {

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

  /* Reset width and period registers.*/
  width = 0U;
  period = 0U;

#if (SPC5_ICU_USE_ETIMER0_CH0 == TRUE)
  /* Driver initialization.*/
  ICUD1.channel = 0U;
  ICUD1.config = NULL;
  ICUD1.etimerp = &SPC5_ETIMER_0;
#endif /* SPC5_ICU_USE_ETIMER0_CH0 */

#if (SPC5_ICU_USE_ETIMER0_CH1 == TRUE)
  /* Driver initialization.*/
  ICUD2.channel = 1U;
  ICUD2.config = NULL;
  ICUD2.etimerp = &SPC5_ETIMER_0;
#endif /* SPC5_ICU_USE_ETIMER0_CH1 */

#if (SPC5_ICU_USE_ETIMER0_CH2 == TRUE)
  /* Driver initialization.*/
  ICUD3.channel = 2U;
  ICUD3.config = NULL;
  ICUD3.etimerp = &SPC5_ETIMER_0;
#endif /* SPC5_ICU_USE_ETIMER0_CH2 */

#if (SPC5_ICU_USE_ETIMER0_CH3 == TRUE)
  /* Driver initialization.*/
  ICUD4.channel = 3U;
  ICUD4.config = NULL;
  ICUD4.etimerp = &SPC5_ETIMER_0;
#endif /* SPC5_ICU_USE_ETIMER0_CH3 */

#if (SPC5_ICU_USE_ETIMER0_CH4 == TRUE)
  /* Driver initialization.*/
  ICUD5.channel = 4U;
  ICUD5.config = NULL;
  ICUD5.etimerp = &SPC5_ETIMER_0;
#endif /* SPC5_ICU_USE_ETIMER0_CH4 */

#if (SPC5_ICU_USE_ETIMER0_CH5 == TRUE)
  /* Driver initialization.*/
  ICUD6.channel = 5U;
  ICUD6.config = NULL;
  ICUD6.etimerp = &SPC5_ETIMER_0;
#endif /* SPC5_ICU_USE_ETIMER0_CH5 */

#if (SPC5_ICU_USE_ETIMER1_CH0 == TRUE)
  /* Driver initialization.*/
  ICUD7.channel = 0U;
  ICUD7.config = NULL;
  ICUD7.etimerp = &SPC5_ETIMER_1;
#endif /* SPC5_ICU_USE_ETIMER1_CH0 */

#if (SPC5_ICU_USE_ETIMER1_CH1 == TRUE)
  /* Driver initialization.*/
  ICUD8.channel = 1U;
  ICUD8.config = NULL;
  ICUD8.etimerp = &SPC5_ETIMER_1;
#endif /* SPC5_ICU_USE_ETIMER1_CH1 */

#if (SPC5_ICU_USE_ETIMER1_CH2 == TRUE)
  /* Driver initialization.*/
  ICUD9.channel = 2U;
  ICUD9.config = NULL;
  ICUD9.etimerp = &SPC5_ETIMER_1;
#endif /* SPC5_ICU_USE_ETIMER1_CH2 */

#if (SPC5_ICU_USE_ETIMER1_CH3 == TRUE)
  /* Driver initialization.*/
  ICUD10.channel = 3U;
  ICUD10.config = NULL;
  ICUD10.etimerp = &SPC5_ETIMER_1;
#endif /* SPC5_ICU_USE_ETIMER1_CH3 */

#if (SPC5_ICU_USE_ETIMER1_CH4 == TRUE)
  /* Driver initialization.*/
  ICUD11.channel = 4U;
  ICUD11.config = NULL;
  ICUD11.etimerp = &SPC5_ETIMER_1;
#endif /* SPC5_ICU_USE_ETIMER1_CH4 */

#if (SPC5_ICU_USE_ETIMER1_CH5 == TRUE)
  /* Driver initialization.*/
  ICUD12.channel = 5U;
  ICUD12.config = NULL;
  ICUD12.etimerp = &SPC5_ETIMER_1;
#endif /* SPC5_ICU_USE_ETIMER1_CH5 */

#if (SPC5_ICU_USE_ETIMER2_CH0 == TRUE)
  /* Driver initialization.*/
  ICUD13.channel = 0U;
  ICUD13.config = NULL;
  ICUD13.etimerp = &SPC5_ETIMER_2;
#endif /* SPC5_ICU_USE_ETIMER2_CH0 */

#if (SPC5_ICU_USE_ETIMER2_CH1 == TRUE)
  /* Driver initialization.*/
  ICUD14.channel = 1U;
  ICUD14.config = NULL;
  ICUD14.etimerp = &SPC5_ETIMER_2;
#endif /* SPC5_ICU_USE_ETIMER2_CH1 */

#if (SPC5_ICU_USE_ETIMER2_CH2 == TRUE)
  /* Driver initialization.*/
  ICUD15.channel = 2U;
  ICUD15.config = NULL;
  ICUD15.etimerp = &SPC5_ETIMER_2;
#endif /* SPC5_ICU_USE_ETIMER2_CH2 */

#if (SPC5_ICU_USE_ETIMER2_CH3 == TRUE)
  /* Driver initialization.*/
  ICUD16.channel = 3U;
  ICUD16.config = NULL;
  ICUD16.etimerp = &SPC5_ETIMER_2;
#endif /* SPC5_ICU_USE_ETIMER2_CH3 */

#if (SPC5_ICU_USE_ETIMER2_CH4 == TRUE)
  /* Driver initialization.*/
  ICUD17.channel = 4U;
  ICUD17.config = NULL;
  ICUD17.etimerp = &SPC5_ETIMER_2;
#endif /* SPC5_ICU_USE_ETIMER2_CH4 */

#if (SPC5_ICU_USE_ETIMER2_CH5 == TRUE)
  /* Driver initialization.*/
  ICUD18.channel = 5U;
  ICUD18.config = NULL;
  ICUD18.etimerp = &SPC5_ETIMER_2;
#endif /* SPC5_ICU_USE_ETIMER2_CH5 */

#if (SPC5_ICU_USE_ETIMER3_CH0 == TRUE)
  /* Driver initialization.*/
  ICUD19.channel = 0U;
  ICUD19.config = NULL;
  ICUD19.etimerp = &SPC5_ETIMER_3;
#endif /* SPC5_ICU_USE_ETIMER3_CH0 */

#if (SPC5_ICU_USE_ETIMER3_CH1 == TRUE)
  /* Driver initialization.*/
  ICUD20.channel = 1U;
  ICUD20.config = NULL;
  ICUD20.etimerp = &SPC5_ETIMER_3;
#endif /* SPC5_ICU_USE_ETIMER3_CH1 */

#if (SPC5_ICU_USE_ETIMER3_CH2 == TRUE)
  /* Driver initialization.*/
  ICUD21.channel = 2U;
  ICUD21.config = NULL;
  ICUD21.etimerp = &SPC5_ETIMER_3;
#endif /* SPC5_ICU_USE_ETIMER3_CH2 */

#if (SPC5_ICU_USE_ETIMER3_CH3 == TRUE)
  /* Driver initialization.*/
  ICUD22.channel = 3U;
  ICUD22.config = NULL;
  ICUD22.etimerp = &SPC5_ETIMER_3;
#endif /* SPC5_ICU_USE_ETIMER3_CH3 */

#if (SPC5_ICU_USE_ETIMER3_CH4 == TRUE)
  /* Driver initialization.*/
  ICUD23.channel = 4U;
  ICUD23.config = NULL;
  ICUD23.etimerp = &SPC5_ETIMER_3;
#endif /* SPC5_ICU_USE_ETIMER3_CH4 */

#if (SPC5_ICU_USE_ETIMER3_CH5 == TRUE)
  /* Driver initialization.*/
  ICUD24.channel = 5U;
  ICUD24.config = NULL;
  ICUD24.etimerp = &SPC5_ETIMER_3;
#endif /* SPC5_ICU_USE_ETIMER3_CH5 */

#if (SPC5_ICU_USE_ETIMER0_CH0 == TRUE)
  INTC_PSR(SPC5_ETIMER0_STS0_NUMBER) = SPC5_ETIMER0_CH0_PRIORITY;
#endif

#if (SPC5_ICU_USE_ETIMER0_CH1 == TRUE)
  INTC_PSR(SPC5_ETIMER0_STS1_NUMBER) = SPC5_ETIMER0_CH1_PRIORITY;
#endif

#if (SPC5_ICU_USE_ETIMER0_CH2 == TRUE)
  INTC_PSR(SPC5_ETIMER0_STS2_NUMBER) = SPC5_ETIMER0_CH2_PRIORITY;
#endif

#if (SPC5_ICU_USE_ETIMER0_CH3 == TRUE)
  INTC_PSR(SPC5_ETIMER0_STS3_NUMBER) = SPC5_ETIMER0_CH3_PRIORITY;
#endif

#if (SPC5_ICU_USE_ETIMER0_CH4 == TRUE)
  INTC_PSR(SPC5_ETIMER0_STS4_NUMBER) = SPC5_ETIMER0_CH4_PRIORITY;
#endif

#if (SPC5_ICU_USE_ETIMER0_CH5 == TRUE)
  INTC_PSR(SPC5_ETIMER0_STS5_NUMBER) = SPC5_ETIMER0_CH5_PRIORITY;
#endif

#if (SPC5_ICU_USE_ETIMER1_CH0 == TRUE)
  INTC_PSR(SPC5_ETIMER1_STS0_NUMBER) = SPC5_ETIMER1_CH0_PRIORITY;
#endif

#if (SPC5_ICU_USE_ETIMER1_CH1 == TRUE)
  INTC_PSR(SPC5_ETIMER1_STS1_NUMBER) = SPC5_ETIMER1_CH1_PRIORITY;
#endif

#if (SPC5_ICU_USE_ETIMER1_CH2 == TRUE)
  INTC_PSR(SPC5_ETIMER1_STS2_NUMBER) = SPC5_ETIMER1_CH2_PRIORITY;
#endif

#if (SPC5_ICU_USE_ETIMER1_CH3 == TRUE)
  INTC_PSR(SPC5_ETIMER1_STS3_NUMBER) = SPC5_ETIMER1_CH3_PRIORITY;
#endif

#if (SPC5_ICU_USE_ETIMER1_CH4 == TRUE)
  INTC_PSR(SPC5_ETIMER1_STS4_NUMBER) = SPC5_ETIMER1_CH4_PRIORITY;
#endif

#if (SPC5_ICU_USE_ETIMER1_CH5 == TRUE)
  INTC_PSR(SPC5_ETIMER1_STS5_NUMBER) = SPC5_ETIMER1_CH5_PRIORITY;
#endif

#if (SPC5_ICU_USE_ETIMER2_CH0 == TRUE)
  INTC_PSR(SPC5_ETIMER2_STS0_NUMBER) = SPC5_ETIMER2_CH0_PRIORITY;
#endif

#if (SPC5_ICU_USE_ETIMER2_CH1 == TRUE)
  INTC_PSR(SPC5_ETIMER2_STS1_NUMBER) = SPC5_ETIMER2_CH1_PRIORITY;
#endif

#if (SPC5_ICU_USE_ETIMER2_CH2 == TRUE)
  INTC_PSR(SPC5_ETIMER2_STS2_NUMBER) = SPC5_ETIMER2_CH2_PRIORITY;
#endif

#if (SPC5_ICU_USE_ETIMER2_CH3 == TRUE)
  INTC_PSR(SPC5_ETIMER2_STS3_NUMBER) = SPC5_ETIMER2_CH3_PRIORITY;
#endif

#if (SPC5_ICU_USE_ETIMER2_CH4 == TRUE)
  INTC_PSR(SPC5_ETIMER2_STS4_NUMBER) = SPC5_ETIMER2_CH4_PRIORITY;
#endif

#if (SPC5_ICU_USE_ETIMER2_CH5 == TRUE)
  INTC_PSR(SPC5_ETIMER2_STS5_NUMBER) = SPC5_ETIMER2_CH5_PRIORITY;
#endif

#if (SPC5_ICU_USE_ETIMER3_CH0 == TRUE)
  INTC_PSR(SPC5_ETIMER3_STS0_NUMBER) = SPC5_ETIMER3_CH0_PRIORITY;
#endif

#if (SPC5_ICU_USE_ETIMER3_CH1 == TRUE)
  INTC_PSR(SPC5_ETIMER3_STS1_NUMBER) = SPC5_ETIMER3_CH1_PRIORITY;
#endif

#if (SPC5_ICU_USE_ETIMER3_CH2 == TRUE)
  INTC_PSR(SPC5_ETIMER3_STS2_NUMBER) = SPC5_ETIMER3_CH2_PRIORITY;
#endif

#if (SPC5_ICU_USE_ETIMER3_CH3 == TRUE)
  INTC_PSR(SPC5_ETIMER3_STS3_NUMBER) = SPC5_ETIMER3_CH3_PRIORITY;
#endif

#if (SPC5_ICU_USE_ETIMER3_CH4 == TRUE)
  INTC_PSR(SPC5_ETIMER3_STS4_NUMBER) = SPC5_ETIMER3_CH4_PRIORITY;
#endif

#if (SPC5_ICU_USE_ETIMER3_CH5 == TRUE)
  INTC_PSR(SPC5_ETIMER3_STS5_NUMBER) = SPC5_ETIMER3_CH5_PRIORITY;
#endif
}

/**
 * @brief   Configures and activates the ICU peripheral.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 * @param[in] config    pointer to the @p ICUConfig object
 *
 * @api
 */
void icu_lld_start(ICUDriver *icup, const ICUConfig *config) {

  osalEnterCritical();
  
  icup->config = config;

  /* Update the number of active ICU channels.*/
#if (SPC5_ICU_USE_ETIMER0_CH0 == TRUE)
  if (&ICUD1 == icup) {
    ETIMER0D.etimer_active_channels++;
  }
#endif /* SPC5_ICU_USE_ETIMER0_CH0 */

#if (SPC5_ICU_USE_ETIMER0_CH1 == TRUE)
  if (&ICUD2 == icup) {
    ETIMER0D.etimer_active_channels++;
  }
#endif /* SPC5_ICU_USE_ETIMER0_CH1 */

#if (SPC5_ICU_USE_ETIMER0_CH2 == TRUE)
  if (&ICUD3 == icup) {
    ETIMER0D.etimer_active_channels++;
  }
#endif /* SPC5_ICU_USE_ETIMER0_CH2 */

#if (SPC5_ICU_USE_ETIMER0_CH3 == TRUE)
  if (&ICUD4 == icup) {
    ETIMER0D.etimer_active_channels++;
  }
#endif /* SPC5_ICU_USE_ETIMER0_CH3 */

#if (SPC5_ICU_USE_ETIMER0_CH4 == TRUE)
  if (&ICUD5 == icup) {
    ETIMER0D.etimer_active_channels++;
  }
#endif /* SPC5_ICU_USE_ETIMER0_CH4 */

#if (SPC5_ICU_USE_ETIMER0_CH5 == TRUE)
  if (&ICUD6 == icup) {
    ETIMER0D.etimer_active_channels++;
  }
#endif /* SPC5_ICU_USE_ETIMER0_CH5 */

#if (SPC5_ICU_USE_ETIMER1_CH0 == TRUE)
  if (&ICUD7 == icup) {
    ETIMER1D.etimer_active_channels++;
  }
#endif /* SPC5_ICU_USE_ETIMER1_CH0 */

#if (SPC5_ICU_USE_ETIMER1_CH1 == TRUE)
  if (&ICUD8 == icup) {
    ETIMER1D.etimer_active_channels++;
  }
#endif /* SPC5_ICU_USE_ETIMER1_CH1 */

#if (SPC5_ICU_USE_ETIMER1_CH2 == TRUE)
  if (&ICUD9 == icup) {
    ETIMER1D.etimer_active_channels++;
  }
#endif /* SPC5_ICU_USE_ETIMER1_CH2 */

#if (SPC5_ICU_USE_ETIMER1_CH3 == TRUE)
  if (&ICUD10 == icup) {
    ETIMER1D.etimer_active_channels++;
  }
#endif /* SPC5_ICU_USE_ETIMER1_CH3 */

#if (SPC5_ICU_USE_ETIMER1_CH4 == TRUE)
  if (&ICUD11 == icup) {
    ETIMER1D.etimer_active_channels++;
  }
#endif /* SPC5_ICU_USE_ETIMER1_CH4 */

#if (SPC5_ICU_USE_ETIMER1_CH5 == TRUE)
  if (&ICUD12 == icup) {
    ETIMER1D.etimer_active_channels++;
  }
#endif /* SPC5_ICU_USE_ETIMER1_CH5 */

#if (SPC5_ICU_USE_ETIMER2_CH0 == TRUE)
  if (&ICUD13 == icup) {
    ETIMER2D.etimer_active_channels++;
  }
#endif /* SPC5_ICU_USE_ETIMER2_CH0 */

#if (SPC5_ICU_USE_ETIMER2_CH1 == TRUE)
  if (&ICUD14 == icup) {
    ETIMER2D.etimer_active_channels++;
  }
#endif /* SPC5_ICU_USE_ETIMER2_CH1 */

#if (SPC5_ICU_USE_ETIMER2_CH2 == TRUE)
  if (&ICUD15 == icup) {
    ETIMER2D.etimer_active_channels++;
  }
#endif /* SPC5_ICU_USE_ETIMER2_CH2 */

#if (SPC5_ICU_USE_ETIMER2_CH3 == TRUE)
  if (&ICUD16 == icup) {
    ETIMER2D.etimer_active_channels++;
  }
#endif /* SPC5_ICU_USE_ETIMER2_CH3 */

#if (SPC5_ICU_USE_ETIMER2_CH4 == TRUE)
  if (&ICUD17 == icup) {
    ETIMER2D.etimer_active_channels++;
  }
#endif /* SPC5_ICU_USE_ETIMER2_CH4 */

#if (SPC5_ICU_USE_ETIMER2_CH5 == TRUE)
  if (&ICUD18 == icup) {
    ETIMER2D.etimer_active_channels++;
  }
#endif /* SPC5_ICU_USE_ETIMER2_CH5 */

#if (SPC5_ICU_USE_ETIMER3_CH0 == TRUE)
  if (&ICUD19 == icup) {
    ETIMER3D.etimer_active_channels++;
  }
#endif /* SPC5_ICU_USE_ETIMER3_CH0 */

#if (SPC5_ICU_USE_ETIMER3_CH1 == TRUE)
  if (&ICUD20 == icup) {
    ETIMER3D.etimer_active_channels++;
  }
#endif /* SPC5_ICU_USE_ETIMER3_CH1 */

#if (SPC5_ICU_USE_ETIMER3_CH2 == TRUE)
  if (&ICUD21 == icup) {
    ETIMER3D.etimer_active_channels++;
  }
#endif /* SPC5_ICU_USE_ETIMER3_CH2 */

#if (SPC5_ICU_USE_ETIMER3_CH3 == TRUE)
  if (&ICUD22 == icup) {
    ETIMER3D.etimer_active_channels++;
  }
#endif /* SPC5_ICU_USE_ETIMER3_CH3 */

#if (SPC5_ICU_USE_ETIMER3_CH4 == TRUE)
  if (&ICUD23 == icup) {
    ETIMER3D.etimer_active_channels++;
  }
#endif /* SPC5_ICU_USE_ETIMER3_CH4 */

#if (SPC5_ICU_USE_ETIMER3_CH5 == TRUE)
  if (&ICUD24 == icup) {
    ETIMER3D.etimer_active_channels++;
  }
#endif /* SPC5_ICU_USE_ETIMER3_CH5 */

  /* Enable ETIMER0 Clock.*/
#if (SPC5_HAS_ETIMER0 == TRUE)
  if (icup->etimerp == &SPC5_ETIMER_0) {
    etimer_enable_clock(&ETIMER0D);
  }
#endif /* SPC5_HAS_ETIMER0 */

  /* Enable ETIMER1 Clock.*/
#if (SPC5_HAS_ETIMER1 == TRUE)
  if (icup->etimerp == &SPC5_ETIMER_1) {
    etimer_enable_clock(&ETIMER1D);
  }
#endif /* SPC5_HAS_ETIMER1 */

  /* Enable ETIMER2 Clock.*/
#if (SPC5_HAS_ETIMER2 == TRUE)
  if (icup->etimerp == &SPC5_ETIMER_2) {
    etimer_enable_clock(&ETIMER2D);
  }
#endif /* SPC5_HAS_ETIMER2 */

  /* Enable ETIMER3 Clock.*/
#if (SPC5_HAS_ETIMER3 == TRUE)
  if (icup->etimerp == &SPC5_ETIMER_3) {
    etimer_enable_clock(&ETIMER3D);
  }
#endif /* SPC5_HAS_ETIMER3 */

  /* Disable counter.*/
  icup->etimerp->CH[icup->channel].CTRL1.B.CNTMODE =
      SPC5_ETIMER_CNTMODE_NO_OPERATION;

  /* Clear pending IRQs (if any).*/
  icup->etimerp->CH[icup->channel].STS.R = 0xFFFF;

  /* Disable all IRQs and DMA requests.*/
  icup->etimerp->CH[icup->channel].INTDMA.R = 0U;

  /* Disable Compare Load 1 and Compare Load 2.*/
  icup->etimerp->CH[icup->channel].CCCTRL.B.CLC1 = 0U;
  icup->etimerp->CH[icup->channel].CCCTRL.B.CLC2 = 0U;

  /* Disable Capture 1 and Capture 2.*/
  icup->etimerp->CH[icup->channel].CCCTRL.B.CPT1MODE =
      SPC5_ETIMER_CPT1MODE_DISABLED;
  icup->etimerp->CH[icup->channel].CCCTRL.B.CPT2MODE =
      SPC5_ETIMER_CPT2MODE_DISABLED;

  /* Reset counter to zero.*/
  icup->etimerp->CH[icup->channel].CNTR.R = 0U;

  /* Initialize channel.*/
  spc5_icu_channel_init(icup);

  osalExitCritical();
}

/**
 * @brief   Deactivates the ICU peripheral.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @api
 */
void icu_lld_stop(ICUDriver *icup) {

  osalEnterCritical();

  /* Disable channel.*/
  icup->etimerp->ENBL.R &= ~(uint16_t)(1UL << (icup->channel));

  /* Update the number of active ICU channels.*/
#if (SPC5_ICU_USE_ETIMER0_CH0 == TRUE)
  if (&ICUD1 == icup) {
    ETIMER0D.etimer_active_channels--;
  }
#endif /* SPC5_ICU_USE_ETIMER0_CH0 */

#if (SPC5_ICU_USE_ETIMER0_CH1 == TRUE)
  if (&ICUD2 == icup) {
    ETIMER0D.etimer_active_channels--;
  }
#endif /* SPC5_ICU_USE_ETIMER0_CH1 */

#if (SPC5_ICU_USE_ETIMER0_CH2 == TRUE)
  if (&ICUD3 == icup) {
    /* Disable channel.*/
    ETIMER0D.etimer_active_channels--;
  }
#endif /* SPC5_ICU_USE_ETIMER0_CH2 */

#if (SPC5_ICU_USE_ETIMER0_CH3 == TRUE)
  if (&ICUD4 == icup) {
    ETIMER0D.etimer_active_channels--;
  }
#endif /* SPC5_ICU_USE_ETIMER0_CH3 */

#if (SPC5_ICU_USE_ETIMER0_CH4 == TRUE)
  if (&ICUD5 == icup) {
    ETIMER0D.etimer_active_channels--;
  }
#endif /* SPC5_ICU_USE_ETIMER0_CH4 */

#if (SPC5_ICU_USE_ETIMER0_CH5 == TRUE)
  if (&ICUD6 == icup) {
    ETIMER0D.etimer_active_channels--;
  }
#endif /* SPC5_ICU_USE_ETIMER0_CH5 */

#if (SPC5_ICU_USE_ETIMER1_CH0 == TRUE)
  if (&ICUD7 == icup) {
    ETIMER1D.etimer_active_channels--;
  }
#endif /* SPC5_ICU_USE_ETIMER1_CH0 */

#if (SPC5_ICU_USE_ETIMER1_CH1 == TRUE)
  if (&ICUD8 == icup) {
    ETIMER1D.etimer_active_channels--;
  }
#endif /* SPC5_ICU_USE_ETIMER1_CH1 */

#if (SPC5_ICU_USE_ETIMER1_CH2 == TRUE)
  if (&ICUD9 == icup) {
    ETIMER1D.etimer_active_channels--;
  }
#endif /* SPC5_ICU_USE_ETIMER1_CH2 */

#if (SPC5_ICU_USE_ETIMER1_CH3 == TRUE)
  if (&ICUD10 == icup) {
    ETIMER1D.etimer_active_channels--;
  }
#endif /* SPC5_ICU_USE_ETIMER1_CH3 */

#if (SPC5_ICU_USE_ETIMER1_CH4 == TRUE)
  if (&ICUD11 == icup) {
    ETIMER1D.etimer_active_channels--;
  }
#endif /* SPC5_ICU_USE_ETIMER1_CH4 */

#if (SPC5_ICU_USE_ETIMER1_CH5 == TRUE)
  if (&ICUD12 == icup) {
    ETIMER1D.etimer_active_channels--;
  }
#endif /* SPC5_ICU_USE_ETIMER1_CH5 */

#if (SPC5_ICU_USE_ETIMER2_CH0 == TRUE)
  if (&ICUD13 == icup) {
    ETIMER2D.etimer_active_channels--;
  }
#endif /* SPC5_ICU_USE_ETIMER2_CH0 */

#if (SPC5_ICU_USE_ETIMER2_CH1 == TRUE)
  if (&ICUD14 == icup) {
    ETIMER2D.etimer_active_channels--;
  }
#endif /* SPC5_ICU_USE_ETIMER2_CH1 */

#if (SPC5_ICU_USE_ETIMER2_CH2 == TRUE)
  if (&ICUD15 == icup) {
    ETIMER2D.etimer_active_channels--;
  }
#endif /* SPC5_ICU_USE_ETIMER2_CH2 */

#if (SPC5_ICU_USE_ETIMER2_CH3 == TRUE)
  if (&ICUD16 == icup) {
    ETIMER2D.etimer_active_channels--;
  }
#endif /* SPC5_ICU_USE_ETIMER2_CH3 */

#if (SPC5_ICU_USE_ETIMER2_CH4 == TRUE)
  if (&ICUD17 == icup) {
    ETIMER2D.etimer_active_channels--;
  }
#endif /* SPC5_ICU_USE_ETIMER2_CH4 */

#if (SPC5_ICU_USE_ETIMER2_CH5 == TRUE)
  if (&ICUD18 == icup) {
    ETIMER2D.etimer_active_channels--;
  }
#endif /* SPC5_ICU_USE_ETIMER2_CH5 */

#if (SPC5_ICU_USE_ETIMER3_CH0 == TRUE)
  if (&ICUD19 == icup) {
    ETIMER3D.etimer_active_channels--;
  }
#endif /* SPC5_ICU_USE_ETIMER3_CH0 */

#if (SPC5_ICU_USE_ETIMER3_CH1 == TRUE)
  if (&ICUD20 == icup) {
    ETIMER3D.etimer_active_channels--;
  }
#endif /* SPC5_ICU_USE_ETIMER3_CH1 */

#if (SPC5_ICU_USE_ETIMER3_CH2 == TRUE)
  if (&ICUD21 == icup) {
    ETIMER3D.etimer_active_channels--;
  }
#endif /* SPC5_ICU_USE_ETIMER3_CH2 */

#if (SPC5_ICU_USE_ETIMER3_CH3 == TRUE)
  if (&ICUD22 == icup) {
    /* Disable channel.*/
    ETIMER3D.etimer_active_channels--;
  }
#endif /* SPC5_ICU_USE_ETIMER3_CH3 */

#if (SPC5_ICU_USE_ETIMER3_CH4 == TRUE)
  if (&ICUD23 == icup) {
    ETIMER3D.etimer_active_channels--;
  }
#endif /* SPC5_ICU_USE_ETIMER3_CH4 */

#if (SPC5_ICU_USE_ETIMER3_CH5 == TRUE)
  if (&ICUD24 == icup) {
    ETIMER3D.etimer_active_channels--;
  }
#endif /* SPC5_ICU_USE_ETIMER3_CH5 */

  /* Disable ETIMER0 Clock.*/
#if (SPC5_HAS_ETIMER0 == TRUE)
  if (icup->etimerp == &SPC5_ETIMER_0) {
    etimer_disable_clock(&ETIMER0D);
  }
#endif /* SPC5_HAS_ETIMER0 */

  /* Disable ETIMER1 Clock.*/
#if (SPC5_HAS_ETIMER1 == TRUE)
  if (icup->etimerp == &SPC5_ETIMER_1) {
    etimer_disable_clock(&ETIMER1D);
  }
#endif /* SPC5_HAS_ETIMER1 */

  /* Disable ETIMER2 Clock.*/
#if (SPC5_HAS_ETIMER2 == TRUE)
  if (icup->etimerp == &SPC5_ETIMER_2) {
    etimer_disable_clock(&ETIMER2D);
  }
#endif /* SPC5_HAS_ETIMER2 */

  /* Disable ETIMER3 Clock.*/
#if (SPC5_HAS_ETIMER3 == TRUE)
  if (icup->etimerp == &SPC5_ETIMER_3) {
    etimer_disable_clock(&ETIMER3D);
  }
#endif /* SPC5_HAS_ETIMER3 */

  osalExitCritical();
}

/**
 * @brief   Enables the input capture.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @api
 */
void icu_lld_enable(ICUDriver *icup) {

  osalEnterCritical();

  /* Clear pending IRQs (if any).*/
  icup->etimerp->CH[icup->channel].STS.R = 0xFFFFU;

  /* Set Capture 1 and Capture 2 Mode.*/
  icup->etimerp->CH[icup->channel].CCCTRL.B.CPT1MODE =
      SPC5_ETIMER_CPT1MODE_RISING_EDGE;
  icup->etimerp->CH[icup->channel].CTRL3.B.ROC =
      SPC5_ETIMER_ROC_REL_ON_CAP1;
  icup->etimerp->CH[icup->channel].CCCTRL.B.CPT2MODE =
      SPC5_ETIMER_CPT2MODE_FALLING_EDGE;

  /* Active interrupts.*/
  if (icup->config->period_callback != NULL || icup->config->width_callback != NULL) {
    icup->etimerp->CH[icup->channel].INTDMA.B.ICF1IE = 1U;
    icup->etimerp->CH[icup->channel].INTDMA.B.ICF2IE = 1U;
  }
  if (icup->config->overflow_callback != NULL) {
    icup->etimerp->CH[icup->channel].INTDMA.B.TOFIE = 1U;
  }

  /* Set Capture FIFO Water Mark.*/
  icup->etimerp->CH[icup->channel].CCCTRL.B.CFWM = 0U;

  /* Enable Counter.*/
#if (ICU_SKIP_FIRST_CAPTURE == TRUE)
  icup->etimerp->CH[icup->channel].CTRL1.B.CNTMODE =
      SPC5_ETIMER_CNTMODE_RFE_SIHA;
#else
  icup->etimerp->CH[icup->channel].CTRL1.B.CNTMODE =
      SPC5_ETIMER_CNTMODE_RE;
#endif /* ICU_SKIP_FIRST_CAPTURE */

  /* Enable Capture process.*/
  icup->etimerp->CH[icup->channel].CCCTRL.B.ARM = 1U;
  
  osalExitCritical();
}

/**
 * @brief   Disables the input capture.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @api
 */
void icu_lld_disable(ICUDriver *icup) {

  osalEnterCritical();

  /* Disable Capture process.*/
  icup->etimerp->CH[icup->channel].CCCTRL.B.ARM = 0U;

  /* Clear pending IRQs (if any).*/
  icup->etimerp->CH[icup->channel].STS.R = 0xFFFFU;

  /* Set Capture 1 and Capture 2 Mode to Disabled.*/
  icup->etimerp->CH[icup->channel].CCCTRL.B.CPT1MODE =
      SPC5_ETIMER_CPT1MODE_DISABLED;
  icup->etimerp->CH[icup->channel].CCCTRL.B.CPT2MODE =
      SPC5_ETIMER_CPT2MODE_DISABLED;

  /* Disable interrupts.*/
  if (icup->config->period_callback != NULL || icup->config->width_callback != NULL) {
    icup->etimerp->CH[icup->channel].INTDMA.B.ICF1IE = 0U;
    icup->etimerp->CH[icup->channel].INTDMA.B.ICF2IE = 0U;
  }
  if (icup->config->overflow_callback != NULL) {
    icup->etimerp->CH[icup->channel].INTDMA.B.TOFIE = 0U;
  }

  osalExitCritical();
}

#endif /* LLD_USE_ICU */

/** @} */
