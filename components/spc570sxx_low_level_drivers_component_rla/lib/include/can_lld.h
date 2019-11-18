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
* EVALUATION ONLY – NOT FOR USE IN PRODUCTION
*****************************************************************************/
/**
 * @file    can_lld.h
 * @brief   SPC5xx CAN low level driver header.
 *
 * @addtogroup CAN
 * @{
 */

#ifndef _CAN_LLD_H_
#define _CAN_LLD_H_

#include "spc5_lld.h"
#include "lldconf.h"

#if (LLD_USE_CAN == TRUE) || defined(__DOXYGEN__)

#include "spc5_flexcan.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    CAN status flags
 * @{
 */
/**
 * @brief   Errors rate warning.
 */
#define CAN_LIMIT_WARNING           1U
/**
 * @brief   Errors rate error.
 */
#define CAN_LIMIT_ERROR             2U
/**
 * @brief   Bus off condition reached.
 */
#define CAN_BUS_OFF_ERROR           4U
/**
 * @brief   Framing error of some kind on the CAN bus.
 */
#define CAN_FRAMING_ERROR           8U
/**
 * @brief   Overflow in receive queue.
 */
#define CAN_OVERFLOW_ERROR          16U
/** @} */

/**
 * @brief   Special mailbox identifier.
 */
#define CAN_ANY_MAILBOX             0U

/**
 * @brief   FIFO operation identifier.
 */
#define CAN_FIFO                    0xFFU

/**
 * @brief   This switch defines whether the driver implementation supports
 *          a low power switch mode with an automatic wakeup feature.
 */
#define CAN_SUPPORTS_SLEEP          FALSE

/**
 * @brief   This implementation supports eight FIFO receive filters.
 */
#define SPC5_CAN_MAX_FILTERS        8U

/**
 * @brief   Enable filters.
 */
#define SPC5_CAN_FILTER_ON          1U

/**
 * @brief   Disable filters.
 */
#define SPC5_CAN_FILTER_OFF         0U

/**
 * @name    CAN registers helper macros
 * @{
 */
#define CAN_MCR_MAXMB(n)            (uint32_t)(n)
#define CAN_MCR_AEN                 (1UL << 12)
#define CAN_MCR_LPRIO_EN            (1UL << 13)
#define CAN_MCR_BCC                 (1UL << 16)
#define CAN_MCR_SRX_DIS             (1UL << 17)
#define CAN_MCR_LPM_ACK             (1UL << 20)
#define CAN_MCR_WRN_EN              (1UL << 21)
#define CAN_MCR_SUPV                (1UL << 23)
#define CAN_MCR_FRZ_ACK             (1UL << 24)
#define CAN_MCR_WAK_MSK             (1UL << 26)
#define CAN_MCR_NOT_RDY             (1UL << 27)
#define CAN_MCR_HALT                (1UL << 28)
#define CAN_MCR_FEN                 (1UL << 29)
#define CAN_MCR_FRZ                 (1UL << 30)
#define CAN_MCR_MDIS                (1UL << 31)

#define CAN_CTRL_PROPSEG(n)         (uint32_t)(n)
#define CAN_CTRL_LOM                (1UL << 3)
#define CAN_CTRL_TSYN               (1UL << 5)
#define CAN_CTRL_BOFF_REC           (1UL << 6)
#define CAN_CTRL_SMP                (1UL << 7)
#define CAN_CTRL_RWRN_MSK           (1UL<< 10)
#define CAN_CTRL_TWRN_MSK           (1UL<< 11)
#define CAN_CTRL_LPB                (1UL<< 12)
#define CAN_CTRL_CLK_SRC            (1UL<< 13)
#define CAN_CTRL_ERR_MSK            (1UL<< 14)
#define CAN_CTRL_BOFF_MSK           (1UL<< 15)
#define CAN_CTRL_PSEG2(n)           ((uint32_t)(n) << 16)
#define CAN_CTRL_PSEG1(n)           ((uint32_t)(n) << 19)
#define CAN_CTRL_RJW(n)             ((uint32_t)(n) << 22)
#define CAN_CTRL_PRESDIV(n)         ((uint32_t)(n) << 24)

#define CAN_IDE_STD                 0U          /**< @brief Standard id.    */
#define CAN_IDE_EXT                 1U           /**< @brief Extended id.    */

#define CAN_RTR_DATA                0U           /**< @brief Data frame.     */
#define CAN_RTR_REMOTE              1U           /**< @brief Remote frame.   */

#define CAN_ESR_ERR_INT             (1UL << 1)
#define CAN_ESR_BOFF_INT            (1UL << 2)
#define CAN_ESR_TWRN_INT            (1UL << 17)
#define CAN_ESR_RWRN_INT            (1UL << 16)

#define CAN_RX_FIFO_FORMAT_A        0x00U
#define CAN_RX_FIFO_FORMAT_B        0x01U
#define CAN_RX_FIFO_FORMAT_C        0x02U
#define CAN_RX_FIFO_FORMAT_D        0x03U
/** @} */

/**
 * @brief   Macro for compatibility with MCAN driver.
 */
#define CAN_ANY_RXBUFFER            CAN_ANY_MAILBOX
#define CAN_ANY_TXBUFFER            CAN_ANY_MAILBOX
#define CAN_ID_STD                  CAN_IDE_STD
/** @} */

/**
 * @brief  canmsg_t constant
 */
#define CAN_MSG_OK        			(1UL << 1)	/**< @brief message OK.    		  */
#define CAN_MSG_WAIT				(1UL << 2)	/**< @brief mailbox is non empty. wait   */
#define CAN_MSG_NOFIFO				(1UL << 3)  /**< @brief FIFO not enabled */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   CAN filters enable setting.
 */
#if !defined(SPC5_CAN_USE_FILTERS) || defined(__DOXYGEN__)
#define SPC5_CAN_USE_FILTERS                FALSE
#endif

/**
 * @brief   FlexCAN clock source selection.
 */
#if !defined(SPC5_CAN_FLEXCAN_USE_EXT_CLK) || defined(__DOXYGEN__)
#define SPC5_CAN_FLEXCAN_USE_EXT_CLK        FALSE
#endif

/**
 * @brief   CAN1 driver enable switch.
 * @details If set to @p TRUE the support for CAN1 is included.
 */
#if !defined(SPC5_CAN_USE_FLEXCAN0) || defined(__DOXYGEN__)
#define SPC5_CAN_USE_FLEXCAN0               FALSE
#endif

/**
 * @brief   CAN2 driver enable switch.
 * @details If set to @p TRUE the support for CAN2 is included.
 */
#if !defined(SPC5_CAN_USE_FLEXCAN1) || defined(__DOXYGEN__)
#define SPC5_CAN_USE_FLEXCAN1               FALSE
#endif

/**
 * @brief   CAN3 driver enable switch.
 * @details If set to @p TRUE the support for CAN3 is included.
 */
#if !defined(SPC5_CAN_USE_FLEXCAN2) || defined(__DOXYGEN__)
#define SPC5_CAN_USE_FLEXCAN2               FALSE
#endif

/**
 * @brief   CAN4 driver enable switch.
 * @details If set to @p TRUE the support for CAN4 is included.
 */
#if !defined(SPC5_CAN_USE_FLEXCAN3) || defined(__DOXYGEN__)
#define SPC5_CAN_USE_FLEXCAN3               FALSE
#endif

/**
 * @brief   CAN5 driver enable switch.
 * @details If set to @p TRUE the support for CAN5 is included.
 */
#if !defined(SPC5_CAN_USE_FLEXCAN4) || defined(__DOXYGEN__)
#define SPC5_CAN_USE_FLEXCAN4               FALSE
#endif

/**
 * @brief   CAN6 driver enable switch.
 * @details If set to @p TRUE the support for CAN6 is included.
 */
#if !defined(SPC5_CAN_USE_FLEXCAN5) || defined(__DOXYGEN__)
#define SPC5_CAN_USE_FLEXCAN5               FALSE
#endif

/**
 * @brief   Number of RX mailboxes.
 */
#if !defined(SPC5_CAN_NUM_RX_MAILBOXES) || defined(__DOXYGEN__)
#define SPC5_CAN_NUM_RX_MAILBOXES           8
#endif

/**
 * @brief   Number of TX mailboxes.
 */
#if !defined(SPC5_CAN_NUM_TX_MAILBOXES) || defined(__DOXYGEN__)
#define SPC5_CAN_NUM_TX_MAILBOXES           24
#endif

/**
 * @brief   CAN1 interrupt priority level setting.
 */
#if !defined(SPC5_CAN_FLEXCAN0_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_CAN_FLEXCAN0_IRQ_PRIORITY      11
#endif

/**
 * @brief   CAN2 interrupt priority level setting.
 */
#if !defined(SPC5_CAN_FLEXCAN1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_CAN_FLEXCAN1_IRQ_PRIORITY      11
#endif

/**
 * @brief   CAN3 interrupt priority level setting.
 */
#if !defined(SPC5_CAN_FLEXCAN2_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_CAN_FLEXCAN2_IRQ_PRIORITY      11
#endif

/**
 * @brief   CAN4 interrupt priority level setting.
 */
#if !defined(SPC5_CAN_FLEXCAN3_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_CAN_FLEXCAN3_IRQ_PRIORITY      11
#endif

/**
 * @brief   CAN5 interrupt priority level setting.
 */
#if !defined(SPC5_CAN_FLEXCAN4_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_CAN_FLEXCAN4_IRQ_PRIORITY      11
#endif

/**
 * @brief   CAN6 interrupt priority level setting.
 */
#if !defined(SPC5_CAN_FLEXCAN5_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_CAN_FLEXCAN5_IRQ_PRIORITY      11
#endif

/**
 * @brief   FlexCAN-0 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_CAN_FLEXCAN0_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_CAN_FLEXCAN0_START_PCTL        (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   FlexCAN-0 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_CAN_FLEXCAN0_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_CAN_FLEXCAN0_STOP_PCTL         (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   FlexCAN-1 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_CAN_FLEXCAN1_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_CAN_FLEXCAN1_START_PCTL        (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   FlexCAN-1 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_CAN_FLEXCAN1_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_CAN_FLEXCAN2_STOP_PCTL         (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   FlexCAN-2 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_CAN_FLEXCAN2_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_CAN_FLEXCAN2_START_PCTL        (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   FlexCAN-2 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_CAN_FLEXCAN2_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_CAN_FLEXCAN2_STOP_PCTL         (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   FlexCAN-3 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_CAN_FLEXCAN3_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_CAN_FLEXCAN3_START_PCTL        (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   FlexCAN-3 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_CAN_FLEXCAN3_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_CAN_FLEXCAN3_STOP_PCTL         (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   FlexCAN-4 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_CAN_FLEXCAN4_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_CAN_FLEXCAN4_START_PCTL        (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   FlexCAN-4 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_CAN_FLEXCAN4_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_CAN_FLEXCAN4_STOP_PCTL         (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   FlexCAN-5 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_CAN_FLEXCAN5_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_CAN_FLEXCAN5_START_PCTL        (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   FlexCAN-5 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_CAN_FLEXCAN5_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_CAN_FLEXCAN5_STOP_PCTL         (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (SPC5_CAN_USE_FLEXCAN0 == TRUE) && (SPC5_HAS_FLEXCAN0 == FALSE)
#error "CAN1 not present in the selected device"
#endif

#if (SPC5_CAN_USE_FLEXCAN1 == TRUE) && (SPC5_HAS_FLEXCAN1 == FALSE)
#error "CAN2 not present in the selected device"
#endif

#if (SPC5_CAN_USE_FLEXCAN2 == TRUE) && (SPC5_HAS_FLEXCAN2 == FALSE)
#error "CAN3 not present in the selected device"
#endif

#if (SPC5_CAN_USE_FLEXCAN3 == TRUE) && (SPC5_HAS_FLEXCAN3 == FALSE)
#error "CAN4 not present in the selected device"
#endif

#if (SPC5_CAN_USE_FLEXCAN4 == TRUE) && (SPC5_HAS_FLEXCAN4 == FALSE)
#error "CAN5 not present in the selected device"
#endif

#if (SPC5_CAN_USE_FLEXCAN5 == TRUE) && (SPC5_HAS_FLEXCAN5 == FALSE)
#error "CAN6 not present in the selected device"
#endif

#if (SPC5_CAN_USE_FLEXCAN0 == FALSE) && (SPC5_CAN_USE_FLEXCAN1 == FALSE)                        \
    && (SPC5_CAN_USE_FLEXCAN2 == FALSE) && (SPC5_CAN_USE_FLEXCAN3 == FALSE)                    \
    && (SPC5_CAN_USE_FLEXCAN4 == FALSE) && (SPC5_CAN_USE_FLEXCAN5 == FALSE)
#error "CAN driver activated but no CAN peripheral assigned"
#endif

#if (SPC5_CAN_NUM_RX_MAILBOXES < 1U) || (SPC5_CAN_NUM_RX_MAILBOXES > SPC5_FLEXCAN0_MB)
#error "invalid number of RX mailboxes"
#endif

#if (SPC5_CAN_NUM_TX_MAILBOXES < 1U) || (SPC5_CAN_NUM_TX_MAILBOXES > SPC5_FLEXCAN0_MB)
#error "invalid number of TX mailboxes"
#endif

#if (SPC5_CAN_NUM_RX_MAILBOXES + SPC5_CAN_NUM_TX_MAILBOXES) > SPC5_FLEXCAN0_MB
#error "invalid amount of RX and TX mailboxes"
#endif

/**
 * @brief   Number of RX mailboxes to be allocated.
 */
#define CAN_TX_MAILBOXES            SPC5_CAN_NUM_TX_MAILBOXES

/**
 * @brief   Number of TX mailboxes to be allocated.
 */
#define CAN_RX_MAILBOXES            SPC5_CAN_NUM_RX_MAILBOXES

#if defined(_SPC564Axx_)
/**
 * @brief   CAN Clock clock divider value.
 * @note    Possible values are 1 and 2.
 */
#if !defined(SPC5_CANCLK_DIV_VALUE) || defined(__DOXYGEN__)
#define SPC5_CANCLK_DIV_VALUE               1U
#endif
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/
/**
 * @brief   Type of a structure representing a CAN driver.
 */
typedef struct CANDriver CANDriver;

/**
 * @brief   Type of a transmission mailbox index.
 */
typedef uint32_t canmbx_t;

 /**
  * @brief   type of return message for can driver
  */
 typedef uint32_t canmsg_t;

 /* PPC register definitions not compatible with MISRA rules on bit-fields 
   (Rules 6.1). */
/*lint -e46 */
/**
 * @brief   CAN TX MB structure.
 */
typedef struct {
  union {
    vuint32_t R;
    struct {
      vuint8_t:4;
      vuint8_t CODE:4;
      vuint8_t:1;
      vuint8_t SRR:1;
      vuint8_t IDE:1;
      vuint8_t RTR:1;
      vuint8_t LENGTH:4;
      vuint16_t TIMESTAMP:16;
    } B;
  } CS;

  union {
    vuint32_t R;
    struct {
      vuint8_t PRIO:3;
      vuint32_t ID:29;
    } B;
  } ID;
  vuint32_t DATA[2];     /* Data buffer in words (32 bits) */
} CAN_TxMailBox_TypeDef;

/**
 * @brief   CAN transmission frame.
 * @note    Accessing the frame data as word16 or word32 is not portable because
 *          machine data endianness, it can be still useful for a quick filling.
 */
typedef struct { 
  union{  
    uint8_t  DLC;                          /**< @brief Data Length C       */
	uint8_t  LENGTH;                       /**< @brief Data length.        */
  };
  uint8_t    RTR;                          /**< @brief Frame type.         */
  union { 
    uint8_t  TYPE;                         /**< @brief Id type. STD or XTD */
    uint8_t  IDE;                          /**< @brief Id type. STD or XTD */  
  };
  union { 
    uint32_t  SID;                         /**< @brief Standard identifier.*/ 
    uint32_t  EID;                         /**< @brief Extended identifier.*/  
	uint32_t  ID;	                       /**< @brief identifier.*/
  };
  union {
    uint8_t   data8[8];                    /**< @brief Frame data.         */
    uint16_t  data16[4];                   /**< @brief Frame data.         */
    uint32_t  data32[2];                   /**< @brief Frame data.         */
  };
} CANTxFrame;

/**
 * @brief   CAN received frame.
 * @note    Accessing the frame data as word16 or word32 is not portable because
 *          machine data endianness, it can be still useful for a quick filling.
 */
typedef struct {
    uint16_t TIME;                         /**< @brief Time stamp.         */
  union{  
    uint8_t  DLC;                          /**< @brief Data Length C       */
	uint8_t  LENGTH;                       /**< @brief Data length.        */
  };
  uint8_t    RTR;                          /**< @brief Frame type.         */
  union { 
    uint8_t  TYPE;                         /**< @brief Id type. STD or XTD */
    uint8_t  IDE;                          /**< @brief Id type. STD or XTD */  
  };
  union { 
    uint32_t  SID;                         /**< @brief Standard identifier.*/ 
    uint32_t  EID;                         /**< @brief Extended identifier.*/  
	uint32_t  ID;	                       /**< @brief identifier.*/
  };
  union {
    uint8_t   data8[8];                    /**< @brief Frame data.         */
    uint16_t  data16[4];                   /**< @brief Frame data.         */
    uint32_t  data32[2];                   /**< @brief Frame data.         */
  };
} CANRxFrame;

/**
 * @brief   CAN filter.
 * @note    Refer to the SPC5 reference manual for info about filters.
 */
typedef struct {
  /**
   * @brief   Filter scale.
   * @note    This bit represents the EXT bit associated to this
   *          filter (0=standard ID mode, 1=extended ID mode).
   */
  uint32_t                  scale:1;
  /**
   * @brief   Filter register (identifier).
   */
  uint32_t                  register1;
} CANFilter;
/*lint +e46 */

/**
 * @brief   FLEXCAN notification callback type.
 *
 * @param[in] canp      pointer to the @p CAN Driver object triggering the
 *                      callback
 * @param[in] crfp      RX frame containg data
 */
typedef void (*flexcancallback_t)(CANDriver *canp, CANRxFrame crfp);

/**
 * @brief   FLEXCAN Error callback type.
 *
 * @param[in] canp                 Pointer to the @p CAN Driver object triggering the error
 * @param[in] esr                  ESR Register Value
 * @param[in] rx_err_counter       Receive Error Counter
 * @param[in] tx_err_counter       Transmit Error Counter
 */
typedef void (*flexcanerrcallback_t)(CANDriver *canp, uint32_t esr,uint8_t rx_err_counter, uint8_t tx_err_counter);

/**
 * @brief   Driver configuration structure.
 */
typedef struct {
 /**
   * @brief   CAN MCR register initialization data.
   * @note    Some bits in this register are enforced by the driver regardless
   *          their status in this field.
   */
  uint32_t                  mcr;
  /**
   * @brief   CAN CTRL register initialization data.
   * @note    Some bits in this register are enforced by the driver regardless
   *          their status in this field.
   */
  uint32_t                  ctrl;
#if SPC5_CAN_USE_FILTERS
  /**
   * @brief   CAN filters structure.
   */
  CANFilter                 RxFilter[CAN_RX_MAILBOXES];
#endif
  /**
   * @brief   CAN FIFO configuration
   */
  uint32_t                 use_fifo;
  uint8_t				   id_acceptance_mask;
  CANFilter                id_filter[SPC5_CAN_MAX_FILTERS * 4U]; 
  /**
   * @brief   first MB available for RX and TX in the driver. if FIFO is enabled
   *          the firts MB available is 8, otherwise is 0
   */
  uint8_t                  first_mb_available;
   /**
   * @brief   FIFO related callbacks
   */ 
  flexcancallback_t        fifo_rx_callback;
  /**
  * @brief    Error callback
  */
  flexcanerrcallback_t     flexcan_err_callback;

} CANConfig;

/**
 * @brief   Structure representing an CAN driver.
 */
struct CANDriver{
  /**
   * @brief   Current configuration data.
   */
  const CANConfig           *config;
  /**
   * @brief   Pointer to the CAN registers.
   */
  volatile struct spc5_flexcan *flexcan;
};


/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    Macro Functions
 * @{
 */
/**
 * @brief   Converts a mailbox index to a bit mask.
 */
#define CAN_MAILBOX_TO_MASK(mbx) (1 << ((mbx) - 1))
/** @} */

/*===========================================================================*/
/* IRQ Handlers                                                              */
/*===========================================================================*/

#if (SPC5_CAN_USE_FLEXCAN0 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_FLEXCAN0_SHARED_IRQ == FALSE)
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_00_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_01_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_02_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_03_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_04_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_05_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_06_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_07_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_08_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_09_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_10_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_11_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_12_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_13_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_14_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_15_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_16_31_HANDLER);
#if (SPC5_FLEXCAN0_MB == 64U)
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_32_63_HANDLER);
#endif
IRQ_HANDLER(SPC5_FLEXCAN0_ESR_ERR_INT_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN0_ESR_BOFF_HANDLER);
#else
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_08_11_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_12_15_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_16_31_HANDLER);
#if (SPC5_FLEXCAN0_MB == 64U)
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_32_63_HANDLER);
#endif
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_00_03_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_04_07_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN0_ESR_ERR_INT_HANDLER); 
IRQ_HANDLER(SPC5_FLEXCAN0_ESR_BOFF_HANDLER);
#endif
#endif /* SPC5_CAN_USE_FLEXCAN0 */
#if (SPC5_CAN_USE_FLEXCAN1 == TRUE)|| defined(__DOXYGEN__)
#if (SPC5_FLEXCAN1_SHARED_IRQ == FALSE)
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_00_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_01_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_02_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_03_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_04_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_05_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_06_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_07_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_08_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_09_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_10_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_11_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_12_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_13_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_14_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_15_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_16_31_HANDLER);
#if (SPC5_FLEXCAN1_MB == 64U)
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_32_63_HANDLER);
#endif
IRQ_HANDLER(SPC5_FLEXCAN1_ESR_ERR_INT_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN1_ESR_BOFF_HANDLER);
#else
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_08_11_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_12_15_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_16_31_HANDLER);
#if (SPC5_FLEXCAN1_MB == 64U)
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_32_63_HANDLER);
#endif
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_00_03_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_04_07_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN1_ESR_ERR_INT_HANDLER); 
IRQ_HANDLER(SPC5_FLEXCAN1_ESR_BOFF_HANDLER);
#endif
#endif /* SPC5_CAN_USE_FLEXCAN1 */
#if (SPC5_CAN_USE_FLEXCAN2 == TRUE)|| defined(__DOXYGEN__)
#if (SPC5_FLEXCAN2_SHARED_IRQ == FALSE)
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_00_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_01_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_02_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_03_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_04_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_05_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_06_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_07_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_08_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_09_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_10_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_11_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_12_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_13_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_14_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_15_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_16_31_HANDLER);
#if (SPC5_FLEXCAN2_MB == 64U)
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_32_63_HANDLER);
#endif
IRQ_HANDLER(SPC5_FLEXCAN2_ESR_ERR_INT_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN2_ESR_BOFF_HANDLER);
#else
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_08_11_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_12_15_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_16_31_HANDLER);
#if (SPC5_FLEXCAN2_MB == 64U)
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_32_63_HANDLER);
#endif
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_00_03_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_04_07_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN2_ESR_ERR_INT_HANDLER); 
IRQ_HANDLER(SPC5_FLEXCAN2_ESR_BOFF_HANDLER);
#endif
#endif /* SPC5_CAN_USE_FLEXCAN2 */
#if (SPC5_CAN_USE_FLEXCAN3 == TRUE)|| defined(__DOXYGEN__)
#if (SPC5_FLEXCAN3_SHARED_IRQ == FALSE)
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_00_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_01_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_02_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_03_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_04_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_05_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_06_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_07_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_08_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_09_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_10_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_11_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_12_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_13_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_14_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_15_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_16_31_HANDLER);
#if (SPC5_FLEXCAN3_MB == 64U)
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_32_63_HANDLER);
#endif
IRQ_HANDLER(SPC5_FLEXCAN3_ESR_ERR_INT_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN3_ESR_BOFF_HANDLER);
#else
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_08_11_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_12_15_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_16_31_HANDLER);
#if (SPC5_FLEXCAN3_MB == 64U)
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_32_63_HANDLER);
#endif
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_00_03_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_04_07_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN3_ESR_ERR_INT_HANDLER); 
IRQ_HANDLER(SPC5_FLEXCAN3_ESR_BOFF_HANDLER);
#endif
#endif /* SPC5_CAN_USE_FLEXCAN3 */
#if (SPC5_CAN_USE_FLEXCAN4 == TRUE)|| defined(__DOXYGEN__)
#if (SPC5_FLEXCAN4_SHARED_IRQ == FALSE)
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_00_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_01_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_02_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_03_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_04_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_05_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_06_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_07_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_08_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_09_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_10_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_11_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_12_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_13_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_14_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_15_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_16_31_HANDLER);
#if (SPC5_FLEXCAN4_MB == 64U)
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_32_63_HANDLER);
#endif
IRQ_HANDLER(SPC5_FLEXCAN4_ESR_ERR_INT_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN4_ESR_BOFF_HANDLER);
#else
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_08_11_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_12_15_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_16_31_HANDLER);
#if (SPC5_FLEXCAN4_MB == 64U)
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_32_63_HANDLER);
#endif
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_00_03_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_04_07_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN4_ESR_ERR_INT_HANDLER); 
IRQ_HANDLER(SPC5_FLEXCAN4_ESR_BOFF_HANDLER);
#endif
#endif /* SPC5_CAN_USE_FLEXCAN4 */
#if (SPC5_CAN_USE_FLEXCAN5 == TRUE)|| defined(__DOXYGEN__)
#if (SPC5_FLEXCAN5_SHARED_IRQ == FALSE)
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_00_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_01_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_02_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_03_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_04_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_05_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_06_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_07_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_08_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_09_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_10_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_11_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_12_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_13_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_14_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_15_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_16_31_HANDLER);
#if (SPC5_FLEXCAN5_MB == 64U)
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_32_63_HANDLER);
#endif
IRQ_HANDLER(SPC5_FLEXCAN5_ESR_ERR_INT_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN5_ESR_BOFF_HANDLER);
#else
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_08_11_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_12_15_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_16_31_HANDLER);
#if (SPC5_FLEXCAN5_MB == 64U)
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_32_63_HANDLER);
#endif
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_00_03_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_04_07_HANDLER);
IRQ_HANDLER(SPC5_FLEXCAN5_ESR_ERR_INT_HANDLER); 
IRQ_HANDLER(SPC5_FLEXCAN5_ESR_BOFF_HANDLER);
#endif
#endif /* SPC5_CAN_USE_FLEXCAN5 */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if (SPC5_CAN_USE_FLEXCAN0 == TRUE) && !defined(__DOXYGEN__)
extern CANDriver CAND1;
#endif

