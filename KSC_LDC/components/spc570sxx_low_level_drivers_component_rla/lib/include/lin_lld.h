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
 * @file    lin_lld.h
 * @brief   SPC5xx LIN low level driver header.
 *
 * @addtogroup LIN
 * @{
 */

#ifndef _SPC5_LIN_LLD_H_
#define _SPC5_LIN_LLD_H_

#include "spc5_lld.h"
#include "lldconf.h"

#if (LLD_USE_LIN == TRUE) || defined(__DOXYGEN__)
#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
#include "spc5_edma.h"
#endif

#include "spc5_linflex.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Lin driver default baud rate
 * @{
 */
#define LIN_DEFAULT_BAUDRATE     20000U
/** @} */

/**
 * @name    Lin driver data length
 * @{
 */
#define LIN_DATA_LEN             8U
/** @} */

/**
 * @name    Lin driver number of filters
 * @{
 */
#define LIN_NUM_FILTERS          16U
/** @} */

/**
 * @name    Offset of Identifier Filter Control registers.
 * @{
 */
#define	SPC5_LINFLEX_IFCR0_OFFSET	0x4CU
/** @} */

/**
 * @name    Bit Mask for the Message ID in Buffer Identifier Register.
 * @{
 */
#define SPC5_BIDR_ID_MASK		0x3FU
/** @} */

/**
 * @name    Bit Mask for Data Field Length in Buffer Identifier Register.
 * @{
 */
#define SPC5_BIDR_DFL_MASK		0x3FU
/** @} */


/**
 * @name    Lin driver Checksum
 * @{
 */
#define LIN_DATA_CHECKSUM             0U /* Enhanced Checksum covering Identifier and Data fields. */
/** @} */

/**
 * @name    Lin Direction Data field
 * @{
 */
#define BIDR_DIR_RECEIVE	            0U /* LINFlexD receives the data and copies them in the BDRs */
#define BIDR_DIR_TRANSMIT               1U /* LINFlexD transmits the data from the BDRs              */
/** @} */

/**
 * @brief  linmsg_t  constant
 * @{
 */
#define LIN_MSG_OK                       (1UL << 1) /**< @brief message OK.                 */
#define LIN_MSG_WAIT                     (1UL << 2) /**< @brief ob or ib is non empty. wait */
#define LIN_MSG_ERROR                    (1UL << 3) /**< @brief ob is non empty. wait       */
#define LIN_MSG_ERROR_LEN                (1UL << 4) /**< @brief buffer len not allowed      */
/** @} */

/**
 * @brief  Default configuration for LIN Interruupt Enable Register
 * @{
 */
#define SPC5_LINIER_DEFAULT			(SPC5_LINIER_HRIE | \
									 SPC5_LINIER_DTIE | \
									 SPC5_LINIER_DRIE | \
									 SPC5_LINIER_WUIE | \
									 SPC5_LINIER_BOIE | \
									 SPC5_LINIER_FEIE | \
									 SPC5_LINIER_HEIE | \
									 SPC5_LINIER_CEIE)
/** @} */

/**
 * @brief  Clear LIN Status Register
 * @{
 */
#define SPC5_LINSR_CLEAR			(SPC5_LINSR_HRF  | \
									 SPC5_LINSR_DTF  | \
									 SPC5_LINSR_DRF  | \
									 SPC5_LINSR_DBEF | \
									 SPC5_LINSR_DBFF | \
									 SPC5_LINSR_WUF  | \
									 SPC5_LINSR_RMB  | \
									 SPC5_LINSR_LINS)
/** @} */

/**
 * @brief  Clear LIN Error Status Register
 * @{
 */
#define SPC5_LINESR_CLEAR			(SPC5_LINESR_NF    | \
									 SPC5_LINESR_BOF   | \
									 SPC5_LINESR_FEF   | \
									 SPC5_LINESR_IDPEF | \
									 SPC5_LINESR_SDEF  | \
									 SPC5_LINESR_SFEF  | \
									 SPC5_LINESR_CEF   | \
									 SPC5_LINESR_BEF   | \
									 SPC5_LINESR_OCF   | \
									 SPC5_LINESR_SZF)
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (SPC5_LINFLEX0_SETTING == SPC5_LINFLEX_MODE_LIN) && (SPC5_HAS_LINFLEX0 == FALSE)
#error "LINFlex-0 not present in the selected device"
#endif

