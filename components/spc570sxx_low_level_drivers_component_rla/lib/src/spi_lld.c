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
 * @file    spi_lld.c
 * @brief   SPC5xx SPI low level driver code.
 *
 * @addtogroup SPI
 * @{
 */

#include "spi_lld.h"

#if (LLD_USE_SPI == TRUE) || defined(__DOXYGEN__)

/* Some forward declarations.*/
#if SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE
static void spi_serve_rx_dma_irq(edma_channel_t channel, void *p);
#endif

#if SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX
static void spi_serve_tx_dma_irq(edma_channel_t channel, void *p);
#endif

#if SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE
static void spi_serve_dma_error_irq(edma_channel_t channel,
                                    void *p,
                                    uint32_t esr);
#endif

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/* Excluded PUSHR bits.*/
#define DSPI_PUSHR_EXCLUDED_BITS    (SPC5_PUSHR_CTAS_MASK   |               \
                                     SPC5_PUSHR_EOQ         |               \
                                     SPC5_PUSHR_TXDATA_MASK)

#define DSPI_POPR8_ADDRESS(spip)    (((uint32_t)&(spip)->dspi->POPR.R) + 3U)
#define DSPI_POPR16_ADDRESS(spip)   (((uint32_t)&(spip)->dspi->POPR.R) + 2U)

/* Set of macros dealing with the variable number of DMAs depending on
   the chosen mode.*/
#if SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX
#define spi_lld_setdma(spip, tx1_cfg, tx2_cfg, rx_cfg) {                    \
  (spip)->tx1_channel = edmaChannelAllocate(&(tx1_cfg));                    \
  (spip)->tx2_channel = edmaChannelAllocate(&(tx2_cfg));                    \
  (spip)->rx_channel = edmaChannelAllocate(&(rx_cfg));                      \
}
#define spi_lld_resetdma(tx1_cfg, tx2_cfg, rx_cfg) {                        \
  edmaChannelRelease(&(tx1_cfg));                                           \
  edmaChannelRelease(&(tx2_cfg));                                           \
  edmaChannelRelease(&(rx_cfg));                                            \
}
#endif

#if SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_ONLY
#define spi_lld_setdma(spip, tx1_cfg, tx2_cfg, rx_cfg) {                    \
  (spip)->rx_channel = edmaChannelAllocate(&(rx_cfg));                      \
}
#define spi_lld_resetdma(tx1_cfg, tx2_cfg, rx_cfg) {                        \
  edmaChannelRelease(&(rx_cfg));                                            \
}
#endif

#if SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_NONE
#define spi_lld_setdma(spip, tx1_cfg, tx2_cfg, rx_cfg) {                    \
}
#define spi_lld_resetdma(tx1_cfg, tx2_cfg, rx_cfg) {                        \
}
#endif

/**
 * @brief   SPI TX/RX status.
 */
#define SPC5_SPI_TXRX_READY                 0U
#define SPC5_SPI_TXRX_BUSY                  1U

#if (SPI_USE_WAIT == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Sets ready flag.
 * @details This function sets the ready flag when a TX/RX transfer starts.
 *
 * @param[in] spip      pointer to the @p SPI Driver object
 *
 * @notapi
 */
#define SPC5_SPI_TXRX_IN_PROGRESS(spip)                                        \
  (spip)->ready = SPC5_SPI_TXRX_BUSY;

/**
 * @brief   Clears ready flag.
 * @details This function clears the ready flag when a TX/RX transfer is
 *          completed.
 *
 * @param[in] spip      pointer to the @p SPI Driver object
 *
 * @notapi
 */
#define SPC5_SPI_TXRX_DONE(spip)                                               \
  (spip)->ready = SPC5_SPI_TXRX_READY;

#ifdef __ghs__
#pragma ghs ZO
#endif
/**
 * @brief   Waits for TX/RX completion.
 * @details This function waits till the current TX/RX transfer is completed.
 *
 * @param[in] spip      pointer to the @p SPI Driver object
 *
 * @notapi
 */
#define SPC5_SPI_WAIT_FOR_TXRX_COMPLETION(spip)                                \
    while ((spip)->ready == SPC5_SPI_TXRX_BUSY) { ; }
#ifdef __ghs__
#pragma ghs O
#endif
#else /* !SPI_USE_WAIT */
#define SPC5_SPI_TXRX_IN_PROGRESS(spip)
#define SPC5_SPI_TXRX_DONE(spip)
#define SPC5_SPI_WAIT_FOR_TXRX_COMPLETION(spip)
#endif

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   SPID1 driver identifier.
 */
#if (SPC5_SPI_USE_DSPI0 == TRUE) || defined(__DOXYGEN__)
SPIDriver SPID1;
#endif

/**
 * @brief   SPID2 driver identifier.
 */
#if (SPC5_SPI_USE_DSPI1 == TRUE) || defined(__DOXYGEN__)
SPIDriver SPID2;
#endif

/**
 * @brief   SPID3 driver identifier.
 */
#if (SPC5_SPI_USE_DSPI2 == TRUE) || defined(__DOXYGEN__)
SPIDriver SPID3;
#endif

/**
 * @brief   SPID4 driver identifier.
 */
#if (SPC5_SPI_USE_DSPI3 == TRUE) || defined(__DOXYGEN__)
SPIDriver SPID4;
#endif

/**
 * @brief   SPID5 driver identifier.
 */
#if (SPC5_SPI_USE_DSPI4 == TRUE) || defined(__DOXYGEN__)
SPIDriver SPID5;
#endif

/**
 * @brief   SPID6 driver identifier.
 */
#if (SPC5_SPI_USE_DSPI5 == TRUE) || defined(__DOXYGEN__)
SPIDriver SPID6;
#endif

/**
 * @brief   SPID7 driver identifier.
 */
#if (SPC5_SPI_USE_DSPI6 == TRUE) || defined(__DOXYGEN__)
SPIDriver SPID7;
#endif

/**
 * @brief   SPID8 driver identifier.
 */
#if (SPC5_SPI_USE_DSPI7 == TRUE) || defined(__DOXYGEN__)
SPIDriver SPID8;
#endif

/**
 * @brief   SPID9 driver identifier.
 */
#if (SPC5_SPI_USE_DSPI8 == TRUE) || defined(__DOXYGEN__)
SPIDriver SPID9;
#endif

/**
 * @brief   SPID10 driver identifier.
 */
#if (SPC5_SPI_USE_DSPI9 == TRUE) || defined(__DOXYGEN__)
SPIDriver SPID10;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

#if (SPC5_SPI_USE_DSPI0 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for DSPI0 TX1.
 */
static const edma_channel_config_t spi_dspi0_tx1_dma_config = {
  SPC5_SPI_DSPI0_TX1_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX  == TRUE)
  SPC5_DSPI0_TX1_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_DSPI0_TX1_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SPI_DSPI0_DMA_IRQ_PRIO,
  spi_serve_tx_dma_irq, spi_serve_dma_error_irq, &SPID1
};

/**
 * @brief   DMA configuration for DSPI0 TX2.
 */
static const edma_channel_config_t spi_dspi0_tx2_dma_config = {
  SPC5_SPI_DSPI0_TX2_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX == TRUE)
  0,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_DSPI0_TX2_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SPI_DSPI0_DMA_IRQ_PRIO,
  spi_serve_tx_dma_irq, spi_serve_dma_error_irq, &SPID1
};
#endif /* SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX */

#if (SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for DSPI0 RX.
 */
static const edma_channel_config_t spi_dspi0_rx_dma_config = {
  SPC5_SPI_DSPI0_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX == TRUE)
  SPC5_DSPI0_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_DSPI0_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SPI_DSPI0_DMA_IRQ_PRIO,
  spi_serve_rx_dma_irq, spi_serve_dma_error_irq, &SPID1
};
#endif /* SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE */
#endif /* SPC5_SPI_USE_DSPI0 */

#if (SPC5_SPI_USE_DSPI1 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for DSPI1 TX1.
 */
static const edma_channel_config_t spi_dspi1_tx1_dma_config = {
  SPC5_SPI_DSPI1_TX1_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX == TRUE)
  SPC5_DSPI1_TX1_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_DSPI1_TX1_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SPI_DSPI1_DMA_IRQ_PRIO,
  spi_serve_tx_dma_irq, spi_serve_dma_error_irq, &SPID2
};

/**
 * @brief   DMA configuration for DSPI1 TX2.
 */
static const edma_channel_config_t spi_dspi1_tx2_dma_config = {
  SPC5_SPI_DSPI1_TX2_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX == TRUE)
  0,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_DSPI1_TX2_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SPI_DSPI1_DMA_IRQ_PRIO,
  spi_serve_tx_dma_irq, spi_serve_dma_error_irq, &SPID2
};
#endif /* SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX */

#if (SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for DSPI1 RX.
 */
static const edma_channel_config_t spi_dspi1_rx_dma_config = {
  SPC5_SPI_DSPI1_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX == TRUE)
  SPC5_DSPI1_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_DSPI1_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SPI_DSPI1_DMA_IRQ_PRIO,
  spi_serve_rx_dma_irq, spi_serve_dma_error_irq, &SPID2
};
#endif /* SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE */
#endif /* SPC5_SPI_USE_DSPI1 */

#if (SPC5_SPI_USE_DSPI2 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for DSPI2 TX1.
 */
static const edma_channel_config_t spi_dspi2_tx1_dma_config = {
  SPC5_SPI_DSPI2_TX1_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX == TRUE)
  SPC5_DSPI2_TX1_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_DSPI2_TX1_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SPI_DSPI2_DMA_IRQ_PRIO,
  spi_serve_tx_dma_irq, spi_serve_dma_error_irq, &SPID3
};

/**
 * @brief   DMA configuration for DSPI2 TX2.
 */
static const edma_channel_config_t spi_dspi2_tx2_dma_config = {
  SPC5_SPI_DSPI2_TX2_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX == TRUE)
  0,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_DSPI2_TX2_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SPI_DSPI2_DMA_IRQ_PRIO,
  spi_serve_tx_dma_irq, spi_serve_dma_error_irq, &SPID3
};
#endif /* SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX */

#if (SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for DSPI2 RX.
 */
static const edma_channel_config_t spi_dspi2_rx_dma_config = {
  SPC5_SPI_DSPI2_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX == TRUE)
  SPC5_DSPI2_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_DSPI2_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SPI_DSPI2_DMA_IRQ_PRIO,
  spi_serve_rx_dma_irq, spi_serve_dma_error_irq, &SPID3
};
#endif /* SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE */
#endif /* SPC5_SPI_USE_DSPI2 */

#if (SPC5_SPI_USE_DSPI3 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for DSPI3 TX1.
 */
