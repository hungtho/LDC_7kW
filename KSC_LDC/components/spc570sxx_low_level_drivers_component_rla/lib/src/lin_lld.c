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
 * @file    lin_lld.c
 * @brief   SPC5xx LIN low level driver code.
 *
 * @addtogroup LIN
 * @{
 */

#include "lin_lld.h"

#if (LLD_USE_LIN == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/* Set of macros dealing with the variable number of DMAs depending on
   the chosen mode.*/
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
#define linflex_lld_setdma(ldp, tx_cfg, rx_cfg) {                           \
  (ldp)->tx_channel = edmaChannelAllocate(&(tx_cfg));                       \
  (ldp)->rx_channel = edmaChannelAllocate(&(rx_cfg));                       \
}
#define linflex_lld_resetdma(tx_cfg, rx_cfg) {                              \
  edmaChannelRelease(&(tx_cfg));                                            \
  edmaChannelRelease(&(rx_cfg));                                            \
}
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
static void linflex_serve_tx_dma_irq(edma_channel_t channel, void *p);
static void linflex_serve_rx_dma_irq(edma_channel_t channel, void *p);
static void linflex_serve_dma_error_irq(edma_channel_t channel,
                                        void *p,
                                        uint32_t esr);
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */

/**
 * @brief   LIINFlex-0 lin driver identifier.
 */
#if (SPC5_LINFLEX0_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
LinDriver LD1;
#endif

/**
 * @brief   LIINFlex-1 lin driver identifier.
 */
#if (SPC5_LINFLEX1_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
LinDriver LD2;
#endif

/**
 * @brief   LIINFlex-2 lin driver identifier.
 */
#if (SPC5_LINFLEX2_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
LinDriver LD3;
#endif

/**
 * @brief   LIINFlex-3 lin driver identifier.
 */
#if (SPC5_LINFLEX3_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
LinDriver LD4;
#endif

/**
 * @brief   LIINFlex-4 lin driver identifier.
 */
#if (SPC5_LINFLEX4_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
LinDriver LD5;
#endif

/**
 * @brief   LIINFlex-5 lin driver identifier.
 */
#if (SPC5_LINFLEX5_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
LinDriver LD6;
#endif

/**
 * @brief   LIINFlex-6 lin driver identifier.
 */
#if (SPC5_LINFLEX6_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
LinDriver LD7;
#endif

/**
 * @brief   LIINFlex-7 lin driver identifier.
 */
#if (SPC5_LINFLEX7_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
LinDriver LD8;
#endif

/**
 * @brief   LIINFlex-8 lin driver identifier.
 */
#if (SPC5_LINFLEX8_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
LinDriver LD9;
#endif

/**
 * @brief   LIINFlex-9 lin driver identifier.
 */
#if (SPC5_LINFLEX9_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
LinDriver LD10;
#endif

/**
 * @brief   LIINFlex-10 lin driver identifier.
 */
#if (SPC5_LINFLEX10_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
LinDriver LD11;
#endif

/**
 * @brief   LIINFlex-11 lin driver identifier.
 */
#if (SPC5_LINFLEX11_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
LinDriver LD12;
#endif

/**
 * @brief   LIINFlex-12 lin driver identifier.
 */
#if (SPC5_LINFLEX12_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
LinDriver LD13;
#endif

/**
 * @brief   LIINFlex-13 lin driver identifier.
 */
#if (SPC5_LINFLEX13_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
LinDriver LD14;
#endif

/**
 * @brief   LIINFlex-14 lin driver identifier.
 */
#if (SPC5_LINFLEX14_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
LinDriver LD15;
#endif

/**
 * @brief   LIINFlex-15 lin driver identifier.
 */
#if (SPC5_LINFLEX15_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
LinDriver LD16;
#endif

/**
 * @brief   LIINFlex-16 lin driver identifier.
 */
#if (SPC5_LINFLEX16_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
LinDriver LD17;
#endif

/**
 * @brief   LIINFlex-17 lin driver identifier.
 */
#if (SPC5_LINFLEX17_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
LinDriver LD18;
#endif

/**
 * @brief   LIINFlex-18 lin driver identifier.
 */
#if (SPC5_LINFLEX18_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
LinDriver LD19;
#endif

/**
 * @brief   LIINFlex-19 lin driver identifier.
 */
#if (SPC5_LINFLEX19_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
LinDriver LD20;
#endif

/**
 * @brief   LIINFlex-20 lin driver identifier.
 */
#if (SPC5_LINFLEX20_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
LinDriver LD21;
#endif

/**
 * @brief   LIINFlex-21 lin driver identifier.
 */
#if (SPC5_LINFLEX21_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
LinDriver LD22;
#endif

/**
 * @brief   LIINFlex-22 lin driver identifier.
 */
#if (SPC5_LINFLEX22_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
LinDriver LD23;
#endif

/**
 * @brief   LIINFlex-23 lin driver identifier.
 */
#if (SPC5_LINFLEX23_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
LinDriver LD24;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   Driver default configuration.
 */

static const LinConfig default_config = {
    LIN_DEFAULT_BAUDRATE,
    LIN_MODE_MASTER,
    SPC5_LIN_API_MODE_SYNCHRONOUS,
    NULL,
    NULL,
    NULL,
    0,
    FALSE,
    NULL
};

#if (SPC5_LINFLEX0_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) || defined(__DOXYGEN__)
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
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &LD1
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
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &LD1
};
#endif /* SPC5_LINFLEX0_DMA_SUPPORTED */
#endif /* (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) */
#endif /* SPC5_LINFLEX0_SETTING */

#if (SPC5_LINFLEX1_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) || defined(__DOXYGEN__)
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
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &LD2
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
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &LD2
};
#endif /* SPC5_LINFLEX1_DMA_SUPPORTED */
#endif /* (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) */
#endif /* SPC5_LINFLEX1_SETTING */

#if (SPC5_LINFLEX2_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) || defined(__DOXYGEN__)
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
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &LD3
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
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &LD3
};
#endif /* SPC5_LINFLEX2_DMA_SUPPORTED */
#endif /* (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) */
#endif /* SPC5_LINFLEX2_SETTING */

#if (SPC5_LINFLEX3_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) || defined(__DOXYGEN__)
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
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &LD4
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
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &LD4
};
#endif /* SPC5_LINFLEX3_DMA_SUPPORTED */
#endif /* (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) */
#endif /* SPC5_LINFLEX3_SETTING */

#if (SPC5_LINFLEX4_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) || defined(__DOXYGEN__)
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
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &LD5
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
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &LD5
};
#endif /* SPC5_LINFLEX4_DMA_SUPPORTED */
#endif /* (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) */
#endif /* SPC5_LINFLEX4_SETTING */

#if (SPC5_LINFLEX5_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) || defined(__DOXYGEN__)
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
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &LD6
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
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &LD6
};
#endif /* SPC5_LINFLEX5_DMA_SUPPORTED */
#endif /* (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) */
#endif /* SPC5_LINFLEX5_SETTING */

#if (SPC5_LINFLEX6_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) || defined(__DOXYGEN__)
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
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &LD7
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
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &LD7
};
#endif /* SPC5_LINFLEX6_DMA_SUPPORTED */
#endif /* (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) */
#endif /* SPC5_LINFLEX6_SETTING */

#if (SPC5_LINFLEX7_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) || defined(__DOXYGEN__)
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
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &LD8
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
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &LD8
};
#endif /* SPC5_LINFLEX7_DMA_SUPPORTED */
#endif /* (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) */
#endif /* SPC5_LINFLEX7_SETTING */

