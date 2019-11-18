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
 * @file    pwm_lld.h
 * @brief   SPC5xx PWM low level driver header.
 *
 * @addtogroup PWM
 * @{
 */

#ifndef _PWM_LLD_H_
#define _PWM_LLD_H_

#include "spc5_lld.h"
#include "lldconf.h"

#if (LLD_USE_PWM == TRUE) || defined(__DOXYGEN__)

#include "spc5_etimer.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    PWM output mode macros
 * @{
 */
/**
 * @brief   Positive PWM logic, active is logic level one.
 */
#define PWM_OUTPUT_ACTIVE_HIGH              0x1U

/**
 * @brief   Inverse PWM logic, active is logic level zero.
 */
#define PWM_OUTPUT_ACTIVE_LOW               0x2U
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   PWMD1 driver enable switch.
 * @details If set to @p TRUE the support for PWMD1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_ETIMER0_CH0) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_ETIMER0_CH0            FALSE
#endif

/**
 * @brief   PWMD2 driver enable switch.
 * @details If set to @p TRUE the support for PWMD2 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_ETIMER0_CH1) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_ETIMER0_CH1            FALSE
#endif

/**
 * @brief   PWMD3 driver enable switch.
 * @details If set to @p TRUE the support for PWMD3 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_ETIMER0_CH2) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_ETIMER0_CH2            FALSE
#endif

/**
 * @brief   PWMD4 driver enable switch.
 * @details If set to @p TRUE the support for PWMD4 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_ETIMER0_CH3) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_ETIMER0_CH3            FALSE
#endif

/**
 * @brief   PWMD5 driver enable switch.
 * @details If set to @p TRUE the support for PWMD5 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_ETIMER0_CH4) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_ETIMER0_CH4            FALSE
#endif

/**
 * @brief   PWMD6 driver enable switch.
 * @details If set to @p TRUE the support for PWMD6 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_ETIMER0_CH5) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_ETIMER0_CH5            FALSE
#endif

/**
 * @brief   PWMD7 driver enable switch.
 * @details If set to @p TRUE the support for PWMD7 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_ETIMER1_CH0) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_ETIMER1_CH0            FALSE
#endif

/**
 * @brief   PWMD8 driver enable switch.
 * @details If set to @p TRUE the support for PWMD8 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_ETIMER1_CH1) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_ETIMER1_CH1            FALSE
#endif

/**
 * @brief   PWMD9 driver enable switch.
 * @details If set to @p TRUE the support for PWMD9 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_ETIMER1_CH2) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_ETIMER1_CH2            FALSE
#endif

/**
 * @brief   PWMD10 driver enable switch.
 * @details If set to @p TRUE the support for PWMD10 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_ETIMER1_CH3) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_ETIMER1_CH3            FALSE
#endif

/**
 * @brief   PWMD11 driver enable switch.
 * @details If set to @p TRUE the support for PWMD11 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_ETIMER1_CH4) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_ETIMER1_CH4            FALSE
#endif

/**
 * @brief   PWMD12 driver enable switch.
 * @details If set to @p TRUE the support for PWMD12 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_ETIMER1_CH5) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_ETIMER1_CH5            FALSE
#endif

/**
 * @brief   PWMD13 driver enable switch.
 * @details If set to @p TRUE the support for PWMD13 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_ETIMER2_CH0) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_ETIMER2_CH0            FALSE
#endif

/**
 * @brief   PWMD14 driver enable switch.
 * @details If set to @p TRUE the support for PWMD14 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_ETIMER2_CH1) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_ETIMER2_CH1            FALSE
#endif

/**
 * @brief   PWMD15 driver enable switch.
 * @details If set to @p TRUE the support for PWMD15 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_ETIMER2_CH2) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_ETIMER2_CH2            FALSE
#endif

/**
 * @brief   PWMD16 driver enable switch.
 * @details If set to @p TRUE the support for PWMD16 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_ETIMER2_CH3) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_ETIMER2_CH3            FALSE
#endif

/**
 * @brief   PWMD17 driver enable switch.
 * @details If set to @p TRUE the support for PWMD17 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_ETIMER2_CH4) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_ETIMER2_CH4            FALSE
#endif

/**
 * @brief   PWMD18 driver enable switch.
 * @details If set to @p TRUE the support for PWMD18 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_ETIMER2_CH5) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_ETIMER2_CH5            FALSE
#endif

/**
 * @brief   PWMD19 driver enable switch.
 * @details If set to @p TRUE the support for PWMD19 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_ETIMER3_CH0) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_ETIMER3_CH0            FALSE
#endif

/**
 * @brief   PWMD20 driver enable switch.
 * @details If set to @p TRUE the support for PWMD20 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_ETIMER3_CH1) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_ETIMER3_CH1            FALSE
#endif

/**
 * @brief   PWMD21 driver enable switch.
 * @details If set to @p TRUE the support for PWMD21 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_ETIMER3_CH2) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_ETIMER3_CH2            FALSE
#endif

/**
 * @brief   PWMD22 driver enable switch.
 * @details If set to @p TRUE the support for PWMD22 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_ETIMER3_CH3) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_ETIMER3_CH3            FALSE
#endif

/**
 * @brief   PWMD23 driver enable switch.
 * @details If set to @p TRUE the support for PWMD23 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_ETIMER3_CH4) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_ETIMER3_CH4            FALSE
#endif

/**
 * @brief   PWMD24 driver enable switch.
 * @details If set to @p TRUE the support for PWMD24 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_PWM_USE_ETIMER3_CH5) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_ETIMER3_CH5            FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (SPC5_PWM_USE_ETIMER0_CH0 == TRUE || SPC5_PWM_USE_ETIMER0_CH1 == TRUE || \
     SPC5_PWM_USE_ETIMER0_CH2 == TRUE || SPC5_PWM_USE_ETIMER0_CH3 == TRUE || \
     SPC5_PWM_USE_ETIMER0_CH4 == TRUE || SPC5_PWM_USE_ETIMER0_CH5 == TRUE)
#define SPC5_PWM_USE_ETIMER0                TRUE
#else
#define SPC5_PWM_USE_ETIMER0                FALSE
#endif

#if (SPC5_PWM_USE_ETIMER1_CH0 == TRUE || SPC5_PWM_USE_ETIMER1_CH1 == TRUE || \
     SPC5_PWM_USE_ETIMER1_CH2 == TRUE || SPC5_PWM_USE_ETIMER1_CH3 == TRUE || \
     SPC5_PWM_USE_ETIMER1_CH4 == TRUE || SPC5_PWM_USE_ETIMER1_CH5 == TRUE)
#define SPC5_PWM_USE_ETIMER1                TRUE
#else
#define SPC5_PWM_USE_ETIMER1                FALSE
#endif

#if (SPC5_PWM_USE_ETIMER2_CH0 == TRUE || SPC5_PWM_USE_ETIMER2_CH1 == TRUE || \
     SPC5_PWM_USE_ETIMER2_CH2 == TRUE || SPC5_PWM_USE_ETIMER2_CH3 == TRUE || \
     SPC5_PWM_USE_ETIMER2_CH4 == TRUE || SPC5_PWM_USE_ETIMER2_CH5 == TRUE)
#define SPC5_PWM_USE_ETIMER2                TRUE
#else
#define SPC5_PWM_USE_ETIMER2                FALSE
#endif

#if (SPC5_PWM_USE_ETIMER3_CH0 == TRUE || SPC5_PWM_USE_ETIMER3_CH1 == TRUE || \
     SPC5_PWM_USE_ETIMER3_CH2 == TRUE || SPC5_PWM_USE_ETIMER3_CH3 == TRUE || \
     SPC5_PWM_USE_ETIMER3_CH4 == TRUE || SPC5_PWM_USE_ETIMER3_CH5 == TRUE)
#define SPC5_PWM_USE_ETIMER3                TRUE
#else
#define SPC5_PWM_USE_ETIMER3                FALSE
#endif

#if (SPC5_HAS_ETIMER0 == FALSE) && (SPC5_PWM_USE_ETIMER0 == TRUE)
#error "ETIMER0 not present in the selected device"
#endif

#if (SPC5_HAS_ETIMER1 == FALSE) && (SPC5_PWM_USE_ETIMER1 == TRUE)
#error "ETIMER1 not present in the selected device"
#endif

#if (SPC5_HAS_ETIMER2 == FALSE) && (SPC5_PWM_USE_ETIMER2 == TRUE)
#error "ETIMER2 not present in the selected device"
#endif

#if (SPC5_HAS_ETIMER3 == FALSE) && (SPC5_PWM_USE_ETIMER3 == TRUE)
#error "ETIMER3 not present in the selected device"
#endif

#if (SPC5_PWM_USE_ETIMER0 == FALSE) && (SPC5_PWM_USE_ETIMER1 == FALSE) && \
    (SPC5_PWM_USE_ETIMER2 == FALSE) && (SPC5_PWM_USE_ETIMER3 == FALSE)
#error "PWM driver activated but no Channels assigned"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a structure representing a PWM driver.
 */
typedef struct PWMDriver PWMDriver;

/**
 * @brief   PWM notification callback type.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 */
typedef void (*pwmcallback_t)(PWMDriver *pwmp);

/**
 * @brief   PWM output mode type.
 */
typedef uint8_t pwmoutmode_t;

/**
 * @brief   PWM prescaler type.
 */
typedef uint8_t  pwmpsc_t;

/**
 * @brief   PWM channel type.
 */
typedef uint8_t pwmchannel_t;

/**
 * @brief   PWM counter type.
 */
typedef uint16_t pwmcnt_t;

/**
 * @brief   Channel configuration structure.
 */
typedef struct {
  /**
   * @brief Channel active logic level.
   */
  pwmoutmode_t              mode;
  /**
   * @brief Channel callback pointer.
   * @note  This callback is invoked on the channel compare event. If set to
   *        @p NULL then the callback is disabled.
   */
  pwmcallback_t             channel_callback;

} PWMChannelConfig;

/**
 * @brief   Driver configuration structure.
 */
typedef struct {
  /**
   * @brief Counter prescaler.
   */
  pwmpsc_t                  psc;
  /**
   * @brief Period in ticks.
   */
  pwmcnt_t                  period;
  /**
   * @brief Periodic callback pointer.
   * @note  This callback is invoked on PWM counter reset. If set to
   *        @p NULL then the callback is disabled.
   */
  pwmcallback_t             period_callback;
  /**
   * @brief Current configuration data.
   */
  PWMChannelConfig          *ch_config;
} PWMConfig;

/**
 * @brief   Structure representing an PWM driver.
 * @note    Implementations may extend this structure to contain more,
 *          architecture dependent, fields.
 */
struct PWMDriver {
  /**
   * @brief eTimer channel number.
   */
  pwmchannel_t              channel;
  /**
   * @brief Current PWM period in ticks.
   */
  pwmcnt_t                  period;
  /**
   * @brief Current PWM width in ticks.
   */
  pwmcnt_t                  width;
  /**
   * @brief Current configuration data.
   */
  const PWMConfig           *config;
  /**
   * @brief Pointer to the eTimerx registers block.
   */
  volatile struct spc5_etimer *etimerp;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    PWM duty cycle conversion
 * @{
 */
/**
 * @brief   Converts from period fraction to counter ticks.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] denominator denominator of the fraction
 * @param[in] numerator numerator of the fraction
 * @return              The counter ticks as fraction of the period.
 *
 * @api
 */
#define PWM_PERIOD_FRACTION(pwmp, denominator, numerator)                   \
  ((pwmcnt_t)((((pwmcnt_t)(pwmp)->period) *                                 \
               (pwmcnt_t)(numerator)) / (pwmcnt_t)(denominator)))

/**
 * @brief   Converts from degrees to pulse width.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] degrees   degrees as an integer between 0 and 36000
 * @return              The pulse width to be passed to @p pwm_lld_enable_channel().
 *
 * @api
 */
#define PWM_DEGREES_TO_WIDTH(pwmp, degrees)                                 \
  PWM_PERIOD_FRACTION(pwmp, 36000, degrees)

/**
 * @brief   Converts from percentage of the period to pulse width in ticks.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] percentage percentage as an integer between 0 and 10000
 * @return              The pulse width to be passed to @p pwm_lld_enable_channel().
 *
 * @api
 */
#define PWM_PERCENTAGE_TO_WIDTH(pwmp, percentage)                           \
  PWM_PERIOD_FRACTION(pwmp, 10000, percentage)
/** @} */

/*===========================================================================*/
/* IRQ Handlers                                                              */
/*===========================================================================*/

#if (SPC5_PWM_USE_ETIMER0_CH0 == TRUE)
IRQ_HANDLER(SPC5_ETIMER0_STS0_HANDLER);
#endif /* SPC5_PWM_USE_ETIMER0_CH0 */

#if (SPC5_PWM_USE_ETIMER0_CH1 == TRUE)
IRQ_HANDLER(SPC5_ETIMER0_STS1_HANDLER);
#endif /* SPC5_PWM_USE_ETIMER0_CH1 */

#if (SPC5_PWM_USE_ETIMER0_CH2 == TRUE)
IRQ_HANDLER(SPC5_ETIMER0_STS2_HANDLER);
#endif /* SPC5_PWM_USE_ETIMER0_CH2 */

#if (SPC5_PWM_USE_ETIMER0_CH3 == TRUE)
IRQ_HANDLER(SPC5_ETIMER0_STS3_HANDLER);
#endif /* SPC5_PWM_USE_ETIMER0_CH3 */

#if (SPC5_PWM_USE_ETIMER0_CH4 == TRUE)
IRQ_HANDLER(SPC5_ETIMER0_STS4_HANDLER);
#endif /* SPC5_PWM_USE_ETIMER0_CH4 */

#if (SPC5_PWM_USE_ETIMER0_CH5 == TRUE)
IRQ_HANDLER(SPC5_ETIMER0_STS5_HANDLER);
#endif /* SPC5_PWM_USE_ETIMER0_CH5 */

#if (SPC5_PWM_USE_ETIMER1_CH0 == TRUE)
IRQ_HANDLER(SPC5_ETIMER1_STS0_HANDLER);
#endif /* SPC5_PWM_USE_ETIMER1_CH0 */

#if (SPC5_PWM_USE_ETIMER1_CH1 == TRUE)
IRQ_HANDLER(SPC5_ETIMER1_STS1_HANDLER);
#endif /* SPC5_PWM_USE_ETIMER1_CH1 */

#if (SPC5_PWM_USE_ETIMER1_CH2 == TRUE)
IRQ_HANDLER(SPC5_ETIMER1_STS2_HANDLER);
#endif /* SPC5_PWM_USE_ETIMER1_CH2 */

#if (SPC5_PWM_USE_ETIMER1_CH3 == TRUE)
IRQ_HANDLER(SPC5_ETIMER1_STS3_HANDLER);
#endif /* SPC5_PWM_USE_ETIMER1_CH3 */

#if (SPC5_PWM_USE_ETIMER1_CH4 == TRUE)
IRQ_HANDLER(SPC5_ETIMER1_STS4_HANDLER);
#endif /* SPC5_PWM_USE_ETIMER1_CH4 */

#if (SPC5_PWM_USE_ETIMER1_CH5 == TRUE)
IRQ_HANDLER(SPC5_ETIMER1_STS5_HANDLER);
#endif /* SPC5_PWM_USE_ETIMER1_CH5 */

#if (SPC5_PWM_USE_ETIMER2_CH0 == TRUE)
IRQ_HANDLER(SPC5_ETIMER2_STS0_HANDLER);
#endif /* SPC5_PWM_USE_ETIMER2_CH0 */

#if (SPC5_PWM_USE_ETIMER2_CH1 == TRUE)
IRQ_HANDLER(SPC5_ETIMER2_STS1_HANDLER);
#endif /* SPC5_PWM_USE_ETIMER2_CH1 */

#if (SPC5_PWM_USE_ETIMER2_CH2 == TRUE)
IRQ_HANDLER(SPC5_ETIMER2_STS2_HANDLER);
#endif /* SPC5_PWM_USE_ETIMER2_CH2 */

#if (SPC5_PWM_USE_ETIMER2_CH3 == TRUE)
IRQ_HANDLER(SPC5_ETIMER2_STS3_HANDLER);
#endif /* SPC5_PWM_USE_ETIMER2_CH3 */

#if (SPC5_PWM_USE_ETIMER2_CH4 == TRUE)
IRQ_HANDLER(SPC5_ETIMER2_STS4_HANDLER);
#endif /* SPC5_PWM_USE_ETIMER2_CH4 */

#if (SPC5_PWM_USE_ETIMER2_CH5 == TRUE)
IRQ_HANDLER(SPC5_ETIMER2_STS5_HANDLER);
#endif /* SPC5_PWM_USE_ETIMER2_CH5 */

#if (SPC5_PWM_USE_ETIMER3_CH0 == TRUE)
IRQ_HANDLER(SPC5_ETIMER3_STS0_HANDLER);
#endif /* SPC5_PWM_USE_ETIMER3_CH0 */

#if (SPC5_PWM_USE_ETIMER3_CH1 == TRUE)
IRQ_HANDLER(SPC5_ETIMER3_STS1_HANDLER);
#endif /* SPC5_PWM_USE_ETIMER3_CH1 */

#if (SPC5_PWM_USE_ETIMER3_CH2 == TRUE)
IRQ_HANDLER(SPC5_ETIMER3_STS2_HANDLER);
#endif /* SPC5_PWM_USE_ETIMER3_CH2 */

#if (SPC5_PWM_USE_ETIMER3_CH3 == TRUE)
IRQ_HANDLER(SPC5_ETIMER3_STS3_HANDLER);
#endif /* SPC5_PWM_USE_ETIMER3_CH3 */

#if (SPC5_PWM_USE_ETIMER3_CH4 == TRUE)
IRQ_HANDLER(SPC5_ETIMER3_STS4_HANDLER);
#endif /* SPC5_PWM_USE_ETIMER3_CH4 */

#if (SPC5_PWM_USE_ETIMER3_CH5 == TRUE)
IRQ_HANDLER(SPC5_ETIMER3_STS5_HANDLER);
#endif /* SPC5_PWM_USE_ETIMER3_CH5 */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/**
 * @brief   PWM drivers.
 */
#if (SPC5_PWM_USE_ETIMER0_CH0 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD1;
#endif

#if (SPC5_PWM_USE_ETIMER0_CH1 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD2;
#endif

#if (SPC5_PWM_USE_ETIMER0_CH2 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD3;
#endif

#if (SPC5_PWM_USE_ETIMER0_CH3 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD4;
#endif

#if (SPC5_PWM_USE_ETIMER0_CH4 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD5;
#endif

#if (SPC5_PWM_USE_ETIMER0_CH5 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD6;
#endif

#if (SPC5_PWM_USE_ETIMER1_CH0 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD7;
#endif

#if (SPC5_PWM_USE_ETIMER1_CH1 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD8;
#endif

#if (SPC5_PWM_USE_ETIMER1_CH2 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD9;
#endif

#if (SPC5_PWM_USE_ETIMER1_CH3 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD10;
#endif

#if (SPC5_PWM_USE_ETIMER1_CH4 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD11;
#endif

#if (SPC5_PWM_USE_ETIMER1_CH5 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD12;
#endif

#if (SPC5_PWM_USE_ETIMER2_CH0 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD13;
#endif

#if (SPC5_PWM_USE_ETIMER2_CH1 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD14;
#endif

#if (SPC5_PWM_USE_ETIMER2_CH2 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD15;
#endif

#if (SPC5_PWM_USE_ETIMER2_CH3 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD16;
#endif

#if (SPC5_PWM_USE_ETIMER2_CH4 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD17;
#endif

#if (SPC5_PWM_USE_ETIMER2_CH5 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD18;
#endif

#if (SPC5_PWM_USE_ETIMER3_CH0 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD19;
#endif

#if (SPC5_PWM_USE_ETIMER3_CH1 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD20;
#endif

#if (SPC5_PWM_USE_ETIMER3_CH2 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD21;
#endif

#if (SPC5_PWM_USE_ETIMER3_CH3 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD22;
#endif

#if (SPC5_PWM_USE_ETIMER3_CH4 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD23;
#endif

#if (SPC5_PWM_USE_ETIMER3_CH5 == TRUE) && !defined(__DOXYGEN__)
extern PWMDriver PWMD24;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void pwm_lld_init(void);
  void pwm_lld_start(PWMDriver *pwmp, const PWMConfig *config);
  void pwm_lld_stop(PWMDriver *pwmp);
  void pwm_lld_change_period(PWMDriver *pwmp, pwmcnt_t period);
  void pwm_lld_enable_channel(PWMDriver *pwmp,
                              pwmchannel_t channel,
                              pwmcnt_t width);
  void pwm_lld_disable_channel(PWMDriver *pwmp, pwmchannel_t channel);
#ifdef __cplusplus
}
#endif

#endif /* LLD_USE_PWM */

#endif /* _PWM_LLD_H_ */

/** @} */
