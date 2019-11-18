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
 * @file    serial_lld.c
 * @brief   SPC5xx SERIAL low level driver code.
 *
 * @addtogroup SERIAL
 * @{
 */

#include "serial_lld.h"

#if (LLD_USE_SERIAL == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/* Set of macros dealing with the variable number of DMAs depending on
   the chosen mode.*/
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
#define linflex_lld_setdma(sdp, tx_cfg, rx_cfg) {                           \
  (sdp)->tx_channel = edmaChannelAllocate(&(tx_cfg));                       \
  (sdp)->rx_channel = edmaChannelAllocate(&(rx_cfg));                       \
}
#define linflex_lld_resetdma(tx_cfg, rx_cfg) {                              \
  edmaChannelRelease(&(tx_cfg));                                            \
  edmaChannelRelease(&(rx_cfg));                                            \
}
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
static void linflex_serve_tx_dma_irq(edma_channel_t channel, void *p);
static void linflex_serve_rx_dma_irq(edma_channel_t channel, void *p);
static void linflex_serve_dma_error_irq(edma_channel_t channel,
                                        void *p,
                                        uint32_t esr);
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */

/**
 * @brief   LIINFlex-0 serial driver identifier.
 */
#if (SPC5_LINFLEX0_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
SerialDriver SD1;
#endif

/**
 * @brief   LIINFlex-1 serial driver identifier.
 */
#if (SPC5_LINFLEX1_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
SerialDriver SD2;
#endif

/**
 * @brief   LIINFlex-2 serial driver identifier.
 */
#if (SPC5_LINFLEX2_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
SerialDriver SD3;
#endif

/**
 * @brief   LIINFlex-3 serial driver identifier.
 */
#if (SPC5_LINFLEX3_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
SerialDriver SD4;
#endif

/**
 * @brief   LIINFlex-4 serial driver identifier.
 */
#if (SPC5_LINFLEX4_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
SerialDriver SD5;
#endif

/**
 * @brief   LIINFlex-5 serial driver identifier.
 */
#if (SPC5_LINFLEX5_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
SerialDriver SD6;
#endif

/**
 * @brief   LIINFlex-6 serial driver identifier.
 */
#if (SPC5_LINFLEX6_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
SerialDriver SD7;
#endif

/**
 * @brief   LIINFlex-7 serial driver identifier.
 */
#if (SPC5_LINFLEX7_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
SerialDriver SD8;
#endif

/**
 * @brief   LIINFlex-8 serial driver identifier.
 */
#if (SPC5_LINFLEX8_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
SerialDriver SD9;
#endif

/**
 * @brief   LIINFlex-9 serial driver identifier.
 */
#if (SPC5_LINFLEX9_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
SerialDriver SD10;
#endif

/**
 * @brief   LIINFlex-10 serial driver identifier.
 */
#if (SPC5_LINFLEX10_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
SerialDriver SD11;
#endif

/**
 * @brief   LIINFlex-11 serial driver identifier.
 */
#if (SPC5_LINFLEX11_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
SerialDriver SD12;
#endif

/**
 * @brief   LIINFlex-12 serial driver identifier.
 */
#if (SPC5_LINFLEX12_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
SerialDriver SD13;
#endif

/**
 * @brief   LIINFlex-13 serial driver identifier.
 */
#if (SPC5_LINFLEX13_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
SerialDriver SD14;
#endif

/**
 * @brief   LIINFlex-14 serial driver identifier.
 */
#if (SPC5_LINFLEX14_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
SerialDriver SD15;
#endif

/**
 * @brief   LIINFlex-15 serial driver identifier.
 */
#if (SPC5_LINFLEX15_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
SerialDriver SD16;
#endif

/**
 * @brief   LIINFlex-16 serial driver identifier.
 */
#if (SPC5_LINFLEX16_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
SerialDriver SD17;
#endif

/**
 * @brief   LIINFlex-17 serial driver identifier.
 */
#if (SPC5_LINFLEX17_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
SerialDriver SD18;
#endif

/**
 * @brief   LIINFlex-18 serial driver identifier.
 */
#if (SPC5_LINFLEX18_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
SerialDriver SD19;
#endif

/**
 * @brief   LIINFlex-19 serial driver identifier.
 */
#if (SPC5_LINFLEX19_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
SerialDriver SD20;
#endif

/**
 * @brief   LIINFlex-20 serial driver identifier.
 */
#if (SPC5_LINFLEX20_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
SerialDriver SD21;
#endif

/**
 * @brief   LIINFlex-21 serial driver identifier.
 */
#if (SPC5_LINFLEX21_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
SerialDriver SD22;
#endif

/**
 * @brief   LIINFlex-22 serial driver identifier.
 */
#if (SPC5_LINFLEX22_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
SerialDriver SD23;
#endif

/**
 * @brief   LIINFlex-23 serial driver identifier.
 */
#if (SPC5_LINFLEX23_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
SerialDriver SD24;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   Driver default configuration.
 */

static const SerialConfig default_config = {SERIAL_DEFAULT_BAUDRATE,
                                            SD_MODE_8BITS_PARITY_NONE,
                                            SPC5_LIN_API_MODE_SYNCHRONOUS,
                                            NULL,
                                            NULL,
                                            FALSE,
                                            NULL};

#if (SPC5_LINFLEX0_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) || defined(__DOXYGEN__)
#if (SPC5_LINFLEX0_DMA_SUPPORTED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for LINFLEX0 RX.
 */
static const edma_channel_config_t linflex0_rx_dma_config = {
  SPC5_SERIAL_LINFLEX0_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX0_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX0_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX0_DMA_IRQ_PRIO,
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &SD1
};

/**
 * @brief   DMA configuration for LINFLEX0 TX.
 */
static const edma_channel_config_t linflex0_tx_dma_config = {
  SPC5_SERIAL_LINFLEX0_TX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX0_TX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX0_TX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX0_DMA_IRQ_PRIO,
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &SD1
};
#endif /* SPC5_LINFLEX0_DMA_SUPPORTED */
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX0_SETTING */

#if (SPC5_LINFLEX1_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) || defined(__DOXYGEN__)
#if (SPC5_LINFLEX1_DMA_SUPPORTED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for LINFLEX1 RX.
 */
static const edma_channel_config_t linflex1_rx_dma_config = {
  SPC5_SERIAL_LINFLEX1_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX1_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX1_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX1_DMA_IRQ_PRIO,
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &SD2
};

/**
 * @brief   DMA configuration for LINFLEX1 TX.
 */
static const edma_channel_config_t linflex1_tx_dma_config = {
  SPC5_SERIAL_LINFLEX1_TX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX1_TX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX1_TX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX1_DMA_IRQ_PRIO,
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &SD2
};
#endif /* SPC5_LINFLEX1_DMA_SUPPORTED */
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX1_SETTING */

#if (SPC5_LINFLEX2_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) || defined(__DOXYGEN__)
#if (SPC5_LINFLEX2_DMA_SUPPORTED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for LINFLEX2 RX.
 */
static const edma_channel_config_t linflex2_rx_dma_config = {
  SPC5_SERIAL_LINFLEX2_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX2_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX2_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX2_DMA_IRQ_PRIO,
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &SD3
};

/**
 * @brief   DMA configuration for LINFLEX2 TX.
 */
static const edma_channel_config_t linflex2_tx_dma_config = {
  SPC5_SERIAL_LINFLEX2_TX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX2_TX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX2_TX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX2_DMA_IRQ_PRIO,
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &SD3
};
#endif /* SPC5_LINFLEX2_DMA_SUPPORTED */
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX2_SETTING */

#if (SPC5_LINFLEX3_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) || defined(__DOXYGEN__)
#if (SPC5_LINFLEX3_DMA_SUPPORTED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for LINFLEX3 RX.
 */
static const edma_channel_config_t linflex3_rx_dma_config = {
  SPC5_SERIAL_LINFLEX3_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX3_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX3_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX3_DMA_IRQ_PRIO,
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &SD4
};

/**
 * @brief   DMA configuration for LINFLEX3 TX.
 */
static const edma_channel_config_t linflex3_tx_dma_config = {
  SPC5_SERIAL_LINFLEX3_TX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX3_TX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX3_TX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX3_DMA_IRQ_PRIO,
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &SD4
};
#endif /* SPC5_LINFLEX3_DMA_SUPPORTED */
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX3_SETTING */

#if (SPC5_LINFLEX4_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) || defined(__DOXYGEN__)
#if (SPC5_LINFLEX4_DMA_SUPPORTED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for LINFLEX4 RX.
 */
static const edma_channel_config_t linflex4_rx_dma_config = {
  SPC5_SERIAL_LINFLEX4_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX4_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX4_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX4_DMA_IRQ_PRIO,
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &SD5
};

/**
 * @brief   DMA configuration for LINFLEX4 TX.
 */