#if (SPC5_LINFLEX8_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) || defined(__DOXYGEN__)
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
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &LD9
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
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &LD9
};
#endif /* SPC5_LINFLEX8_DMA_SUPPORTED */
#endif /* (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) */
#endif /* SPC5_LINFLEX8_SETTING */

#if (SPC5_LINFLEX9_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) || defined(__DOXYGEN__)
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
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &LD10
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
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &LD10
};
#endif /* SPC5_LINFLEX9_DMA_SUPPORTED */
#endif /* (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) */
#endif /* SPC5_LINFLEX9_SETTING */

#if (SPC5_LINFLEX10_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) || defined(__DOXYGEN__)
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
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &LD11
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
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &LD11
};
#endif /* SPC5_LINFLEX10_DMA_SUPPORTED */
#endif /* (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) */
#endif /* SPC5_LINFLEX10_SETTING */

#if (SPC5_LINFLEX11_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) || defined(__DOXYGEN__)
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
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &LD12
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
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &LD12
};
#endif /* SPC5_LINFLEX11_DMA_SUPPORTED */
#endif /* (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) */
#endif /* SPC5_LINFLEX11_SETTING */

#if (SPC5_LINFLEX12_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) || defined(__DOXYGEN__)
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
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &LD13
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
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &LD13
};
#endif /* SPC5_LINFLEX12_DMA_SUPPORTED */
#endif /* (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) */
#endif /* SPC5_LINFLEX12_SETTING */

#if (SPC5_LINFLEX13_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) || defined(__DOXYGEN__)
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
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &LD14
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
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &LD14
};
#endif /* SPC5_LINFLEX13_DMA_SUPPORTED */
#endif /* (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) */
#endif /* SPC5_LINFLEX13_SETTING */

#if (SPC5_LINFLEX14_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) || defined(__DOXYGEN__)
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
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &LD15
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
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &LD15
};
#endif /* SPC5_LINFLEX14_DMA_SUPPORTED */
#endif /* (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) */
#endif /* SPC5_LINFLEX14_SETTING */

#if (SPC5_LINFLEX15_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) || defined(__DOXYGEN__)
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
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &LD16
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
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &LD16
};
#endif /* SPC5_LINFLEX15_DMA_SUPPORTED */
#endif /* (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) */
#endif /* SPC5_LINFLEX15_SETTING */

#if (SPC5_LINFLEX16_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) || defined(__DOXYGEN__)
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
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &LD17
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
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &LD17
};
#endif /* SPC5_LINFLEX16_DMA_SUPPORTED */
#endif /* (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) */
#endif /* SPC5_LINFLEX16_SETTING */

#if (SPC5_LINFLEX17_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) || defined(__DOXYGEN__)
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
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &LD18
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
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &LD18
};
#endif /* SPC5_LINFLEX17_DMA_SUPPORTED */
#endif /* (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) */
#endif /* SPC5_LINFLEX17_SETTING */

#if (SPC5_LINFLEX18_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) || defined(__DOXYGEN__)
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
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &LD19
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
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &LD19
};
#endif /* SPC5_LINFLEX18_DMA_SUPPORTED */
#endif /* (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) */
#endif /* SPC5_LINFLEX18_SETTING */

#if (SPC5_LINFLEX19_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) || defined(__DOXYGEN__)
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
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &LD20
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
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &LD20
};
#endif /* SPC5_LINFLEX19_DMA_SUPPORTED */
#endif /* (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) */
#endif /* SPC5_LINFLEX19_SETTING */

#if (SPC5_LINFLEX20_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) || defined(__DOXYGEN__)
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
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &LD21
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
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &LD21
};
#endif /* SPC5_LINFLEX20_DMA_SUPPORTED */
#endif /* (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) */
#endif /* SPC5_LINFLEX20_SETTING */

#if (SPC5_LINFLEX21_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) || defined(__DOXYGEN__)
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
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &LD22
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
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &LD22
};
#endif /* SPC5_LINFLEX21_DMA_SUPPORTED */
#endif /* (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) */
#endif /* SPC5_LINFLEX21_SETTING */

#if (SPC5_LINFLEX22_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) || defined(__DOXYGEN__)
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
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &LD23
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
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &LD23
};
#endif /* SPC5_LINFLEX22_DMA_SUPPORTED */
#endif /* (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) */
#endif /* SPC5_LINFLEX22_SETTING */

#if (SPC5_LINFLEX23_SETTING == SPC5_LINFLEX_MODE_LIN) || defined(__DOXYGEN__)
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) || defined(__DOXYGEN__)
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
  linflex_serve_rx_dma_irq, linflex_serve_dma_error_irq, &LD24
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
  linflex_serve_tx_dma_irq, linflex_serve_dma_error_irq, &LD24
};
#endif /* SPC5_LINFLEX23_DMA_SUPPORTED */
#endif /* (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) */
#endif /* SPC5_LINFLEX23_SETTING */

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void spc5_linflex_set_filter(LinDriver *ldp, uint32_t filter, LinFilter *filter_cfg)
{
  uint32_t ifcrx_off;
  uint16_t ifer;
  uint8_t ifmr;
  uint32_t *pFilter;
  uint32_t p;

  ifcrx_off = (((filter * 4U) + SPC5_LINFLEX_IFCR0_OFFSET) / 4U);
  p = (uint32_t) (ldp->linflexlp);

  p += ifcrx_off * sizeof (uint32_t);
  pFilter = (uint32_t *)p;

  ifer = (uint16_t)(ldp->linflexlp->IFER.R & 0xFFFFU);
  ldp->linflexlp->IFER.R = (uint16_t) (ifer | (((uint32_t)1U << filter))); /* Enable #Filter */

  *pFilter = ((((uint32_t)filter_cfg->datasize - 1U) & SPC5_BIDR_DFL_MASK) << 10U) |
             ((uint32_t)filter_cfg->direction << 9U) |
             ((uint32_t)filter_cfg->ccs << 8U) |
             ((uint32_t)filter_cfg->value & SPC5_BIDR_ID_MASK);

  if (filter_cfg->mode == LIN_MASK_MODE) {
    ifmr = (uint8_t)(ldp->linflexlp->IFMR.R & 0xFFU);                  /* Filter 2n and 2n+1 are in mask mode. */
    ldp->linflexlp->IFMR.R = (uint16_t) (ifmr | (((uint32_t)1U << (filter >> 1)))); /* Set the Mode Register.  */

    pFilter++;                                                         /* Now pFilter -> Filter2n+1            */

    *pFilter = ((uint32_t)filter_cfg->mask_value & SPC5_BIDR_ID_MASK); /* Set the Mask value for filter 2n+1.  */
  }
}

/**
 * @brief   LINFlex initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] ldp       pointer to a @p LinDriver object
 */