#if (SPC5_CAN_USE_FLEXCAN1 == TRUE) && !defined(__DOXYGEN__)
extern CANDriver CAND2;
#endif

#if (SPC5_CAN_USE_FLEXCAN2 == TRUE) && !defined(__DOXYGEN__)
extern CANDriver CAND3;
#endif

#if (SPC5_CAN_USE_FLEXCAN3 == TRUE) && !defined(__DOXYGEN__)
extern CANDriver CAND4;
#endif

#if (SPC5_CAN_USE_FLEXCAN4 == TRUE) && !defined(__DOXYGEN__)
extern CANDriver CAND5;
#endif

#if (SPC5_CAN_USE_FLEXCAN5 == TRUE) && !defined(__DOXYGEN__)
extern CANDriver CAND6;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void can_lld_init(void);
  void can_lld_start(CANDriver *canp,const CANConfig *config);
  void can_lld_stop(CANDriver *canp);
  uint8_t can_lld_is_tx_empty(CANDriver *canp,
                           canmbx_t mailbox); 
  canmsg_t can_lld_transmit(CANDriver *canp,
                        canmbx_t mailbox,
                        const CANTxFrame *ctfp);
  uint8_t can_lld_is_rx_nonempty(CANDriver *canp,
                              canmbx_t mailbox);
  canmsg_t can_lld_receive(CANDriver *canp,
                       canmbx_t mailbox,
                       CANRxFrame *crfp);
#ifdef __cplusplus
}
#endif

#endif /* LLD_USE_CAN */

#endif /* _CAN_LLD_H_ */

/** @} */