static const edma_channel_config_t linflex4_tx_dma_config = {
  SPC5_SERIAL_LINFLEX4_TX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX4_TX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX4_TX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX4_DMA_IRQ_PRIO,
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &SD5
};
#endif /* SPC5_LINFLEX4_DMA_SUPPORTED */
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX4_SETTING */

#if (SPC5_LINFLEX5_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) || defined(__DOXYGEN__)
#if (SPC5_LINFLEX5_DMA_SUPPORTED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for LINFLEX5 RX.
 */
static const edma_channel_config_t linflex5_rx_dma_config = {
  SPC5_SERIAL_LINFLEX5_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX5_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX5_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX5_DMA_IRQ_PRIO,
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &SD6
};

/**
 * @brief   DMA configuration for LINFLEX5 TX.
 */
static const edma_channel_config_t linflex5_tx_dma_config = {
  SPC5_SERIAL_LINFLEX5_TX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX5_TX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX5_TX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX5_DMA_IRQ_PRIO,
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &SD6
};
#endif /* SPC5_LINFLEX5_DMA_SUPPORTED */
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX5_SETTING */

#if (SPC5_LINFLEX6_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) || defined(__DOXYGEN__)
#if (SPC5_LINFLEX6_DMA_SUPPORTED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for LINFLEX6 RX.
 */
static const edma_channel_config_t linflex6_rx_dma_config = {
  SPC5_SERIAL_LINFLEX6_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX6_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX6_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX6_DMA_IRQ_PRIO,
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &SD7
};

/**
 * @brief   DMA configuration for LINFLEX6 TX.
 */
static const edma_channel_config_t linflex6_tx_dma_config = {
  SPC5_SERIAL_LINFLEX6_TX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX6_TX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX6_TX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX6_DMA_IRQ_PRIO,
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &SD7
};
#endif /* SPC5_LINFLEX6_DMA_SUPPORTED */
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX6_SETTING */

#if (SPC5_LINFLEX7_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) || defined(__DOXYGEN__)
#if (SPC5_LINFLEX7_DMA_SUPPORTED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for LINFLEX7 RX.
 */
static const edma_channel_config_t linflex7_rx_dma_config = {
  SPC5_SERIAL_LINFLEX7_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX7_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX7_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX7_DMA_IRQ_PRIO,
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &SD8
};

/**
 * @brief   DMA configuration for LINFLEX7 TX.
 */
static const edma_channel_config_t linflex7_tx_dma_config = {
  SPC5_SERIAL_LINFLEX7_TX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX7_TX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX7_TX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX7_DMA_IRQ_PRIO,
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &SD8
};
#endif /* SPC5_LINFLEX7_DMA_SUPPORTED */
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX7_SETTING */

#if (SPC5_LINFLEX8_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) || defined(__DOXYGEN__)
#if (SPC5_LINFLEX8_DMA_SUPPORTED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for LINFLEX8 RX.
 */
static const edma_channel_config_t linflex8_rx_dma_config = {
  SPC5_SERIAL_LINFLEX8_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX8_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX8_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX8_DMA_IRQ_PRIO,
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &SD9
};

/**
 * @brief   DMA configuration for LINFLEX8 TX.
 */
static const edma_channel_config_t linflex8_tx_dma_config = {
  SPC5_SERIAL_LINFLEX8_TX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX8_TX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX8_TX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX8_DMA_IRQ_PRIO,
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &SD9
};
#endif /* SPC5_LINFLEX8_DMA_SUPPORTED */
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX8_SETTING */

#if (SPC5_LINFLEX9_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) || defined(__DOXYGEN__)
#if (SPC5_LINFLEX9_DMA_SUPPORTED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for LINFLEX9 RX.
 */
static const edma_channel_config_t linflex9_rx_dma_config = {
  SPC5_SERIAL_LINFLEX9_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX9_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX9_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX9_DMA_IRQ_PRIO,
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &SD10
};

/**
 * @brief   DMA configuration for LINFLEX9 TX.
 */
static const edma_channel_config_t linflex9_tx_dma_config = {
  SPC5_SERIAL_LINFLEX9_TX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX9_TX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX9_TX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX9_DMA_IRQ_PRIO,
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &SD10
};
#endif /* SPC5_LINFLEX9_DMA_SUPPORTED */
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX9_SETTING */

#if (SPC5_LINFLEX10_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) || defined(__DOXYGEN__)
#if (SPC5_LINFLEX10_DMA_SUPPORTED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for LINFLEX10 RX.
 */
static const edma_channel_config_t linflex10_rx_dma_config = {
  SPC5_SERIAL_LINFLEX10_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX10_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX10_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX10_DMA_IRQ_PRIO,
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &SD11
};

/**
 * @brief   DMA configuration for LINFLEX10 TX.
 */
static const edma_channel_config_t linflex10_tx_dma_config = {
  SPC5_SERIAL_LINFLEX10_TX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX10_TX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX10_TX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX10_DMA_IRQ_PRIO,
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &SD11
};
#endif /* SPC5_LINFLEX10_DMA_SUPPORTED */
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX10_SETTING */

#if (SPC5_LINFLEX11_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) || defined(__DOXYGEN__)
#if (SPC5_LINFLEX11_DMA_SUPPORTED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for LINFLEX11 RX.
 */
static const edma_channel_config_t linflex11_rx_dma_config = {
  SPC5_SERIAL_LINFLEX11_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX11_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX11_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX11_DMA_IRQ_PRIO,
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &SD12
};

/**
 * @brief   DMA configuration for LINFLEX11 TX.
 */
static const edma_channel_config_t linflex11_tx_dma_config = {
  SPC5_SERIAL_LINFLEX11_TX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX11_TX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX11_TX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX11_DMA_IRQ_PRIO,
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &SD12
};
#endif /* SPC5_LINFLEX11_DMA_SUPPORTED */
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX11_SETTING */

#if (SPC5_LINFLEX12_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) || defined(__DOXYGEN__)
#if (SPC5_LINFLEX12_DMA_SUPPORTED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for LINFLEX12 RX.
 */
static const edma_channel_config_t linflex12_rx_dma_config = {
  SPC5_SERIAL_LINFLEX12_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX12_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX12_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX12_DMA_IRQ_PRIO,
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &SD13
};

/**
 * @brief   DMA configuration for LINFLEX12 TX.
 */
static const edma_channel_config_t linflex12_tx_dma_config = {
  SPC5_SERIAL_LINFLEX12_TX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX12_TX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX12_TX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX12_DMA_IRQ_PRIO,
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &SD13
};
#endif /* SPC5_LINFLEX12_DMA_SUPPORTED */
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX12_SETTING */

#if (SPC5_LINFLEX13_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) || defined(__DOXYGEN__)
#if (SPC5_LINFLEX13_DMA_SUPPORTED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for LINFLEX13 RX.
 */
static const edma_channel_config_t linflex13_rx_dma_config = {
  SPC5_SERIAL_LINFLEX13_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX13_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX13_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX13_DMA_IRQ_PRIO,
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &SD14
};

/**
 * @brief   DMA configuration for LINFLEX13 TX.
 */
static const edma_channel_config_t linflex13_tx_dma_config = {
  SPC5_SERIAL_LINFLEX13_TX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX13_TX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX13_TX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX13_DMA_IRQ_PRIO,
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &SD14
};
#endif /* SPC5_LINFLEX13_DMA_SUPPORTED */
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX13_SETTING */

#if (SPC5_LINFLEX14_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) || defined(__DOXYGEN__)
#if (SPC5_LINFLEX14_DMA_SUPPORTED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for LINFLEX14 RX.
 */
static const edma_channel_config_t linflex14_rx_dma_config = {
  SPC5_SERIAL_LINFLEX14_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX14_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX14_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX14_DMA_IRQ_PRIO,
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &SD15
};

/**
 * @brief   DMA configuration for LINFLEX14 TX.
 */
static const edma_channel_config_t linflex14_tx_dma_config = {
  SPC5_SERIAL_LINFLEX14_TX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX14_TX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX14_TX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX14_DMA_IRQ_PRIO,
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &SD15
};
#endif /* SPC5_LINFLEX14_DMA_SUPPORTED */
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX14_SETTING */

#if (SPC5_LINFLEX15_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) || defined(__DOXYGEN__)
#if (SPC5_LINFLEX15_DMA_SUPPORTED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for LINFLEX15 RX.
 */
static const edma_channel_config_t linflex15_rx_dma_config = {
  SPC5_SERIAL_LINFLEX15_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX15_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX15_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX15_DMA_IRQ_PRIO,
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &SD16
};

/**
 * @brief   DMA configuration for LINFLEX15 TX.
 */
static const edma_channel_config_t linflex15_tx_dma_config = {
  SPC5_SERIAL_LINFLEX15_TX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX15_TX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX15_TX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX15_DMA_IRQ_PRIO,
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &SD16
};
#endif /* SPC5_LINFLEX15_DMA_SUPPORTED */
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX15_SETTING */

