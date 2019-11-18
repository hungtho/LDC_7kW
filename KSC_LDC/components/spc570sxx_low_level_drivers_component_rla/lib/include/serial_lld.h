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
 * @file    serial_lld.h
 * @brief   SPC5xx SERIAL low level driver header.
 *
 * @addtogroup SERIAL
 * @{
 */

#ifndef _SERIAL_LLD_H_
#define _SERIAL_LLD_H_

#include "spc5_lld.h"
#include "lldconf.h"

#if (LLD_USE_SERIAL == TRUE) || defined(__DOXYGEN__)
#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
#include "spc5_edma.h"
#endif
#include "spc5_linflex.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Serial driver default baud rate
 * @{
 */
#define SERIAL_DEFAULT_BAUDRATE             38400U
/** @} */

/**
 * @name    Serial driver allowable modes
 * @{
 */
#define SD_MODE_8BITS_PARITY_NONE           (SPC5_UARTCR_WL)
#define SD_MODE_8BITS_PARITY_EVEN           (SPC5_UARTCR_WL |               \
                                             SPC5_UARTCR_PCE)
#define SD_MODE_8BITS_PARITY_ODD            (SPC5_UARTCR_WL |               \
                                             SPC5_UARTCR_PCE |              \
                                             SPC5_UARTCR_OP)
/** @} */

/**
 * @brief  serialmsg_t  constant
 * @{
 */
#define SERIAL_MSG_OK                       (1UL << 1) /**< @brief message OK.                 */
#define SERIAL_MSG_WAIT                     (1UL << 2) /**< @brief ob or ib is non empty. wait */
#define SERIAL_MSG_ERROR                    (1UL << 3) /**< @brief ob is non empty. wait       */
#define SERIAL_MSG_ERROR_LEN                (1UL << 4) /**< @brief buffer len not allowed      */
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */

/**
 * @brief   LINFlex Serial driver buffered IO mode buffer size.
 */
#if !defined(SPC5_SERIAL_BUFFERED_IO_SIZE) || defined(__DOXYGEN__)
#define SPC5_SERIAL_BUFFERED_IO_SIZE        16U
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (SPC5_LINFLEX0_SETTING == SPC5_LINFLEX_MODE_SERIAL) && (SPC5_HAS_LINFLEX0 == FALSE)
#error "LINFlex-0 not present in the selected device"
#endif

#if (SPC5_LINFLEX1_SETTING == SPC5_LINFLEX_MODE_SERIAL) && (SPC5_HAS_LINFLEX1 == FALSE)
#error "LINFlex-1 not present in the selected device"
#endif

#if (SPC5_LINFLEX2_SETTING == SPC5_LINFLEX_MODE_SERIAL) && (SPC5_HAS_LINFLEX2 == FALSE)
#error "LINFlex-2 not present in the selected device"
#endif

#if (SPC5_LINFLEX3_SETTING == SPC5_LINFLEX_MODE_SERIAL) && (SPC5_HAS_LINFLEX3 == FALSE)
#error "LINFlex-3 not present in the selected device"
#endif

#if (SPC5_LINFLEX4_SETTING == SPC5_LINFLEX_MODE_SERIAL) && (SPC5_HAS_LINFLEX4 == FALSE)
#error "LINFlex-4 not present in the selected device"
#endif

#if (SPC5_LINFLEX5_SETTING == SPC5_LINFLEX_MODE_SERIAL) && (SPC5_HAS_LINFLEX5 == FALSE)
#error "LINFlex-5 not present in the selected device"
#endif

#if (SPC5_LINFLEX6_SETTING == SPC5_LINFLEX_MODE_SERIAL) && (SPC5_HAS_LINFLEX6 == FALSE)
#error "LINFlex-6 not present in the selected device"
#endif

#if (SPC5_LINFLEX7_SETTING == SPC5_LINFLEX_MODE_SERIAL) && (SPC5_HAS_LINFLEX7 == FALSE)
#error "LINFlex-7 not present in the selected device"
#endif

