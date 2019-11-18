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
 * @file    spc5_linflex.c
 * @brief   SPC5xx LINFLEX low level driver code.
 *
 * @addtogroup LINFLEX
 * @{
 */

#include "spc5_linflex.h"
#include "serial_lld.h"
#include "lin_lld.h"

#if (LLD_USE_SERIAL == TRUE) || (LLD_USE_LIN == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/**
 * @brief   LinFlex initialization flag.
 */
static uint8_t linflex_initialization = 0U;

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   LIINFlex-0 serial driver identifier.
 */
#if (SPC5_LINFLEX0_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
LinflexDriver LinflexD1;
#endif

/**
 * @brief   LIINFlex-1 serial driver identifier.
 */
#if (SPC5_LINFLEX1_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
LinflexDriver LinflexD2;
#endif

/**
 * @brief   LIINFlex-2 serial driver identifier.
 */
#if (SPC5_LINFLEX2_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
LinflexDriver LinflexD3;
#endif

/**
 * @brief   LIINFlex-3 serial driver identifier.
 */
#if (SPC5_LINFLEX3_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
LinflexDriver LinflexD4;
#endif

/**
 * @brief   LIINFlex-4 serial driver identifier.
 */
#if (SPC5_LINFLEX4_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
LinflexDriver LinflexD5;
#endif

/**
 * @brief   LIINFlex-5 serial driver identifier.
 */
#if (SPC5_LINFLEX5_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
LinflexDriver LinflexD6;
#endif

/**
 * @brief   LIINFlex-6 serial driver identifier.
 */
#if (SPC5_LINFLEX6_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
LinflexDriver LinflexD7;
#endif

/**
 * @brief   LIINFlex-7 serial driver identifier.
 */
#if (SPC5_LINFLEX7_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
LinflexDriver LinflexD8;
#endif

/**
 * @brief   LIINFlex-8 serial driver identifier.
 */
#if (SPC5_LINFLEX8_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
LinflexDriver LinflexD9;
#endif

/**
 * @brief   LIINFlex-9 serial driver identifier.
 */
#if (SPC5_LINFLEX9_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
LinflexDriver LinflexD10;
#endif

/**
 * @brief   LIINFlex-10 serial driver identifier.
 */
#if (SPC5_LINFLEX10_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
LinflexDriver LinflexD11;
#endif

/**
 * @brief   LIINFlex-11 serial driver identifier.
 */
#if (SPC5_LINFLEX11_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
LinflexDriver LinflexD12;
#endif

/**
 * @brief   LIINFlex-12 serial driver identifier.
 */
#if (SPC5_LINFLEX12_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
LinflexDriver LinflexD13;
#endif

/**
 * @brief   LIINFlex-13 serial driver identifier.
 */
#if (SPC5_LINFLEX13_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
LinflexDriver LinflexD14;
#endif

/**
 * @brief   LIINFlex-14 serial driver identifier.
 */
#if (SPC5_LINFLEX14_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
LinflexDriver LinflexD15;
#endif

/**
 * @brief   LIINFlex-15 serial driver identifier.
 */
#if (SPC5_LINFLEX15_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
LinflexDriver LinflexD16;
#endif

/**
 * @brief   LIINFlex-16 serial driver identifier.
 */
#if (SPC5_LINFLEX16_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
LinflexDriver LinflexD17;
#endif

/**
 * @brief   LIINFlex-17 serial driver identifier.
 */
#if (SPC5_LINFLEX17_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
LinflexDriver LinflexD18;
#endif

/**
 * @brief   LIINFlex-18 serial driver identifier.
 */
#if (SPC5_LINFLEX18_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
LinflexDriver LinflexD19;
#endif

/**
 * @brief   LIINFlex-19 serial driver identifier.
 */
#if (SPC5_LINFLEX19_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
LinflexDriver LinflexD20;
#endif

/**
 * @brief   LIINFlex-20 serial driver identifier.
 */
#if (SPC5_LINFLEX20_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
LinflexDriver LinflexD21;
#endif

/**
 * @brief   LIINFlex-21 serial driver identifier.
 */
#if (SPC5_LINFLEX21_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
LinflexDriver LinflexD22;
#endif

/**
 * @brief   LIINFlex-22 serial driver identifier.
 */
#if (SPC5_LINFLEX22_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
LinflexDriver LinflexD23;
#endif

/**
 * @brief   LIINFlex-23 serial driver identifier.
 */
#if (SPC5_LINFLEX23_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
LinflexDriver LinflexD24;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if (SPC5_LINFLEX0_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX0_RXI_HANDLER)
#error "SPC5_LINFLEX0_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-0 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX0_RXI_HANDLER) {
  IRQ_PROLOGUE();

  LinflexD1.rxi_lincallback(LinflexD1.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX0_TXI_HANDLER)
#error "SPC5_LINFLEX0_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-0 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX0_TXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD1.txi_lincallback(LinflexD1.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX0_ERR_HANDLER)
#error "SPC5_LINFLEX0_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-0 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX0_ERR_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD1.err_lincallback(LinflexD1.device);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_LINFLEX1_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX1_RXI_HANDLER)
#error "SPC5_LINFLEX1_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-1 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX1_RXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD2.rxi_lincallback(LinflexD2.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX1_TXI_HANDLER)
#error "SPC5_LINFLEX1_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-1 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX1_TXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD2.txi_lincallback(LinflexD2.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX1_ERR_HANDLER)
#error "SPC5_LINFLEX1_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-1 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX1_ERR_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD2.err_lincallback(LinflexD2.device);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_LINFLEX2_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX2_RXI_HANDLER)
#error "SPC5_LINFLEX2_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-2 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX2_RXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD3.rxi_lincallback(LinflexD3.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX2_TXI_HANDLER)
#error "SPC5_LINFLEX2_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-2 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX2_TXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD3.txi_lincallback(LinflexD3.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX2_ERR_HANDLER)
#error "SPC5_LINFLEX2_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-2 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX2_ERR_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD3.err_lincallback(LinflexD3.device);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_LINFLEX3_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX3_RXI_HANDLER)
#error "SPC5_LINFLEX3_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-3 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX3_RXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD4.rxi_lincallback(LinflexD4.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX3_TXI_HANDLER)
#error "SPC5_LINFLEX3_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-3 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX3_TXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD4.txi_lincallback(LinflexD4.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX3_ERR_HANDLER)
#error "SPC5_LINFLEX3_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-3 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX3_ERR_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD4.err_lincallback(LinflexD4.device);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_LINFLEX4_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX4_RXI_HANDLER)
#error "SPC5_LINFLEX4_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-4 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX4_RXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD5.rxi_lincallback(LinflexD5.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX4_TXI_HANDLER)
#error "SPC5_LINFLEX4_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-4 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX4_TXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD5.txi_lincallback(LinflexD5.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX4_ERR_HANDLER)
#error "SPC5_LINFLEX4_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-4 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX4_ERR_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD5.err_lincallback(LinflexD5.device);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_LINFLEX5_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX5_RXI_HANDLER)
#error "SPC5_LINFLEX5_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-5 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX5_RXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD6.rxi_lincallback(LinflexD6.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX5_TXI_HANDLER)
#error "SPC5_LINFLEX5_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-5 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX5_TXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD6.txi_lincallback(LinflexD6.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX5_ERR_HANDLER)
#error "SPC5_LINFLEX5_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-5 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX5_ERR_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD6.err_lincallback(LinflexD6.device);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_LINFLEX6_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX6_RXI_HANDLER)
#error "SPC5_LINFLEX6_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-6 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX6_RXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD7.rxi_lincallback(LinflexD7.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX6_TXI_HANDLER)
#error "SPC5_LINFLEX6_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-6 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX6_TXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD7.txi_lincallback(LinflexD7.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX6_ERR_HANDLER)
#error "SPC5_LINFLEX6_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-6 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX6_ERR_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD7.err_lincallback(LinflexD7.device);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_LINFLEX7_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX7_RXI_HANDLER)
#error "SPC5_LINFLEX7_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-7 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX7_RXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD8.rxi_lincallback(LinflexD8.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX7_TXI_HANDLER)
#error "SPC5_LINFLEX7_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-7 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX7_TXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD8.txi_lincallback(LinflexD8.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX7_ERR_HANDLER)
#error "SPC5_LINFLEX7_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-7 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX7_ERR_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD8.err_lincallback(LinflexD8.device);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_LINFLEX8_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX8_RXI_HANDLER)
#error "SPC5_LINFLEX8_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-8 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX8_RXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD9.rxi_lincallback(LinflexD9.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX8_TXI_HANDLER)
#error "SPC5_LINFLEX8_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-8 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX8_TXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD9.txi_lincallback(LinflexD9.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX8_ERR_HANDLER)
#error "SPC5_LINFLEX8_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-8 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX8_ERR_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD9.err_lincallback(LinflexD9.device);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_LINFLEX9_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX9_RXI_HANDLER)
#error "SPC5_LINFLEX9_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-9 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX9_RXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD10.rxi_lincallback(LinflexD10.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX9_TXI_HANDLER)
#error "SPC5_LINFLEX9_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-9 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX9_TXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD10.txi_lincallback(LinflexD10.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX9_ERR_HANDLER)
#error "SPC5_LINFLEX9_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-9 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX9_ERR_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD10.err_lincallback(LinflexD10.device);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_LINFLEX10_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX10_RXI_HANDLER)
#error "SPC5_LINFLEX10_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-10 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX10_RXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD11.rxi_lincallback(LinflexD11.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX10_TXI_HANDLER)
#error "SPC5_LINFLEX10_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-10 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX10_TXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD11.txi_lincallback(LinflexD11.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX10_ERR_HANDLER)
#error "SPC5_LINFLEX10_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-10 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX10_ERR_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD11.err_lincallback(LinflexD11.device);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_LINFLEX11_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX11_RXI_HANDLER)
#error "SPC5_LINFLEX11_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-11 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX11_RXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD12.rxi_lincallback(LinflexD12.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX11_TXI_HANDLER)
#error "SPC5_LINFLEX11_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-11 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX11_TXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD12.txi_lincallback(LinflexD12.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX11_ERR_HANDLER)
#error "SPC5_LINFLEX11_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-11 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX11_ERR_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD12.err_lincallback(LinflexD12.device);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_LINFLEX12_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX12_RXI_HANDLER)
#error "SPC5_LINFLEX12_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-12 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX12_RXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD13.rxi_lincallback(LinflexD13.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX12_TXI_HANDLER)
#error "SPC5_LINFLEX12_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-12 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX12_TXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD13.txi_lincallback(LinflexD13.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX12_ERR_HANDLER)
#error "SPC5_LINFLEX12_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-12 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX12_ERR_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD13.err_lincallback(LinflexD13.device);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_LINFLEX13_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX13_RXI_HANDLER)
#error "SPC5_LINFLEX13_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-13 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX13_RXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD14.rxi_lincallback(LinflexD14.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX13_TXI_HANDLER)
#error "SPC5_LINFLEX13_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-13 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX13_TXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD14.txi_lincallback(LinflexD14.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX13_ERR_HANDLER)
#error "SPC5_LINFLEX13_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-13 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX13_ERR_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD14.err_lincallback(LinflexD14.device);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_LINFLEX14_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX14_RXI_HANDLER)
#error "SPC5_LINFLEX14_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-14 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX14_RXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD15.rxi_lincallback(LinflexD15.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX14_TXI_HANDLER)
#error "SPC5_LINFLEX14_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-14 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX14_TXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD15.txi_lincallback(LinflexD15.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX14_ERR_HANDLER)
#error "SPC5_LINFLEX14_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-14 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX14_ERR_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD15.err_lincallback(LinflexD15.device);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_LINFLEX15_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX15_RXI_HANDLER)
#error "SPC5_LINFLEX15_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-15 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX15_RXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD16.rxi_lincallback(LinflexD16.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX15_TXI_HANDLER)
#error "SPC5_LINFLEX15_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-15 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX15_TXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD16.txi_lincallback(LinflexD16.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX15_ERR_HANDLER)
#error "SPC5_LINFLEX15_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-15 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX15_ERR_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD16.err_lincallback(LinflexD16.device);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_LINFLEX16_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX16_RXI_HANDLER)
#error "SPC5_LINFLEX16_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-16 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX16_RXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD17.rxi_lincallback(LinflexD17.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX16_TXI_HANDLER)
#error "SPC5_LINFLEX16_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-16 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX16_TXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD17.txi_lincallback(LinflexD17.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX16_ERR_HANDLER)
#error "SPC5_LINFLEX16_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-16 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX16_ERR_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD17.err_lincallback(LinflexD17.device);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_LINFLEX17_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX17_RXI_HANDLER)
#error "SPC5_LINFLEX17_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-17 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX17_RXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD18.rxi_lincallback(LinflexD18.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX17_TXI_HANDLER)
#error "SPC5_LINFLEX17_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-17 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX17_TXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD18.txi_lincallback(LinflexD18.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX17_ERR_HANDLER)
#error "SPC5_LINFLEX17_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-17 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX17_ERR_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD18.err_lincallback(LinflexD18.device);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_LINFLEX18_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX18_RXI_HANDLER)
#error "SPC5_LINFLEX18_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-18 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX18_RXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD19.rxi_lincallback(LinflexD19.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX18_TXI_HANDLER)
#error "SPC5_LINFLEX18_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-18 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX18_TXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD19.txi_lincallback(LinflexD19.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX18_ERR_HANDLER)
#error "SPC5_LINFLEX18_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-18 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX18_ERR_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD19.err_lincallback(LinflexD19.device);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_LINFLEX19_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX19_RXI_HANDLER)
#error "SPC5_LINFLEX19_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-19 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX19_RXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD20.rxi_lincallback(LinflexD20.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX19_TXI_HANDLER)
#error "SPC5_LINFLEX19_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-19 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX19_TXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD20.txi_lincallback(LinflexD20.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX19_ERR_HANDLER)
#error "SPC5_LINFLEX19_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-19 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX19_ERR_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD20.err_lincallback(LinflexD20.device);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_LINFLEX20_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX20_RXI_HANDLER)
#error "SPC5_LINFLEX20_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-20 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX20_RXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD21.rxi_lincallback(LinflexD21.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX20_TXI_HANDLER)
#error "SPC5_LINFLEX20_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-20 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX20_TXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD21.txi_lincallback(LinflexD21.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX20_ERR_HANDLER)
#error "SPC5_LINFLEX20_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-20 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX20_ERR_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD21.err_lincallback(LinflexD21.device);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_LINFLEX21_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX21_RXI_HANDLER)
#error "SPC5_LINFLEX21_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-21 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX21_RXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD22.rxi_lincallback(LinflexD22.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX21_TXI_HANDLER)
#error "SPC5_LINFLEX21_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-21 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX21_TXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD22.txi_lincallback(LinflexD22.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX21_ERR_HANDLER)
#error "SPC5_LINFLEX21_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-21 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX21_ERR_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD22.err_lincallback(LinflexD22.device);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_LINFLEX22_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX22_RXI_HANDLER)
#error "SPC5_LINFLEX22_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-22 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX22_RXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD23.rxi_lincallback(LinflexD23.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX22_TXI_HANDLER)
#error "SPC5_LINFLEX22_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-22 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX22_TXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD23.txi_lincallback(LinflexD23.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX22_ERR_HANDLER)
#error "SPC5_LINFLEX22_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-22 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX22_ERR_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD23.err_lincallback(LinflexD23.device);

  IRQ_EPILOGUE();
}
#endif