#if (SPC5_LINFLEX16_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) || defined(__DOXYGEN__)
#if (SPC5_LINFLEX16_DMA_SUPPORTED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for LINFLEX16 RX.
 */
static const edma_channel_config_t linflex16_rx_dma_config = {
  SPC5_SERIAL_LINFLEX16_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX16_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX16_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX16_DMA_IRQ_PRIO,
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &SD17
};

/**
 * @brief   DMA configuration for LINFLEX16 TX.
 */
static const edma_channel_config_t linflex16_tx_dma_config = {
  SPC5_SERIAL_LINFLEX16_TX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX16_TX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX16_TX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX16_DMA_IRQ_PRIO,
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &SD17
};
#endif /* SPC5_LINFLEX16_DMA_SUPPORTED */
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX16_SETTING */

#if (SPC5_LINFLEX17_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) || defined(__DOXYGEN__)
#if (SPC5_LINFLEX17_DMA_SUPPORTED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for LINFLEX17 RX.
 */
static const edma_channel_config_t linflex17_rx_dma_config = {
  SPC5_SERIAL_LINFLEX17_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX17_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX17_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX17_DMA_IRQ_PRIO,
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &SD18
};

/**
 * @brief   DMA configuration for LINFLEX17 TX.
 */
static const edma_channel_config_t linflex17_tx_dma_config = {
  SPC5_SERIAL_LINFLEX17_TX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX17_TX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX17_TX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX17_DMA_IRQ_PRIO,
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &SD18
};
#endif /* SPC5_LINFLEX17_DMA_SUPPORTED */
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX17_SETTING */

#if (SPC5_LINFLEX18_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) || defined(__DOXYGEN__)
#if (SPC5_LINFLEX18_DMA_SUPPORTED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for LINFLEX18 RX.
 */
static const edma_channel_config_t linflex18_rx_dma_config = {
  SPC5_SERIAL_LINFLEX18_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX18_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX18_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX18_DMA_IRQ_PRIO,
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &SD19
};

/**
 * @brief   DMA configuration for LINFLEX18 TX.
 */
static const edma_channel_config_t linflex18_tx_dma_config = {
  SPC5_SERIAL_LINFLEX18_TX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX18_TX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX18_TX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX18_DMA_IRQ_PRIO,
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &SD19
};
#endif /* SPC5_LINFLEX18_DMA_SUPPORTED */
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX18_SETTING */

#if (SPC5_LINFLEX19_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) || defined(__DOXYGEN__)
#if (SPC5_LINFLEX19_DMA_SUPPORTED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for LINFLEX19 RX.
 */
static const edma_channel_config_t linflex19_rx_dma_config = {
  SPC5_SERIAL_LINFLEX19_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX19_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX19_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX19_DMA_IRQ_PRIO,
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &SD20
};

/**
 * @brief   DMA configuration for LINFLEX19 TX.
 */
static const edma_channel_config_t linflex19_tx_dma_config = {
  SPC5_SERIAL_LINFLEX19_TX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX19_TX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX19_TX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX19_DMA_IRQ_PRIO,
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &SD20
};
#endif /* SPC5_LINFLEX19_DMA_SUPPORTED */
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX19_SETTING */

#if (SPC5_LINFLEX20_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) || defined(__DOXYGEN__)
#if (SPC5_LINFLEX20_DMA_SUPPORTED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for LINFLEX20 RX.
 */
static const edma_channel_config_t linflex20_rx_dma_config = {
  SPC5_SERIAL_LINFLEX20_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX20_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX20_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX20_DMA_IRQ_PRIO,
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &SD21
};

/**
 * @brief   DMA configuration for LINFLEX20 TX.
 */
static const edma_channel_config_t linflex20_tx_dma_config = {
  SPC5_SERIAL_LINFLEX20_TX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX20_TX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX20_TX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX20_DMA_IRQ_PRIO,
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &SD21
};
#endif /* SPC5_LINFLEX20_DMA_SUPPORTED */
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX20_SETTING */

#if (SPC5_LINFLEX21_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) || defined(__DOXYGEN__)
#if (SPC5_LINFLEX21_DMA_SUPPORTED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for LINFLEX21 RX.
 */
static const edma_channel_config_t linflex21_rx_dma_config = {
  SPC5_SERIAL_LINFLEX21_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX21_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX21_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX21_DMA_IRQ_PRIO,
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &SD22
};

/**
 * @brief   DMA configuration for LINFLEX21 TX.
 */
static const edma_channel_config_t linflex21_tx_dma_config = {
  SPC5_SERIAL_LINFLEX21_TX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX21_TX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX21_TX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX21_DMA_IRQ_PRIO,
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &SD22
};
#endif /* SPC5_LINFLEX21_DMA_SUPPORTED */
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX21_SETTING */

#if (SPC5_LINFLEX22_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) || defined(__DOXYGEN__)
#if (SPC5_LINFLEX22_DMA_SUPPORTED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for LINFLEX22 RX.
 */
static const edma_channel_config_t linflex22_rx_dma_config = {
  SPC5_SERIAL_LINFLEX22_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX22_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX22_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX22_DMA_IRQ_PRIO,
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &SD23
};

/**
 * @brief   DMA configuration for LINFLEX22 TX.
 */
static const edma_channel_config_t linflex22_tx_dma_config = {
  SPC5_SERIAL_LINFLEX22_TX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX22_TX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX22_TX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX22_DMA_IRQ_PRIO,
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &SD23
};
#endif /* SPC5_LINFLEX22_DMA_SUPPORTED */
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX22_SETTING */

#if (SPC5_LINFLEX23_SETTING == SPC5_LINFLEX_MODE_SERIAL) || defined(__DOXYGEN__)
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) || defined(__DOXYGEN__)
#if (SPC5_LINFLEX23_DMA_SUPPORTED == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for LINFLEX23 RX.
 */
static const edma_channel_config_t linflex23_rx_dma_config = {
  SPC5_SERIAL_LINFLEX23_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX23_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX23_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX23_DMA_IRQ_PRIO,
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &SD24
};

/**
 * @brief   DMA configuration for LINFLEX23 TX.
 */
static const edma_channel_config_t linflex23_tx_dma_config = {
  SPC5_SERIAL_LINFLEX23_TX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_LINFLEX23_TX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_LINFLEX23_TX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SERIAL_LINFLEX23_DMA_IRQ_PRIO,
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &SD24
};
#endif /* SPC5_LINFLEX23_DMA_SUPPORTED */
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX23_SETTING */

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   LINFlex initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 */
static void spc5_linflex_init(SerialDriver *sdp) {

  uint32_t div;

  volatile struct spc5_linflexl *linflexlp = sdp->linflexlp;

  /* Enters the configuration mode.*/
  linflexlp->LINCR1.R = 1;                      /* INIT bit.                */

  /* Configures the LINFlex in UART mode with all the required
   parameters.*/
  linflexlp->UARTCR.R = 1;                      /* UART mode FIRST.         */
  linflexlp->UARTCR.R = (uint16_t)(
      SPC5_UARTCR_UART | SPC5_UARTCR_RXEN | (uint16_t)(sdp->config->mode));
  div = sdp->clock / (sdp->config->speed);
  linflexlp->LINFBRR.R = (uint16_t)(div & 15U); /* Fractional divider.      */
  linflexlp->LINIBRR.R = (uint16_t)(div >> 4);  /* Integer divider.         */
  linflexlp->UARTSR.R = 0xFFFF;                 /* Clearing UARTSR register.*/

#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
  if ((sdp->dma_supported == TRUE) && (sdp->config->dma_enable == TRUE)) {
    /* TX FIFO mode enabled.*/
    linflexlp->UARTCR.B.TFBM = 1U;

    /* RX FIFO mode enabled.*/
    linflexlp->UARTCR.B.RFBM = 1U;
  }
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */

  /* Leaves the configuration mode.*/
  linflexlp->LINCR1.R = 0;
}

/**
 * @brief   LINFlex de-initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] linflexlp  pointer to a LINFlex I/O block
 */
static void spc5_linflex_deinit(volatile struct spc5_linflexl *linflexlp) {

  /* Enters the configuration mode.*/
  linflexlp->LINCR1.R = 1;                      /* INIT bit.                */

  /* Resets the LINFlex registers.*/
  linflexlp->LINFBRR.R = 0;                     /* Fractional divider.      */
  linflexlp->LINIBRR.R = 0;                     /* Integer divider.         */
  linflexlp->UARTSR.R = 0xFFFF;                 /* Clearing UARTSR register.*/
  linflexlp->UARTCR.R = 1;                      /* UART mode.               */
  linflexlp->LINIER.R = 0;                      /* Interrupts disabled.     */

  /* Leaves the configuration mode.*/
  linflexlp->LINCR1.R = 0;
}

/**
 * @brief   Common RXI IRQ handler.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 */
