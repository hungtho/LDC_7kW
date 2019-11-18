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
 * @file    can_lld.c
 * @brief   SPC5xx CAN low level driver code.
 *
 * @addtogroup CAN
 * @{
 */

#include "can_lld.h"

#if (LLD_USE_CAN == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief CAN1 driver identifier.*/
#if (SPC5_CAN_USE_FLEXCAN0 == TRUE) || defined(__DOXYGEN__)
CANDriver CAND1;
#endif

/** @brief CAN2 driver identifier.*/
#if (SPC5_CAN_USE_FLEXCAN1 == TRUE) || defined(__DOXYGEN__)
CANDriver CAND2;
#endif

/** @brief CAN3 driver identifier.*/
#if (SPC5_CAN_USE_FLEXCAN2 == TRUE) || defined(__DOXYGEN__)
CANDriver CAND3;
#endif

/** @brief CAN4 driver identifier.*/
#if (SPC5_CAN_USE_FLEXCAN3 == TRUE) || defined(__DOXYGEN__)
CANDriver CAND4;
#endif

/** @brief CAN5 driver identifier.*/
#if (SPC5_CAN_USE_FLEXCAN4 == TRUE) || defined(__DOXYGEN__)
CANDriver CAND5;
#endif

/** @brief CAN6 driver identifier.*/
#if (SPC5_CAN_USE_FLEXCAN5 == TRUE) || defined(__DOXYGEN__)
CANDriver CAND6;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Common TX and RX ISR handler.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 *
 * @notapi
 */
static void can_lld_handler(CANDriver *canp) {
  uint32_t iflag1, iflag2;

  iflag2 = 0;

  iflag1 = canp->flexcan->IFRL.R;

  /* check if FIFO interrupt occurred */

  if ((canp->config->use_fifo == TRUE) && ((iflag1 & 0x20UL) != 0UL)) {
    CANRxFrame crfp;

    /* read message */
    crfp.data32[0] = canp->flexcan->BUF[0].DATA.W[0];
    crfp.data32[1] = canp->flexcan->BUF[0].DATA.W[1];
    crfp.RTR = canp->flexcan->BUF[0].CS.B.RTR;
    crfp.IDE = canp->flexcan->BUF[0].CS.B.IDE;
    if (crfp.IDE != 0U) {
      crfp.EID = canp->flexcan->BUF[0].ID.R & 0x1FFFFFFFU;
    }
    else {
      crfp.SID = canp->flexcan->BUF[0].ID.B.STD_ID;
    }
    crfp.LENGTH = canp->flexcan->BUF[0].CS.B.LENGTH;
    crfp.TIME = canp->flexcan->BUF[0].CS.B.TIMESTAMP;

    /* callback */
    if (canp->config->fifo_rx_callback != NULL) {
      canp->config->fifo_rx_callback(canp, crfp);
    }

    /* clear flag */
    canp->flexcan->IFRL.R = (1UL << 5);
  }
  /* clear flag */
  else {
    canp->flexcan->IFRL.R = iflag1;
  }

  /* ISR flag clean for MB greater than 32 */
#if (SPC5_CAN_USE_FLEXCAN0 == TRUE) 
#if (SPC5_FLEXCAN0_MB == 64U)
  if(&CAND1 == canp) {
    iflag2 = canp->flexcan->IFRH.R;
    canp->flexcan->IFRH.R = canp->flexcan->IFRH.R;
  }
#endif
#endif
#if (SPC5_CAN_USE_FLEXCAN1 == TRUE) 
#if (SPC5_FLEXCAN1_MB == 64U)
  if(&CAND2 == canp) {
    iflag2 = canp->flexcan->IFRH.R;
    canp->flexcan->IFRH.R = canp->flexcan->IFRH.R;
  }
#endif
#endif
#if (SPC5_CAN_USE_FLEXCAN2 == TRUE) 
#if (SPC5_FLEXCAN2_MB == 64U)
  if(&CAND3 == canp) {
    iflag2 = canp->flexcan->IFRH.R;
    canp->flexcan->IFRH.R = canp->flexcan->IFRH.R;
  }
#endif
#endif
#if (SPC5_CAN_USE_FLEXCAN3 == TRUE) 
#if (SPC5_FLEXCAN3_MB == 64U)
  if(&CAND4 == canp) {
    iflag2 = canp->flexcan->IFRH.R;
    canp->flexcan->IFRH.R = canp->flexcan->IFRH.R;
  }
#endif
#endif
#if (SPC5_CAN_USE_FLEXCAN4 == TRUE) 
#if (SPC5_FLEXCAN4_MB == 64U)
  if(&CAND5 == canp) {
    iflag2 = canp->flexcan->IFRH.R;
    canp->flexcan->IFRH.R = canp->flexcan->IFRH.R;
  }
#endif
#endif
#if (SPC5_CAN_USE_FLEXCAN5 == TRUE) 
#if(SPC5_FLEXCAN5_MB == 64U)
  if(&CAND6 == canp) {
    iflag2 = canp->flexcan->IFRH.R;
    canp->flexcan->IFRH.R = canp->flexcan->IFRH.R;
  }
#endif
#endif
  (void)iflag2;
}

/**
 * @brief   Common error ISR handler.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 *
 * @notapi
 */
static void can_lld_err_handler(CANDriver *canp) {

  uint32_t esr = canp->flexcan->ESR.R;
  uint8_t rx_err_counter = (uint8_t)canp->flexcan->ECR.B.RXECNT;
  uint8_t tx_err_counter = (uint8_t)canp->flexcan->ECR.B.TXECNT;

  /*callback*/
  if (canp->config->flexcan_err_callback != NULL){
	  canp->config->flexcan_err_callback(canp,esr,rx_err_counter,tx_err_counter);
  }

  /* Error event.*/
  if (((esr & CAN_ESR_TWRN_INT) != 0U) || ((esr & CAN_ESR_RWRN_INT) != 0U)) {
    canp->flexcan->ESR.R = CAN_ESR_TWRN_INT;
    canp->flexcan->ESR.R = CAN_ESR_RWRN_INT;
  }

  if ((esr & CAN_ESR_BOFF_INT) != 0U) {
    canp->flexcan->ESR.R = CAN_ESR_BOFF_INT;
  }

  if ((esr & CAN_ESR_ERR_INT) != 0U) {
    canp->flexcan->ESR.R = CAN_ESR_ERR_INT;
  }
}

#if (SPC5_CAN_USE_FLEXCAN0 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_FLEXCAN0_SHARED_IRQ == FALSE)
/**
 * @brief   CAN1 RX interrupt handler for MB 0.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_00_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND1);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN1 RX interrupt handler for MB 1.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_01_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND1);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN1 RX interrupt handler for MB 2.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_02_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND1);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN1 RX interrupt handler for MB 3.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_03_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND1);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN1 RX interrupt handler for MB 4.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_04_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND1);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN1 RX interrupt handler for MB 5.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_05_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND1);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN1 RX interrupt handler for MB 6.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_06_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND1);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN1 RX interrupt handler for MB 7.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_07_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND1);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN1 TX interrupt handler for MB 8.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_08_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND1);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN1 TX interrupt handler for MB 9.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_09_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND1);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN1 TX interrupt handler for MB 10.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_10_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND1);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN1 TX interrupt handler for MB 11.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_11_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND1);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN1 TX interrupt handler for MB 12.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_12_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND1);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN1 TX interrupt handler for MB 13.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_13_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND1);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN1 TX interrupt handler for MB 14.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_14_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND1);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN1 TX interrupt handler for MB 15.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_15_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND1);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN1 TX interrupt handler for MB 16-31.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_16_31_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND1);

  IRQ_EPILOGUE();
}

#if (SPC5_FLEXCAN0_MB == 64U)
/**
 * @brief   CAN1 TX interrupt handler for MB 32-63.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_32_63_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND1);

  IRQ_EPILOGUE();
}
#endif

/**
 * @brief   CAN1 ESR_ERR_INT interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN0_ESR_ERR_INT_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_err_handler(&CAND1);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN1 ESR_BOFF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN0_ESR_BOFF_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_err_handler(&CAND1);

  IRQ_EPILOGUE();
}
#else
/**
 * @brief   CAN1 TX interrupt handler for MB 8-11.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_08_11_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND1);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN1 TX interrupt handler for MB 12-15.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_12_15_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND1);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN1 TX interrupt handler for MB 16-31.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_16_31_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND1);

  IRQ_EPILOGUE();
}

#if (SPC5_FLEXCAN0_MB == 64U)
/**
 * @brief   CAN1 TX interrupt handler for MB 32-63.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_32_63_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND1);

  IRQ_EPILOGUE();
}
#endif

/*
 * @brief   CAN1 RX interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_00_03_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND1);

  IRQ_EPILOGUE();
}

/*
 * @brief   CAN1 RX interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_04_07_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND1);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN1 ESR_ERR_INT interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN0_ESR_ERR_INT_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_err_handler(&CAND1);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN1 ESR_BOFF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN0_ESR_BOFF_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_err_handler(&CAND1);

  IRQ_EPILOGUE();
}
#endif
#endif /* SPC5_CAN_USE_FLEXCAN0 */

#if (SPC5_CAN_USE_FLEXCAN1 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_FLEXCAN1_SHARED_IRQ == FALSE)
/**
 * @brief   CAN2 RX interrupt handler for MB 0.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_00_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND2);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN2 RX interrupt handler for MB 1.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_01_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND2);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN2 RX interrupt handler for MB 2.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_02_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND2);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN2 RX interrupt handler for MB 3.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_03_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND2);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN2 RX interrupt handler for MB 4.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_04_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND2);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN2 RX interrupt handler for MB 5.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_05_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND2);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN2 RX interrupt handler for MB 6.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_06_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND2);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN2 RX interrupt handler for MB 7.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_07_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND2);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN2 TX interrupt handler for MB 8.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_08_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND2);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN2 TX interrupt handler for MB 9.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_09_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND2);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN2 TX interrupt handler for MB 10.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_10_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND2);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN2 TX interrupt handler for MB 11.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_11_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND2);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN2 TX interrupt handler for MB 12.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_12_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND2);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN2 TX interrupt handler for MB 13.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_13_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND2);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN2 TX interrupt handler for MB 14.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_14_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND2);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN2 TX interrupt handler for MB 15.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_15_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND2);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN2 TX interrupt handler for MB 16-31.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_16_31_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND2);

  IRQ_EPILOGUE();
}

#if (SPC5_FLEXCAN1_MB == 64U)
/**
 * @brief   CAN2 TX interrupt handler for MB 32-63.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_32_63_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND2);

  IRQ_EPILOGUE();
}
#endif

/**
 * @brief   CAN2 ESR_ERR_INT interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN1_ESR_ERR_INT_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_err_handler(&CAND2);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN2 ESR_BOFF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN1_ESR_BOFF_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_err_handler(&CAND2);

  IRQ_EPILOGUE();
}
#else
/**
 * @brief   CAN2 TX interrupt handler for MB 8-11.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_08_11_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND2);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN2 TX interrupt handler for MB 12-15.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_12_15_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND2);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN2 TX interrupt handler for MB 16-31.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_16_31_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND2);

  IRQ_EPILOGUE();
}

#if (SPC5_FLEXCAN1_MB == 64U)
/**
 * @brief   CAN2 TX interrupt handler for MB 32-63.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_32_63_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND2);

  IRQ_EPILOGUE();
}
#endif

/*
 * @brief   CAN2 RX interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_00_03_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND2);

  IRQ_EPILOGUE();
}

/*
 * @brief   CAN2 RX interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN1_BUF_04_07_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND2);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN2 ESR_ERR_INT interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN1_ESR_ERR_INT_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_err_handler(&CAND2);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN2 ESR_BOFF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN1_ESR_BOFF_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_err_handler(&CAND2);

  IRQ_EPILOGUE();
}
#endif
#endif /* SPC5_CAN_USE_FLEXCAN1 */

#if (SPC5_CAN_USE_FLEXCAN2 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_FLEXCAN2_SHARED_IRQ == FALSE)
/**
 * @brief   CAN3 RX interrupt handler for MB 0.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_00_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND3);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN3 RX interrupt handler for MB 1.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_01_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND3);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN3 RX interrupt handler for MB 2.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_02_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND3);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN3 RX interrupt handler for MB 3.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_03_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND3);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN3 RX interrupt handler for MB 4.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_04_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND3);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN3 RX interrupt handler for MB 5.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_05_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND3);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN3 RX interrupt handler for MB 6.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_06_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND3);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN3 RX interrupt handler for MB 7.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_07_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND3);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN3 TX interrupt handler for MB 8.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_08_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND3);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN3 TX interrupt handler for MB 9.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_09_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND3);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN3 TX interrupt handler for MB 10.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_10_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND3);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN3 TX interrupt handler for MB 11.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_11_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND3);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN3 TX interrupt handler for MB 12.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_12_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND3);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN3 TX interrupt handler for MB 13.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_13_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND3);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN3 TX interrupt handler for MB 14.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_14_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND3);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN3 TX interrupt handler for MB 15.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_15_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND3);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN3 TX interrupt handler for MB 16-31.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_16_31_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND3);

  IRQ_EPILOGUE();
}

#if (SPC5_FLEXCAN2_MB == 64U)
/**
 * @brief   CAN3 TX interrupt handler for MB 32-63.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_32_63_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND3);

  IRQ_EPILOGUE();
}
#endif

/**
 * @brief   CAN3 ESR_ERR_INT interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN2_ESR_ERR_INT_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_err_handler(&CAND3);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN3 ESR_BOFF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN2_ESR_BOFF_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_err_handler(&CAND3);

  IRQ_EPILOGUE();
}
#else
/**
 * @brief   CAN3 TX interrupt handler for MB 8-11.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_08_11_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND3);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN3 TX interrupt handler for MB 12-15.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_12_15_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND3);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN3 TX interrupt handler for MB 16-31.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_16_31_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND3);

  IRQ_EPILOGUE();
}

#if (SPC5_FLEXCAN2_MB == 64U)
/**
 * @brief   CAN3 TX interrupt handler for MB 32-63.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_32_63_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND3);

  IRQ_EPILOGUE();
}
#endif

/*
 * @brief   CAN3 RX interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_00_03_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND3);

  IRQ_EPILOGUE();
}

/*
 * @brief   CAN3 RX interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN2_BUF_04_07_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND3);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN3 ESR_ERR_INT interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN2_ESR_ERR_INT_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_err_handler(&CAND3);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN3 ESR_BOFF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN2_ESR_BOFF_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_err_handler(&CAND3);

  IRQ_EPILOGUE();
}
#endif
#endif /* SPC5_CAN_USE_FLEXCAN2 */

#if (SPC5_CAN_USE_FLEXCAN3 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_FLEXCAN3_SHARED_IRQ == FALSE)
/**
 * @brief   CAN4 RX interrupt handler for MB 0.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_00_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND4);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN4 RX interrupt handler for MB 1.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_01_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND4);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN4 RX interrupt handler for MB 2.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_02_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND4);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN4 RX interrupt handler for MB 3.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_03_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND4);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN4 RX interrupt handler for MB 4.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_04_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND4);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN4 RX interrupt handler for MB 5.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_05_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND4);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN4 RX interrupt handler for MB 6.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_06_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND4);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN4 RX interrupt handler for MB 7.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_07_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND4);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN4 TX interrupt handler for MB 8.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_08_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND4);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN4 TX interrupt handler for MB 9.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_09_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND4);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN4 TX interrupt handler for MB 10.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_10_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND4);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN4 TX interrupt handler for MB 11.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_11_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND4);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN4 TX interrupt handler for MB 12.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_12_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND4);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN4 TX interrupt handler for MB 13.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_13_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND4);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN4 TX interrupt handler for MB 14.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_14_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND4);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN4 TX interrupt handler for MB 15.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_15_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND4);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN4 TX interrupt handler for MB 16-31.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_16_31_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND4);

  IRQ_EPILOGUE();
}

#if (SPC5_FLEXCAN3_MB == 64U)
/**
 * @brief   CAN4 TX interrupt handler for MB 32-63.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_32_63_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND4);

  IRQ_EPILOGUE();
}
#endif

/**
 * @brief   CAN4 ESR_ERR_INT interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN3_ESR_ERR_INT_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_err_handler(&CAND4);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN4 ESR_BOFF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN3_ESR_BOFF_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_err_handler(&CAND4);

  IRQ_EPILOGUE();
}
#else
/**
 * @brief   CAN4 TX interrupt handler for MB 8-11.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_08_11_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND4);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN4 TX interrupt handler for MB 12-15.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_12_15_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND4);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN4 TX interrupt handler for MB 16-31.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_16_31_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND4);

  IRQ_EPILOGUE();
}

#if (SPC5_FLEXCAN3_MB == 64U)
/**
 * @brief   CAN4 TX interrupt handler for MB 32-63.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_32_63_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND4);

  IRQ_EPILOGUE();
}
#endif

/*
 * @brief   CAN4 RX interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_00_03_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND4);

  IRQ_EPILOGUE();
}

/*
 * @brief   CAN4 RX interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN3_BUF_04_07_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND4);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN4 ESR_ERR_INT interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN3_ESR_ERR_INT_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_err_handler(&CAND4);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN4 ESR_BOFF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN3_ESR_BOFF_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_err_handler(&CAND4);

  IRQ_EPILOGUE();
}
#endif
#endif /* SPC5_CAN_USE_FLEXCAN3 */

#if (SPC5_CAN_USE_FLEXCAN4 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_FLEXCAN4_SHARED_IRQ == FALSE)
/**
 * @brief   CAN5 RX interrupt handler for MB 0.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_00_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND5);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN5 RX interrupt handler for MB 1.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_01_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND5);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN5 RX interrupt handler for MB 2.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_02_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND5);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN5 RX interrupt handler for MB 3.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_03_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND5);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN5 RX interrupt handler for MB 4.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_04_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND5);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN5 RX interrupt handler for MB 5.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_05_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND5);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN5 RX interrupt handler for MB 6.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_06_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND5);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN5 RX interrupt handler for MB 7.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_07_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND5);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN5 TX interrupt handler for MB 8.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_08_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND5);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN5 TX interrupt handler for MB 9.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_09_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND5);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN5 TX interrupt handler for MB 10.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_10_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND5);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN5 TX interrupt handler for MB 11.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_11_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND5);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN5 TX interrupt handler for MB 12.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_12_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND5);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN5 TX interrupt handler for MB 13.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_13_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND5);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN5 TX interrupt handler for MB 14.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_14_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND5);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN5 TX interrupt handler for MB 15.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_15_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND5);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN5 TX interrupt handler for MB 16-31.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_16_31_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND5);

  IRQ_EPILOGUE();
}

#if (SPC5_FLEXCAN4_MB == 64U)
/**
 * @brief   CAN5 TX interrupt handler for MB 32-63.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_32_63_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND5);

  IRQ_EPILOGUE();
}
#endif

/**
 * @brief   CAN5 ESR_ERR_INT interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN4_ESR_ERR_INT_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_err_handler(&CAND5);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN5 ESR_BOFF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN4_ESR_BOFF_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_err_handler(&CAND5);

  IRQ_EPILOGUE();
}
#else
/**
 * @brief   CAN5 TX interrupt handler for MB 8-11.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_08_11_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND5);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN5 TX interrupt handler for MB 12-15.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_12_15_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND5);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN5 TX interrupt handler for MB 16-31.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_16_31_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND5);

  IRQ_EPILOGUE();
}

#if (SPC5_FLEXCAN4_MB == 64U)
/**
 * @brief   CAN5 TX interrupt handler for MB 32-63.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_32_63_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND5);

  IRQ_EPILOGUE();
}
#endif

/*
 * @brief   CAN5 RX interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_00_03_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND5);

  IRQ_EPILOGUE();
}

/*
 * @brief   CAN5 RX interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN4_BUF_04_07_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND5);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN5 ESR_ERR_INT interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN4_ESR_ERR_INT_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_err_handler(&CAND5);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN5 ESR_BOFF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN4_ESR_BOFF_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_err_handler(&CAND5);

  IRQ_EPILOGUE();
}
#endif
#endif /* SPC5_CAN_USE_FLEXCAN4 */

#if (SPC5_CAN_USE_FLEXCAN5 == TRUE) || defined(__DOXYGEN__)
#if (SPC5_FLEXCAN5_SHARED_IRQ == FALSE)
/**
 * @brief   CAN6 RX interrupt handler for MB 0.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_00_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND6);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN6 RX interrupt handler for MB 1.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_01_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND6);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN6 RX interrupt handler for MB 2.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_02_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND6);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN6 RX interrupt handler for MB 3.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_03_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND6);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN6 RX interrupt handler for MB 4.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_04_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND6);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN6 RX interrupt handler for MB 5.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_05_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND6);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN6 RX interrupt handler for MB 6.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_06_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND6);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN6 RX interrupt handler for MB 7.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_07_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND6);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN6 TX interrupt handler for MB 8.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_08_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND6);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN6 TX interrupt handler for MB 9.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_09_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND6);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN6 TX interrupt handler for MB 10.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_10_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND6);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN6 TX interrupt handler for MB 11.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_11_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND6);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN6 TX interrupt handler for MB 12.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_12_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND6);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN6 TX interrupt handler for MB 13.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_13_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND6);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN6 TX interrupt handler for MB 14.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_14_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND6);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN6 TX interrupt handler for MB 15.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_15_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND6);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN6 TX interrupt handler for MB 16-31.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_16_31_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND6);

  IRQ_EPILOGUE();
}

#if (SPC5_FLEXCAN5_MB == 64U)
/**
 * @brief   CAN6 TX interrupt handler for MB 32-63.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_32_63_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND6);

  IRQ_EPILOGUE();
}
#endif

/**
 * @brief   CAN6 ESR_ERR_INT interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN5_ESR_ERR_INT_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_err_handler(&CAND6);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN6 ESR_BOFF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN5_ESR_BOFF_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_err_handler(&CAND6);

  IRQ_EPILOGUE();
}
#else
/**
 * @brief   CAN6 TX interrupt handler for MB 8-11.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_08_11_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND6);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN6 TX interrupt handler for MB 12-15.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_12_15_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND6);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN6 TX interrupt handler for MB 16-31.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_16_31_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND6);

  IRQ_EPILOGUE();
}

#if (SPC5_FLEXCAN5_MB == 64U)
/**
 * @brief   CAN6 TX interrupt handler for MB 32-63.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_32_63_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND6);

  IRQ_EPILOGUE();
}
#endif

/*
 * @brief   CAN6 RX interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_00_03_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND6);

  IRQ_EPILOGUE();
}

/*
 * @brief   CAN6 RX interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN5_BUF_04_07_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_handler(&CAND6);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN6 ESR_ERR_INT interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN5_ESR_ERR_INT_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_err_handler(&CAND6);

  IRQ_EPILOGUE();
}

/**
 * @brief   CAN6 ESR_BOFF interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FLEXCAN5_ESR_BOFF_HANDLER) {

  IRQ_PROLOGUE();

  can_lld_err_handler(&CAND6);

  IRQ_EPILOGUE();
}
#endif
#endif /* SPC5_CAN_USE_FLEXCAN5 */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level CAN driver initialization.
 *
 * @notapi
 */
void can_lld_init(void) {

#if (SPC5_CAN_USE_FLEXCAN0 == TRUE)
  /* Driver initialization.*/
  CAND1.config = NULL;
  CAND1.flexcan = &SPC5_FLEXCAN_0;
#if (SPC5_FLEXCAN0_SHARED_IRQ == FALSE)
  INTC_PSR(SPC5_FLEXCAN0_ESR_ERR_INT_NUMBER) =
  SPC5_CAN_FLEXCAN0_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN0_ESR_BOFF_NUMBER) =
  SPC5_CAN_FLEXCAN0_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN0_BUF_00_NUMBER) =
  SPC5_CAN_FLEXCAN0_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN0_BUF_01_NUMBER) =
  SPC5_CAN_FLEXCAN0_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN0_BUF_02_NUMBER) =
  SPC5_CAN_FLEXCAN0_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN0_BUF_03_NUMBER) =
  SPC5_CAN_FLEXCAN0_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN0_BUF_04_NUMBER) =
  SPC5_CAN_FLEXCAN0_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN0_BUF_05_NUMBER) =
  SPC5_CAN_FLEXCAN0_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN0_BUF_06_NUMBER) =
  SPC5_CAN_FLEXCAN0_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN0_BUF_07_NUMBER) =
  SPC5_CAN_FLEXCAN0_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN0_BUF_08_NUMBER) =
  SPC5_CAN_FLEXCAN0_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN0_BUF_09_NUMBER) =
  SPC5_CAN_FLEXCAN0_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN0_BUF_10_NUMBER) =
  SPC5_CAN_FLEXCAN0_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN0_BUF_11_NUMBER) =
  SPC5_CAN_FLEXCAN0_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN0_BUF_12_NUMBER) =
  SPC5_CAN_FLEXCAN0_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN0_BUF_13_NUMBER) =
  SPC5_CAN_FLEXCAN0_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN0_BUF_14_NUMBER) =
  SPC5_CAN_FLEXCAN0_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN0_BUF_15_NUMBER) =
  SPC5_CAN_FLEXCAN0_IRQ_PRIORITY;