static void spc5_linflex_start(LinDriver *ldp) {
  uint32_t div;
  uint32_t i = 0;

  volatile struct spc5_linflexl *linflexlp = ldp->linflexlp;

  linflexlp->LINCR1.B.SLEEP = 0;                /* exit sleep mode */
  /* Enters the configuration mode.*/
  linflexlp->LINCR1.B.INIT = 1;                      /* Enter in Initialization Mode       */

  while (0x1000U != (linflexlp->LINSR.R & 0xF000U)) {}

  linflexlp->LINCR1.B.MME = (uint8_t)ldp->config->mode;  /* Select Master/Slave mode */

  linflexlp->LINCR1.B.MBL = 3U;                 /* 13-bit break */
  linflexlp->LINCR1.B.LASE = 0;                 /* Auto synchronization disabled */
  linflexlp->LINCR2.B.IOBE = 0;                 /* Bit Error does not reset LIN state machine */

  linflexlp->LINTCSR.B.MODE = 0; /* LIN timeout mode for 8-bit counter */
  linflexlp->LINTCSR.B.IOT  = 0; /* LIN state machine does not reset to Idle on timeout */
  linflexlp->LINTCSR.B.TOCE = 0; /* Time-out counter enable - OCF flag is set if an output compare event occurs */

  linflexlp->LINSR.R = 0xFFFF;        /* Clearing LINSR register. */
  linflexlp->LINIER.R = 0;            /* LIN Interrupts disabled.     */

  /* Set the LIN baud Rate */
  div = ldp->clock / (ldp->config->speed);
  linflexlp->LINFBRR.R = (uint16_t)(div & 15U); /* Fractional divider.      */
  linflexlp->LINIBRR.R = (uint16_t)(div >> 4);  /* Integer divider.         */

  if ((ldp->config->mode == LIN_MODE_SLAVE) && (ldp->config->filters != NULL)) {
    while (i < LIN_NUM_FILTERS) {
      LinFilter *p = ldp->config->filters[i];
      if (p != NULL) {
        spc5_linflex_set_filter(ldp, i, p);
        if ( p->mode == LIN_MASK_MODE ) { /* The filter 2n is in Mask mode, then skip the next one because the */
          i += 1U;                        /* Mask value has been already set in filter 2n+1.                   */
        }
      }
      i += 1U;
    }
  }

#if 0
  if (ldp->config->loopBack)
    /* Loop Back mode. 0 - Disabled, 1 - Enabled. */
    linflexlp->LINCR1.B.LBKM = 1;
#endif

  if (ldp->config->tx_end_cb != NULL) {
    /* Enable Header transmit interrupt */
    ldp->linflexlp->LINIER.B.HRIE = 1;
  }

  if (ldp->config->rx_end_cb != NULL) {
    /* Enable receive interrupt */
    ldp->linflexlp->LINIER.B.DRIE = 1;
  }

  /* Leaves the configuration mode.*/
  linflexlp->LINCR1.B.INIT = 0;  /* Enter in normal mode */
}

/**
 * @brief   LINFlex de-initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] linflexlp  pointer to a LINFlex I/O block
 */
static void spc5_linflex_stop(volatile struct spc5_linflexl *linflexlp) {

  /* Enters the configuration mode.*/
  linflexlp->LINCR1.R = 1;                      /* INIT bit.                */

  while (0x1000U != (linflexlp->LINSR.R & 0xF000U)) {}

  /* Resets the LINFlex registers.*/
  linflexlp->LINIBRR.R = 0;       /* LIN Integer Baud Rate register         */
  linflexlp->LINFBRR.R = 0;       /* LIN Fractional Baud Rate register      */
  linflexlp->LINSR.R   = 0xFFFF;  /* Clearing LINSR register.               */
  linflexlp->LINCR2.R  = 0;       /* Clear LIN control register 2.          */

  linflexlp->LINIER.R = 0;        /* Disable LIN Interrupts                 */
  
  /* Leaves the configuration mode.*/
  linflexlp->LINCR1.B.INIT = 0;   /* Enter in normal mode                   */
}

#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
static void ld_txi_dma(LinDriver *ldp, uint8_t *tx_buf, size_t len) {

  /* Setup DMA TCD parameters.*/
  edmaChannelSetup(
                   ldp->tx_channel,             /* channel.                 */
                   tx_buf,                      /* src.                     */
                   ((vuint32_t)&ldp->linflexlp->BDRL),/* dst.               */
                   1,                           /* soff, advance by 1.      */
                   1,                           /* doff, advance by 1.      */
                   0,                           /* ssize, 8 bits transfers. */
                   0,                           /* dsize, 8 bits transfers. */
                   len,                         /* len bytes                */
                   1,                           /* iter.                    */
                   0,                           /* slast, no source adjust. */
                   0,                           /* dlast, no dest.adjust.   */
                   EDMA_TCD_MODE_DREQ | EDMA_TCD_MODE_INT_END | EDMA_TCD_MODE_START); /* mode.    */

}
#endif

static void ld_txi(LinDriver *ldp, uint8_t *tx_buf, size_t len) {
  volatile struct spc5_linflexl *linflexlp = ldp->linflexlp;
  uint8_t gtFour = 0;
  uint16_t i;
  union tmpData {
    uint32_t w;
    uint8_t  b[4];
  } tData;

  while (len > 0U) {
    /* Clean temporary buffer before transmit data. */
    for(i = 0; i < 4U; i++) {
      tData.b[i] = 0;
    }

    i = 0;
    /* Fill temporary buffer with the characters to transmit (tx_buf). */
    while ((i < len) && (i < 4U)) {
      tData.b[i++] = *tx_buf++;
    }

    /* Transfer temporary buffer to Buffer Data Register. */
    if (gtFour == 0U) {
      linflexlp->BDRL.R =  tData.w;
      gtFour = 1;
    } else {
      linflexlp->BDRM.R =  tData.w;
    }

    len -= 4U;
  }
}

/**
 * @brief   Common TXI IRQ handler.
 *
 * @param[in] ldp       pointer to a @p LinDriver object
 */
static void spc5xx_lin_txi_interrupt(uint32_t ildp) {
  uint32_t status;
  LinDriver *ldp = (LinDriver *) ildp;
  volatile struct spc5_linflexl *linflexlp = ldp->linflexlp;

  status = linflexlp->LINSR.R;

  if ((status & SPC5_LINSR_HRF) != 0U) {

    /* Slave transmit mode */

    if (ldp->config->tx_end_cb != NULL) {

      uint16_t len;
      uint8_t tx_buf[LIN_DATA_LEN];

      /* Call the related callback.
       * tx_buf will be filled by this callback
       */
      len = (uint16_t)ldp->config->tx_end_cb(ldp,
                      linflexlp->BIDR.B.ID,
                      tx_buf,
                      (uint16_t)linflexlp->BIDR.B.DFL + 1U);

      if (len != 0U) {

         /* Check if DMA is enabled.*/
 #if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
        /* Check if the LinFlex support DMA.*/
        if ((ldp->dma_supported == TRUE) && (ldp->config->dma_enable == TRUE)) {
          ld_txi_dma(ldp, tx_buf, len);
        } else {
          ld_txi(ldp, tx_buf, len);
        }

#else
        ld_txi(ldp, tx_buf, len);
#endif
        /* Trigger Data transmission*/
        linflexlp->LINCR2.B.DTRQ = 1;
      }
    }

    /* Acknowledge header received interrupt */
    linflexlp->LINSR.B.HRF = 1;

  } else {

    /* Master transmit mode */

    /* Acknowledge transmit interrupt */
    linflexlp->LINSR.B.DTF = 1;

    /* Disable transmit interrupt */
    linflexlp->LINIER.B.DTIE = 0;

    /* Call the related callback.*/
    if (ldp->config->tx_end_cb != NULL) {
      /* Ignoring return value */
      (void)ldp->config->tx_end_cb(ldp, ldp->tx_id_msg, ldp->tx_buf, ldp->tx_len);
    }

    SPC5_LIN_TX_DONE(ldp);
  }
}

#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
static void ld_rxi_dma(LinDriver *ldp, uint8_t *rx_buf, size_t len) {

  /* Setup DMA TCD parameters.*/
  edmaChannelSetup(
                   ldp->rx_channel,             /* channel.                 */
                   ((vuint32_t)&ldp->linflexlp->BDRL), /* src.              */
                   rx_buf,                      /* dst.                     */
                   1,                           /* soff, advance by 1.      */
                   1,                           /* doff, advance by 1.      */
                   0,                           /* ssize, 8 bits transfers. */
                   0,                           /* dsize, 8 bits transfers. */
                   len,                         /* len bytes.               */
                   1,                           /* iter.                    */
                   0,                           /* slast, no source adjust. */
                   0,                           /* dlast, no dest.adjust.   */
                   EDMA_TCD_MODE_DREQ | EDMA_TCD_MODE_INT_END | EDMA_TCD_MODE_START); /* mode.    */

}
#endif

