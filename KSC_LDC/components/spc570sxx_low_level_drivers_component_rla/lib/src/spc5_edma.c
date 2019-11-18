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
* EVALUATION ONLY - NOT FOR USE IN PRODUCTION
*****************************************************************************/
/**
 * @file    spc5_edma.c
 * @brief   SPC5xx EDMA helper driver code for RLA components.
 *
 * @addtogroup EDMA
 * @{
 */

#include "spc5_edma.h"

#if (LLD_USE_DMA == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

static const uint8_t dma1_g0[16] = {SPC5_EDMA1_GROUP0_PRIORITIES};
#if (SPC5_EDMA_NCHANNELS > 16U) || defined(__DOXYGEN__)
static const uint8_t dma1_g1[16] = {SPC5_EDMA1_GROUP1_PRIORITIES};
#endif
#if (SPC5_EDMA_NCHANNELS > 32U) || defined(__DOXYGEN__)
static const uint8_t dma1_g2[16] = {SPC5_EDMA1_GROUP2_PRIORITIES};
static const uint8_t dma1_g3[16] = {SPC5_EDMA1_GROUP3_PRIORITIES};
#endif
#if (SPC5_EDMA_NCHANNELS > 64U) || defined(__DOXYGEN__)
static const uint8_t dma0_g0[16] = {SPC5_EDMA0_GROUP0_PRIORITIES};
#endif
#if (SPC5_EDMA_NCHANNELS > 80U) || defined(__DOXYGEN__)
static const uint8_t dma0_g1[16] = {SPC5_EDMA0_GROUP1_PRIORITIES};
#endif

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   Configurations for the various EDMA channels.
 */
static const edma_channel_config_t *channels[SPC5_EDMA_NCHANNELS];

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if !defined(SPC5_EDMA_COMBO_CH_ERR_HANDLER)
#error "SPC5_EDMA_COMBO_CH_ERR_HANDLER not defined"
#endif
/**
 * @brief   EDMA (channels 0..31) error interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_COMBO_CH_ERR_HANDLER) {

  edma_channel_t channel;
  uint32_t es;
  uint32_t dma1_errl;
#if (SPC5_EDMA_NCHANNELS > 32U)
  uint32_t dma1_errh;
#endif
#if (SPC5_EDMA_NCHANNELS > 64U)
  uint32_t dma0_errl;
#endif

  IRQ_PROLOGUE();

  /* Scanning for errors.*/
  channel = 0;