static void spc5xx_serve_rxi_interrupt(uint32_t isdp) {
  SerialDriver *sdp = (SerialDriver *) isdp;
  uint16_t sr = sdp->linflexlp->UARTSR.R;

  sdp->linflexlp->UARTSR.R = (uint16_t)(SPC5_UARTSR_NF | SPC5_UARTSR_DRF | SPC5_UARTSR_PE0);

  if ((sr & SPC5_UARTSR_DRF) == SPC5_UARTSR_DRF) {
    if(sdp->config->api_mode != SPC5_LIN_API_MODE_BUFFERED_IO) {
      *sdp->rx_buf = (uint8_t)sdp->linflexlp->BDRM.B.DATA4;
      sdp->rx_buf++;
      sdp->linflexlp->UARTSR.R = (uint16_t)SPC5_UARTSR_RMB;
      sdp->rx_len--;
      
      if (sdp->rx_len == 0UL) {
        sdp->linflexlp->UARTCR.B.RXEN = 0;
        sdp->linflexlp->LINIER.B.DRIE = 0; /* Interrupts disabled. */
        if (sdp->linflexlp->UARTCR.B.TXEN == 0UL) {
          sdp->linflexlp->LINIER.R &= ~((uint16_t)(SPC5_LIN_INTERRUPTS));
        }
      
        /* Call the related callback.*/
        if (sdp->config->rx_end_cb != NULL){
          sdp->config->rx_end_cb(sdp);
        }
      
        SPC5_LIN_RX_DONE(sdp);
      }
    } else {
      *sdp->rx_write_ptr = (uint8_t)sdp->linflexlp->BDRM.B.DATA4;
      sdp->rx_write_ptr++;
      if (sdp->rx_write_ptr == sdp->rx_end_ptr) {
        sdp->rx_write_ptr = sdp->rx_buffered_io;
      }
      if (sdp->rx_write_ptr == sdp->rx_read_ptr) {
       sdp->rx_read_ptr++;
       if (sdp->rx_read_ptr == sdp->rx_end_ptr) {
         sdp->rx_read_ptr = sdp->rx_buffered_io;
       }
      }
    }
  }
}

/**
 * @brief   Common TXI IRQ handler.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 */
static void spc5xx_serve_txi_interrupt(uint32_t isdp) {
  SerialDriver *sdp = (SerialDriver *) isdp;

  sdp->linflexlp->UARTSR.R = (uint16_t)SPC5_UARTSR_DTF;

  if (sdp->tx_len != 0UL) {
    sdp->linflexlp->BDRL.B.DATA0 = *sdp->tx_buf++;
    sdp->tx_len--;
  } else {
    sdp->linflexlp->UARTCR.B.TXEN = 0;
    sdp->linflexlp->LINIER.B.DTIE = 0; /* Interrupts disabled. */
    if (sdp->linflexlp->UARTCR.B.RXEN == 0U) {
      sdp->linflexlp->LINIER.R &= ~((uint16_t)(SPC5_LIN_INTERRUPTS));
    }

    /* Call the related callback.*/
    if (sdp->config->tx_end_cb != NULL){
      sdp->config->tx_end_cb(sdp);
    }

    SPC5_LIN_TX_DONE(sdp);
  }
}

/**
 * @brief   Common ERR IRQ handler.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 */