#else
  INTC_PSR(SPC5_FLEXCAN0_ESR_ERR_INT_NUMBER) =
  SPC5_CAN_FLEXCAN0_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN0_ESR_BOFF_NUMBER) =
  SPC5_CAN_FLEXCAN0_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN0_BUF_00_03_NUMBER) =
  SPC5_CAN_FLEXCAN0_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN0_BUF_04_07_NUMBER) =
  SPC5_CAN_FLEXCAN0_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN0_BUF_08_11_NUMBER) =
  SPC5_CAN_FLEXCAN0_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN0_BUF_12_15_NUMBER) =
  SPC5_CAN_FLEXCAN0_IRQ_PRIORITY;
#endif
  INTC_PSR(SPC5_FLEXCAN0_BUF_16_31_NUMBER) =
  SPC5_CAN_FLEXCAN0_IRQ_PRIORITY;
#if (SPC5_FLEXCAN0_MB == 64U)
  INTC_PSR(SPC5_FLEXCAN0_BUF_32_63_NUMBER) =
  SPC5_CAN_FLEXCAN0_IRQ_PRIORITY;
#endif
#endif

#if (SPC5_CAN_USE_FLEXCAN1 == TRUE)
  /* Driver initialization.*/
  CAND2.config = NULL;
  CAND2.flexcan = &SPC5_FLEXCAN_1;
