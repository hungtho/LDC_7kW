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
 * @file    spi_lld.h
 * @brief   SPC5xx SPI low level driver header.
 *
 * @addtogroup SPI
 * @{
 */

#ifndef _SPI_LLD_H_
#define _SPI_LLD_H_

#include "spc5_lld.h"
#include "lldconf.h"

#if (LLD_USE_SPI == TRUE) || defined(__DOXYGEN__)

#if SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE
#include "spc5_edma.h"
#endif
#include "spc5_dspi.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   SPID1 driver enable switch.
 * @details If set to @p TRUE the support for DSPI0 is included.
 */
#if !defined(SPC5_SPI_USE_DSPI0) || defined(__DOXYGEN__)
#define SPC5_SPI_USE_DSPI0                  FALSE
#endif

/**
 * @brief   SPID2 driver enable switch.
 * @details If set to @p TRUE the support for DSPI1 is included.
 */
#if !defined(SPC5_SPI_USE_DSPI1) || defined(__DOXYGEN__)
#define SPC5_SPI_USE_DSPI1                  FALSE
#endif

/**
 * @brief   SPID3 driver enable switch.
 * @details If set to @p TRUE the support for DSPI2 is included.
 */
#if !defined(SPC5_SPI_USE_DSPI2) || defined(__DOXYGEN__)
#define SPC5_SPI_USE_DSPI2                  FALSE
#endif

/**
 * @brief   SPID4 driver enable switch.
 * @details If set to @p TRUE the support for DSPI3 is included.
 */
#if !defined(SPC5_SPI_USE_DSPI3) || defined(__DOXYGEN__)
#define SPC5_SPI_USE_DSPI3                  FALSE
#endif

/**
 * @brief   SPID5 driver enable switch.
 * @details If set to @p TRUE the support for DSPI4 is included.
 */
#if !defined(SPC5_SPI_USE_DSPI4) || defined(__DOXYGEN__)
#define SPC5_SPI_USE_DSPI4                  FALSE
#endif

/**
 * @brief   SPID6 driver enable switch.
 * @details If set to @p TRUE the support for DSPI5 is included.
 */
#if !defined(SPC5_SPI_USE_DSPI5) || defined(__DOXYGEN__)
#define SPC5_SPI_USE_DSPI5                  FALSE
#endif

/**
 * @brief   SPID7 driver enable switch.
 * @details If set to @p TRUE the support for DSPI6 is included.
 */
#if !defined(SPC5_SPI_USE_DSPI6) || defined(__DOXYGEN__)
#define SPC5_SPI_USE_DSPI6                  FALSE
#endif

/**
 * @brief   SPID8 driver enable switch.
 * @details If set to @p TRUE the support for DSPI7 is included.
 */
#if !defined(SPC5_SPI_USE_DSPI7) || defined(__DOXYGEN__)
#define SPC5_SPI_USE_DSPI7                  FALSE
#endif

/**
 * @brief   SPID9 driver enable switch.
 * @details If set to @p TRUE the support for DSPI8 is included.
 */
#if !defined(SPC5_SPI_USE_DSPI8) || defined(__DOXYGEN__)
#define SPC5_SPI_USE_DSPI8                  FALSE
#endif

/**
 * @brief   SPID10 driver enable switch.
 * @details If set to @p TRUE the support for DSPI9 is included.
 */
#if !defined(SPC5_SPI_USE_DSPI9) || defined(__DOXYGEN__)
#define SPC5_SPI_USE_DSPI9                  FALSE
#endif

/**
 * @brief   Selects the DMA mode for the SPI driver.
 * @details The driver is able to work using three distinct DMA modes:
 *          - SPC5_SPI_DMA_RX_AND_TX, 3 DMA channels are used for both RX
 *            and TX. Note, the transmission size is limited to 512 frames
 *            in this mode because EDMA limitations.
 *          - SPC5_SPI_DMA_RX_ONLY, 1 DMA channels is used for RX only. TX
 *            is handled using interrupts.
 *          - SPC5_SPI_DMA_NONE, the DMA is not used at all, the drivers
 *            works in a fully interrupt-driven way.
 *          .
 * @note    DMA modes are only possible on those platforms where a DMA
 *          controllers is present.
 */
#if !defined(SPC5_SPI_DMA_MODE) || defined(__DOXYGEN__)
#define SPC5_SPI_DMA_MODE                   SPC5_SPI_DMA_NONE
#endif

/**
 * @brief   DSPI0 MCR PCS defaults.
 */
#if !defined(SPC5_SPI_DSPI0_MCR) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI0_MCR                  (SPC5_MCR_PCSIS0 |              \
                                             SPC5_MCR_PCSIS1 |              \
                                             SPC5_MCR_PCSIS2 |              \
                                             SPC5_MCR_PCSIS3 |              \
                                             SPC5_MCR_PCSIS4 |              \
                                             SPC5_MCR_PCSIS5 |              \
                                             SPC5_MCR_PCSIS6 |              \
                                             SPC5_MCR_PCSIS7)