#if (SPC5_EDMA_NCHANNELS == 16U)
  dma1_errl = (0x0000FFFFUL | SPC5_EDMA1.ERRL.R);
  while ((dma1_errl != 0U) && (channel < SPC5_EDMA_NCHANNELS)) {
#elif (SPC5_EDMA_NCHANNELS == 32U)
  dma1_errl = SPC5_EDMA1.ERRL.R;
  while ((dma1_errl != 0U) && (channel < SPC5_EDMA_NCHANNELS)) {
#elif (SPC5_EDMA_NCHANNELS == 64U)
  dma1_errl = SPC5_EDMA1.ERRL.R;
  dma1_errh = SPC5_EDMA1.ERRH.R;
  while (((dma1_errl  != 0U) || (dma1_errh != 0U)) && (channel < SPC5_EDMA_NCHANNELS)) {
#elif (SPC5_EDMA_NCHANNELS == 80U)
  dma1_errl = SPC5_EDMA1.ERRL.R;
  dma1_errh = SPC5_EDMA1.ERRH.R;
  dma0_errl = (0x0000FFFFUL | SPC5_EDMA0.ERRL.R);
  while (((dma1_errl != 0U) || (dma1_errh != 0U) || (dma0_errl != 0U)) && (channel < SPC5_EDMA_NCHANNELS)) {
#elif (SPC5_EDMA_NCHANNELS == 96U)
  dma1_errl = SPC5_EDMA1.ERRL.R;
  dma1_errh = SPC5_EDMA1.ERRH.R;
  dma0_errl = SPC5_EDMA0.ERRL.R;	
  while (((dma1_errl != 0U) || (dma1_errh != 0U) || (dma0_errl != 0U)) && (channel < SPC5_EDMA_NCHANNELS)) {
#endif
    /* The check (channel < SPC5_EDMA_NCHANNELS) in the previous while cycle 
	   guarantees that the pointer channels[channel] doesn't exceed its bound
       when SPC5_EDMA_NCHANNELS = 16. So MISRA Rules 18.1 can be skipped.*/
    /*lint -e661 -e662*/
    if (channel < 32U) {
      if ((dma1_errl & (1UL << (uint32_t)channel)) != 0U) {
        /* Error flag cleared.*/
        SPC5_EDMA1.CERR.R = (uint8_t)channel;
        es = SPC5_EDMA1.ES.R;

        /* If the channel is not associated then the error is simply discarded
           else the error callback is invoked.*/
        if ((channels[channel] != NULL) &&
            (channels[channel]->dma_error_func != NULL)) {
          channels[channel]->dma_error_func(channel,
                                            channels[channel]->dma_param,
                                            es);
        }
      }
    }
    /*lint +e661 +e662*/
#if (SPC5_EDMA_NCHANNELS > 32U)
    else if (channel < 64U) {
      if ((dma1_errh & (1UL << (uint32_t)(channel - 32U))) != 0U) {
        /* Error flag cleared.*/
        SPC5_EDMA1.CERR.R = (uint8_t)channel;
        es = SPC5_EDMA1.ES.R;

        /* If the channel is not associated then the error is simply discarded
           else the error callback is invoked.*/
        if ((channels[channel] != NULL) &&
            (channels[channel]->dma_error_func != NULL)) {
          channels[channel]->dma_error_func(channel,
                                            channels[channel]->dma_param,
                                            es);
        }
      }
    } else {
#if (SPC5_EDMA_NCHANNELS > 64U)
      if ((dma0_errl & (1UL << (uint32_t)(channel - 96U))) != 0U) {
        /* Error flag cleared.*/
        SPC5_EDMA0.CERR.R = (uint8_t)channel;
        es = SPC5_EDMA0.ES.R;

        /* If the channel is not associated then the error is simply discarded
           else the error callback is invoked.*/
        if ((channels[channel] != NULL) &&
            (channels[channel]->dma_error_func != NULL)) {
          channels[channel]->dma_error_func(channel,
                                            channels[channel]->dma_param,
                                            es);
        }
      }
#endif
    }
#endif

    channel++;

#if (SPC5_EDMA_NCHANNELS == 16U)
    dma1_errl = (0x0000FFFFUL | SPC5_EDMA1.ERRL.R);
#elif (SPC5_EDMA_NCHANNELS == 32U)
    dma1_errl = SPC5_EDMA1.ERRL.R;
#elif (SPC5_EDMA_NCHANNELS == 64U)
    dma1_errl = SPC5_EDMA1.ERRL.R;
    dma1_errh = SPC5_EDMA1.ERRH.R;
#elif (SPC5_EDMA_NCHANNELS == 80U)
    dma1_errl = SPC5_EDMA1.ERRL.R;
    dma1_errh = SPC5_EDMA1.ERRH.R;
    dma0_errl = (0x0000FFFFUL | SPC5_EDMA0.ERRL.R);
#elif (SPC5_EDMA_NCHANNELS == 96U)
    dma1_errl = SPC5_EDMA1.ERRL.R;
    dma1_errh = SPC5_EDMA1.ERRH.R;
    dma0_errl = SPC5_EDMA0.ERRL.R;	
#endif

  }

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH0_HANDLER)
#error "SPC5_EDMA_CH0_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 0 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH0_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[0] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 0U;
  channels[0]->dma_func(0, channels[0]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH1_HANDLER)
#error "SPC5_EDMA_CH1_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 1 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH1_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[1] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 1U;
  channels[1]->dma_func(1, channels[1]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH2_HANDLER)
#error "SPC5_EDMA_CH2_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 2 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH2_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[2] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 2U;
  channels[2]->dma_func(2, channels[2]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH3_HANDLER)
#error "SPC5_EDMA_CH3_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 3 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH3_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[3] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 3U;
  channels[3]->dma_func(3, channels[3]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH4_HANDLER)
#error "SPC5_EDMA_CH4_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 4 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH4_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[4] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 4U;
  channels[4]->dma_func(4, channels[4]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH5_HANDLER)
#error "SPC5_EDMA_CH5_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 5 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH5_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[5] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 5U;
  channels[5]->dma_func(5, channels[5]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH6_HANDLER)
#error "SPC5_EDMA_CH6_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 6 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH6_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[6] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 6U;
  channels[6]->dma_func(6, channels[6]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH7_HANDLER)
#error "SPC5_EDMA_CH7_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 7 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH7_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[7] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 7U;
  channels[7]->dma_func(7, channels[7]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH8_HANDLER)
#error "SPC5_EDMA_CH8_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 8 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH8_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[8] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 8U;
  channels[8]->dma_func(8, channels[8]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH9_HANDLER)
#error "SPC5_EDMA_CH9_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 9 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH9_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[9] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 9U;
  channels[9]->dma_func(9, channels[9]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH10_HANDLER)
#error "SPC5_EDMA_CH10_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 10 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH10_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[10] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 10U;
  channels[10]->dma_func(10, channels[10]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH11_HANDLER)
#error "SPC5_EDMA_CH11_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 11 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH11_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[11] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 11U;
  channels[11]->dma_func(11, channels[11]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH12_HANDLER)
#error "SPC5_EDMA_CH12_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 12 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH12_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[12] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 12U;
  channels[12]->dma_func(12, channels[12]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH13_HANDLER)
#error "SPC5_EDMA_CH13_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 13 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH13_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[13] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 13U;
  channels[13]->dma_func(13, channels[13]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH14_HANDLER)
#error "SPC5_EDMA_CH14_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 14 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH14_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[14] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 14U;
  channels[14]->dma_func(14, channels[14]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH15_HANDLER)
#error "SPC5_EDMA_CH15_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 15 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH15_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[15] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 15U;
  channels[15]->dma_func(15, channels[15]->dma_param);

  IRQ_EPILOGUE();
}

#if (SPC5_EDMA_NCHANNELS > 16U) || defined(__DOXYGEN__)
#if !defined(SPC5_EDMA_CH16_HANDLER)
#error "SPC5_EDMA_CH16_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 16 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH16_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[16] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 16U;
  channels[16]->dma_func(16, channels[16]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH17_HANDLER)
#error "SPC5_EDMA_CH17_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 17 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH17_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[17] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 17U;
  channels[17]->dma_func(17, channels[17]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH18_HANDLER)
#error "SPC5_EDMA_CH18_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 18 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH18_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[18] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 18U;
  channels[18]->dma_func(18, channels[18]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH19_HANDLER)
#error "SPC5_EDMA_CH19_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 19 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH19_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[19] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 19U;
  channels[19]->dma_func(19, channels[19]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH20_HANDLER)
#error "SPC5_EDMA_CH20_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 20 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH20_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[20] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 20U;
  channels[20]->dma_func(20, channels[20]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH21_HANDLER)
#error "SPC5_EDMA_CH21_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 21 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH21_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[21] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 21U;
  channels[21]->dma_func(21, channels[21]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH22_HANDLER)
#error "SPC5_EDMA_CH22_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 22 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH22_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[22] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 22U;
  channels[22]->dma_func(22, channels[22]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH23_HANDLER)
#error "SPC5_EDMA_CH23_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 23 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH23_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[23] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 23U;
  channels[23]->dma_func(23, channels[23]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH24_HANDLER)
#error "SPC5_EDMA_CH24_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 24 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH24_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[24] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 24U;
  channels[24]->dma_func(24, channels[24]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH25_HANDLER)
#error "SPC5_EDMA_CH25_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 25 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH25_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[25] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 25U;
  channels[25]->dma_func(25, channels[25]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH26_HANDLER)
#error "SPC5_EDMA_CH26_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 26 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH26_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[26] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 26U;
  channels[26]->dma_func(26, channels[26]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH27_HANDLER)
#error "SPC5_EDMA_CH27_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 27 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH27_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[27] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 27U;
  channels[27]->dma_func(27, channels[27]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH28_HANDLER)
#error "SPC5_EDMA_CH28_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 28 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH28_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[28] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 28U;
  channels[28]->dma_func(28, channels[28]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH29_HANDLER)
#error "SPC5_EDMA_CH29_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 29 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH29_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[29] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 29U;
  channels[29]->dma_func(29, channels[29]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH30_HANDLER)
#error "SPC5_EDMA_CH30_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 30 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH30_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[30] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 30U;
  channels[30]->dma_func(30, channels[30]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH31_HANDLER)
#error "SPC5_EDMA_CH31_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 31 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH31_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[31] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 31U;
  channels[31]->dma_func(31, channels[31]->dma_param);

  IRQ_EPILOGUE();
}

#if (SPC5_EDMA_NCHANNELS > 32U) || defined(__DOXYGEN__)
#if !defined(SPC5_EDMA_CH32_HANDLER)
#error "SPC5_EDMA_CH32_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 32 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH32_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[32] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 32U;
  channels[32]->dma_func(32, channels[32]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH33_HANDLER)
#error "SPC5_EDMA_CH33_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 33 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH33_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[33] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 33U;
  channels[33]->dma_func(33, channels[33]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH34_HANDLER)
#error "SPC5_EDMA_CH34_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 34 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH34_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[34] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 34U;
  channels[34]->dma_func(34, channels[34]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH35_HANDLER)
#error "SPC5_EDMA_CH35_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 35 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH35_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[35] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 35U;
  channels[35]->dma_func(35, channels[35]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH36_HANDLER)
#error "SPC5_EDMA_CH36_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 36 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH36_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[36] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 36U;
  channels[36]->dma_func(36, channels[36]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH37_HANDLER)
#error "SPC5_EDMA_CH37_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 37 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH37_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[37] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 37U;
  channels[37]->dma_func(37, channels[37]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH38_HANDLER)
#error "SPC5_EDMA_CH38_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 38 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH38_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[38] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 38U;
  channels[38]->dma_func(38, channels[38]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH39_HANDLER)
#error "SPC5_EDMA_CH39_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 39 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH39_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[39] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 39U;
  channels[39]->dma_func(39, channels[39]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH40_HANDLER)
#error "SPC5_EDMA_CH40_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 40 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH40_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[40] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 40U;
  channels[40]->dma_func(40, channels[40]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH41_HANDLER)
#error "SPC5_EDMA_CH41_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 41 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH41_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[41] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 41U;
  channels[41]->dma_func(41, channels[41]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH42_HANDLER)
#error "SPC5_EDMA_CH42_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 42 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH42_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[42] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 42U;
  channels[42]->dma_func(42, channels[42]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH43_HANDLER)
#error "SPC5_EDMA_CH43_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 43 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH43_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[43] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 43U;
  channels[43]->dma_func(43, channels[43]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH44_HANDLER)
#error "SPC5_EDMA_CH44_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 44 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH44_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[44] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 44U;
  channels[44]->dma_func(44, channels[44]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH45_HANDLER)
#error "SPC5_EDMA_CH45_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 45 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH45_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[45] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 45U;
  channels[45]->dma_func(45, channels[45]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH46_HANDLER)
#error "SPC5_EDMA_CH46_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 46 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH46_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[46] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 46U;
  channels[46]->dma_func(46, channels[46]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH47_HANDLER)
#error "SPC5_EDMA_CH47_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 47 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH47_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[47] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 47U;
  channels[47]->dma_func(47, channels[47]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH48_HANDLER)
#error "SPC5_EDMA_CH48_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 48 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH48_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[48] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 48U;
  channels[48]->dma_func(48, channels[48]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH49_HANDLER)
#error "SPC5_EDMA_CH49_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 49 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH49_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[49] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 49U;
  channels[49]->dma_func(49, channels[49]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH50_HANDLER)
#error "SPC5_EDMA_CH50_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 50 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH50_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[50] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 50U;
  channels[50]->dma_func(50, channels[50]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH51_HANDLER)
#error "SPC5_EDMA_CH51_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 51 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH51_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[51] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 51U;
  channels[51]->dma_func(51, channels[51]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH52_HANDLER)
#error "SPC5_EDMA_CH52_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 52 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH52_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[52] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 52U;
  channels[52]->dma_func(52, channels[52]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH53_HANDLER)
#error "SPC5_EDMA_CH53_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 53 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH53_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[53] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 53U;
  channels[53]->dma_func(53, channels[53]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH54_HANDLER)
#error "SPC5_EDMA_CH54_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 54 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH54_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[54] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 54U;
  channels[54]->dma_func(54, channels[54]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH55_HANDLER)
#error "SPC5_EDMA_CH55_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 55 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH55_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[55] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 55U;
  channels[55]->dma_func(55, channels[55]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH56_HANDLER)
#error "SPC5_EDMA_CH56_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 56 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH56_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[56] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 56U;
  channels[56]->dma_func(56, channels[56]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH57_HANDLER)
#error "SPC5_EDMA_CH57_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 57 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH57_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[57] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 57U;
  channels[57]->dma_func(57, channels[57]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH58_HANDLER)
#error "SPC5_EDMA_CH58_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 58 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH58_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[58] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 58U;
  channels[58]->dma_func(58, channels[58]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH59_HANDLER)
#error "SPC5_EDMA_CH59_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 59 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH59_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[59] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 59U;
  channels[59]->dma_func(59, channels[59]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH60_HANDLER)
#error "SPC5_EDMA_CH60_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 60 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH60_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[60] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 60U;
  channels[60]->dma_func(60, channels[60]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH61_HANDLER)
#error "SPC5_EDMA_CH61_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 61 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH61_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[61] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 61U;
  channels[61]->dma_func(61, channels[61]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH62_HANDLER)
#error "SPC5_EDMA_CH62_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 62 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH62_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[62] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 62U;
  channels[62]->dma_func(62, channels[62]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH63_HANDLER)
#error "SPC5_EDMA_CH63_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 63 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH63_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[63] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA1.CINT.R = 63U;
  channels[63]->dma_func(63, channels[63]->dma_param);

  IRQ_EPILOGUE();
}