#if (SPC5_FLEXCAN1_SHARED_IRQ == FALSE)
  INTC_PSR(SPC5_FLEXCAN1_ESR_ERR_INT_NUMBER) =
  SPC5_CAN_FLEXCAN1_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN1_ESR_BOFF_NUMBER) =
  SPC5_CAN_FLEXCAN1_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN1_BUF_00_NUMBER) =
  SPC5_CAN_FLEXCAN1_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN1_BUF_01_NUMBER) =
  SPC5_CAN_FLEXCAN1_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN1_BUF_02_NUMBER) =
  SPC5_CAN_FLEXCAN1_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN1_BUF_03_NUMBER) =
  SPC5_CAN_FLEXCAN1_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN1_BUF_04_NUMBER) =
  SPC5_CAN_FLEXCAN1_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN1_BUF_05_NUMBER) =
  SPC5_CAN_FLEXCAN1_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN1_BUF_06_NUMBER) =
  SPC5_CAN_FLEXCAN1_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN1_BUF_07_NUMBER) =
  SPC5_CAN_FLEXCAN1_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN1_BUF_08_NUMBER) =
  SPC5_CAN_FLEXCAN1_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN1_BUF_09_NUMBER) =
  SPC5_CAN_FLEXCAN1_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN1_BUF_10_NUMBER) =
  SPC5_CAN_FLEXCAN1_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN1_BUF_11_NUMBER) =
  SPC5_CAN_FLEXCAN1_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN1_BUF_12_NUMBER) =
  SPC5_CAN_FLEXCAN1_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN1_BUF_13_NUMBER) =
  SPC5_CAN_FLEXCAN1_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN1_BUF_14_NUMBER) =
  SPC5_CAN_FLEXCAN1_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN1_BUF_15_NUMBER) =
  SPC5_CAN_FLEXCAN1_IRQ_PRIORITY;