static const edma_channel_config_t spi_dspi3_tx1_dma_config = {
  SPC5_SPI_DSPI3_TX1_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX == TRUE)
  SPC5_DSPI3_TX1_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_DSPI3_TX1_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SPI_DSPI3_DMA_IRQ_PRIO,
  spi_serve_tx_dma_irq, spi_serve_dma_error_irq, &SPID4
};

/**
 * @brief   DMA configuration for DSPI3 TX2.
 */
static const edma_channel_config_t spi_dspi3_tx2_dma_config = {
  SPC5_SPI_DSPI3_TX2_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX == TRUE)
  0,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_DSPI3_TX2_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SPI_DSPI3_DMA_IRQ_PRIO,
  spi_serve_tx_dma_irq, spi_serve_dma_error_irq, &SPID4
};
#endif /* SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX */

#if (SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for DSPI3 RX.
 */
static const edma_channel_config_t spi_dspi3_rx_dma_config = {
  SPC5_SPI_DSPI3_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX == TRUE)
  SPC5_DSPI3_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_DSPI3_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SPI_DSPI3_DMA_IRQ_PRIO,
  spi_serve_rx_dma_irq, spi_serve_dma_error_irq, &SPID4
};
#endif /* SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE */
#endif /* SPC5_SPI_USE_DSPI3 */

#if (SPC5_SPI_USE_DSPI4 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for DSPI4 TX1.
 */
static const edma_channel_config_t spi_dspi4_tx1_dma_config = {
  SPC5_SPI_DSPI4_TX1_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX == TRUE)
  SPC5_DSPI4_TX1_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_DSPI4_TX1_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SPI_DSPI4_DMA_IRQ_PRIO,
  spi_serve_tx_dma_irq, spi_serve_dma_error_irq, &SPID5
};

/**
 * @brief   DMA configuration for DSPI4 TX2.
 */
static const edma_channel_config_t spi_dspi4_tx2_dma_config = {
  SPC5_SPI_DSPI4_TX2_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX == TRUE)
  0,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_DSPI4_TX2_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SPI_DSPI4_DMA_IRQ_PRIO,
  spi_serve_tx_dma_irq, spi_serve_dma_error_irq, &SPID5
};
#endif /* SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX */

#if (SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for DSPI4 RX.
 */
static const edma_channel_config_t spi_dspi4_rx_dma_config = {
  SPC5_SPI_DSPI4_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX == TRUE)
  SPC5_DSPI4_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_DSPI4_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SPI_DSPI4_DMA_IRQ_PRIO,
  spi_serve_rx_dma_irq, spi_serve_dma_error_irq, &SPID5
};
#endif /* SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE */
#endif /* SPC5_SPI_USE_DSPI4 */

#if (SPC5_SPI_USE_DSPI5 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for DSPI5 TX1.
 */
static const edma_channel_config_t spi_dspi5_tx1_dma_config = {
  SPC5_SPI_DSPI5_TX1_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX == TRUE)
  SPC5_DSPI5_TX1_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_DSPI5_TX1_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SPI_DSPI5_DMA_IRQ_PRIO,
  spi_serve_tx_dma_irq, spi_serve_dma_error_irq, &SPID6
};

/**
 * @brief   DMA configuration for DSPI5 TX2.
 */
static const edma_channel_config_t spi_dspi5_tx2_dma_config = {
  SPC5_SPI_DSPI5_TX2_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX == TRUE)
  0,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_DSPI5_TX2_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SPI_DSPI5_DMA_IRQ_PRIO,
  spi_serve_tx_dma_irq, spi_serve_dma_error_irq, &SPID6
};
#endif /* SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX */

#if (SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for DSPI5 RX.
 */
static const edma_channel_config_t spi_dspi5_rx_dma_config = {
  SPC5_SPI_DSPI5_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX == TRUE)
  SPC5_DSPI5_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_DSPI5_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SPI_DSPI5_DMA_IRQ_PRIO,
  spi_serve_rx_dma_irq, spi_serve_dma_error_irq, &SPID6
};
#endif /* SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE */
#endif /* SPC5_SPI_USE_DSPI5 */

#if (SPC5_SPI_USE_DSPI6 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for DSPI6 TX1.
 */
static const edma_channel_config_t spi_dspi6_tx1_dma_config = {
  SPC5_SPI_DSPI6_TX1_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX == TRUE)
  SPC5_DSPI6_TX1_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_DSPI6_TX1_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SPI_DSPI6_DMA_IRQ_PRIO,
  spi_serve_tx_dma_irq, spi_serve_dma_error_irq, &SPID7
};

/**
 * @brief   DMA configuration for DSPI6 TX2.
 */
static const edma_channel_config_t spi_dspi6_tx2_dma_config = {
  SPC5_SPI_DSPI6_TX2_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX == TRUE)
  0,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_DSPI6_TX2_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SPI_DSPI6_DMA_IRQ_PRIO,
  spi_serve_tx_dma_irq, spi_serve_dma_error_irq, &SPID7
};
#endif /* SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX */

#if (SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for DSPI6 RX.
 */
static const edma_channel_config_t spi_dspi6_rx_dma_config = {
  SPC5_SPI_DSPI6_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX == TRUE)
  SPC5_DSPI6_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_DSPI6_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SPI_DSPI6_DMA_IRQ_PRIO,
  spi_serve_rx_dma_irq, spi_serve_dma_error_irq, &SPID7
};
#endif /* SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE */
#endif /* SPC5_SPI_USE_DSPI6 */

#if (SPC5_SPI_USE_DSPI7 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for DSPI7 TX1.
 */
static const edma_channel_config_t spi_dspi7_tx1_dma_config = {
  SPC5_SPI_DSPI7_TX1_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX == TRUE)
  SPC5_DSPI7_TX1_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_DSPI7_TX1_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SPI_DSPI7_DMA_IRQ_PRIO,
  spi_serve_tx_dma_irq, spi_serve_dma_error_irq, &SPID8
};

/**
 * @brief   DMA configuration for DSPI7 TX2.
 */
static const edma_channel_config_t spi_dspi7_tx2_dma_config = {
  SPC5_SPI_DSPI7_TX2_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX == TRUE)
  0,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_DSPI7_TX2_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SPI_DSPI7_DMA_IRQ_PRIO,
  spi_serve_tx_dma_irq, spi_serve_dma_error_irq, &SPID8
};
#endif /* SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX */

#if (SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for DSPI7 RX.
 */
static const edma_channel_config_t spi_dspi7_rx_dma_config = {
  SPC5_SPI_DSPI7_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX == TRUE)
  SPC5_DSPI7_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_DSPI7_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SPI_DSPI7_DMA_IRQ_PRIO,
  spi_serve_rx_dma_irq, spi_serve_dma_error_irq, &SPID8
};
#endif /* SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE */
#endif /* SPC5_SPI_USE_DSPI7 */

#if (SPC5_SPI_USE_DSPI8 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for DSPI8 TX1.
 */
static const edma_channel_config_t spi_dspi8_tx1_dma_config = {
  SPC5_SPI_DSPI8_TX1_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX == TRUE)
  SPC5_DSPI8_TX1_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_DSPI8_TX1_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SPI_DSPI8_DMA_IRQ_PRIO,
  spi_serve_tx_dma_irq, spi_serve_dma_error_irq, &SPID9
};

/**
 * @brief   DMA configuration for DSPI8 TX2.
 */
static const edma_channel_config_t spi_dspi8_tx2_dma_config = {
  SPC5_SPI_DSPI8_TX2_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX == TRUE)
  0,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_DSPI8_TX2_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SPI_DSPI8_DMA_IRQ_PRIO,
  spi_serve_tx_dma_irq, spi_serve_dma_error_irq, &SPID9
};
#endif /* SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX */

#if (SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for DSPI8 RX.
 */
static const edma_channel_config_t spi_dspi8_rx_dma_config = {
  SPC5_SPI_DSPI8_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX == TRUE)
  SPC5_DSPI8_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_DSPI8_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SPI_DSPI8_DMA_IRQ_PRIO,
  spi_serve_rx_dma_irq, spi_serve_dma_error_irq, &SPID9
};
#endif /* SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE */
#endif /* SPC5_SPI_USE_DSPI8 */

#if (SPC5_SPI_USE_DSPI9 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for DSPI9 TX1.
 */
static const edma_channel_config_t spi_dspi9_tx1_dma_config = {
  SPC5_SPI_DSPI9_TX1_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX == TRUE)
  SPC5_DSPI9_TX1_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_DSPI9_TX1_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SPI_DSPI9_DMA_IRQ_PRIO,
  spi_serve_tx_dma_irq, spi_serve_dma_error_irq, &SPID10
};

/**
 * @brief   DMA configuration for DSPI9 TX2.
 */
static const edma_channel_config_t spi_dspi9_tx2_dma_config = {
  SPC5_SPI_DSPI9_TX2_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX == TRUE)
  0,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_DSPI9_TX2_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SPI_DSPI9_DMA_IRQ_PRIO,
  spi_serve_tx_dma_irq, spi_serve_dma_error_irq, &SPID10
};
#endif /* SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX */

#if (SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE) || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for DSPI9 RX.
 */
static const edma_channel_config_t spi_dspi9_rx_dma_config = {
  SPC5_SPI_DSPI9_RX_DMA_CH_ID,
#if (SPC5_EDMA_HAS_MUX == TRUE)
  SPC5_DSPI9_RX_DMA_DEV_ID,
#if (SPC5_EDMA_NUM_OF_MUX > 1U)
  SPC5_DSPI9_RX_DMA_MUX,
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
#endif /* SPC5_EDMA_HAS_MUX */
  SPC5_SPI_DSPI9_DMA_IRQ_PRIO,
  spi_serve_rx_dma_irq, spi_serve_dma_error_irq, &SPID10
};
#endif /* SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE */
#endif /* SPC5_SPI_USE_DSPI9 */

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Initializes the invariant part of the @p SPIDriver structure.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] dspi      the physical DSPI unit to be associated to the object
 *
 * @notapi
 */
static void spi_lld_obj_init(SPIDriver *spip, struct spc5_dspi *dspi) {

  spip->dspi = dspi;
#if (SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX)
  spip->tx1_channel = EDMA_ERROR;
  spip->tx2_channel = EDMA_ERROR;
#endif
#if (SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE)
  spip->rx_channel  = EDMA_ERROR;
#endif
}

/**
 * @brief   DSPI unit setup for transfer.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
static void spi_dspi_start(SPIDriver *spip) {

  spip->dspi->SR.R = 0xFFFFFFFFUL;
  spip->dspi->MCR.B.HALT = 0;
}

/**
 * @brief   DSPI unit transfer stop.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
static void spi_dspi_stop(SPIDriver *spip) {

  /* Stops the DSPI and clears the queues.*/
  spip->dspi->MCR.R |= SPC5_MCR_HALT | SPC5_MCR_CLR_TXF | SPC5_MCR_CLR_RXF;
}