static void ld_rxi(LinDriver *ldp, uint8_t *rx_buf, size_t len) {
  volatile struct spc5_linflexl *linflexlp = ldp->linflexlp;
  uint8_t gtFour = 0;
  uint16_t i;
  union tmpData {
    uint32_t w;
    uint8_t  b[4];
  } tData;

  while (len > 0U) {
    /* Clean temporary buffer before to receive data. */
    for(i = 0; i < 4U; i++) {
      tData.b[i] = 0;
    }

    /* Transfer Buffer Data Register to temporary data. */
    if (gtFour == 0U) {
      tData.w = linflexlp->BDRL.R;
      gtFour = 1;
    } else {
      tData.w = linflexlp->BDRM.R;
    }

    i = 0;
    /* Fill RX buffer (rx_buf) from temporary data buffer with the character to receive. */
    while ((i < len) && (i < 4U)) {
      *rx_buf++ = tData.b[i++];
    }

    len -= 4U;
  }
}

/**
 * @brief   Common RXI IRQ handler.
 *
 * @param[in] ldp       pointer to a @p LinDriver object
 */
static void spc5xx_lin_rxi_interrupt(uint32_t ildp) {
  LinDriver *ldp = (LinDriver *) ildp;
  volatile struct spc5_linflexl *linflexlp = ldp->linflexlp;
  uint8_t *pbuf;
  uint16_t len;
  uint8_t id_msg;
  uint8_t rx_buf[LIN_DATA_LEN];

  /* Acknowledge receive interrupt */
  linflexlp->LINSR.B.DRF = 1;

  /* Retrieve message id */
  id_msg = linflexlp->BIDR.B.ID;

  /* Retrieve message data length */
  len = (uint16_t)linflexlp->BIDR.B.DFL + 1U;

  if (ldp->config->mode == LIN_MODE_MASTER) {

	 /* Master receive */

    /* Disable receive interrupt */
    linflexlp->LINIER.B.DRIE = 0;

    /* Check the size of the data buffer */
    if (ldp->rx_len < len) {
      len = ldp->rx_len;
    }

    /* Set returned data length */
    ldp->rx_len = len;

    /* Set RX Buffer */
    pbuf = ldp->rx_buf;

  } else {

    /* Slave receive */

    /* Set RX Buffer */
    pbuf = rx_buf;

  }

  /* Check if DMA is enabled.*/
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
  /* Check if the LinFlex support DMA.*/
  if ((ldp->dma_supported == TRUE) && (ldp->config->dma_enable == TRUE)) {
    ld_rxi_dma(ldp, pbuf, len);
  } else {
    ld_rxi(ldp, pbuf, len);
  }
#else
  ld_rxi(ldp, pbuf, len);
#endif

  /* Call the related callback.*/
  if (ldp->config->rx_end_cb != NULL) {
    /* Ignoring return value */
    (void)ldp->config->rx_end_cb(ldp, id_msg, pbuf, len);
  }

  SPC5_LIN_RX_DONE(ldp);
}

/**
 * @brief   Common ERR IRQ handler.
 *
 * @param[in] ldp       pointer to a @p LinDriver object
 */
static void spc5xx_lin_err_interrupt(uint32_t ildp) {
  LinDriver *ldp = (LinDriver *) ildp;

  (void)ldp;

}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level lin driver initialization.
 *
 * @init
 */