#if (SPC5_EDMA_NCHANNELS > 64U) || defined(__DOXYGEN__)
#if !defined(SPC5_EDMA_CH64_HANDLER)
#error "SPC5_EDMA_CH64_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 64 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH64_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[64] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 0U;
  channels[64]->dma_func(64, channels[64]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH65_HANDLER)
#error "SPC5_EDMA_CH65_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 65 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH65_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[65] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 1U;
  channels[65]->dma_func(65, channels[65]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH66_HANDLER)
#error "SPC5_EDMA_CH66_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 66 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH66_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[66] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 2U;
  channels[66]->dma_func(66, channels[66]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH67_HANDLER)
#error "SPC5_EDMA_CH67_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 67 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH67_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[67] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 3U;
  channels[67]->dma_func(67, channels[67]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH68_HANDLER)
#error "SPC5_EDMA_CH68_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 68 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH68_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[68] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 4U;
  channels[68]->dma_func(68, channels[68]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH69_HANDLER)
#error "SPC5_EDMA_CH69_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 69 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH69_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[69] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 5U;
  channels[69]->dma_func(69, channels[69]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH70_HANDLER)
#error "SPC5_EDMA_CH70_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 70 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH70_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[70] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 6U;
  channels[70]->dma_func(70, channels[70]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH71_HANDLER)
