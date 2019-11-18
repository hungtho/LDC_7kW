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
 * @file    pit_lld.c
 * @brief   SPC5xx PIT low level driver code.
 *
 * @addtogroup PIT
 * @{
 */

#include "pit_lld.h"

#if (LLD_USE_PIT == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   PIT driver identifier.
 */
PITDriver PITD;

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/
	
/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if (PIT_CHANNEL_CH0_ENABLED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   PIT CHANNEL 0 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC_PIT_CH0_HANDLER) {
  IRQ_PROLOGUE();

  osalEnterCriticalFromISR();

  /* call the related callback */
  if (PITD.pit_ch_config[0].callback != NULL){
    PITD.pit_ch_config[0].callback();
  }
  
  PITD.pit_tagp->CH[0].TFLG.R = 1;

  osalExitCriticalFromISR();    /* Resets the PIT channel 0 IRQ flag.*/

  IRQ_EPILOGUE();
}
#endif

#if (PIT_CHANNEL_CH1_ENABLED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   PIT CHANNEL 1 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC_PIT_CH1_HANDLER) {
  IRQ_PROLOGUE();

  osalEnterCriticalFromISR();

  /* call the related callback */
  if (PITD.pit_ch_config[1].callback != NULL){
    PITD.pit_ch_config[1].callback();
  }
  
  PITD.pit_tagp->CH[1].TFLG.R = 1;

  osalExitCriticalFromISR();    /* Resets the PIT channel 1 IRQ flag.*/

  IRQ_EPILOGUE();
}
#endif

#if (PIT_CHANNEL_CH2_ENABLED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   PIT CHANNEL 2 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC_PIT_CH2_HANDLER) {
  IRQ_PROLOGUE();

  osalEnterCriticalFromISR();

  /* call the related callback */
  if (PITD.pit_ch_config[2].callback != NULL){
    PITD.pit_ch_config[2].callback();
  }
  
  PITD.pit_tagp->CH[2].TFLG.R = 1;

  osalExitCriticalFromISR();    /* Resets the PIT channel 2 IRQ flag.*/

  IRQ_EPILOGUE();
}
#endif

#if (PIT_CHANNEL_CH3_ENABLED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   PIT CHANNEL 3 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC_PIT_CH3_HANDLER) {
  IRQ_PROLOGUE();

  osalEnterCriticalFromISR();

  /* call the related callback */
  if (PITD.pit_ch_config[3].callback != NULL){
    PITD.pit_ch_config[3].callback();
  }
  
  PITD.pit_tagp->CH[3].TFLG.R = 1;

  osalExitCriticalFromISR();    /* Resets the PIT channel 3 IRQ flag.*/

  IRQ_EPILOGUE();
}
#endif

#if (PIT_CHANNEL_CH4_ENABLED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   PIT CHANNEL 4 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC_PIT_CH4_HANDLER) {
  IRQ_PROLOGUE();

  osalEnterCriticalFromISR();

  /* call the related callback */
  if (PITD.pit_ch_config[4].callback != NULL){
    PITD.pit_ch_config[4].callback();
  }
  
  PITD.pit_tagp->CH[4].TFLG.R = 1;

  osalExitCriticalFromISR();    /* Resets the PIT channel 4 IRQ flag.*/

  IRQ_EPILOGUE();
}
#endif

#if (PIT_CHANNEL_CH5_ENABLED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   PIT CHANNEL 5 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC_PIT_CH5_HANDLER) {
  IRQ_PROLOGUE();

  osalEnterCriticalFromISR();

  /* call the related callback */
  if (PITD.pit_ch_config[5].callback != NULL){
    PITD.pit_ch_config[5].callback();
  }
  
  PITD.pit_tagp->CH[5].TFLG.R = 1;

  osalExitCriticalFromISR();    /* Resets the PIT channel 5 IRQ flag.*/

  IRQ_EPILOGUE();
}
#endif

#if (PIT_CHANNEL_CH6_ENABLED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   PIT CHANNEL 6 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC_PIT_CH6_HANDLER) {
  IRQ_PROLOGUE();

  osalEnterCriticalFromISR();

  /* call the related callback */
  if (PITD.pit_ch_config[6].callback != NULL){
    PITD.pit_ch_config[6].callback();
  }
  
  PITD.pit_tagp->CH[6].TFLG.R = 1;

  osalExitCriticalFromISR();    /* Resets the PIT channel 6 IRQ flag.*/

  IRQ_EPILOGUE();
}
#endif

#if (PIT_CHANNEL_CH7_ENABLED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   PIT CHANNEL 7 interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC_PIT_CH7_HANDLER) {
  IRQ_PROLOGUE();

  osalEnterCriticalFromISR();

  /* call the related callback */
  if (PITD.pit_ch_config[7].callback != NULL){
    PITD.pit_ch_config[7].callback();
  }
  
  PITD.pit_tagp->CH[7].TFLG.R = 1;
  
  osalExitCriticalFromISR();    /* Resets the PIT channel 7 IRQ flag.*/

  IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level pit driver initialization.
 *
 * @init
 */
