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
 * @file    saradc_lld.c
 * @brief   SPC5xx SARADC low level driver code.
 *
 * @addtogroup SARADC
 * @{
 */

#include "saradc_lld.h"

#if (LLD_USE_SARADC == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#if (SPC5_SARADC_USE_DMA == TRUE) || defined(__DOXYGEN__)
static void saradc_lld_serve_dma_irq(edma_channel_t channel, void *p);
static void saradc_lld_dma_error_irq(edma_channel_t channel, void *p, uint32_t esr);
static void saradc_lld_configure_channels_dma(SARADCDriver *saradcp);
#endif
#if (SPC5_SARADC_MAX_NUMOFCHANNELS > 0U) || defined(__DOXYGEN__)
static void saradc_lld_configure_channels(SARADCDriver *saradcp);
#endif

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   SARADC12_1 driver identifier.
 */
#if (SPC5_SARADC_USE_SARADC12_0 == TRUE) || defined(__DOXYGEN__)
SARADCDriver SARADC12D1;
#endif

/**
 * @brief   SARADC12_2 driver identifier.
 */
#if (SPC5_SARADC_USE_SARADC12_1 == TRUE) || defined(__DOXYGEN__)
SARADCDriver SARADC12D2;
#endif

/**
 * @brief   SARADC12_3 driver identifier.
 */
#if (SPC5_SARADC_USE_SARADC12_2 == TRUE) || defined(__DOXYGEN__)
SARADCDriver SARADC12D3;
#endif

/**
 * @brief   SARADC12_4 driver identifier.
 */
#if (SPC5_SARADC_USE_SARADC12_3 == TRUE) || defined(__DOXYGEN__)
SARADCDriver SARADC12D4;
#endif

/**
 * @brief   SARADC12_5 driver identifier.
 */
#if (SPC5_SARADC_USE_SARADC12_4 == TRUE) || defined(__DOXYGEN__)
SARADCDriver SARADC12D5;
#endif

/**
 * @brief   SARADC12_6 driver identifier.
 */
#if (SPC5_SARADC_USE_SARADC12_5 == TRUE) || defined(__DOXYGEN__)
SARADCDriver SARADC12D6;
#endif

/**
 * @brief   SARADC12_7 driver identifier.
 */
#if (SPC5_SARADC_USE_SARADC12_6 == TRUE) || defined(__DOXYGEN__)
SARADCDriver SARADC12D7;
#endif

/**
 * @brief   SARADC12_SV driver identifier.
 */
#if (SPC5_SARADC_USE_SARADC12_SV == TRUE) || defined(__DOXYGEN__)
SARADCDriver SARADC12DSV;
#endif

/**
 * @brief   SARADC12_SV driver identifier.
 */
#if (SPC5_SARADC_USE_SARADC12_SV_1 == TRUE) || defined(__DOXYGEN__)
SARADCDriver SARADC12DSV1;
#endif

/**
 * @brief   SARADC10_1 driver identifier.
 */
#if (SPC5_SARADC_USE_SARADC10_0 == TRUE) || defined(__DOXYGEN__)
SARADCDriver SARADC10D1;
#endif

/**
 * @brief   SARADC10_2 driver identifier.
 */
#if (SPC5_SARADC_USE_SARADC10_1 == TRUE) || defined(__DOXYGEN__)
SARADCDriver SARADC10D2;
#endif

/**
 * @brief   SARADC10_2 driver identifier.
 */
#if (SPC5_SARADC_USE_SARADC10_STANDBY == TRUE) || defined(__DOXYGEN__)
SARADCDriver SARADC10DSB;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

#if (SPC5_SARADC_USE_DMA == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for SARADC12_SV
 */
#if (SPC5_SARADC_USE_SARADC12_SV == TRUE) || defined(__DOXYGEN__)
static const edma_channel_config_t saradc12_sv_dma_config = {
SPC5_SARADC12_SV_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
SPC5_SARADC12_SV_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
SPC5_SARADC12_SV_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
SPC5_SARADC12_SV_DMA_IRQ_PRIO,
saradc_lld_serve_dma_irq, saradc_lld_dma_error_irq, &SARADC12DSV};
#endif

/**
 * @brief   DMA configuration for SARADC12_0
 */
#if (SPC5_SARADC_USE_SARADC12_0 == TRUE) || defined(__DOXYGEN__)
static const edma_channel_config_t saradc12_0_dma_config = {
SPC5_SARADC12_0_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
 SPC5_SARADC12_0_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
SPC5_SARADC12_0_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
SPC5_SARADC12_0_DMA_IRQ_PRIO,
saradc_lld_serve_dma_irq, saradc_lld_dma_error_irq, &SARADC12D1};
#endif

/**
 * @brief   DMA configuration for SARADC12_1
 */
#if (SPC5_SARADC_USE_SARADC12_1 == TRUE) || defined(__DOXYGEN__)
static const edma_channel_config_t saradc12_1_dma_config = {
SPC5_SARADC12_1_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
 SPC5_SARADC12_1_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
SPC5_SARADC12_1_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
SPC5_SARADC12_1_DMA_IRQ_PRIO,
saradc_lld_serve_dma_irq, saradc_lld_dma_error_irq, &SARADC12D2};
#endif

/**
 * @brief   DMA configuration for SARADC12_2
 */
#if (SPC5_SARADC_USE_SARADC12_2 == TRUE) || defined(__DOXYGEN__)
static const edma_channel_config_t saradc12_2_dma_config = {
SPC5_SARADC12_2_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
 SPC5_SARADC12_2_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
SPC5_SARADC12_2_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
SPC5_SARADC12_2_DMA_IRQ_PRIO,
saradc_lld_serve_dma_irq, saradc_lld_dma_error_irq, &SARADC12D3};
#endif

/**
 * @brief   DMA configuration for SARADC12_3
 */
#if (SPC5_SARADC_USE_SARADC12_3 == TRUE) || defined(__DOXYGEN__)
static const edma_channel_config_t saradc12_3_dma_config = {
SPC5_SARADC12_3_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
 SPC5_SARADC12_3_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
SPC5_SARADC12_3_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
SPC5_SARADC12_3_DMA_IRQ_PRIO,
saradc_lld_serve_dma_irq, saradc_lld_dma_error_irq, &SARADC12D4};
#endif

/**
 * @brief   DMA configuration for SARADC12_4
 */
#if (SPC5_SARADC_USE_SARADC12_4 == TRUE) || defined(__DOXYGEN__)
static const edma_channel_config_t saradc12_4_dma_config = {
SPC5_SARADC12_4_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
 SPC5_SARADC12_4_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
SPC5_SARADC12_4_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
SPC5_SARADC12_4_DMA_IRQ_PRIO,
saradc_lld_serve_dma_irq, saradc_lld_dma_error_irq, &SARADC12D5};
#endif

/**
 * @brief   DMA configuration for SARADC12_5
 */
#if (SPC5_SARADC_USE_SARADC12_5 == TRUE) || defined(__DOXYGEN__)
static const edma_channel_config_t saradc12_5_dma_config = {
SPC5_SARADC12_5_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
 SPC5_SARADC12_5_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
SPC5_SARADC12_5_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
SPC5_SARADC12_5_DMA_IRQ_PRIO,
saradc_lld_serve_dma_irq, saradc_lld_dma_error_irq, &SARADC12D6};
#endif

/**
 * @brief   DMA configuration for SARADC12_6
 */
#if (SPC5_SARADC_USE_SARADC12_6 == TRUE) || defined(__DOXYGEN__)
static const edma_channel_config_t saradc12_6_dma_config = {
SPC5_SARADC12_6_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
 SPC5_SARADC12_6_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
SPC5_SARADC12_6_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
SPC5_SARADC12_6_DMA_IRQ_PRIO,
saradc_lld_serve_dma_irq, saradc_lld_dma_error_irq, &SARADC12D7};
#endif

/**
 * @brief   DMA configuration for SARADC10_0
 */
#if (SPC5_SARADC_USE_SARADC10_0 == TRUE) || defined(__DOXYGEN__)
static const edma_channel_config_t saradc10_0_dma_config = {
SPC5_SARADC10_0_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
 SPC5_SARADC10_0_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
SPC5_SARADC10_0_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
SPC5_SARADC10_0_DMA_IRQ_PRIO,
saradc_lld_serve_dma_irq, saradc_lld_dma_error_irq, &SARADC10D1};
#endif

/**
 * @brief   DMA configuration for SARADC10_1
 */
#if (SPC5_SARADC_USE_SARADC10_1 == TRUE) || defined(__DOXYGEN__)
static const edma_channel_config_t saradc10_1_dma_config = {
SPC5_SARADC10_1_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
 SPC5_SARADC10_1_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
SPC5_SARADC10_1_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
SPC5_SARADC10_1_DMA_IRQ_PRIO,
saradc_lld_serve_dma_irq, saradc_lld_dma_error_irq, &SARADC10D2};
#endif

/**
 * @brief   DMA configuration for SARADC10_STANDBY
 */
#if (SPC5_SARADC_USE_SARADC10_STANDBY == TRUE) || defined(__DOXYGEN__)
static const edma_channel_config_t saradc10_standby_dma_config = {
SPC5_SARADC10_STANDBY_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
 SPC5_SARADC10_STANDBY_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
SPC5_SARADC10_STANDBY_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
SPC5_SARADC10_STANDBY_DMA_IRQ_PRIO,
saradc_lld_serve_dma_irq, saradc_lld_dma_error_irq, &SARADC10DSB};
#endif

#endif

/*===========================================================================*/
/* Driver local functions and macros.                                        */
/*===========================================================================*/

/**
 * @brief   Unsigned two's complement.
 *
 * @param[in] n         the value to be complemented
 *
 * @notapi
 */
#define CPL2(n) ((~(uint32_t)(n)) + 1UL)

/**
 * @brief   SARADC ISR service routine.
 *
 * @param[in] saradcp      pointer to the @p SARADCDriver object
 *
 * @notapi
 */
static void saradc_lld_serve_interrupt(SARADCDriver *saradcp) {

  osalEnterCriticalFromISR();

  #if (SPC5_SARADC_MAX_NUMOFCHANNELS > 0U) || defined(__DOXYGEN__)
  /* check watchdog register */
  if (saradcp->saradc->WTISR.R != 0UL) {
    uint8_t i;
    uint8_t reg_index;
    uint8_t bit_index;
	
    /* scan out of range channel and call related callback */
    for (i = 0; i < saradcp->config->numofchannels; i++) {
      if (saradcp->config->ch[i].type == SARADC_CHANNEL_INTERNAL && saradcp->config->ch[i].thr != SARADC_WATCHDOG_REGISTER_NONE) {
        reg_index = (uint8_t)(saradcp->config->ch[i].number / 32U);
        bit_index = (uint8_t)(saradcp->config->ch[i].number % 32U);
        if ((saradcp->saradc->ICAWORR[reg_index].R & (1UL << bit_index)) == (1UL << bit_index)) {
          /* out of range callback */
          saradcp->config->ch[i].oorcallback(saradcp);
          /*clear flag */
          saradcp->saradc->ICAWORR[reg_index].R |= (1UL << bit_index);
        }
      }
      else if (saradcp->config->ch[i].type == SARADC_CHANNEL_TEST && saradcp->config->ch[i].thr != SARADC_WATCHDOG_REGISTER_NONE) {
        bit_index = (uint8_t)(saradcp->config->ch[i].number - 96U);
        if ((saradcp->saradc->TCAWORR.R & (1UL << bit_index)) == (1UL << bit_index)) {
          /* out of range callback */
          saradcp->config->ch[i].oorcallback(saradcp);
          /*clear flag */
          saradcp->saradc->TCAWORR.R |= (1UL << bit_index);
        }
      }
      else {
        /* MISRA */
      }
    }
    /* clear WTISR */
    saradcp->saradc->WTISR.R = 0xFFFFFFFFUL;
  }
#endif
  /* callback */
  if (saradcp->config->convcallback != NULL) {
    saradcp->config->convcallback(saradcp);
  }
  /* clear flag*/
  saradcp->saradc->ISR.R = 0xFFFFFFFFUL;

  osalExitCriticalFromISR();

}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if (SPC5_SARADC_USE_SARADC12_0 == TRUE)
/**
 * @brief   SARADC12_0 Interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_SARADC12_0_HANDLER) {

  IRQ_PROLOGUE();

  saradc_lld_serve_interrupt(&SARADC12D1);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_SARADC_USE_SARADC12_1 == TRUE)
/**
 * @brief   SARADC12_1 Interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_SARADC12_1_HANDLER) {

  IRQ_PROLOGUE();

  saradc_lld_serve_interrupt(&SARADC12D2);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_SARADC_USE_SARADC12_2 == TRUE)
/**
 * @brief   SARADC12_2 Interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_SARADC12_2_HANDLER) {

  IRQ_PROLOGUE();

  saradc_lld_serve_interrupt(&SARADC12D3);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_SARADC_USE_SARADC12_3 == TRUE)
/**
 * @brief   SARADC12_3 Interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_SARADC12_3_HANDLER) {

  IRQ_PROLOGUE();

  saradc_lld_serve_interrupt(&SARADC12D4);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_SARADC_USE_SARADC12_4 == TRUE)
/**
 * @brief   SARADC12_4 Interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_SARADC12_4_HANDLER) {

  IRQ_PROLOGUE();

  saradc_lld_serve_interrupt(&SARADC12D5);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_SARADC_USE_SARADC12_5 == TRUE)
/**
 * @brief   SARADC12_5 Interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_SARADC12_5_HANDLER) {

  IRQ_PROLOGUE();

  saradc_lld_serve_interrupt(&SARADC12D6);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_SARADC_USE_SARADC12_6 == TRUE)
/**
 * @brief   SARADC12_6 Interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_SARADC12_6_HANDLER) {

  IRQ_PROLOGUE();

  saradc_lld_serve_interrupt(&SARADC12D7);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_SARADC_USE_SARADC12_SV == TRUE)
/**
 * @brief   SARADC12_SV Interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_SARADC12_SV_HANDLER) {

  IRQ_PROLOGUE();

  saradc_lld_serve_interrupt(&SARADC12DSV);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_SARADC_USE_SARADC12_SV_1 == TRUE)
/**
 * @brief   SARADC12_SV_1 Interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_SARADC12_SV_1_HANDLER) {

  IRQ_PROLOGUE();

  saradc_lld_serve_interrupt(&SARADC12DSV1);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_SARADC_USE_SARADC10_0 == TRUE)
/**
 * @brief   SARADC10_0 Interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_SARADC10_0_HANDLER) {

  IRQ_PROLOGUE();

  saradc_lld_serve_interrupt(&SARADC10D1);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_SARADC_USE_SARADC10_1 == TRUE)
/**
 * @brief   SARADC10_1 Interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_SARADC10_1_HANDLER) {

  IRQ_PROLOGUE();

  saradc_lld_serve_interrupt(&SARADC10D2);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_SARADC_USE_SARADC10_STANDBY == TRUE)
/**
 * @brief   SARADC10_STANDBY Interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_SARADC10_STANDBY_HANDLER) {

  IRQ_PROLOGUE();

  saradc_lld_serve_interrupt(&SARADC10DSB);

  IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level SARADC driver initialization.
 *
 * @init
 */
void saradc_lld_init(void) {

#if (SPC5_SARADC_USE_SARADC12_0 == TRUE)
  SARADC12D1.config = NULL;
  SARADC12D1.saradc = &SPC5_SARADC12_0;
  INTC_PSR(SPC5_SARADC12_0_INT_NUMBER) = SPC5_SARADC12_0_PRIORITY;
#endif
#if (SPC5_SARADC_USE_SARADC12_1 == TRUE)
  SARADC12D2.config = NULL;
  SARADC12D2.saradc = &SPC5_SARADC12_1;
  INTC_PSR(SPC5_SARADC12_1_INT_NUMBER) = SPC5_SARADC12_1_PRIORITY;
#endif
#if (SPC5_SARADC_USE_SARADC12_2 == TRUE)
  SARADC12D3.config = NULL;
  SARADC12D3.saradc = &SPC5_SARADC12_2;
  INTC_PSR(SPC5_SARADC12_2_INT_NUMBER) = SPC5_SARADC12_2_PRIORITY;
#endif
#if (SPC5_SARADC_USE_SARADC12_3 == TRUE)
  SARADC12D4.config = NULL;
  SARADC12D4.saradc = &SPC5_SARADC12_3;
  INTC_PSR(SPC5_SARADC12_3_INT_NUMBER) = SPC5_SARADC12_3_PRIORITY;
#endif
#if (SPC5_SARADC_USE_SARADC12_4 == TRUE)
  SARADC12D5.config = NULL;
  SARADC12D5.saradc = &SPC5_SARADC12_4;
  INTC_PSR(SPC5_SARADC12_4_INT_NUMBER) = SPC5_SARADC12_4_PRIORITY;
#endif
#if (SPC5_SARADC_USE_SARADC12_5 == TRUE)
  SARADC12D6.config = NULL;
  SARADC12D6.saradc = &SPC5_SARADC12_5;
  INTC_PSR(SPC5_SARADC12_5_INT_NUMBER) = SPC5_SARADC12_5_PRIORITY;
#endif
#if (SPC5_SARADC_USE_SARADC12_6 == TRUE)
  SARADC12D7.config = NULL;
  SARADC12D7.saradc = &SPC5_SARADC12_6;
  INTC_PSR(SPC5_SARADC12_6_INT_NUMBER) = SPC5_SARADC12_6_PRIORITY;
#endif
#if (SPC5_SARADC_USE_SARADC12_SV == TRUE)
  SARADC12DSV.config = NULL;
  SARADC12DSV.saradc = &SPC5_SARADC12_SV;
  SARADC12DSV.noofinstances = 0U;
  INTC_PSR(SPC5_SARADC12_SV_INT_NUMBER) = SPC5_SARADC12_SV_PRIORITY;
#endif
#if (SPC5_SARADC_USE_SARADC12_SV_1 == TRUE)
  SARADC12DSV1.config = NULL;
  SARADC12DSV1.saradc = &SPC5_SARADC12_SV_1;
  INTC_PSR(SPC5_SARADC12_SV_1_INT_NUMBER) = SPC5_SARADC12_SV_1_PRIORITY;
#endif
#if (SPC5_SARADC_USE_SARADC10_0 == TRUE)
  SARADC10D1.config = NULL;
  SARADC10D1.saradc = &SPC5_SARADC10_0;
  INTC_PSR(SPC5_SARADC10_0_INT_NUMBER) = SPC5_SARADC10_0_PRIORITY;
#endif
#if (SPC5_SARADC_USE_SARADC10_1 == TRUE)
  SARADC10D2.config = NULL;
  SARADC10D2.saradc = &SPC5_SARADC10_1;
  INTC_PSR(SPC5_SARADC10_1_INT_NUMBER) = SPC5_SARADC10_1_PRIORITY;
#endif
#if (SPC5_SARADC_USE_SARADC10_STANDBY == TRUE)
  SARADC10DSB.config = NULL;
  SARADC10DSB.saradc = &SPC5_SARADC10_STANDBY;
  INTC_PSR(SPC5_SARADC10_STANDBY_INT_NUMBER) = SPC5_SARADC10_STANDBY_PRIORITY;
#endif
}

/**
 * @brief   Configures and activates the SARADC peripheral.
 *
 * @param[in] saradcp      pointer to the @p SARADCDriver object
 * @param[in] config	   pointer to the @p SARADCDconfig object
 *
 * @api
 */
void saradc_lld_start(SARADCDriver *saradcp, SARADCConfig *config) {

  uint16_t i;

  saradcp->config = config;

#if (SPC5_SARADC_USE_SARADC12_0 == TRUE)
  if (&SARADC12D1 == saradcp) {
    SPCSetPeripheralClockMode(SPC5_SARADC12_0_PCTL, (SPC5_ME_PCTL_RUN(2) | SPC5_ME_PCTL_LP(2)));
#if (SPC5_SARADC_USE_DMA == TRUE) || defined(__DOXYGEN__)
    if (saradcp->config->dma_enabled == TRUE) {
      /* Allocate DMA channel for Selected SARADC */
      saradcp->dma_channel = (uint8_t)edmaChannelAllocate(&saradc12_0_dma_config);
    }
#endif
  }
#endif
#if (SPC5_SARADC_USE_SARADC12_1 == TRUE)
  if (&SARADC12D2 == saradcp) {
    SPCSetPeripheralClockMode(SPC5_SARADC12_1_PCTL, (SPC5_ME_PCTL_RUN(2) | SPC5_ME_PCTL_LP(2)));
#if (SPC5_SARADC_USE_DMA == TRUE) || defined(__DOXYGEN__)
    if (saradcp->config->dma_enabled == TRUE) {
      /* Allocate DMA channel for Selected SARADC */
      saradcp->dma_channel = (uint8_t)edmaChannelAllocate(&saradc12_1_dma_config);
    }
#endif
  }
#endif
#if (SPC5_SARADC_USE_SARADC12_2 == TRUE)
  if (&SARADC12D3 == saradcp) {
    SPCSetPeripheralClockMode(SPC5_SARADC12_2_PCTL, (SPC5_ME_PCTL_RUN(2) | SPC5_ME_PCTL_LP(2)));
#if (SPC5_SARADC_USE_DMA == TRUE) || defined(__DOXYGEN__)
    if (saradcp->config->dma_enabled == TRUE) {
      /* Allocate DMA channel for Selected SARADC */
      saradcp->dma_channel = (uint8_t)edmaChannelAllocate(&saradc12_2_dma_config);
    }
#endif
  }
#endif
#if (SPC5_SARADC_USE_SARADC12_3 == TRUE)
  if (&SARADC12D4 == saradcp) {
    SPCSetPeripheralClockMode(SPC5_SARADC12_3_PCTL, (SPC5_ME_PCTL_RUN(2) | SPC5_ME_PCTL_LP(2)));
#if (SPC5_SARADC_USE_DMA == TRUE) || defined(__DOXYGEN__)
    if (saradcp->config->dma_enabled == TRUE) {
      /* Allocate DMA channel for Selected SARADC */
      saradcp->dma_channel = (uint8_t)edmaChannelAllocate(&saradc12_3_dma_config);
    }
#endif
  }
#endif
#if (SPC5_SARADC_USE_SARADC12_4 == TRUE)
  if (&SARADC12D5 == saradcp) {
    SPCSetPeripheralClockMode(SPC5_SARADC12_4_PCTL, (SPC5_ME_PCTL_RUN(2) | SPC5_ME_PCTL_LP(2)));
#if (SPC5_SARADC_USE_DMA == TRUE) || defined(__DOXYGEN__)
    if (saradcp->config->dma_enabled == TRUE) {
      /* Allocate DMA channel for Selected SARADC */
      saradcp->dma_channel = (uint8_t)edmaChannelAllocate(&saradc12_4_dma_config);
    }
#endif
  }
#endif
#if (SPC5_SARADC_USE_SARADC12_5 == TRUE)
  if (&SARADC12D6 == saradcp) {
    SPCSetPeripheralClockMode(SPC5_SARADC12_5_PCTL, (SPC5_ME_PCTL_RUN(2) | SPC5_ME_PCTL_LP(2)));
#if (SPC5_SARADC_USE_DMA == TRUE) || defined(__DOXYGEN__)
    if (saradcp->config->dma_enabled == TRUE) {
      /* Allocate DMA channel for Selected SARADC */
      saradcp->dma_channel = (uint8_t)edmaChannelAllocate(&saradc12_5_dma_config);
    }
#endif
  }
#endif
#if (SPC5_SARADC_USE_SARADC12_6 == TRUE)
  if (&SARADC12D7 == saradcp) {
    SPCSetPeripheralClockMode(SPC5_SARADC12_6_PCTL, (SPC5_ME_PCTL_RUN(2) | SPC5_ME_PCTL_LP(2)));
#if (SPC5_SARADC_USE_DMA == TRUE) || defined(__DOXYGEN__)
    if (saradcp->config->dma_enabled == TRUE) {
      /* Allocate DMA channel for Selected SARADC */
      saradcp->dma_channel = (uint8_t)edmaChannelAllocate(&saradc12_6_dma_config);
    }
#endif 
  }
#endif
#if (SPC5_SARADC_USE_SARADC12_SV == TRUE)
  if (&SARADC12DSV == saradcp) {
    if (SARADC12DSV.noofinstances == 0U) {
      SPCSetPeripheralClockMode(SPC5_SARADC12_SV_PCTL, (SPC5_ME_PCTL_RUN(2) | SPC5_ME_PCTL_LP(2)));
    }
    SARADC12DSV.noofinstances++;
#if (SPC5_SARADC_USE_DMA == TRUE) || defined(__DOXYGEN__)
    if (saradcp->config->dma_enabled == TRUE) {
      /* Allocate DMA channel for Selected SARADC */
      saradcp->dma_channel = (uint8_t)edmaChannelAllocate(&saradc12_sv_dma_config);
    }
#endif
  }
#endif
#if (SPC5_SARADC_USE_SARADC12_SV_1 == TRUE)
  if (&SARADC12DSV1 == saradcp) {
    SPCSetPeripheralClockMode(SPC5_SARADC12_SV_1_PCTL, (SPC5_ME_PCTL_RUN(2) | SPC5_ME_PCTL_LP(2)));
  }
#endif
#if (SPC5_SARADC_USE_SARADC10_0 == TRUE)
  if (&SARADC10D1 == saradcp) {
    SPCSetPeripheralClockMode(SPC5_SARADC10_0_PCTL, (SPC5_ME_PCTL_RUN(2) | SPC5_ME_PCTL_LP(2)));
#if (SPC5_SARADC_USE_DMA == TRUE) || defined(__DOXYGEN__)
    if (saradcp->config->dma_enabled == TRUE) {
      /* Allocate DMA channel for Selected SARADC */
      saradcp->dma_channel = (uint8_t)edmaChannelAllocate(&saradc10_0_dma_config);
    }
#endif
  }
#endif
#if (SPC5_SARADC_USE_SARADC10_1 == TRUE)
  if (&SARADC10D2 == saradcp) {
    SPCSetPeripheralClockMode(SPC5_SARADC10_1_PCTL, (SPC5_ME_PCTL_RUN(2) | SPC5_ME_PCTL_LP(2)));
#if (SPC5_SARADC_USE_DMA == TRUE) || defined(__DOXYGEN__)
    if (saradcp->config->dma_enabled == TRUE) {
      /* Allocate DMA channel for Selected SARADC */
      saradcp->dma_channel = (uint8_t)edmaChannelAllocate(&saradc10_1_dma_config);
    }
#endif
  }
#endif
#if (SPC5_SARADC_USE_SARADC10_STANDBY == TRUE)
  if (&SARADC10DSB == saradcp) {
    SPCSetPeripheralClockMode(SPC5_SARADC10_STANDBY_PCTL, (SPC5_ME_PCTL_RUN(2) | SPC5_ME_PCTL_LP(2)));
  }
#if (SPC5_SARADC_USE_DMA == TRUE) || defined(__DOXYGEN__)
    if (saradcp->config->dma_enabled == TRUE) {
      /* Allocate DMA channel for Selected SARADC */
      saradcp->dma_channel = (uint8_t)edmaChannelAllocate(&saradc10_standby_dma_config);
    }
#endif
#endif

  /* Set power down delay [PDED * 1/(SARADC clock frequency)].*/
  //saradcp->saradc->PDEDR.B.PDED = 0x10;
  /* power up module */
  saradcp->saradc->MCR.B.PWDN = 0U;
  /* Oneshot or Scan mode selection */
  saradcp->saradc->MCR.B.MODE = saradcp->config->mode;
  /* Enables converted data to be overwritten by a new conversion */
  saradcp->saradc->MCR.B.OWREN = 1U;
  /*  Freeze ADC when enter debug mode */
  saradcp->saradc->MCR.B.FRZ = 1U;

  /* Configure triggers */
  if (saradcp->config->trigger != SARADC_TRIGGER_DISABLED){
    saradcp->saradc->MCR.B.NTRGEN = SARADC_TRIGGER_ENABLED;
    saradcp->saradc->MCR.B.NEDGESEL = saradcp->config->trigger;
  }

  /* configure CTRL registers */
  for (i = 0; i < 4U; i++) {
    saradcp->saradc->CTR[i].B.CRES = saradcp->config->ctrx[i].cres;
    saradcp->saradc->CTR[i].B.PRECHG = saradcp->config->ctrx[i].prechg;
    saradcp->saradc->CTR[i].B.INPSAMP = saradcp->config->ctrx[i].inpsamp;
  }
#if (SPC5_SARADC_USE_DMA == TRUE) || defined(__DOXYGEN__)
  if (saradcp->config->dma_enabled == TRUE) {
    saradc_lld_configure_channels_dma(saradcp);
  }
#endif

  /* Configure analog watchdog thresolds */
  for (i = 0; i < saradcp->config->numofthresholds; i++) {
    float step;
    float thr;
    uint16_t low;
    uint16_t high;
    step = (float)4096 / saradcp->config->vref;
    thr = step * saradcp->config->thr[i].low;
    low = (uint16_t)(thr);
    if (saradcp->config->thr[i].high >= saradcp->config->vref) {
      high = 0xFFFFU;
    }
    else {
      thr = step * saradcp->config->thr[i].high;
      high = (uint16_t)(thr);
    }
    saradcp->saradc->WTHRHLR[i].B.THRL = low;
    saradcp->saradc->WTHRHLR[i].B.THRH = high;
  }

  if (saradcp->config->dma_enabled == FALSE) {

#if (SPC5_SARADC_MAX_NUMOFCHANNELS > 0U) || defined(__DOXYGEN__)
  /* configure channels if DMA is not used */
    saradc_lld_configure_channels(saradcp);
#endif

    /* Clear interrupt Flags */
    saradcp->saradc->ISR.R = 0x3FUL;

    /*Enable Normal Chain conversion interrupt if callback is enabled*/
    if (saradcp->config->convcallback != NULL) {
      saradcp->saradc->IMR.R = 0x01UL;
    }

    /* Clear Watchdog interrupt flags */
    saradcp->saradc->WTISR.R = 0xFFUL;

    /* Enable interrupts for configured thresholds */
    for (i = 0; i < saradcp->config->numofthresholds; i++) {
      saradcp->saradc->WTIMR.R |= 3UL << (i * 2U);
    }
  }
}

/**
 * @brief   Deactivates the SARADC peripheral.
 *
 * @param[in] saradcp      pointer to the @p SARADCDriver object
 *
 * @api
 */
void saradc_lld_stop(SARADCDriver *saradcp) {

  uint8_t i;
  uint8_t skip = 0U;

#if (SPC5_SARADC_USE_SARADC12_SV == TRUE)
  /* If more than one istance of Supervisor are running, do not stop the driver */
  if (&SARADC12DSV == saradcp && SARADC12DSV.noofinstances > 1U) {
	  skip = 1U;
  }
#endif

  if (skip == 0U){
    /*all interrupt and conversion are disabled */
    for (i = 0; i < 3U; i++) {
      saradcp->saradc->ICIMR[i].R = 0;
      saradcp->saradc->ICNCMR[i].R = 0;
    }
    /* Puts the SARADC Peripheral in Power-Down Mode.*/
    saradcp->saradc->MCR.B.PWDN = 1U;
  }

  /* Disables the peripheral.*/
#if (SPC5_SARADC_USE_SARADC12_0 == TRUE)
  if (&SARADC12D1 == saradcp) {
#if (SPC5_SARADC_USE_DMA == TRUE) || defined(__DOXYGEN__)
    if (saradcp->config->dma_enabled == TRUE) {
      edmaChannelRelease(&saradc12_0_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_SARADC12_0_PCTL, (SPC5_ME_PCTL_RUN(0) | SPC5_ME_PCTL_LP(0)));
  }
#endif
#if (SPC5_SARADC_USE_SARADC12_1 == TRUE)
  if (&SARADC12D2 == saradcp) {
#if (SPC5_SARADC_USE_DMA == TRUE) || defined(__DOXYGEN__)
    if (saradcp->config->dma_enabled == TRUE) {
      edmaChannelRelease(&saradc12_1_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_SARADC12_1_PCTL, (SPC5_ME_PCTL_RUN(0) | SPC5_ME_PCTL_LP(0)));
  }
#endif
#if (SPC5_SARADC_USE_SARADC12_2 == TRUE)
  if (&SARADC12D3 == saradcp) {
#if (SPC5_SARADC_USE_DMA == TRUE) || defined(__DOXYGEN__)
    if (saradcp->config->dma_enabled == TRUE) {
      edmaChannelRelease(&saradc12_2_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_SARADC12_2_PCTL, (SPC5_ME_PCTL_RUN(0) | SPC5_ME_PCTL_LP(0)));
  }
#endif
#if (SPC5_SARADC_USE_SARADC12_3 == TRUE)
  if (&SARADC12D4 == saradcp) {
#if (SPC5_SARADC_USE_DMA == TRUE) || defined(__DOXYGEN__)
    if (saradcp->config->dma_enabled == TRUE) {
      edmaChannelRelease(&saradc12_3_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_SARADC12_3_PCTL, (SPC5_ME_PCTL_RUN(0) | SPC5_ME_PCTL_LP(0)));
  }
#endif
#if (SPC5_SARADC_USE_SARADC12_4 == TRUE)
  if (&SARADC12D5 == saradcp) {
#if (SPC5_SARADC_USE_DMA == TRUE) || defined(__DOXYGEN__)
    if (saradcp->config->dma_enabled == TRUE) {
      edmaChannelRelease(&saradc12_4_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_SARADC12_4_PCTL, (SPC5_ME_PCTL_RUN(0) | SPC5_ME_PCTL_LP(0)));
  }
#endif
#if (SPC5_SARADC_USE_SARADC12_5 == TRUE)
  if (&SARADC12D6 == saradcp) {
#if (SPC5_SARADC_USE_DMA == TRUE) || defined(__DOXYGEN__)
    if (saradcp->config->dma_enabled == TRUE) {
      edmaChannelRelease(&saradc12_5_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_SARADC12_5_PCTL, (SPC5_ME_PCTL_RUN(0) | SPC5_ME_PCTL_LP(0)));
  }
#endif
#if (SPC5_SARADC_USE_SARADC12_6 == TRUE)
  if (&SARADC12D7 == saradcp) {
#if (SPC5_SARADC_USE_DMA == TRUE) || defined(__DOXYGEN__)
    if (saradcp->config->dma_enabled == TRUE) {
      edmaChannelRelease(&saradc12_6_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_SARADC12_6_PCTL, (SPC5_ME_PCTL_RUN(0) | SPC5_ME_PCTL_LP(0)));
  }
#endif
#if (SPC5_SARADC_USE_SARADC12_SV == TRUE)
  if (&SARADC12DSV == saradcp) {
#if (SPC5_SARADC_USE_DMA == TRUE) || defined(__DOXYGEN__)
    if (saradcp->config->dma_enabled == TRUE) {
      edmaChannelRelease(&saradc12_sv_dma_config);
    }
#endif
    if (SARADC12DSV.noofinstances == 1U){
      SPCSetPeripheralClockMode(SPC5_SARADC12_SV_PCTL, (SPC5_ME_PCTL_RUN(0) | SPC5_ME_PCTL_LP(0)));
      SARADC12DSV.noofinstances = 0U;
    }
    else{
    	SARADC12DSV.noofinstances--;
    }
  }
#endif
#if (SPC5_SARADC_USE_SARADC12_SV_1 == TRUE)
  if (&SARADC12DSV1 == saradcp) {
    SPCSetPeripheralClockMode(SPC5_SARADC12_SV_1_PCTL, (SPC5_ME_PCTL_RUN(0) | SPC5_ME_PCTL_LP(0)));
  }
#endif
#if (SPC5_SARADC_USE_SARADC10_0 == TRUE)
  if (&SARADC10D1 == saradcp) {
#if (SPC5_SARADC_USE_DMA == TRUE) || defined(__DOXYGEN__)
    if (saradcp->config->dma_enabled == TRUE) {
      edmaChannelRelease(&saradc10_0_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_SARADC10_0_PCTL, (SPC5_ME_PCTL_RUN(0) | SPC5_ME_PCTL_LP(0)));
  }
#endif
#if (SPC5_SARADC_USE_SARADC10_1 == TRUE)
  if (&SARADC10D2 == saradcp) {
#if (SPC5_SARADC_USE_DMA == TRUE) || defined(__DOXYGEN__)
    if (saradcp->config->dma_enabled == TRUE) {
      edmaChannelRelease(&saradc10_1_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_SARADC10_1_PCTL, (SPC5_ME_PCTL_RUN(0) | SPC5_ME_PCTL_LP(0)));
  }
#endif
#if (SPC5_SARADC_USE_SARADC10_STANDBY == TRUE)
  if (&SARADC10DSB == saradcp) {
    SPCSetPeripheralClockMode(SPC5_SARADC10_STANDBY_PCTL, (SPC5_ME_PCTL_RUN(0) | SPC5_ME_PCTL_LP(0)));
  }
#endif
}

/**
 * @brief   Starts an SARADC conversion.
 *
 * @param[in] saradcp    pointer to the @p SARADCDriver object
 *
 * @api
 */
void saradc_lld_start_conversion(SARADCDriver *saradcp) {

  /* Start Conversion */
  saradcp->saradc->MCR.B.NSTART = 1U;

}

/**
 * @brief   Stops an ongoing conversion.
 *
 * @param[in] saradcp    pointer to the @p SARADCDriver object
 *
 * @api
 */
void saradc_lld_stop_conversion(SARADCDriver *saradcp) {

  /* stop conversion */
  saradcp->saradc->MCR.B.NSTART = 0U;

}

/**
 * @brief   Read converted value of a Channel
 *
 * @param[in] saradcp      pointer to the @p SARADCDriver object
 * @param[in] channel      channel to read the value of
 *
 * @return                 converted value, or SARADC_INVALID_DATA on error
 *
 * @sa
 * SARADC_INVALID_DATA
 *
 * @api
 */
uint16_t saradc_lld_readchannel(SARADCDriver *saradcp, uint16_t channel) {

#if (SPC5_SARADC_BYPASS_VALID_CHECK == FALSE)
  if (channel < 96U) {
    /* return the value only if not yet read. If valid flag is not 1
	   then return SARADC_INVALID_DATA. */
    if (saradcp->saradc->ICDR[channel].B.VALID == 0U) {
      return SARADC_INVALID_DATA;
    }
  }
  else {
    if (saradcp->saradc->TCDR[channel - 96U].B.VALID == 0U) {
      return SARADC_INVALID_DATA;
    }
  }
#endif
  /* channel to read is an internal channel */
  if (channel < 96U) {
    return (uint16_t)saradcp->saradc->ICDR[channel].B.CDATA;
  }
  /* channel to read is an external channel */
  else {
    return (uint16_t)saradcp->saradc->TCDR[channel - 96U].B.CDATA;
  }

}

#if (SPC5_SARADC_USE_DMA == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   DMA events service routine.
 *
 * @param[in] channel   the channel number
 * @param[in] p         Pointer to SARADC driver
 *
 * @isr
 */
static void saradc_lld_serve_dma_irq(edma_channel_t channel, void *p) {

  /*Casting of void pointer needful for using the same DMA driver
   in different drivers. */
  /*lint -e9087 */
  SARADCDriver* saradcp = (SARADCDriver *)p;
  /*lint +e9087 */

  (void)channel;
  
  /* Start DMA channel.*/
  edmaChannelStart(saradcp->dma_channel);

  /* Call the related callback.*/
  saradcp->config->dmaconvcallback(saradcp, saradcp->dma_buffer, SPC5_SARADC_DMA_BUFFER_LENGHT);
}

/**
 * @brief   Shared ISR for DMA error events.
 *
 * @param[in] channel   the channel number
 * @param[in] p         parameter for the registered function
 * @param[in] esr       content of the ESR register
 *
 * @isr
 */
static void saradc_lld_dma_error_irq(edma_channel_t channel, void *p, uint32_t esr) {
  /*Casting of void pointer needful for using the same DMA driver
   in different drivers. */
  /*lint -e9087 */
  SARADCDriver* saradcp = (SARADCDriver *)p;
  /*lint +e9087 */

  (void)channel;
  (void)esr;

  /* Clearing channels state.*/
  edmaChannelStop(saradcp->dma_channel);

  SPC5_SARADC_DMA_ERROR_HOOK(saradcp);
}

/**
 * @brief   Configures Channels for Conversion when DMA mode used
 *
 * @param[in] saradcp      pointer to the @p SARADCDriver object
 * @param[in] config       pointer to the @p SARADCDconfig object
 *
 * @notapi
 */
static void saradc_lld_configure_channels_dma(SARADCDriver *saradcp) {
  uint8_t i;
  vuint8_t *src;

  /* configure channels if DMA is used (only INTERNAL channels supported)*/
  for (i = 0; i < saradcp->config->dmacfg.noofchannels; i++) {
    uint8_t reg_index;
    uint8_t bit_index;
    reg_index = (uint8_t)((saradcp->config->dmacfg.number + i) / 32U);
    bit_index = (uint8_t)((saradcp->config->dmacfg.number + i) % 32U);
    /* Enable Normal Conversion Mode for Channel */
    saradcp->saradc->ICNCMR[reg_index].R |= (1UL << bit_index);
    /* Only last saradc channel must be associated to DMA  */
    if (i == (saradcp->config->dmacfg.noofchannels - 1U)) {
      saradcp->saradc->ICDSR[reg_index].R |= (1UL << bit_index);
    }
    /* Channel Precharge Configuration */
    saradcp->saradc->ICDR[saradcp->config->dmacfg.number + i].B.PCE = saradcp->config->dmacfg.precharge;
    /* Channel CTR register Selection */
    saradcp->saradc->ICDR[saradcp->config->dmacfg.number + i].B.CTSEL = saradcp->config->dmacfg.ctr;
    /* Channel voltage reference Selection (if supported) */
#if (SPC5_SARADC_HAS_ALT_REFERENCE == TRUE)
    saradcp->saradc->ICDR[saradcp->config->dmacfg.number + i].B.REFSEL = saradcp->config->dmacfg.reference;
#endif
  }

  src = ((vuint8_t *)&saradcp->saradc->ICDR[saradcp->config->dmacfg.number].R) + 2U;

  /* Setup DMA TCD parameters.*/
  edmaChannelSetup(
      /* channel */
      saradcp->dma_channel,
      /* src */
      src,
      /* dst */
      saradcp->dma_buffer,
      /* soff */
      0x04,
      /* doff, advance by 2.    */
      0x02,
      /* ssize, 16 bits transfers. */
      0x01,
      /* dsize, 16 bits transfers. */
      0x01,
      /* mloff and nbytes */
      (0xBFFFFFFFUL) & ((CPL2((uint32_t)saradcp->config->dmacfg.noofchannels * 4U) << 10U) | (2U * (uint32_t)saradcp->config->dmacfg.noofchannels)),
      /* iter */
      saradcp->config->dmacfg.noofconversions,
      /* slast =  (-soff) */
      CPL2((uint32_t)saradcp->config->dmacfg.noofchannels * 4U),
      /* dlast  */
      CPL2((uint32_t)saradcp->config->dmacfg.noofchannels * (uint32_t)saradcp->config->dmacfg.noofconversions *2U),
      /* mode */
      EDMA_TCD_MODE_DREQ | EDMA_TCD_MODE_INT_END);

  /* Enable DMA */
  saradcp->saradc->DMAE.B.DMAEN = 1U;

  /* Start DMA channel.*/
  edmaChannelStart(saradcp->dma_channel);
}
#endif

#if (SPC5_SARADC_MAX_NUMOFCHANNELS > 0U) || defined(__DOXYGEN__)
/**
 * @brief   Configures Channels for Conversion when DMA mode is not used
 *
 * @param[in] saradcp      pointer to the @p SARADCDriver object
 * @param[in] config	   pointer to the @p SARADCDconfig object
 *
 * @notapi
 */
static void saradc_lld_configure_channels(SARADCDriver *saradcp) {
  uint8_t i;

  for (i = 0; i < saradcp->config->numofchannels; i++) {
    if (saradcp->config->ch[i].type == SARADC_CHANNEL_INTERNAL) {
      uint8_t reg_index;
      uint8_t bit_index;
      reg_index = (uint8_t)(saradcp->config->ch[i].number / 32U);
      bit_index = (uint8_t)(saradcp->config->ch[i].number % 32U);
      /* Enable Interrupt for channel (only if callback is enable */
      if (saradcp->config->convcallback != NULL) {
        saradcp->saradc->ICIMR[reg_index].R |= (1UL << bit_index);
      }
      /* Enable Normal Conversion Mode for Channel */
      saradcp->saradc->ICNCMR[reg_index].R |= (1UL << bit_index);
      /* Channel Precharge Configuration */
      saradcp->saradc->ICDR[saradcp->config->ch[i].number].B.PCE = saradcp->config->ch[i].precharge;
      /* Channel CTR register Selection */
      saradcp->saradc->ICDR[saradcp->config->ch[i].number].B.CTSEL = saradcp->config->ch[i].ctr;
      /* Channel voltage reference Selection (if supported) */
      /* Select threshold register and activate if enabled */
      if (saradcp->config->ch[i].thr != SARADC_WATCHDOG_REGISTER_NONE) {
        reg_index = (uint8_t)(saradcp->config->ch[i].number / 8U);
        bit_index = (uint8_t)(saradcp->config->ch[i].number % 8U) * 4U;
        saradcp->saradc->ICWSELR[reg_index].R |= (uint32_t)saradcp->config->ch[i].thr << (bit_index);
        reg_index = (uint8_t)(saradcp->config->ch[i].number / 32U);
        bit_index = (uint8_t)(saradcp->config->ch[i].number % 32U);
        saradcp->saradc->ICWENR[reg_index].R |= 1UL << bit_index;
      }
#if (SPC5_SARADC_HAS_ALT_REFERENCE == TRUE)
      saradcp->saradc->ICDR[saradcp->config->ch[i].number].B.REFSEL = saradcp->config->ch[i].reference;
#endif
    }
    else if (saradcp->config->ch[i].type == SARADC_CHANNEL_TEST) {
      uint8_t reg_index;
      uint8_t bit_index;
      bit_index = (uint8_t)(saradcp->config->ch[i].number - 96U);
      /* Enable Interrupt for channel (only if callback is enable */
      if (saradcp->config->convcallback != NULL) {
        saradcp->saradc->TCIMR.R |= (1UL << bit_index);
      }
      /* Enable Normal Conversion Mode for Channel */
      saradcp->saradc->TCNCMR.R |= (1UL << bit_index);
      /* Channel Precharge Configuration */
      saradcp->saradc->TCDR[saradcp->config->ch[i].number - 96U].B.PCE = saradcp->config->ch[i].precharge;
      /* Channel CTR register Selection */
      saradcp->saradc->TCDR[saradcp->config->ch[i].number - 96U].B.CTSEL = saradcp->config->ch[i].ctr;
      /* Channel voltage reference Selection (if supported) */
#if (SPC5_SARADC_HAS_ALT_REFERENCE == TRUE)
      saradcp->saradc->TCDR[saradcp->config->ch[i].number - 96U].B.REFSEL = saradcp->config->ch[i].reference;
#endif
      /* Select threshold register and activate if enabled */
      if (saradcp->config->ch[i].thr != SARADC_WATCHDOG_REGISTER_NONE) {
        uint8_t testch;
        testch = (uint8_t)(saradcp->config->ch[i].number - 96U);
        reg_index = (uint8_t)(testch / 8U);
        bit_index = (uint8_t)(testch % 8U) * 4U;
        saradcp->saradc->TCWSELR[reg_index].R |= (uint32_t)saradcp->config->ch[i].thr << (bit_index);
        bit_index = testch;
        saradcp->saradc->TCWENR.R |= 1UL << bit_index;
      }
    }
    else {
      /* MISRA */
    }
  }
}
#endif

#endif /* LLD_USE_SARADC */
/** @} */