#else
  INTC_PSR(SPC5_FLEXCAN1_ESR_ERR_INT_NUMBER) =
  SPC5_CAN_FLEXCAN1_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN1_ESR_BOFF_NUMBER) =
  SPC5_CAN_FLEXCAN1_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN1_BUF_00_03_NUMBER) =
  SPC5_CAN_FLEXCAN1_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN1_BUF_04_07_NUMBER) =
  SPC5_CAN_FLEXCAN1_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN1_BUF_08_11_NUMBER) =
  SPC5_CAN_FLEXCAN1_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN1_BUF_12_15_NUMBER) =
  SPC5_CAN_FLEXCAN1_IRQ_PRIORITY;
#endif
  INTC_PSR(SPC5_FLEXCAN1_BUF_16_31_NUMBER) =
  SPC5_CAN_FLEXCAN1_IRQ_PRIORITY;
#if (SPC5_FLEXCAN1_MB == 64U)
  INTC_PSR(SPC5_FLEXCAN1_BUF_32_63_NUMBER) =
  SPC5_CAN_FLEXCAN1_IRQ_PRIORITY;
#endif
#endif

#if (SPC5_CAN_USE_FLEXCAN2 == TRUE)
  /* Driver initialization.*/
  CAND3.config = NULL;
  CAND3.flexcan = &SPC5_FLEXCAN_2;
#if (SPC5_FLEXCAN2_SHARED_IRQ == FALSE)
  INTC_PSR(SPC5_FLEXCAN2_ESR_ERR_INT_NUMBER) =
  SPC5_CAN_FLEXCAN2_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN2_ESR_BOFF_NUMBER) =
  SPC5_CAN_FLEXCAN2_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN2_BUF_00_NUMBER) =
  SPC5_CAN_FLEXCAN2_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN2_BUF_01_NUMBER) =
  SPC5_CAN_FLEXCAN2_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN2_BUF_02_NUMBER) =
  SPC5_CAN_FLEXCAN2_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN2_BUF_03_NUMBER) =
  SPC5_CAN_FLEXCAN2_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN2_BUF_04_NUMBER) =
  SPC5_CAN_FLEXCAN2_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN2_BUF_05_NUMBER) =
  SPC5_CAN_FLEXCAN2_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN2_BUF_06_NUMBER) =
  SPC5_CAN_FLEXCAN2_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN2_BUF_07_NUMBER) =
  SPC5_CAN_FLEXCAN2_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN2_BUF_08_NUMBER) =
  SPC5_CAN_FLEXCAN2_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN2_BUF_09_NUMBER) =
  SPC5_CAN_FLEXCAN2_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN2_BUF_10_NUMBER) =
  SPC5_CAN_FLEXCAN2_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN2_BUF_11_NUMBER) =
  SPC5_CAN_FLEXCAN2_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN2_BUF_12_NUMBER) =
  SPC5_CAN_FLEXCAN2_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN2_BUF_13_NUMBER) =
  SPC5_CAN_FLEXCAN2_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN2_BUF_14_NUMBER) =
  SPC5_CAN_FLEXCAN2_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN2_BUF_15_NUMBER) =
  SPC5_CAN_FLEXCAN2_IRQ_PRIORITY;
#else
  INTC_PSR(SPC5_FLEXCAN2_ESR_ERR_INT_NUMBER) =
  SPC5_CAN_FLEXCAN2_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN2_ESR_BOFF_NUMBER) =
  SPC5_CAN_FLEXCAN2_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN2_BUF_00_03_NUMBER) =
  SPC5_CAN_FLEXCAN2_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN2_BUF_04_07_NUMBER) =
  SPC5_CAN_FLEXCAN2_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN2_BUF_08_11_NUMBER) =
  SPC5_CAN_FLEXCAN2_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN2_BUF_12_15_NUMBER) =
  SPC5_CAN_FLEXCAN2_IRQ_PRIORITY;
#endif
  INTC_PSR(SPC5_FLEXCAN2_BUF_16_31_NUMBER) =
  SPC5_CAN_FLEXCAN2_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN2_BUF_32_63_NUMBER) =
  SPC5_CAN_FLEXCAN2_IRQ_PRIORITY;
#endif

#if (SPC5_CAN_USE_FLEXCAN3 == TRUE)
  /* Driver initialization.*/
  CAND4.config = NULL;;
  CAND4.flexcan = &SPC5_FLEXCAN_3;
#if (SPC5_FLEXCAN3_SHARED_IRQ == FALSE)
  INTC_PSR(SPC5_FLEXCAN3_ESR_ERR_INT_NUMBER) =
  SPC5_CAN_FLEXCAN3_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN3_ESR_BOFF_NUMBER) =
  SPC5_CAN_FLEXCAN3_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN3_BUF_00_NUMBER) =
  SPC5_CAN_FLEXCAN3_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN3_BUF_01_NUMBER) =
  SPC5_CAN_FLEXCAN3_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN3_BUF_02_NUMBER) =
  SPC5_CAN_FLEXCAN3_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN3_BUF_03_NUMBER) =
  SPC5_CAN_FLEXCAN3_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN3_BUF_04_NUMBER) =
  SPC5_CAN_FLEXCAN3_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN3_BUF_05_NUMBER) =
  SPC5_CAN_FLEXCAN3_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN3_BUF_06_NUMBER) =
  SPC5_CAN_FLEXCAN3_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN3_BUF_07_NUMBER) =
  SPC5_CAN_FLEXCAN3_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN3_BUF_08_NUMBER) =
  SPC5_CAN_FLEXCAN3_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN3_BUF_09_NUMBER) =
  SPC5_CAN_FLEXCAN3_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN3_BUF_10_NUMBER) =
  SPC5_CAN_FLEXCAN3_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN3_BUF_11_NUMBER) =
  SPC5_CAN_FLEXCAN3_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN3_BUF_12_NUMBER) =
  SPC5_CAN_FLEXCAN3_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN3_BUF_13_NUMBER) =
  SPC5_CAN_FLEXCAN3_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN3_BUF_14_NUMBER) =
  SPC5_CAN_FLEXCAN3_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN3_BUF_15_NUMBER) =
  SPC5_CAN_FLEXCAN3_IRQ_PRIORITY;
#else
  INTC_PSR(SPC5_FLEXCAN3_ESR_ERR_INT_NUMBER) =
  SPC5_CAN_FLEXCAN3_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN3_ESR_BOFF_NUMBER) =
  SPC5_CAN_FLEXCAN3_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN3_BUF_00_03_NUMBER) =
  SPC5_CAN_FLEXCAN3_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN3_BUF_04_07_NUMBER) =
  SPC5_CAN_FLEXCAN3_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN3_BUF_08_11_NUMBER) =
  SPC5_CAN_FLEXCAN3_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN3_BUF_12_15_NUMBER) =
  SPC5_CAN_FLEXCAN3_IRQ_PRIORITY;
#endif
  INTC_PSR(SPC5_FLEXCAN3_BUF_16_31_NUMBER) =
  SPC5_CAN_FLEXCAN3_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN3_BUF_32_63_NUMBER) =
  SPC5_CAN_FLEXCAN3_IRQ_PRIORITY;
#endif

#if (SPC5_CAN_USE_FLEXCAN4 == TRUE)
  /* Driver initialization.*/
  CAND5.config = NULL;
  CAND5.flexcan = &SPC5_FLEXCAN_4;
