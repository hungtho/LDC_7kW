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
 * @file    stm_lld.c
 * @brief   SPC5xx STM low level driver code.
 *
 * @addtogroup STM
 * @{
 */

#include "stm_lld.h"

#if (LLD_USE_STM == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   STM2 driver identifier.
 */
#if (SPC5_STM_USE_STM0 == TRUE) || defined(__DOXYGEN__)
STMDriver STMD1;
#endif

/**
 * @brief   STM1 driver identifier.
 */
#if (SPC5_STM_USE_STM1 == TRUE) || defined(__DOXYGEN__)
STMDriver STMD2;
#endif

/**
 * @brief   STM2 driver identifier.
 */
#if (SPC5_STM_USE_STM2 == TRUE) || defined(__DOXYGEN__)
STMDriver STMD3;
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
#if (SPC5_STM_USE_STM0 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_STM0_CH0_ENABLED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   STM0 CHANNEL 0 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_STM0_CH0_HANDLER) {
  IRQ_PROLOGUE();
  osalEnterCriticalFromISR();

  /* call related callback */
  if (STMD1.stm_ch_config[0].callback != NULL){
    STMD1.stm_ch_config[0].callback();
  }

  STMD1.stm_tagp->CH[0].CIR.B.CIF = STM_CCR_CIF_CLEAR ;

  osalExitCriticalFromISR();
  IRQ_EPILOGUE();
}
#endif

#if (SPC5_STM0_CH1_CH3_SHARED_INT == TRUE) || defined(__DOXYGEN__)
#if (SPC5_STM0_CH1_ENABLED == TRUE) || (SPC5_STM0_CH2_ENABLED == TRUE) || (SPC5_STM0_CH3_ENABLED == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_STM0_CH1_CH3_HANDLER) {
  uint8_t i;
  IRQ_PROLOGUE();
  osalEnterCriticalFromISR();
  /*check interrupt flags for channels from 1 to 3 and call related callbacks*/
  for (i=1U;i<=3U;i++){
    if (STMD1.stm_tagp->CH[i].CIR.B.CIF == 1U){
      /* call related callback */
      if (STMD1.stm_ch_config[i].callback != NULL){
        STMD1.stm_ch_config[i].callback();
      }
      STMD1.stm_tagp->CH[i].CIR.B.CIF = STM_CCR_CIF_CLEAR ;
    }
  }
  osalExitCriticalFromISR();
  IRQ_EPILOGUE();
}
#endif

#else

#if (SPC5_STM0_CH1_ENABLED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   STM0 CHANNEL 1 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_STM0_CH1_HANDLER) {
  IRQ_PROLOGUE();
  osalEnterCriticalFromISR();

  /* call related callback */
  if (STMD1.stm_ch_config[1].callback != NULL){
    STMD1.stm_ch_config[1].callback();
  }

  STMD1.stm_tagp->CH[1].CIR.B.CIF = STM_CCR_CIF_CLEAR ;

  osalExitCriticalFromISR();
  IRQ_EPILOGUE();
}
#endif

#if (SPC5_STM0_CH2_ENABLED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   STM0 CHANNEL 2 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_STM0_CH2_HANDLER) {
  IRQ_PROLOGUE();
  osalEnterCriticalFromISR();

  /* call related callback */
  if (STMD1.stm_ch_config[2].callback != NULL){
    STMD1.stm_ch_config[2].callback();
  }

  STMD1.stm_tagp->CH[2].CIR.B.CIF = STM_CCR_CIF_CLEAR ;

  osalExitCriticalFromISR();
  IRQ_EPILOGUE();
}
#endif

#if (SPC5_STM0_CH3_ENABLED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   STM0 CHANNEL 3 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_STM0_CH3_HANDLER) {
  IRQ_PROLOGUE();
  osalEnterCriticalFromISR();

  /* call related callback */
  if (STMD1.stm_ch_config[3].callback != NULL){
    STMD1.stm_ch_config[3].callback();
  }

  STMD1.stm_tagp->CH[3].CIR.B.CIF = STM_CCR_CIF_CLEAR ;

  osalExitCriticalFromISR();
  IRQ_EPILOGUE();
}
#endif

#endif
#endif