static void spc5xx_serve_err_interrupt(uint32_t isdp) {
  SerialDriver *sdp = (SerialDriver *) isdp;

  sdp->linflexlp->UARTSR.R = (uint16_t)(SPC5_UARTSR_BOF | SPC5_UARTSR_FEF |
  SPC5_UARTSR_SZF);
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level serial driver initialization.
 *
 * @init
 */
void sd_lld_init(void) {

#if (SPC5_LINFLEX0_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  SD1.linflexlp = &SPC5_LINFLEX0L;
  SD1.linflexhp = &SPC5_LINFLEX0H;
  SD1.clock = SPC5_LINFLEX0_CLK;
  LinflexD1.device = (uint32_t)&SD1;
  LinflexD1.rxi_lincallback = spc5xx_serve_rxi_interrupt;
  LinflexD1.txi_lincallback = spc5xx_serve_txi_interrupt;
  LinflexD1.err_lincallback = spc5xx_serve_err_interrupt;
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
  SD1.dma_supported = SPC5_LINFLEX0_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX0_SETTING */

#if (SPC5_LINFLEX1_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  SD2.linflexlp = &SPC5_LINFLEX1L;
  SD2.linflexhp = &SPC5_LINFLEX1H;
  SD2.clock = SPC5_LINFLEX1_CLK;
  LinflexD2.device = (uint32_t)&SD2;
  LinflexD2.rxi_lincallback = spc5xx_serve_rxi_interrupt;
  LinflexD2.txi_lincallback = spc5xx_serve_txi_interrupt;
  LinflexD2.err_lincallback = spc5xx_serve_err_interrupt;

#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
  SD2.dma_supported = SPC5_LINFLEX1_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX1_SETTING*/

#if (SPC5_LINFLEX2_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  SD3.linflexlp = &SPC5_LINFLEX2L;
  SD3.linflexhp = &SPC5_LINFLEX2H;
  SD3.clock = SPC5_LINFLEX2_CLK;
  LinflexD3.device = (uint32_t)&SD3;
  LinflexD3.rxi_lincallback = spc5xx_serve_rxi_interrupt;
  LinflexD3.txi_lincallback = spc5xx_serve_txi_interrupt;
  LinflexD3.err_lincallback = spc5xx_serve_err_interrupt;
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
  SD3.dma_supported = SPC5_LINFLEX2_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX2_SETTING */

#if (SPC5_LINFLEX3_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  SD4.linflexlp = &SPC5_LINFLEX3L;
  SD4.linflexhp = &SPC5_LINFLEX3H;
  SD4.clock = SPC5_LINFLEX3_CLK;
  LinflexD4.device = (uint32_t)&SD4;
  LinflexD4.rxi_lincallback = spc5xx_serve_rxi_interrupt;
  LinflexD4.txi_lincallback = spc5xx_serve_txi_interrupt;
  LinflexD4.err_lincallback = spc5xx_serve_err_interrupt;
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
  SD4.dma_supported = SPC5_LINFLEX3_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX3_SETTING */

#if (SPC5_LINFLEX4_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  SD5.linflexlp = &SPC5_LINFLEX4L;
  SD5.linflexhp = &SPC5_LINFLEX4H;
  SD5.clock = SPC5_LINFLEX4_CLK;
  LinflexD5.device = (uint32_t)&SD5;
  LinflexD5.rxi_lincallback = spc5xx_serve_rxi_interrupt;
  LinflexD5.txi_lincallback = spc5xx_serve_txi_interrupt;
  LinflexD5.err_lincallback = spc5xx_serve_err_interrupt;
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
  SD5.dma_supported = SPC5_LINFLEX4_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX4_SETTING*/

#if (SPC5_LINFLEX5_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  SD6.linflexlp = &SPC5_LINFLEX5L;
  SD6.linflexhp = &SPC5_LINFLEX5H;
  SD6.clock = SPC5_LINFLEX5_CLK;
  LinflexD6.device = (uint32_t)&SD6;
  LinflexD6.rxi_lincallback = spc5xx_serve_rxi_interrupt;
  LinflexD6.txi_lincallback = spc5xx_serve_txi_interrupt;
  LinflexD6.err_lincallback = spc5xx_serve_err_interrupt;
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
  SD6.dma_supported = SPC5_LINFLEX5_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX5_SETTING */

#if (SPC5_LINFLEX6_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  SD7.linflexlp = &SPC5_LINFLEX6L;
  SD7.linflexhp = &SPC5_LINFLEX6H;
  SD7.clock = SPC5_LINFLEX6_CLK;
  LinflexD7.device = (uint32_t)&SD7;
  LinflexD7.rxi_lincallback = spc5xx_serve_rxi_interrupt;
  LinflexD7.txi_lincallback = spc5xx_serve_txi_interrupt;
  LinflexD7.err_lincallback = spc5xx_serve_err_interrupt;
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
  SD7.dma_supported = SPC5_LINFLEX6_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX6_SETTING */

#if (SPC5_LINFLEX7_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  SD8.linflexlp = &SPC5_LINFLEX7L;
  SD8.linflexhp = &SPC5_LINFLEX7H;
  SD8.clock = SPC5_LINFLEX7_CLK;
  LinflexD8.device = (uint32_t)&SD8;
  LinflexD8.rxi_lincallback = spc5xx_serve_rxi_interrupt;
  LinflexD8.txi_lincallback = spc5xx_serve_txi_interrupt;
  LinflexD8.err_lincallback = spc5xx_serve_err_interrupt;
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
  SD8.dma_supported = SPC5_LINFLEX7_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX7_SETTING */

#if (SPC5_LINFLEX8_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  SD9.linflexlp = &SPC5_LINFLEX8L;
  SD9.linflexhp = &SPC5_LINFLEX8H;
  SD9.clock = SPC5_LINFLEX8_CLK;
  LinflexD9.device = (uint32_t)&SD9;
  LinflexD9.rxi_lincallback = spc5xx_serve_rxi_interrupt;
  LinflexD9.txi_lincallback = spc5xx_serve_txi_interrupt;
  LinflexD9.err_lincallback = spc5xx_serve_err_interrupt;
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
  SD9.dma_supported = SPC5_LINFLEX8_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX8_SETTING */

#if (SPC5_LINFLEX9_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  SD10.linflexlp = &SPC5_LINFLEX9L;
  SD10.linflexhp = &SPC5_LINFLEX9H;
  SD10.clock = SPC5_LINFLEX9_CLK;
  LinflexD10.device = (uint32_t)&SD10;
  LinflexD10.rxi_lincallback = spc5xx_serve_rxi_interrupt;
  LinflexD10.txi_lincallback = spc5xx_serve_txi_interrupt;
  LinflexD10.err_lincallback = spc5xx_serve_err_interrupt;
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
  SD10.dma_supported = SPC5_LINFLEX9_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX9_SETTING */

#if (SPC5_LINFLEX10_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  SD11.linflexlp = &SPC5_LINFLEX10L;
  SD11.linflexhp = &SPC5_LINFLEX10H;
  SD11.clock = SPC5_LINFLEX10_CLK;
  LinflexD11.device = (uint32_t)&SD11;
  LinflexD11.rxi_lincallback = spc5xx_serve_rxi_interrupt;
  LinflexD11.txi_lincallback = spc5xx_serve_txi_interrupt;
  LinflexD11.err_lincallback = spc5xx_serve_err_interrupt;
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
  SD11.dma_supported = SPC5_LINFLEX10_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX10_SETTING */

#if (SPC5_LINFLEX11_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  SD12.linflexlp = &SPC5_LINFLEX11L;
  SD12.linflexhp = &SPC5_LINFLEX11H;
  SD12.clock = SPC5_LINFLEX11_CLK;
  LinflexD12.device = (uint32_t)&SD12;
  LinflexD12.rxi_lincallback = spc5xx_serve_rxi_interrupt;
  LinflexD12.txi_lincallback = spc5xx_serve_txi_interrupt;
  LinflexD12.err_lincallback = spc5xx_serve_err_interrupt;
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
  SD12.dma_supported = SPC5_LINFLEX11_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX11_SETTING */

#if (SPC5_LINFLEX12_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  SD13.linflexlp = &SPC5_LINFLEX12L;
  SD13.linflexhp = &SPC5_LINFLEX12H;
  SD13.clock = SPC5_LINFLEX12_CLK;
  LinflexD13.device = (uint32_t)&SD13;
  LinflexD13.rxi_lincallback = spc5xx_serve_rxi_interrupt;
  LinflexD13.txi_lincallback = spc5xx_serve_txi_interrupt;
  LinflexD13.err_lincallback = spc5xx_serve_err_interrupt;
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
  SD13.dma_supported = SPC5_LINFLEX12_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX12_SETTING */

#if (SPC5_LINFLEX13_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  SD14.linflexlp = &SPC5_LINFLEX13L;
  SD14.linflexhp = &SPC5_LINFLEX13H;
  SD14.clock = SPC5_LINFLEX13_CLK;
  LinflexD14.device = (uint32_t)&SD14;
  LinflexD14.rxi_lincallback = spc5xx_serve_rxi_interrupt;
  LinflexD14.txi_lincallback = spc5xx_serve_txi_interrupt;
  LinflexD14.err_lincallback = spc5xx_serve_err_interrupt;
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
  SD14.dma_supported = SPC5_LINFLEX13_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX13_SETTING */

#if (SPC5_LINFLEX14_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  SD15.linflexlp = &SPC5_LINFLEX14L;
  SD15.linflexhp = &SPC5_LINFLEX14H;
  SD15.clock = SPC5_LINFLEX14_CLK;
  LinflexD15.device = (uint32_t)&SD15;
  LinflexD15.rxi_lincallback = spc5xx_serve_rxi_interrupt;
  LinflexD15.txi_lincallback = spc5xx_serve_txi_interrupt;
  LinflexD15.err_lincallback = spc5xx_serve_err_interrupt;
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
  SD15.dma_supported = SPC5_LINFLEX14_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX14_SETTING */

#if (SPC5_LINFLEX15_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  SD16.linflexlp = &SPC5_LINFLEX15L;
  SD16.linflexhp = &SPC5_LINFLEX15H;
  SD16.clock = SPC5_LINFLEX15_CLK;
  LinflexD16.device = (uint32_t)&SD16;
  LinflexD16.rxi_lincallback = spc5xx_serve_rxi_interrupt;
  LinflexD16.txi_lincallback = spc5xx_serve_txi_interrupt;
  LinflexD16.err_lincallback = spc5xx_serve_err_interrupt;
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
  SD16.dma_supported = SPC5_LINFLEX15_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX15_SETTING */

#if (SPC5_LINFLEX16_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  SD17.linflexlp = &SPC5_LINFLEX16L;
  SD17.linflexhp = &SPC5_LINFLEX16H;
  SD17.clock = SPC5_LINFLEX16_CLK;
  LinflexD17.device = (uint32_t)&SD17;
  LinflexD17.rxi_lincallback = spc5xx_serve_rxi_interrupt;
  LinflexD17.txi_lincallback = spc5xx_serve_txi_interrupt;
  LinflexD17.err_lincallback = spc5xx_serve_err_interrupt;
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
  SD17.dma_supported = SPC5_LINFLEX16_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX16_SETTING */

#if (SPC5_LINFLEX17_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  SD18.linflexlp = &SPC5_LINFLEX17L;
  SD18.linflexhp = &SPC5_LINFLEX17H;
  SD18.clock = SPC5_LINFLEX17_CLK;
  LinflexD18.device = (uint32_t)&SD18;
  LinflexD18.rxi_lincallback = spc5xx_serve_rxi_interrupt;
  LinflexD18.txi_lincallback = spc5xx_serve_txi_interrupt;
  LinflexD18.err_lincallback = spc5xx_serve_err_interrupt;
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
  SD18.dma_supported = SPC5_LINFLEX17_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX17_SETTING */

#if (SPC5_LINFLEX18_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  SD19.linflexlp = &SPC5_LINFLEX18L;
  SD19.linflexhp = &SPC5_LINFLEX18H;
  SD19.clock = SPC5_LINFLEX18_CLK;
  LinflexD19.device = (uint32_t)&SD19;
  LinflexD19.rxi_lincallback = spc5xx_serve_rxi_interrupt;
  LinflexD19.txi_lincallback = spc5xx_serve_txi_interrupt;
  LinflexD19.err_lincallback = spc5xx_serve_err_interrupt;
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
  SD19.dma_supported = SPC5_LINFLEX18_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX18_SETTING */

#if (SPC5_LINFLEX19_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  SD20.linflexlp = &SPC5_LINFLEX19L;
  SD20.linflexhp = &SPC5_LINFLEX19H;
  SD20.clock = SPC5_LINFLEX19_CLK;
  LinflexD20.device = (uint32_t)&SD20;
  LinflexD20.rxi_lincallback = spc5xx_serve_rxi_interrupt;
  LinflexD20.txi_lincallback = spc5xx_serve_txi_interrupt;
  LinflexD20.err_lincallback = spc5xx_serve_err_interrupt;
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
  SD20.dma_supported = SPC5_LINFLEX19_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX19_SETTING */

#if (SPC5_LINFLEX20_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  SD21.linflexlp = &SPC5_LINFLEX20L;
  SD21.linflexhp = &SPC5_LINFLEX20H;
  SD21.clock = SPC5_LINFLEX20_CLK;
  LinflexD21.device = (uint32_t)&SD21;
  LinflexD21.rxi_lincallback = spc5xx_serve_rxi_interrupt;
  LinflexD21.txi_lincallback = spc5xx_serve_txi_interrupt;
  LinflexD21.err_lincallback = spc5xx_serve_err_interrupt;
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
  SD21.dma_supported = SPC5_LINFLEX20_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX20_SETTING */

#if (SPC5_LINFLEX21_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  SD22.linflexlp = &SPC5_LINFLEX21L;
  SD22.linflexhp = &SPC5_LINFLEX21H;
  SD22.clock = SPC5_LINFLEX21_CLK;
  LinflexD22.device = (uint32_t)&SD22;
  LinflexD22.rxi_lincallback = spc5xx_serve_rxi_interrupt;
  LinflexD22.txi_lincallback = spc5xx_serve_txi_interrupt;
  LinflexD22.err_lincallback = spc5xx_serve_err_interrupt;
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
  SD22.dma_supported = SPC5_LINFLEX21_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX21_SETTING */

#if (SPC5_LINFLEX22_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  SD23.linflexlp = &SPC5_LINFLEX22L;
  SD23.linflexhp = &SPC5_LINFLEX22H;
  SD23.clock = SPC5_LINFLEX22_CLK;
  LinflexD23.device = (uint32_t)&SD23;
  LinflexD23.rxi_lincallback = spc5xx_serve_rxi_interrupt;
  LinflexD23.txi_lincallback = spc5xx_serve_txi_interrupt;
  LinflexD23.err_lincallback = spc5xx_serve_err_interrupt;
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
  SD23.dma_supported = SPC5_LINFLEX22_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX22_SETTING */

#if (SPC5_LINFLEX23_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  SD24.linflexlp = &SPC5_LINFLEX23L;
  SD24.linflexhp = &SPC5_LINFLEX23H;
  SD24.clock = SPC5_LINFLEX23_CLK;
  LinflexD24.device = (uint32_t)&SD24;
  LinflexD24.rxi_lincallback = spc5xx_serve_rxi_interrupt;
  LinflexD24.txi_lincallback = spc5xx_serve_txi_interrupt;
  LinflexD24.err_lincallback = spc5xx_serve_err_interrupt;
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
  SD24.dma_supported = SPC5_LINFLEX23_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX23_SETTING */

  linflex_lld_init();
}

/**
 * @brief   Low level serial driver configuration and start.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] config    the architecture-dependent serial driver configuration.
 *                      If this parameter is set to @p NULL then a default
 *                      configuration is used.
 *
 * @api
 */
void sd_lld_start(SerialDriver *sdp, const SerialConfig *config) {

  if (config == NULL) {
    sdp->config = &default_config;
  } else {
    sdp->config = config;
  }
#if (SPC5_LINFLEX0_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD1 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX0_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_setdma(sdp, linflex0_tx_dma_config, linflex0_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX0_PCTL,
    SPC5_LINFLEX0_START_PCTL);
  }