#if (SPC5_FLEXCAN4_SHARED_IRQ == FALSE)
  INTC_PSR(SPC5_FLEXCAN4_ESR_ERR_INT_NUMBER) =
  SPC5_CAN_FLEXCAN4_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN4_ESR_BOFF_NUMBER) =
  SPC5_CAN_FLEXCAN4_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN4_BUF_00_NUMBER) =
  SPC5_CAN_FLEXCAN4_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN4_BUF_01_NUMBER) =
  SPC5_CAN_FLEXCAN4_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN4_BUF_02_NUMBER) =
  SPC5_CAN_FLEXCAN4_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN4_BUF_03_NUMBER) =
  SPC5_CAN_FLEXCAN4_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN4_BUF_04_NUMBER) =
  SPC5_CAN_FLEXCAN4_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN4_BUF_05_NUMBER) =
  SPC5_CAN_FLEXCAN4_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN4_BUF_06_NUMBER) =
  SPC5_CAN_FLEXCAN4_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN4_BUF_07_NUMBER) =
  SPC5_CAN_FLEXCAN4_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN4_BUF_08_NUMBER) =
  SPC5_CAN_FLEXCAN4_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN4_BUF_09_NUMBER) =
  SPC5_CAN_FLEXCAN4_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN4_BUF_10_NUMBER) =
  SPC5_CAN_FLEXCAN4_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN4_BUF_11_NUMBER) =
  SPC5_CAN_FLEXCAN4_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN4_BUF_12_NUMBER) =
  SPC5_CAN_FLEXCAN4_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN4_BUF_13_NUMBER) =
  SPC5_CAN_FLEXCAN4_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN4_BUF_14_NUMBER) =
  SPC5_CAN_FLEXCAN4_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN4_BUF_15_NUMBER) =
  SPC5_CAN_FLEXCAN4_IRQ_PRIORITY;
#else
  INTC_PSR(SPC5_FLEXCAN4_ESR_ERR_INT_NUMBER) =
  SPC5_CAN_FLEXCAN4_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN4_ESR_BOFF_NUMBER) =
  SPC5_CAN_FLEXCAN4_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN4_BUF_00_03_NUMBER) =
  SPC5_CAN_FLEXCAN4_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN4_BUF_04_07_NUMBER) =
  SPC5_CAN_FLEXCAN4_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN4_BUF_08_11_NUMBER) =
  SPC5_CAN_FLEXCAN4_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN4_BUF_12_15_NUMBER) =
  SPC5_CAN_FLEXCAN4_IRQ_PRIORITY;
#endif
  INTC_PSR(SPC5_FLEXCAN4_BUF_16_31_NUMBER) =
  SPC5_CAN_FLEXCAN4_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN4_BUF_32_63_NUMBER) =
  SPC5_CAN_FLEXCAN4_IRQ_PRIORITY;
#endif

#if (SPC5_CAN_USE_FLEXCAN5 == TRUE)
  /* Driver initialization.*/
  CAND6.config = NULL;
  CAND6.flexcan = &SPC5_FLEXCAN_5;
#if (SPC5_FLEXCAN5_SHARED_IRQ == FALSE)
  INTC_PSR(SPC5_FLEXCAN5_ESR_ERR_INT_NUMBER) =
  SPC5_CAN_FLEXCAN5_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN5_ESR_BOFF_NUMBER) =
  SPC5_CAN_FLEXCAN5_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN5_BUF_00_NUMBER) =
  SPC5_CAN_FLEXCAN5_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN5_BUF_01_NUMBER) =
  SPC5_CAN_FLEXCAN5_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN5_BUF_02_NUMBER) =
  SPC5_CAN_FLEXCAN5_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN5_BUF_03_NUMBER) =
  SPC5_CAN_FLEXCAN5_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN5_BUF_04_NUMBER) =
  SPC5_CAN_FLEXCAN5_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN5_BUF_05_NUMBER) =
  SPC5_CAN_FLEXCAN5_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN5_BUF_06_NUMBER) =
  SPC5_CAN_FLEXCAN5_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN5_BUF_07_NUMBER) =
  SPC5_CAN_FLEXCAN5_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN5_BUF_08_NUMBER) =
  SPC5_CAN_FLEXCAN5_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN5_BUF_09_NUMBER) =
  SPC5_CAN_FLEXCAN5_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN5_BUF_10_NUMBER) =
  SPC5_CAN_FLEXCAN5_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN5_BUF_11_NUMBER) =
  SPC5_CAN_FLEXCAN5_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN5_BUF_12_NUMBER) =
  SPC5_CAN_FLEXCAN5_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN5_BUF_13_NUMBER) =
  SPC5_CAN_FLEXCAN5_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN5_BUF_14_NUMBER) =
  SPC5_CAN_FLEXCAN5_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN5_BUF_15_NUMBER) =
  SPC5_CAN_FLEXCAN5_IRQ_PRIORITY;
#else
  INTC_PSR(SPC5_FLEXCAN5_ESR_ERR_INT_NUMBER) =
  SPC5_CAN_FLEXCAN5_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN5_ESR_BOFF_NUMBER) =
  SPC5_CAN_FLEXCAN5_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN5_BUF_00_03_NUMBER) =
  SPC5_CAN_FLEXCAN5_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN5_BUF_04_07_NUMBER) =
  SPC5_CAN_FLEXCAN5_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN5_BUF_08_11_NUMBER) =
  SPC5_CAN_FLEXCAN5_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN5_BUF_12_15_NUMBER) =
  SPC5_CAN_FLEXCAN5_IRQ_PRIORITY;
#endif
  INTC_PSR(SPC5_FLEXCAN5_BUF_16_31_NUMBER) =
  SPC5_CAN_FLEXCAN5_IRQ_PRIORITY;
  INTC_PSR(SPC5_FLEXCAN5_BUF_32_63_NUMBER) =
  SPC5_CAN_FLEXCAN5_IRQ_PRIORITY;
#endif
}

/**
 * @brief   Configures and activates the CAN peripheral.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[in] config	pointer to the @p CANconfig configuration
 *
 * @notapi
 */