void lin_lld_init(void) {

#if (SPC5_LINFLEX0_SETTING == SPC5_LINFLEX_MODE_LIN)
  LD1.linflexlp = &SPC5_LINFLEX0L;
  LD1.linflexhp = &SPC5_LINFLEX0H;
  LD1.clock = SPC5_LINFLEX0_CLK;
  LinflexD1.device = (uint32_t)&LD1;
  LinflexD1.rxi_lincallback = spc5xx_lin_rxi_interrupt;
  LinflexD1.txi_lincallback = spc5xx_lin_txi_interrupt;
  LinflexD1.err_lincallback = spc5xx_lin_err_interrupt;
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
  LD1.dma_supported = SPC5_LINFLEX0_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX0_SETTING */

#if (SPC5_LINFLEX1_SETTING == SPC5_LINFLEX_MODE_LIN)
  LD2.linflexlp = &SPC5_LINFLEX1L;
  LD2.linflexhp = &SPC5_LINFLEX1H;
  LD2.clock = SPC5_LINFLEX1_CLK;
  LinflexD2.device = (uint32_t)&LD2;
  LinflexD2.rxi_lincallback = spc5xx_lin_rxi_interrupt;
  LinflexD2.txi_lincallback = spc5xx_lin_txi_interrupt;
  LinflexD2.err_lincallback = spc5xx_lin_err_interrupt;
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
  LD2.dma_supported = SPC5_LINFLEX1_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX1_SETTING */

#if (SPC5_LINFLEX2_SETTING == SPC5_LINFLEX_MODE_LIN)
  LD3.linflexlp = &SPC5_LINFLEX2L;
  LD3.linflexhp = &SPC5_LINFLEX2H;
  LD3.clock = SPC5_LINFLEX2_CLK;
  LinflexD3.device = (uint32_t)&LD3;
  LinflexD3.rxi_lincallback = spc5xx_lin_rxi_interrupt;
  LinflexD3.txi_lincallback = spc5xx_lin_txi_interrupt;
  LinflexD3.err_lincallback = spc5xx_lin_err_interrupt;
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
  LD3.dma_supported = SPC5_LINFLEX2_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX2_SETTING */

#if (SPC5_LINFLEX3_SETTING == SPC5_LINFLEX_MODE_LIN)
  LD4.linflexlp = &SPC5_LINFLEX3L;
  LD4.linflexhp = &SPC5_LINFLEX3H;
  LD4.clock = SPC5_LINFLEX3_CLK;
  LinflexD4.device = (uint32_t)&LD4;
  LinflexD4.rxi_lincallback = spc5xx_lin_rxi_interrupt;
  LinflexD4.txi_lincallback = spc5xx_lin_txi_interrupt;
  LinflexD4.err_lincallback = spc5xx_lin_err_interrupt;
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
  LD4.dma_supported = SPC5_LINFLEX3_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX3_SETTING */

#if (SPC5_LINFLEX4_SETTING == SPC5_LINFLEX_MODE_LIN)
  LD5.linflexlp = &SPC5_LINFLEX4L;
  LD5.linflexhp = &SPC5_LINFLEX4H;
  LD5.clock = SPC5_LINFLEX4_CLK;
  LinflexD5.device = (uint32_t)&LD5;
  LinflexD5.rxi_lincallback = spc5xx_lin_rxi_interrupt;
  LinflexD5.txi_lincallback = spc5xx_lin_txi_interrupt;
  LinflexD5.err_lincallback = spc5xx_lin_err_interrupt;
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
  LD5.dma_supported = SPC5_LINFLEX4_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX4_SETTING */

#if (SPC5_LINFLEX5_SETTING == SPC5_LINFLEX_MODE_LIN)
  LD6.linflexlp = &SPC5_LINFLEX5L;
  LD6.linflexhp = &SPC5_LINFLEX5H;
  LD6.clock = SPC5_LINFLEX5_CLK;
  LinflexD6.device = (uint32_t)&LD6;
  LinflexD6.rxi_lincallback = spc5xx_lin_rxi_interrupt;
  LinflexD6.txi_lincallback = spc5xx_lin_txi_interrupt;
  LinflexD6.err_lincallback = spc5xx_lin_err_interrupt;
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
  LD6.dma_supported = SPC5_LINFLEX5_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX5_SETTING */

#if (SPC5_LINFLEX6_SETTING == SPC5_LINFLEX_MODE_LIN)
  LD7.linflexlp = &SPC5_LINFLEX6L;
  LD7.linflexhp = &SPC5_LINFLEX6H;
  LD7.clock = SPC5_LINFLEX6_CLK;
  LinflexD7.device = (uint32_t)&LD7;
  LinflexD7.rxi_lincallback = spc5xx_lin_rxi_interrupt;
  LinflexD7.txi_lincallback = spc5xx_lin_txi_interrupt;
  LinflexD7.err_lincallback = spc5xx_lin_err_interrupt;
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
  LD7.dma_supported = SPC5_LINFLEX6_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX6_SETTING */

#if (SPC5_LINFLEX7_SETTING == SPC5_LINFLEX_MODE_LIN)
  LD8.linflexlp = &SPC5_LINFLEX7L;
  LD8.linflexhp = &SPC5_LINFLEX7H;
  LD8.clock = SPC5_LINFLEX7_CLK;
  LinflexD8.device = (uint32_t)&LD8;
  LinflexD8.rxi_lincallback = spc5xx_lin_rxi_interrupt;
  LinflexD8.txi_lincallback = spc5xx_lin_txi_interrupt;
  LinflexD8.err_lincallback = spc5xx_lin_err_interrupt;
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
  LD8.dma_supported = SPC5_LINFLEX7_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX7_SETTING */

#if (SPC5_LINFLEX8_SETTING == SPC5_LINFLEX_MODE_LIN)
  LD9.linflexlp = &SPC5_LINFLEX8L;
  LD9.linflexhp = &SPC5_LINFLEX8H;
  LD9.clock = SPC5_LINFLEX8_CLK;
  LinflexD9.device = (uint32_t)&LD9;
  LinflexD9.rxi_lincallback = spc5xx_lin_rxi_interrupt;
  LinflexD9.txi_lincallback = spc5xx_lin_txi_interrupt;
  LinflexD9.err_lincallback = spc5xx_lin_err_interrupt;
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
  LD9.dma_supported = SPC5_LINFLEX8_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX8_SETTING */

#if (SPC5_LINFLEX9_SETTING == SPC5_LINFLEX_MODE_LIN)
  LD10.linflexlp = &SPC5_LINFLEX9L;
  LD10.linflexhp = &SPC5_LINFLEX9H;
  LD10.clock = SPC5_LINFLEX9_CLK;
  LinflexD10.device = (uint32_t)&LD10;
  LinflexD10.rxi_lincallback = spc5xx_lin_rxi_interrupt;
  LinflexD10.txi_lincallback = spc5xx_lin_txi_interrupt;
  LinflexD10.err_lincallback = spc5xx_lin_err_interrupt;
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
  LD10.dma_supported = SPC5_LINFLEX9_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX9_SETTING */

#if (SPC5_LINFLEX10_SETTING == SPC5_LINFLEX_MODE_LIN)
  LD11.linflexlp = &SPC5_LINFLEX10L;
  LD11.linflexhp = &SPC5_LINFLEX10H;
  LD11.clock = SPC5_LINFLEX10_CLK;
  LinflexD11.device = (uint32_t)&LD11;
  LinflexD11.rxi_lincallback = spc5xx_lin_rxi_interrupt;
  LinflexD11.txi_lincallback = spc5xx_lin_txi_interrupt;
  LinflexD11.err_lincallback = spc5xx_lin_err_interrupt;
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
  LD11.dma_supported = SPC5_LINFLEX10_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX10_SETTING */

#if (SPC5_LINFLEX11_SETTING == SPC5_LINFLEX_MODE_LIN)
  LD12.linflexlp = &SPC5_LINFLEX11L;
  LD12.linflexhp = &SPC5_LINFLEX11H;
  LD12.clock = SPC5_LINFLEX11_CLK;
  LinflexD12.device = (uint32_t)&LD12;
  LinflexD12.rxi_lincallback = spc5xx_lin_rxi_interrupt;
  LinflexD12.txi_lincallback = spc5xx_lin_txi_interrupt;
  LinflexD12.err_lincallback = spc5xx_lin_err_interrupt;
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
  LD12.dma_supported = SPC5_LINFLEX11_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX11_SETTING */

#if (SPC5_LINFLEX12_SETTING == SPC5_LINFLEX_MODE_LIN)
  LD13.linflexlp = &SPC5_LINFLEX12L;
  LD13.linflexhp = &SPC5_LINFLEX12H;
  LD13.clock = SPC5_LINFLEX12_CLK;
  LinflexD13.device = (uint32_t)&LD13;
  LinflexD13.rxi_lincallback = spc5xx_lin_rxi_interrupt;
  LinflexD13.txi_lincallback = spc5xx_lin_txi_interrupt;
  LinflexD13.err_lincallback = spc5xx_lin_err_interrupt;
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
  LD13.dma_supported = SPC5_LINFLEX12_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX12_SETTING */

#if (SPC5_LINFLEX13_SETTING == SPC5_LINFLEX_MODE_LIN)
  LD14.linflexlp = &SPC5_LINFLEX13L;
  LD14.linflexhp = &SPC5_LINFLEX13H;
  LD14.clock = SPC5_LINFLEX13_CLK;
  LinflexD14.device = (uint32_t)&LD14;
  LinflexD14.rxi_lincallback = spc5xx_lin_rxi_interrupt;
  LinflexD14.txi_lincallback = spc5xx_lin_txi_interrupt;
  LinflexD14.err_lincallback = spc5xx_lin_err_interrupt;
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
  LD14.dma_supported = SPC5_LINFLEX13_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX13_SETTING */

#if (SPC5_LINFLEX14_SETTING == SPC5_LINFLEX_MODE_LIN)
  LD15.linflexlp = &SPC5_LINFLEX14L;
  LD15.linflexhp = &SPC5_LINFLEX14H;
  LD15.clock = SPC5_LINFLEX14_CLK;
  LinflexD15.device = (uint32_t)&LD15;
  LinflexD15.rxi_lincallback = spc5xx_lin_rxi_interrupt;
  LinflexD15.txi_lincallback = spc5xx_lin_txi_interrupt;
  LinflexD15.err_lincallback = spc5xx_lin_err_interrupt;
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
  LD15.dma_supported = SPC5_LINFLEX14_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX14_SETTING */

#if (SPC5_LINFLEX15_SETTING == SPC5_LINFLEX_MODE_LIN)
  LD16.linflexlp = &SPC5_LINFLEX15L;
  LD16.linflexhp = &SPC5_LINFLEX15H;
  LD16.clock = SPC5_LINFLEX15_CLK;
  LinflexD16.device = (uint32_t)&LD16;
  LinflexD16.rxi_lincallback = spc5xx_lin_rxi_interrupt;
  LinflexD16.txi_lincallback = spc5xx_lin_txi_interrupt;
  LinflexD16.err_lincallback = spc5xx_lin_err_interrupt;
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
  LD16.dma_supported = SPC5_LINFLEX15_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX15_SETTING */

#if (SPC5_LINFLEX16_SETTING == SPC5_LINFLEX_MODE_LIN)
  LD17.linflexlp = &SPC5_LINFLEX16L;
  LD17.linflexhp = &SPC5_LINFLEX16H;
  LD17.clock = SPC5_LINFLEX16_CLK;
  LinflexD17.device = (uint32_t)&LD17;
  LinflexD17.rxi_lincallback = spc5xx_lin_rxi_interrupt;
  LinflexD17.txi_lincallback = spc5xx_lin_txi_interrupt;
  LinflexD17.err_lincallback = spc5xx_lin_err_interrupt;
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
  LD17.dma_supported = SPC5_LINFLEX16_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX16_SETTING */

#if (SPC5_LINFLEX17_SETTING == SPC5_LINFLEX_MODE_LIN)
  LD18.linflexlp = &SPC5_LINFLEX17L;
  LD18.linflexhp = &SPC5_LINFLEX17H;
  LD18.clock = SPC5_LINFLEX17_CLK;
  LinflexD18.device = (uint32_t)&LD18;
  LinflexD18.rxi_lincallback = spc5xx_lin_rxi_interrupt;
  LinflexD18.txi_lincallback = spc5xx_lin_txi_interrupt;
  LinflexD18.err_lincallback = spc5xx_lin_err_interrupt;
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
  LD18.dma_supported = SPC5_LINFLEX17_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX17_SETTING */

#if (SPC5_LINFLEX18_SETTING == SPC5_LINFLEX_MODE_LIN)
  LD19.linflexlp = &SPC5_LINFLEX18L;
  LD19.linflexhp = &SPC5_LINFLEX18H;
  LD19.clock = SPC5_LINFLEX18_CLK;
  LinflexD19.device = (uint32_t)&LD19;
  LinflexD19.rxi_lincallback = spc5xx_lin_rxi_interrupt;
  LinflexD19.txi_lincallback = spc5xx_lin_txi_interrupt;
  LinflexD19.err_lincallback = spc5xx_lin_err_interrupt;
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
  LD19.dma_supported = SPC5_LINFLEX18_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX18_SETTING */

#if (SPC5_LINFLEX19_SETTING == SPC5_LINFLEX_MODE_LIN)
  LD20.linflexlp = &SPC5_LINFLEX19L;
  LD20.linflexhp = &SPC5_LINFLEX19H;
  LD20.clock = SPC5_LINFLEX19_CLK;
  LinflexD20.device = (uint32_t)&LD20;
  LinflexD20.rxi_lincallback = spc5xx_lin_rxi_interrupt;
  LinflexD20.txi_lincallback = spc5xx_lin_txi_interrupt;
  LinflexD20.err_lincallback = spc5xx_lin_err_interrupt;
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
  LD20.dma_supported = SPC5_LINFLEX19_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX19_SETTING */

#if (SPC5_LINFLEX20_SETTING == SPC5_LINFLEX_MODE_LIN)
  LD21.linflexlp = &SPC5_LINFLEX20L;
  LD21.linflexhp = &SPC5_LINFLEX20H;
  LD21.clock = SPC5_LINFLEX20_CLK;
  LinflexD21.device = (uint32_t)&LD21;
  LinflexD21.rxi_lincallback = spc5xx_lin_rxi_interrupt;
  LinflexD21.txi_lincallback = spc5xx_lin_txi_interrupt;
  LinflexD21.err_lincallback = spc5xx_lin_err_interrupt;
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
  LD21.dma_supported = SPC5_LINFLEX20_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX20_SETTING */

#if (SPC5_LINFLEX21_SETTING == SPC5_LINFLEX_MODE_LIN)
  LD22.linflexlp = &SPC5_LINFLEX21L;
  LD22.linflexhp = &SPC5_LINFLEX21H;
  LD22.clock = SPC5_LINFLEX21_CLK;
  LinflexD22.device = (uint32_t)&LD22;
  LinflexD22.rxi_lincallback = spc5xx_lin_rxi_interrupt;
  LinflexD22.txi_lincallback = spc5xx_lin_txi_interrupt;
  LinflexD22.err_lincallback = spc5xx_lin_err_interrupt;
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
  LD22.dma_supported = SPC5_LINFLEX21_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX21_SETTING */

#if (SPC5_LINFLEX22_SETTING == SPC5_LINFLEX_MODE_LIN)
  LD23.linflexlp = &SPC5_LINFLEX22L;
  LD23.linflexhp = &SPC5_LINFLEX22H;
  LD23.clock = SPC5_LINFLEX22_CLK;
  LinflexD23.device = (uint32_t)&LD23;
  LinflexD23.rxi_lincallback = spc5xx_lin_rxi_interrupt;
  LinflexD23.txi_lincallback = spc5xx_lin_txi_interrupt;
  LinflexD23.err_lincallback = spc5xx_lin_err_interrupt;
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
  LD23.dma_supported = SPC5_LINFLEX22_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX22_SETTING */

#if (SPC5_LINFLEX23_SETTING == SPC5_LINFLEX_MODE_LIN)
  LD24.linflexlp = &SPC5_LINFLEX23L;
  LD24.linflexhp = &SPC5_LINFLEX23H;
  LD24.clock = SPC5_LINFLEX23_CLK;
  LinflexD24.device = (uint32_t)&LD24;
  LinflexD24.rxi_lincallback = spc5xx_lin_rxi_interrupt;
  LinflexD24.txi_lincallback = spc5xx_lin_txi_interrupt;
  LinflexD24.err_lincallback = spc5xx_lin_err_interrupt;
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
  LD24.dma_supported = SPC5_LINFLEX23_DMA_SUPPORTED;
#endif /* (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */
#endif /* SPC5_LINFLEX23_SETTING */

  linflex_lld_init();
}