/**
 * @brief   Prefills the TX FIFO with idle frames.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in,out] np    pointer to the number of frames to send, must be
 *                      greater than zero, contains the number of remaining
 *                      frames on return
 *
 * @notapi
 */
static void spi_dspi_prefill_txfifo_idle(SPIDriver *spip, size_t *np) {
  uint32_t cmd = spip->config->pushr;

  while (spip->dspi->SR.B.TXCTR < SPC5_DSPI_FIFO_DEPTH) {
    if (--(*np) == 0U) {
      spip->dspi->PUSHR.R = (SPC5_PUSHR_EOQ | cmd | 0xFFFFUL) & ~SPC5_PUSHR_CONT;
      break;
    }
    spip->dspi->PUSHR.R = cmd | 0x0000FFFFUL;

#if SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_NONE
    /* Emptying the RX FIFO.*/
    while ((spip->rx_cnt > 0U) && (spip->dspi->SR.B.RXCTR > 0U)) {
      uint32_t frame = spip->dspi->POPR.R;
      if (spip->rx_ptr != NULL) {
        if (spip->dspi->CTAR[0].B.FMSZ < 8U) {
          *spip->rx_ptr8++ = (uint8_t)frame;
        } else {
          *spip->rx_ptr16++ = (uint16_t)frame;
        }
      }
      spip->rx_cnt--;
    }
#endif
  }
}

/**
 * @brief   Prefills the TX FIFO using 8 bits frames.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in,out] np    pointer to the number of frames to send, must be
 *                      greater than zero, contains the number of remaining
 *                      frames on return
 * @param[in,out] txpp  pointer to the pointer to the transmit buffer
 *
 * @notapi
 */
static void spi_dspi_prefill_txfifo8(SPIDriver *spip,
                                     size_t *np,
                                     const uint8_t **txpp) {
  uint32_t cmd = spip->config->pushr;
  uint32_t frame;

  while (spip->dspi->SR.B.TXCTR < SPC5_DSPI_FIFO_DEPTH) {
    frame = **txpp;
    (*txpp)++;

    if (--(*np) == 0U) {
      spip->dspi->PUSHR.R = (SPC5_PUSHR_EOQ | cmd | frame) & ~SPC5_PUSHR_CONT;
      break;
    }
    spip->dspi->PUSHR.R = cmd | frame;

#if SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_NONE
    /* Emptying the RX FIFO.*/
    while ((spip->rx_cnt > 0U) && (spip->dspi->SR.B.RXCTR > 0U)) {
      frame = spip->dspi->POPR.R;
      if (spip->rx_ptr != NULL) {
        *spip->rx_ptr8++ = (uint8_t)frame;
      }
      spip->rx_cnt--;
    }
#endif
  }
}

/**
 * @brief   Prefills the TX FIFO using 16 bits frames.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in,out] np    pointer to the number of frames to send, must be
 *                      greater than zero, contains the number of remaining
 *                      frames on return
 * @param[in,out] txpp  pointer to the pointer to the transmit buffer
 *
 * @notapi
 */
static void spi_dspi_prefill_txfifo16(SPIDriver *spip,
                                      size_t *np,
                                      const uint16_t **txpp) {
  uint32_t cmd = spip->config->pushr;
  uint32_t frame;

  while (spip->dspi->SR.B.TXCTR < SPC5_DSPI_FIFO_DEPTH) {
    frame = **txpp;
    (*txpp)++;

    if (--(*np) == 0U) {
      spip->dspi->PUSHR.R = (SPC5_PUSHR_EOQ | cmd | frame) & ~SPC5_PUSHR_CONT;
      break;
    }
    spip->dspi->PUSHR.R = cmd | frame;

#if SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_NONE
    /* Emptying the RX FIFO.*/
    while ((spip->rx_cnt > 0U) && (spip->dspi->SR.B.RXCTR > 0U)) {
      frame = spip->dspi->POPR.R;
      if (spip->rx_ptr != NULL) {
        *spip->rx_ptr16++ = (uint16_t)frame;
      }
      spip->rx_cnt--;
    }
#endif
  }
}

/**
 * @brief   Starts reception using DMA ignoring the received data.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 *
 * @notapi
 */
static void spi_start_rx_ignore(SPIDriver *spip, size_t n) {

#if SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_NONE
  /* Setting up the fields required for operation continuation.*/
  spip->rx_ptr = NULL;
  spip->rx_cnt = n;

  spip->dspi->RSER.B.RFDFRE = 1;

#else /* SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE */
  static uint32_t datasink;
  uint32_t *pdatasink = &datasink;

  edmaChannelSetup(spip->rx_channel,            /* channel.                 */
                   DSPI_POPR8_ADDRESS(spip),    /* src.                     */
                   pdatasink,                   /* dst.                     */
                   0,                           /* soff, do not advance.    */
                   0,                           /* doff, do not advance.    */
                   0,                           /* ssize, 8 bits transfers. */
                   0,                           /* dsize, 8 bits transfers. */
                   1,                           /* nbytes, always one.      */
                   n,                           /* iter.                    */
                   0,                           /* slast.                   */
                   0,                           /* dlast.                   */
                   EDMA_TCD_MODE_DREQ | EDMA_TCD_MODE_INT_END);     /* mode.*/

  /* Starting RX DMA channel.*/
  edmaChannelStart(spip->rx_channel);

  /* Only when Slave Transmit */
  if (spip->dspi->MCR.B.MSTR == 0UL) {
	  spip->dspi->RSER.B.TFFFRE = 1;
  }
#endif /* SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE */
}

/**
 * @brief   Starts reception using DMA for frames up to 8 bits.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @notapi
 */
static void spi_start_rx8(SPIDriver *spip, size_t n, uint8_t *rxbuf) {

#if SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_NONE
  /* Setting up the fields required for operation continuation.*/
  spip->rx_ptr8 = rxbuf;
  spip->rx_cnt  = n;

  spip->dspi->RSER.B.RFDFRE = 1;

#else /* SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE */
  edmaChannelSetup(spip->rx_channel,            /* channel.                 */
                   DSPI_POPR8_ADDRESS(spip),    /* src.                     */
                   rxbuf,                       /* dst.                     */
                   0,                           /* soff, do not advance.    */
                   1,                           /* doff, advance by one.    */
                   0,                           /* ssize, 8 bits transfers. */
                   0,                           /* dsize, 8 bits transfers. */
                   1,                           /* nbytes, always one.      */
                   n,                           /* iter.                    */
                   0,                           /* slast.                   */
                   0,                           /* dlast.                   */
                   EDMA_TCD_MODE_DREQ | EDMA_TCD_MODE_INT_END);     /* mode.*/

  /* Starting RX DMA channel.*/
  edmaChannelStart(spip->rx_channel);
#endif /* SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE */
}

/**
 * @brief   Starts reception using DMA for frames up to 16 bits.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @notapi
 */
static void spi_start_rx16(SPIDriver *spip, size_t n, uint16_t *rxbuf) {

#if SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_NONE
  /* Setting up the fields required for operation continuation.*/
  spip->rx_ptr16 = rxbuf;
  spip->rx_cnt   = n;

  spip->dspi->RSER.B.RFDFRE = 1;

#else /* SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE */
  edmaChannelSetup(spip->rx_channel,            /* channel.                 */
                   DSPI_POPR16_ADDRESS(spip),   /* src.                     */
                   rxbuf,                       /* dst.                     */
                   0,                           /* soff, do not advance.    */
                   2,                           /* doff, advance by two.    */
                   1,                           /* ssize, 16 bits transfers.*/
                   1,                           /* dsize, 16 bits transfers.*/
                   2,                           /* nbytes, always two.      */
                   n,                           /* iter.                    */
                   0,                           /* slast, no source adjust. */
                   0,                           /* dlast.                   */
                   EDMA_TCD_MODE_DREQ | EDMA_TCD_MODE_INT_END); /* mode.    */

  /* Starting RX DMA channel.*/
  edmaChannelStart(spip->rx_channel);
#endif /* SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE */
}

/**
 * @brief   Starts transmission using DMA for frames up to 8 bits.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 *
 * @notapi
 */
static void spi_start_tx_ignore(SPIDriver *spip, size_t n) {

#if SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_RX_AND_TX
  /* Preloading the TX FIFO with as much frames as possible.*/
  spi_dspi_prefill_txfifo_idle(spip, &n);

  /* This is the case where the whole operation can be satisfied using the
     preloading alone.*/
  if (n == 0U) {
    return;
  }

  /* Setting up the fields required for operation continuation.*/
  spip->tx_ptr = NULL;
  spip->tx_cnt = n;

  /* Enabling the TFFF interrupt source for transfer continuation.*/
  spip->dspi->RSER.B.TFFFRE = 1;

#else /* SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX */
  /* Special case when the data to be transmitted can entirely fit into the
     TX FIFO, in this case the TX DMAs are not activated.*/
  if (n <= SPC5_DSPI_FIFO_DEPTH) {
    spi_dspi_prefill_txfifo_idle(spip, &n);
    return;
  }

  /* Preparing the TX intermediate buffer with the fixed part.*/
  spip->tx_cmd = spip->config->pushr | (uint32_t)0xFFFF;

  /* The first frame is pushed by the CPU, then the DMA is activated to
     send the following frames. This should reduce latency on the operation
     start.*/
  spip->dspi->PUSHR.R = spip->tx_last = spip->tx_cmd;

  /* Setting up TX1 DMA TCD parameters for 32 bits transfers.*/
  edmaChannelSetup(spip->tx1_channel,           /* channel.                 */
                   &spip->tx_cmd,               /* src.                     */
                   &spip->dspi->PUSHR.R,        /* dst.                     */
                   0,                           /* soff, do not advance.    */
                   0,                           /* doff, do not advance.    */
                   2,                           /* ssize, 32 bits transfers.*/
                   2,                           /* dsize, 32 bits transfers.*/
                   4,                           /* nbytes, always four.     */
                   n - 2U,                      /* iter.                    */
                   0,                           /* slast, no source adjust. */
                   0,                           /* dlast, no dest.adjust.   */
                   EDMA_TCD_MODE_DREQ | EDMA_TCD_MODE_INT_END); /* mode.    */

  /* Starting TX1 DMA channel.*/
  edmaChannelStart(spip->tx1_channel);
#endif /* SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX */
}

/**
 * @brief   Starts transmission using DMA for frames up to 8 bits.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @notapi
 */