#error "SPC5_EDMA_CH71_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 71 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH71_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[71] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 7U;
  channels[71]->dma_func(71, channels[71]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH72_HANDLER)
#error "SPC5_EDMA_CH72_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 72 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH72_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[72] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 8U;
  channels[72]->dma_func(72, channels[72]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH73_HANDLER)
#error "SPC5_EDMA_CH73_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 73 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH73_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[73] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 9U;
  channels[73]->dma_func(73, channels[73]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH74_HANDLER)
#error "SPC5_EDMA_CH74_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 74 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH74_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[74] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 10U;
  channels[74]->dma_func(74, channels[74]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH75_HANDLER)
#error "SPC5_EDMA_CH75_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 75 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH75_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[75] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 11U;
  channels[75]->dma_func(75, channels[75]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH76_HANDLER)
#error "SPC5_EDMA_CH76_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 76 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH76_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[76] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 12U;
  channels[76]->dma_func(76, channels[76]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH77_HANDLER)
#error "SPC5_EDMA_CH77_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 77 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH77_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[77] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 13U;
  channels[77]->dma_func(77, channels[77]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH78_HANDLER)
#error "SPC5_EDMA_CH78_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 78 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH78_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[78] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 14U;
  channels[78]->dma_func(78, channels[78]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH79_HANDLER)
