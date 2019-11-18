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
* EVALUATION ONLY – NOT FOR USE IN PRODUCTION
*****************************************************************************/
/**
 * @file    icu_lld.h
 * @brief   SPC5xx ICU low level driver header.
 *
 * @addtogroup ICU
 * @{
 */

#ifndef _ICU_LLD_H_
#define _ICU_LLD_H_

#include "spc5_lld.h"
#include "lldconf.h"

#if (LLD_USE_ICU == TRUE) || defined(__DOXYGEN__)

#include "spc5_etimer.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Mode options
 * @{
 */

/**
 * @brief   Skip first capture cycle.
 * @details If set to @p TRUE the first capture cycle is skipped.
 * @note    The default is @p FALSE.
 */
#if !defined(ICU_JUMP_FIRST_CAPTURE) || defined(__DOXYGEN__)
#define ICU_SKIP_FIRST_CAPTURE              FALSE
#endif
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   ICUD1 driver enable switch.
 * @details If set to @p TRUE the support for ICUD1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_ICU_USE_ETIMER0_CH0) || defined(__DOXYGEN__)
#define SPC5_ICU_USE_ETIMER0_CH0            FALSE
#endif

/**
 * @brief   ICUD2 driver enable switch.
 * @details If set to @p TRUE the support for ICUD2 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_ICU_USE_ETIMER0_CH1) || defined(__DOXYGEN__)
#define SPC5_ICU_USE_ETIMER0_CH1            FALSE
#endif

/**
 * @brief   ICUD3 driver enable switch.
 * @details If set to @p TRUE the support for ICUD3 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_ICU_USE_ETIMER0_CH2) || defined(__DOXYGEN__)
#define SPC5_ICU_USE_ETIMER0_CH2            FALSE
#endif

/**
 * @brief   ICUD4 driver enable switch.
 * @details If set to @p TRUE the support for ICUD4 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_ICU_USE_ETIMER0_CH3) || defined(__DOXYGEN__)
#define SPC5_ICU_USE_ETIMER0_CH3            FALSE
#endif

/**
 * @brief   ICUD5 driver enable switch.
 * @details If set to @p TRUE the support for ICUD5 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_ICU_USE_ETIMER0_CH4) || defined(__DOXYGEN__)
#define SPC5_ICU_USE_ETIMER0_CH4            FALSE
#endif

/**
 * @brief   ICUD6 driver enable switch.
 * @details If set to @p TRUE the support for ICUD6 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_ICU_USE_ETIMER0_CH5) || defined(__DOXYGEN__)
#define SPC5_ICU_USE_ETIMER0_CH5            FALSE
#endif

/**
 * @brief   ICUD7 driver enable switch.
 * @details If set to @p TRUE the support for ICUD7 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_ICU_USE_ETIMER1_CH0) || defined(__DOXYGEN__)
#define SPC5_ICU_USE_ETIMER1_CH0            FALSE
#endif

/**
 * @brief   ICUD8 driver enable switch.
 * @details If set to @p TRUE the support for ICUD8 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_ICU_USE_ETIMER1_CH1) || defined(__DOXYGEN__)
#define SPC5_ICU_USE_ETIMER1_CH1            FALSE
#endif

/**
 * @brief   ICUD9 driver enable switch.
 * @details If set to @p TRUE the support for ICUD9 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_ICU_USE_ETIMER1_CH2) || defined(__DOXYGEN__)
#define SPC5_ICU_USE_ETIMER1_CH2            FALSE
#endif

/**
 * @brief   ICUD10 driver enable switch.
 * @details If set to @p TRUE the support for ICUD10 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_ICU_USE_ETIMER1_CH3) || defined(__DOXYGEN__)
#define SPC5_ICU_USE_ETIMER1_CH3            FALSE
#endif

/**
 * @brief   ICUD11 driver enable switch.
 * @details If set to @p TRUE the support for ICUD11 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_ICU_USE_ETIMER1_CH4) || defined(__DOXYGEN__)
#define SPC5_ICU_USE_ETIMER1_CH4            FALSE
#endif

/**
 * @brief   ICUD12 driver enable switch.
 * @details If set to @p TRUE the support for ICUD12 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_ICU_USE_ETIMER1_CH5) || defined(__DOXYGEN__)
#define SPC5_ICU_USE_ETIMER1_CH5            FALSE
#endif

/**
 * @brief   ICUD13 driver enable switch.
 * @details If set to @p TRUE the support for ICUD13 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_ICU_USE_ETIMER2_CH0) || defined(__DOXYGEN__)
#define SPC5_ICU_USE_ETIMER2_CH0            FALSE
#endif

/**
 * @brief   ICUD14 driver enable switch.
 * @details If set to @p TRUE the support for ICUD14 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_ICU_USE_ETIMER2_CH1) || defined(__DOXYGEN__)
#define SPC5_ICU_USE_ETIMER2_CH1            FALSE
#endif

/**
 * @brief   ICUD15 driver enable switch.
 * @details If set to @p TRUE the support for ICUD15 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_ICU_USE_ETIMER2_CH2) || defined(__DOXYGEN__)
#define SPC5_ICU_USE_ETIMER2_CH2            FALSE
#endif

/**
 * @brief   ICUD16 driver enable switch.
 * @details If set to @p TRUE the support for ICUD16 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_ICU_USE_ETIMER2_CH3) || defined(__DOXYGEN__)
#define SPC5_ICU_USE_ETIMER2_CH3            FALSE
#endif

/**
 * @brief   ICUD17 driver enable switch.
 * @details If set to @p TRUE the support for ICUD17 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_ICU_USE_ETIMER2_CH4) || defined(__DOXYGEN__)
#define SPC5_ICU_USE_ETIMER2_CH4            FALSE
#endif

/**
 * @brief   ICUD18 driver enable switch.
 * @details If set to @p TRUE the support for ICUD18 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_ICU_USE_ETIMER2_CH5) || defined(__DOXYGEN__)
#define SPC5_ICU_USE_ETIMER2_CH5            FALSE
#endif

/**
 * @brief   ICUD19 driver enable switch.
 * @details If set to @p TRUE the support for ICUD19 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_ICU_USE_ETIMER3_CH0) || defined(__DOXYGEN__)
#define SPC5_ICU_USE_ETIMER3_CH0            FALSE
#endif

/**
 * @brief   ICUD20 driver enable switch.
 * @details If set to @p TRUE the support for ICUD20 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_ICU_USE_ETIMER3_CH1) || defined(__DOXYGEN__)
#define SPC5_ICU_USE_ETIMER3_CH1            FALSE
#endif

/**
 * @brief   ICUD21 driver enable switch.
 * @details If set to @p TRUE the support for ICUD21 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_ICU_USE_ETIMER3_CH2) || defined(__DOXYGEN__)
#define SPC5_ICU_USE_ETIMER3_CH2            FALSE
#endif

/**
 * @brief   ICUD22 driver enable switch.
 * @details If set to @p TRUE the support for ICUD22 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_ICU_USE_ETIMER3_CH3) || defined(__DOXYGEN__)
#define SPC5_ICU_USE_ETIMER3_CH3            FALSE
#endif

/**
 * @brief   ICUD23 driver enable switch.
 * @details If set to @p TRUE the support for ICUD23 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_ICU_USE_ETIMER3_CH4) || defined(__DOXYGEN__)
#define SPC5_ICU_USE_ETIMER3_CH4            FALSE
#endif

/**
 * @brief   ICUD24 driver enable switch.
 * @details If set to @p TRUE the support for ICUD24 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_ICU_USE_ETIMER3_CH5) || defined(__DOXYGEN__)
#define SPC5_ICU_USE_ETIMER3_CH5            FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (SPC5_ICU_USE_ETIMER0_CH0 == TRUE || SPC5_ICU_USE_ETIMER0_CH1 == TRUE || \
     SPC5_ICU_USE_ETIMER0_CH2 == TRUE || SPC5_ICU_USE_ETIMER0_CH3 == TRUE || \
     SPC5_ICU_USE_ETIMER0_CH4 == TRUE || SPC5_ICU_USE_ETIMER0_CH5 == TRUE)
#define SPC5_ICU_USE_ETIMER0                TRUE
#else
#define SPC5_ICU_USE_ETIMER0                FALSE
#endif

#if (SPC5_ICU_USE_ETIMER1_CH0 == TRUE || SPC5_ICU_USE_ETIMER1_CH1 == TRUE || \
     SPC5_ICU_USE_ETIMER1_CH2 == TRUE || SPC5_ICU_USE_ETIMER1_CH3 == TRUE || \
     SPC5_ICU_USE_ETIMER1_CH4 == TRUE || SPC5_ICU_USE_ETIMER1_CH5 == TRUE)
#define SPC5_ICU_USE_ETIMER1                TRUE
#else
#define SPC5_ICU_USE_ETIMER1                FALSE
#endif

#if (SPC5_ICU_USE_ETIMER2_CH0 == TRUE || SPC5_ICU_USE_ETIMER2_CH1 == TRUE || \
     SPC5_ICU_USE_ETIMER2_CH2 == TRUE || SPC5_ICU_USE_ETIMER2_CH3 == TRUE || \
     SPC5_ICU_USE_ETIMER2_CH4 == TRUE || SPC5_ICU_USE_ETIMER2_CH5 == TRUE)
#define SPC5_ICU_USE_ETIMER2                TRUE
#else
#define SPC5_ICU_USE_ETIMER2                FALSE
#endif

#if (SPC5_ICU_USE_ETIMER3_CH0 == TRUE || SPC5_ICU_USE_ETIMER3_CH1 == TRUE || \
     SPC5_ICU_USE_ETIMER3_CH2 == TRUE || SPC5_ICU_USE_ETIMER3_CH3 == TRUE || \
     SPC5_ICU_USE_ETIMER3_CH4 == TRUE || SPC5_ICU_USE_ETIMER3_CH5 == TRUE)
#define SPC5_ICU_USE_ETIMER3                TRUE
#else
#define SPC5_ICU_USE_ETIMER3                FALSE
#endif

#if (SPC5_HAS_ETIMER0 == FALSE) && (SPC5_ICU_USE_ETIMER0 == TRUE)
#error "ETIMER0 not present in the selected device"
#endif

#if (SPC5_HAS_ETIMER1 == FALSE) && (SPC5_ICU_USE_ETIMER1 == TRUE)
#error "ETIMER1 not present in the selected device"
#endif

#if (SPC5_HAS_ETIMER2 == FALSE) && (SPC5_ICU_USE_ETIMER2 == TRUE)
#error "ETIMER2 not present in the selected device"
#endif

#if (SPC5_HAS_ETIMER3 == FALSE) && (SPC5_ICU_USE_ETIMER3 == TRUE)
#error "ETIMER3 not present in the selected device"
#endif

#if (SPC5_ICU_USE_ETIMER0 == FALSE) && (SPC5_ICU_USE_ETIMER1 == FALSE) && \
    (SPC5_ICU_USE_ETIMER2 == FALSE) && (SPC5_ICU_USE_ETIMER3 == FALSE)
#error "ICU driver activated but no Channels assigned"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a structure representing an ICU driver.
 */