#if (SPC5_LINFLEX8_SETTING == SPC5_LINFLEX_MODE_SERIAL) && (SPC5_HAS_LINFLEX8 == FALSE)
#error "LINFlex-8 not present in the selected device"
#endif

#if (SPC5_LINFLEX9_SETTING == SPC5_LINFLEX_MODE_SERIAL) && (SPC5_HAS_LINFLEX9 == FALSE)
#error "LINFlex-9 not present in the selected device"
#endif

#if (SPC5_LINFLEX10_SETTING == SPC5_LINFLEX_MODE_SERIAL) && (SPC5_HAS_LINFLEX10 == FALSE)
#error "LINFlex-10 not present in the selected device"
#endif

#if (SPC5_LINFLEX11_SETTING == SPC5_LINFLEX_MODE_SERIAL) && (SPC5_HAS_LINFLEX11 == FALSE)
#error "LINFlex-11 not present in the selected device"
#endif

#if (SPC5_LINFLEX12_SETTING == SPC5_LINFLEX_MODE_SERIAL) && (SPC5_HAS_LINFLEX12 == FALSE)
#error "LINFlex-12 not present in the selected device"
#endif

#if (SPC5_LINFLEX13_SETTING == SPC5_LINFLEX_MODE_SERIAL) && (SPC5_HAS_LINFLEX13 == FALSE)
#error "LINFlex-13 not present in the selected device"
#endif

#if (SPC5_LINFLEX14_SETTING == SPC5_LINFLEX_MODE_SERIAL) && (SPC5_HAS_LINFLEX14 == FALSE)
#error "LINFlex-14 not present in the selected device"
#endif

#if (SPC5_LINFLEX15_SETTING == SPC5_LINFLEX_MODE_SERIAL) && (SPC5_HAS_LINFLEX15 == FALSE)
#error "LINFlex-15 not present in the selected device"
#endif

#if (SPC5_LINFLEX16_SETTING == SPC5_LINFLEX_MODE_SERIAL) && (SPC5_HAS_LINFLEX16 == FALSE)
#error "LINFlex-16 not present in the selected device"
#endif

#if (SPC5_LINFLEX17_SETTING == SPC5_LINFLEX_MODE_SERIAL) && (SPC5_HAS_LINFLEX17 == FALSE)
#error "LINFlex-17 not present in the selected device"
#endif

#if (SPC5_LINFLEX18_SETTING == SPC5_LINFLEX_MODE_SERIAL) && (SPC5_HAS_LINFLEX18 == FALSE)
#error "LINFlex-18 not present in the selected device"
#endif

#if (SPC5_LINFLEX19_SETTING == SPC5_LINFLEX_MODE_SERIAL) && (SPC5_HAS_LINFLEX19 == FALSE)
#error "LINFlex-19 not present in the selected device"
#endif

#if (SPC5_LINFLEX20_SETTING == SPC5_LINFLEX_MODE_SERIAL) && (SPC5_HAS_LINFLEX20 == FALSE)
#error "LINFlex-20 not present in the selected device"
#endif

#if (SPC5_LINFLEX21_SETTING == SPC5_LINFLEX_MODE_SERIAL) && (SPC5_HAS_LINFLEX21 == FALSE)
#error "LINFlex-21 not present in the selected device"
#endif

#if (SPC5_LINFLEX22_SETTING == SPC5_LINFLEX_MODE_SERIAL) && (SPC5_HAS_LINFLEX22 == FALSE)
#error "LINFlex-22 not present in the selected device"
#endif

#if (SPC5_LINFLEX23_SETTING == SPC5_LINFLEX_MODE_SERIAL) && (SPC5_HAS_LINFLEX23 == FALSE)
#error "LINFlex-23 not present in the selected device"
#endif