#error "SPC5_EDMA_CH79_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 79 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH79_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[79] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 15U;
  channels[79]->dma_func(79, channels[79]->dma_param);

  IRQ_EPILOGUE();
}

#if (SPC5_EDMA_NCHANNELS > 80U) || defined(__DOXYGEN__)
#if !defined(SPC5_EDMA_CH80_HANDLER)
#error "SPC5_EDMA_CH80_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 80 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH80_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[80] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 16U;
  channels[80]->dma_func(80, channels[80]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH81_HANDLER)
#error "SPC5_EDMA_CH81_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 81 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH81_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[81] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 17U;
  channels[81]->dma_func(81, channels[81]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH82_HANDLER)
#error "SPC5_EDMA_CH82_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 82 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH82_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[82] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 18U;
  channels[82]->dma_func(82, channels[82]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH83_HANDLER)
#error "SPC5_EDMA_CH83_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 83 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH83_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[83] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 19U;
  channels[83]->dma_func(83, channels[83]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH84_HANDLER)
#error "SPC5_EDMA_CH84_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 84 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH84_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[84] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 20U;
  channels[84]->dma_func(84, channels[84]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH85_HANDLER)
#error "SPC5_EDMA_CH85_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 85 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH85_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[85] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 21U;
  channels[85]->dma_func(85, channels[85]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH86_HANDLER)
#error "SPC5_EDMA_CH86_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 86 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH86_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[86] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 22U;
  channels[86]->dma_func(86, channels[86]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH87_HANDLER)
#error "SPC5_EDMA_CH87_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 87 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH87_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[87] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 23U;
  channels[87]->dma_func(87, channels[87]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH88_HANDLER)
#error "SPC5_EDMA_CH88_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 88 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH88_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[88] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 24U;
  channels[88]->dma_func(88, channels[88]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH89_HANDLER)
#error "SPC5_EDMA_CH89_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 89 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH89_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[89] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 25U;
  channels[89]->dma_func(89, channels[89]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH90_HANDLER)
#error "SPC5_EDMA_CH90_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 90 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH90_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[90] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 26U;
  channels[90]->dma_func(90, channels[90]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH91_HANDLER)
#error "SPC5_EDMA_CH91_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 91 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH91_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[91] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 27U;
  channels[91]->dma_func(91, channels[91]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH92_HANDLER)
#error "SPC5_EDMA_CH92_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 92 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH92_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[92] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 28U;
  channels[92]->dma_func(92, channels[92]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH93_HANDLER)