#if (SPC5_LINFLEX23_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
#if !defined(SPC5_LINFLEX23_RXI_HANDLER)
#error "SPC5_LINFLEX23_RXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-23 RXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX23_RXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD24.rxi_lincallback(LinflexD24.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX23_TXI_HANDLER)
#error "SPC5_LINFLEX23_TXI_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-23 TXI interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX23_TXI_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD24.txi_lincallback(LinflexD24.device);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_LINFLEX23_ERR_HANDLER)
#error "SPC5_LINFLEX23_ERR_HANDLER not defined"
#endif
/**
 * @brief   LINFlex-23 ERR interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_LINFLEX23_ERR_HANDLER) {

  IRQ_PROLOGUE();

  LinflexD24.err_lincallback(LinflexD24.device);

  IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level linflex driver initialization.
 *
 * @init
 */
void linflex_lld_init(void) {

  /* Check the LinFlex initialization flag in
     order to run the initialization once.*/
  if (linflex_initialization == 0U) {
    linflex_initialization++;

#if (SPC5_LINFLEX0_SETTING != SPC5_LINFLEX_MODE_NONE)
    INTC_PSR (SPC5_LINFLEX0_RXI_NUMBER) = SPC5_LINFLEX0_PRIORITY;
    INTC_PSR (SPC5_LINFLEX0_TXI_NUMBER) = SPC5_LINFLEX0_PRIORITY;
    INTC_PSR (SPC5_LINFLEX0_ERR_NUMBER) = SPC5_LINFLEX0_PRIORITY;
#endif /* SPC5_LINFLEX0_SETTING */

#if (SPC5_LINFLEX1_SETTING != SPC5_LINFLEX_MODE_NONE)
    INTC_PSR (SPC5_LINFLEX1_RXI_NUMBER) = SPC5_LINFLEX1_PRIORITY;
    INTC_PSR (SPC5_LINFLEX1_TXI_NUMBER) = SPC5_LINFLEX1_PRIORITY;
    INTC_PSR (SPC5_LINFLEX1_ERR_NUMBER) = SPC5_LINFLEX1_PRIORITY;
#endif /* SPC5_LINFLEX1_SETTING*/

#if (SPC5_LINFLEX2_SETTING != SPC5_LINFLEX_MODE_NONE)
    INTC_PSR (SPC5_LINFLEX2_RXI_NUMBER) = SPC5_LINFLEX2_PRIORITY;
    INTC_PSR (SPC5_LINFLEX2_TXI_NUMBER) = SPC5_LINFLEX2_PRIORITY;
    INTC_PSR (SPC5_LINFLEX2_ERR_NUMBER) = SPC5_LINFLEX2_PRIORITY;
#endif /* SPC5_LINFLEX2_SETTING */

#if (SPC5_LINFLEX3_SETTING != SPC5_LINFLEX_MODE_NONE)
    INTC_PSR (SPC5_LINFLEX3_RXI_NUMBER) = SPC5_LINFLEX3_PRIORITY;
    INTC_PSR (SPC5_LINFLEX3_TXI_NUMBER) = SPC5_LINFLEX3_PRIORITY;
    INTC_PSR (SPC5_LINFLEX3_ERR_NUMBER) = SPC5_LINFLEX3_PRIORITY;
#endif /* SPC5_LINFLEX3_SETTING */

#if (SPC5_LINFLEX4_SETTING != SPC5_LINFLEX_MODE_NONE)
    INTC_PSR (SPC5_LINFLEX4_RXI_NUMBER) = SPC5_LINFLEX4_PRIORITY;
    INTC_PSR (SPC5_LINFLEX4_TXI_NUMBER) = SPC5_LINFLEX4_PRIORITY;
    INTC_PSR (SPC5_LINFLEX4_ERR_NUMBER) = SPC5_LINFLEX4_PRIORITY;
#endif /* SPC5_LINFLEX4_SETTING*/

#if (SPC5_LINFLEX5_SETTING != SPC5_LINFLEX_MODE_NONE)
    INTC_PSR (SPC5_LINFLEX5_RXI_NUMBER) = SPC5_LINFLEX5_PRIORITY;
    INTC_PSR (SPC5_LINFLEX5_TXI_NUMBER) = SPC5_LINFLEX5_PRIORITY;
    INTC_PSR (SPC5_LINFLEX5_ERR_NUMBER) = SPC5_LINFLEX5_PRIORITY;
#endif /* SPC5_LINFLEX5_SETTING */

#if (SPC5_LINFLEX6_SETTING != SPC5_LINFLEX_MODE_NONE)
    INTC_PSR (SPC5_LINFLEX6_RXI_NUMBER) = SPC5_LINFLEX6_PRIORITY;
    INTC_PSR (SPC5_LINFLEX6_TXI_NUMBER) = SPC5_LINFLEX6_PRIORITY;
    INTC_PSR (SPC5_LINFLEX6_ERR_NUMBER) = SPC5_LINFLEX6_PRIORITY;
#endif /* SPC5_LINFLEX6_SETTING */

#if (SPC5_LINFLEX7_SETTING != SPC5_LINFLEX_MODE_NONE)
    INTC_PSR (SPC5_LINFLEX7_RXI_NUMBER) = SPC5_LINFLEX7_PRIORITY;
    INTC_PSR (SPC5_LINFLEX7_TXI_NUMBER) = SPC5_LINFLEX7_PRIORITY;
    INTC_PSR (SPC5_LINFLEX7_ERR_NUMBER) = SPC5_LINFLEX7_PRIORITY;
#endif /* SPC5_LINFLEX7_SETTING */

#if (SPC5_LINFLEX8_SETTING != SPC5_LINFLEX_MODE_NONE)
    INTC_PSR (SPC5_LINFLEX8_RXI_NUMBER) = SPC5_LINFLEX8_PRIORITY;
    INTC_PSR (SPC5_LINFLEX8_TXI_NUMBER) = SPC5_LINFLEX8_PRIORITY;
    INTC_PSR (SPC5_LINFLEX8_ERR_NUMBER) = SPC5_LINFLEX8_PRIORITY;
#endif /* SPC5_LINFLEX8_SETTING */

#if (SPC5_LINFLEX9_SETTING != SPC5_LINFLEX_MODE_NONE)
    INTC_PSR (SPC5_LINFLEX9_RXI_NUMBER) = SPC5_LINFLEX9_PRIORITY;
    INTC_PSR (SPC5_LINFLEX9_TXI_NUMBER) = SPC5_LINFLEX9_PRIORITY;
    INTC_PSR (SPC5_LINFLEX9_ERR_NUMBER) = SPC5_LINFLEX9_PRIORITY;
#endif /* SPC5_LINFLEX9_SETTING */

#if (SPC5_LINFLEX10_SETTING != SPC5_LINFLEX_MODE_NONE)
    INTC_PSR (SPC5_LINFLEX10_RXI_NUMBER) = SPC5_LINFLEX10_PRIORITY;
    INTC_PSR (SPC5_LINFLEX10_TXI_NUMBER) = SPC5_LINFLEX10_PRIORITY;
    INTC_PSR (SPC5_LINFLEX10_ERR_NUMBER) = SPC5_LINFLEX10_PRIORITY;
#endif /* SPC5_LINFLEX10_SETTING */

#if (SPC5_LINFLEX11_SETTING != SPC5_LINFLEX_MODE_NONE)
    INTC_PSR (SPC5_LINFLEX11_RXI_NUMBER) = SPC5_LINFLEX11_PRIORITY;
    INTC_PSR (SPC5_LINFLEX11_TXI_NUMBER) = SPC5_LINFLEX11_PRIORITY;
    INTC_PSR (SPC5_LINFLEX11_ERR_NUMBER) = SPC5_LINFLEX11_PRIORITY;
#endif /* SPC5_LINFLEX11_SETTING */

#if (SPC5_LINFLEX12_SETTING != SPC5_LINFLEX_MODE_NONE)
    INTC_PSR (SPC5_LINFLEX12_RXI_NUMBER) = SPC5_LINFLEX12_PRIORITY;
    INTC_PSR (SPC5_LINFLEX12_TXI_NUMBER) = SPC5_LINFLEX12_PRIORITY;
    INTC_PSR (SPC5_LINFLEX12_ERR_NUMBER) = SPC5_LINFLEX12_PRIORITY;
#endif /* SPC5_LINFLEX12_SETTING */

#if (SPC5_LINFLEX13_SETTING != SPC5_LINFLEX_MODE_NONE)
    INTC_PSR (SPC5_LINFLEX13_RXI_NUMBER) = SPC5_LINFLEX13_PRIORITY;
    INTC_PSR (SPC5_LINFLEX13_TXI_NUMBER) = SPC5_LINFLEX13_PRIORITY;
    INTC_PSR (SPC5_LINFLEX13_ERR_NUMBER) = SPC5_LINFLEX13_PRIORITY;
#endif /* SPC5_LINFLEX13_SETTING */

#if (SPC5_LINFLEX14_SETTING != SPC5_LINFLEX_MODE_NONE)
    INTC_PSR (SPC5_LINFLEX14_RXI_NUMBER) = SPC5_LINFLEX14_PRIORITY;
    INTC_PSR (SPC5_LINFLEX14_TXI_NUMBER) = SPC5_LINFLEX14_PRIORITY;
    INTC_PSR (SPC5_LINFLEX14_ERR_NUMBER) = SPC5_LINFLEX14_PRIORITY;
#endif /* SPC5_LINFLEX14_SETTING */

#if (SPC5_LINFLEX15_SETTING != SPC5_LINFLEX_MODE_NONE)
    INTC_PSR (SPC5_LINFLEX15_RXI_NUMBER) = SPC5_LINFLEX15_PRIORITY;
    INTC_PSR (SPC5_LINFLEX15_TXI_NUMBER) = SPC5_LINFLEX15_PRIORITY;
    INTC_PSR (SPC5_LINFLEX15_ERR_NUMBER) = SPC5_LINFLEX15_PRIORITY;
#endif /* SPC5_LINFLEX15_SETTING */

#if (SPC5_LINFLEX16_SETTING != SPC5_LINFLEX_MODE_NONE)
    INTC_PSR (SPC5_LINFLEX16_RXI_NUMBER) = SPC5_LINFLEX16_PRIORITY;
    INTC_PSR (SPC5_LINFLEX16_TXI_NUMBER) = SPC5_LINFLEX16_PRIORITY;
    INTC_PSR (SPC5_LINFLEX16_ERR_NUMBER) = SPC5_LINFLEX16_PRIORITY;
#endif /* SPC5_LINFLEX16_SETTING */

#if (SPC5_LINFLEX17_SETTING != SPC5_LINFLEX_MODE_NONE)
    INTC_PSR (SPC5_LINFLEX17_RXI_NUMBER) = SPC5_LINFLEX17_PRIORITY;
    INTC_PSR (SPC5_LINFLEX17_TXI_NUMBER) = SPC5_LINFLEX17_PRIORITY;
    INTC_PSR (SPC5_LINFLEX17_ERR_NUMBER) = SPC5_LINFLEX17_PRIORITY;
#endif /* SPC5_LINFLEX17_SETTING */

#if (SPC5_LINFLEX18_SETTING != SPC5_LINFLEX_MODE_NONE)
    INTC_PSR (SPC5_LINFLEX18_RXI_NUMBER) = SPC5_LINFLEX18_PRIORITY;
    INTC_PSR (SPC5_LINFLEX18_TXI_NUMBER) = SPC5_LINFLEX18_PRIORITY;
    INTC_PSR (SPC5_LINFLEX18_ERR_NUMBER) = SPC5_LINFLEX18_PRIORITY;
#endif /* SPC5_LINFLEX18_SETTING */

#if (SPC5_LINFLEX19_SETTING != SPC5_LINFLEX_MODE_NONE)
    INTC_PSR (SPC5_LINFLEX19_RXI_NUMBER) = SPC5_LINFLEX19_PRIORITY;
    INTC_PSR (SPC5_LINFLEX19_TXI_NUMBER) = SPC5_LINFLEX19_PRIORITY;
    INTC_PSR (SPC5_LINFLEX19_ERR_NUMBER) = SPC5_LINFLEX19_PRIORITY;
#endif /* SPC5_LINFLEX19_SETTING */

#if (SPC5_LINFLEX20_SETTING != SPC5_LINFLEX_MODE_NONE)
    INTC_PSR (SPC5_LINFLEX20_RXI_NUMBER) = SPC5_LINFLEX20_PRIORITY;
    INTC_PSR (SPC5_LINFLEX20_TXI_NUMBER) = SPC5_LINFLEX20_PRIORITY;
    INTC_PSR (SPC5_LINFLEX20_ERR_NUMBER) = SPC5_LINFLEX20_PRIORITY;
#endif /* SPC5_LINFLEX20_SETTING */

#if (SPC5_LINFLEX21_SETTING != SPC5_LINFLEX_MODE_NONE)
    INTC_PSR (SPC5_LINFLEX21_RXI_NUMBER) = SPC5_LINFLEX21_PRIORITY;
    INTC_PSR (SPC5_LINFLEX21_TXI_NUMBER) = SPC5_LINFLEX21_PRIORITY;
    INTC_PSR (SPC5_LINFLEX21_ERR_NUMBER) = SPC5_LINFLEX21_PRIORITY;
#endif /* SPC5_LINFLEX21_SETTING */

#if (SPC5_LINFLEX22_SETTING != SPC5_LINFLEX_MODE_NONE)
    INTC_PSR (SPC5_LINFLEX22_RXI_NUMBER) = SPC5_LINFLEX22_PRIORITY;
    INTC_PSR (SPC5_LINFLEX22_TXI_NUMBER) = SPC5_LINFLEX22_PRIORITY;
    INTC_PSR (SPC5_LINFLEX22_ERR_NUMBER) = SPC5_LINFLEX22_PRIORITY;
#endif /* SPC5_LINFLEX22_SETTING */

#if (SPC5_LINFLEX23_SETTING != SPC5_LINFLEX_MODE_NONE)
    INTC_PSR (SPC5_LINFLEX23_RXI_NUMBER) = SPC5_LINFLEX23_PRIORITY;
    INTC_PSR (SPC5_LINFLEX23_TXI_NUMBER) = SPC5_LINFLEX23_PRIORITY;
    INTC_PSR (SPC5_LINFLEX23_ERR_NUMBER) = SPC5_LINFLEX23_PRIORITY;
#endif /* SPC5_LINFLEX23_SETTING */

  }
}

#endif /* LLD_USE_SERIAL */

/** @} */