#endif

/**
 * @brief   DSPI1 MCR PCS defaults.
 */
#if !defined(SPC5_SPI_DSPI1_MCR) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI1_MCR                  (SPC5_MCR_PCSIS0 |              \
                                             SPC5_MCR_PCSIS1 |              \
                                             SPC5_MCR_PCSIS2 |              \
                                             SPC5_MCR_PCSIS3 |              \
                                             SPC5_MCR_PCSIS4 |              \
                                             SPC5_MCR_PCSIS5 |              \
                                             SPC5_MCR_PCSIS6 |              \
                                             SPC5_MCR_PCSIS7)
#endif

/**
 * @brief   DSP2 MCR PCS defaults.
 */
#if !defined(SPC5_SPI_DSPI2_MCR) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI2_MCR                  (SPC5_MCR_PCSIS0 |              \
                                             SPC5_MCR_PCSIS1 |              \
                                             SPC5_MCR_PCSIS2 |              \
                                             SPC5_MCR_PCSIS3 |              \
                                             SPC5_MCR_PCSIS4 |              \
                                             SPC5_MCR_PCSIS5 |              \
                                             SPC5_MCR_PCSIS6 |              \
                                             SPC5_MCR_PCSIS7)
#endif

/**
 * @brief   DSPI3 MCR PCS defaults.
 */
#if !defined(SPC5_SPI_DSPI3_MCR) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI3_MCR                  (SPC5_MCR_PCSIS0 |              \
                                             SPC5_MCR_PCSIS1 |              \
                                             SPC5_MCR_PCSIS2 |              \
                                             SPC5_MCR_PCSIS3 |              \
                                             SPC5_MCR_PCSIS4 |              \
                                             SPC5_MCR_PCSIS5 |              \
                                             SPC5_MCR_PCSIS6 |              \
                                             SPC5_MCR_PCSIS7)
#endif

/**
 * @brief   DSPI4 MCR PCS defaults.
 */
#if !defined(SPC5_SPI_DSPI4_MCR) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI4_MCR                  (SPC5_MCR_PCSIS0 |              \
                                             SPC5_MCR_PCSIS1 |              \
                                             SPC5_MCR_PCSIS2 |              \
                                             SPC5_MCR_PCSIS3 |              \
                                             SPC5_MCR_PCSIS4 |              \
                                             SPC5_MCR_PCSIS5 |              \
                                             SPC5_MCR_PCSIS6 |              \
                                             SPC5_MCR_PCSIS7)
#endif

/**
 * @brief   DSPI5 MCR PCS defaults.
 */
#if !defined(SPC5_SPI_DSPI5_MCR) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI5_MCR                  (SPC5_MCR_PCSIS0 |              \
                                             SPC5_MCR_PCSIS1 |              \
                                             SPC5_MCR_PCSIS2 |              \
                                             SPC5_MCR_PCSIS3 |              \
                                             SPC5_MCR_PCSIS4 |              \
                                             SPC5_MCR_PCSIS5 |              \
                                             SPC5_MCR_PCSIS6 |              \
                                             SPC5_MCR_PCSIS7)
#endif

/**
 * @brief   DSPI6 MCR PCS defaults.
 */
#if !defined(SPC5_SPI_DSPI6_MCR) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI6_MCR                  (SPC5_MCR_PCSIS0 |              \
                                             SPC5_MCR_PCSIS1 |              \
                                             SPC5_MCR_PCSIS2 |              \
                                             SPC5_MCR_PCSIS3 |              \
                                             SPC5_MCR_PCSIS4 |              \
                                             SPC5_MCR_PCSIS5 |              \
                                             SPC5_MCR_PCSIS6 |              \
                                             SPC5_MCR_PCSIS7)
#endif

/**
 * @brief   DSPI7 MCR PCS defaults.
 */
#if !defined(SPC5_SPI_DSPI7_MCR) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI7_MCR                  (SPC5_MCR_PCSIS0 |              \
                                             SPC5_MCR_PCSIS1 |              \
                                             SPC5_MCR_PCSIS2 |              \
                                             SPC5_MCR_PCSIS3 |              \
                                             SPC5_MCR_PCSIS4 |              \
                                             SPC5_MCR_PCSIS5 |              \
                                             SPC5_MCR_PCSIS6 |              \
                                             SPC5_MCR_PCSIS7)