#if (SPC5_LINFLEX1_SETTING == SPC5_LINFLEX_MODE_LIN) && (SPC5_HAS_LINFLEX1 == FALSE)
#error "LINFlex-1 not present in the selected device"
#endif

#if (SPC5_LINFLEX2_SETTING == SPC5_LINFLEX_MODE_LIN) && (SPC5_HAS_LINFLEX2 == FALSE)
#error "LINFlex-2 not present in the selected device"
#endif

#if (SPC5_LINFLEX3_SETTING == SPC5_LINFLEX_MODE_LIN) && (SPC5_HAS_LINFLEX3 == FALSE)
#error "LINFlex-3 not present in the selected device"
#endif

#if (SPC5_LINFLEX4_SETTING == SPC5_LINFLEX_MODE_LIN) && (SPC5_HAS_LINFLEX4 == FALSE)
#error "LINFlex-4 not present in the selected device"
#endif

#if (SPC5_LINFLEX5_SETTING == SPC5_LINFLEX_MODE_LIN) && (SPC5_HAS_LINFLEX5 == FALSE)
#error "LINFlex-5 not present in the selected device"
#endif

#if (SPC5_LINFLEX6_SETTING == SPC5_LINFLEX_MODE_LIN) && (SPC5_HAS_LINFLEX6 == FALSE)
#error "LINFlex-6 not present in the selected device"
#endif

#if (SPC5_LINFLEX7_SETTING == SPC5_LINFLEX_MODE_LIN) && (SPC5_HAS_LINFLEX7 == FALSE)
#error "LINFlex-7 not present in the selected device"
#endif

#if (SPC5_LINFLEX8_SETTING == SPC5_LINFLEX_MODE_LIN) && (SPC5_HAS_LINFLEX8 == FALSE)
#error "LINFlex-8 not present in the selected device"
#endif

#if (SPC5_LINFLEX9_SETTING == SPC5_LINFLEX_MODE_LIN) && (SPC5_HAS_LINFLEX9 == FALSE)
#error "LINFlex-9 not present in the selected device"
#endif

#if (SPC5_LINFLEX10_SETTING == SPC5_LINFLEX_MODE_LIN) && (SPC5_HAS_LINFLEX10 == FALSE)
#error "LINFlex-10 not present in the selected device"
#endif

#if (SPC5_LINFLEX11_SETTING == SPC5_LINFLEX_MODE_LIN) && (SPC5_HAS_LINFLEX11 == FALSE)
#error "LINFlex-11 not present in the selected device"
#endif

#if (SPC5_LINFLEX12_SETTING == SPC5_LINFLEX_MODE_LIN) && (SPC5_HAS_LINFLEX12 == FALSE)
#error "LINFlex-12 not present in the selected device"
#endif

#if (SPC5_LINFLEX13_SETTING == SPC5_LINFLEX_MODE_LIN) && (SPC5_HAS_LINFLEX13 == FALSE)
#error "LINFlex-13 not present in the selected device"
#endif

#if (SPC5_LINFLEX14_SETTING == SPC5_LINFLEX_MODE_LIN) && (SPC5_HAS_LINFLEX14 == FALSE)
#error "LINFlex-14 not present in the selected device"
#endif

#if (SPC5_LINFLEX15_SETTING == SPC5_LINFLEX_MODE_LIN) && (SPC5_HAS_LINFLEX15 == FALSE)
#error "LINFlex-15 not present in the selected device"
#endif

#if (SPC5_LINFLEX16_SETTING == SPC5_LINFLEX_MODE_LIN) && (SPC5_HAS_LINFLEX16 == FALSE)
#error "LINFlex-16 not present in the selected device"
#endif

#if (SPC5_LINFLEX17_SETTING == SPC5_LINFLEX_MODE_LIN) && (SPC5_HAS_LINFLEX17 == FALSE)
#error "LINFlex-17 not present in the selected device"
#endif

#if (SPC5_LINFLEX18_SETTING == SPC5_LINFLEX_MODE_LIN) && (SPC5_HAS_LINFLEX18 == FALSE)
#error "LINFlex-18 not present in the selected device"
#endif

#if (SPC5_LINFLEX19_SETTING == SPC5_LINFLEX_MODE_LIN) && (SPC5_HAS_LINFLEX19 == FALSE)
#error "LINFlex-19 not present in the selected device"
#endif

