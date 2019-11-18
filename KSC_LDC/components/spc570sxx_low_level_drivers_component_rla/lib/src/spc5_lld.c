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
 * @file    spc5_lld.c
 * @brief   SPC5xx subsystem low level driver code.
 *
 * @addtogroup SPC5_LLD
 * @{
 */

#include "spc5_lld.h"
#include "spc5_lld_cfg.h"

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/
 /**
 * @brief   SPC5xx subsystem low level driver initialization.
 *
 * @api
 */
void spc5LLDInit(void) {
  
#if (LLD_USE_DMA == TRUE) || defined(__DOXYGEN__)
  edmaInit();
#endif
#if (LLD_USE_PIT == TRUE) || defined(__DOXYGEN__)
  pit_lld_init();
#endif
#if (LLD_USE_STM == TRUE) || defined(__DOXYGEN__)
  stm_lld_init();
#endif
#if (LLD_USE_SERIAL == TRUE) || defined(__DOXYGEN__)
  sd_lld_init();
#endif
#if (LLD_USE_LIN == TRUE) || defined(__DOXYGEN__)
  lin_lld_init();
#endif
#if (LLD_USE_PWM == TRUE)|| defined(__DOXYGEN__)
  pwm_lld_init();
#endif
#if (LLD_USE_ICU == TRUE) || defined(__DOXYGEN__)
  icu_lld_init();
#endif
#if (LLD_USE_CAN  == TRUE) || defined(__DOXYGEN__)
  can_lld_init();
#endif
#if (LLD_USE_SPI == TRUE) || defined(__DOXYGEN__)
  spi_lld_init();
#endif
#if (LLD_USE_ADC == TRUE) || defined(__DOXYGEN__)
  adc_lld_init();
#endif
#if (LLD_USE_CTU == TRUE) || defined(__DOXYGEN__)
  ctu_lld_init();
#endif
#if (LLD_USE_I2C == TRUE) || defined(__DOXYGEN__)
  i2c_lld_init();
#endif
#if (LLD_USE_RTC == TRUE) || defined(__DOXYGEN__)
  rtc_lld_init();
#endif
#if (LLD_USE_SWT == TRUE) || defined(__DOXYGEN__)
  swt_lld_init();
#endif
#if (LLD_USE_SARADC == TRUE) || defined(__DOXYGEN__)
  saradc_lld_init();
#endif
#if (LLD_USE_SDADC == TRUE) || defined(__DOXYGEN__)
  sdadc_lld_init();
#endif
#if (LLD_USE_CRC == TRUE) || defined(__DOXYGEN__)
  crc_lld_init();
#endif
#if (LLD_USE_FCCU == TRUE) || defined(__DOXYGEN__)
  fccu_lld_init();
#endif
#if (LLD_USE_WKPU == TRUE) || defined(__DOXYGEN__)
  wkpu_lld_init();
#endif
#if (LLD_USE_I2S == TRUE) || defined(__DOXYGEN__)
  i2s_lld_init();
#endif
#if (LLD_USE_BCTU == TRUE) || defined(__DOXYGEN__)
  bctu_lld_init();
#endif
#if (LLD_USE_CMPU == TRUE) || defined(__DOXYGEN__)
  cmpu_lld_init();
#endif
}
/** @} */