#endif

/**
 * @brief   DSPI8 MCR PCS defaults.
 */
#if !defined(SPC5_SPI_DSPI8_MCR) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI8_MCR                  (SPC5_MCR_PCSIS0 |              \
                                             SPC5_MCR_PCSIS1 |              \
                                             SPC5_MCR_PCSIS2 |              \
                                             SPC5_MCR_PCSIS3 |              \
                                             SPC5_MCR_PCSIS4 |              \
                                             SPC5_MCR_PCSIS5 |              \
                                             SPC5_MCR_PCSIS6 |              \
                                             SPC5_MCR_PCSIS7)
#endif

/**
 * @brief   DSPI9 MCR PCS defaults.
 */
#if !defined(SPC5_SPI_DSPI9_MCR) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI9_MCR                  (SPC5_MCR_PCSIS0 |              \
                                             SPC5_MCR_PCSIS1 |              \
                                             SPC5_MCR_PCSIS2 |              \
                                             SPC5_MCR_PCSIS3 |              \
                                             SPC5_MCR_PCSIS4 |              \
                                             SPC5_MCR_PCSIS5 |              \
                                             SPC5_MCR_PCSIS6 |              \
                                             SPC5_MCR_PCSIS7)
#endif

/**
 * @brief   DSPI0 DMA IRQ priority.
 */
#if !defined(SPC5_SPI_DSPI0_DMA_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI0_DMA_IRQ_PRIO         10
#endif

/**
 * @brief   DSPI1 DMA IRQ priority.
 */
#if !defined(SPC5_SPI_DSPI1_DMA_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI1_DMA_IRQ_PRIO         10
#endif

/**
 * @brief   DSPI2 DMA IRQ priority.
 */
#if !defined(SPC5_SPI_DSPI2_DMA_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI2_DMA_IRQ_PRIO         10
#endif

/**
 * @brief   DSPI3 DMA IRQ priority.
 */
#if !defined(SPC5_SPI_DSPI3_DMA_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI3_DMA_IRQ_PRIO         10
#endif

/**
 * @brief   DSPI4 DMA IRQ priority.
 */
#if !defined(SPC5_SPI_DSPI4_DMA_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI4_DMA_IRQ_PRIO         10
#endif

/**
 * @brief   DSPI5 DMA IRQ priority.
 */
#if !defined(SPC5_SPI_DSPI5_DMA_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI5_DMA_IRQ_PRIO         10
#endif

/**
 * @brief   DSPI6 DMA IRQ priority.
 */
#if !defined(SPC5_SPI_DSPI6_DMA_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI6_DMA_IRQ_PRIO         10
#endif

/**
 * @brief   DSPI7 DMA IRQ priority.
 */
#if !defined(SPC5_SPI_DSPI7_DMA_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI7_DMA_IRQ_PRIO         10
#endif

/**
 * @brief   DSPI8 DMA IRQ priority.
 */
#if !defined(SPC5_SPI_DSPI8_DMA_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI8_DMA_IRQ_PRIO         10
#endif

/**
 * @brief   DSPI9 DMA IRQ priority.
 */
#if !defined(SPC5_SPI_DSPI9_DMA_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI9_DMA_IRQ_PRIO         10
#endif

/**
 * @brief   SPI DMA error hook.
 */
#if !defined(SPC5_SPI_DMA_ERROR_HOOK) || defined(__DOXYGEN__)
#define SPC5_SPI_DMA_ERROR_HOOK(spip)       irqSysHalt()
#endif

/**
 * @brief   DSPI0 IRQ priority.
 */
#if !defined(SPC5_SPI_DSPI0_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI0_IRQ_PRIO             10
#endif

/**
 * @brief   DSPI1 IRQ priority.
 */
#if !defined(SPC5_SPI_DSPI1_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI1_IRQ_PRIO             10
#endif

/**
 * @brief   DSPI2 IRQ priority.
 */
#if !defined(SPC5_SPI_DSPI2_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI2_IRQ_PRIO             10
#endif

/**
 * @brief   DSPI3 IRQ priority.
 */
#if !defined(SPC5_SPI_DSPI3_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI3_IRQ_PRIO             10
#endif

/**
 * @brief   DSPI4 IRQ priority.
 */
#if !defined(SPC5_SPI_DSPI4_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI4_IRQ_PRIO             10
#endif

/**
 * @brief   DSPI5 IRQ priority.
 */
#if !defined(SPC5_SPI_DSPI5_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI5_IRQ_PRIO             10
#endif