#endif
#if (SPC5_LINFLEX1_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD2 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX1_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_setdma(sdp, linflex1_tx_dma_config, linflex1_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX1_PCTL,
    SPC5_LINFLEX1_START_PCTL);
  }
#endif
#if (SPC5_LINFLEX2_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD3 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX2_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_setdma(sdp, linflex2_tx_dma_config, linflex2_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX2_PCTL,
    SPC5_LINFLEX2_START_PCTL);
  }
#endif
#if (SPC5_LINFLEX3_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD4 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX3_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_setdma(sdp, linflex3_tx_dma_config, linflex3_rx_dma_config);
    }
#endif
#ifdef SPC5_LINFLEX3_WORKAROUND
	/* With SPC574Sxx, in order to use LINFlex3 also
	   the LINFlex1 has to be clocked.*/
    SPCSetPeripheralClockMode(SPC5_LINFLEX1_PCTL,
    SPC5_LINFLEX1_START_PCTL);
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX3_PCTL,
    SPC5_LINFLEX3_START_PCTL);
  }
#endif
#if (SPC5_LINFLEX4_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD5 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX4_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_setdma(sdp, linflex4_tx_dma_config, linflex4_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX4_PCTL,
    SPC5_LINFLEX4_START_PCTL);
  }
#endif
#if (SPC5_LINFLEX5_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD6 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX5_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_setdma(sdp, linflex5_tx_dma_config, linflex5_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX5_PCTL,
    SPC5_LINFLEX5_START_PCTL);
  }
#endif
#if (SPC5_LINFLEX6_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD7 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX6_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_setdma(sdp, linflex6_tx_dma_config, linflex6_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX6_PCTL,
    SPC5_LINFLEX6_START_PCTL);
  }
#endif
#if (SPC5_LINFLEX7_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD8 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX7_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_setdma(sdp, linflex7_tx_dma_config, linflex7_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX7_PCTL,
    SPC5_LINFLEX7_START_PCTL);
  }
#endif
#if (SPC5_LINFLEX8_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD9 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX8_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_setdma(sdp, linflex8_tx_dma_config, linflex8_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX8_PCTL,
    SPC5_LINFLEX8_START_PCTL);
  }
#endif
#if (SPC5_LINFLEX9_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD10 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX9_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_setdma(sdp, linflex9_tx_dma_config, linflex9_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX9_PCTL,
    SPC5_LINFLEX9_START_PCTL);
  }
#endif
#if (SPC5_LINFLEX10_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD11 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX10_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_setdma(sdp, linflex10_tx_dma_config, linflex10_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX10_PCTL,
    SPC5_LINFLEX10_START_PCTL);
  }
#endif
#if (SPC5_LINFLEX11_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD12 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX11_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_setdma(sdp, linflex11_tx_dma_config, linflex11_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX11_PCTL,
    SPC5_LINFLEX11_START_PCTL);
  }
#endif
#if (SPC5_LINFLEX12_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD13 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX12_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_setdma(sdp, linflex12_tx_dma_config, linflex12_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX12_PCTL,
    SPC5_LINFLEX12_START_PCTL);
  }
#endif
#if (SPC5_LINFLEX13_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD14 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX13_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_setdma(sdp, linflex13_tx_dma_config, linflex13_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX13_PCTL,
    SPC5_LINFLEX13_START_PCTL);
  }
#endif
#if (SPC5_LINFLEX14_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD15 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX14_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_setdma(sdp, linflex14_tx_dma_config, linflex14_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX14_PCTL,
    SPC5_LINFLEX14_START_PCTL);
  }
#endif
#if (SPC5_LINFLEX15_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD16 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX15_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_setdma(sdp, linflex15_tx_dma_config, linflex15_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX15_PCTL,
    SPC5_LINFLEX15_START_PCTL);
  }
#endif
#if (SPC5_LINFLEX16_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD17 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX16_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_setdma(sdp, linflex16_tx_dma_config, linflex16_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX16_PCTL,
    SPC5_LINFLEX16_START_PCTL);
  }
#endif
#if (SPC5_LINFLEX17_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD18 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX17_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_setdma(sdp, linflex17_tx_dma_config, linflex17_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX17_PCTL,
    SPC5_LINFLEX17_START_PCTL);
  }
#endif

#if (SPC5_LINFLEX18_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD19 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX18_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_setdma(sdp, linflex18_tx_dma_config, linflex18_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX18_PCTL,
    SPC5_LINFLEX18_START_PCTL);
  }
#endif

#if (SPC5_LINFLEX19_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD20 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX19_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_setdma(sdp, linflex19_tx_dma_config, linflex19_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX19_PCTL,
    SPC5_LINFLEX19_START_PCTL);
  }
#endif

#if (SPC5_LINFLEX20_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD21 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX20_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_setdma(sdp, linflex20_tx_dma_config, linflex20_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX20_PCTL,
    SPC5_LINFLEX20_START_PCTL);
  }
#endif

#if (SPC5_LINFLEX21_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD22 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX21_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_setdma(sdp, linflex21_tx_dma_config, linflex21_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX21_PCTL,
    SPC5_LINFLEX21_START_PCTL);
  }
#endif

#if (SPC5_LINFLEX22_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD23 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX22_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_setdma(sdp, linflex22_tx_dma_config, linflex22_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX22_PCTL,
    SPC5_LINFLEX22_START_PCTL);
  }
#endif

#if (SPC5_LINFLEX23_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD24 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX23_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_setdma(sdp, linflex23_tx_dma_config, linflex23_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX23_PCTL,
    SPC5_LINFLEX23_START_PCTL);
  }
#endif

  spc5_linflex_init(sdp);

  if(sdp->config->api_mode == SPC5_LIN_API_MODE_BUFFERED_IO) {
    sdp->rx_write_ptr = sdp->rx_buffered_io;
    sdp->rx_read_ptr = sdp->rx_buffered_io;
    sdp->rx_end_ptr = sdp->rx_buffered_io + SPC5_SERIAL_BUFFERED_IO_SIZE;

    sdp->linflexlp->LINIER.R = (uint16_t)(SPC5_LINIER_DRIE | SPC5_LIN_INTERRUPTS); /* Interrupts enabled. */
    sdp->linflexlp->UARTCR.B.RXEN = 1;
  }
}

/**
 * @brief   Low level serial driver stop.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 *
 * @api
 */
void sd_lld_stop(SerialDriver *sdp) {

  spc5_linflex_deinit(sdp->linflexlp);

#if (SPC5_LINFLEX0_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD1 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX0_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex0_tx_dma_config, linflex0_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX0_PCTL,
    SPC5_LINFLEX0_STOP_PCTL);
    return;
  }
#endif
#if (SPC5_LINFLEX1_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD2 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX1_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex1_tx_dma_config, linflex1_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX1_PCTL,
    SPC5_LINFLEX1_STOP_PCTL);
    return;
  }
#endif
#if (SPC5_LINFLEX2_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD3 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX2_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex2_tx_dma_config, linflex2_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX2_PCTL,
    SPC5_LINFLEX2_STOP_PCTL);
    return;
  }