typedef struct ICUDriver ICUDriver;

/**
 * @brief   ICU notification callback type.
 *
 * @param[in] icup      pointer to a @p ICUDriver object
 */
typedef void (*icucallback_t)(ICUDriver *icup);

/**
 * @brief ICU driver mode.
 */
typedef enum {
  ICU_INPUT_ACTIVE_HIGH = 0, /**< Trigger on rising edge.            */
  ICU_INPUT_ACTIVE_LOW = 1   /**< Trigger on falling edge.           */
} icumode_t;

/**
 * @brief   ICU prescaler type.
 */
typedef uint8_t  icupsc_t;

/**
 * @brief   ICU channel type.
 */
typedef uint8_t icuchannel_t;

/**
 * @brief   ICU counter type.
 */
typedef uint16_t icucnt_t;

/**
 * @brief   Driver configuration structure.
 * @note    It could be empty on some architectures.
 */
typedef struct {
  /**
   * @brief   Counter prescaler.
   */
  icupsc_t                  psc;
  /**
   * @brief   Driver mode.
   */
  icumode_t                 mode;
  /**
   * @brief   Callback for pulse width measurement.
   */
  icucallback_t             width_callback;
  /**
   * @brief   Callback for cycle period measurement.
   */
  icucallback_t             period_callback;
  /**
   * @brief   Callback for timer overflow.
   */
  icucallback_t             overflow_callback;
} ICUConfig;