static void spi_start_tx8(SPIDriver *spip, size_t n, const uint8_t *txbuf) {

#if SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_RX_AND_TX
  /* Enabling the EOQF interrupt source for transfer complete.
   * This interrupt is not valid in slave mode.
   */
  if (spip->dspi->MCR.B.MSTR == 1UL) {
	spip->dspi->RSER.B.EOQFRE = 1;
  }

  /* Preloading the TX FIFO with as much frames as possible.*/
  spi_dspi_prefill_txfifo8(spip, &n, &txbuf);

  /* This is the case where the whole operation can be satisfied using the
     preloading alone.*/
  if (n == 0U) {
    return;
  }

  /* Setting up the fields required for operation continuation.*/
  spip->tx_ptr8 = txbuf;
  spip->tx_cnt = n;

  /* Enabling the TFFF interrupt source for transfer continuation.*/
  spip->dspi->RSER.B.TFFFRE = 1;

#else /* SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX */
  uint8_t last_frame;
  const uint8_t *src, *dst;
  
  /* Special case when the data to be transmitted can entirely fit into the
     TX FIFO, in this case the TX DMAs are not activated.*/
  if (n <= SPC5_DSPI_FIFO_DEPTH) {
    spi_dspi_prefill_txfifo8(spip, &n, &txbuf);
    return;
  }

  /* Preparing the TX intermediate buffer with the fixed part.*/
  spip->tx_cmd = spip->config->pushr;

  /* The first frame is pushed immediately, then the DMA is activated to
     send the following frames. This should reduce latency on the operation
     start.*/
  spip->dspi->PUSHR.R = spip->config->pushr | (uint32_t)*txbuf;

  /* The last frame is a special case, will be pushed by the TX FIFO drain
     interrupt handler or the DMA final callback.*/
  last_frame = txbuf[n - 1U];
  spip->tx_last = (uint32_t)last_frame;

  /* At least two frames left, the DMA is enabled in order to handle the
     long transfer, note that the final frame is not pushed by the DMA.*/
  /* Setting up TX1 DMA TCD parameters for 8 bits transfers.*/
  src = txbuf + 1;
  dst = ((const uint8_t *)&spip->tx_cmd) + 3;
  edmaChannelSetupLinked(
                   spip->tx1_channel,           /* channel.                 */
                   spip->tx2_channel,           /* linkch.                  */
				   src,                         /* src.                     */
                   dst,                         /* dst.                     */
                   1,                           /* soff, advance by 1.      */
                   0,                           /* doff, do not advance.    */
                   0,                           /* ssize, 8 bits transfers. */
                   0,                           /* dsize, 8 bits transfers. */
                   1,                           /* nbytes, always one.      */
                   n - 2U,                      /* iter.                    */
                   0,                           /* slast, no source adjust. */
                   0,                           /* dlast, no dest.adjust.   */
                   EDMA_TCD_MODE_DREQ);         /* mode.                    */

  /* Setting up TX2 DMA TCD parameters for 32 bits transfers.*/
  edmaChannelSetup(spip->tx2_channel,           /* channel.                 */
                   &spip->tx_cmd,               /* src.                     */
                   &spip->dspi->PUSHR.R,        /* dst.                     */
                   0,                           /* soff, do not advance.    */
                   0,                           /* doff, do not advance.    */
                   2,                           /* ssize, 32 bits transfers.*/
                   2,                           /* dsize, 32 bits transfers.*/
                   4,                           /* nbytes, always four.     */
                   n - 2U,                      /* iter.                    */
                   0,                           /* slast, no source adjust. */
                   0,                           /* dlast, no dest.adjust.   */
                   EDMA_TCD_MODE_DREQ | EDMA_TCD_MODE_INT_END); /* mode.    */

  /* Starting TX DMA channels.*/
  edmaChannelStart(spip->tx2_channel);
  edmaChannelStart(spip->tx1_channel);
#endif /* SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX */
}

/**
 * @brief   Starts transmission using DMA for frames up to 16 bits.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @notapi
 */
static void spi_start_tx16(SPIDriver *spip, size_t n, const uint16_t *txbuf) {

#if SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_RX_AND_TX
  /* Enabling the EOQF interrupt source for transfer complete.
   * This interrupt is not valid in slave mode.
   */
  if (spip->dspi->MCR.B.MSTR == 1UL) {
	spip->dspi->RSER.B.EOQFRE = 1;
  }

  /* Preloading the TX FIFO with as much frames as possible.*/
  spi_dspi_prefill_txfifo16(spip, &n, &txbuf);

  /* This is the case where the whole operation can be satisfied using the
     preloading alone.*/
  if (n == 0U) {
    return;
  }

  /* Setting up the fields required for operation continuation.*/
  spip->tx_ptr16 = txbuf;
  spip->tx_cnt = n;

  /* Enabling the TFFF interrupt source for transfer continuation.*/
  spip->dspi->RSER.B.TFFFRE = 1;

#else /* SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX */
  uint16_t last_frame;
  const uint16_t *src;
  const uint8_t  *dst;
  
  /* Special case when the data to be transmitted can entirely fit into the
     TX FIFO, in this case the TX DMAs are not activated.*/
  if (n <= SPC5_DSPI_FIFO_DEPTH) {
    spi_dspi_prefill_txfifo16(spip, &n, &txbuf);
    return;
  }

  /* Preparing the TX intermediate buffer with the fixed part.*/
  spip->tx_cmd = spip->config->pushr;

  /* The first frame is pushed immediately, then the DMA is activated to
     send the following frames. This should reduce latency on the operation
     start.*/
  spip->dspi->PUSHR.R = spip->config->pushr | (uint32_t)*txbuf;

  /* The last frame is a special case, will be pushed by the TX FIFO drain
     interrupt handler or the DMA final callback.*/
  last_frame = txbuf[n - 1U];
  spip->tx_last = (uint32_t)last_frame;

  /* At least two frames left, the DMA is enabled in order to handle the
     long transfer, note that the final frame is not pushed by the DMA.*/
  /* Setting up TX1 DMA TCD parameters for 16 bits transfers.*/
  src = txbuf + 1;
  dst = ((const uint8_t *)&spip->tx_cmd) + 2;
  edmaChannelSetupLinked(
                   spip->tx1_channel,           /* channel.                 */
                   spip->tx2_channel,           /* linkch.                  */
                   src,                         /* src.                     */
                   dst,                         /* dst.                     */
                   2,                           /* soff, advance by 2.      */
                   0,                           /* doff, do not advance.    */
                   1,                           /* ssize, 16 bits transfers.*/
                   1,                           /* dsize, 16 bits transfers.*/
                   2,                           /* nbytes, always two.      */
                   n - 2U,                      /* iter.                    */
                   0,                           /* slast, no source adjust. */
                   0,                           /* dlast, no dest.adjust.   */
                   EDMA_TCD_MODE_DREQ);         /* mode.                    */

  /* Setting up TX2 DMA TCD parameters for 32 bits transfers.*/
  edmaChannelSetup(spip->tx2_channel,           /* channel.                 */
                   &spip->tx_cmd,               /* src.                     */
                   &spip->dspi->PUSHR.R,        /* dst.                     */
                   0,                           /* soff, do not advance.    */
                   0,                           /* doff, do not advance.    */
                   2,                           /* ssize, 32 bits transfers.*/
                   2,                           /* dsize, 32 bits transfers.*/
                   4,                           /* nbytes, always four.     */
                   n - 2U,                      /* iter.                    */
                   0,                           /* slast, no source adjust. */
                   0,                           /* dlast, no dest.adjust.   */
                   EDMA_TCD_MODE_DREQ | EDMA_TCD_MODE_INT_END); /* mode.    */

  /* Starting TX DMA channels.*/
  edmaChannelStart(spip->tx2_channel);
  edmaChannelStart(spip->tx1_channel);
#endif /* SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX */
}

#if (SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE) || defined(__DOXYGEN__)
/**
 * @brief   Shared RX DMA events service routine.
 *
 * @param[in] channel   the channel number
 * @param[in] p         parameter for the registered function
 *
 * @notapi
 */
static void spi_serve_rx_dma_irq(edma_channel_t channel, void *p) {
  /*Casting of void pointer needful for using the same DMA driver
    in different drivers. */
  /*lint -e9087 */
  SPIDriver *spip = (SPIDriver *)p;
  /*lint +e9087 */

  /* Clearing RX channel state.*/
  edmaChannelStop(channel);

  /* Stops the transfer.*/
  spi_dspi_stop(spip);

  /* Call the related callback.*/
  if (spip->config->end_cb != NULL) {
      spip->config->end_cb(spip);
  }

  SPC5_SPI_TXRX_DONE(spip);
}
#endif /* SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE */

#if (SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX) || defined(__DOXYGEN__)
/**
 * @brief   Shared TX1/TX2 DMA events service routine.
 *
 * @param[in] channel   the channel number
 * @param[in] p         parameter for the registered function
 *
 * @notapi
 */
static void spi_serve_tx_dma_irq(edma_channel_t channel, void *p) {
  /*Casting of void pointer needful for using the same DMA driver
    in different drivers. */
  /*lint -e9087 */
  SPIDriver *spip = (SPIDriver *)p;
  /*lint +e9087 */

  (void)channel;

  /* Clearing TX channels state.*/
  edmaChannelStop(spip->tx1_channel);
  edmaChannelStop(spip->tx2_channel);

  /* If the TX FIFO is full then the push of the last frame is delegated to
     an interrupt handler else it is performed immediately. Both conditions
     can be true depending on the SPI speed and ISR latency.*/
  if (spip->dspi->SR.B.TFFF != 0U) {
    spip->dspi->PUSHR.R = (spip->config->pushr | spip->tx_last | SPC5_PUSHR_EOQ) &
                          ~SPC5_PUSHR_CONT;
  }
  else {
    spip->dspi->RSER.B.TFFFDIRS = 0;
  }
}
#endif /* SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX */

#if (SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE) || defined(__DOXYGEN__)
/**
 * @brief   Shared ISR for DMA error events.
 *
 * @param[in] channel   the channel number
 * @param[in] p         parameter for the registered function
 * @param[in] esr       content of the ESR register
 *
 * @notapi
 */
static void spi_serve_dma_error_irq(edma_channel_t channel,
                                    void *p,
                                    uint32_t esr) {
  /*Casting of void pointer needful for using the same DMA driver
    in different drivers. */
  /*lint -e9087 */
  SPIDriver *spip = (SPIDriver *)p;
  /*lint +e9087 */

  (void)channel;
  (void)esr;

  /* Stops the transfer.*/
  spi_dspi_stop(spip);

#if SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX
  /* Clearing TX channels state.*/
  edmaChannelStop(spip->tx1_channel);
  edmaChannelStop(spip->tx2_channel);
#endif
  /* Clearing RX channel state.*/
  edmaChannelStop(spip->rx_channel);

  SPC5_SPI_DMA_ERROR_HOOK(spip);
}
#endif /* SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE */

#if (SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_NONE) || defined(__DOXYGEN__)
/**
 * @brief   Shared ISR for EOQF DSPI events.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
static void spi_serve_dspi_eoqf(SPIDriver *spip) {

	/* Emptying the RX FIFO.*/
	while ((spip->rx_cnt > 0U) && (spip->dspi->SR.B.RXCTR > 0U)) {
		uint32_t frame = spip->dspi->POPR.R;
		if (spip->rx_ptr != NULL) {
			if (spip->dspi->CTAR[0].B.FMSZ < 8U) {
				*spip->rx_ptr8++ = (uint8_t)frame;
			} else {
				*spip->rx_ptr16++ = (uint16_t)frame;
			}
		}
		spip->rx_cnt--;
	}

	/* Interrupt served.*/
	spip->dspi->SR.R = SPC5_RSER_EOQF_RE;

	/* Done with transfer, disable interrupt */
	spip->dspi->RSER.B.EOQFRE = 0;

	if (spip->rx_cnt == 0U) {

		/* Stops the transfer.*/
		spi_dspi_stop(spip);

		/* Call the related callback.*/
		if (spip->config->end_cb != NULL) {
			spip->config->end_cb(spip);
		}

		SPC5_SPI_TXRX_DONE(spip);
	}
}


/**
 * @brief   Shared ISR for RFDF DSPI events.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
static void spi_serve_dspi_rfdf(SPIDriver *spip) {

  /* valid only in slave mode */
  uint32_t cs0;

  /* Emptying the RX FIFO.*/
  while ((spip->rx_cnt > 0U) && (spip->dspi->SR.B.RXCTR > 0U)) {
    uint32_t frame = spip->dspi->POPR.R;
    if (spip->rx_ptr != NULL) {
      if (spip->dspi->CTAR[0].B.FMSZ < 8U) {
        *spip->rx_ptr8++ = (uint8_t)frame;
      } else {
        *spip->rx_ptr16++ = (uint16_t)frame;
      }
    }
    spip->rx_cnt--;
  }

  /* Interrupt served.*/
  spip->dspi->SR.R = SPC5_RSER_RFDF_RE;

  if (spip->dspi->MCR.B.MSTR == 1U) {
	/* this DSPI is in master mode */
	cs0 = 0U;
  } else {
	/* this DSPI is in slave mode, actually assuming CS0 is active low */
    cs0 = pal_readpad(spip->config->ssport, spip->config->sspad);
  }

  if ((cs0 != 0U) || (spip->rx_cnt == 0U)) {

	/* Done with transfer, disable interrupt */
	spip->dspi->RSER.B.RFDFRE = 0;

	/* Stops the transfer.*/
    spi_dspi_stop(spip);

    /* Call the related callback.*/
    if (spip->config->end_cb != NULL) {
        spip->config->end_cb(spip);
    }

    SPC5_SPI_TXRX_DONE(spip);
  }
}
#endif /* SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE */

/**
 * @brief   Shared ISR for TFFF DSPI events.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
static void spi_serve_dspi_tfff(SPIDriver *spip) {

#if SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_RX_AND_TX
  if (spip->tx_cnt > 0U) {
	  /* Filling the TX FIFO.*/
	  if (spip->tx_ptr == NULL) {
		  spi_dspi_prefill_txfifo_idle(spip, &spip->tx_cnt);
	  } else {
		  if (spip->dspi->CTAR[0].B.FMSZ < 8U) {
			  spi_dspi_prefill_txfifo8(spip, &spip->tx_cnt, &spip->tx_ptr8);
		  } else {
			  spi_dspi_prefill_txfifo16(spip, &spip->tx_cnt, &spip->tx_ptr16);
		  }
	  }
  }

  /* Interrupt served.*/
  spip->dspi->SR.R = SPC5_RSER_TFFF_RE;

  /* Done with transfer, disable interrupt */
  if (spip->tx_cnt == 0U) {
	  spip->dspi->RSER.B.TFFFRE = 0;
  }
#else
  /* Interrupt served and back to DMA mode.*/
  spip->dspi->RSER.B.TFFFDIRS = 1;
  spip->dspi->SR.B.TFFF = 1;

  /* Pushing last frame.*/
  spip->dspi->PUSHR.R = (spip->config->pushr | spip->tx_last | SPC5_PUSHR_EOQ) &
                         ~SPC5_PUSHR_CONT;
#endif /* SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_RX_AND_TX */
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/
#if (SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_NONE) || defined(__DOXYGEN__)

#if (SPC5_SPI_USE_DSPI0 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_DSPI0_RFDF_HANDLER)
#error "SPC5_DSPI0_RFDF_HANDLER not defined"
#endif
/**
 * @brief   DSPI0 RFDF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_DSPI0_RFDF_HANDLER) {

  IRQ_PROLOGUE();

  spi_serve_dspi_rfdf(&SPID1);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_DSPI0_EOQF_HANDLER)
#error "SPC5_DSPI0_EOQF_HANDLER not defined"
#endif
/**
 * @brief   DSPI0 EOQF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_DSPI0_EOQF_HANDLER) {

  IRQ_PROLOGUE();

  spi_serve_dspi_eoqf(&SPID1);

  IRQ_EPILOGUE();
}
#endif /* SPC5_SPI_USE_DSPI0 */

#if (SPC5_SPI_USE_DSPI1 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_DSPI1_RFDF_HANDLER)
#error "SPC5_DSPI1_RFDF_HANDLER not defined"
#endif
/**
 * @brief   DSPI1 RFDF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_DSPI1_RFDF_HANDLER) {

  IRQ_PROLOGUE();

  spi_serve_dspi_rfdf(&SPID2);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_DSPI1_EOQF_HANDLER)
#error "SPC5_DSPI1_EOQF_HANDLER not defined"
#endif
/**
 * @brief   DSPI1 EOQF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_DSPI1_EOQF_HANDLER) {

  IRQ_PROLOGUE();

  spi_serve_dspi_eoqf(&SPID2);

  IRQ_EPILOGUE();
}
#endif /* SPC5_SPI_USE_DSPI1 */

#if (SPC5_SPI_USE_DSPI2 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_DSPI2_RFDF_HANDLER)
#error "SPC5_DSPI2_RFDF_HANDLER not defined"
#endif
/**
 * @brief   DSPI2 RFDF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_DSPI2_RFDF_HANDLER) {

  IRQ_PROLOGUE();

  spi_serve_dspi_rfdf(&SPID3);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_DSPI2_EOQF_HANDLER)
#error "SPC5_DSPI2_EOQF_HANDLER not defined"
#endif
/**
 * @brief   DSPI2 EOQF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_DSPI2_EOQF_HANDLER) {

  IRQ_PROLOGUE();

  spi_serve_dspi_eoqf(&SPID3);

  IRQ_EPILOGUE();
}
#endif /* SPC5_SPI_USE_DSPI2 */

#if (SPC5_SPI_USE_DSPI3 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_DSPI3_RFDF_HANDLER)
#error "SPC5_DSPI3_RFDF_HANDLER not defined"
#endif
/**
 * @brief   DSPI3 RFDF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_DSPI3_RFDF_HANDLER) {

  IRQ_PROLOGUE();

  spi_serve_dspi_rfdf(&SPID4);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_DSPI3_EOQF_HANDLER)
#error "SPC5_DSPI3_EOQF_HANDLER not defined"
#endif
/**
 * @brief   DSPI3 EOQF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_DSPI3_EOQF_HANDLER) {

  IRQ_PROLOGUE();

  spi_serve_dspi_eoqf(&SPID4);

  IRQ_EPILOGUE();
}
#endif /* SPC5_SPI_USE_DSPI3 */

#if (SPC5_SPI_USE_DSPI4 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_DSPI4_RFDF_HANDLER)
#error "SPC5_DSPI4_RFDF_HANDLER not defined"
#endif
/**
 * @brief   DSPI4 RFDF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_DSPI4_RFDF_HANDLER) {

  IRQ_PROLOGUE();

  spi_serve_dspi_rfdf(&SPID5);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_DSPI4_EOQF_HANDLER)
#error "SPC5_DSPI4_EOQF_HANDLER not defined"
#endif
/**
 * @brief   DSPI4 EOQF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_DSPI4_EOQF_HANDLER) {

  IRQ_PROLOGUE();

  spi_serve_dspi_eoqf(&SPID5);

  IRQ_EPILOGUE();
}
#endif /* SPC5_SPI_USE_DSPI4 */

#if (SPC5_SPI_USE_DSPI5 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_DSPI5_RFDF_HANDLER)
#error "SPC5_DSPI5_RFDF_HANDLER not defined"
#endif
/**
 * @brief   DSPI5 RFDF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_DSPI5_RFDF_HANDLER) {

  IRQ_PROLOGUE();

  spi_serve_dspi_rfdf(&SPID6);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_DSPI5_EOQF_HANDLER)
#error "SPC5_DSPI5_EOQF_HANDLER not defined"
#endif
/**
 * @brief   DSPI5 EOQF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_DSPI5_EOQF_HANDLER) {

  IRQ_PROLOGUE();

  spi_serve_dspi_eoqf(&SPID6);

  IRQ_EPILOGUE();
}
#endif /* SPC5_SPI_USE_DSPI5 */

#if (SPC5_SPI_USE_DSPI6 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_DSPI6_RFDF_HANDLER)
#error "SPC5_DSPI6_RFDF_HANDLER not defined"
#endif
/**
 * @brief   DSPI6 RFDF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_DSPI6_RFDF_HANDLER) {

  IRQ_PROLOGUE();

  spi_serve_dspi_rfdf(&SPID7);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_DSPI6_EOQF_HANDLER)
#error "SPC5_DSPI6_EOQF_HANDLER not defined"
#endif
/**
 * @brief   DSPI6 EOQF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_DSPI6_EOQF_HANDLER) {

  IRQ_PROLOGUE();

  spi_serve_dspi_eoqf(&SPID7);

  IRQ_EPILOGUE();
}
#endif /* SPC5_SPI_USE_DSPI6 */

#if (SPC5_SPI_USE_DSPI7 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_DSPI7_RFDF_HANDLER)
#error "SPC5_DSPI7_RFDF_HANDLER not defined"
#endif
/**
 * @brief   DSPI7 RFDF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_DSPI7_RFDF_HANDLER) {

  IRQ_PROLOGUE();

  spi_serve_dspi_rfdf(&SPID8);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_DSPI7_EOQF_HANDLER)
#error "SPC5_DSPI7_EOQF_HANDLER not defined"
#endif
/**
 * @brief   DSPI7 EOQF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_DSPI7_EOQF_HANDLER) {

  IRQ_PROLOGUE();

  spi_serve_dspi_eoqf(&SPID8);

  IRQ_EPILOGUE();
}
#endif /* SPC5_SPI_USE_DSPI7 */