/**
 * @brief   DSPI6 IRQ priority.
 */
#if !defined(SPC5_SPI_DSPI6_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI6_IRQ_PRIO             10
#endif

/**
 * @brief   DSPI7 IRQ priority.
 */
#if !defined(SPC5_SPI_DSPI7_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI7_IRQ_PRIO             10
#endif

/**
 * @brief   DSPI8 IRQ priority.
 */
#if !defined(SPC5_SPI_DSPI8_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI8_IRQ_PRIO             10
#endif

/**
 * @brief   DSPI9 IRQ priority.
 */
#if !defined(SPC5_SPI_DSPI9_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI9_IRQ_PRIO             10
#endif

/**
 * @brief   DSPI0 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_SPI_DSPI0_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI0_START_PCTL           (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   DSPI0 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_SPI_DSPI0_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI0_STOP_PCTL            (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   DSPI1 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_SPI_DSPI1_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI1_START_PCTL           (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   DSPI1 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_SPI_DSPI1_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI1_STOP_PCTL            (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   DSPI2 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_SPI_DSPI2_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI2_START_PCTL           (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   DSPI2 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_SPI_DSPI2_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI2_STOP_PCTL            (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   DSPI3 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_SPI_DSPI3_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI3_START_PCTL           (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   DSPI3 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_SPI_DSPI3_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI3_STOP_PCTL            (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   DSPI4 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_SPI_DSPI4_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI4_START_PCTL           (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   DSPI4 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_SPI_DSPI4_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI4_STOP_PCTL            (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   DSPI5 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_SPI_DSPI5_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI5_START_PCTL           (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   DSPI5 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_SPI_DSPI5_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI5_STOP_PCTL            (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   DSPI6 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_SPI_DSPI6_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI6_START_PCTL           (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   DSPI6 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_SPI_DSPI6_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI6_STOP_PCTL            (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   DSPI7 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_SPI_DSPI7_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI7_START_PCTL           (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   DSPI7 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_SPI_DSPI7_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI7_STOP_PCTL            (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   DSPI8 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_SPI_DSPI8_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI8_START_PCTL           (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   DSPI8 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_SPI_DSPI8_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI8_STOP_PCTL            (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   DSPI9 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_SPI_DSPI9_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI9_START_PCTL           (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   DSPI9 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_SPI_DSPI9_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI9_STOP_PCTL            (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !(SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_NONE) &&                            \
    !(SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_ONLY) &&                         \
    !(SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX)
#error "invalid SPC5_SPI_DMA_MODE selected"
#endif

#if (SPC5_HAS_EDMA == FALSE) && (SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE)
#error "SPI with DMA modes are not supported on this device, no DMA found"
#endif

#if (SPC5_SPI_USE_DSPI0 == TRUE) && (SPC5_HAS_DSPI0 == FALSE)
#error "DSPI0 not present in the selected device"
#endif

#if (SPC5_SPI_USE_DSPI1 == TRUE) && (SPC5_HAS_DSPI1 == FALSE)
#error "DSPI1 not present in the selected device"
#endif

#if (SPC5_SPI_USE_DSPI2 == TRUE) && (SPC5_HAS_DSPI2 == FALSE)
#error "DSPI2 not present in the selected device"
#endif

#if (SPC5_SPI_USE_DSPI3 == TRUE) && (SPC5_HAS_DSPI3 == FALSE)
#error "DSPI3 not present in the selected device"
#endif

#if (SPC5_SPI_USE_DSPI4 == TRUE) && (SPC5_HAS_DSPI4 == FALSE)
#error "DSPI4 not present in the selected device"
#endif

#if (SPC5_SPI_USE_DSPI5 == TRUE) && (SPC5_HAS_DSPI5 == FALSE)
#error "DSPI5 not present in the selected device"
#endif

#if (SPC5_SPI_USE_DSPI6 == TRUE) && (SPC5_HAS_DSPI6 == FALSE)
#error "DSPI6 not present in the selected device"
#endif

#if (SPC5_SPI_USE_DSPI7 == TRUE) && (SPC5_HAS_DSPI7 == FALSE)
#error "DSPI7 not present in the selected device"
#endif

#if (SPC5_SPI_USE_DSPI8 == TRUE) && (SPC5_HAS_DSPI8 == FALSE)
#error "DSPI8 not present in the selected device"
#endif

#if (SPC5_SPI_USE_DSPI9 == TRUE) && (SPC5_HAS_DSPI9 == FALSE)
#error "DSPI9 not present in the selected device"
#endif

#if (SPC5_SPI_USE_DSPI0 == FALSE) && (SPC5_SPI_USE_DSPI1 == FALSE) &&       \
    (SPC5_SPI_USE_DSPI2 == FALSE) && (SPC5_SPI_USE_DSPI3 == FALSE) &&       \
    (SPC5_SPI_USE_DSPI4 == FALSE) && (SPC5_SPI_USE_DSPI5 == FALSE) &&       \
    (SPC5_SPI_USE_DSPI6 == FALSE) && (SPC5_SPI_USE_DSPI7 == FALSE) &&       \
    (SPC5_SPI_USE_DSPI8 == FALSE) && (SPC5_SPI_USE_DSPI9 == FALSE)
#error "SPI driver activated but no DSPI peripheral assigned"
#endif

#if (SPC5_HAS_EDMA == TRUE) && (SPC5_SPI_USE_DSPI0 == TRUE)
#if ((SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_ONLY) &&                         \
     (!defined(SPC5_SPI_DSPI0_RX_DMA_CH_ID))) ||                            \
    ((SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX) &&                       \
     (!defined(SPC5_SPI_DSPI0_TX1_DMA_CH_ID) ||                             \
      !defined(SPC5_SPI_DSPI0_TX2_DMA_CH_ID) ||                             \
	  !defined(SPC5_SPI_DSPI0_RX_DMA_CH_ID)))
#error "DMA channels not defined for DSPI0"
#endif
#endif

#if (SPC5_HAS_EDMA == TRUE) && (SPC5_SPI_USE_DSPI1 == TRUE)
#if ((SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_ONLY) &&                         \
     (!defined(SPC5_SPI_DSPI1_RX_DMA_CH_ID))) ||                            \
    ((SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX) &&                       \
     (!defined(SPC5_SPI_DSPI1_TX1_DMA_CH_ID) ||                             \
      !defined(SPC5_SPI_DSPI1_TX2_DMA_CH_ID) ||                             \
	  !defined(SPC5_SPI_DSPI1_RX_DMA_CH_ID)))
#error "DMA channels not defined for DSPI1"
#endif
#endif

#if (SPC5_HAS_EDMA == TRUE) && (SPC5_SPI_USE_DSPI2 == TRUE)
#if ((SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_ONLY) &&                         \
     (!defined(SPC5_SPI_DSPI2_RX_DMA_CH_ID))) ||                            \
    ((SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX) &&                       \
     (!defined(SPC5_SPI_DSPI2_TX1_DMA_CH_ID) ||                             \
      !defined(SPC5_SPI_DSPI2_TX2_DMA_CH_ID) ||                             \
	  !defined(SPC5_SPI_DSPI2_RX_DMA_CH_ID)))
#error "DMA channels not defined for DSPI2"
#endif
#endif

#if (SPC5_HAS_EDMA == TRUE) && (SPC5_SPI_USE_DSPI3 == TRUE)
#if ((SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_ONLY) &&                         \
     (!defined(SPC5_SPI_DSPI3_RX_DMA_CH_ID))) ||                            \
    ((SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX) &&                       \
     (!defined(SPC5_SPI_DSPI3_TX1_DMA_CH_ID) ||                             \
      !defined(SPC5_SPI_DSPI3_TX2_DMA_CH_ID) ||                             \
	  !defined(SPC5_SPI_DSPI3_RX_DMA_CH_ID)))
#error "DMA channels not defined for DSPI3"
#endif
#endif

#if (SPC5_HAS_EDMA == TRUE) && (SPC5_SPI_USE_DSPI4 == TRUE)
#if ((SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_ONLY) &&                         \
     (!defined(SPC5_SPI_DSPI4_RX_DMA_CH_ID))) ||                            \
    ((SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX) &&                       \
     (!defined(SPC5_SPI_DSPI4_TX1_DMA_CH_ID) ||                             \
      !defined(SPC5_SPI_DSPI4_TX2_DMA_CH_ID) ||                             \
	  !defined(SPC5_SPI_DSPI4_RX_DMA_CH_ID)))
#error "DMA channels not defined for DSPI4"
#endif
#endif

#if (SPC5_HAS_EDMA == TRUE) && (SPC5_SPI_USE_DSPI5 == TRUE)
#if ((SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_ONLY) &&                         \
     (!defined(SPC5_SPI_DSPI5_RX_DMA_CH_ID))) ||                            \
    ((SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX) &&                       \
     (!defined(SPC5_SPI_DSPI5_TX1_DMA_CH_ID) ||                             \
      !defined(SPC5_SPI_DSPI5_TX2_DMA_CH_ID) ||                             \
	  !defined(SPC5_SPI_DSPI5_RX_DMA_CH_ID)))
#error "DMA channels not defined for DSPI5"
#endif
#endif

#if (SPC5_HAS_EDMA == TRUE) && (SPC5_SPI_USE_DSPI6 == TRUE)
#if ((SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_ONLY) &&                         \
     (!defined(SPC5_SPI_DSPI6_RX_DMA_CH_ID))) ||                            \
    ((SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX) &&                       \
     (!defined(SPC5_SPI_DSPI6_TX1_DMA_CH_ID) ||                             \
      !defined(SPC5_SPI_DSPI6_TX2_DMA_CH_ID) ||                             \
	  !defined(SPC5_SPI_DSPI6_RX_DMA_CH_ID)))
#error "DMA channels not defined for DSPI6"
#endif
#endif

#if (SPC5_HAS_EDMA == TRUE) && (SPC5_SPI_USE_DSPI7 == TRUE)
#if ((SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_ONLY) &&                         \
     (!defined(SPC5_SPI_DSPI7_RX_DMA_CH_ID))) ||                            \
    ((SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX) &&                       \
     (!defined(SPC5_SPI_DSPI7_TX1_DMA_CH_ID) ||                             \
      !defined(SPC5_SPI_DSPI7_TX2_DMA_CH_ID) ||                             \
	  !defined(SPC5_SPI_DSPI7_RX_DMA_CH_ID)))
#error "DMA channels not defined for DSPI7"
#endif
#endif

#if (SPC5_HAS_EDMA == TRUE) && (SPC5_SPI_USE_DSPI8 == TRUE)
#if ((SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_ONLY) &&                         \
     (!defined(SPC5_SPI_DSPI8_RX_DMA_CH_ID))) ||                            \
    ((SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX) &&                       \
     (!defined(SPC5_SPI_DSPI8_TX1_DMA_CH_ID) ||                             \
      !defined(SPC5_SPI_DSPI8_TX2_DMA_CH_ID) ||                             \
	  !defined(SPC5_SPI_DSPI8_RX_DMA_CH_ID)))
#error "DMA channels not defined for DSPI8"
#endif
#endif

#if (SPC5_HAS_EDMA == TRUE) && (SPC5_SPI_USE_DSPI9 == TRUE)
#if ((SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_ONLY) &&                         \
     (!defined(SPC5_SPI_DSPI9_RX_DMA_CH_ID))) ||                            \
    ((SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX) &&                       \
     (!defined(SPC5_SPI_DSPI9_TX1_DMA_CH_ID) ||                             \
      !defined(SPC5_SPI_DSPI9_TX2_DMA_CH_ID) ||                             \
	  !defined(SPC5_SPI_DSPI9_RX_DMA_CH_ID)))
#error "DMA channels not defined for DSPI9"
#endif
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a structure representing an SPI driver.
 */