#if (SPC5_LINFLEX20_SETTING == SPC5_LINFLEX_MODE_LIN) && (SPC5_HAS_LINFLEX20 == FALSE)
#error "LINFlex-20 not present in the selected device"
#endif

#if (SPC5_LINFLEX21_SETTING == SPC5_LINFLEX_MODE_LIN) && (SPC5_HAS_LINFLEX21 == FALSE)
#error "LINFlex-21 not present in the selected device"
#endif

#if (SPC5_LINFLEX22_SETTING == SPC5_LINFLEX_MODE_LIN) && (SPC5_HAS_LINFLEX22 == FALSE)
#error "LINFlex-22 not present in the selected device"
#endif

#if (SPC5_LINFLEX23_SETTING == SPC5_LINFLEX_MODE_LIN) && (SPC5_HAS_LINFLEX23 == FALSE)
#error "LINFlex-23 not present in the selected device"
#endif

#if (SPC5_LINFLEX0_SETTING  != SPC5_LINFLEX_MODE_LIN) && (SPC5_LINFLEX1_SETTING  != SPC5_LINFLEX_MODE_LIN) && \
    (SPC5_LINFLEX2_SETTING  != SPC5_LINFLEX_MODE_LIN) && (SPC5_LINFLEX3_SETTING  != SPC5_LINFLEX_MODE_LIN) && \
    (SPC5_LINFLEX4_SETTING  != SPC5_LINFLEX_MODE_LIN) && (SPC5_LINFLEX5_SETTING  != SPC5_LINFLEX_MODE_LIN) && \
    (SPC5_LINFLEX6_SETTING  != SPC5_LINFLEX_MODE_LIN) && (SPC5_LINFLEX7_SETTING  != SPC5_LINFLEX_MODE_LIN) && \
    (SPC5_LINFLEX8_SETTING  != SPC5_LINFLEX_MODE_LIN) && (SPC5_LINFLEX9_SETTING  != SPC5_LINFLEX_MODE_LIN) && \
    (SPC5_LINFLEX10_SETTING != SPC5_LINFLEX_MODE_LIN) && (SPC5_LINFLEX11_SETTING != SPC5_LINFLEX_MODE_LIN) && \
    (SPC5_LINFLEX12_SETTING != SPC5_LINFLEX_MODE_LIN) && (SPC5_LINFLEX13_SETTING != SPC5_LINFLEX_MODE_LIN) && \
    (SPC5_LINFLEX14_SETTING != SPC5_LINFLEX_MODE_LIN) && (SPC5_LINFLEX15_SETTING != SPC5_LINFLEX_MODE_LIN) && \
    (SPC5_LINFLEX16_SETTING != SPC5_LINFLEX_MODE_LIN) && (SPC5_LINFLEX17_SETTING != SPC5_LINFLEX_MODE_LIN) && \
    (SPC5_LINFLEX18_SETTING != SPC5_LINFLEX_MODE_LIN) && (SPC5_LINFLEX19_SETTING != SPC5_LINFLEX_MODE_LIN) && \
    (SPC5_LINFLEX20_SETTING != SPC5_LINFLEX_MODE_LIN) && (SPC5_LINFLEX21_SETTING != SPC5_LINFLEX_MODE_LIN) && \
    (SPC5_LINFLEX22_SETTING != SPC5_LINFLEX_MODE_LIN) && (SPC5_LINFLEX23_SETTING != SPC5_LINFLEX_MODE_LIN)
#error "LIN driver activated but no LINFlex peripheral assigned"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief    Lin driver allowable modes
 */
typedef enum
{
	LIN_MODE_SLAVE,                  /**< @brief   LINFlexD is in slave mode.        */
	LIN_MODE_MASTER,                 /**< @brief   LINFlexD is in master mode.       */
	LIN_MODE_MASTER_SLAVE            /**< @brief   LINFlexD is in master/slave mode. */
} lin_mode_t;

/**
 * @brief    Lin Message Direction Data Type
 *
 */
typedef enum
{
	LIN_DIRECTION_RECEIVE,           /**< @brief   LINFlexD receives the data and copies them in the BDRs. */
	LIN_DIRECTION_TRANSMIT           /**< @brief   LINFlexD transmits the data from the BDRs.              */
} lin_message_direction_t;

/**
 * @brief    Lin Message Checksum Data Type
 *
 */
