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
 * @file    fccu_lld.h
 * @brief   SPC5xx FCCU low level driver header.
 *
 * @addtogroup FCCU
 * @{
 */

#ifndef _FCCU_LLD_H_
#define _FCCU_LLD_H_

#include "spc5_lld.h"
#include "lldconf.h"

#if (LLD_USE_FCCU == TRUE) || defined(__DOXYGEN__)

#include "spc5_fccu.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    FCCU Control Register bits definitions
 * @{
 */
#define SPC5_FCCU_CTRL_OPS_IDLE             0U
#define SPC5_FCCU_CTRL_OPS_PROGRESS         1U
#define SPC5_FCCU_CTRL_OPS_ABORTED          2U
#define SPC5_FCCU_CTRL_OPS_SUCCESS          3U
#define SPC5_FCCU_CTRL_OPR_OP0              0U  /* No operation.*/
#define SPC5_FCCU_CTRL_OPR_OP1              1U  /* Set the FCCU into the CONFIG state.*/
#define SPC5_FCCU_CTRL_OPR_OP2              2U  /* Set the FCCU into the NORMAL state.*/
#define SPC5_FCCU_CTRL_OPR_OP3              3U  /* Read the FCCU state.*/
#define SPC5_FCCU_CTRL_OPR_OP4              4U  /* Read the FCCU frozen status flags.*/
#define SPC5_FCCU_CTRL_OPR_OP5              5U  /* Read the FCCU frozen status flags.*/
#define SPC5_FCCU_CTRL_OPR_OP6              6U  /* Read the FCCU frozen status flags.*/
#define SPC5_FCCU_CTRL_OPR_OP7              7U  /* Read the FCCU frozen status flags.*/
#define SPC5_FCCU_CTRL_OPR_OP10             10U /* Read the RF status register.*/
#define SPC5_FCCU_CTRL_OPR_OP12             12U /* RF status clear operation in progress.*/
#define SPC5_FCCU_CTRL_OPR_OP13             13U /* Clear the freeze status registers.*/
#define SPC5_FCCU_CTRL_OPR_OP14             14U /* CONFIG to NORMAL FCCU state (configuration timeout) in progress.*/
#define SPC5_FCCU_CTRL_OPR_OP15             15U /* Clear the operation status.*/
#define SPC5_FCCU_CTRL_OPR_OP17             17U /* Read the ALARM timer.*/
#define SPC5_FCCU_CTRL_OPR_OP19             19U /* Read the CFG timer.*/
#define SPC5_FCCU_CTRL_OPR_OP20             20U /* Read the Error Pin low counter value.*/
/** @} */

/**
 * @name    FCCU Control Key Register bits definitions
 * @{
 */
#define SPC5_FCCU_CTRLK_OP1_KEY             0x913756AFUL
#define SPC5_FCCU_CTRLK_OP2_KEY             0x825A132BUL
/** @} */

/**
 * @name    FCCU Configuration Register bits definitions
 * @{
 */
#define SPC5_FCCU_CFG_OD_PUSH_PULL          0U
#define SPC5_FCCU_CFG_OD_OPEN_DRAIN         1U
#define SPC5_FCCU_CFG_SM_SLOW               0U
#define SPC5_FCCU_CFG_SM_FAST               1U
#define SPC5_FCCU_CFG_PS_1H_0L              0U
#define SPC5_FCCU_CFG_PS_1L_0H              1U
#define SPC5_FCCU_CFG_FOM_DUAL_RAIL         0U
#define SPC5_FCCU_CFG_FOM_TIME_SWITCHING    1U
#define SPC5_FCCU_CFG_FOM_BI_STABLE         2U
/** @} */

/**
 * @name    FCCU RFS Configuration Registers bits definitions
 * @{
 */
#define SPC5_FCCU_RFSC_MASK                 (3UL)
#define SPC5_FCCU_RFSC_NO_RESET             0U
#define SPC5_FCCU_RFSC_SHORT_FUNCTIONAL     1U
#define SPC5_FCCU_RFSC_LONG_FUNCTIONAL      2U
/** @} */

/**
 * @name    FCCU Recoverable Fault Key Register bits definitions
 * @{
 */

#define SPC5_FCCU_RECOVERABLE_FAULT_KEY     0xAB3498FEUL
/** @} */

/**
 * @name    FCCU RF Enable Registers bits definitions
 * @{
 */
#define SPC5_FCCU_RFE_MASK                  (1UL)
#define SPC5_FCCU_RFE_DISABLE               0U
#define SPC5_FCCU_RFE_ENABLE                1U
/** @} */

/**
 * @name    FCCU RF Timeout Enable Registers bits definitions
 * @{
 */
#define SPC5_FCCU_RFTOE_MASK                (1UL)
#define SPC5_FCCU_RFTOE_DISABLE             0U
#define SPC5_FCCU_RFTOE_ENABLE              1U
/** @} */

/**
 * @name    FCCU Transient Register bits definitions
 * @{
 */
#define SPC5_FCCU_TRANSKEY_CONF_UNLOCK      0xBC
/** @} */

/**
 * @name    FCCU IRQ Alarm Enable Registers bits definitions
 * @{
 */
#define SPC5_FCCU_IRQEN_MASK                (1UL)
#define SPC5_FCCU_IRQEN_DISABLE             0U
#define SPC5_FCCU_IRQEN_ENABLE              1U
/** @} */

/**
 * @name    FCCU Status Register bits definitions
 * @{
 */
#define SPC5_FCCU_STATUS_NORMAL             0U
#define SPC5_FCCU_STATUS_CONFIG             1U
#define SPC5_FCCU_STATUS_ALARM              2U
#define SPC5_FCCU_STATUS_FAULT              3U
/** @} */

/**
 * @name    FCCU NMI Enable Registers bits definitions
 * @{
 */
#define SPC5_FCCU_NMIEN_MASK                (1UL)
#define SPC5_FCCU_NMIEN_DISABLE             0U
#define SPC5_FCCU_NMIEN_ENABLE              1U
/** @} */

/**
 * @name    FCCU EOUT Signaling Enable Registers bits definitions
 * @{
 */
#define SPC5_FCCU_EOUTEN_MASK               (1UL)
#define SPC5_FCCU_EOUTEN_DISABLE            0U
#define SPC5_FCCU_EOUTEN_ENABLE             1U
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   FCCU Configuration interrupt priority level setting.
 */
#if !defined(SPC5_FCCU_CONFIG_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_FCCU_CONFIG_IRQ_PRIO           INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#endif


/**
 * @brief   FCCU Alarm interrupt priority level setting.
 */
#if !defined(SPC5_FCCU_ALARM_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_FCCU_ALARM_IRQ_PRIO            INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#endif

/**
 * @brief   FCCU max value of Delta_T in us.
 */
#if !defined(SPC5_FCCU_MAX_DELTA_T_US) || defined(__DOXYGEN__)
#define SPC5_FCCU_MAX_DELTA_T_US            10000UL
#endif

/**
 * @brief   FCCU error hook.
 */
#if !defined(SPC5_FCCU_ERROR_HOOK) || defined(__DOXYGEN__)
#define SPC5_FCCU_ERROR_HOOK(fccudp)        irqSysHalt()
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a structure representing a FCCU driver.
 */
typedef struct FCCUDriver FCCUDriver;

/**
 * @brief   FCCU notification callback type.
 *
 * @param[in] fccup     pointer to the @p FCCUDriver object triggering the
 *                      callback
 */
typedef void (*fccucallback_t)(FCCUDriver *fccudp);

/**
 * @brief   FCCU channel configuration structure.
 */
typedef struct {
  /**
   * @brief   Flag set if the alarm timeout is enabled on the channel.
   */
  uint8_t                   alarm_timeout_en;
  /**
   * @brief   Flag set if the alarm IRQ is enabled on the channel.
   */
  uint8_t                   alarm_irq_en;
  /**
   * @brief   Callback function related to the channel alarm IRQ.
   */
  fccucallback_t            alarm_irq_cb;
  /**
   * @brief   Internal recoverable fault reaction.
   */
  uint8_t                   int_reaction;
  /**
   * @brief   Flag set if the Non-Maskerable Interrupt is enabled on the channel.
   */
  uint8_t                   nmi_en;
  /**
   * @brief   Flag set if the error out signaling is enabled on the channel.
   */
  uint8_t                   eout_en;
} FCCUChannelConfig;

/**
 * @brief   FCCU driver configuration structure.
 */
typedef struct {
  /**
   * @brief   Flag set if the EOUT pins must be enabled after reset.
   */
  uint8_t                   eout_en_after_reset;
  /**
   * @brief   FCCU open drain selection.
   */
  uint8_t                   od;
  /**
   * @brief   FCCU switching mode selection.
   */
  uint8_t                   sm;
  /**
   * @brief   FCCU polarity switch selection.
   */
  uint8_t                   ps;
  /**
   * @brief   FCCU fault outptut mode selection.
   */
  uint8_t                   fom;
  /**
   * @brief   FCCU configuration timeout value.
   */
  uint32_t                  config_timeout;
  /**
   * @brief   Flag set if the configuration timeout irq is enabled.
   */
  uint32_t                  config_timeout_irq_en;
  /**
   * @brief   Callback function related to the configuration timeout IRQ.
   */
  fccucallback_t            config_irq_cb;
  /**
   * @brief   FCCU alarm timeout value.
   */
  uint32_t                  alarm_timeout;
  /**
   * @brief   FCCU delta_t value [us].
   */
  uint32_t                  delta_t;
  /**
   * @brief   FCCU channels configurations.
   */
  const FCCUChannelConfig   *channel[128];
} FCCUConfig;

/**
 * @brief   Structure representing an FCCU driver.
 */
struct FCCUDriver {
  /**
   * @brief   Current configuration data.
   */
  const FCCUConfig          *config;
  /**
   * @brief   Pointer to the FCCU registers block.
   */
  volatile struct spc5_fccu *fccup;
};


/**
 * @brief   Structure representing the FCCU status.
 */
typedef struct {
  uint8_t                   fsm_status;

  uint32_t                  rf_status_reg[SPC5_FCCU_CH_NUMBER / 32U];
} FCCUStatus;


/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   FCCU register value calcutation.
 * @details Return an unsigned int with val left-shifted of pos bits and "1" in 
 *          the other positions.
 */
#define SPC5_FCCU_SET_REG_FIELD(reg, mask, pos, val)       (((uint32_t)reg & ~((uint32_t)(mask) << pos)) | ((uint32_t)(val) << pos))

/*===========================================================================*/
/* IRQ Handlers                                                              */
/*===========================================================================*/

IRQ_HANDLER(SPC5_FCCU_ALARM_HANDLER);
IRQ_HANDLER(SPC5_FCCU_CONFIG_HANDLER);

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

extern FCCUDriver FCCUD1;

#ifdef __cplusplus
extern "C" {
#endif
  void fccu_lld_init(void);
  void fccu_lld_clear(FCCUDriver *fccudp);
  void fccu_lld_reset(FCCUDriver *fccudp);
  void fccu_lld_start(FCCUDriver *fccudp, const FCCUConfig *config);
  uint8_t fccu_lld_status(FCCUDriver *fccudp, FCCUStatus *status);
  void fccu_lld_clear_fault(FCCUDriver *fccudp, uint8_t rf_ch);
#ifdef __cplusplus
}
#endif

#endif /* LLD_USE_FCCU */

#endif /* _FCCU_LLD_H_ */

/** @} */