/**
 * @brief   Low level lin driver configuration and start.
 *
 * @param[in] ldp       pointer to a @p LinDriver object
 * @param[in] config    the architecture-dependent lin driver configuration.
 *                      If this parameter is set to @p NULL then a default
 *                      configuration is used.
 *
 * @api
 */
void lin_lld_start(LinDriver *ldp, const LinConfig *config) {

  if (config == NULL) {
    ldp->config = &default_config;
  } else {
    ldp->config = config;
  }
#if (SPC5_LINFLEX0_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD1 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX0_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_setdma(ldp, linflex0_tx_dma_config, linflex0_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX0_PCTL,
    SPC5_LINFLEX0_START_PCTL);
  }
#endif

#if (SPC5_LINFLEX1_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD2 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX1_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_setdma(ldp, linflex1_tx_dma_config, linflex1_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX1_PCTL,
    SPC5_LINFLEX1_START_PCTL);
  }
#endif

#if (SPC5_LINFLEX2_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD3 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX2_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_setdma(ldp, linflex2_tx_dma_config, linflex2_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX2_PCTL,
    SPC5_LINFLEX2_START_PCTL);
  }
#endif

#if (SPC5_LINFLEX3_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD4 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX3_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_setdma(ldp, linflex3_tx_dma_config, linflex3_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX3_PCTL,
    SPC5_LINFLEX3_START_PCTL);
  }
#endif

#if (SPC5_LINFLEX4_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD5 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX4_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_setdma(ldp, linflex4_tx_dma_config, linflex4_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX4_PCTL,
    SPC5_LINFLEX4_START_PCTL);
  }
#endif

#if (SPC5_LINFLEX5_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD6 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX5_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_setdma(ldp, linflex5_tx_dma_config, linflex5_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX5_PCTL,
    SPC5_LINFLEX5_START_PCTL);
  }
#endif

#if (SPC5_LINFLEX6_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD7 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX6_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_setdma(ldp, linflex6_tx_dma_config, linflex6_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX6_PCTL,
    SPC5_LINFLEX6_START_PCTL);
  }