typedef enum
{
	LIN_ENHANCED_CHECKSUM,           /**< @brief   Enhanced Checksum covering Identifier and Data fields. */
	LIN_CLASSIC_CHECKSUM             /**< @brief   Classic Checksum covering Data field only.             */
} lin_checksum_t;

/**
 * @brief    Lin Message Checksum Data Type
 *
 */
typedef enum
{
	LIN_LIST_MODE,                   /**< @brief   FIlter is configured in list mode. */
	LIN_MASK_MODE                    /**< @brief   Filter is configured in mask mode. */
} lin_filter_mode_t;

/**
 * @brief   Type of a structure representing a LIN driver.
 */
typedef struct LinDriver LinDriver;

/**
 * @brief   LIN notification callback type.
 *
 * @param[in] ldp       pointer to the @p LinDriver object triggering the
 *                      callback
 */
typedef uint32_t (*linclbk_t)(LinDriver *ldp, uint8_t idMessage, uint8_t *buffer, uint16_t len);


/**
 * @brief   LIN DMA error callback type.
 *
 * @param[in] ldp       pointer to the @p LinDriver object triggering the
 *                      callback
 */
typedef void (*ldma_errorcallback_t)(LinDriver *ldp);

/**
 * @brief   Generic Lin Filter configuration structure.
 * @details This strcuture represents a filter configuration.
 *          It is included in Lin slave configuration structure.
 */
typedef struct {
  /**
   * @brief   Value. Filter value, it is in the range [0, 63]
   */
  uint8_t                         value;
  /**
   * @brief   Message data size. It is in the range [1, 8].
   */
  uint8_t                         datasize;
  /**
   * @brief   Message direction (Transmit, Receive).
   */
  lin_message_direction_t         direction;
  /**
   * @brief   Message Checksum Data Type.
   */
  lin_checksum_t                  ccs;
  /**
   * @brief   Callback function associated to the TX completion.
   */
  lin_filter_mode_t               mode;
  /**
   * @brief   Mask Value. Mask Filter value, it is in the range [0, 63]
   */
  uint8_t                         mask_value;
} LinFilter;


/**
 * @brief   Generic Lin Driver configuration structure.
 * @details An instance of this structure must be passed to @p lin_start()
 *          in order to configure and start a lin driver operations.
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
   * @brief   Lin Driver Mode flags.
   */
  lin_mode_t                      mode;
  /**
   * @brief   API Mode flags.
   */
  uint8_t                         api_mode;
  /**
   * @brief   Callback function associated to the TX completion.
   */
  linclbk_t                       tx_end_cb;
  /**
   * @brief   Callback function associated to the RX completion.
   */
  linclbk_t                       rx_end_cb;
  /**
   * @brief   Filters list.
   */
  LinFilter                      **filters;
  /**
   * @brief   Linflex Loopback Mode Flag.
   */
  uint8_t                         loopBack;
  /**
   * @brief   Flag set if the DMA is enabled.
   */
  uint8_t                         dma_enable;
  /**
   * @brief   DMA error callback.
   */
  ldma_errorcallback_t            dma_err_cb;
} LinConfig;


/**
 * @brief   Structure representing an LinDriver driver.
 */
struct LinDriver {
  /**
   * @brief   Current configuration data.
   */
  const LinConfig                 *config;
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
   * @brief   Id of the received message.
   */
  uint8_t                         rx_id_msg;
  /**
   * @brief   Id of the transmitted message.
   */
  uint8_t                         tx_id_msg;
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

#if (SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON)
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
#endif /* SPC5_LIN_DMA_MODE == SPC5_LIN_DMA_ON */
};

/**
 * @brief   @p type of return message for lin driver
 */