/**
 * @brief   Structure representing an ICU driver.
 */
struct ICUDriver {
  /**
   * @brief eTimer channel number.
   */
  icuchannel_t              channel;
  /**
   * @brief Current configuration data.
   */
  const ICUConfig           *config;
  /**
   * @brief Pointer to the eTimerx registers block.
   */
  volatile struct spc5_etimer *etimerp;
  /**
   * @brief CCR register used for width capture.
   */
  volatile vuint16_t        *wccrp;
  /**
   * @brief CCR register used for period capture.
   */
  volatile vuint16_t        *pccrp;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* IRQ Handlers                                                              */
/*===========================================================================*/

#if (SPC5_ICU_USE_ETIMER0_CH0 == TRUE)
IRQ_HANDLER(SPC5_ETIMER0_STS0_HANDLER);
#endif /* SPC5_ICU_USE_ETIMER0_CH0 */

#if (SPC5_ICU_USE_ETIMER0_CH1 == TRUE)
IRQ_HANDLER(SPC5_ETIMER0_STS1_HANDLER);
#endif /* SPC5_ICU_USE_ETIMER0_CH1 */

#if (SPC5_ICU_USE_ETIMER0_CH2 == TRUE)
IRQ_HANDLER(SPC5_ETIMER0_STS2_HANDLER);
#endif /* SPC5_ICU_USE_ETIMER0_CH2 */

#if (SPC5_ICU_USE_ETIMER0_CH3 == TRUE)
IRQ_HANDLER(SPC5_ETIMER0_STS3_HANDLER);
#endif /* SPC5_ICU_USE_ETIMER0_CH3 */

#if (SPC5_ICU_USE_ETIMER0_CH4 == TRUE)
IRQ_HANDLER(SPC5_ETIMER0_STS4_HANDLER);
#endif /* SPC5_ICU_USE_ETIMER0_CH4 */

#if (SPC5_ICU_USE_ETIMER0_CH5 == TRUE)
IRQ_HANDLER(SPC5_ETIMER0_STS5_HANDLER);
#endif /* SPC5_ICU_USE_ETIMER0_CH5 */

#if (SPC5_ICU_USE_ETIMER1_CH0 == TRUE)
IRQ_HANDLER(SPC5_ETIMER1_STS0_HANDLER);
#endif /* SPC5_ICU_USE_ETIMER1_CH0 */

#if (SPC5_ICU_USE_ETIMER1_CH1 == TRUE)
IRQ_HANDLER(SPC5_ETIMER1_STS1_HANDLER);
#endif /* SPC5_ICU_USE_ETIMER1_CH1 */

#if (SPC5_ICU_USE_ETIMER1_CH2 == TRUE)
IRQ_HANDLER(SPC5_ETIMER1_STS2_HANDLER);
#endif /* SPC5_ICU_USE_ETIMER1_CH2 */

#if (SPC5_ICU_USE_ETIMER1_CH3 == TRUE)
IRQ_HANDLER(SPC5_ETIMER1_STS3_HANDLER);
#endif /* SPC5_ICU_USE_ETIMER1_CH3 */

#if (SPC5_ICU_USE_ETIMER1_CH4 == TRUE)
IRQ_HANDLER(SPC5_ETIMER1_STS4_HANDLER);
#endif /* SPC5_ICU_USE_ETIMER1_CH4 */

#if (SPC5_ICU_USE_ETIMER1_CH5 == TRUE)
IRQ_HANDLER(SPC5_ETIMER1_STS5_HANDLER);
#endif /* SPC5_ICU_USE_ETIMER1_CH5 */

#if (SPC5_ICU_USE_ETIMER2_CH0 == TRUE)
IRQ_HANDLER(SPC5_ETIMER2_STS0_HANDLER);
#endif /* SPC5_ICU_USE_ETIMER2_CH0 */

#if (SPC5_ICU_USE_ETIMER2_CH1 == TRUE)
IRQ_HANDLER(SPC5_ETIMER2_STS1_HANDLER);
#endif /* SPC5_ICU_USE_ETIMER2_CH1 */

#if (SPC5_ICU_USE_ETIMER2_CH2 == TRUE)
IRQ_HANDLER(SPC5_ETIMER2_STS2_HANDLER);
#endif /* SPC5_ICU_USE_ETIMER2_CH2 */

#if (SPC5_ICU_USE_ETIMER2_CH3 == TRUE)
IRQ_HANDLER(SPC5_ETIMER2_STS3_HANDLER);
#endif /* SPC5_ICU_USE_ETIMER2_CH3 */

#if (SPC5_ICU_USE_ETIMER2_CH4 == TRUE)
IRQ_HANDLER(SPC5_ETIMER2_STS4_HANDLER);
#endif /* SPC5_ICU_USE_ETIMER2_CH4 */

#if (SPC5_ICU_USE_ETIMER2_CH5 == TRUE)
IRQ_HANDLER(SPC5_ETIMER2_STS5_HANDLER);
#endif /* SPC5_ICU_USE_ETIMER2_CH5 */

#if (SPC5_ICU_USE_ETIMER3_CH0 == TRUE)
IRQ_HANDLER(SPC5_ETIMER3_STS0_HANDLER);
#endif /* SPC5_ICU_USE_ETIMER3_CH0 */

#if (SPC5_ICU_USE_ETIMER3_CH1 == TRUE)
IRQ_HANDLER(SPC5_ETIMER3_STS1_HANDLER);
#endif /* SPC5_ICU_USE_ETIMER3_CH1 */

#if (SPC5_ICU_USE_ETIMER3_CH2 == TRUE)
IRQ_HANDLER(SPC5_ETIMER3_STS2_HANDLER);
#endif /* SPC5_ICU_USE_ETIMER3_CH2 */

#if (SPC5_ICU_USE_ETIMER3_CH3 == TRUE)
IRQ_HANDLER(SPC5_ETIMER3_STS3_HANDLER);
#endif /* SPC5_ICU_USE_ETIMER3_CH3 */

#if (SPC5_ICU_USE_ETIMER3_CH4 == TRUE)
IRQ_HANDLER(SPC5_ETIMER3_STS4_HANDLER);
#endif /* SPC5_ICU_USE_ETIMER3_CH4 */

#if (SPC5_ICU_USE_ETIMER3_CH5 == TRUE)
IRQ_HANDLER(SPC5_ETIMER3_STS5_HANDLER);
#endif /* SPC5_ICU_USE_ETIMER3_CH5 */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/**
 * @brief   ICU drivers.
 */
#if (SPC5_ICU_USE_ETIMER0_CH0 == TRUE) && !defined(__DOXYGEN__)
extern ICUDriver ICUD1;
#endif

#if (SPC5_ICU_USE_ETIMER0_CH1 == TRUE) && !defined(__DOXYGEN__)
extern ICUDriver ICUD2;
#endif

#if (SPC5_ICU_USE_ETIMER0_CH2 == TRUE) && !defined(__DOXYGEN__)
extern ICUDriver ICUD3;
#endif

#if (SPC5_ICU_USE_ETIMER0_CH3 == TRUE) && !defined(__DOXYGEN__)
extern ICUDriver ICUD4;
#endif

#if (SPC5_ICU_USE_ETIMER0_CH4 == TRUE) && !defined(__DOXYGEN__)
extern ICUDriver ICUD5;
#endif

#if (SPC5_ICU_USE_ETIMER0_CH5 == TRUE) && !defined(__DOXYGEN__)
extern ICUDriver ICUD6;
#endif

#if (SPC5_ICU_USE_ETIMER1_CH0 == TRUE) && !defined(__DOXYGEN__)
extern ICUDriver ICUD7;
#endif

#if (SPC5_ICU_USE_ETIMER1_CH1 == TRUE) && !defined(__DOXYGEN__)
extern ICUDriver ICUD8;
#endif

#if (SPC5_ICU_USE_ETIMER1_CH2 == TRUE) && !defined(__DOXYGEN__)
extern ICUDriver ICUD9;
#endif

#if (SPC5_ICU_USE_ETIMER1_CH3 == TRUE) && !defined(__DOXYGEN__)
extern ICUDriver ICUD10;
#endif

#if (SPC5_ICU_USE_ETIMER1_CH4 == TRUE) && !defined(__DOXYGEN__)
extern ICUDriver ICUD11;
#endif

#if (SPC5_ICU_USE_ETIMER1_CH5 == TRUE) && !defined(__DOXYGEN__)
extern ICUDriver ICUD12;
#endif

#if (SPC5_ICU_USE_ETIMER2_CH0 == TRUE) && !defined(__DOXYGEN__)
extern ICUDriver ICUD13;
#endif

#if (SPC5_ICU_USE_ETIMER2_CH1 == TRUE) && !defined(__DOXYGEN__)
extern ICUDriver ICUD14;
#endif

#if (SPC5_ICU_USE_ETIMER2_CH2 == TRUE) && !defined(__DOXYGEN__)
extern ICUDriver ICUD15;
#endif

#if (SPC5_ICU_USE_ETIMER2_CH3 == TRUE) && !defined(__DOXYGEN__)
extern ICUDriver ICUD16;
#endif

#if (SPC5_ICU_USE_ETIMER2_CH4 == TRUE) && !defined(__DOXYGEN__)
extern ICUDriver ICUD17;
#endif

#if (SPC5_ICU_USE_ETIMER2_CH5 == TRUE) && !defined(__DOXYGEN__)
extern ICUDriver ICUD18;
#endif

#if (SPC5_ICU_USE_ETIMER3_CH0 == TRUE) && !defined(__DOXYGEN__)
extern ICUDriver ICUD19;
#endif

#if (SPC5_ICU_USE_ETIMER3_CH1 == TRUE) && !defined(__DOXYGEN__)
extern ICUDriver ICUD20;
#endif

#if (SPC5_ICU_USE_ETIMER3_CH2 == TRUE) && !defined(__DOXYGEN__)
extern ICUDriver ICUD21;
#endif

#if (SPC5_ICU_USE_ETIMER3_CH3 == TRUE) && !defined(__DOXYGEN__)
extern ICUDriver ICUD22;
#endif

#if (SPC5_ICU_USE_ETIMER3_CH4 == TRUE) && !defined(__DOXYGEN__)
extern ICUDriver ICUD23;
#endif

#if (SPC5_ICU_USE_ETIMER3_CH5 == TRUE) && !defined(__DOXYGEN__)
extern ICUDriver ICUD24;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void icu_lld_init(void);
  void icu_lld_start(ICUDriver *icup, const ICUConfig *config);
  void icu_lld_stop(ICUDriver *icup);
  void icu_lld_enable(ICUDriver *icup);
  void icu_lld_disable(ICUDriver *icup);
#ifdef __cplusplus
}
#endif

#endif /* LLD_USE_ICU */

#endif /* _ICU_LLD_H_ */

/** @} */