#endif

#if (SPC5_LINFLEX7_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD8 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX7_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_setdma(ldp, linflex7_tx_dma_config, linflex7_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX7_PCTL,
    SPC5_LINFLEX7_START_PCTL);
  }
#endif

#if (SPC5_LINFLEX8_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD9 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX8_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_setdma(ldp, linflex8_tx_dma_config, linflex8_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX8_PCTL,
    SPC5_LINFLEX8_START_PCTL);
  }
#endif

#if (SPC5_LINFLEX9_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD10 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX9_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_setdma(ldp, linflex9_tx_dma_config, linflex9_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX9_PCTL,
    SPC5_LINFLEX9_START_PCTL);
  }
#endif

#if (SPC5_LINFLEX10_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD11 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX10_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_setdma(ldp, linflex10_tx_dma_config, linflex10_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX10_PCTL,
    SPC5_LINFLEX10_START_PCTL);
  }
#endif

#if (SPC5_LINFLEX11_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD12 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX11_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_setdma(ldp, linflex11_tx_dma_config, linflex11_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX11_PCTL,
    SPC5_LINFLEX11_START_PCTL);
  }
#endif

#if (SPC5_LINFLEX12_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD13 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX12_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_setdma(ldp, linflex12_tx_dma_config, linflex12_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX12_PCTL,
    SPC5_LINFLEX12_START_PCTL);
  }
#endif

#if (SPC5_LINFLEX13_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD14 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX13_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_setdma(ldp, linflex13_tx_dma_config, linflex13_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX13_PCTL,
    SPC5_LINFLEX13_START_PCTL);
  }
#endif

#if (SPC5_LINFLEX14_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD15 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX14_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_setdma(ldp, linflex14_tx_dma_config, linflex14_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX14_PCTL,
    SPC5_LINFLEX14_START_PCTL);
  }
#endif

#if (SPC5_LINFLEX15_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD16 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX15_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_setdma(ldp, linflex15_tx_dma_config, linflex15_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX15_PCTL,
    SPC5_LINFLEX15_START_PCTL);
  }
#endif

#if (SPC5_LINFLEX16_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD17 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX16_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_setdma(ldp, linflex16_tx_dma_config, linflex16_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX16_PCTL,
    SPC5_LINFLEX16_START_PCTL);
  }
#endif

#if (SPC5_LINFLEX17_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD18 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX17_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_setdma(ldp, linflex17_tx_dma_config, linflex17_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX17_PCTL,
    SPC5_LINFLEX17_START_PCTL);
  }
#endif

#if (SPC5_LINFLEX18_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD19 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX18_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_setdma(ldp, linflex18_tx_dma_config, linflex18_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX18_PCTL,
    SPC5_LINFLEX18_START_PCTL);
  }
#endif

#if (SPC5_LINFLEX19_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD20 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX19_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_setdma(ldp, linflex19_tx_dma_config, linflex19_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX19_PCTL,
    SPC5_LINFLEX19_START_PCTL);
  }
#endif

#if (SPC5_LINFLEX20_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD21 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX20_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_setdma(ldp, linflex20_tx_dma_config, linflex20_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX20_PCTL,
    SPC5_LINFLEX20_START_PCTL);
  }
#endif

#if (SPC5_LINFLEX21_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD22 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX21_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_setdma(ldp, linflex21_tx_dma_config, linflex21_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX21_PCTL,
    SPC5_LINFLEX21_START_PCTL);
  }
#endif

#if (SPC5_LINFLEX22_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD23 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX22_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_setdma(ldp, linflex22_tx_dma_config, linflex22_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX22_PCTL,
    SPC5_LINFLEX22_START_PCTL);
  }
#endif

#if (SPC5_LINFLEX23_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD24 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX23_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_setdma(ldp, linflex23_tx_dma_config, linflex23_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX23_PCTL,
    SPC5_LINFLEX23_START_PCTL);
  }
#endif

  spc5_linflex_start(ldp);
}

/**
 * @brief   Low level lin driver stop.
 *
 * @param[in] ldp       pointer to a @p LinDriver object
 *
 * @api
 */
void lin_lld_stop(LinDriver *ldp) {

  spc5_linflex_stop(ldp->linflexlp);

#if (SPC5_LINFLEX0_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD1 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX0_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex0_tx_dma_config, linflex0_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX0_PCTL,
    SPC5_LINFLEX0_STOP_PCTL);
    return;
  }
#endif

#if (SPC5_LINFLEX1_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD2 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX1_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex1_tx_dma_config, linflex1_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX1_PCTL,
    SPC5_LINFLEX1_STOP_PCTL);
    return;
  }
#endif

#if (SPC5_LINFLEX2_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD3 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX2_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex2_tx_dma_config, linflex2_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX2_PCTL,
    SPC5_LINFLEX2_STOP_PCTL);
    return;
  }
#endif

#if (SPC5_LINFLEX3_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD4 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX3_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex3_tx_dma_config, linflex3_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX3_PCTL,
    SPC5_LINFLEX3_STOP_PCTL);
    return;
  }
#endif

#if (SPC5_LINFLEX4_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD5 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX4_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex4_tx_dma_config, linflex4_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX4_PCTL,
    SPC5_LINFLEX4_STOP_PCTL);
    return;
  }
#endif

#if (SPC5_LINFLEX5_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD6 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX5_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex5_tx_dma_config, linflex5_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX5_PCTL,
    SPC5_LINFLEX5_STOP_PCTL);
    return;
  }
#endif

#if (SPC5_LINFLEX6_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD7 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX6_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex6_tx_dma_config, linflex6_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX6_PCTL,
    SPC5_LINFLEX6_STOP_PCTL);
    return;
  }
#endif

#if (SPC5_LINFLEX7_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD8 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX7_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex7_tx_dma_config, linflex7_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX7_PCTL,
    SPC5_LINFLEX7_STOP_PCTL);
    return;
  }
#endif

#if (SPC5_LINFLEX8_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD9 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX8_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex8_tx_dma_config, linflex8_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX8_PCTL,
    SPC5_LINFLEX8_STOP_PCTL);
    return;
  }
#endif

#if (SPC5_LINFLEX9_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD10 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX9_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex9_tx_dma_config, linflex9_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX9_PCTL,
    SPC5_LINFLEX9_STOP_PCTL);
    return;
  }
#endif

#if (SPC5_LINFLEX10_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD11 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX10_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex10_tx_dma_config, linflex10_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX10_PCTL,
    SPC5_LINFLEX10_STOP_PCTL);
    return;
  }
#endif

#if (SPC5_LINFLEX11_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD12 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX11_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex11_tx_dma_config, linflex11_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX11_PCTL,
    SPC5_LINFLEX11_STOP_PCTL);
    return;
  }
#endif

#if (SPC5_LINFLEX12_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD13 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX12_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex12_tx_dma_config, linflex12_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX12_PCTL,
    SPC5_LINFLEX12_STOP_PCTL);
    return;
  }
#endif

#if (SPC5_LINFLEX13_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD14 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX13_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex13_tx_dma_config, linflex13_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX13_PCTL,
    SPC5_LINFLEX13_STOP_PCTL);
    return;
  }
#endif

#if (SPC5_LINFLEX14_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD15 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX14_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex14_tx_dma_config, linflex14_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX14_PCTL,
    SPC5_LINFLEX14_STOP_PCTL);
    return;
  }
#endif

#if (SPC5_LINFLEX15_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD16 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX15_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex15_tx_dma_config, linflex15_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX15_PCTL,
    SPC5_LINFLEX15_STOP_PCTL);
    return;
  }