#error "SPC5_EDMA_CH93_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 93 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH93_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[93] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 29U;
  channels[93]->dma_func(93, channels[93]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH94_HANDLER)
#error "SPC5_EDMA_CH94_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 94 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH94_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[94] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 30U;
  channels[94]->dma_func(94, channels[94]->dma_param);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_EDMA_CH95_HANDLER)
#error "SPC5_EDMA_CH95_HANDLER not defined"
#endif
/**
 * @brief   EDMA channel 95 interrupt.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_EDMA_CH95_HANDLER) {

  IRQ_PROLOGUE();

  if (channels[95] == NULL) {
    SPC5_EDMA_ERROR_HANDLER();
  }
  SPC5_EDMA0.CINT.R = 31U;
  channels[95]->dma_func(95, channels[95]->dma_param);

  IRQ_EPILOGUE();
}
#endif /* SPC5_EDMA_NCHANNELS > 64 */
#endif /* SPC5_EDMA_NCHANNELS > 80 */
#endif /* SPC5_EDMA_NCHANNELS > 32 */
#endif /* SPC5_EDMA_NCHANNELS > 16 */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   EDMA driver initialization.
 *
 * @special
 */
void edmaInit(void) {
  uint8_t i;

  SPC5_EDMA1.CR.R   = SPC5_EDMA_DMA1_CR_SETTING;
  SPC5_EDMA1.ERQL.R = 0x00000000UL;
  SPC5_EDMA1.EEIL.R = 0x00000000UL;
  SPC5_EDMA1.INTL.R = 0xFFFFFFFFUL;
  SPC5_EDMA1.ERRL.R = 0xFFFFFFFFUL;
#if (SPC5_EDMA_NCHANNELS > 32U)
  SPC5_EDMA1.ERQH.R = 0x00000000UL;
  SPC5_EDMA1.EEIH.R = 0x00000000UL;
  SPC5_EDMA1.INTH.R = 0xFFFFFFFFUL;
  SPC5_EDMA1.ERRH.R = 0xFFFFFFFFUL;
#endif
#if (SPC5_EDMA_NCHANNELS > 64U)
  SPC5_EDMA0.CR.R   = SPC5_EDMA_DMA0_CR_SETTING;
  SPC5_EDMA0.ERQL.R = 0x00000000UL;
  SPC5_EDMA0.EEIL.R = 0x00000000UL;
  SPC5_EDMA0.INTL.R = 0xFFFFFFFFUL;
  SPC5_EDMA0.ERRL.R = 0xFFFFFFFFUL;
#endif

  /* Initializing all the channels with a different priority within the
     channels group.*/
  for (i = 0; i < 16U; i++) {
    SPC5_EDMA1.DCHPRI[i].R       = dma1_g0[i];
#if (SPC5_EDMA_NCHANNELS > 16U)
    SPC5_EDMA1.DCHPRI[i + 16U].R = dma1_g1[i];
#endif
#if (SPC5_EDMA_NCHANNELS > 32U)
    SPC5_EDMA1.DCHPRI[i + 32U].R = dma1_g2[i];
    SPC5_EDMA1.DCHPRI[i + 48U].R = dma1_g3[i];
#endif
#if (SPC5_EDMA_NCHANNELS > 64U)
    SPC5_EDMA0.DCHPRI[i].R       = dma0_g0[i];
#endif
#if (SPC5_EDMA_NCHANNELS > 80U)
    SPC5_EDMA0.DCHPRI[i + 16U].R = dma0_g1[i];
#endif
  }

  /* Error interrupt source.*/
  INTC_PSR(SPC5_EDMA_COMBO_CH_ERR_NUMBER) = SPC5_EDMA_COMBO_CH_ERR_IRQ_PRIO;

  /* DMA Main PCTL (36) setup.*/
  SPCSetPeripheralClockMode(SPC5_EDMA_MUX0_PCTL, SPC5_EDMA_MUX_START_PCTL);

  /* DMA Secondary PCTL (164) setup.
     Some platforms have a secondary PCTL related to the odd MUXs (1, 3, 5).*/
#if defined(SPC5_EDMA_MUX1_PCTL)
  SPCSetPeripheralClockMode(SPC5_EDMA_MUX1_PCTL, SPC5_EDMA_MUX_START_PCTL);
#endif

  /* DMA Third PCTL (163) setup.
     Chorus 10M have a third PCTL related to the MUX4.*/
#if defined(SPC5_EDMA_MUX2_PCTL)
  SPCSetPeripheralClockMode(SPC5_EDMA_MUX2_PCTL, SPC5_EDMA_MUX_START_PCTL);
#endif

}

