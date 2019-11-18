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
 * @file    fccu_lld.c
 * @brief   SPC5xx FCCU low level driver code.
 *
 * @addtogroup FCCU
 * @{
 */

#include "fccu_lld.h"

#if (LLD_USE_FCCU == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   FCCU driver identifier.
 */
FCCUDriver FCCUD1;

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   FCCU transiently unlock.
 *
 * @param[in] fccudp    pointer to a @p FCCUDriver object
 *
 * @notapi
 */
static inline void fccu_lld_trans_unlock(FCCUDriver *fccudp) {

  fccudp->fccup->TRANS_LOCK.B.TRANSKEY = SPC5_FCCU_TRANSKEY_CONF_UNLOCK;
}

/**
 * @brief   FCCU operation status.
 *
 * @param[in] fccudp    pointer to a @p FCCUDriver object
 *
 * @return              operation status
 *
 * @notapi
 */
static inline uint8_t fccu_lld_get_op_status(FCCUDriver *fccudp) {

  return (uint8_t)(fccudp->fccup->CTRL.B.OPS);
}

/**
 * @brief   Set FCCU into CONFIG state.
 *
 * @param[in] fccudp    pointer to a @p FCCUDriver object
 *
 * @notapi
 */
static void fccu_lld_set_into_config(FCCUDriver *fccudp) {

  fccudp->fccup->CTRLK.R = SPC5_FCCU_CTRLK_OP1_KEY;
  fccudp->fccup->CTRL.R  = (uint32_t)SPC5_FCCU_CTRL_OPR_OP1;

  while(fccu_lld_get_op_status(fccudp) != SPC5_FCCU_CTRL_OPS_SUCCESS) {

  }
}

/**
 * @brief   Set FCCU into NORMAL state.
 *
 * @param[in] fccudp    pointer to a @p FCCUDriver object
 *
 * @notapi
 */
static void fccu_lld_set_into_normal(FCCUDriver *fccudp) {

  fccudp->fccup->CTRLK.R = SPC5_FCCU_CTRLK_OP2_KEY;
  fccudp->fccup->CTRL.R  = (uint32_t)SPC5_FCCU_CTRL_OPR_OP2;

  while(fccu_lld_get_op_status(fccudp) != SPC5_FCCU_CTRL_OPS_SUCCESS) {

  }
}

/**
 * @brief   Read FCCU Status.
 *
 * @param[in] fccudp    pointer to a @p FCCUDriver object
 *
 * @return              FCCU Status
 *
 * @notapi
 */
static inline uint8_t fccu_lld_get_status(FCCUDriver *fccudp) {

  fccudp->fccup->CTRL.R = (uint32_t)SPC5_FCCU_CTRL_OPR_OP3;

  while(fccu_lld_get_op_status(fccudp) != SPC5_FCCU_CTRL_OPS_SUCCESS) {

  }

  return (uint8_t)(fccudp->fccup->STAT.B.STATUS);
}

/**
 * @brief   Read FCCU RF Status register.
 *
 * @param[in] fccudp    pointer to a @p FCCUDriver object
 * @param[in] rf_reg    FCCU RF Status register number
 *
 * @return              FCCU RF Status register value
 *
 * @notapi
 */
static inline uint32_t fccu_lld_get_rf_status_reg(FCCUDriver *fccudp, uint8_t rf_reg) {

  fccudp->fccup->CTRL.R = (uint32_t)SPC5_FCCU_CTRL_OPR_OP10;

  while(fccu_lld_get_op_status(fccudp) != SPC5_FCCU_CTRL_OPS_SUCCESS) {

  }

  return (uint32_t)(fccudp->fccup->RF_S[rf_reg].R);
}

/**
 * @brief   Clear FCCU RF Status register bits.
 *
 * @param[in] fccudp    pointer to a @p FCCUDriver object
 * @param[in] rf_reg    FCCU RF Status register number
 * @param[in] value     FCCU RF Status register value
 *
 * @notapi
 */
static inline void fccu_lld_set_rf_status_reg(FCCUDriver *fccudp, uint8_t rf_reg, uint32_t value) {

  fccudp->fccup->RFK.R = SPC5_FCCU_RECOVERABLE_FAULT_KEY;
  fccudp->fccup->RF_S[rf_reg].R = (uint32_t)(value);

  while(fccu_lld_get_op_status(fccudp) != SPC5_FCCU_CTRL_OPS_SUCCESS) {

  }
}

#if (SPC5_FCCU_HAS_ERR_INPUT_PIN == TRUE)
/**
 * @brief   Read FCCU RF Status related to a specific channel.
 *
 * @param[in] fccudp    pointer to a @p FCCUDriver object
 * @param[in] rf_ch     FCCU RF channel
 *
 * @return              FCCU RF Status releated to the channel rf_ch
 *
 * @notapi
 */
static uint8_t fccu_lld_get_ch_status(FCCUDriver *fccudp, uint8_t rf_ch) {

  return (uint8_t)((fccu_lld_get_rf_status_reg(fccudp, (uint8_t)(rf_ch / 32U)) & (1UL << (rf_ch % 32U))) >> (rf_ch % 32U));
}
#endif /* SPC5_FCCU_HAS_ERR_INPUT_PIN */

/**
 * @brief   Check FCCU RF Enable registers are reset to the default values.
 *
 * @param[in] fccudp    pointer to a @p FCCUDriver object
 *
 * @return              TRUE if all FCCU RF Enable registers are reset,
 *                      FALSE otherwise
 *
 * @notapi
 */
static uint8_t fcc_lld_check_rf_reset(FCCUDriver *fccudp) {

  uint8_t i;
  uint32_t rf_e_reset_val[] = SPC5_FCCU_RF_E_REG_RESET_VAL;

  for(i = 0; i < (SPC5_FCCU_CH_NUMBER / 32U); i++) {
    if(fccudp->fccup->RF_E[i].R != rf_e_reset_val[i]) {
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * @brief   FCCU Alarm interrupt handler function.
 *
 * @param[in] fccudp    pointer to a @p FCCUDriver object
 *
 * @notapi
 */
static inline void fccu_serve_alarm_interrupt(FCCUDriver *fccudp) {

  uint8_t i, j;
  uint32_t rf_reg_value;
  uint32_t toe_reg_value;
  uint32_t iae_reg_value;

  osalEnterCriticalFromISR();

  for(i = 0; i < (SPC5_FCCU_CH_NUMBER / 32U); i++) {
    rf_reg_value = fccu_lld_get_rf_status_reg(fccudp, i);
    toe_reg_value = fccudp->fccup->RF_TOE[i].R;
    iae_reg_value = fccudp->fccup->IRQ_ALARM_EN[i].R;
    if(rf_reg_value != 0U) {
      for(j = 0; j < 32U; j++) {
        /* If a recovery fault occurs and the related time out is enabled and a
		   timeout callback function is defined, the callback is invoked.*/
        if(((0x1U & (rf_reg_value >> j)) == 1U)  &&
           ((0x1U & (toe_reg_value >> j)) == 1U) &&
           ((0x1U & (iae_reg_value >> j)) == 1U) &&
           (fccudp->config->channel[(i * 32U + j)]->alarm_irq_cb != NULL)) {
          fccudp->config->channel[(i * 32U + j)]->alarm_irq_cb(fccudp);
        }
      }
    }
  }

  osalExitCriticalFromISR();
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if !defined(SPC5_FCCU_ALARM_HANDLER)
#error "SPC5_FCCU_ALARM_HANDLER not defined"
#endif
/**
 * @brief   FCCU Alarm interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FCCU_ALARM_HANDLER) {

  IRQ_PROLOGUE();

  fccu_serve_alarm_interrupt(&FCCUD1);

  IRQ_EPILOGUE();
}

#if !defined(SPC5_FCCU_CONFIG_HANDLER)
#error "SPC5_FCCU_CONFIG_HANDLER not defined"
#endif
/**
 * @brief   FCCU Configuration interrupt handler.
 *
 * @isr
 */
IRQ_HANDLER(SPC5_FCCU_CONFIG_HANDLER) {

  FCCUDriver *fccudp = &FCCUD1;

  IRQ_PROLOGUE();

  /* If a configuration timeout callback is defined, it is invoked.*/
  if(fccudp->config->config_irq_cb != NULL) {
    fccudp->config->config_irq_cb(fccudp);
  }

  /* Clear the configuration timeout Status flag.*/
  fccudp->fccup->IRQ_STAT.B.CFG_TO_STAT = 1U;

  IRQ_EPILOGUE();
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level FCCU driver initialization.
 *
 * @init
 */
void fccu_lld_init(void) {

  FCCUD1.fccup = &SPC5_FCCU;
  INTC_PSR(SPC5_FCCU_CONFIG_INT_NUMBER) = SPC5_FCCU_CONFIG_IRQ_PRIO;
}

/**
 * @brief   Clear FCCU RF flags after a PowerOn reset.
 *
 * @param[in] fccudp    pointer to a @p FCCUDriver object
 *
 * @api
 */
void fccu_lld_clear(FCCUDriver *fccudp) {

  uint8_t i;

  /* FCCU transiently unlock.*/
  fccu_lld_trans_unlock(fccudp);

  do {
    /* Clear all FCCU RF Status registers.*/
    for(i = 0; i < (SPC5_FCCU_CH_NUMBER / 32U); i++) {
      fccu_lld_set_rf_status_reg(fccudp, i, 0xFFFFFFFFUL);
    }
  } while(fccu_lld_get_status(fccudp) != SPC5_FCCU_STATUS_NORMAL);
}

/**
 * @brief   Reset FCCU RF flags after a functional reset.
 *
 * @param[in] fccudp    pointer to a @p FCCUDriver object
 *
 * @api
 */
void fccu_lld_reset(FCCUDriver *fccudp) {

  uint8_t i;

  /* FCCU transiently unlock.*/
  fccu_lld_trans_unlock(fccudp);

  do {
#if (SPC5_FCCU_HAS_ERR_INPUT_PIN == TRUE)
    if(fccu_lld_get_status(fccudp) == SPC5_FCCU_STATUS_FAULT) {
      /* If an EIN_ERR fault occurs and the pin PB[11] is configured as EOUT, a
         loopback EOUT-EIN can be in place. So EOUT function has to be temporary
         disabled till the fault is raised (Tmin = 250us + SPC5_FCCU_MAX_DELTA_T_US).*/
      if(fccu_lld_get_ch_status(fccudp, SPC5_FCCU_EIN_ERR_CH) == 1U) {
        SIUL2.MSCR_IO[SPC5_FCCU_EIN_MSCR_IO].B.SSS = 0x0U;
      }

      osalThreadDelayMicroseconds(250UL + SPC5_FCCU_MAX_DELTA_T_US);
    }
#endif /*SPC5_FCCU_HAS_ERR_INPUT_PIN */
    /* Clear all FCCU RF Status registers.*/
    for(i = 0; i < (SPC5_FCCU_CH_NUMBER / 32U); i++) {
      fccu_lld_set_rf_status_reg(fccudp, i, 0xFFFFFFFFUL);
    }

    /* Reset FCCU through the RGM register.*/
    MC_RGM_FCCU_RST = 1;
    MC_RGM_FCCU_RST = 0;

    /* Verify RF Enable registers are reset after the FCCU reset.*/
    do {

    } while(fcc_lld_check_rf_reset(fccudp) != TRUE);

    /* FCCU transiently unlock.*/
    fccu_lld_trans_unlock(fccudp);

  } while(fccu_lld_get_status(fccudp) != SPC5_FCCU_STATUS_NORMAL);

#if (SPC5_FCCU_HAS_ERR_INPUT_PIN == TRUE)
  /* Set back the EOUT function.*/
  SIUL2.MSCR_IO[SPC5_FCCU_EIN_MSCR_IO].B.SSS = 0x5U;
#endif /* SPC5_FCCU_HAS_ERR_INPUT_PIN */
}

/**
 * @brief   Low level FCCU driver configuration and start.
 *
 * @param[in] fccudp    pointer to a @p FCCUDriver object
 * @param[in] config    the architecture-dependent FCCU driver configuration.
 *
 * @api
 */
void fccu_lld_start(FCCUDriver *fccudp, const FCCUConfig *config) {

  uint8_t i;
  uint8_t ch_reg;
  uint8_t ch_pos;

  /* Setup the FCCU configuration.*/
  fccudp->config = config;

  /* The ALARM IRQ is enabled in the fccu_lld_start function (instead of in the
     fccu_lld_init function) in order to avoid that after a functional reset the
     related callback function will be called before its initialization.*/
  INTC_PSR(SPC5_FCCU_ALARM_INT_NUMBER)  = SPC5_FCCU_ALARM_IRQ_PRIO;

  /* FCCU transiently unlock.*/
  fccu_lld_trans_unlock(fccudp);

  /* Setup configuration timeout.*/
  fccudp->fccup->CFG_TO.B.TO = (uint8_t)(fccudp->config->config_timeout);

  /* Check if enable the configuration timeout irq.*/
  if(fccudp->config->config_timeout_irq_en == TRUE) {
    fccudp->fccup->IRQ_EN.B.CFG_TO_IEN = 1U;
  }

  /* Set FCCU into CONFIG state.*/
  fccu_lld_set_into_config(fccudp);

  /* Check if the FCCU is set in CONFIG state.*/
  if(fccu_lld_get_status(fccudp) != SPC5_FCCU_STATUS_CONFIG) {
    SPC5_FCCU_ERROR_HOOK(fccudp);
  }

  /* Configure all the recoverable faults as SW recoverable faults.*/
  for(i = 0; i < (SPC5_FCCU_CH_NUMBER / 32U); i++) {
    fccudp->fccup->RF_CFG[i].R = 0xFFFFFFFFUL;
  }

  /* Setup recoverable fault state configuration.*/
  for(i = 0; i < SPC5_FCCU_CH_NUMBER; i++) {
    ch_reg = i / 16U;
    ch_pos = ((i % 16U) * 2U);
    if(fccudp->config->channel[i] != NULL) {
      fccudp->fccup->RFS_CFG[ch_reg].R = SPC5_FCCU_SET_REG_FIELD(fccudp->fccup->RFS_CFG[ch_reg].R, SPC5_FCCU_RFSC_MASK, ch_pos, (fccudp->config->channel[i]->int_reaction));
    } else {
      fccudp->fccup->RFS_CFG[ch_reg].R = SPC5_FCCU_SET_REG_FIELD(fccudp->fccup->RFS_CFG[ch_reg].R, SPC5_FCCU_RFSC_MASK, ch_pos, SPC5_FCCU_RFSC_NO_RESET);
    }
  }

  /* Enable recoverable faults.*/
  for(i = 0; i < SPC5_FCCU_CH_NUMBER; i++) {
    ch_reg = i / 32U;
    ch_pos = (i % 32U);
    if(fccudp->config->channel[i] != NULL) {
      fccudp->fccup->RF_E[ch_reg].R = SPC5_FCCU_SET_REG_FIELD(fccudp->fccup->RF_E[ch_reg].R, SPC5_FCCU_RFE_MASK, ch_pos, SPC5_FCCU_RFE_ENABLE);
    } else {
      fccudp->fccup->RF_E[ch_reg].R = SPC5_FCCU_SET_REG_FIELD(fccudp->fccup->RF_E[ch_reg].R, SPC5_FCCU_RFE_MASK, ch_pos, SPC5_FCCU_RFE_DISABLE);
    }
  }

  /* Enable timeout.*/
  for(i = 0; i < SPC5_FCCU_CH_NUMBER; i++) {
    ch_reg = i / 32U;
    ch_pos = (i % 32U);
    if((fccudp->config->channel[i] != NULL) && (fccudp->config->channel[i]->alarm_timeout_en == TRUE)) {
      fccudp->fccup->RF_TOE[ch_reg].R = SPC5_FCCU_SET_REG_FIELD(fccudp->fccup->RF_TOE[ch_reg].R, SPC5_FCCU_RFTOE_MASK, ch_pos, SPC5_FCCU_RFTOE_ENABLE);
    } else {
      fccudp->fccup->RF_TOE[ch_reg].R = SPC5_FCCU_SET_REG_FIELD(fccudp->fccup->RF_TOE[ch_reg].R, SPC5_FCCU_RFTOE_MASK, ch_pos, SPC5_FCCU_RFTOE_DISABLE);
    }
  }

  /* Set Alarm Timeout.*/
  fccudp->fccup->RF_TO.R = fccudp->config->alarm_timeout;

  /* Set delta_t value (Tmin = 250us + delta_t).*/
  fccudp->fccup->DELTA_T.R = fccudp->config->delta_t;

  /* Enable IRQ Alarm.*/
  for(i = 0; i < SPC5_FCCU_CH_NUMBER; i++) {
    ch_reg = i / 32U;
    ch_pos = (i % 32U);
    if((fccudp->config->channel[i] != NULL) && (fccudp->config->channel[i]->alarm_irq_en == TRUE)) {
      fccudp->fccup->IRQ_ALARM_EN[ch_reg].R = SPC5_FCCU_SET_REG_FIELD(fccudp->fccup->IRQ_ALARM_EN[ch_reg].R, SPC5_FCCU_IRQEN_MASK, ch_pos, SPC5_FCCU_IRQEN_ENABLE);
    } else {
      fccudp->fccup->IRQ_ALARM_EN[ch_reg].R = SPC5_FCCU_SET_REG_FIELD(fccudp->fccup->IRQ_ALARM_EN[ch_reg].R, SPC5_FCCU_IRQEN_MASK, ch_pos, SPC5_FCCU_IRQEN_DISABLE);
    }
  }

  /* Enable Non-Maskerable Interrupt.*/
  for(i = 0; i < SPC5_FCCU_CH_NUMBER; i++) {
    ch_reg = i / 32U;
    ch_pos = (i % 32U);
    if((fccudp->config->channel[i] != NULL) && (fccudp->config->channel[i]->nmi_en == TRUE)) {
      fccudp->fccup->NMI_EN[ch_reg].R = SPC5_FCCU_SET_REG_FIELD(fccudp->fccup->NMI_EN[ch_reg].R, SPC5_FCCU_NMIEN_MASK, ch_pos, SPC5_FCCU_NMIEN_ENABLE);
    } else {
      fccudp->fccup->NMI_EN[ch_reg].R = SPC5_FCCU_SET_REG_FIELD(fccudp->fccup->NMI_EN[ch_reg].R, SPC5_FCCU_NMIEN_MASK, ch_pos, SPC5_FCCU_NMIEN_DISABLE);
    }
  }

  /* Enable Error Out Signaling.*/
  for(i = 0; i < SPC5_FCCU_CH_NUMBER; i++) {
    ch_reg = i / 32U;
    ch_pos = (i % 32U);
    if((fccudp->config->channel[i] != NULL) && (fccudp->config->channel[i]->eout_en == TRUE)) {
      fccudp->fccup->EOUT_SIG_EN[ch_reg].R = SPC5_FCCU_SET_REG_FIELD(fccudp->fccup->EOUT_SIG_EN[ch_reg].R, SPC5_FCCU_EOUTEN_MASK, ch_pos, SPC5_FCCU_EOUTEN_ENABLE);
    } else {
      fccudp->fccup->EOUT_SIG_EN[ch_reg].R = SPC5_FCCU_SET_REG_FIELD(fccudp->fccup->EOUT_SIG_EN[ch_reg].R, SPC5_FCCU_EOUTEN_MASK, ch_pos, SPC5_FCCU_EOUTEN_DISABLE);
    }
  }

  /* Set the FCCU Configuration register fields.*/
  fccudp->fccup->CFG.B.FOM = fccudp->config->fom;
  fccudp->fccup->CFG.B.PS = fccudp->config->ps;
  fccudp->fccup->CFG.B.SM = fccudp->config->sm;
  fccudp->fccup->CFG.B.OD = fccudp->config->od;
  if(fccudp->config->eout_en_after_reset == TRUE) {
    /* Enable the EOUT error pin after reset.*/
    fccudp->fccup->CFG.B.FCCU_SET_AFTER_RESET = 0x1;
  }

  /* Set FCCU into NORMAL state.*/
  fccu_lld_set_into_normal(fccudp);

  /* Check if the FCCU is set in NORMAL state.*/
  if(fccu_lld_get_status(fccudp) != SPC5_FCCU_STATUS_NORMAL) {
    SPC5_FCCU_ERROR_HOOK(fccudp);
  }
}

/**
 * @brief   Return the FCCU status.
 *
 * @param[in] fccudp    pointer to a @p FCCUDriver object
 * @param[in] status    struct containing the FCCU state machine status and the
 *                      values of the RF Status registers
 *
 * @return              FALSE if no fault occur, TRUE otherwise
 *
 * @api
 */
uint8_t fccu_lld_status(FCCUDriver *fccudp, FCCUStatus *status) {

  uint8_t i;
  uint8_t fault = FALSE;

  status->fsm_status = fccu_lld_get_status(fccudp);

  for(i = 0; i < SPC5_FCCU_CH_NUMBER / 32U; i++) {
    status->rf_status_reg[i] = fccu_lld_get_rf_status_reg(fccudp, i);
    if(status->rf_status_reg[i] != 0UL) {
      fault = TRUE;
    }
  }

  return fault;
}

/**
 * @brief   Clear the FCCU RF fault related to the specified channel.
 *
 * @param[in] fccudp    pointer to a @p FCCUDriver object
 * @param[in] rf_ch     FCCU RF channel
 *
 * @api
 */
void fccu_lld_clear_fault(FCCUDriver *fccudp, uint8_t rf_ch) {

  /* FCCU transiently unlock.*/
  fccu_lld_trans_unlock(fccudp);

  do {
    /* Clear the FCCU RF fault.*/
    fccu_lld_set_rf_status_reg(fccudp, (rf_ch / 32U), (1UL << (rf_ch % 32U)));
  } while(fccu_lld_get_status(fccudp) != SPC5_FCCU_STATUS_NORMAL);
}

#endif /* LLD_USE_FCCU */

/** @} */