#if (SPC5_LINFLEX0_SETTING  != SPC5_LINFLEX_MODE_SERIAL) && (SPC5_LINFLEX1_SETTING  != SPC5_LINFLEX_MODE_SERIAL) && \
    (SPC5_LINFLEX2_SETTING  != SPC5_LINFLEX_MODE_SERIAL) && (SPC5_LINFLEX3_SETTING  != SPC5_LINFLEX_MODE_SERIAL) && \
    (SPC5_LINFLEX4_SETTING  != SPC5_LINFLEX_MODE_SERIAL) && (SPC5_LINFLEX5_SETTING  != SPC5_LINFLEX_MODE_SERIAL) && \
    (SPC5_LINFLEX6_SETTING  != SPC5_LINFLEX_MODE_SERIAL) && (SPC5_LINFLEX7_SETTING  != SPC5_LINFLEX_MODE_SERIAL) && \
    (SPC5_LINFLEX8_SETTING  != SPC5_LINFLEX_MODE_SERIAL) && (SPC5_LINFLEX9_SETTING  != SPC5_LINFLEX_MODE_SERIAL) && \
    (SPC5_LINFLEX10_SETTING != SPC5_LINFLEX_MODE_SERIAL) && (SPC5_LINFLEX11_SETTING != SPC5_LINFLEX_MODE_SERIAL) && \
    (SPC5_LINFLEX12_SETTING != SPC5_LINFLEX_MODE_SERIAL) && (SPC5_LINFLEX13_SETTING != SPC5_LINFLEX_MODE_SERIAL) && \
    (SPC5_LINFLEX14_SETTING != SPC5_LINFLEX_MODE_SERIAL) && (SPC5_LINFLEX15_SETTING != SPC5_LINFLEX_MODE_SERIAL) && \
    (SPC5_LINFLEX16_SETTING != SPC5_LINFLEX_MODE_SERIAL) && (SPC5_LINFLEX17_SETTING != SPC5_LINFLEX_MODE_SERIAL) && \
    (SPC5_LINFLEX18_SETTING != SPC5_LINFLEX_MODE_SERIAL) && (SPC5_LINFLEX19_SETTING != SPC5_LINFLEX_MODE_SERIAL) && \
    (SPC5_LINFLEX20_SETTING != SPC5_LINFLEX_MODE_SERIAL) && (SPC5_LINFLEX21_SETTING != SPC5_LINFLEX_MODE_SERIAL) && \
    (SPC5_LINFLEX22_SETTING != SPC5_LINFLEX_MODE_SERIAL) && (SPC5_LINFLEX23_SETTING != SPC5_LINFLEX_MODE_SERIAL)
#error "SERIAL driver activated but no LINFlex peripheral assigned"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/


/**
 * @brief   Type of a structure representing a SERIAL driver.
 */
typedef struct SerialDriver SerialDriver;

/**
 * @brief   SERIAL notification callback type.
 *
 * @param[in] sdp       pointer to the @p SerialDriver object triggering the
 *                      callback
 */
typedef void (*linflexcallback_t)(SerialDriver *sdp);

/**
 * @brief   SERIAL DMA error callback type.
 *
 * @param[in] sdp       pointer to the @p SerialDriver object triggering the
 *                      callback
 */
typedef void (*sdma_errorcallback_t)(SerialDriver *sdp);

/**
 * @brief   Generic Serial Driver configuration structure.
 * @details An instance of this structure must be passed to @p sdStart()
 *          in order to configure and start a serial driver operations.
 * @note    This structure content is architecture dependent, each driver
 *          implementation defines its own version and the custom static
 *          initializers.
 */
typedef struct {
  /**
   * @brief   Bit rate.
   */
  uint32_t                        speed;
  /**
   * @brief   Mode flags.
   */
  uint8_t                         mode;
  /**
   * @brief   API Mode flags.
   */
  uint8_t                         api_mode;
  /**
   * @brief   Callback function associated to the TX completion.
   */
  linflexcallback_t               tx_end_cb;
  /**
   * @brief   Callback function associated to the RX completion.
   */
  linflexcallback_t               rx_end_cb;
  /**
   * @brief   Flag set if the DMA is enabled.
   */
  uint8_t                         dma_enable;
  /**
   * @brief   DMA error callback.
   */
  sdma_errorcallback_t             dma_err_cb;
} SerialConfig;