/**
 * @brief   EDMA mux slot set.
 *
 * @param[in] ccfg      channel configuration
 *
 * @notapi
 */
static void edmaSetMuxSlot(const edma_channel_config_t *ccfg) {

#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  uint32_t source = ccfg->dma_channel;

  if(ccfg->dma_mux == 0U) {
    /* Programming the MUX0.*/
    SPC5_DMAMUX0.CHCONFIG[source].R = (uint8_t)(0x80U |
                                                ccfg->dma_periph);
  } else if (ccfg->dma_mux == 1U) {
    source = source - SPC5_EDMA_MUX0_CH_NUM;
    /* Programming the MUX1.*/
    SPC5_DMAMUX1.CHCONFIG[source].R = (uint8_t)(0x80U |
                                                ccfg->dma_periph);
#if (SPC5_EDMA_NUM_OF_MUX > 2U)
  } else if (ccfg->dma_mux == 2U) {
    source = source - (SPC5_EDMA_MUX0_CH_NUM + SPC5_EDMA_MUX1_CH_NUM);
    /* Programming the MUX2.*/
    SPC5_DMAMUX2.CHCONFIG[source].R = (uint8_t)(0x80U |
                                                ccfg->dma_periph);
#endif
#if (SPC5_EDMA_NUM_OF_MUX > 3U)
  } else if (ccfg->dma_mux == 3U) {
    source = source - (SPC5_EDMA_MUX0_CH_NUM + SPC5_EDMA_MUX1_CH_NUM + 
                       SPC5_EDMA_MUX2_CH_NUM);
    /* Programming the MUX3.*/
    SPC5_DMAMUX3.CHCONFIG[source].R = (uint8_t)(0x80U |
                                                ccfg->dma_periph);
#endif
#if (SPC5_EDMA_NUM_OF_MUX > 4U)
  } else if (ccfg->dma_mux == 4U) {
    source = source - (SPC5_EDMA_MUX0_CH_NUM + SPC5_EDMA_MUX1_CH_NUM + 
                       SPC5_EDMA_MUX2_CH_NUM + SPC5_EDMA_MUX3_CH_NUM);
    /* Programming the MUX4.*/
    SPC5_DMAMUX4.CHCONFIG[source].R = (uint8_t)(0x80U |
                                                ccfg->dma_periph);
#endif
#if (SPC5_EDMA_NUM_OF_MUX > 5U)
  } else if (ccfg->dma_mux == 5U) {
    source = source - (SPC5_EDMA_MUX0_CH_NUM + SPC5_EDMA_MUX1_CH_NUM +
                       SPC5_EDMA_MUX2_CH_NUM + SPC5_EDMA_MUX3_CH_NUM +
                       SPC5_EDMA_MUX4_CH_NUM);
    /* Programming the MUX5.*/
    SPC5_DMAMUX5.CHCONFIG[source].R = (uint8_t)(0x80U |
                                                ccfg->dma_periph);
#endif
  } else {
    ; /*close if...else if as per MISRA rules*/
  }
#else
  /* Programming the MUX.*/
  SPC5_DMAMUX0.CHCONFIG[ccfg->dma_channel].R = (uint8_t)(0x80U |
                                                         ccfg->dma_periph);
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
}

/**
 * @brief   EDMA channel allocation.
 *
 * @param[in] ccfg      channel configuration
 * @return              The channel number.
 * @retval EDMA_ERROR   if the channel cannot be allocated.
 *
 * @special
 */
edma_channel_t edmaChannelAllocate(const edma_channel_config_t *ccfg) {

  /* If the channel is already taken then an error is returned.*/
  if (channels[ccfg->dma_channel] != NULL) {
    return EDMA_ERROR;
  }

  edmaSetMuxSlot(ccfg);

  /* Associating the configuration to the channel.*/
  channels[ccfg->dma_channel] = ccfg;

  /* If an error callback is defined then the error interrupt source is
     enabled for the channel.*/
  if (ccfg->dma_error_func != NULL) {
#if (SPC5_EDMA_NCHANNELS <= 64U)
    SPC5_EDMA1.SEEI.R = (uint8_t)ccfg->dma_channel;
#else
    if (ccfg->dma_channel < 64U) {
      SPC5_EDMA1.SEEI.R = (uint8_t)ccfg->dma_channel;
    } else {
      SPC5_EDMA0.SEEI.R = (uint8_t)ccfg->dma_channel - 64U;
    }
#endif /* SPC5_EDMA_NCHANNELS <= 64U */
  }

  /* Setting up IRQ priority for the selected channel.*/

  /* Method of accessing PSR DMA channels by incrementing channel index in the INTC_PSR
     macro not comply with MISRA C because it involves a cast on a composite expression.*/
  /*lint -e9032 -e9033*/
  INTC_PSR(SPC5_EDMA_CH0_NUMBER + ccfg->dma_channel) = ccfg->dma_irq_prio;
  /*lint +e9032 +e9033*/

  return ccfg->dma_channel;
}