#endif

#if (SPC5_LINFLEX16_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD17 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX16_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex16_tx_dma_config, linflex16_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX16_PCTL,
    SPC5_LINFLEX16_STOP_PCTL);
    return;
  }
#endif

#if (SPC5_LINFLEX17_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD18 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX17_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex17_tx_dma_config, linflex17_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX17_PCTL,
    SPC5_LINFLEX17_STOP_PCTL);
    return;
  }
#endif

#if (SPC5_LINFLEX18_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD19 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX18_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex18_tx_dma_config, linflex18_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX18_PCTL,
    SPC5_LINFLEX18_STOP_PCTL);
    return;
  }
#endif

#if (SPC5_LINFLEX19_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD20 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX19_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex19_tx_dma_config, linflex19_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX19_PCTL,
    SPC5_LINFLEX19_STOP_PCTL);
    return;
  }
#endif

#if (SPC5_LINFLEX20_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD21 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX20_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex20_tx_dma_config, linflex20_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX20_PCTL,
    SPC5_LINFLEX20_STOP_PCTL);
    return;
  }

#endif

#if (SPC5_LINFLEX21_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD22 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX21_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex21_tx_dma_config, linflex21_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX21_PCTL,
    SPC5_LINFLEX21_STOP_PCTL);
    return;
  }

#endif

#if (SPC5_LINFLEX22_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD23 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX22_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex22_tx_dma_config, linflex22_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX22_PCTL,
    SPC5_LINFLEX22_STOP_PCTL);
    return;
  }

#endif

#if (SPC5_LINFLEX23_SETTING == SPC5_LINFLEX_MODE_LIN)
  if (&LD24 == ldp) {
#if ((SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) && (SPC5_LINFLEX23_DMA_SUPPORTED == TRUE))
    if (ldp->config->dma_enable == TRUE) {
      linflex_lld_resetdma(linflex23_tx_dma_config, linflex23_rx_dma_config);
    }
#endif
    SPCSetPeripheralClockMode(SPC5_LINFLEX23_PCTL,
    SPC5_LINFLEX23_STOP_PCTL);
    return;
  }

#endif

}

static void lin_master_transmit(LinDriver *ldp, uint8_t idMessage, uint8_t *buffer, uint16_t msg_len) {
  volatile struct spc5_linflexl *linflexlp = ldp->linflexlp;

  if ((msg_len == 0U) || (msg_len > 8U)) {
	  return;
  }

  SPC5_LIN_TX_IN_PROGRESS(ldp);

  /* Save message parameters */
  ldp->tx_id_msg = idMessage;
  ldp->tx_buf = buffer;
  ldp->tx_len = msg_len;

  /* Set Header */
  linflexlp->BIDR.R = 0;
  linflexlp->BIDR.B.ID = idMessage & SPC5_BIDR_ID_MASK;
  linflexlp->BIDR.B.DFL = (uint8_t)(((uint32_t)msg_len - 1U) & SPC5_BIDR_DFL_MASK); /* Configure DFL in BIDR */
  linflexlp->BIDR.B.CCS = 0;
  linflexlp->BIDR.B.DIR = (uint8_t)LIN_DIRECTION_TRANSMIT;

  /* Check if DMA is enabled.*/
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
  /* Check if the LinFlex support DMA.*/
  if ((ldp->dma_supported == TRUE) && (ldp->config->dma_enable == TRUE)) {
    ld_txi_dma(ldp, buffer, msg_len);
  } else {
    ld_txi(ldp, buffer, msg_len);
  }
#else
  ld_txi(ldp, buffer, msg_len);
#endif /* (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) */

  /* Enable transmit interrupt */
  linflexlp->LINIER.B.DTIE = 1;

  /* Trigger Header transmission*/
  linflexlp->LINCR2.B.HTRQ = 1;

  SPC5_LIN_WAIT_FOR_TX_COMPLETION(ldp);
}

static uint16_t lin_master_receive(LinDriver *ldp, uint8_t idMessage, uint8_t *buffer, uint16_t msg_len) {
  volatile struct spc5_linflexl *linflexlp = ldp->linflexlp;

  if ((msg_len == 0U) || (msg_len > 8U)) {
    return 0;
  }

  SPC5_LIN_RX_IN_PROGRESS(ldp);

  /* Prepare buffer for receiving data */
  ldp->rx_buf = buffer;
  ldp->rx_len = msg_len;

  /* Prepare message header */
  linflexlp->BIDR.R = 0;
  linflexlp->BIDR.B.ID  = idMessage & SPC5_BIDR_ID_MASK;
  linflexlp->BIDR.B.DFL = (uint8_t)(((uint32_t)msg_len - 1U) & SPC5_BIDR_DFL_MASK); /* Configure DFL in BIDR */
  linflexlp->BIDR.B.CCS = 0;
  linflexlp->BIDR.B.DIR = (uint8_t)LIN_DIRECTION_RECEIVE;

  /* Enable Data Receive interrupt */
  linflexlp->LINIER.B.DRIE = 1;

  /* Trigger Header transmission*/
  linflexlp->LINCR2.B.HTRQ = 1;

  SPC5_LIN_WAIT_FOR_RX_COMPLETION(ldp);

  /* rx_len could have been changed at interrupt time */
  return ldp->rx_len;
}


/**
 * @brief   Low level lin driver transmit for LIN device configured as Master
 *
 * @param[in] ldp       pointer to a @p LinDriver object
 * @param[in] idMessage Id Message for the buffer to transmit.
 * @param[in] buffer    data buffer to transmit
 * @param[in] msg_len   number of bytes to transmit
 *
 * @api
 */
void lin_lld_transmit(LinDriver *ldp, uint8_t idMessage, uint8_t *buffer, uint16_t msg_len) {
  if (ldp->config->mode == LIN_MODE_MASTER) {
    lin_master_transmit(ldp, idMessage, buffer, msg_len);
  }
}


/**
 * @brief   Low level lin driver receive for LIN device configured as Master
 *
 * @param[in]  ldp       pointer to a @p LinDriver object
 * @param[in]  idMessage Id Message for the buffer to receive.
 * @param[out] buffer    pointer to data buffer to receive
 * @param[in]  buf_len   size of input data buffer
 *
 * @return               number of received bytes
 *
 * @api
 */
uint16_t lin_lld_receive(LinDriver *ldp, uint8_t idMessage, uint8_t *buffer, uint16_t buf_len) {
  uint16_t msg_len;

  if (ldp->config->mode == LIN_MODE_MASTER) {
    msg_len = lin_master_receive(ldp, idMessage, buffer, buf_len);
  } else {
    msg_len = 0;
  }

  return msg_len;
}

#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) || defined(__DOXYGEN__)
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
  LinDriver *ldp = (LinDriver *)p;
  /*lint +e9087 */

  (void)channel;

  /* Clearing TX channel state.*/
  edmaChannelStop(ldp->tx_channel);
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
  LinDriver *ldp = (LinDriver *)p;
  /*lint +e9087 */

  (void)channel;

  /* Clearing RX channel state.*/
  edmaChannelStop(ldp->rx_channel);
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
  LinDriver *ldp = (LinDriver *)p;
  /*lint +e9087 */

  (void)channel;
  (void)esr;

  /* Clearing TX/RX channels state.*/
  edmaChannelStop(ldp->tx_channel);
  edmaChannelStop(ldp->rx_channel);

  if (ldp->config->dma_err_cb != NULL) {
    ldp->config->dma_err_cb(ldp);
  }
}
#endif /* (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON) */

#endif /* LLD_USE_LIN */

/** @} */