#if (SPC5_SPI_USE_DSPI8 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_DSPI8_RFDF_HANDLER)
#error "SPC5_DSPI8_RFDF_HANDLER not defined"
#endif
/**
 * @brief   DSPI8 RFDF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_DSPI8_RFDF_HANDLER) {

  IRQ_PROLOGUE();

  spi_serve_dspi_rfdf(&SPID9);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_DSPI8_EOQF_HANDLER)
#error "SPC5_DSPI8_EOQF_HANDLER not defined"
#endif
/**
 * @brief   DSPI8 EOQF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_DSPI8_EOQF_HANDLER) {

  IRQ_PROLOGUE();

  spi_serve_dspi_eoqf(&SPID9);

  IRQ_EPILOGUE();
}
#endif /* SPC5_SPI_USE_DSPI8 */

#if (SPC5_SPI_USE_DSPI9 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_DSPI9_RFDF_HANDLER)
#error "SPC5_DSPI9_RFDF_HANDLER not defined"
#endif
/**
 * @brief   DSPI9 RFDF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_DSPI9_RFDF_HANDLER) {

  IRQ_PROLOGUE();

  spi_serve_dspi_rfdf(&SPID10);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_DSPI9_EOQF_HANDLER)
#error "SPC5_DSPI9_EOQF_HANDLER not defined"
#endif
/**
 * @brief   DSPI9 EOQF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_DSPI9_EOQF_HANDLER) {

  IRQ_PROLOGUE();

  spi_serve_dspi_eoqf(&SPID10);

  IRQ_EPILOGUE();
}
#endif /* SPC5_SPI_USE_DSPI9 */
#endif /* SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_NONE */


#if (SPC5_SPI_USE_DSPI0 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_DSPI0_TFFF_HANDLER)
#error "SPC5_DSPI0_TFFF_HANDLER not defined"
#endif
/**
 * @brief   DSPI0 TFFF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_DSPI0_TFFF_HANDLER) {

  IRQ_PROLOGUE();

  spi_serve_dspi_tfff(&SPID1);

  IRQ_EPILOGUE();
}
#endif /* SPC5_SPI_USE_DSPI0 */

#if (SPC5_SPI_USE_DSPI1 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_DSPI1_TFFF_HANDLER)
#error "SPC5_DSPI1_TFFF_HANDLER not defined"
#endif
/**
 * @brief   DSPI1 TFFF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_DSPI1_TFFF_HANDLER) {

  IRQ_PROLOGUE();

  spi_serve_dspi_tfff(&SPID2);

  IRQ_EPILOGUE();
}
#endif /* SPC5_SPI_USE_DSPI1 */

#if (SPC5_SPI_USE_DSPI2 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_DSPI2_TFFF_HANDLER)
#error "SPC5_DSPI2_TFFF_HANDLER not defined"
#endif
/**
 * @brief   DSPI2 TFFF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_DSPI2_TFFF_HANDLER) {

  IRQ_PROLOGUE();

  spi_serve_dspi_tfff(&SPID3);

  IRQ_EPILOGUE();
}
#endif /* SPC5_SPI_USE_DSPI2 */

#if (SPC5_SPI_USE_DSPI3 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_DSPI3_TFFF_HANDLER)
#error "SPC5_DSPI3_TFFF_HANDLER not defined"
#endif
/**
 * @brief   DSPI3 TFFF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_DSPI3_TFFF_HANDLER) {

  IRQ_PROLOGUE();

  spi_serve_dspi_tfff(&SPID4);

  IRQ_EPILOGUE();
}
#endif /* SPC5_SPI_USE_DSPI3 */

#if (SPC5_SPI_USE_DSPI4 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_DSPI4_TFFF_HANDLER)
#error "SPC5_DSPI4_TFFF_HANDLER not defined"
#endif
/**
 * @brief   DSPI4 TFFF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_DSPI4_TFFF_HANDLER) {

  IRQ_PROLOGUE();

  spi_serve_dspi_tfff(&SPID5);

  IRQ_EPILOGUE();
}
#endif /* SPC5_SPI_USE_DSPI4 */

#if (SPC5_SPI_USE_DSPI5 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_DSPI5_TFFF_HANDLER)
#error "SPC5_DSPI5_TFFF_HANDLER not defined"
#endif
/**
 * @brief   DSPI5 TFFF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_DSPI5_TFFF_HANDLER) {

  IRQ_PROLOGUE();

  spi_serve_dspi_tfff(&SPID6);

  IRQ_EPILOGUE();
}
#endif /* SPC5_SPI_USE_DSPI5 */

#if (SPC5_SPI_USE_DSPI6 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_DSPI6_TFFF_HANDLER)
#error "SPC5_DSPI6_TFFF_HANDLER not defined"
#endif
/**
 * @brief   DSPI6 TFFF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_DSPI6_TFFF_HANDLER) {

  IRQ_PROLOGUE();

  spi_serve_dspi_tfff(&SPID7);

  IRQ_EPILOGUE();
}
#endif /* SPC5_SPI_USE_DSPI6 */

#if (SPC5_SPI_USE_DSPI7 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_DSPI7_TFFF_HANDLER)
#error "SPC5_DSPI7_TFFF_HANDLER not defined"
#endif
/**
 * @brief   DSPI7 TFFF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_DSPI7_TFFF_HANDLER) {

  IRQ_PROLOGUE();

  spi_serve_dspi_tfff(&SPID8);

  IRQ_EPILOGUE();
}
#endif /* SPC5_SPI_USE_DSPI7 */

#if (SPC5_SPI_USE_DSPI8 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_DSPI8_TFFF_HANDLER)
#error "SPC5_DSPI8_TFFF_HANDLER not defined"
#endif
/**
 * @brief   DSPI8 TFFF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_DSPI8_TFFF_HANDLER) {

  IRQ_PROLOGUE();

  spi_serve_dspi_tfff(&SPID9);

  IRQ_EPILOGUE();
}
#endif /* SPC5_SPI_USE_DSPI8 */

#if (SPC5_SPI_USE_DSPI9 == TRUE) || defined(__DOXYGEN__)
#if !defined(SPC5_DSPI9_TFFF_HANDLER)
#error "SPC5_DSPI9_TFFF_HANDLER not defined"
#endif
/**
 * @brief   DSPI9 TFFF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_DSPI9_TFFF_HANDLER) {

  IRQ_PROLOGUE();

  spi_serve_dspi_tfff(&SPID10);

  IRQ_EPILOGUE();
}
#endif /* SPC5_SPI_USE_DSPI9 */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level SPI driver initialization.
 *
 * @init
 */