/**
 * @brief   EDMA mux slot reset.
 *
 * @param[in] ccfg      channel configuration
 *
 * @notapi
 */
static void edmaResetMuxSlot(const edma_channel_config_t *ccfg) {
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  uint32_t source = ccfg->dma_channel;

  if(ccfg->dma_mux == 0U) {
    /* Programming the MUX0.*/
    SPC5_DMAMUX0.CHCONFIG[source].R = 0;
  }else if (ccfg->dma_mux == 1U) {
    source = source - SPC5_EDMA_MUX0_CH_NUM;
    /* Programming the MUX1.*/
    SPC5_DMAMUX1.CHCONFIG[source].R = 0;
#if (SPC5_EDMA_NUM_OF_MUX > 2U)
  } else if (ccfg->dma_mux == 2U) {
    source = source - (SPC5_EDMA_MUX0_CH_NUM + SPC5_EDMA_MUX1_CH_NUM);
    /* Programming the MUX2.*/
    SPC5_DMAMUX2.CHCONFIG[source].R = 0;
#endif
#if (SPC5_EDMA_NUM_OF_MUX > 3U)
  } else if (ccfg->dma_mux == 3U) {
    source = source - (SPC5_EDMA_MUX0_CH_NUM + SPC5_EDMA_MUX1_CH_NUM + 
                       SPC5_EDMA_MUX2_CH_NUM);
    /* Programming the MUX3.*/
    SPC5_DMAMUX3.CHCONFIG[source].R = 0;
#endif
#if (SPC5_EDMA_NUM_OF_MUX > 4U)
  } else if (ccfg->dma_mux == 4U) {
    source = source - (SPC5_EDMA_MUX0_CH_NUM + SPC5_EDMA_MUX1_CH_NUM + 
                       SPC5_EDMA_MUX2_CH_NUM + SPC5_EDMA_MUX3_CH_NUM);
    /* Programming the MUX4.*/
    SPC5_DMAMUX4.CHCONFIG[source].R = 0;
#endif
#if (SPC5_EDMA_NUM_OF_MUX > 5U)
  } else if (ccfg->dma_mux == 5U) {
    source = source - (SPC5_EDMA_MUX0_CH_NUM + SPC5_EDMA_MUX1_CH_NUM + 
                       SPC5_EDMA_MUX2_CH_NUM + SPC5_EDMA_MUX3_CH_NUM +
                       SPC5_EDMA_MUX4_CH_NUM);
    /* Programming the MUX5.*/
    SPC5_DMAMUX5.CHCONFIG[source].R = 0;
#endif
  } else {
    ; /*close if...else if as per MISRA rules*/
  }
#else
  /* Programming the MUX.*/
  SPC5_DMAMUX0.CHCONFIG[ccfg->dma_channel].R = 0;
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
}


/**
 * @brief   EDMA channel release.
 *
 * @param[in] ccfg      channel configuration
 *
 * @special
 */
void edmaChannelRelease(const edma_channel_config_t *ccfg) {

  /* Enforcing a stop.*/
  edmaChannelStop(ccfg->dma_channel);

  edmaResetMuxSlot(ccfg);

  /* Clearing ISR sources for the channel.*/
#if (SPC5_EDMA_NCHANNELS <= 64U)
  SPC5_EDMA1.CINT.R = (uint8_t)(ccfg->dma_channel);
  SPC5_EDMA1.CEEI.R = (uint8_t)(ccfg->dma_channel);
  SPC5_EDMA1.CERR.R = (uint8_t)(ccfg->dma_channel);
#else
  if (ccfg->dma_channel < 64U) {
    SPC5_EDMA1.CINT.R = (uint8_t)(ccfg->dma_channel);
    SPC5_EDMA1.CEEI.R = (uint8_t)(ccfg->dma_channel);
    SPC5_EDMA1.CERR.R = (uint8_t)(ccfg->dma_channel);
  } else {
    SPC5_EDMA0.CINT.R = (uint8_t)(ccfg->dma_channel - 64U);
    SPC5_EDMA0.CEEI.R = (uint8_t)(ccfg->dma_channel - 64U);
    SPC5_EDMA0.CERR.R = (uint8_t)(ccfg->dma_channel - 64U);
  }
#endif /* SPC5_EDMA_NCHANNELS <= 64U */

  /* The channels is flagged as available.*/
  channels[ccfg->dma_channel] = NULL;
}

#endif /* LLD_USE_DMA */

/** @} */