typedef struct SPIDriver SPIDriver;

/**
 * @brief SPI mode type.
 */
typedef enum {
  SPI_MASTER = 0,
  SPI_SLAVE  = 1
} spimode_t;

/**
 * @brief   SPI notification callback type.
 *
 * @param[in] spip      pointer to the @p SPIDriver object triggering the
 *                      callback
 */
typedef void (*spicallback_t)(SPIDriver *spip);

/**
 * @brief   Driver configuration structure.
 * @note    Implementations may extend this structure to contain more,
 *          architecture dependent, fields.
 */
typedef struct {
  /**
   * @brief   SPI mode (Master/Slave).
   */
  spimode_t             mode;
  /**
   * @brief   Operation complete callback.
   */
  spicallback_t         end_cb;
  /* End of the mandatory fields.*/
  /**
   * @brief   The chip select line port.
   */
  ioportid_t            ssport;
  /**
   * @brief   The chip select line pad number.
   */
  uint16_t              sspad;
  /**
   * @brief   DSPI CTAR0 value for this session.
   */
  uint32_t              ctar0;
  /**
   * @brief   DSPI PUSHR command for this session.
   * @note    Only CTAR0 can be referenced, the other CTARs are not
   *          initialized. The data part must be left to zero.
   */
  uint32_t              pushr;
} SPIConfig;