void can_lld_start(CANDriver *canp, const CANConfig *config) {

  uint8_t mb_index = 0;
#if SPC5_CAN_USE_FILTERS
  uint8_t id;
#endif

  osalEnterCritical();

  canp->config = config;

#if defined(_SPC564Axx_)
  /* CAN Clock Divider setting.*/
  SIU.SYSDIV.B.CAN_SRC = (uint8_t)(SPC5_CANCLK_DIV_VALUE - 1U);
#endif

  /* Clock activation.*/
#if (SPC5_CAN_USE_FLEXCAN0 == TRUE)
  /* Set peripheral clock mode.*/
  if (&CAND1 == canp) {
    SPC5_FLEXCAN0_ENABLE_CLOCK();
#if (SPC5_CAN_FLEXCAN0_USE_EXT_CLK == FALSE)
    canp->flexcan->CR.R |= CAN_CTRL_CLK_SRC;
#endif
   /* Inactivate all mailboxes */
    for (mb_index = 0; mb_index < SPC5_FLEXCAN0_MB; mb_index++) {
      canp->flexcan->BUF[mb_index].CS.R = 0UL;
    }
  }
#endif
#if (SPC5_CAN_USE_FLEXCAN1 == TRUE)
  /* Set peripheral clock mode.*/
  if(&CAND2 == canp) {
    SPC5_FLEXCAN1_ENABLE_CLOCK();
#if (SPC5_CAN_FLEXCAN1_USE_EXT_CLK == FALSE)
    canp->flexcan->CR.R |= CAN_CTRL_CLK_SRC;
#endif
   /* Inactivate all mailboxes */
    for (mb_index = 0; mb_index < SPC5_FLEXCAN1_MB; mb_index++) {
      canp->flexcan->BUF[mb_index].CS.R = 0UL;
    }
  }
#endif
#if (SPC5_CAN_USE_FLEXCAN2 == TRUE)
  /* Set peripheral clock mode.*/
  if(&CAND3 == canp) {
    SPC5_FLEXCAN2_ENABLE_CLOCK();
#if (SPC5_CAN_FLEXCAN2_USE_EXT_CLK == FALSE)
    canp->flexcan->CR.R |= CAN_CTRL_CLK_SRC;
#endif
   /* Inactivate all mailboxes */
    for (mb_index = 0; mb_index < SPC5_FLEXCAN2_MB; mb_index++) {
      canp->flexcan->BUF[mb_index].CS.R = 0UL;
    }
  }
#endif
#if (SPC5_CAN_USE_FLEXCAN3 == TRUE)
  /* Set peripheral clock mode.*/
  if(&CAND4 == canp) {
    SPC5_FLEXCAN3_ENABLE_CLOCK();
#if (SPC5_CAN_FLEXCAN3_USE_EXT_CLK == FALSE)
    canp->flexcan->CR.R |= CAN_CTRL_CLK_SRC;
#endif
   /* Inactivate all mailboxes */
    for (mb_index = 0; mb_index < SPC5_FLEXCAN3_MB; mb_index++) {
      canp->flexcan->BUF[mb_index].CS.R = 0UL;
    }
  }
#endif
#if (SPC5_CAN_USE_FLEXCAN4 == TRUE)
  /* Set peripheral clock mode.*/
  if(&CAND5 == canp) {
    SPC5_FLEXCAN4_ENABLE_CLOCK();
#if (SPC5_CAN_FLEXCAN4_USE_EXT_CLK == FALSE)
    canp->flexcan->CR.R |= CAN_CTRL_CLK_SRC;
#endif
   /* Inactivate all mailboxes */
    for (mb_index = 0; mb_index < SPC5_FLEXCAN4_MB; mb_index++) {
      canp->flexcan->BUF[mb_index].CS.R = 0UL;
    }
  }
#endif
#if (SPC5_CAN_USE_FLEXCAN5== TRUE)
  /* Set peripheral clock mode.*/
  if(&CAND6 == canp) {
    SPC5_FLEXCAN5_ENABLE_CLOCK();
#if (SPC5_CAN_FLEXCAN5_USE_EXT_CLK == FALSE)
    canp->flexcan->CR.R |= CAN_CTRL_CLK_SRC;
#endif
   /* Inactivate all mailboxes */
    for (mb_index = 0; mb_index < SPC5_FLEXCAN5_MB; mb_index++) {
      canp->flexcan->BUF[mb_index].CS.R = 0UL;
    }
  }
#endif

  /* Enable the device.*/
  canp->flexcan->MCR.R &= ~CAN_MCR_MDIS;

  /*
   * Individual filtering per MB, disable frame self reception,
   * disable the FIFO, enable SuperVisor mode.
   */
#if (SPC5_CAN_USE_FLEXCAN0 == TRUE)
  if (&CAND1 == canp) {
    canp->flexcan->MCR.R |= CAN_MCR_SUPV | CAN_MCR_MAXMB(SPC5_FLEXCAN0_MB - 1U);
  }
#endif

#if (SPC5_CAN_USE_FLEXCAN1 == TRUE)
  if(&CAND2 == canp) {
    canp->flexcan->MCR.R |= CAN_MCR_SUPV | CAN_MCR_MAXMB(SPC5_FLEXCAN1_MB - 1U);
  }
#endif

#if (SPC5_CAN_USE_FLEXCAN2 == TRUE)
  if(&CAND3 == canp) {
    canp->flexcan->MCR.R |= CAN_MCR_SUPV | CAN_MCR_MAXMB(SPC5_FLEXCAN2_MB - 1U);
  }
#endif

#if (SPC5_CAN_USE_FLEXCAN3 == TRUE)
  if(&CAND4 == canp) {
    canp->flexcan->MCR.R |= CAN_MCR_SUPV | CAN_MCR_MAXMB(SPC5_FLEXCAN3_MB - 1U);
  }
#endif

#if (SPC5_CAN_USE_FLEXCAN4 == TRUE)
  if(&CAND5 == canp) {
    canp->flexcan->MCR.R |= CAN_MCR_SUPV | CAN_MCR_MAXMB(SPC5_FLEXCAN4_MB - 1U);
  }
#endif

#if (SPC5_CAN_USE_FLEXCAN5 == TRUE)
  if(&CAND6 == canp) {
    canp->flexcan->MCR.R |= CAN_MCR_SUPV | CAN_MCR_MAXMB(SPC5_FLEXCAN5_MB - 1U);
  }
#endif

  canp->flexcan->CR.R |= CAN_CTRL_TSYN | CAN_CTRL_RJW(3);

  /* TX MB initialization.*/
#if (SPC5_CAN_USE_FLEXCAN0 == TRUE)
  if (&CAND1 == canp) {
    for (mb_index = 0U; mb_index < CAN_TX_MAILBOXES; mb_index++) {
      canp->flexcan->BUF[mb_index + CAN_RX_MAILBOXES + canp->config->first_mb_available].CS.B.CODE = 8U;
    }
  }
#endif
#if (SPC5_CAN_USE_FLEXCAN1 == TRUE)
  if(&CAND2 == canp) {
    for(mb_index = 0U; mb_index < CAN_TX_MAILBOXES; mb_index++) {
      canp->flexcan->BUF[mb_index + CAN_RX_MAILBOXES + canp->config->first_mb_available].CS.B.CODE = 8U;
    }
  }
#endif
#if (SPC5_CAN_USE_FLEXCAN2 == TRUE)
  if(&CAND3 == canp) {
    for(mb_index = 0U; mb_index < CAN_TX_MAILBOXES; mb_index++) {
      canp->flexcan->BUF[mb_index + CAN_RX_MAILBOXES + canp->config->first_mb_available].CS.B.CODE = 8U;
    }
  }
#endif
#if (SPC5_CAN_USE_FLEXCAN3 == TRUE)
  if(&CAND4 == canp) {
    for(mb_index = 0U; mb_index < CAN_TX_MAILBOXES; mb_index++) {
      canp->flexcan->BUF[mb_index + CAN_RX_MAILBOXES + canp->config->first_mb_available].CS.B.CODE = 8U;
    }
  }
#endif
#if (SPC5_CAN_USE_FLEXCAN4 == TRUE)
  if(&CAND5 == canp) {
    for(mb_index = 0U; mb_index < CAN_TX_MAILBOXES; mb_index++) {
      canp->flexcan->BUF[mb_index + CAN_RX_MAILBOXES + canp->config->first_mb_available].CS.B.CODE = 8U;
    }
  }
#endif
#if (SPC5_CAN_USE_FLEXCAN5 == TRUE)
  if(&CAND6 == canp) {
    for(mb_index = 0U; mb_index < CAN_TX_MAILBOXES; mb_index++) {
      canp->flexcan->BUF[mb_index + CAN_RX_MAILBOXES + canp->config->first_mb_available].CS.B.CODE = 8U;
    }
  }
#endif

  /* Unlock Message buffers.*/
  (void)canp->flexcan->TIMER.R;

  /* MCR initialization.*/
  canp->flexcan->MCR.R |= canp->config->mcr;

  /*FIFO initialization */
  if (canp->config->use_fifo == TRUE) {
    canp->flexcan->MCR.B.FEN = 1U;
    /* Set Filtering Format Mode */
    canp->flexcan->MCR.B.IDAM = canp->config->id_acceptance_mask;
  }

  /* CTRL initialization.*/
  canp->flexcan->CR.R |= canp->config->ctrl;

  /* Interrupt sources initialization.*/
  canp->flexcan->MCR.R |= CAN_MCR_WRN_EN;

  canp->flexcan->CR.R |= CAN_CTRL_BOFF_MSK | CAN_CTRL_ERR_MSK |
  CAN_CTRL_TWRN_MSK | CAN_CTRL_RWRN_MSK;

#if (SPC5_CAN_USE_FILTERS == FALSE)
  /* RX MB initialization.*/
  for(mb_index = 0U; mb_index < CAN_RX_MAILBOXES; mb_index++) {
    canp->flexcan->BUF[mb_index + canp->config->first_mb_available].CS.B.CODE = 0U;
    if(mb_index < 4U) {
      canp->flexcan->BUF[mb_index+ + canp->config->first_mb_available].CS.B.IDE = 0U;
    }
    else {
      canp->flexcan->BUF[mb_index + canp->config->first_mb_available].CS.B.IDE = 1U;
    }
    canp->flexcan->BUF[mb_index + canp->config->first_mb_available].ID.R = 0U;
    canp->flexcan->BUF[mb_index + canp->config->first_mb_available].CS.B.CODE = 4U;
  }

  /* Receive all.*/
  canp->flexcan->RXGMASK.R = 0x00000000u;
#else
  for (id = 0U; id < CAN_RX_MAILBOXES; id++) {
    canp->flexcan->BUF[id + canp->config->first_mb_available].CS.B.CODE = 0U;
    if (canp->config->RxFilter[id].scale != 0U) {
      canp->flexcan->BUF[id + canp->config->first_mb_available].CS.B.IDE = 1U;
      canp->flexcan->BUF[id + canp->config->first_mb_available].ID.R = canp->config->RxFilter[id].register1;
    }
    else {
      canp->flexcan->BUF[id + canp->config->first_mb_available].CS.B.IDE = 0U;
      canp->flexcan->BUF[id + canp->config->first_mb_available].ID.B.STD_ID = (uint16_t)canp->config->RxFilter[id].register1;
      canp->flexcan->BUF[id + canp->config->first_mb_available].ID.B.EXT_ID = 0U;
    }
    /* RX MB initialization.*/
    canp->flexcan->BUF[id + canp->config->first_mb_available].CS.B.CODE = 4U;
  }
  canp->flexcan->RXGMASK.R = 0x0FFFFFFFU;
#endif
  /* write FIFO ID table */
  if (canp->config->use_fifo == TRUE) {
	uint8_t count;
    uint32_t address_offset;
    /* address of ID table 0 . Offset 0xE0*/
    address_offset = (uint32_t)(&canp->flexcan->BUF[0]) + 0x60UL;
    /* write ID table in FORMAT A mode */
    if (canp->config->id_acceptance_mask == CAN_RX_FIFO_FORMAT_A) {
      for (count = 0; count < SPC5_CAN_MAX_FILTERS; count++) {
        if (canp->config->id_filter[count].scale == CAN_IDE_EXT) {
          *(vuint32_t*)(address_offset) = ((uint32_t)canp->config->id_filter[count].scale << 30) + (canp->config->id_filter[count].register1 << 1);
        }
        else {
          *(vuint32_t*)(address_offset)  = (canp->config->id_filter[count].register1 << 19);
        }
        address_offset += 4UL;
      }
    }
  }
  /* Enable MBs interrupts.*/
#if (SPC5_CAN_USE_FLEXCAN0 == TRUE)
  if (&CAND1 == canp) {
#if(SPC5_FLEXCAN0_MB == 32U)
    canp->flexcan->IMRL.R = 0xFFFFFFFFU;
#endif
#if(SPC5_FLEXCAN0_MB == 64U)
    canp->flexcan->IMRL.R = 0xFFFFFFFFU;
    canp->flexcan->IMRH.R = 0xFFFFFFFFU;
#endif
  }
#endif
#if (SPC5_CAN_USE_FLEXCAN1 == TRUE)
  if(&CAND2 == canp) {
#if(SPC5_FLEXCAN1_MB == 32U)
    canp->flexcan->IMRL.R = 0xFFFFFFFFU;
#endif
#if(SPC5_FLEXCAN1_MB == 64U)
    canp->flexcan->IMRL.R = 0xFFFFFFFFU;
    canp->flexcan->IMRH.R = 0xFFFFFFFFU;
#endif
  }
#endif
#if (SPC5_CAN_USE_FLEXCAN2 == TRUE)
  if(&CAND3 == canp) {
#if (SPC5_FLEXCAN2_MB == 32U)
    canp->flexcan->IMRL.R = 0xFFFFFFFFU;
#endif
#if (SPC5_FLEXCAN2_MB == 64U)
    canp->flexcan->IMRL.R = 0xFFFFFFFFU;
    canp->flexcan->IMRH.R = 0xFFFFFFFFU;
#endif
  }
#endif
#if (SPC5_CAN_USE_FLEXCAN3 == TRUE)
  if(&CAND4 == canp) {
#if (SPC5_FLEXCAN3_MB == 32U)
    canp->flexcan->IMRL.R = 0xFFFFFFFFU;
#endif
#if (SPC5_FLEXCAN3_MB == 64U)
    canp->flexcan->IMRL.R = 0xFFFFFFFFU;
    canp->flexcan->IMRH.R = 0xFFFFFFFFU;
#endif
  }
#endif
#if (SPC5_CAN_USE_FLEXCAN4 == TRUE)
  if(&CAND5 == canp) {
#if (SPC5_FLEXCAN4_MB == 32U)
    canp->flexcan->IMRL.R = 0xFFFFFFFFU;
#endif
#if(SPC5_FLEXCAN4_MB == 64U)
    canp->flexcan->IMRL.R = 0xFFFFFFFFU;
    canp->flexcan->IMRH.R = 0xFFFFFFFFU;
#endif
  }
#endif
#if (SPC5_CAN_USE_FLEXCAN5 == TRUE)
  if(&CAND6 == canp) {
#if(SPC5_FLEXCAN5_MB == 32U)
    canp->flexcan->IMRL.R = 0xFFFFFFFFU;
#endif
#if(SPC5_FLEXCAN5_MB == 64U)
    canp->flexcan->IMRL.R = 0xFFFFFFFFU;
    canp->flexcan->IMRH.R = 0xFFFFFFFFU;
#endif
  }
#endif

  /* CAN BUS synchronization.*/
  canp->flexcan->MCR.B.HALT = 0U;

  osalExitCritical();
}