void spi_lld_init(void) {

#if (SPC5_SPI_USE_DSPI0 == TRUE)
  /* Driver initialization.*/
  SPC5_DSPI0_ENABLE_CLOCK();
  SPID1.config = NULL;
  spi_lld_obj_init(&SPID1, &SPC5_DSPI0);

  SPC5_DSPI0.MCR.R  = SPC5_MCR_HALT | SPC5_MCR_MDIS | SPC5_SPI_DSPI0_MCR;
#if (SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_NONE)
  INTC_PSR(SPC5_DSPI0_RFDF_NUMBER) = SPC5_SPI_DSPI0_IRQ_PRIO;
  INTC_PSR(SPC5_DSPI0_TFFF_NUMBER) = SPC5_SPI_DSPI0_IRQ_PRIO;
  INTC_PSR(SPC5_DSPI0_EOQF_NUMBER) = SPC5_SPI_DSPI0_IRQ_PRIO;
#else
  INTC_PSR(SPC5_DSPI0_TFFF_NUMBER) = SPC5_SPI_DSPI0_IRQ_PRIO;
#endif
#endif /* SPC5_SPI_USE_DSPI0 */

#if (SPC5_SPI_USE_DSPI1 == TRUE)
  /* Driver initialization.*/
  SPC5_DSPI1_ENABLE_CLOCK();
  SPID2.config = NULL;
  spi_lld_obj_init(&SPID2, &SPC5_DSPI1);

  SPC5_DSPI1.MCR.R  = SPC5_MCR_HALT | SPC5_MCR_MDIS | SPC5_SPI_DSPI1_MCR;
#if (SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_NONE)
  INTC_PSR(SPC5_DSPI1_RFDF_NUMBER) = SPC5_SPI_DSPI1_IRQ_PRIO;
  INTC_PSR(SPC5_DSPI1_TFFF_NUMBER) = SPC5_SPI_DSPI1_IRQ_PRIO;
  INTC_PSR(SPC5_DSPI1_EOQF_NUMBER) = SPC5_SPI_DSPI1_IRQ_PRIO;
#else
  INTC_PSR(SPC5_DSPI1_TFFF_NUMBER) = SPC5_SPI_DSPI1_IRQ_PRIO;
#endif
#endif /* SPC5_SPI_USE_DSPI1 */

#if (SPC5_SPI_USE_DSPI2 == TRUE)
  /* Driver initialization.*/
  SPC5_DSPI2_ENABLE_CLOCK();
  SPID3.config = NULL;
  spi_lld_obj_init(&SPID3, &SPC5_DSPI2);

  SPC5_DSPI2.MCR.R  = SPC5_MCR_HALT | SPC5_MCR_MDIS | SPC5_SPI_DSPI2_MCR;
#if (SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_NONE)
  INTC_PSR(SPC5_DSPI2_RFDF_NUMBER) = SPC5_SPI_DSPI2_IRQ_PRIO;
  INTC_PSR(SPC5_DSPI2_TFFF_NUMBER) = SPC5_SPI_DSPI2_IRQ_PRIO;
  INTC_PSR(SPC5_DSPI2_EOQF_NUMBER) = SPC5_SPI_DSPI2_IRQ_PRIO;
#else
  INTC_PSR(SPC5_DSPI2_TFFF_NUMBER) = SPC5_SPI_DSPI2_IRQ_PRIO;
#endif
#endif /* SPC5_SPI_USE_DSPI2 */

#if (SPC5_SPI_USE_DSPI3 == TRUE)
  /* Driver initialization.*/
  SPC5_DSPI3_ENABLE_CLOCK();
  SPID4.config = NULL;
  spi_lld_obj_init(&SPID4, &SPC5_DSPI3);

  SPC5_DSPI3.MCR.R  = SPC5_MCR_HALT | SPC5_MCR_MDIS | SPC5_SPI_DSPI3_MCR;
#if (SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_NONE)
  INTC_PSR(SPC5_DSPI3_RFDF_NUMBER) = SPC5_SPI_DSPI3_IRQ_PRIO;
  INTC_PSR(SPC5_DSPI3_TFFF_NUMBER) = SPC5_SPI_DSPI3_IRQ_PRIO;
  INTC_PSR(SPC5_DSPI3_EOQF_NUMBER) = SPC5_SPI_DSPI3_IRQ_PRIO;
#else
  INTC_PSR(SPC5_DSPI3_TFFF_NUMBER) = SPC5_SPI_DSPI3_IRQ_PRIO;
#endif
#endif /* SPC5_SPI_USE_DSPI3 */

#if (SPC5_SPI_USE_DSPI4 == TRUE)
  /* Driver initialization.*/
  SPC5_DSPI4_ENABLE_CLOCK();
  SPID5.config = NULL;
  spi_lld_obj_init(&SPID5, &SPC5_DSPI4);

  SPC5_DSPI4.MCR.R  = SPC5_MCR_HALT | SPC5_MCR_MDIS | SPC5_SPI_DSPI4_MCR;
#if (SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_NONE)
  INTC_PSR(SPC5_DSPI4_RFDF_NUMBER) = SPC5_SPI_DSPI4_IRQ_PRIO;
  INTC_PSR(SPC5_DSPI4_TFFF_NUMBER) = SPC5_SPI_DSPI4_IRQ_PRIO;
  INTC_PSR(SPC5_DSPI4_EOQF_NUMBER) = SPC5_SPI_DSPI4_IRQ_PRIO;
#else
  INTC_PSR(SPC5_DSPI4_TFFF_NUMBER) = SPC5_SPI_DSPI4_IRQ_PRIO;
#endif
#endif /* SPC5_SPI_USE_DSPI4 */

#if (SPC5_SPI_USE_DSPI5 == TRUE)
  /* Driver initialization.*/
  SPC5_DSPI5_ENABLE_CLOCK();
  SPID6.config = NULL;
  spi_lld_obj_init(&SPID6, &SPC5_DSPI5);

  SPC5_DSPI5.MCR.R  = SPC5_MCR_HALT | SPC5_MCR_MDIS | SPC5_SPI_DSPI5_MCR;
#if (SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_NONE)
  INTC_PSR(SPC5_DSPI5_RFDF_NUMBER) = SPC5_SPI_DSPI5_IRQ_PRIO;
  INTC_PSR(SPC5_DSPI5_TFFF_NUMBER) = SPC5_SPI_DSPI5_IRQ_PRIO;
  INTC_PSR(SPC5_DSPI5_EOQF_NUMBER) = SPC5_SPI_DSPI5_IRQ_PRIO;
#else
  INTC_PSR(SPC5_DSPI5_TFFF_NUMBER) = SPC5_SPI_DSPI5_IRQ_PRIO;
#endif
#endif /* SPC5_SPI_USE_DSPI5 */

#if (SPC5_SPI_USE_DSPI6 == TRUE)
  /* Driver initialization.*/
  SPC5_DSPI6_ENABLE_CLOCK();
  SPID7.config = NULL;
  spi_lld_obj_init(&SPID7, &SPC5_DSPI6);

  SPC5_DSPI6.MCR.R  = SPC5_MCR_HALT | SPC5_MCR_MDIS | SPC5_SPI_DSPI6_MCR;
#if (SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_NONE)
  INTC_PSR(SPC5_DSPI6_RFDF_NUMBER) = SPC5_SPI_DSPI6_IRQ_PRIO;
  INTC_PSR(SPC5_DSPI6_TFFF_NUMBER) = SPC5_SPI_DSPI6_IRQ_PRIO;
  INTC_PSR(SPC5_DSPI6_EOQF_NUMBER) = SPC5_SPI_DSPI6_IRQ_PRIO;
#else
  INTC_PSR(SPC5_DSPI6_TFFF_NUMBER) = SPC5_SPI_DSPI6_IRQ_PRIO;
#endif
#endif /* SPC5_SPI_USE_DSPI6 */

#if (SPC5_SPI_USE_DSPI7 == TRUE)
  /* Driver initialization.*/
  SPC5_DSPI7_ENABLE_CLOCK();
  SPID8.config = NULL;
  spi_lld_obj_init(&SPID8, &SPC5_DSPI7);

  SPC5_DSPI7.MCR.R  = SPC5_MCR_HALT | SPC5_MCR_MDIS | SPC5_SPI_DSPI7_MCR;
#if (SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_NONE)
  INTC_PSR(SPC5_DSPI7_RFDF_NUMBER) = SPC5_SPI_DSPI7_IRQ_PRIO;
  INTC_PSR(SPC5_DSPI7_TFFF_NUMBER) = SPC5_SPI_DSPI7_IRQ_PRIO;
  INTC_PSR(SPC5_DSPI7_EOQF_NUMBER) = SPC5_SPI_DSPI7_IRQ_PRIO;
#else
  INTC_PSR(SPC5_DSPI7_TFFF_NUMBER) = SPC5_SPI_DSPI7_IRQ_PRIO;
#endif
#endif /* SPC5_SPI_USE_DSPI7 */

#if (SPC5_SPI_USE_DSPI8 == TRUE)
  /* Driver initialization.*/
  SPC5_DSPI8_ENABLE_CLOCK();
  SPID9.config = NULL;
  spi_lld_obj_init(&SPID9, &SPC5_DSPI8);

  SPC5_DSPI8.MCR.R  = SPC5_MCR_HALT | SPC5_MCR_MDIS | SPC5_SPI_DSPI8_MCR;
#if (SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_NONE)
  INTC_PSR(SPC5_DSPI8_RFDF_NUMBER) = SPC5_SPI_DSPI8_IRQ_PRIO;
  INTC_PSR(SPC5_DSPI8_TFFF_NUMBER) = SPC5_SPI_DSPI8_IRQ_PRIO;
  INTC_PSR(SPC5_DSPI8_EOQF_NUMBER) = SPC5_SPI_DSPI8_IRQ_PRIO;
#else
  INTC_PSR(SPC5_DSPI8_TFFF_NUMBER) = SPC5_SPI_DSPI8_IRQ_PRIO;
#endif
#endif /* SPC5_SPI_USE_DSPI8 */

#if (SPC5_SPI_USE_DSPI9 == TRUE)
  /* Driver initialization.*/
  SPC5_DSPI9_ENABLE_CLOCK();
  SPID10.config = NULL;
  spi_lld_obj_init(&SPID10, &SPC5_DSPI9);

  SPC5_DSPI9.MCR.R  = SPC5_MCR_HALT | SPC5_MCR_MDIS | SPC5_SPI_DSPI9_MCR;
#if (SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_NONE)
  INTC_PSR(SPC5_DSPI9_RFDF_NUMBER) = SPC5_SPI_DSPI9_IRQ_PRIO;
  INTC_PSR(SPC5_DSPI9_TFFF_NUMBER) = SPC5_SPI_DSPI9_IRQ_PRIO;
  INTC_PSR(SPC5_DSPI9_EOQF_NUMBER) = SPC5_SPI_DSPI9_IRQ_PRIO;
#else
  INTC_PSR(SPC5_DSPI9_TFFF_NUMBER) = SPC5_SPI_DSPI9_IRQ_PRIO;
#endif
#endif /* SPC5_SPI_USE_DSPI9 */
}

/**
 * @brief   Configures and activates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] config    pointer to the @p SPIConfig object
 *
 * @api
 */
void spi_lld_start(SPIDriver *spip, const SPIConfig *config) {

  spip->config = config;

  /* Enables the peripheral.*/
#if (SPC5_SPI_USE_DSPI0 == TRUE)
  if (&SPID1 == spip) {
    spi_lld_setdma(spip,
                   spi_dspi0_tx1_dma_config,
                   spi_dspi0_tx2_dma_config,
                   spi_dspi0_rx_dma_config)
  }
#endif /* SPC5_SPI_USE_DSPI0 */

#if (SPC5_SPI_USE_DSPI1 == TRUE)
  if (&SPID2 == spip) {
    spi_lld_setdma(spip,
                   spi_dspi1_tx1_dma_config,
                   spi_dspi1_tx2_dma_config,
                   spi_dspi1_rx_dma_config)
  }
#endif /* SPC5_SPI_USE_DSPI1 */

#if (SPC5_SPI_USE_DSPI2 == TRUE)
  if (&SPID3 == spip) {
    spi_lld_setdma(spip,
                   spi_dspi2_tx1_dma_config,
                   spi_dspi2_tx2_dma_config,
                   spi_dspi2_rx_dma_config)
  }
#endif /* SPC5_SPI_USE_DSPI2 */

#if (SPC5_SPI_USE_DSPI3 == TRUE)
  if (&SPID4 == spip) {
    spi_lld_setdma(spip,
                   spi_dspi3_tx1_dma_config,
                   spi_dspi3_tx2_dma_config,
                   spi_dspi3_rx_dma_config)
  }
#endif /* SPC5_SPI_USE_DSPI3 */

#if (SPC5_SPI_USE_DSPI4 == TRUE)
  if (&SPID5 == spip) {
    spi_lld_setdma(spip,
                   spi_dspi4_tx1_dma_config,
                   spi_dspi4_tx2_dma_config,
                   spi_dspi4_rx_dma_config)
  }
#endif /* SPC5_SPI_USE_DSPI4 */

#if (SPC5_SPI_USE_DSPI5 == TRUE)
  if (&SPID6 == spip) {
    spi_lld_setdma(spip,
                   spi_dspi5_tx1_dma_config,
                   spi_dspi5_tx2_dma_config,
                   spi_dspi5_rx_dma_config)
  }
#endif /* SPC5_SPI_USE_DSPI5 */

#if (SPC5_SPI_USE_DSPI6 == TRUE)
  if (&SPID7 == spip) {
    spi_lld_setdma(spip,
                   spi_dspi6_tx1_dma_config,
                   spi_dspi6_tx2_dma_config,
                   spi_dspi6_rx_dma_config)
  }
#endif /* SPC5_SPI_USE_DSPI6 */

#if (SPC5_SPI_USE_DSPI7 == TRUE)
  if (&SPID8 == spip) {
    spi_lld_setdma(spip,
                   spi_dspi7_tx1_dma_config,
                   spi_dspi7_tx2_dma_config,
                   spi_dspi7_rx_dma_config)
  }
#endif /* SPC5_SPI_USE_DSPI7 */

#if (SPC5_SPI_USE_DSPI8 == TRUE)
  if (&SPID9 == spip) {
    spi_lld_setdma(spip,
                   spi_dspi8_tx1_dma_config,
                   spi_dspi8_tx2_dma_config,
                   spi_dspi8_rx_dma_config)
  }
#endif /* SPC5_SPI_USE_DSPI8 */

#if (SPC5_SPI_USE_DSPI9 == TRUE)
  if (&SPID10 == spip) {
    spi_lld_setdma(spip,
                   spi_dspi9_tx1_dma_config,
                   spi_dspi9_tx2_dma_config,
                   spi_dspi9_rx_dma_config)
  }
#endif /* SPC5_SPI_USE_DSPI9 */

  /* Set SPI mode (Master/Slave).*/
  if (spip->config->mode == SPI_MASTER) {
    spip->dspi->MCR.B.MSTR = 1U;
  } else {
    spip->dspi->MCR.B.MSTR = 0U;
  }

  /* Configures the peripheral, the RSER register setting depend on the
     chosen DMA use mode.*/
  spip->dspi->MCR.B.MDIS = 0;
  spip->dspi->CTAR[0].R  = spip->config->ctar0;
#if SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_ONLY
  spip->dspi->RSER.R     = SPC5_RSER_RFDF_RE | SPC5_RSER_RFDF_DIRS;
#endif
#if SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX
  spip->dspi->RSER.R     = SPC5_RSER_TFFF_RE | SPC5_RSER_TFFF_DIRS |
                           SPC5_RSER_RFDF_RE | SPC5_RSER_RFDF_DIRS;
#endif
  spip->dspi->SR.R       = 0xFFFFFFFFUL;
}