/**
 * @brief   Structure representing an SPI driver.
 * @note    Implementations may extend this structure to contain more,
 *          architecture dependent, fields.
 */
struct SPIDriver {
  /**
   * @brief   Current configuration data.
   */
  const SPIConfig           *config;
  /**
   * @brief   Waiting flag.
   */
  volatile uint8_t           ready;
  /**
   * @brief   Pointer to the DSPI registers block.
   */
  struct spc5_dspi          *dspi;
#if (SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE) || defined(__DOXYGEN__)
  /**
   * @brief   EDMA channel used for receive.
   */
  edma_channel_t            rx_channel;
#else /* SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_NONE */
  /**
   * @brief   Memory pointer for RX operations.
   */
  union {
    void                    *rx_ptr;
    uint8_t                 *rx_ptr8;
    uint16_t                *rx_ptr16;
  };
  /**
   * @brief   Remaining frames to be received.
   */
  size_t                    rx_cnt;
#endif /* SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_NONE */
#if (SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_RX_AND_TX) || defined(__DOXYGEN__)
  /**
   * @brief   EDMA channel used for data memory to memory copy.
   */
  edma_channel_t            tx1_channel;
  /**
   * @brief   EDMA channel used for transmit.
   */
  edma_channel_t            tx2_channel;
  /**
   * @brief   Command for the ongoing TX operation.
   */
  uint32_t                  tx_cmd;
  /**
   * @brief   Last frame of a transmission sequence.
   */
  uint32_t                  tx_last;
#else /* SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_RX_AND_TX */
  /**
   * @brief   Memory pointer for TX operations.
   */
  union {
    const void              *tx_ptr;
    const uint8_t           *tx_ptr8;
    const uint16_t          *tx_ptr16;
  };
  /**
   * @brief   Remaining frames to be transmitted.
   */
  size_t                    tx_cnt;
#endif /* SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_RX_AND_TX */
};