/**
 * @brief   Structure representing an SerialDriver driver.
 */
struct SerialDriver {
  /**
   * @brief   Current configuration data.
   */
  const SerialConfig              *config;
  /**
   * @brief   Input Buffer.
   */
  uint8_t                         *rx_buf;
  /**
   * @brief   Output Buffer.
   */
  uint8_t                         *tx_buf;
  /**
   * @brief   Number of bytes to receive.
   */
  uint16_t                        rx_len;
  /**
   * @brief   Number of bytes to transmit.
   */
  uint16_t                        tx_len;
  /**
   * @brief   Receive in progress.
   */
  volatile uint16_t               rx_busy;
  /**
   * @brief   Transmit in progress.
   */
  volatile uint16_t               tx_busy;
  /**
   * @brief   Pointer to the volatile LINFlex registers block low (from LINCR1 to IFMR).
   */
  volatile struct spc5_linflexl   *linflexlp;
  /**
   * @brief   Pointer to the volatile LINFlex registers block high (from GCR to DMARXE).
   */
  volatile struct spc5_linflexh   *linflexhp;
  /**
   * @brief   Clock value for this unit.
   */
  uint32_t                        clock;

#if (SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON)
  /**
   * @brief   Flag set if the LinFlex supports the dma.
   */
  uint8_t                         dma_supported;
  /**
   * @brief   EDMA channel used for transmit.
   */
  edma_channel_t                  tx_channel;
  /**
   * @brief   EDMA channel used for receive.
   */
  edma_channel_t                  rx_channel;
#endif /* SPC5_SERIAL_DMA_MODE == SPC5_SERIAL_DMA_ON) */

  /**
   * @brief   RX Buffer.
   */
  uint8_t                         rx_buffered_io[SPC5_SERIAL_BUFFERED_IO_SIZE + 1U];
  /**
   * @brief   Pointer to the first available byte in the RX buffer.
   */
  uint8_t                         *rx_write_ptr;
  /**
   * @brief   Pointer to the first valid byte within the RX buffer.
   */
  uint8_t                         *rx_read_ptr;
  /**
   * @brief   Pointer to the end of the RX buffer.
   */
  uint8_t                         *rx_end_ptr;
};

/**
 * @brief   @p type of return message for serial driver
 */