#if (SPC5_STM_USE_STM1 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_STM1_CH0_ENABLED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   STM1 CHANNEL 0 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_STM1_CH0_HANDLER) {
  IRQ_PROLOGUE();
  osalEnterCriticalFromISR();

  /* call related callback */
  if (STMD2.stm_ch_config[0].callback != NULL){
    STMD2.stm_ch_config[0].callback();
  }

  STMD2.stm_tagp->CH[0].CIR.B.CIF = STM_CCR_CIF_CLEAR ;

  osalExitCriticalFromISR();
  IRQ_EPILOGUE();
}
#endif

#if (SPC5_STM1_CH1_ENABLED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   STM1 CHANNEL 1 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_STM1_CH1_HANDLER) {
  IRQ_PROLOGUE();
  osalEnterCriticalFromISR();

  /* call related callback */
  if (STMD2.stm_ch_config[1].callback != NULL){
    STMD2.stm_ch_config[1].callback();
  }

  STMD2.stm_tagp->CH[1].CIR.B.CIF = STM_CCR_CIF_CLEAR ;

  osalExitCriticalFromISR();
  IRQ_EPILOGUE();
}
#endif

#if (SPC5_STM1_CH2_ENABLED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   STM1 CHANNEL 2 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_STM1_CH2_HANDLER) {
  IRQ_PROLOGUE();
  osalEnterCriticalFromISR();

  /* call related callback */
  if (STMD2.stm_ch_config[2].callback != NULL){
    STMD2.stm_ch_config[2].callback();
  }

  STMD2.stm_tagp->CH[2].CIR.B.CIF = STM_CCR_CIF_CLEAR ;

  osalExitCriticalFromISR();
  IRQ_EPILOGUE();
}
#endif
#if (SPC5_STM1_CH3_ENABLED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   STM1 CHANNEL 3 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_STM1_CH3_HANDLER) {
  IRQ_PROLOGUE();
  osalEnterCriticalFromISR();

  /* call related callback */
  if (STMD2.stm_ch_config[3].callback != NULL){
    STMD2.stm_ch_config[3].callback();
  }

  STMD2.stm_tagp->CH[3].CIR.B.CIF = STM_CCR_CIF_CLEAR ;

  osalExitCriticalFromISR();
  IRQ_EPILOGUE();
}
#endif
#endif

#if (SPC5_STM_USE_STM2 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_STM2_CH0_ENABLED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   STM2 CHANNEL 0 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_STM2_CH0_HANDLER) {
  IRQ_PROLOGUE();
  osalEnterCriticalFromISR();

  /* call related callback */
  if (STMD3.stm_ch_config[0].callback != NULL){
    STMD3.stm_ch_config[0].callback();
  }

  STMD3.stm_tagp->CH[0].CIR.B.CIF = STM_CCR_CIF_CLEAR ;

  osalExitCriticalFromISR();
  IRQ_EPILOGUE();
}
#endif

#if (SPC5_STM2_CH1_ENABLED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   STM2 CHANNEL 1 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_STM2_CH1_HANDLER) {
  IRQ_PROLOGUE();
  osalEnterCriticalFromISR();

  /* call related callback */
  if (STMD3.stm_ch_config[1].callback != NULL){
    STMD3.stm_ch_config[1].callback();
  }

  STMD3.stm_tagp->CH[1].CIR.B.CIF = STM_CCR_CIF_CLEAR ;

  osalExitCriticalFromISR();
  IRQ_EPILOGUE();
}
#endif

#if (SPC5_STM2_CH2_ENABLED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   STM2 CHANNEL 2 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_STM2_CH2_HANDLER) {
  IRQ_PROLOGUE();
  osalEnterCriticalFromISR();

  /* call related callback */
  if (STMD3.stm_ch_config[2].callback != NULL){
    STMD3.stm_ch_config[2].callback();
  }

  STMD3.stm_tagp->CH[2].CIR.B.CIF = STM_CCR_CIF_CLEAR ;

  osalExitCriticalFromISR();
  IRQ_EPILOGUE();
}
#endif
#if (SPC5_STM2_CH3_ENABLED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   STM2 CHANNEL 3 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_STM2_CH3_HANDLER) {
  IRQ_PROLOGUE();
  osalEnterCriticalFromISR();

  /* call related callback */
  if (STMD3.stm_ch_config[3].callback != NULL){
    STMD3.stm_ch_config[3].callback();
  }

  STMD3.stm_tagp->CH[3].CIR.B.CIF = STM_CCR_CIF_CLEAR ;

  osalExitCriticalFromISR();
  IRQ_EPILOGUE();
}
#endif
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level stm driver initialization.
 *
 * @init
 */