/*===========================================================================*/
/* IRQ Handlers                                                              */
/*===========================================================================*/

#if (SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_NONE) || defined(__DOXYGEN__)
#if (SPC5_SPI_USE_DSPI0 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_DSPI0_RFDF_HANDLER);
IRQ_HANDLER(SPC5_DSPI0_EOQF_HANDLER);
IRQ_HANDLER(SPC5_DSPI0_TFFF_HANDLER);
#endif /* SPC5_SPI_USE_DSPI0 */
#if (SPC5_SPI_USE_DSPI1 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_DSPI1_RFDF_HANDLER);
IRQ_HANDLER(SPC5_DSPI1_EOQF_HANDLER);
IRQ_HANDLER(SPC5_DSPI1_TFFF_HANDLER);
#endif /* SPC5_SPI_USE_DSPI1 */
#if (SPC5_SPI_USE_DSPI2 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_DSPI2_RFDF_HANDLER);
IRQ_HANDLER(SPC5_DSPI2_EOQF_HANDLER);
IRQ_HANDLER(SPC5_DSPI2_TFFF_HANDLER);
#endif /* SPC5_SPI_USE_DSPI2 */
#if (SPC5_SPI_USE_DSPI3 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_DSPI3_RFDF_HANDLER);
IRQ_HANDLER(SPC5_DSPI3_EOQF_HANDLER);
IRQ_HANDLER(SPC5_DSPI3_TFFF_HANDLER);
#endif /* SPC5_SPI_USE_DSPI3 */
#if (SPC5_SPI_USE_DSPI4 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_DSPI4_RFDF_HANDLER);
IRQ_HANDLER(SPC5_DSPI4_EOQF_HANDLER);
IRQ_HANDLER(SPC5_DSPI4_TFFF_HANDLER);
#endif /* SPC5_SPI_USE_DSPI4 */
#if (SPC5_SPI_USE_DSPI5 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_DSPI5_RFDF_HANDLER);
IRQ_HANDLER(SPC5_DSPI5_EOQF_HANDLER);
IRQ_HANDLER(SPC5_DSPI5_TFFF_HANDLER);
#endif /* SPC5_SPI_USE_DSPI5 */
#if (SPC5_SPI_USE_DSPI6 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_DSPI6_RFDF_HANDLER);
IRQ_HANDLER(SPC5_DSPI6_EOQF_HANDLER);
IRQ_HANDLER(SPC5_DSPI6_TFFF_HANDLER);
#endif /* SPC5_SPI_USE_DSPI6 */
#if (SPC5_SPI_USE_DSPI7 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_DSPI7_RFDF_HANDLER);
IRQ_HANDLER(SPC5_DSPI7_EOQF_HANDLER);
IRQ_HANDLER(SPC5_DSPI7_TFFF_HANDLER);
#endif /* SPC5_SPI_USE_DSPI7 */
#if (SPC5_SPI_USE_DSPI8 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_DSPI8_RFDF_HANDLER);
IRQ_HANDLER(SPC5_DSPI8_EOQF_HANDLER);
IRQ_HANDLER(SPC5_DSPI8_TFFF_HANDLER);
#endif /* SPC5_SPI_USE_DSPI8 */
#if (SPC5_SPI_USE_DSPI9 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_DSPI9_RFDF_HANDLER);
IRQ_HANDLER(SPC5_DSPI9_EOQF_HANDLER);
IRQ_HANDLER(SPC5_DSPI9_TFFF_HANDLER);
#endif /* SPC5_SPI_USE_DSPI9 */
#endif /* SPC5_SPI_DMA_MODE == SPC5_SPI_DMA_NONE */