/**
 * @brief   Deactivates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @api
 */
void spi_lld_stop(SPIDriver *spip) {

  /* Releases the allocated EDMA channels.*/
#if (SPC5_SPI_USE_DSPI0 == TRUE)
  if (&SPID1 == spip) {
    spi_lld_resetdma(spi_dspi0_tx1_dma_config,
                     spi_dspi0_tx2_dma_config,
                     spi_dspi0_rx_dma_config)
  }
#endif /* SPC5_SPI_USE_DSPI0 */

#if (SPC5_SPI_USE_DSPI1 == TRUE)
  if (&SPID2 == spip) {
    spi_lld_resetdma(spi_dspi1_tx1_dma_config,
                     spi_dspi1_tx2_dma_config,
                     spi_dspi1_rx_dma_config)
  }
#endif /* SPC5_SPI_USE_DSPI1 */

#if (SPC5_SPI_USE_DSPI2 == TRUE)
  if (&SPID3 == spip) {
    spi_lld_resetdma(spi_dspi2_tx1_dma_config,
                     spi_dspi2_tx2_dma_config,
                     spi_dspi2_rx_dma_config)
  }
#endif /* SPC5_SPI_USE_DSPI2 */

#if (SPC5_SPI_USE_DSPI3 == TRUE)
  if (&SPID4 == spip) {
    spi_lld_resetdma(spi_dspi3_tx1_dma_config,
                     spi_dspi3_tx2_dma_config,
                     spi_dspi3_rx_dma_config)
  }
#endif /* SPC5_SPI_USE_DSPI3 */

#if (SPC5_SPI_USE_DSPI4 == TRUE)
  if (&SPID5 == spip) {
    spi_lld_resetdma(spi_dspi4_tx1_dma_config,
                     spi_dspi4_tx2_dma_config,
                     spi_dspi4_rx_dma_config)
  }
#endif /* SPC5_SPI_USE_DSPI4 */

#if (SPC5_SPI_USE_DSPI5 == TRUE)
  if (&SPID6 == spip) {
    spi_lld_resetdma(spi_dspi5_tx1_dma_config,
                     spi_dspi5_tx2_dma_config,
                     spi_dspi5_rx_dma_config)
  }
#endif /* SPC5_SPI_USE_DSPI5 */

#if (SPC5_SPI_USE_DSPI6 == TRUE)
  if (&SPID7 == spip) {
    spi_lld_resetdma(spi_dspi6_tx1_dma_config,
                     spi_dspi6_tx2_dma_config,
                     spi_dspi6_rx_dma_config)
  }
#endif /* SPC5_SPI_USE_DSPI6 */

#if (SPC5_SPI_USE_DSPI7 == TRUE)
  if (&SPID8 == spip) {
    spi_lld_resetdma(spi_dspi7_tx1_dma_config,
                     spi_dspi7_tx2_dma_config,
                     spi_dspi7_rx_dma_config)
  }
#endif /* SPC5_SPI_USE_DSPI7 */

#if (SPC5_SPI_USE_DSPI8 == TRUE)
  if (&SPID9 == spip) {
    spi_lld_resetdma(spi_dspi8_tx1_dma_config,
                     spi_dspi8_tx2_dma_config,
                     spi_dspi8_rx_dma_config)
  }
#endif /* SPC5_SPI_USE_DSPI8 */

#if (SPC5_SPI_USE_DSPI9 == TRUE)
  if (&SPID10 == spip) {
    spi_lld_resetdma(spi_dspi9_tx1_dma_config,
                     spi_dspi9_tx2_dma_config,
                     spi_dspi9_rx_dma_config)
  }
#endif /* SPC5_SPI_USE_DSPI9 */

  /* Resets the peripheral.*/
  spip->dspi->CTAR[0].R  = 0;
  spip->dspi->RSER.R     = 0;
  spip->dspi->SR.R       = 0xFFFFFFFFUL;
  spip->dspi->MCR.R     |= SPC5_MCR_HALT |
                           SPC5_MCR_CLR_TXF | SPC5_MCR_CLR_RXF;
  spip->dspi->MCR.B.MDIS = 1;
}

/**
 * @brief   Asserts the slave select signal and prepares for transfers.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @api
 */
void spi_lld_select(SPIDriver *spip) {

  pal_clearpad(spip->config->ssport, spip->config->sspad);
}

/**
 * @brief   Deasserts the slave select signal.
 * @details The previously selected peripheral is unselected.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @api
 */
void spi_lld_unselect(SPIDriver *spip) {

  pal_setpad(spip->config->ssport, spip->config->sspad);
}

/**
 * @brief   Ignores data on the SPI bus.
 * @details This asynchronous function starts the transmission of a series of
 *          idle words on the SPI bus and ignores the received data.
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be ignored
 *
 * @api
 */
void spi_lld_ignore(SPIDriver *spip, size_t n) {

  /* Starting transfer.*/
  spi_dspi_start(spip);

  SPC5_SPI_TXRX_IN_PROGRESS(spip);

  /* Setting up the DMA channels.*/
  spi_start_rx_ignore(spip, n);
  spi_start_tx_ignore(spip, n);

  SPC5_SPI_WAIT_FOR_TXRX_COMPLETION(spip);
}

/**
 * @brief   Exchanges data on the SPI bus.
 * @details This asynchronous function starts a simultaneous transmit/receive
 *          operation.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below or
 *          equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 * @param[in] txbuf     the pointer to the transmit buffer
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @api
 */
void spi_lld_exchange(SPIDriver *spip, size_t n,
                      const void *txbuf, void *rxbuf) {

  /* Starting transfer.*/
  spi_dspi_start(spip);

  SPC5_SPI_TXRX_IN_PROGRESS(spip);

  /* DMAs require a different setup depending on the frame size.*/
  if (spip->dspi->CTAR[0].B.FMSZ < 8U) {
    spi_start_rx8(spip, n, rxbuf);
    spi_start_tx8(spip, n, txbuf);
  }
  else {
    spi_start_rx16(spip, n, rxbuf);
    spi_start_tx16(spip, n, txbuf);
  }

  SPC5_SPI_WAIT_FOR_TXRX_COMPLETION(spip);
}

/**
 * @brief   Sends data over the SPI bus.
 * @details This asynchronous function starts a transmit operation.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below or
 *          equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @api
 */
void spi_lld_send(SPIDriver *spip, size_t n, const void *txbuf) {

  /* Starting transfer.*/
  spi_dspi_start(spip);

  SPC5_SPI_TXRX_IN_PROGRESS(spip);

#if SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_NONE
  /* Only for slave transmit */
  if (spip->dspi->MCR.B.MSTR == 0UL) {
    spi_start_rx_ignore(spip, n);
  }
#else
  /* Setting up the RX DMA channel.*/
  spi_start_rx_ignore(spip, n);
#endif

  /* DMAs require a different setup depending on the frame size.*/
  if (spip->dspi->CTAR[0].B.FMSZ < 8U) {
    spi_start_tx8(spip, n, txbuf);
  } else {
    spi_start_tx16(spip, n, txbuf);
  }

  SPC5_SPI_WAIT_FOR_TXRX_COMPLETION(spip);
}

/**
 * @brief   Receives data from the SPI bus.
 * @details This asynchronous function starts a receive operation.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below or
 *          equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to receive
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @api
 */
void spi_lld_receive(SPIDriver *spip, size_t n, void *rxbuf) {

  /* Starting transfer.*/
  spi_dspi_start(spip);

  SPC5_SPI_TXRX_IN_PROGRESS(spip);

  /* DMAs require a different setup depending on the frame size.*/
  if (spip->dspi->CTAR[0].B.FMSZ < 8U) {
    spi_start_rx8(spip, n, rxbuf);
  } else {
    spi_start_rx16(spip, n, rxbuf);
  }

#if SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_NONE
  /* Only for master receive */
  if (spip->dspi->MCR.B.MSTR == 1UL) {
	/* Clock-in the data */
    spi_start_tx_ignore(spip, n);
  }
#else
  spi_start_tx_ignore(spip, n);
#endif

  SPC5_SPI_WAIT_FOR_TXRX_COMPLETION(spip);
}

/**
 * @brief   Exchanges one frame using a polled wait.
 * @details This synchronous function exchanges one frame using a polled
 *          synchronization method. This function is useful when exchanging
 *          small amount of data on high speed channels, usually in this
 *          situation is much more efficient just wait for completion using
 *          polling than suspending the thread waiting for an interrupt.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] frame     the data frame to send over the SPI bus
 * @return              The received data frame from the SPI bus.
 *
 * @api
 */
uint16_t spi_lld_polled_exchange(SPIDriver *spip, uint16_t frame) {
  uint32_t popr;

  /* Starting transfer.*/
  spi_dspi_start(spip);

  /* Data exchange.*/
  spip->dspi->PUSHR.R = (SPC5_PUSHR_EOQ | spip->config->pushr |
                         (uint32_t)frame) & ~SPC5_PUSHR_CONT;
  while (spip->dspi->SR.B.RFDF == 0U) {
  }
  popr = spip->dspi->POPR.R;

  /* Stopping transfer.*/
  spi_dspi_stop(spip);

  return (uint16_t)popr;
}

#endif /* LLD_USE_SPI */

/** @} */