typedef uint32_t serialmsg_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if (SPC5_LINFLEX0_SETTING == SPC5_LINFLEX_MODE_SERIAL) && !defined(__DOXYGEN__)
extern SerialDriver SD1;
#endif
#if (SPC5_LINFLEX1_SETTING == SPC5_LINFLEX_MODE_SERIAL) && !defined(__DOXYGEN__)
extern SerialDriver SD2;
#endif
#if (SPC5_LINFLEX2_SETTING == SPC5_LINFLEX_MODE_SERIAL) && !defined(__DOXYGEN__)
extern SerialDriver SD3;
#endif
#if (SPC5_LINFLEX3_SETTING == SPC5_LINFLEX_MODE_SERIAL) && !defined(__DOXYGEN__)
extern SerialDriver SD4;
#endif
#if (SPC5_LINFLEX4_SETTING == SPC5_LINFLEX_MODE_SERIAL) && !defined(__DOXYGEN__)
extern SerialDriver SD5;
#endif
#if (SPC5_LINFLEX5_SETTING == SPC5_LINFLEX_MODE_SERIAL) && !defined(__DOXYGEN__)
extern SerialDriver SD6;
#endif
#if (SPC5_LINFLEX6_SETTING == SPC5_LINFLEX_MODE_SERIAL) && !defined(__DOXYGEN__)
extern SerialDriver SD7;
#endif
#if (SPC5_LINFLEX7_SETTING == SPC5_LINFLEX_MODE_SERIAL) && !defined(__DOXYGEN__)
extern SerialDriver SD8;
#endif
#if (SPC5_LINFLEX8_SETTING == SPC5_LINFLEX_MODE_SERIAL) && !defined(__DOXYGEN__)
extern SerialDriver SD9;
#endif
#if (SPC5_LINFLEX9_SETTING == SPC5_LINFLEX_MODE_SERIAL) && !defined(__DOXYGEN__)
extern SerialDriver SD10;
#endif
#if (SPC5_LINFLEX10_SETTING == SPC5_LINFLEX_MODE_SERIAL) && !defined(__DOXYGEN__)
extern SerialDriver SD11;
#endif
#if (SPC5_LINFLEX11_SETTING == SPC5_LINFLEX_MODE_SERIAL) && !defined(__DOXYGEN__)
extern SerialDriver SD12;
#endif
#if (SPC5_LINFLEX12_SETTING == SPC5_LINFLEX_MODE_SERIAL) && !defined(__DOXYGEN__)
extern SerialDriver SD13;
#endif
#if (SPC5_LINFLEX13_SETTING == SPC5_LINFLEX_MODE_SERIAL) && !defined(__DOXYGEN__)
extern SerialDriver SD14;
#endif
#if (SPC5_LINFLEX14_SETTING == SPC5_LINFLEX_MODE_SERIAL) && !defined(__DOXYGEN__)
extern SerialDriver SD15;
#endif
#if (SPC5_LINFLEX15_SETTING == SPC5_LINFLEX_MODE_SERIAL) && !defined(__DOXYGEN__)
extern SerialDriver SD16;
#endif
#if (SPC5_LINFLEX16_SETTING == SPC5_LINFLEX_MODE_SERIAL) && !defined(__DOXYGEN__)
extern SerialDriver SD17;
#endif
#if (SPC5_LINFLEX17_SETTING == SPC5_LINFLEX_MODE_SERIAL) && !defined(__DOXYGEN__)
extern SerialDriver SD18;
#endif
#if (SPC5_LINFLEX18_SETTING == SPC5_LINFLEX_MODE_SERIAL) && !defined(__DOXYGEN__)
extern SerialDriver SD19;
#endif
#if (SPC5_LINFLEX19_SETTING == SPC5_LINFLEX_MODE_SERIAL) && !defined(__DOXYGEN__)
extern SerialDriver SD20;
#endif
#if (SPC5_LINFLEX20_SETTING == SPC5_LINFLEX_MODE_SERIAL) && !defined(__DOXYGEN__)
extern SerialDriver SD21;
#endif
#if (SPC5_LINFLEX21_SETTING == SPC5_LINFLEX_MODE_SERIAL) && !defined(__DOXYGEN__)
extern SerialDriver SD22;
#endif
#if (SPC5_LINFLEX22_SETTING == SPC5_LINFLEX_MODE_SERIAL) && !defined(__DOXYGEN__)
extern SerialDriver SD23;
#endif
#if (SPC5_LINFLEX23_SETTING == SPC5_LINFLEX_MODE_SERIAL) && !defined(__DOXYGEN__)
extern SerialDriver SD24;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void sd_lld_init(void);
  void sd_lld_start(SerialDriver *sdp, const SerialConfig *config);
  void sd_lld_stop(SerialDriver *sdp);
  serialmsg_t sd_lld_write(SerialDriver *sdp, uint8_t *buffer, uint16_t len);
  void sd_lld_write_dma(SerialDriver *sdp, size_t n, uint8_t *txbuf);
  uint16_t sd_lld_read(SerialDriver* sdp, uint8_t* buffer, uint16_t len);
#ifdef __cplusplus
}
#endif

#endif /* LLD_USE_SERIAL */

#endif /* _SERIAL_LLD_H_ */

/** @} */
