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
 * @file    stm_lld.h
 * @brief   SPC5xx STM low level driver header.
 *
 * @addtogroup STM
 * @{
 */

#ifndef _STM_LLD_H_
#define _STM_LLD_H_

#include "spc5_lld.h"
#include "lldconf.h"

#if (LLD_USE_STM == TRUE) || defined(__DOXYGEN__)

#include "spc5_stm.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    STM definitions
 * @{
 */
#define STM_CR_TEN_ENABLE     1U   /**< @brief STM Counter Enabled.  */
#define STM_CR_TEN_DISABLE    0U   /**< @brief STM Counter Disabled. */
#define STM_CR_FRZ_ENABLE     1U   /**< @brief STM Counter freeze when entering in debug.  */
#define STM_CIR_CEN_ENABLE    1U   /**< @brief Channel Enabled.      */
#define STM_CIR_CEN_DISABLE   0U   /**< @brief Channel Disabled.     */
#define STM_CCR_CIF_CLEAR     1U   /**< @brief Interrupt Clear Flag. */
#define STM_COUNTER_ENABLED	  1U   /**< @brief Counter Enabled Flag.  */
#define STM_COUNTER_DISABLED  0U   /**< @brief COunter Disabled Flag. */

/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
 
/**
 * @brief   STM0 driver enable switch.
 * @details If set to @p TRUE the support for STM0 is included.
 */
#if !defined(SPC5_STM_USE_STM0) || defined(__DOXYGEN__)
#define SPC5_STM_USE_STM0                          FALSE
#endif 

/**
 * @brief   STM1 driver enable switch.
 * @details If set to @p TRUE the support for STM1 is included.
 */
#if !defined(SPC5_STM_USE_STM1) || defined(__DOXYGEN__)
#define SPC5_STM_USE_STM1                          FALSE
#endif 

/**
 * @brief   STM2 driver enable switch.
 * @details If set to @p TRUE the support for STM2 is included.
 */
#if !defined(SPC5_STM_USE_STM2) || defined(__DOXYGEN__)
#define SPC5_STM_USE_STM2                          FALSE
#endif 

/**
 * @brief   STM0 System Clock Prescaler.
 * @details Select the prescaler for STM0 clock.
 */
#if !defined(SPC5_STM0_SYSCLOCK_PRE) || defined(__DOXYGEN__)
#define SPC5_STM0_SYSCLOCK_PRE                      1
#endif 

/**
 * @brief   STM1 System Clock Prescaler.
 * @details Select the prescaler for STM1 clock.
 */
#if !defined(SPC5_STM1_SYSCLOCK_PRE) || defined(__DOXYGEN__)
#define SPC5_STM1_SYSCLOCK_PRE                      1
#endif 

/**
 * @brief   STM2 System Clock Prescaler.
 * @details Select the prescaler for STM2 clock.
 */
#if !defined(SPC5_STM2_SYSCLOCK_PRE) || defined(__DOXYGEN__)
#define SPC5_STM2_SYSCLOCK_PRE                      1
#endif 

/**
 * @brief   STM0 CH0 IRQ priority.
 *
 */
#if !defined(SPC5_STM0_CH0_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_STM0_CH0_IRQ_PRIORITY                 INTC_PSR_ENABLE(INTC_PSR_MAINCORE,12)
#endif

/**
 * @brief   STM0 CH1 IRQ priority.
 *
 */
#if !defined(SPC5_STM0_CH1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_STM0_CH1_IRQ_PRIORITY                 INTC_PSR_ENABLE(INTC_PSR_MAINCORE,12)
#endif

/**
 * @brief   STM0 CH2 IRQ priority.
 *
 */
#if !defined(SPC5_STM0_CH2_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_STM0_CH2_IRQ_PRIORITY                 INTC_PSR_ENABLE(INTC_PSR_MAINCORE,12)
#endif

/**
 * @brief   STM0 CH3 IRQ priority.
 *
 */
#if !defined(SPC5_STM0_CH3_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_STM0_CH3_IRQ_PRIORITY                 INTC_PSR_ENABLE(INTC_PSR_MAINCORE,12)
#endif

/**
 * @brief   STM0 CH1_CH3 IRQ priority (for systems having shared IRQ (Monaco, Andorra)
 *
 */
#if !defined(SPC5_STM0_CH1_CH3_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_STM0_CH1_CH3_IRQ_PRIORITY             INTC_PSR_ENABLE(INTC_PSR_MAINCORE,12)
#endif

/**
 * @brief   STM1 CH0 IRQ priority.
 *
 */
#if !defined(SPC5_STM1_CH0_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_STM1_CH0_IRQ_PRIORITY                 INTC_PSR_ENABLE(INTC_PSR_MAINCORE,12)
#endif
 
/**
 * @brief   STM1 CH1 IRQ priority.
 *
 */
#if !defined(SPC5_STM1_CH1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_STM1_CH1_IRQ_PRIORITY                 INTC_PSR_ENABLE(INTC_PSR_MAINCORE,12)
#endif

/**
 * @brief   STM1 CH2 IRQ priority.
 *
 */
#if !defined(SPC5_STM1_CH2_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_STM1_CH2_IRQ_PRIORITY                 INTC_PSR_ENABLE(INTC_PSR_MAINCORE,12)
#endif

/**
 * @brief   STM1 CH3 IRQ priority.
 *
 */
#if !defined(SPC5_STM1_CH3_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_STM1_CH3_IRQ_PRIORITY                 INTC_PSR_ENABLE(INTC_PSR_MAINCORE,12)
#endif

/**
 * @brief   STM2 CH0 IRQ priority.
 *
 */
#if !defined(SPC5_STM2_CH0_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_STM2_CH0_IRQ_PRIORITY                 INTC_PSR_ENABLE(INTC_PSR_MAINCORE,12)
#endif

/**
 * @brief   STM2 CH1 IRQ priority.
 *
 */
#if !defined(SPC5_STM2_CH1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_STM2_CH1_IRQ_PRIORITY                 INTC_PSR_ENABLE(INTC_PSR_MAINCORE,12)
#endif

/**
 * @brief   STM2 CH2 IRQ priority.
 *
 */
#if !defined(SPC5_STM2_CH2_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_STM2_CH2_IRQ_PRIORITY                 INTC_PSR_ENABLE(INTC_PSR_MAINCORE,12)
#endif

/**
 * @brief   STM2 CH3 IRQ priority.
 *
 */
#if !defined(SPC5_STM2_CH3_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_STM2_CH3_IRQ_PRIORITY                 INTC_PSR_ENABLE(INTC_PSR_MAINCORE,12)
#endif

/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/
#if (SPC5_STM_USE_STM0 == FALSE) && (SPC5_STM_USE_STM1 == FALSE) && (SPC5_STM_USE_STM2 == FALSE)
#error "STM driver activated but no STM peripheral assigned"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief Type of a structure representing an STM driver.
 */
typedef struct STMDriver STMDriver;

/**
 * @brief   STM notification callback type.
 *
 * @param[in] adcp      pointer to the @p STMDriver object triggering the
 *                      callback
 */
typedef void (*stmcallback_t)(void);

/**
 * @brief   Structure representing a STM Channel configuration
 */
typedef struct {
  /**
   * @brief   Channel Enabled.
   */  
   uint8_t enabled;
  /**
   * @brief   number of tick for interrupt generation.
   */
   uint32_t counter;
  /**
   * @brief   callback function.
   */
   stmcallback_t callback;
}STM_CH_Config;

/**
 * @brief   Structure representing a STM driver.
 */
struct STMDriver {
  /**
  * @brief   System Clock Prescaler
  */
  uint8_t noofchannels;
  /**
   * @brief   System Clock Prescaler
  */
  uint32_t prescaler;
  /**
   * @brief   Current configuration data.
   */
  STM_CH_Config *stm_ch_config;
  /**
   * @brief Pointer to the STM registers block.
   */
  volatile struct spc5_stm  *stm_tagp;
  /**
   * @brief counter status flag. running (ENBALED) or not (DISABLED)
   */
  uint8_t status;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/


/*===========================================================================*/
/* IRQ Handlers                                                              */
/*===========================================================================*/

#if (SPC5_STM_USE_STM0  == TRUE)   || defined(__DOXYGEN__)
#if (SPC5_STM0_CH0_ENABLED == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_STM0_CH0_HANDLER);
#endif
#if (SPC5_STM0_CH1_CH3_SHARED_INT == TRUE) || defined(__DOXYGEN__)
#if (SPC5_STM0_CH1_ENABLED == TRUE) || (SPC5_STM0_CH2_ENABLED == TRUE) || \
    (SPC5_STM0_CH3_ENABLED == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_STM0_CH1_CH3_HANDLER);
#endif
#else
#if (SPC5_STM0_CH1_ENABLED == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_STM0_CH1_HANDLER);
#endif
#if (SPC5_STM0_CH2_ENABLED == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_STM0_CH2_HANDLER);
#endif
#if (SPC5_STM0_CH3_ENABLED == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_STM0_CH3_HANDLER);
#endif
#endif
#endif

#if (SPC5_STM_USE_STM1  == TRUE)   || defined(__DOXYGEN__)
#if (SPC5_STM1_CH0_ENABLED == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_STM1_CH0_HANDLER);
#endif
#if (SPC5_STM1_CH1_ENABLED == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_STM1_CH1_HANDLER);
#endif
#if (SPC5_STM1_CH2_ENABLED == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_STM1_CH2_HANDLER);
#endif
#if (SPC5_STM1_CH3_ENABLED == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_STM1_CH3_HANDLER);
#endif
#endif

#if (SPC5_STM_USE_STM2  == TRUE)   || defined(__DOXYGEN__)
#if (SPC5_STM2_CH0_ENABLED == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_STM2_CH0_HANDLER);
#endif
#if (SPC5_STM2_CH1_ENABLED == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_STM2_CH1_HANDLER);
#endif
#if (SPC5_STM2_CH2_ENABLED == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_STM2_CH2_HANDLER);
#endif
#if (SPC5_STM2_CH3_ENABLED == TRUE) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_STM2_CH3_HANDLER);
#endif
#endif

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if (SPC5_STM_USE_STM0 == TRUE) || defined(__DOXYGEN__)
extern STMDriver STMD1;
#endif
#if (SPC5_STM_USE_STM1 == TRUE) || defined(__DOXYGEN__)
extern STMDriver STMD2;
#endif
#if (SPC5_STM_USE_STM2 == TRUE) || defined(__DOXYGEN__)
extern STMDriver STMD3;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void     stm_lld_init          (void);
  void     stm_lld_start         (STMDriver *stmd, STM_CH_Config *config);
  void     stm_lld_enable        (STMDriver *stmd);
  void     stm_lld_disable       (STMDriver *stmd);
  void     stm_lld_stop			 (STMDriver *stmd);
  void     stm_lld_resetcounter  (STMDriver *stmd);
  uint32_t stm_lld_getcounter    (STMDriver *stmd);
  void     stm_lld_setcounter    (STMDriver *stmd,uint32_t value);
#ifdef __cplusplus
}
#endif

#endif /* LLD_USE_STM */

#endif /* _STM_LLD_H_ */

/** @} */