/**
 * @brief   Deactivates the CAN peripheral.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 *
 * @api
 */
void can_lld_stop(CANDriver *canp) {

  osalEnterCritical();

  /* Disable Interrupt sources.*/
  canp->flexcan->MCR.R &= ~CAN_MCR_WRN_EN;
  canp->flexcan->CR.R &= ~(CAN_CTRL_BOFF_MSK | CAN_CTRL_ERR_MSK |
  CAN_CTRL_TWRN_MSK | CAN_CTRL_RWRN_MSK);
  canp->flexcan->IMRL.R = 0x00000000U;
  
  /* Set clock source to its default value */
  canp->flexcan->CR.R &= ~(CAN_CTRL_CLK_SRC);
  
  /* Disable FLEXCAN module */
  canp->flexcan->MCR.R |= CAN_MCR_MDIS;

  while (canp->flexcan->MCR.B.LPMACK != 1U) {
  }

#if (SPC5_CAN_USE_FLEXCAN0 == TRUE)
  /* Set peripheral clock mode.*/
  if (&CAND1 == canp) {
    SPC5_FLEXCAN0_DISABLE_CLOCK();
  }
#endif
#if (SPC5_CAN_USE_FLEXCAN1 == TRUE)
  /* Set peripheral clock mode.*/
  if(&CAND2 == canp) {
    SPC5_FLEXCAN1_DISABLE_CLOCK();
  }
#endif
#if (SPC5_CAN_USE_FLEXCAN2 == TRUE)
  /* Set peripheral clock mode.*/
  if(&CAND3 == canp) {
    SPC5_FLEXCAN2_DISABLE_CLOCK();
  }
#endif
#if (SPC5_CAN_USE_FLEXCAN3 == TRUE)
  /* Set peripheral clock mode.*/
  if(&CAND4 == canp) {
    SPC5_FLEXCAN3_DISABLE_CLOCK();
  }
#endif
#if (SPC5_CAN_USE_FLEXCAN4 == TRUE)
  /* Set peripheral clock mode.*/
  if(&CAND5 == canp) {
    SPC5_FLEXCAN4_DISABLE_CLOCK();
  }
#endif
#if (SPC5_CAN_USE_FLEXCAN5 == TRUE)
  /* Set peripheral clock mode.*/
  if(&CAND6 == canp) {
    SPC5_FLEXCAN5_DISABLE_CLOCK();
  }
#endif
  osalExitCritical();

}

/**
 * @brief   Determines whether a frame can be transmitted.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[in] mailbox   mailbox number, @p CAN_ANY_MAILBOX for any mailbox
 *
 * @return              The queue space availability.
 * @retval FALSE        no space in the transmit queue.
 * @retval TRUE         transmit slot available.
 *
 * @api
 */