typedef uint32_t linmsg_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if (SPC5_LINFLEX0_SETTING == SPC5_LINFLEX_MODE_LIN) && !defined(__DOXYGEN__)
extern LinDriver LD1;
#endif
#if (SPC5_LINFLEX1_SETTING == SPC5_LINFLEX_MODE_LIN) && !defined(__DOXYGEN__)
extern LinDriver LD2;
#endif
#if (SPC5_LINFLEX2_SETTING == SPC5_LINFLEX_MODE_LIN) && !defined(__DOXYGEN__)
extern LinDriver LD3;
#endif
#if (SPC5_LINFLEX3_SETTING == SPC5_LINFLEX_MODE_LIN) && !defined(__DOXYGEN__)
extern LinDriver LD4;
#endif
#if (SPC5_LINFLEX4_SETTING == SPC5_LINFLEX_MODE_LIN) && !defined(__DOXYGEN__)
extern LinDriver LD5;
#endif
#if (SPC5_LINFLEX5_SETTING == SPC5_LINFLEX_MODE_LIN) && !defined(__DOXYGEN__)
extern LinDriver LD6;
#endif
#if (SPC5_LINFLEX6_SETTING == SPC5_LINFLEX_MODE_LIN) && !defined(__DOXYGEN__)
extern LinDriver LD7;
#endif
#if (SPC5_LINFLEX7_SETTING == SPC5_LINFLEX_MODE_LIN) && !defined(__DOXYGEN__)
extern LinDriver LD8;
#endif
#if (SPC5_LINFLEX8_SETTING == SPC5_LINFLEX_MODE_LIN) && !defined(__DOXYGEN__)
extern LinDriver LD9;
#endif
#if (SPC5_LINFLEX9_SETTING == SPC5_LINFLEX_MODE_LIN) && !defined(__DOXYGEN__)
extern LinDriver LD10;
#endif
#if (SPC5_LINFLEX10_SETTING == SPC5_LINFLEX_MODE_LIN) && !defined(__DOXYGEN__)
extern LinDriver LD11;
#endif
#if (SPC5_LINFLEX11_SETTING == SPC5_LINFLEX_MODE_LIN) && !defined(__DOXYGEN__)
extern LinDriver LD12;
#endif
#if (SPC5_LINFLEX12_SETTING == SPC5_LINFLEX_MODE_LIN) && !defined(__DOXYGEN__)
extern LinDriver LD13;
#endif
#if (SPC5_LINFLEX13_SETTING == SPC5_LINFLEX_MODE_LIN) && !defined(__DOXYGEN__)
extern LinDriver LD14;
#endif
#if (SPC5_LINFLEX14_SETTING == SPC5_LINFLEX_MODE_LIN) && !defined(__DOXYGEN__)
extern LinDriver LD15;
#endif
#if (SPC5_LINFLEX15_SETTING == SPC5_LINFLEX_MODE_LIN) && !defined(__DOXYGEN__)
extern LinDriver LD16;
#endif
#if (SPC5_LINFLEX16_SETTING == SPC5_LINFLEX_MODE_LIN) && !defined(__DOXYGEN__)
extern LinDriver LD17;
#endif
#if (SPC5_LINFLEX17_SETTING == SPC5_LINFLEX_MODE_LIN) && !defined(__DOXYGEN__)
extern LinDriver LD18;
#endif
#if (SPC5_LINFLEX18_SETTING == SPC5_LINFLEX_MODE_LIN) && !defined(__DOXYGEN__)
extern LinDriver LD19;
#endif
#if (SPC5_LINFLEX19_SETTING == SPC5_LINFLEX_MODE_LIN) && !defined(__DOXYGEN__)
extern LinDriver LD20;
#endif
#if (SPC5_LINFLEX20_SETTING == SPC5_LINFLEX_MODE_LIN) && !defined(__DOXYGEN__)
extern LinDriver LD21;
#endif
#if (SPC5_LINFLEX21_SETTING == SPC5_LINFLEX_MODE_LIN) && !defined(__DOXYGEN__)
extern LinDriver LD22;
#endif
#if (SPC5_LINFLEX22_SETTING == SPC5_LINFLEX_MODE_LIN) && !defined(__DOXYGEN__)
extern LinDriver LD23;
#endif
#if (SPC5_LINFLEX23_SETTING == SPC5_LINFLEX_MODE_LIN) && !defined(__DOXYGEN__)
extern LinDriver LD24;
#endif

/*===========================================================================*/
/* Include configuration.                                                    */
/*===========================================================================*/
#include <lin_lld_cfg.h>

#ifdef __cplusplus
extern "C" {
#endif

  void lin_lld_init(void);
  void lin_lld_start(LinDriver *ldp, const LinConfig *config);
  void lin_lld_stop(LinDriver *ldp);

  void lin_lld_transmit(LinDriver *ldp, uint8_t idMessage, uint8_t *buffer, uint16_t msg_len);
  uint16_t lin_lld_receive(LinDriver *ldp, uint8_t idMessage, uint8_t *buffer, uint16_t buf_len);
#ifdef __cplusplus
}
#endif

#endif /* LLD_USE_LIN */

#endif /* _SPC5_LIN_LLD_H_ */

/** @} */