#endif
#if (SPC5_LINFLEX3_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD4 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX3_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex3_tx_dma_config, linflex3_rx_dma_config);
    }
#endif
#ifdef SPC5_LINFLEX3_WORKAROUND
	/* With SPC574Sxx, in order to use LINFlex3 also
	   the LINFlex1 has to be clocked.*/
    SPCSetPeripheralClockMode(SPC5_LINFLEX1_PCTL,
    SPC5_LINFLEX1_STOP_PCTL);
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX3_PCTL,
    SPC5_LINFLEX3_STOP_PCTL);
    return;
  }
#endif
#if (SPC5_LINFLEX4_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD5 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX4_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex4_tx_dma_config, linflex4_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX4_PCTL,
    SPC5_LINFLEX4_STOP_PCTL);
    return;
  }
#endif
#if (SPC5_LINFLEX5_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD6 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX5_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex5_tx_dma_config, linflex5_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX5_PCTL,
    SPC5_LINFLEX5_STOP_PCTL);
    return;
  }
#endif
#if (SPC5_LINFLEX6_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD7 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX6_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex6_tx_dma_config, linflex6_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX6_PCTL,
    SPC5_LINFLEX6_STOP_PCTL);
    return;
  }
#endif
#if (SPC5_LINFLEX7_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD8 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX7_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex7_tx_dma_config, linflex7_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX7_PCTL,
    SPC5_LINFLEX7_STOP_PCTL);
    return;
  }
#endif
#if (SPC5_LINFLEX8_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD9 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX8_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex8_tx_dma_config, linflex8_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX8_PCTL,
    SPC5_LINFLEX8_STOP_PCTL);
    return;
  }
#endif
#if (SPC5_LINFLEX9_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD10 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX9_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex9_tx_dma_config, linflex9_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX9_PCTL,
    SPC5_LINFLEX9_STOP_PCTL);
    return;
  }
#endif
#if (SPC5_LINFLEX10_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD11 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX10_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex10_tx_dma_config, linflex10_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX10_PCTL,
    SPC5_LINFLEX10_STOP_PCTL);
    return;
  }
#endif
#if (SPC5_LINFLEX11_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD12 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX11_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex11_tx_dma_config, linflex11_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX11_PCTL,
    SPC5_LINFLEX11_STOP_PCTL);
    return;
  }
#endif
#if (SPC5_LINFLEX12_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD13 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX12_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex12_tx_dma_config, linflex12_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX12_PCTL,
    SPC5_LINFLEX12_STOP_PCTL);
    return;
  }
#endif
#if (SPC5_LINFLEX13_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD14 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX13_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex13_tx_dma_config, linflex13_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX13_PCTL,
    SPC5_LINFLEX13_STOP_PCTL);
    return;
  }
#endif
#if (SPC5_LINFLEX14_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD15 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX14_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex14_tx_dma_config, linflex14_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX14_PCTL,
    SPC5_LINFLEX14_STOP_PCTL);
    return;
  }
#endif
#if (SPC5_LINFLEX15_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD16 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX15_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex15_tx_dma_config, linflex15_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX15_PCTL,
    SPC5_LINFLEX15_STOP_PCTL);
    return;
  }
#endif
#if (SPC5_LINFLEX16_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD17 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX16_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex16_tx_dma_config, linflex16_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX16_PCTL,
    SPC5_LINFLEX16_STOP_PCTL);
    return;
  }
#endif
#if (SPC5_LINFLEX17_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD18 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX17_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex17_tx_dma_config, linflex17_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX17_PCTL,
    SPC5_LINFLEX17_STOP_PCTL);
    return;
  }
#endif

#if (SPC5_LINFLEX18_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD19 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX18_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex18_tx_dma_config, linflex18_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX18_PCTL,
    SPC5_LINFLEX18_STOP_PCTL);
    return;
  }
#endif

#if (SPC5_LINFLEX19_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD20 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX19_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex19_tx_dma_config, linflex19_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX19_PCTL,
    SPC5_LINFLEX19_STOP_PCTL);
    return;
  }
#endif

#if (SPC5_LINFLEX20_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD21 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX20_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex20_tx_dma_config, linflex20_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX20_PCTL,
    SPC5_LINFLEX20_STOP_PCTL);
    return;
  }
#endif

#if (SPC5_LINFLEX21_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD22 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX21_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex21_tx_dma_config, linflex21_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX21_PCTL,
    SPC5_LINFLEX21_STOP_PCTL);
    return;
  }
#endif

#if (SPC5_LINFLEX22_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD23 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX22_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex22_tx_dma_config, linflex22_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX22_PCTL,
    SPC5_LINFLEX22_STOP_PCTL);
    return;
  }
#endif

#if (SPC5_LINFLEX23_SETTING == SPC5_LINFLEX_MODE_SERIAL)
  if (&SD24 == sdp) {
#if ((SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) && (SPC5_LINFLEX23_DMA_SUPPORTED == TRUE))
    if (sdp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex23_tx_dma_config, linflex23_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX23_PCTL,
    SPC5_LINFLEX23_STOP_PCTL);
    return;
  }
#endif

}

/**
 * @brief   Serial write with DMA
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] txbuf     data buffer to be transmitted
 * @param[in] n         number of bytes to be transmitted
 *
 * @notapi
 */
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
static void sd_tx_dma(SerialDriver *sdp, uint8_t *txbuf, size_t n) {

  /* Setup DMA TCD parameters.*/
  edmaChannelSetup(
                   sdp->tx_channel,             /* channel.                 */
                   txbuf,                       /* src.                     */
                   ((vuint32_t)&sdp->linflexlp->BDRL) + 3U, /* dst.         */
                   1,                           /* soff, advance by 1.      */
                   0,                           /* doff, do not advance.    */
                   0,                           /* ssize, 8 bits transfers. */
                   0,                           /* dsize, 8 bits transfers. */
                   1,                           /* nbytes, always one.      */
                   n,                           /* iter.                    */
                   0,                           /* slast, no source adjust. */
                   0,                           /* dlast, no dest.adjust.   */
                   EDMA_TCD_MODE_DREQ | EDMA_TCD_MODE_INT_END); /* mode.    */

  /* Start TX DMA channel.*/
  edmaChannelStart(sdp->tx_channel);

  /* Enable DMA TX channel.*/
  sdp->linflexhp->DMATXE.R = 1UL;
}
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */

/**
 * @brief   Serial write without DMA
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] buffer    data buffer to be transmitted
 * @param[in] len       number of bytes to be transmitted
 *
 * @notapi
 */
#ifdef __ghs__
#pragma ghs ZO
#endif
static void sd_tx(SerialDriver *sdp, uint8_t *buffer, uint16_t len) {

  if (len > 0UL) {
    uint8_t c;

    sdp->tx_buf = buffer;
    sdp->tx_len = len - 1U;
    sdp->linflexlp->LINIER.R |= (uint16_t)(SPC5_LINIER_DTIE | SPC5_LIN_INTERRUPTS); /* Interrupts enabled. */
    sdp->linflexlp->UARTCR.B.TXEN = 1;

    /* Update of tx_buf is shared with interrupt routine.
     * A write to DATA0 register can trigger an interrupt, make sure
     * to increment tx_buf before writing to DATA0 register.
     */
    c = *sdp->tx_buf++;
    sdp->linflexlp->BDRL.B.DATA0 = c;
  }
}
#ifdef __ghs__
#pragma ghs O
#endif


/**
 * @brief   Serial write without IRQ support (synchronous transmission)
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] buffer    data buffer to be transmitted
 * @param[in] len       number of bytes to be transmitted
 *
 * @notapi
 */
static void sd_tx_no_irq(SerialDriver *sdp, uint8_t *buffer, uint16_t len) {

  if (len > 0U) {

    /* If DMA mode is enabled, TX FIFO mode is disabled
       before starting the no-irq transfer.*/
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
    if ((sdp->dma_supported == TRUE) && (sdp->config->dma_enable == TRUE)) {
      /* Enters the configuration mode.*/
      sdp->linflexlp->LINCR1.R = 1;
      /* TX FIFO mode disabled.*/
      sdp->linflexlp->UARTCR.B.TFBM = 0U;
      /* Leaves the configuration mode.*/
      sdp->linflexlp->LINCR1.R = 0;
    }
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */

    sdp->linflexlp->UARTSR.R = (uint16_t)SPC5_UARTSR_DTF;
    sdp->linflexlp->UARTCR.B.TXEN = 1;
    /* Transmit the first byte.*/
    sdp->linflexlp->BDRL.B.DATA0 = *buffer++;
    len--;

    /* Transmit all the remain bytes.*/
    while(len > 0U) {
      if(sdp->linflexlp->UARTSR.B.DTF == 1U) {
        sdp->linflexlp->UARTSR.R = (uint16_t)SPC5_UARTSR_DTF;
        sdp->linflexlp->BDRL.B.DATA0 = *buffer++;
        len--;
      }
    }

    /* Wait till the last byte is transmitted, than clear
       the related Data Transmission Completed bit.*/
    while(sdp->linflexlp->UARTSR.B.DTF == 0U) {
      ;
    }
    sdp->linflexlp->UARTSR.R = (uint16_t)SPC5_UARTSR_DTF;

    sdp->linflexlp->UARTCR.B.TXEN = 0;

    /* If DMA mode is enabled, TX FIFO mode is re-enabled
       at the end of the no-irq transfer.*/
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
    if ((sdp->dma_supported == TRUE) && (sdp->config->dma_enable == TRUE)) {
      /* Enters the configuration mode.*/
      sdp->linflexlp->LINCR1.R = 1;
      /* TX FIFO mode enabled.*/
      sdp->linflexlp->UARTCR.B.TFBM = 1U;
      /* Leaves the configuration mode.*/
      sdp->linflexlp->LINCR1.R = 0;
    }
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */

  }
}