uint8_t can_lld_is_tx_empty(CANDriver *canp, canmbx_t mailbox) {

  uint8_t mbid = 0;

  if (mailbox == CAN_ANY_MAILBOX) {
#if (SPC5_CAN_USE_FLEXCAN0 == TRUE)
    if (&CAND1 == canp) {
      for (mbid = 0; mbid < CAN_TX_MAILBOXES; mbid++) {
        if (canp->flexcan->BUF[mbid + CAN_RX_MAILBOXES + canp->config->first_mb_available].CS.B.CODE == 0x08U) {
          return TRUE;
        }
      }
      return FALSE;
    }
#endif
#if (SPC5_CAN_USE_FLEXCAN1 == TRUE)
    if(&CAND2 == canp) {
      for (mbid = 0; mbid < CAN_TX_MAILBOXES; mbid++) {
        if (canp->flexcan->BUF[mbid + CAN_RX_MAILBOXES + canp->config->first_mb_available].CS.B.CODE == 0x08U) {
          return TRUE;
        }
      }
      return FALSE;
    }
#endif
#if (SPC5_CAN_USE_FLEXCAN2 == TRUE)
    if(&CAND3 == canp) {
      for (mbid = 0; mbid < CAN_TX_MAILBOXES; mbid++) {
        if (canp->flexcan->BUF[mbid + CAN_RX_MAILBOXES + canp->config->first_mb_available].CS.B.CODE == 0x08U) {
          return TRUE;
        }
      }
      return FALSE;
    }
#endif
#if (SPC5_CAN_USE_FLEXCAN3 == TRUE)
    if(&CAND4 == canp) {
      for (mbid = 0; mbid < CAN_TX_MAILBOXES; mbid++) {
        if (canp->flexcan->BUF[mbid + CAN_RX_MAILBOXES + canp->config->first_mb_available].CS.B.CODE == 0x08U) {
          return TRUE;
        }
      }
      return FALSE;
    }
#endif
#if (SPC5_CAN_USE_FLEXCAN4 == TRUE)
    if(&CAND5 == canp) {
      for (mbid = 0; mbid < CAN_TX_MAILBOXES; mbid++) {
        if (canp->flexcan->BUF[mbid + CAN_RX_MAILBOXES + canp->config->first_mb_available].CS.B.CODE == 0x08U) {
          return TRUE;
        }
      }
      return FALSE;
    }
#endif
#if (SPC5_CAN_USE_FLEXCAN5 == TRUE)
    if(&CAND6 == canp) {
      for (mbid = 0; mbid < CAN_TX_MAILBOXES; mbid++) {
        if (canp->flexcan->BUF[mbid + CAN_RX_MAILBOXES + canp->config->first_mb_available].CS.B.CODE == 0x08U) {
          return TRUE;
        }
      }
      return FALSE;
    }
#endif
  }
  else {
    if (canp->flexcan->BUF[mailbox + CAN_RX_MAILBOXES + canp->config->first_mb_available - 1U].CS.B.CODE == 0x08U) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * @brief   Inserts a frame into the transmit queue.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[in] ctfp      pointer to the CAN frame to be transmitted
 * @param[in] mailbox   mailbox number,  @p CAN_ANY_MAILBOX for any mailbox
 *
 * @return              operation result
 *
 * @api
 */
canmsg_t can_lld_transmit(CANDriver *canp, canmbx_t mailbox, const CANTxFrame *ctfp) {

  CAN_TxMailBox_TypeDef *tmbp = NULL;
  uint8_t mbid = 0;

  /*check if mailbox is non empty */
  if (can_lld_is_tx_empty(canp, mailbox) == FALSE) {
    return CAN_MSG_WAIT;
  }

  osalEnterCritical();
  /*allow Cast from different pointer*/
  /*lint -e9087 -e740 -e9005*/
  /* Pointer to a free transmission mailbox.*/
  if (mailbox == CAN_ANY_MAILBOX) {
#if (SPC5_CAN_USE_FLEXCAN0 == TRUE)
    if (&CAND1 == canp) {
      for (mbid = 0; mbid < CAN_TX_MAILBOXES; mbid++) {
        if (canp->flexcan->BUF[mbid + CAN_RX_MAILBOXES + canp->config->first_mb_available].CS.B.CODE == 8U) {
          tmbp = (CAN_TxMailBox_TypeDef *)&canp->flexcan->BUF[mbid + CAN_RX_MAILBOXES + canp->config->first_mb_available];
          break;
        }
      }
    }
#endif
#if (SPC5_CAN_USE_FLEXCAN1 == TRUE)
    if(&CAND2 == canp) {
      for (mbid = 0; mbid < CAN_TX_MAILBOXES; mbid++) {
        if (canp->flexcan->BUF[mbid + CAN_RX_MAILBOXES + canp->config->first_mb_available].CS.B.CODE == 8U) {
          tmbp = (CAN_TxMailBox_TypeDef *)&canp->flexcan->BUF[mbid + CAN_RX_MAILBOXES+ canp->config->first_mb_available];
          break;
        }
      }
    }
#endif
#if (SPC5_CAN_USE_FLEXCAN2 == TRUE)
    if(&CAND3 == canp) {
      for (mbid = 0; mbid < CAN_TX_MAILBOXES; mbid++) {
        if (canp->flexcan->BUF[mbid + CAN_RX_MAILBOXES + canp->config->first_mb_available].CS.B.CODE == 8U) {
          tmbp = (CAN_TxMailBox_TypeDef *)&canp->flexcan->BUF[mbid + CAN_RX_MAILBOXES + canp->config->first_mb_available];
          break;
        }
      }
    }
#endif
#if (SPC5_CAN_USE_FLEXCAN3 == TRUE)
    if(&CAND4 == canp) {
      for (mbid = 0; mbid < CAN_TX_MAILBOXES; mbid++) {
        if (canp->flexcan->BUF[mbid + CAN_RX_MAILBOXES + canp->config->first_mb_available].CS.B.CODE == 8U) {
          tmbp = (CAN_TxMailBox_TypeDef *)&canp->flexcan->BUF[mbid + CAN_RX_MAILBOXES + canp->config->first_mb_available];
          break;
        }
      }
    }
#endif
#if (SPC5_CAN_USE_FLEXCAN4 == TRUE)
    if(&CAND5 == canp) {
      for (mbid = 0; mbid < CAN_TX_MAILBOXES; mbid++) {
        if (canp->flexcan->BUF[mbid + CAN_RX_MAILBOXES + canp->config->first_mb_available].CS.B.CODE == 8U) {
          tmbp = (CAN_TxMailBox_TypeDef *)&canp->flexcan->BUF[mbid + CAN_RX_MAILBOXES + canp->config->first_mb_available];
          break;
        }
      }
    }
#endif
#if (SPC5_CAN_USE_FLEXCAN5 == TRUE)
    if(&CAND6 == canp) {
      for (mbid = 0; mbid < CAN_TX_MAILBOXES; mbid++) {
        if (canp->flexcan->BUF[mbid + CAN_RX_MAILBOXES + canp->config->first_mb_available].CS.B.CODE == 8U) {
          tmbp = (CAN_TxMailBox_TypeDef *)&canp->flexcan->BUF[mbid + CAN_RX_MAILBOXES + canp->config->first_mb_available];
          break;
        }
      }
    }
#endif
  }
  else {
    tmbp = (CAN_TxMailBox_TypeDef *)&canp->flexcan->BUF[mailbox + CAN_RX_MAILBOXES + canp->config->first_mb_available - 1U];
  }
  /*lint +e9087 +e740 +e9005*/
  /* Preparing the message.*/
  if (ctfp->IDE != 0U) {
    tmbp->CS.B.IDE = 1U;
    tmbp->CS.B.RTR = 0U;
    tmbp->ID.R = ctfp->EID;
  }
  else {
    tmbp->CS.B.IDE = 0U;
    tmbp->CS.B.RTR = 0U;
    tmbp->ID.R = (uint32_t)(ctfp->SID) << 18;
  }
  tmbp->CS.B.LENGTH = ctfp->LENGTH;
  tmbp->DATA[0] = ctfp->data32[0];
  tmbp->DATA[1] = ctfp->data32[1];
  tmbp->CS.B.CODE = 0x0CU;

  osalExitCritical();

  return CAN_MSG_OK;
}

/**
 * @brief   Determines whether a frame has been received.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[in] mailbox   mailbox number, @p CAN_ANY_MAILBOX for any mailbox
 *
 * @return              The queue space availability.
 * @retval FALSE        no space in the transmit queue.
 * @retval TRUE         transmit slot available.
 *
 * @api
 */

uint8_t can_lld_is_rx_nonempty(CANDriver *canp, canmbx_t mailbox) {

  uint8_t mbid = 0;
  uint8_t mb_status = FALSE;

  if (mailbox == CAN_ANY_MAILBOX) {
    for (mbid = 0; mbid < CAN_RX_MAILBOXES; mbid++) {
      if (canp->flexcan->BUF[mbid + canp->config->first_mb_available].CS.B.CODE == 2U) {
        mb_status = TRUE;
      }
    }
    return mb_status;
  }
  else if (mailbox > 0U && ((uint8_t)mailbox <= CAN_RX_MAILBOXES)) {
    if (canp->flexcan->BUF[mailbox + canp->config->first_mb_available - 1U].CS.B.CODE == 2U) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
  else {
    return FALSE;
  }
}

/**
 * @brief   Receives a frame from the input queue.
 *
 * @param[in] canp      pointer to the @p CANDriver object
 * @param[in] mailbox   mailbox number, @p CAN_ANY_MAILBOX for any mailbox
 * @param[out] crfp     pointer to the buffer where the CAN frame is copied
 *
 * @return              operation result
 *
 * @api
 */
canmsg_t can_lld_receive(CANDriver *canp, canmbx_t mailbox, CANRxFrame *crfp) {

  uint32_t mbid = 0, index = 0;

    if (can_lld_is_rx_nonempty(canp, mailbox) == FALSE) {
      return CAN_MSG_WAIT;
    }

    osalEnterCritical();

    if (mailbox != CAN_ANY_MAILBOX) {
      mbid = mailbox + canp->config->first_mb_available - 1U;
    }
    else {
      for (index = 0; index < CAN_RX_MAILBOXES; index++) {
        if (canp->flexcan->BUF[index + canp->config->first_mb_available].CS.B.CODE == 2U) {
          mbid = index + canp->config->first_mb_available;
          break;
        }
      }
    }

  /* Lock the RX MB.*/
  (void)canp->flexcan->BUF[mbid].CS.B.CODE;

  /* Fetches the message.*/
  crfp->data32[0] = canp->flexcan->BUF[mbid].DATA.W[0];
  crfp->data32[1] = canp->flexcan->BUF[mbid].DATA.W[1];

  /* Decodes the various fields in the RX frame.*/
  crfp->RTR = canp->flexcan->BUF[mbid].CS.B.RTR;
  crfp->IDE = canp->flexcan->BUF[mbid].CS.B.IDE;
  if (crfp->IDE != 0U) {
    crfp->EID = canp->flexcan->BUF[mbid].ID.R & 0x1FFFFFFFU;
  }
  else {
    crfp->SID = canp->flexcan->BUF[mbid].ID.B.STD_ID;
  }
  crfp->LENGTH = canp->flexcan->BUF[mbid].CS.B.LENGTH;
  crfp->TIME = canp->flexcan->BUF[mbid].CS.B.TIMESTAMP;

  /* Unlock the RX MB.*/
  (void)canp->flexcan->TIMER.R;

  /*Release Mailbox */
    canp->flexcan->BUF[mbid].CS.B.CODE = 4U;
    osalExitCritical();

  return CAN_MSG_OK;

}

#endif /* LLD_USE_CAN */

/** @} */
