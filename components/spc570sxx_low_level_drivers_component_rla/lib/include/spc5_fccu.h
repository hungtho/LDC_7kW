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
 * @file    spc5_fccu.h
 * @brief   FCCU helper driver header.
 *
 * @addtogroup FCCU
 * @{
 */

#ifndef _SPC5_FCCU_H_
#define _SPC5_FCCU_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/* PPC register definitions not compatible with MISRA rules on bit-fields 
 (Rules 6.1). */
/*lint -e46 */
struct spc5_fccu {

  union {
    vuint32_t R;
    struct {
      vuint32_t FILTER_BYPASS:1;
      vuint32_t FILTER_WIDTH:2;
      vuint32_t unused_2:19;
      vuint32_t DEBUG:1;
      vuint32_t unused_1:1;
      vuint32_t OPS:2;
      vuint32_t unused_0:1;
      vuint32_t OPR:5;
    } B;
  } CTRL;

  union {
    vuint32_t R;
    struct {
      vuint32_t CTRLK:32;
    } B;
  } CTRLK;

  union {
    vuint32_t R;
    struct {
      vuint32_t TGL:7; /*This field is present in SPC57Sxx*/
      vuint32_t FCCU_SET_AFTER_RESET:1;
      vuint32_t FCCU_SET_CLEAR:2;
      vuint32_t unused_1:6;
	  vuint32_t FOPE:1; /*This field is present in SPC57Sxx*/
	  vuint32_t FOME:2; /*This field is present in SPC57Sxx*/
      vuint32_t OD:1;
      vuint32_t unused_0:1;
      vuint32_t SM:1;
      vuint32_t PS:1;
      vuint32_t FOM:3;
      vuint32_t FOP:6;  /*This field is present in SPC57Sxx*/
    } B;
  } CFG;

  vuint8_t ADR_reserved0[16];

  union {
    vuint32_t R;
    struct {
      vuint32_t RFC31:1;
      vuint32_t RFC30:1;
      vuint32_t RFC29:1;
      vuint32_t RFC28:1;
      vuint32_t RFC27:1;
      vuint32_t RFC26:1;
      vuint32_t RFC25:1;
      vuint32_t RFC24:1;
      vuint32_t RFC23:1;
      vuint32_t RFC22:1;
      vuint32_t RFC21:1;
      vuint32_t RFC20:1;
      vuint32_t RFC19:1;
      vuint32_t RFC18:1;
      vuint32_t RFC17:1;
      vuint32_t RFC16:1;
      vuint32_t RFC15:1;
      vuint32_t RFC14:1;
      vuint32_t RFC13:1;
      vuint32_t RFC12:1;
      vuint32_t RFC11:1;
      vuint32_t RFC10:1;
      vuint32_t RFC9:1;
      vuint32_t RFC8:1;
      vuint32_t RFC7:1;
      vuint32_t RFC6:1;
      vuint32_t RFC5:1;
      vuint32_t RFC4:1;
      vuint32_t RFC3:1;
      vuint32_t RFC2:1;
      vuint32_t RFC1:1;
      vuint32_t RFC0:1;
    } B;
  } RF_CFG[4];

  vuint8_t ADR_reserved1[32];

  union {
    vuint32_t R;
    struct {
      vuint32_t RFSC15:2;
      vuint32_t RFSC14:2;
      vuint32_t RFSC13:2;
      vuint32_t RFSC12:2;
      vuint32_t RFSC11:2;
      vuint32_t RFSC10:2;
      vuint32_t RFSC9:2;
      vuint32_t RFSC8:2;
      vuint32_t RFSC7:2;
      vuint32_t RFSC6:2;
      vuint32_t RFSC5:2;
      vuint32_t RFSC4:2;
      vuint32_t RFSC3:2;
      vuint32_t RFSC2:2;
      vuint32_t RFSC1:2;
      vuint32_t RFSC0:2;
    } B;
  } RFS_CFG[8];

  vuint8_t ADR_reserved2[20];

  union {
    vuint32_t R;
    struct {
      vuint32_t RFS31:1;
      vuint32_t RFS30:1;
      vuint32_t RFS29:1;
      vuint32_t RFS28:1;
      vuint32_t RFS27:1;
      vuint32_t RFS26:1;
      vuint32_t RFS25:1;
      vuint32_t RFS24:1;
      vuint32_t RFS23:1;
      vuint32_t RFS22:1;
      vuint32_t RFS21:1;
      vuint32_t RFS20:1;
      vuint32_t RFS19:1;
      vuint32_t RFS18:1;
      vuint32_t RFS17:1;
      vuint32_t RFS16:1;
      vuint32_t RFS15:1;
      vuint32_t RFS14:1;
      vuint32_t RFS13:1;
      vuint32_t RFS12:1;
      vuint32_t RFS11:1;
      vuint32_t RFS10:1;
      vuint32_t RFS9:1;
      vuint32_t RFS8:1;
      vuint32_t RFS7:1;
      vuint32_t RFS6:1;
      vuint32_t RFS5:1;
      vuint32_t RFS4:1;
      vuint32_t RFS3:1;
      vuint32_t RFS2:1;
      vuint32_t RFS1:1;
      vuint32_t RFS0:1;
    } B;
  } RF_S[4];

  union {
    vuint32_t R;
    struct {
      vuint32_t RFK:32;
    } B;
  } RFK;

  union {
    vuint32_t R;
    struct {
      vuint32_t RFE31:1;
      vuint32_t RFE30:1;
      vuint32_t RFE29:1;
      vuint32_t RFE28:1;
      vuint32_t RFE27:1;
      vuint32_t RFE26:1;
      vuint32_t RFE25:1;
      vuint32_t RFE24:1;
      vuint32_t RFE23:1;
      vuint32_t RFE22:1;
      vuint32_t RFE21:1;
      vuint32_t RFE20:1;
      vuint32_t RFE19:1;
      vuint32_t RFE18:1;
      vuint32_t RFE17:1;
      vuint32_t RFE16:1;
      vuint32_t RFE15:1;
      vuint32_t RFE14:1;
      vuint32_t RFE13:1;
      vuint32_t RFE12:1;
      vuint32_t RFE11:1;
      vuint32_t RFE10:1;
      vuint32_t RFE9:1;
      vuint32_t RFE8:1;
      vuint32_t RFE7:1;
      vuint32_t RFE6:1;
      vuint32_t RFE5:1;
      vuint32_t RFE4:1;
      vuint32_t RFE3:1;
      vuint32_t RFE2:1;
      vuint32_t RFE1:1;
      vuint32_t RFE0:1;
    } B;
  } RF_E[4];

  union {
    vuint32_t R;
    struct {
      vuint32_t RFTOE31:1;
      vuint32_t RFTOE30:1;
      vuint32_t RFTOE29:1;
      vuint32_t RFTOE28:1;
      vuint32_t RFTOE27:1;
      vuint32_t RFTOE26:1;
      vuint32_t RFTOE25:1;
      vuint32_t RFTOE24:1;
      vuint32_t RFTOE23:1;
      vuint32_t RFTOE22:1;
      vuint32_t RFTOE21:1;
      vuint32_t RFTOE20:1;
      vuint32_t RFTOE19:1;
      vuint32_t RFTOE18:1;
      vuint32_t RFTOE17:1;
      vuint32_t RFTOE16:1;
      vuint32_t RFTOE15:1;
      vuint32_t RFTOE14:1;
      vuint32_t RFTOE13:1;
      vuint32_t RFTOE12:1;
      vuint32_t RFTOE11:1;
      vuint32_t RFTOE10:1;
      vuint32_t RFTOE9:1;
      vuint32_t RFTOE8:1;
      vuint32_t RFTOE7:1;
      vuint32_t RFTOE6:1;
      vuint32_t RFTOE5:1;
      vuint32_t RFTOE4:1;
      vuint32_t RFTOE3:1;
      vuint32_t RFTOE2:1;
      vuint32_t RFTOE1:1;
      vuint32_t RFTOE0:1;
    } B;
  } RF_TOE[4];

  union {
    vuint32_t R;
    struct {
      vuint32_t TO:32;
    } B;
  } RF_TO;

  union {
    vuint32_t R;
    struct {
      vuint32_t unused_0:29;
      vuint32_t TO:3;
    } B;
  } CFG_TO;

  union {
    vuint32_t R;
    struct {
      vuint32_t unused_1:26;
      vuint32_t EIN1:1;
      vuint32_t EIN0:1;
      vuint32_t unused_0:2;
      vuint32_t EOUT1:1;
      vuint32_t EOUT0:1;
    } B;
  } EINOUT;

  union {
    vuint32_t R;
    struct {
      vuint32_t unused_0:26;
      vuint32_t PHYSICERRORPIN:2;
      vuint32_t ESTAT:1;
      vuint32_t STATUS:3;
    } B;
  } STAT;

  union {
    vuint32_t R;
    struct {
      vuint32_t unused_0:24;
      vuint32_t NAFS:8;
    } B;
  } N2AF_STATUS;

  union {
    vuint32_t R;
    struct {
      vuint32_t unused_0:22;
      vuint32_t AF_SRC:2;
      vuint32_t AFFS:8;
    } B;
  } A2FF_STATUS;

  union {
    vuint32_t R;
    struct {
      vuint32_t unused_0:22;
      vuint32_t NF_SRC:2;
      vuint32_t NFFS:8;
    } B;
  } N2FF_STATUS;

  union {
    vuint32_t R;
    struct {
      vuint32_t unused_0:24;
      vuint32_t FAFS:8;
    } B;
  } F2A_STATUS;

  vuint8_t ADR_reserved3[8];

  union {
    vuint32_t R;
    struct {
      vuint32_t unused_0:25;
      vuint32_t FRFC:7;
    } B;
  } RFF;

  union {
    vuint32_t R;
    struct {
      vuint32_t unused_0:27;
	  vuint32_t DMA_REQ:1;   /*This field is present in SPC57Sxx*/
	  vuint32_t IRQ_EOUT:1;  /*This field is present in SPC57Sxx*/
      vuint32_t NMI_STAT:1;
      vuint32_t ALRM_STAT:1;
      vuint32_t CFG_TO_STAT:1;
    } B;
  } IRQ_STAT;

  union {
    vuint32_t R;
    struct {
      vuint32_t unused_1:28;
	  vuint32_t IRQ_EOUT_EN:1; /*This field is present in SPC57Sxx*/
      vuint32_t unused_0:2;
      vuint32_t CFG_TO_IEN:1;
    } B;
  } IRQ_EN;

  union {
    vuint32_t R;
    struct {
      vuint32_t XTMR:32;
    } B;
  } XTMR;

  union {
    vuint32_t R;
    struct {
      vuint32_t VL3:1;
      vuint32_t FS3:1;
      vuint32_t unused_3:2;
      vuint32_t MCS3:4;
      vuint32_t VL2:1;
      vuint32_t FS2:1;
      vuint32_t unused_2:2;
      vuint32_t MCS2:4;
      vuint32_t VL1:1;
      vuint32_t FS1:1;
      vuint32_t unused_1:2;
      vuint32_t MCS1:4;
      vuint32_t VL0:1;
      vuint32_t FS0:1;
      vuint32_t unused_0:2;
      vuint32_t MCS0:4;
    } B;
  } MCS;

  union {
    vuint32_t R;
    struct {
      vuint32_t unused_0:24;
      vuint32_t TRANSKEY:8;
    } B;
  } TRANS_LOCK;

  union {
    vuint32_t R;
    struct {
      vuint32_t unused_0:24;
      vuint32_t PERMNTKEY:8;
    } B;
  } PERMNT_LOCK;

  union {
    vuint32_t R;
    struct {
      vuint32_t unused_0:18;
      vuint32_t DELTA_T:14;
    } B;
  } DELTA_T;

  union {
    vuint32_t R;
    struct {
      vuint32_t IRQEN31:1;
      vuint32_t IRQEN30:1;
      vuint32_t IRQEN29:1;
      vuint32_t IRQEN28:1;
      vuint32_t IRQEN27:1;
      vuint32_t IRQEN26:1;
      vuint32_t IRQEN25:1;
      vuint32_t IRQEN24:1;
      vuint32_t IRQEN23:1;
      vuint32_t IRQEN22:1;
      vuint32_t IRQEN21:1;
      vuint32_t IRQEN20:1;
      vuint32_t IRQEN19:1;
      vuint32_t IRQEN18:1;
      vuint32_t IRQEN17:1;
      vuint32_t IRQEN16:1;
      vuint32_t IRQEN15:1;
      vuint32_t IRQEN14:1;
      vuint32_t IRQEN13:1;
      vuint32_t IRQEN12:1;
      vuint32_t IRQEN11:1;
      vuint32_t IRQEN10:1;
      vuint32_t IRQEN9:1;
      vuint32_t IRQEN8:1;
      vuint32_t IRQEN7:1;
      vuint32_t IRQEN6:1;
      vuint32_t IRQEN5:1;
      vuint32_t IRQEN4:1;
      vuint32_t IRQEN3:1;
      vuint32_t IRQEN2:1;
      vuint32_t IRQEN1:1;
      vuint32_t IRQEN0:1;
    } B;
  } IRQ_ALARM_EN[4];

  union {
    vuint32_t R;
    struct {
      vuint32_t NMIEN31:1;
      vuint32_t NMIEN30:1;
      vuint32_t NMIEN29:1;
      vuint32_t NMIEN28:1;
      vuint32_t NMIEN27:1;
      vuint32_t NMIEN26:1;
      vuint32_t NMIEN25:1;
      vuint32_t NMIEN24:1;
      vuint32_t NMIEN23:1;
      vuint32_t NMIEN22:1;
      vuint32_t NMIEN21:1;
      vuint32_t NMIEN20:1;
      vuint32_t NMIEN19:1;
      vuint32_t NMIEN18:1;
      vuint32_t NMIEN17:1;
      vuint32_t NMIEN16:1;
      vuint32_t NMIEN15:1;
      vuint32_t NMIEN14:1;
      vuint32_t NMIEN13:1;
      vuint32_t NMIEN12:1;
      vuint32_t NMIEN11:1;
      vuint32_t NMIEN10:1;
      vuint32_t NMIEN9:1;
      vuint32_t NMIEN8:1;
      vuint32_t NMIEN7:1;
      vuint32_t NMIEN6:1;
      vuint32_t NMIEN5:1;
      vuint32_t NMIEN4:1;
      vuint32_t NMIEN3:1;
      vuint32_t NMIEN2:1;
      vuint32_t NMIEN1:1;
      vuint32_t NMIEN0:1;
    } B;
  } NMI_EN[4];

  union {
    vuint32_t R;
    struct {
      vuint32_t EOUTEN31:1;
      vuint32_t EOUTEN30:1;
      vuint32_t EOUTEN29:1;
      vuint32_t EOUTEN28:1;
      vuint32_t EOUTEN27:1;
      vuint32_t EOUTEN26:1;
      vuint32_t EOUTEN25:1;
      vuint32_t EOUTEN24:1;
      vuint32_t EOUTEN23:1;
      vuint32_t EOUTEN22:1;
      vuint32_t EOUTEN21:1;
      vuint32_t EOUTEN20:1;
      vuint32_t EOUTEN19:1;
      vuint32_t EOUTEN18:1;
      vuint32_t EOUTEN17:1;
      vuint32_t EOUTEN16:1;
      vuint32_t EOUTEN15:1;
      vuint32_t EOUTEN14:1;
      vuint32_t EOUTEN13:1;
      vuint32_t EOUTEN12:1;
      vuint32_t EOUTEN11:1;
      vuint32_t EOUTEN10:1;
      vuint32_t EOUTEN9:1;
      vuint32_t EOUTEN8:1;
      vuint32_t EOUTEN7:1;
      vuint32_t EOUTEN6:1;
      vuint32_t EOUTEN5:1;
      vuint32_t EOUTEN4:1;
      vuint32_t EOUTEN3:1;
      vuint32_t EOUTEN2:1;
      vuint32_t EOUTEN1:1;
      vuint32_t EOUTEN0:1;
    } B;
  } EOUT_SIG_EN[4];

};
/*lint +e46 */

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/**
 * @name    FCCU unit reference
 * @{
 */
#define SPC5_FCCU       (*(struct spc5_fccu *)SPC5_FCCU_BASE)

/** @} */

#endif /* _SPC5_FCCU_H_ */

/** @} */
