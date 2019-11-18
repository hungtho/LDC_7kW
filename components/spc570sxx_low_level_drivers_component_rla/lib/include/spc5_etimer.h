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
 * @file    spc5_etimer.h
 * @brief   SPC5xx ICU-PWM low level drivers common header.
 *
 * @addtogroup SPC5xx_eTIMER
 * @{
 */

#ifndef _SPC5_ETIMER_H_
#define _SPC5_ETIMER_H_

#include "spc5_lld.h"
#include "lldconf.h"

#if (LLD_USE_ICU == TRUE) || (LLD_USE_PWM == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Mode options
 * @{
 */
#define SPC5_ETIMER_CNTMODE_NO_OPERATION    0U /* No operation */
#define SPC5_ETIMER_CNTMODE_RE              1U /* Count rising edges of primary source */
#define SPC5_ETIMER_CNTMODE_RFE             2U /* Count rising and falling edges of primary source */
#define SPC5_ETIMER_CNTMODE_RFE_SIHA        3U /* Count rising edges of primary source while secondary input high active */
#define SPC5_ETIMER_CNTMODE_QUADRATURE      4U /* Quadrature count mode, uses primary and secondary sources */
#define SPC5_ETIMER_CNTMODE_RE_SSSD         5U /* Count primary source rising edges, secondary source specifies direction */
#define SPC5_ETIMER_CNTMODE_ESS_TRIGGER     6U /* Edge of secondary source triggers primary count till compare */
#define SPC5_ETIMER_CNTMODE_CASCADE         7U /* Cascaded counter mode, up/down */

#define SPC5_ETIMER_OUTMODE_SC              0x0U /* Software controlled */
#define SPC5_ETIMER_OUTMODE_COC             0x1U /* Clear OFLAG output on successful compare (COMP1 or COMP2) */
#define SPC5_ETIMER_OUTMODE_SOC             0x2U /* Set OFLAG output on successful compare (COMP1 or COMP2) */
#define SPC5_ETIMER_OUTMODE_TOC             0x3U /* Toggle OFLAG output on successful compare (COMP1 or COMP2) */
#define SPC5_ETIMER_OUTMODE_TOAC            0x4U /* Toggle OFLAG output using alternating compare registers */
#define SPC5_ETIMER_OUTMODE_SC1CSS          0x5U /* Set on compare with COMP1, cleared on secondary source input edge */
#define SPC5_ETIMER_OUTMODE_SC2CSS          0x6U /* Set on compare with COMP2, cleared on secondary source input edge */
#define SPC5_ETIMER_OUTMODE_SCCCRO          0x7U /* Set on compare, cleared on counter roll-over */
#define SPC5_ETIMER_OUTMODE_SC1CC2          0x8U /* Set on successful compare on COMP1, clear on successful compare on COMP2 */
#define SPC5_ETIMER_OUTMODE_ACACCS          0x9U /* Asserted while counter is active, cleared when counter is stopped */
#define SPC5_ETIMER_OUTMODE_ACUCCD          0xAU /* Asserted when counting up, cleared when counting down */
#define SPC5_ETIMER_OUTMODE_EGCCA           0XFU /* Enable gated clock output while counter is active */

#define SPC5_ETIMER_PRISRC_DIV1             0x18U /* IP Bus clock divide by 1 prescaler */
#define SPC5_ETIMER_PRISRC_DIV2             0x19U /* IP Bus clock divide by 2 prescaler */
#define SPC5_ETIMER_PRISRC_DIV4             0x1AU /* IP Bus clock divide by 4 prescaler */
#define SPC5_ETIMER_PRISRC_DIV8             0x1BU /* IP Bus clock divide by 8 prescaler */
#define SPC5_ETIMER_PRISRC_DIV16            0x1CU /* IP Bus clock divide by 16 prescaler */
#define SPC5_ETIMER_PRISRC_DIV32            0x1DU /* IP Bus clock divide by 32 prescaler */
#define SPC5_ETIMER_PRISRC_DIV64            0x1EU /* IP Bus clock divide by 64 prescaler */
#define SPC5_ETIMER_PRISRC_DIV128           0x1FU /* IP Bus clock divide by 128 prescaler */

#define SPC5_ETIMER_CPT1MODE_DISABLED       0U /* Disabled */
#define SPC5_ETIMER_CPT1MODE_FALLING_EDGE   1U /* Capture falling edges */
#define SPC5_ETIMER_CPT1MODE_RISING_EDGE    2U /* Capture rising edges */
#define SPC5_ETIMER_CPT1MODE_ANY_EDGE       3U /* Capture any edge */

#define SPC5_ETIMER_CPT2MODE_DISABLED       0U /* Disabled */
#define SPC5_ETIMER_CPT2MODE_FALLING_EDGE   1U /* Capture falling edges */
#define SPC5_ETIMER_CPT2MODE_RISING_EDGE    2U /* Capture rising edges */
#define SPC5_ETIMER_CPT2MODE_ANY_EDGE       3U /* Capture any edge */

#define SPC5_ETIMER_ROC_DO_NOT_RELOAD       0U /* Do not reload the counter on a capture event */
#define SPC5_ETIMER_ROC_REL_ON_CAP1         1U /* Reload the counter on a capture 1 event */
#define SPC5_ETIMER_ROC_REL_ON_CAP2         2U /* Reload the counter on a capture 2 event */
#define SPC5_ETIMER_ROC_REL_ON_CAP1_CAP2    3U /* Reload the counter on both a capture 1 event and a capture 2 event */
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   ETIMER0 CH0 interrupt priority level setting.
 */
#if !defined(SPC5_ETIMER0_CH0_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_ETIMER0_CH0_PRIORITY           7
#endif

/**
 * @brief   ETIMER0 CH1 interrupt priority level setting.
 */
#if !defined(SPC5_ETIMER0_CH1_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_ETIMER0_CH1_PRIORITY           7
#endif

/**
 * @brief   ETIMER0 CH2 interrupt priority level setting.
 */
#if !defined(SPC5_ETIMER0_CH2_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_ETIMER0_CH2_PRIORITY           7
#endif

/**
 * @brief   ETIMER0 CH3 interrupt priority level setting.
 */
#if !defined(SPC5_ETIMER0_CH3_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_ETIMER0_CH3_PRIORITY           7
#endif

/**
 * @brief   ETIMER0 CH4 interrupt priority level setting.
 */
#if !defined(SPC5_ETIMER0_CH4_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_ETIMER0_CH4_PRIORITY           7
#endif

/**
 * @brief   ETIMER0 CH5 interrupt priority level setting.
 */
#if !defined(SPC5_ETIMER0_CH5_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_ETIMER0_CH5_PRIORITY           7
#endif

/**
 * @brief   eTIMER0 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_ETIMER0_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_ETIMER0_START_PCTL             (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   eTIMER0 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_ETIMER0_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_ETIMER0_STOP_PCTL              (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   ETIMER1 CH0 interrupt priority level setting.
 */
#if !defined(SPC5_ETIMER1_CH0_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_ETIMER1_CH0_PRIORITY           7
#endif

/**
 * @brief   ETIMER1 CH1 interrupt priority level setting.
 */
#if !defined(SPC5_ETIMER1_CH1_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_ETIMER1_CH1_PRIORITY           7
#endif

/**
 * @brief   ETIMER1 CH2 interrupt priority level setting.
 */
#if !defined(SPC5_ETIMER1_CH2_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_ETIMER1_CH2_PRIORITY           7
#endif

/**
 * @brief   ETIMER1 CH3 interrupt priority level setting.
 */
#if !defined(SPC5_ETIMER1_CH3_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_ETIMER1_CH3_PRIORITY           7
#endif

/**
 * @brief   ETIMER1 CH4 interrupt priority level setting.
 */
#if !defined(SPC5_ETIMER1_CH4_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_ETIMER1_CH4_PRIORITY           7
#endif

/**
 * @brief   ETIMER1 CH5 interrupt priority level setting.
 */
#if !defined(SPC5_ETIMER1_CH5_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_ETIMER1_CH5_PRIORITY           7
#endif

/**
 * @brief   eTIMER1 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_ETIMER1_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_ETIMER1_START_PCTL             (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   eTIMER1 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_ETIMER1_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_ETIMER1_STOP_PCTL              (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   ETIMER2 CH0 interrupt priority level setting.
 */
#if !defined(SPC5_ETIMER2_CH0_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_ETIMER2_CH0_PRIORITY           7
#endif

/**
 * @brief   ETIMER2 CH1 interrupt priority level setting.
 */
#if !defined(SPC5_ETIMER2_CH1_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_ETIMER2_CH1_PRIORITY           7
#endif

/**
 * @brief   ETIMER2 CH2 interrupt priority level setting.
 */
#if !defined(SPC5_ETIMER2_CH2_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_ETIMER2_CH2_PRIORITY           7
#endif

/**
 * @brief   ETIMER2 CH3 interrupt priority level setting.
 */
#if !defined(SPC5_ETIMER2_CH3_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_ETIMER2_CH3_PRIORITY           7
#endif

/**
 * @brief   ETIMER2 CH4 interrupt priority level setting.
 */
#if !defined(SPC5_ETIMER2_CH4_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_ETIMER2_CH4_PRIORITY           7
#endif

/**
 * @brief   ETIMER2 CH5 interrupt priority level setting.
 */
#if !defined(SPC5_ETIMER2_CH5_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_ETIMER2_CH5_PRIORITY           7
#endif

/**
 * @brief   eTIMER2 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_ETIMER2_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_ETIMER2_START_PCTL             (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   eTIMER2 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_ETIMER2_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_ETIMER2_STOP_PCTL              (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   ETIMER3 CH0 interrupt priority level setting.
 */
#if !defined(SPC5_ETIMER3_CH0_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_ETIMER3_CH0_PRIORITY           7
#endif

/**
 * @brief   ETIMER3 CH1 interrupt priority level setting.
 */
#if !defined(SPC5_ETIMER3_CH1_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_ETIMER3_CH1_PRIORITY           7
#endif

/**
 * @brief   ETIMER3 CH2 interrupt priority level setting.
 */
#if !defined(SPC5_ETIMER3_CH2_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_ETIMER3_CH2_PRIORITY           7
#endif

/**
 * @brief   ETIMER3 CH3 interrupt priority level setting.
 */
#if !defined(SPC5_ETIMER3_CH3_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_ETIMER3_CH3_PRIORITY           7
#endif

/**
 * @brief   ETIMER3 CH4 interrupt priority level setting.
 */
#if !defined(SPC5_ETIMER3_CH4_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_ETIMER3_CH4_PRIORITY           7
#endif

/**
 * @brief   ETIMER3 CH5 interrupt priority level setting.
 */
#if !defined(SPC5_ETIMER3_CH5_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_ETIMER3_CH5_PRIORITY           7
#endif

/**
 * @brief   eTIMER3 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_ETIMER3_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_ETIMER3_START_PCTL             (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   eTIMER3 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_ETIMER3_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_ETIMER3_STOP_PCTL              (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/* PPC register definitions not compatible with MISRA rules on bit-fields 
   (Rules 6.1). */
/*lint -e46 */
struct spc5_etimer {

  struct {
    union {
      vuint16_t R;
      struct {
        vuint16_t COMP1:16;
      } B;
    } COMP1; /* Compare Register 1 */

    union {
      vuint16_t R;
      struct {
        vuint16_t COMP2:16;
      } B;
    } COMP2; /* Compare Register 2 */

    union {
      vuint16_t R;
      struct {
        vuint16_t CAPT1:16;
      } B;
    } CAPT1; /* Capture Register 1 */

    union {
      vuint16_t R;
      struct {
        vuint16_t CAPT2:16;
      } B;
    } CAPT2; /* Capture Register 2 */

    union {
      vuint16_t R;
      struct {
        vuint16_t LOAD:16;
      } B;
    } LOAD; /* Load Register */

    union {
      vuint16_t R;
      struct {
        vuint16_t HOLD:16;
      } B;
    } HOLD; /* Hold Register */

    union {
      vuint16_t R;
      struct {
        vuint16_t CNTR:16;
      } B;
    } CNTR; /* Counter Register */

    union {
      vuint16_t R;
      struct {
        vuint16_t CNTMODE:3;
        vuint16_t PRISRC:5;
        vuint16_t ONCE:1;
        vuint16_t LENGTH:1;
        vuint16_t DIR:1;
        vuint16_t SECSRC:5;
      } B;
    } CTRL1; /* Control Register 1 */

    union {
      vuint16_t R;
      struct {
        vuint16_t OEN:1;
        vuint16_t RDNT:1;
        vuint16_t INPUT:1;
        vuint16_t VAL:1;
        vuint16_t FORCE:1;
        vuint16_t COFRC:1;
        vuint16_t COINIT:2;
        vuint16_t SIPS:1;
        vuint16_t PIPS:1;
        vuint16_t OPS:1;
        vuint16_t MSTR:1;
        vuint16_t OUTMODE:4;
      } B;
    } CTRL2; /* Control Register 2 */

    union {
      vuint16_t R;
      struct {
        vuint16_t STPEN:1;
        vuint16_t ROC:2;
        vuint16_t FMODE:1;
        vuint16_t FDIS:4;
        vuint16_t C2FCNT:3;
        vuint16_t C1FCNT:3;
        vuint16_t DBGEN:2;
      } B;
    } CTRL3; /* Control Register 3 */

    union {
      vuint16_t R;
      struct {
        vuint16_t unused_0:6;
        vuint16_t WDF:1;
        vuint16_t RCF:1;
        vuint16_t ICF2:1;
        vuint16_t ICF1:1;
        vuint16_t IEHF:1;
        vuint16_t IELF:1;
        vuint16_t TOF:1;
        vuint16_t TCF2:1;
        vuint16_t TCF1:1;
        vuint16_t TCF:1;
      } B;
    } STS; /* Status Register */

    union {
      vuint16_t R;
      struct {
        vuint16_t ICF2DE:1;
        vuint16_t ICF1DE:1;
        vuint16_t CMPLD2DE:1;
        vuint16_t CMPLD1DE:1;
        vuint16_t unused_0:2;
        vuint16_t WDFIE:1;
        vuint16_t RCFIE:1;
        vuint16_t ICF2IE:1;
        vuint16_t ICF1IE:1;
        vuint16_t IEHFIE:1;
        vuint16_t IELFIE:1;
        vuint16_t TOFIE:1;
        vuint16_t TCF2IE:1;
        vuint16_t TCF1IE:1;
        vuint16_t TCFIE:1;
      } B;
    } INTDMA; /* Interrupt and DMA Register */

    union {
      vuint16_t R;
      struct {
        vuint16_t CMPLD1:16;
      } B;
    } CMPLD1; /* Compare Load Register 1 */

    union {
      vuint16_t R;
      struct {
        vuint16_t CMPLD2:16;
      } B;
    } CMPLD2; /* Compare Load Register 2 */

    union {
      vuint16_t R;
      struct {
        vuint16_t CLC2:3;
        vuint16_t CLC1:3;
        vuint16_t CMPMODE:2;
        vuint16_t CPT2MODE:2;
        vuint16_t CPT1MODE:2;
        vuint16_t CFWM:2;
        vuint16_t ONESHOT:1;
        vuint16_t ARM:1;
      } B;
    } CCCTRL; /* Compare and Capture Control Register */

    union {
      vuint16_t R;
      struct {
        vuint16_t unused_0:5;
        vuint16_t FILT_CNT:3;
        vuint16_t FILT_PER:8;
      } B;
    } FILT; /* Input Filter Register */

  } CH[6];

  vuint8_t ADR_reserved0[64];

  union {
    vuint16_t R;
    struct {
      vuint16_t WDTOL:16;
    } B;
  } WDTOL; /* Watchdog Time-out Low Word Register */

  union {
    vuint16_t R;
    struct {
      vuint16_t WDTOH:16;
    } B;
  } WDTOH; /* Watchdog Time-out High Word Register */

  union {
    vuint16_t R;
    struct {
      vuint16_t unused_1:3;
      vuint16_t FTEST:1;
      vuint16_t unused_0:8;
      vuint16_t FLVL:4;
    } B;
  } FCTRL; /* Fault Control Register */

  union {
    vuint16_t R;
    struct {
      vuint16_t unused_1:4;
      vuint16_t FFPIN:4;
      vuint16_t unused_0:4;
      vuint16_t FFLAG:4;
    } B;
  } FSTS; /* Fault Status Register */

  union {
    vuint16_t R;
    struct {
      vuint16_t unused_0:5;
      vuint16_t FFILT_CNT:3;
      vuint16_t FFILT_PER:8;
    } B;
  } FFILT; /* Fault Filter Register */

  vuint8_t ADR_reserved1[2];

  union {
    vuint16_t R;
    struct {
      vuint16_t unused_0:10;
      vuint16_t ENBL:6;
    } B;
  } ENBL;  /* Channel Enable Register */

  vuint8_t ADR_reserved2[2];

  union {
    vuint16_t R;
    struct {
      vuint16_t DREQ0_EN:1;
      vuint16_t unused_0:10;
      vuint16_t DREQ0:5;
    } B;
  } DREQ0; /* DMA Request 0 Select Register */

  union {
    vuint16_t R;
    struct {
      vuint16_t DREQ1_EN:1;
      vuint16_t unused_0:10;
      vuint16_t DREQ1:5;
    } B;
  } DREQ1; /* DMA Request 1 Select Register */

};
/*lint +e46 */

/**
 * @brief   Type of a structure representing the ETIMER data.
 */
typedef struct ETIMERData ETIMERData;

/**
 * @brief   Structure representing the ETIMER data.
 */
struct ETIMERData {
  /**
   * @brief Number of eTIMERx active channels.
   */
  uint32_t                  etimer_active_channels;
  /**
   * @brief Pointer to the eTIMERx registers block.
   */
  volatile struct spc5_etimer *etimerp;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    eTIMER units references
 * @{
 */
#if (SPC5_HAS_ETIMER0 == TRUE) || defined(__DOXYGEN__)
#define SPC5_ETIMER_0       (*(volatile struct spc5_etimer *)SPC5_ETIMER0_BASE)
#endif

#if (SPC5_HAS_ETIMER1 == TRUE) || defined(__DOXYGEN__)
#define SPC5_ETIMER_1       (*(volatile struct spc5_etimer *)SPC5_ETIMER1_BASE)
#endif

#if (SPC5_HAS_ETIMER2 == TRUE) || defined(__DOXYGEN__)
#define SPC5_ETIMER_2       (*(volatile struct spc5_etimer *)SPC5_ETIMER2_BASE)
#endif

#if (SPC5_HAS_ETIMER3 == TRUE) || defined(__DOXYGEN__)
#define SPC5_ETIMER_3       (*(volatile struct spc5_etimer *)SPC5_ETIMER3_BASE)
#endif
/** @} */

/*===========================================================================*/
/* IRQ Handlers                                                              */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if (SPC5_HAS_ETIMER0 == TRUE) && !defined(__DOXYGEN__)
extern ETIMERData ETIMER0D;
#endif

#if (SPC5_HAS_ETIMER1 == TRUE) && !defined(__DOXYGEN__)
extern ETIMERData ETIMER1D;
#endif

#if (SPC5_HAS_ETIMER2 == TRUE) && !defined(__DOXYGEN__)
extern ETIMERData ETIMER2D;
#endif

#if (SPC5_HAS_ETIMER3 == TRUE) && !defined(__DOXYGEN__)
extern ETIMERData ETIMER3D;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void etimer_enable_clock(ETIMERData *etimerdp);
  void etimer_disable_clock(ETIMERData *etimerdp);
#ifdef __cplusplus
}
#endif

#endif /* LLD_USE_ICU || LLD_USE_PWM */

#endif /* _SPC5_ETIMER_H_ */

/** @} */