void stm_lld_init(void) {
#if (SPC5_STM_USE_STM0 == TRUE) || defined(__DOXYGEN__)
  STMD1.stm_ch_config = NULL;
#if (SPC5_STM0_CH0_ENABLED == TRUE)
  INTC_PSR(SPC5_STM0_CH0_INT_NUMBER) = SPC5_STM0_CH0_IRQ_PRIORITY;
#endif
#if (SPC5_STM0_CH1_CH3_SHARED_INT == TRUE)
#if (SPC5_STM0_CH1_ENABLED == TRUE) || (SPC5_STM0_CH2_ENABLED == TRUE) || (SPC5_STM0_CH3_ENABLED == TRUE)
  INTC_PSR(SPC5_STM0_CH1_CH3_INT_NUMBER) = SPC5_STM0_CH1_CH3_IRQ_PRIORITY;
#endif
#else
#if (SPC5_STM0_CH1_ENABLED == TRUE)
 INTC_PSR(SPC5_STM0_CH1_INT_NUMBER) = SPC5_STM0_CH1_IRQ_PRIORITY;
#endif
#if (SPC5_STM0_CH2_ENABLED == TRUE)
 INTC_PSR(SPC5_STM0_CH2_INT_NUMBER) = SPC5_STM0_CH2_IRQ_PRIORITY;
#endif
#if (SPC5_STM0_CH3_ENABLED == TRUE)
 INTC_PSR(SPC5_STM0_CH3_INT_NUMBER) = SPC5_STM0_CH3_IRQ_PRIORITY;
#endif
#endif
#endif

#if (SPC5_STM_USE_STM1 == TRUE) || defined(__DOXYGEN__)
  STMD2.stm_ch_config = NULL;
#if (SPC5_STM1_CH0_ENABLED == TRUE)
  INTC_PSR(SPC5_STM1_CH0_INT_NUMBER) = SPC5_STM1_CH0_IRQ_PRIORITY;
#endif
#if (SPC5_STM1_CH1_ENABLED == TRUE)
 INTC_PSR(SPC5_STM1_CH1_INT_NUMBER) = SPC5_STM1_CH1_IRQ_PRIORITY;
#endif
#if (SPC5_STM1_CH2_ENABLED == TRUE)
 INTC_PSR(SPC5_STM1_CH2_INT_NUMBER) = SPC5_STM1_CH2_IRQ_PRIORITY;
#endif
#if (SPC5_STM1_CH3_ENABLED == TRUE)
 INTC_PSR(SPC5_STM1_CH3_INT_NUMBER) = SPC5_STM1_CH3_IRQ_PRIORITY;
#endif
#endif

#if (SPC5_STM_USE_STM2 == TRUE) || defined(__DOXYGEN__)
  STMD3.stm_ch_config = NULL;
#if (SPC5_STM2_CH0_ENABLED == TRUE)
  INTC_PSR(SPC5_STM2_CH0_INT_NUMBER) = SPC5_STM2_CH0_IRQ_PRIORITY;
#endif
#if (SPC5_STM2_CH1_ENABLED == TRUE)
 INTC_PSR(SPC5_STM2_CH1_INT_NUMBER) = SPC5_STM2_CH1_IRQ_PRIORITY;
#endif
#if (SPC5_STM2_CH2_ENABLED == TRUE)
 INTC_PSR(SPC5_STM2_CH2_INT_NUMBER) = SPC5_STM2_CH2_IRQ_PRIORITY;
#endif
#if (SPC5_STM2_CH3_ENABLED == TRUE)
 INTC_PSR(SPC5_STM2_CH3_INT_NUMBER) = SPC5_STM2_CH3_IRQ_PRIORITY;
#endif
#endif
}

/**
 * @brief   Activate and configure STM peripheral
 *
 * @param[in] stmd      pointer to the @p STMDriver object
 * @param[in] config	pointer to the  @p STM_CH_Config object
 *
 * @api
 */
void stm_lld_start (STMDriver *stmd, STM_CH_Config *config){
  stmd->stm_ch_config = config;
#if (SPC5_STM_USE_STM0 == TRUE)
  if (&STMD1 == stmd) {
    stmd->prescaler = SPC5_STM0_SYSCLOCK_PRE;
    stmd->stm_tagp  = &SPC5_STM0;
    stmd->noofchannels = SPC5_STM0_CHANNELS;
  }
#endif
#if (SPC5_STM_USE_STM1 == TRUE)
  if (&STMD2 == stmd) {
    stmd->prescaler = SPC5_STM1_SYSCLOCK_PRE;
    stmd->stm_tagp = &SPC5_STM1;
    stmd->noofchannels = SPC5_STM1_CHANNELS;
  }
#endif
#if (SPC5_STM_USE_STM2 == TRUE)
  if (&STMD3 == stmd) {
    stmd->prescaler = SPC5_STM2_SYSCLOCK_PRE;
    stmd->stm_tagp = &SPC5_STM2;
    stmd->noofchannels = SPC5_STM2_CHANNELS;
  }
#endif
}