#if (SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE) || defined(__DOXYGEN__)
#if (SPC5_SPI_USE_DSPI0 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_DSPI0_TFFF_HANDLER);
#endif /* SPC5_SPI_USE_DSPI0 */
#if (SPC5_SPI_USE_DSPI1 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_DSPI1_TFFF_HANDLER) ;
#endif /* SPC5_SPI_USE_DSPI1 */
#if (SPC5_SPI_USE_DSPI2 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_DSPI2_TFFF_HANDLER);
#endif /* SPC5_SPI_USE_DSPI2 */
#if (SPC5_SPI_USE_DSPI3 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_DSPI3_TFFF_HANDLER);
#endif /* SPC5_SPI_USE_DSPI3 */
#if (SPC5_SPI_USE_DSPI4 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_DSPI4_TFFF_HANDLER);
#endif /* SPC5_SPI_USE_DSPI4 */
#if (SPC5_SPI_USE_DSPI5 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_DSPI5_TFFF_HANDLER);
#endif /* SPC5_SPI_USE_DSPI5 */
#if (SPC5_SPI_USE_DSPI6 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_DSPI6_TFFF_HANDLER);
#endif /* SPC5_SPI_USE_DSPI6 */
#if (SPC5_SPI_USE_DSPI7 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_DSPI7_TFFF_HANDLER);
#endif /* SPC5_SPI_USE_DSPI7 */
#if (SPC5_SPI_USE_DSPI8 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_DSPI8_TFFF_HANDLER);
#endif /* SPC5_SPI_USE_DSPI8 */
#if (SPC5_SPI_USE_DSPI9 == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_DSPI9_TFFF_HANDLER);
#endif /* SPC5_SPI_USE_DSPI9 */
#endif /* SPC5_SPI_DMA_MODE != SPC5_SPI_DMA_NONE */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if (SPC5_SPI_USE_DSPI0 == TRUE) && !defined(__DOXYGEN__)
extern SPIDriver SPID1;
#endif

#if (SPC5_SPI_USE_DSPI1 == TRUE) && !defined(__DOXYGEN__)
extern SPIDriver SPID2;
#endif

#if (SPC5_SPI_USE_DSPI2 == TRUE) && !defined(__DOXYGEN__)
extern SPIDriver SPID3;
#endif

#if (SPC5_SPI_USE_DSPI3 == TRUE) && !defined(__DOXYGEN__)
extern SPIDriver SPID4;
#endif

#if (SPC5_SPI_USE_DSPI4 == TRUE) && !defined(__DOXYGEN__)
extern SPIDriver SPID5;
#endif

#if (SPC5_SPI_USE_DSPI5 == TRUE) && !defined(__DOXYGEN__)
extern SPIDriver SPID6;
#endif

#if (SPC5_SPI_USE_DSPI6 == TRUE) && !defined(__DOXYGEN__)
extern SPIDriver SPID7;
#endif

#if (SPC5_SPI_USE_DSPI7 == TRUE) && !defined(__DOXYGEN__)
extern SPIDriver SPID8;
#endif

#if (SPC5_SPI_USE_DSPI8 == TRUE) && !defined(__DOXYGEN__)
extern SPIDriver SPID9;
#endif

#if (SPC5_SPI_USE_DSPI9 == TRUE) && !defined(__DOXYGEN__)
extern SPIDriver SPID10;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void spi_lld_init(void);
  void spi_lld_start(SPIDriver *spip, const SPIConfig *config);
  void spi_lld_stop(SPIDriver *spip);
  void spi_lld_select(SPIDriver *spip);
  void spi_lld_unselect(SPIDriver *spip);
  void spi_lld_ignore(SPIDriver *spip, size_t n);
  void spi_lld_exchange(SPIDriver *spip, size_t n,
                        const void *txbuf, void *rxbuf);
  void spi_lld_send(SPIDriver *spip, size_t n, const void *txbuf);
  void spi_lld_receive(SPIDriver *spip, size_t n, void *rxbuf);
  uint16_t spi_lld_polled_exchange(SPIDriver *spip, uint16_t frame);
#ifdef __cplusplus
}
#endif

#endif /* LLD_USE_SPI */

#endif /* _SPI_LLD_H_ */

/** @} */