/**
 * @brief   Low level serial driver write
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] buffer    data buffer to be transmitted
 * @param[in] len       number of bytes to be transmitted
 *
 * @return              operation result
 *
 * @api
 */
serialmsg_t sd_lld_write(SerialDriver *sdp, uint8_t *buffer, uint16_t len) {

  /* If the interrupts are disabled, only a synchronous transmission without
     interrupt support is allowed.*/
  if(irqGetExtIntEnable() == 0U) {
    sd_tx_no_irq(sdp, buffer, len);
  } else {
    SPC5_LIN_TX_IN_PROGRESS(sdp);

    /* Check if DMA is enabled.*/
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
    /* Check if the LinFlex support DMA.*/
    if ((sdp->dma_supported == TRUE) && (sdp->config->dma_enable == TRUE)) {
      sd_tx_dma(sdp, buffer, len);
    } else {
      sd_tx(sdp, buffer, len);
    }
#else
    sd_tx(sdp, buffer, len);
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */

    SPC5_LIN_WAIT_FOR_TX_COMPLETION(sdp);
  }

  return SERIAL_MSG_OK;
}

/**
 * @brief   Serial read with DMA
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] rxbuf     data buffer to be transmitted
 * @param[in] n         number of bytes to be transmitted
 *
 * @return              The number of bytes effectively read.
 *
 * @notapi
 */
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
static uint16_t sd_rx_dma(SerialDriver *sdp, uint8_t *rxbuf, size_t n) {

  /* Setup DMA TCD parameters.*/
  edmaChannelSetup(
                   sdp->rx_channel,             /* channel.                 */
                   ((vuint32_t)&sdp->linflexlp->BDRM) + 3U, /* src.         */
                   rxbuf,                       /* dst.                     */
                   0,                           /* soff, do not advance.    */
                   1,                           /* doff, advance by 1.      */
                   0,                           /* ssize, 8 bits transfers. */
                   0,                           /* dsize, 8 bits transfers. */
                   1,                           /* nbytes, always one.      */
                   n,                           /* iter.                    */
                   0,                           /* slast, no source adjust. */
                   0,                           /* dlast, no dest.adjust.   */
                   EDMA_TCD_MODE_DREQ | EDMA_TCD_MODE_INT_END); /* mode.    */

  /* Start RX DMA channel.*/
  edmaChannelStart(sdp->rx_channel);

  /* Enable DMA RX channel.*/
  sdp->linflexhp->DMARXE.R = 1UL;
  
  return (uint16_t)n;
}
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */

/**
 * @brief   Serial read without DMA
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[out] buffer   data buffer receiver
 * @param[out] len      number of bytes received
 *
 * @return              The number of bytes effectively read.
 *
 * @notapi
 */
static uint16_t sd_rx(SerialDriver* sdp, uint8_t* buffer, uint16_t len) {

  if (len > 0UL) {
    if(sdp->config->api_mode == SPC5_LIN_API_MODE_BUFFERED_IO) {
      uint16_t l = len;
      sdp->linflexlp->LINIER.B.DRIE = 0;
      while (l != 0U) {
        if (sdp->rx_read_ptr != sdp->rx_write_ptr) {
          sdp->linflexlp->LINIER.B.DRIE = 0;
          *buffer++ = *sdp->rx_read_ptr++;
          if (sdp->rx_read_ptr == sdp->rx_end_ptr) {
            sdp->rx_read_ptr = sdp->rx_buffered_io;
          }
          l--;
        } else {
             sdp->linflexlp->LINIER.B.DRIE = 1;
        }
      }
      sdp->linflexlp->LINIER.B.DRIE = 1;
    } else {
      sdp->rx_buf = buffer;
      sdp->rx_len = len;
      sdp->linflexlp->LINIER.R |= (uint16_t)(SPC5_LINIER_DRIE | SPC5_LIN_INTERRUPTS); /* Interrupts enabled. */
      sdp->linflexlp->UARTCR.B.RXEN = 1;
    }
  }
  return len;
}

/**
 * @brief   Low level serial driver read
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[out] buffer   data buffer receiver
 * @param[out] len      number of bytes received
 *
 * @return              The number of bytes effectively read.
 *
 * @api
 */
uint16_t sd_lld_read(SerialDriver* sdp, uint8_t* buffer, uint16_t len) {

  uint16_t ret;

  /* If the interrupts are disabled, reception is not possible otherwise the
     execution should be blocked till all bytes are received.*/
  if(irqGetExtIntEnable() == 0U) {
    ret = 0U;
  } else {

    if(sdp->config->api_mode != SPC5_LIN_API_MODE_BUFFERED_IO) {
      SPC5_LIN_RX_IN_PROGRESS(sdp);
	}

    /* Check if DMA is enabled.*/
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
    /* Check if the LinFlex support DMA.*/
    if ((sdp->dma_supported == TRUE) && (sdp->config->dma_enable == TRUE)) {
      ret = sd_rx_dma(sdp, buffer, len);
    } else {
      ret = sd_rx(sdp, buffer, len);
    }
#else
    ret = sd_rx(sdp, buffer, len);
#endif

    if(sdp->config->api_mode != SPC5_LIN_API_MODE_BUFFERED_IO) {
      SPC5_LIN_WAIT_FOR_RX_COMPLETION(sdp);
	}
  }

  return ret;
}

#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) || defined(__DOXYGEN__)
/**
 * @brief   Shared TX DMA events service routine.
 *
 * @param[in] channel   the channel number
 * @param[in] p         parameter for the registered function
 *
 * @notapi
 */
static void linflex_serve_tx_dma_irq(edma_channel_t channel, void *p) {

  /*Casting of void pointer needful for using the same DMA driver
    in different drivers. */
  /*lint -e9087 */
  SerialDriver *sdp = (SerialDriver *)p;
  /*lint +e9087 */

  (void)channel;

  /* Clearing TX channel state.*/
  edmaChannelStop(sdp->tx_channel);

  /* Call the related callback.*/
  if (sdp->config->tx_end_cb != NULL) {
    sdp->config->tx_end_cb(sdp);
  }

  SPC5_LIN_TX_DONE(sdp);
}

/**
 * @brief   Shared RX DMA events service routine.
 *
 * @param[in] channel   the channel number
 * @param[in] p         parameter for the registered function
 *
 * @notapi
 */
static void linflex_serve_rx_dma_irq(edma_channel_t channel, void *p) {

  /*Casting of void pointer needful for using the same DMA driver
    in different drivers. */
  /*lint -e9087 */
  SerialDriver *sdp = (SerialDriver *)p;
  /*lint +e9087 */

  (void)channel;

  /* Clearing RX channel state.*/
  edmaChannelStop(sdp->rx_channel);

  /* Call the related callback.*/
  if (sdp->config->rx_end_cb != NULL) {
    sdp->config->rx_end_cb(sdp);
  }

  SPC5_LIN_RX_DONE(sdp);
}

/**
 * @brief   Shared ISR for DMA error events.
 *
 * @param[in] channel   the channel number
 * @param[in] p         parameter for the registered function
 * @param[in] esr       content of the ESR register
 *
 * @notapi
 */
static void linflex_serve_dma_error_irq(edma_channel_t channel,
                                        void *p,
                                        uint32_t esr) {

  /*Casting of void pointer needful for using the same DMA driver
    in different drivers. */
  /*lint -e9087 */
  SerialDriver *sdp = (SerialDriver *)p;
  /*lint +e9087 */

  (void)channel;
  (void)esr;

  /* Clearing TX/RX channels state.*/
  edmaChannelStop(sdp->tx_channel);
  edmaChannelStop(sdp->rx_channel);

  if (sdp->config->dma_err_cb != NULL) {
    sdp->config->dma_err_cb(sdp);
  }
}
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */

#endif /* LLD_USE_SERIAL */

/** @} */