/**
 * @brief   Enable Global Counter for STMD driver and activate channels 
 *
 * @param[in] stmd      pointer to the @p STMDriver object
 *
 * @api
 */
void stm_lld_enable (STMDriver *stmd){
  uint8_t i;

  /* set the prescaler */
  stmd->stm_tagp->CR.R = (stmd->prescaler-1U) << 8 ;
  /* set compare register and enable channels */
  for (i=0; i<stmd->noofchannels;i++){
    if (stmd->stm_ch_config[i].enabled == TRUE){
      /* set compare register */
      stmd->stm_tagp->CH[i].CMP.B.CMP = stmd->stm_ch_config[i].counter;
      /* clear  interrupt flag */
      stmd->stm_tagp->CH[i].CIR.B.CIF = STM_CCR_CIF_CLEAR;
	  /* enable channel */
      stmd->stm_tagp->CH[i].CCR.B.CEN = STM_CIR_CEN_ENABLE;
    }
  }
  /* reset counter */
  stmd->stm_tagp->CNT.R     = 0x00000000U;
  /* Freeze in debug */
  stmd->stm_tagp->CR.B.FRZ  = STM_CR_FRZ_ENABLE;
  /*enable STM */
  stmd->stm_tagp->CR.B.TEN  = STM_CR_TEN_ENABLE;
}

/**
 * @brief   Disable Global Counter for STMD driver and deactivate channels
 *
 * @param[in] stmd      pointer to the @p STMDriver object
 *
 * @api
 */
void stm_lld_disable (STMDriver *stmd){
  uint8_t i;

  /* set compare register and enable channels */
  for (i=0; i<stmd->noofchannels;i++){
    if (stmd->stm_ch_config[i].enabled == TRUE){
      /* set compare registers to 0 */
      stmd->stm_tagp->CH[i].CMP.B.CMP = 0x0000000U;
      /* clear  interrupt flag */
      stmd->stm_tagp->CH[i].CIR.B.CIF = STM_CCR_CIF_CLEAR;
	  /* disable channel */
      stmd->stm_tagp->CH[i].CCR.B.CEN = STM_CIR_CEN_DISABLE;
    }
  }

  /*disable STM */
  stmd->stm_tagp->CR.B.TEN  = STM_CR_TEN_DISABLE;
}


/**
 * @brief   STOP the STM Driver
 *
 * @param[in] stmd      pointer to the @p STMDriver object
 *
 * @api
 */
void stm_lld_stop (STMDriver *stmd){
  if (stmd->stm_tagp->CR.B.TEN  ==  STM_CR_TEN_ENABLE){
    stm_lld_disable(stmd);
  }
  stmd->stm_ch_config = NULL;
}

/**
 * @brief   Counter Reset of STM Driver
 *
 * @param[in] stmd      pointer to the @p STMDriver object
 *
 * @api
 */
void stm_lld_resetcounter (STMDriver *stmd){
  stm_lld_setcounter (stmd,0x00000000U);
}

/**
 * @brief   Get Counter value of STM Driver
 *
 * @param[in] stmd      pointer to the @p STMDriver object
 * @return              current counter value
 *
 * @api
 */
uint32_t stm_lld_getcounter (STMDriver *stmd){
  return (stmd->stm_tagp->CNT.R);
}

/**
 * @brief   Set counter value of STM Driver
 *
 * @param[in] stmd      pointer to the @p STMDriver object
 * @param[in] value     value of counter to be set
 * @api
 */
void stm_lld_setcounter (STMDriver *stmd,uint32_t value){

  /*disable counter */
  stmd->stm_tagp->CR.B.TEN  = STM_CR_TEN_DISABLE;

  /* reset counter */
  stmd->stm_tagp->CNT.R     = value;

  /*enable counter */
  stmd->stm_tagp->CR.B.TEN  = STM_CR_TEN_ENABLE;
}

#endif /* LLD_USE_STM */

/** @} */