void pit_lld_init(void) {

PITD.pit_tagp = &SPC5_PIT;

#if (PIT_CHANNEL_CH0_ENABLED == TRUE)
  INTC_PSR(SPC_PIT_CH0_INT_NUMBER) = SPC5_PIT_CH0_IRQ_PRIO;
#endif
#if (PIT_CHANNEL_CH1_ENABLED == TRUE)
  INTC_PSR(SPC_PIT_CH1_INT_NUMBER) = SPC5_PIT_CH1_IRQ_PRIO;
#endif
#if (PIT_CHANNEL_CH2_ENABLED == TRUE)
  INTC_PSR(SPC_PIT_CH2_INT_NUMBER) = SPC5_PIT_CH2_IRQ_PRIO;
#endif
#if (PIT_CHANNEL_CH3_ENABLED == TRUE)
  INTC_PSR(SPC_PIT_CH3_INT_NUMBER) = SPC5_PIT_CH3_IRQ_PRIO;
#endif
#if (PIT_CHANNEL_CH4_ENABLED == TRUE)
  INTC_PSR(SPC_PIT_CH4_INT_NUMBER) = SPC5_PIT_CH4_IRQ_PRIO;
#endif
#if (PIT_CHANNEL_CH5_ENABLED == TRUE)
  INTC_PSR(SPC_PIT_CH5_INT_NUMBER) = SPC5_PIT_CH5_IRQ_PRIO;
#endif
#if (PIT_CHANNEL_CH6_ENABLED == TRUE)
  INTC_PSR(SPC_PIT_CH6_INT_NUMBER) = SPC5_PIT_CH6_IRQ_PRIO;
#endif
#if (PIT_CHANNEL_CH7_ENABLED == TRUE)
  INTC_PSR(SPC_PIT_CH7_INT_NUMBER) = SPC5_PIT_CH7_IRQ_PRIO;
#endif
}

/**
 * @brief   Activate and configure PIT peripheral
 *
 * @param[in] pitd      pointer to the @p PITDriver object
 * @param[in] config	pointer to the  @p PIT_CH_Config object
 *
 * @api
 */
void pit_lld_start (PITDriver *pitd, PIT_CH_Config *config){
  pitd->pit_ch_config = config;   /* set Driver configuration                 */

/* The following code has been removed because the PIT0 is already enabled in 
   the OSAL component for the system time*/
#if 0
  SPC5_PIT_ENABLE_CLOCK();        /* start the peripheral clock               */

  pitd->pit_tagp->PITMCR.R = 1;   /* PIT clock enabled, stop while debugging. */
#endif
}

/**
 * @brief   start a PIT Channel with frequency and callback defined in the configurator
 *
 * @param[in] pitd      pointer to the @p PITDriver object
 * @param[in] channel	number of channel to activate
 *
 * @api
 */
void pit_lld_channel_start (PITDriver *pitd, uint8_t channel){
  uint32_t reg;
  if (pitd->pit_ch_config[channel].enabled == TRUE){
    reg = (SPCGetPeripheralClock(SPC5_PERIPHERAL_PIT0) / pitd->pit_ch_config[channel].frequency) - 1U;
    pitd->pit_tagp->CH[channel].LDVAL.R = reg;
    pitd->pit_tagp->CH[channel].CVAL.R  = reg;
    pitd->pit_tagp->CH[channel].TFLG.R  = 1U;    /* Interrupt flag cleared.                  */
    pitd->pit_tagp->CH[channel].TCTRL.R = 3U;    /* Timer active, interrupt enabled.         */
  }
}

/**
 * @brief   Change PIT Channel frequency 
 *
 * @param[in] pitd       pointer to the @p PITDriver object
 * @param[in] channel	 number of channel to activate
 * @param[in] frequency  new frequency in Hertz
 *
 * @api
 */
void pit_lld_channel_set_freq (PITDriver *pitd, uint8_t channel,uint32_t frequency){
  uint32_t reg;
   
  if (pitd->pit_ch_config[channel].enabled == TRUE){
	/* Stop timer */	
    pitd->pit_tagp->CH[channel].TCTRL.R = 0U; 
    /* Set new frequency and restart timer */
    pitd->pit_ch_config[channel].frequency = frequency;    
    reg = (SPCGetPeripheralClock(SPC5_PERIPHERAL_PIT0) / pitd->pit_ch_config[channel].frequency) - 1U;
    pitd->pit_tagp->CH[channel].LDVAL.R = reg;
    pitd->pit_tagp->CH[channel].CVAL.R  = reg;
    pitd->pit_tagp->CH[channel].TFLG.R  = 1U;    /* Interrupt flag cleared.                  */
    pitd->pit_tagp->CH[channel].TCTRL.R = 3U;    /* Timer active, interrupt enabled.         */
  }
}

/**
 * @brief   STOP a PIT Channel 
 *
 * @param[in] pitd      pointer to the @p PITDriver object
 * @param[in] channel	number of channel to deactivate
 *
 * @api
 */
void pit_lld_channel_stop (PITDriver *pitd, uint8_t channel){
  if (pitd->pit_ch_config[channel].enabled == TRUE){
    if (pitd->pit_tagp->CH[channel].TCTRL.R != 0U){
	  pitd->pit_tagp->CH[channel].TCTRL.R = 0U;    /* Timer Inactive, interrupt disabled.         */
	}
  }
}

/**
 * @brief   STOP the PIT peripheral
 *
 * @param[in] pitd      pointer to the @p PITDriver object
 *
 * @api
 */
void pit_lld_stop (PITDriver *pitd){
  pitd->pit_ch_config =  NULL;    /* set Driver configuration   */
  pitd->pit_tagp->PITMCR.R = 0;   /* PIT clock Disabled.        */
  SPC5_PIT_DISABLE_CLOCK();       /* stop the peripheral clock  */
}
#endif /* LLD_USE_PIT */

/** @} */
