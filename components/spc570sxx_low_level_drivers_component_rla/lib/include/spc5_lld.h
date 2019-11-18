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
 * @file    spc5_lld.h
 * @brief   SPC5xx subsystem low level driver header.
 *
 * @addtogroup SPC5_LLD
 * @{
 */

#ifndef _SPC5_LLD_H_
#define _SPC5_LLD_H_

#include "platform.h"
#include "board.h"
#include "clock.h"
#include "osal.h"
#include "lldconf.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   DMA driver enable switch.
 * @note    The default is @p FALSE (DMA driver disabled).
 */
#if !defined(LLD_USE_DMA) || defined(__DOXYGEN__)
#define LLD_USE_DMA                         FALSE
#endif

/**
 * @brief   PIT driver enable switch.
 * @note    The default is @p FALSE (PIT driver disabled).
 */
#if !defined(LLD_USE_PIT) || defined(__DOXYGEN__)
#define LLD_USE_PIT                         FALSE
#endif

/**
 * @brief   STM driver enable switch.
 * @note    The default is @p FALSE (STM driver disabled).
 */
#if !defined(LLD_USE_STM) || defined(__DOXYGEN__)
#define LLD_USE_STM                         FALSE
#endif

/**
 * @brief   SERIAL driver enable switch.
 * @note    The default is @p FALSE (SERIAL driver disabled).
 */
#if !defined(LLD_USE_SERIAL) || defined(__DOXYGEN__)
#define LLD_USE_SERIAL                      FALSE
#endif

/**
 * @brief   LIN driver enable switch.
 * @note    The default is @p FALSE (LIN driver disabled).
 */
#if !defined(LLD_USE_LIN) || defined(__DOXYGEN__)
#define LLD_USE_LIN                         FALSE
#endif

/**
 * @brief   PWM driver enable switch.
 * @note    The default is @p FALSE (PWM driver disabled).
 */
#if !defined(LLD_USE_PWM) || defined(__DOXYGEN__)
#define LLD_USE_PWM                         FALSE
#endif

/**
 * @brief   ICU driver enable switch.
 * @note    The default is @p FALSE (ICU driver disabled).
 */
#if !defined(LLD_USE_ICU) || defined(__DOXYGEN__)
#define LLD_USE_ICU                         FALSE
#endif

/**
 * @brief   CAN driver enable switch.
 * @note    The default is @p FALSE (CAN driver disabled).
 */
#if !defined(LLD_USE_CAN) || defined(__DOXYGEN__)
#define LLD_USE_CAN                         FALSE
#endif

/**
 * @brief   SPI driver enable switch.
 * @note    The default is @p FALSE (SPI driver disabled).
 */
#if !defined(LLD_USE_SPI) || defined(__DOXYGEN__)
#define LLD_USE_SPI                         FALSE
#endif

/**
 * @brief   ADC driver enable switch.
 * @note    The default is @p FALSE (ADC driver disabled).
 */
#if !defined(LLD_USE_ADC) || defined(__DOXYGEN__)
#define LLD_USE_ADC                         FALSE
#endif

/**
 * @brief   CTU driver enable switch.
 * @note    The default is @p FALSE (CTU driver disabled).
 */
#if !defined(LLD_USE_CTU) || defined(__DOXYGEN__)
#define LLD_USE_CTU                         FALSE
#endif

/**
 * @brief   I2C driver enable switch.
 * @note    The default is @p FALSE (I2C driver disabled).
 */
#if !defined(LLD_USE_I2C) || defined(__DOXYGEN__)
#define LLD_USE_I2C                         FALSE
#endif

/**
 * @brief   RTC driver enable switch.
 * @note    The default is @p FALSE (RTC driver disabled).
 */
#if !defined(LLD_USE_RTC) || defined(__DOXYGEN__)
#define LLD_USE_RTC                         FALSE
#endif

/**
 * @brief   SWT driver enable switch.
 * @note    The default is @p FALSE (SWT driver disabled).
 */
#if !defined(LLD_USE_SWT) || defined(__DOXYGEN__)
#define LLD_USE_SWT                         FALSE
#endif

/**
 * @brief   SARADC driver enable switch.
 * @note    The default is @p FALSE (SARADC driver disabled).
 */
#if !defined(LLD_USE_SARADC) || defined(__DOXYGEN__)
#define LLD_USE_SARADC                      FALSE
#endif

/**
 * @brief   SDADC driver enable switch.
 * @note    The default is @p FALSE (SDADC driver disabled).
 */
#if !defined(LLD_USE_SDADC) || defined(__DOXYGEN__)
#define LLD_USE_SDADC                      FALSE
#endif

/**
 * @brief   CRC driver enable switch.
 * @note    The default is @p FALSE (CRC driver disabled).
 */
#if !defined(LLD_USE_CRC) || defined(__DOXYGEN__)
#define LLD_USE_CRC                         FALSE
#endif

/**
 * @brief   FCCU driver enable switch.
 * @note    The default is @p FALSE (FCCU driver disabled).
 */
#if !defined(LLD_USE_FCCU) || defined(__DOXYGEN__)
#define LLD_USE_FCCU                        FALSE
#endif

/**
 * @brief   WKPU driver enable switch.
 * @note    The default is @p FALSE (WKPU driver disabled).
 */
#if !defined(LLD_USE_WKPU) || defined(__DOXYGEN__)
#define LLD_USE_WKPU                        FALSE
#endif

/**
 * @brief   I2S driver enable switch.
 * @note    The default is @p FALSE (I2S driver disabled).
 */
#if !defined(LLD_USE_I2S) || defined(__DOXYGEN__)
#define LLD_USE_I2S                         FALSE
#endif

/**
 * @brief   BCTU driver enable switch.
 * @note    The default is @p FALSE (BCTU driver disabled).
 */
#if !defined(LLD_USE_BCTU) || defined(__DOXYGEN__)
#define LLD_USE_BCTU                        FALSE
#endif

/**
 * @brief   CMPU driver enable switch.
 * @note    The default is @p FALSE (CMPU driver disabled).
 */
#if !defined(LLD_USE_CMPU) || defined(__DOXYGEN__)
#define LLD_USE_CMPU                        FALSE
#endif

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* IRQ Handlers                                                              */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/
#ifdef __cplusplus
extern "C" {
#endif
  void spc5LLDInit(void);
#ifdef __cplusplus
}
#endif

#endif /* _SPC5_LLD_H_ */

/** @} */
