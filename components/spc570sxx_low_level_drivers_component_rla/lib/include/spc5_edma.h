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
 * @file    spc5_edma.h
 * @brief   SPC5xx EDMA helper driver header for RLA components.
 *
 * @addtogroup EDMA
 * @{
 */

#ifndef _SPC5_EDMA_H_
#define _SPC5_EDMA_H_

#include "spc5_lld.h"
#include "lldconf.h"

#if (LLD_USE_DMA == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   EDMA channel allocation error.
 */
#define EDMA_ERROR                          0xFFFFFFFFUL

/**
 * @name    EDMA CR register definitions
 * @{
 */
#define EDMA_CR_CX                          (1UL << 17)
#define EDMA_CR_ECX                         (1UL << 16)
#define EDMA_CR_GRP3PRI_MASK                (3UL << 14)
#define EDMA_CR_GRP3PRI(n)                  ((uint32_t)(n) << 14)
#define EDMA_CR_GRP2PRI_MASK                (3UL << 12)
#define EDMA_CR_GRP2PRI(n)                  ((uint32_t)(n) << 12)
#define EDMA_CR_GRP1PRI_MASK                (3UL << 10)
#define EDMA_CR_GRP1PRI(n)                  ((uint32_t)(n) << 10)
#define EDMA_CR_GRP0PRI_MASK                (3UL << 8)
#define EDMA_CR_GRP0PRI(n)                  ((uint32_t)(n) << 8)
#define EDMA_CR_EMLM                        (1UL << 7)
#define EDMA_CR_CLM                         (1UL << 6)
#define EDMA_CR_HALT                        (1UL << 5)
#define EDMA_CR_HOE                         (1UL << 4)
#define EDMA_CR_ERGA                        (1UL << 3)
#define EDMA_CR_ERCA                        (1UL << 2)
#define EDMA_CR_EDBG                        (1UL << 1)
#define EDMA_CR_EBW                         (1UL << 0)
/** @} */

/**
 * @name    EDMA mode constants
 * @{
 */
#define EDMA_TCD_MODE_START                 (1UL << 0)
#define EDMA_TCD_MODE_INT_END               (1UL << 1)
#define EDMA_TCD_MODE_INT_HALF              (1UL << 2)
#define EDMA_TCD_MODE_DREQ                  (1UL << 3)
#define EDMA_TCD_MODE_SG                    (1UL << 4)
#define EDMA_TCD_MODE_MELINK                (1UL << 5)
#define EDMA_TCD_MODE_ACTIVE                (1UL << 6)
#define EDMA_TCD_MODE_DONE                  (1UL << 7)
#define EDMA_TCD_MODE_MLINKCH_MASK          (63UL << 8)
#define EDMA_TCD_MODE_MLINKCH(n)            ((uint32_t)(n) << 8)
#define EDMA_TCD_MODE_BWC_MASK              (3UL << 14)
#define EDMA_TCD_MODE_BWC(n)                ((uint32_t)(n) << 14)
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Default EDMA DMA1 CR register initialization.
 */
#if !defined(SPC5_EDMA_DMA0_CR_SETTING) || defined(__DOXYGEN__)
#define SPC5_EDMA_DMA0_CR_SETTING           (EDMA_CR_GRP3PRI(3) |           \
                                             EDMA_CR_GRP2PRI(2) |           \
                                             EDMA_CR_GRP1PRI(1) |           \
                                             EDMA_CR_GRP0PRI(0) |           \
                                             EDMA_CR_EMLM       |           \
                                             EDMA_CR_ERGA)
#endif

/**
 * @brief   Default EDMA DMA1 CR register initialization.
 */
#if !defined(SPC5_EDMA_DMA1_CR_SETTING) || defined(__DOXYGEN__)
#define SPC5_EDMA_DMA1_CR_SETTING           (EDMA_CR_GRP3PRI(3) |           \
                                             EDMA_CR_GRP2PRI(2) |           \
                                             EDMA_CR_GRP1PRI(1) |           \
                                             EDMA_CR_GRP0PRI(0) |           \
                                             EDMA_CR_EMLM       |           \
                                             EDMA_CR_ERGA)
#endif

/**
 * @brief   Static priorities for channels group 0 of DMA1.
 */
#if !defined(SPC5_EDMA1_GROUP0_PRIORITIES) || defined(__DOXYGEN__)
#define SPC5_EDMA1_GROUP0_PRIORITIES                                        \
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
#endif

/**
 * @brief   Static priorities for channels group 1 of DMA1.
 */
#if !defined(SPC5_EDMA1_GROUP1_PRIORITIES) || defined(__DOXYGEN__)
#define SPC5_EDMA1_GROUP1_PRIORITIES                                        \
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
#endif

/**
 * @brief   Static priorities for channels group 2 of DMA1.
 */
#if !defined(SPC5_EDMA1_GROUP2_PRIORITIES) || defined(__DOXYGEN__)
#define SPC5_EDMA1_GROUP2_PRIORITIES                                        \
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
#endif

/**
 * @brief   Static priorities for channels group 3 of DMA1.
 */
#if !defined(SPC5_EDMA1_GROUP3_PRIORITIES) || defined(__DOXYGEN__)
#define SPC5_EDMA1_GROUP3_PRIORITIES                                        \
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
#endif

/**
 * @brief   Static priorities for channels group 0 of DMA0.
 */
#if !defined(SPC5_EDMA0_GROUP0_PRIORITIES) || defined(__DOXYGEN__)
#define SPC5_EDMA0_GROUP0_PRIORITIES                                        \
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
#endif

/**
 * @brief   Static priorities for channels group 1 of DMA0.
 */
#if !defined(SPC5_EDMA0_GROUP1_PRIORITIES) || defined(__DOXYGEN__)
#define SPC5_EDMA0_GROUP1_PRIORITIES                                        \
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
#endif

/**
 * @brief   EDMA error handler IRQ priority.
 */
#if !defined(SPC5_EDMA_COMBO_CH_ERR_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_EDMA_COMBO_CH_ERR_IRQ_PRIO     15U
#endif

/**
 * @brief   EDMA peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_EDMA_MUX_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_EDMA_MUX_START_PCTL            (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   EDMA critical error handler, must not return.
 */
#if !defined(SPC5_EDMA_ERROR_HANDLER) || defined(__DOXYGEN__)
#define SPC5_EDMA_ERROR_HANDLER()           irqSysHalt()
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of eDMA channel number.
 */
typedef uint32_t edma_channel_t;

/**
 * @brief   Type of eDMA channel priority level.
 */
typedef uint16_t dma_irq_prio_t;

/**
 * @brief   Type of an eDMA TCD.
 */
typedef struct {
  union {
    uint32_t word[8];
  };
} edma_tcd_t;

/* PPC register definitions not compatible with MISRA rules on bit-fields 
   (Rules 6.1). */
/*lint -e46 */
/**
 * @brief   SPC5 eDMA registers block.
 * @note    Redefined from the SPC5 headers because the non uniform
 *          declaration of the SubModules registers among the various
 *          sub-families.
 */
typedef struct {
  union {
    vuint32_t R;
    struct {
      vuint32_t :14;
      vuint32_t CX:1;
      vuint32_t ECX:1;
      vuint32_t GRP3PRI:2;
      vuint32_t GRP2PRI:2;
      vuint32_t GRP1PRI:2;
      vuint32_t GRP0PRI:2;
      vuint32_t EMLM:1;
      vuint32_t CLM:1;
      vuint32_t HALT:1;
      vuint32_t HOE:1;
      vuint32_t ERGA:1;
      vuint32_t ERCA:1;
      vuint32_t EDBG:1;
      vuint32_t EBW:1; /* Only present in SPC574S */
    } B;
  } CR; /* DMA Control Register @baseaddress + 0x0 */

  union {
    vuint32_t R;
    struct {
      vuint32_t VLD:1;
      vuint32_t :13;
      vuint32_t UCE:1;
      vuint32_t ECX:1;
      vuint32_t GPE:1;
      vuint32_t CPE:1;
      vuint32_t ERRCHN:6;
      vuint32_t SAE:1;
      vuint32_t SOE:1;
      vuint32_t DAE:1;
      vuint32_t DOE:1;
      vuint32_t NCE:1;
      vuint32_t SGE:1;
      vuint32_t SBE:1;
      vuint32_t DBE:1;
    } B;
  } ES; /* Error Status Register @baseaddress + 0x4 */

  union {
    vuint32_t R;
    struct {
      vuint32_t ERQ63:1;
      vuint32_t ERQ62:1;
      vuint32_t ERQ61:1;
      vuint32_t ERQ60:1;
      vuint32_t ERQ59:1;
      vuint32_t ERQ58:1;
      vuint32_t ERQ57:1;
      vuint32_t ERQ56:1;
      vuint32_t ERQ55:1;
      vuint32_t ERQ54:1;
      vuint32_t ERQ53:1;
      vuint32_t ERQ52:1;
      vuint32_t ERQ51:1;
      vuint32_t ERQ50:1;
      vuint32_t ERQ49:1;
      vuint32_t ERQ48:1;
      vuint32_t ERQ47:1;
      vuint32_t ERQ46:1;
      vuint32_t ERQ45:1;
      vuint32_t ERQ44:1;
      vuint32_t ERQ43:1;
      vuint32_t ERQ42:1;
      vuint32_t ERQ41:1;
      vuint32_t ERQ40:1;
      vuint32_t ERQ39:1;
      vuint32_t ERQ38:1;
      vuint32_t ERQ37:1;
      vuint32_t ERQ36:1;
      vuint32_t ERQ35:1;
      vuint32_t ERQ34:1;
      vuint32_t ERQ33:1;
      vuint32_t ERQ32:1;
    } B;
  } ERQH; /* DMA Enable Request Register High @baseaddress + 0x8 */

  union {
    vuint32_t R;
    struct {
      vuint32_t ERQ31:1;
      vuint32_t ERQ30:1;
      vuint32_t ERQ29:1;
      vuint32_t ERQ28:1;
      vuint32_t ERQ27:1;
      vuint32_t ERQ26:1;
      vuint32_t ERQ25:1;
      vuint32_t ERQ24:1;
      vuint32_t ERQ23:1;
      vuint32_t ERQ22:1;
      vuint32_t ERQ21:1;
      vuint32_t ERQ20:1;
      vuint32_t ERQ19:1;
      vuint32_t ERQ18:1;
      vuint32_t ERQ17:1;
      vuint32_t ERQ16:1;
      vuint32_t ERQ15:1;
      vuint32_t ERQ14:1;
      vuint32_t ERQ13:1;
      vuint32_t ERQ12:1;
      vuint32_t ERQ11:1;
      vuint32_t ERQ10:1;
      vuint32_t ERQ9:1;
      vuint32_t ERQ8:1;
      vuint32_t ERQ7:1;
      vuint32_t ERQ6:1;
      vuint32_t ERQ5:1;
      vuint32_t ERQ4:1;
      vuint32_t ERQ3:1;
      vuint32_t ERQ2:1;
      vuint32_t ERQ1:1;
      vuint32_t ERQ0:1;
    } B;
  } ERQL; /* DMA Enable Request Register Low @baseaddress + 0xC */

  union {
    vuint32_t R;
    struct {
      vuint32_t EEI63:1;
      vuint32_t EEI62:1;
      vuint32_t EEI61:1;
      vuint32_t EEI60:1;
      vuint32_t EEI59:1;
      vuint32_t EEI58:1;
      vuint32_t EEI57:1;
      vuint32_t EEI56:1;
      vuint32_t EEI55:1;
      vuint32_t EEI54:1;
      vuint32_t EEI53:1;
      vuint32_t EEI52:1;
      vuint32_t EEI51:1;
      vuint32_t EEI50:1;
      vuint32_t EEI49:1;
      vuint32_t EEI48:1;
      vuint32_t EEI47:1;
      vuint32_t EEI46:1;
      vuint32_t EEI45:1;
      vuint32_t EEI44:1;
      vuint32_t EEI43:1;
      vuint32_t EEI42:1;
      vuint32_t EEI41:1;
      vuint32_t EEI40:1;
      vuint32_t EEI39:1;
      vuint32_t EEI38:1;
      vuint32_t EEI37:1;
      vuint32_t EEI36:1;
      vuint32_t EEI35:1;
      vuint32_t EEI34:1;
      vuint32_t EEI33:1;
      vuint32_t EEI32:1;
    } B;
  } EEIH; /* DMA Enable Error Interrupt Register High @baseaddress + 0x10 */

  union {
    vuint32_t R;
    struct {
      vuint32_t EEI31:1;
      vuint32_t EEI30:1;
      vuint32_t EEI29:1;
      vuint32_t EEI28:1;
      vuint32_t EEI27:1;
      vuint32_t EEI26:1;
      vuint32_t EEI25:1;
      vuint32_t EEI24:1;
      vuint32_t EEI23:1;
      vuint32_t EEI22:1;
      vuint32_t EEI21:1;
      vuint32_t EEI20:1;
      vuint32_t EEI19:1;
      vuint32_t EEI18:1;
      vuint32_t EEI17:1;
      vuint32_t EEI16:1;
      vuint32_t EEI15:1;
      vuint32_t EEI14:1;
      vuint32_t EEI13:1;
      vuint32_t EEI12:1;
      vuint32_t EEI11:1;
      vuint32_t EEI10:1;
      vuint32_t EEI9:1;
      vuint32_t EEI8:1;
      vuint32_t EEI7:1;
      vuint32_t EEI6:1;
      vuint32_t EEI5:1;
      vuint32_t EEI4:1;
      vuint32_t EEI3:1;
      vuint32_t EEI2:1;
      vuint32_t EEI1:1;
      vuint32_t EEI0:1;
    } B;
  } EEIL; /* DMA Enable Error Interrupt Register Low @baseaddress + 0x14 */

  union {
    vuint8_t R;
    struct {
      vuint8_t NOP:1;
      vuint8_t SAER:1;
      vuint8_t SERQ:6;
    } B;
  } SERQ; /* DMA Set Enable Request Register @baseaddress + 0x18 */

  union {
    vuint8_t R;
    struct {
      vuint8_t NOP:1;
      vuint8_t CAER:1;
      vuint8_t CERQ:6;
    } B;
  } CERQ; /* DMA Clear Enable Request Register @baseaddress + 0x19 */

  union {
    vuint8_t R;
    struct {
      vuint8_t NOP:1;
      vuint8_t SAEE:1;
      vuint8_t SEEI:6;
    } B;
  } SEEI; /* DMA Set Enable Error Interrupt Register @baseaddress + 0x1A */

  union {
    vuint8_t R;
    struct {
      vuint8_t NOP:1;
      vuint8_t CAEE:1;
      vuint8_t CEEI:6;
    } B;
  } CEEI; /* DMA Clear Enable Error Interrupt Register @baseaddress + 0x1B */

  union {
    vuint8_t R;
    struct {
      vuint8_t NOP:1;
      vuint8_t CAIR:1;
      vuint8_t CINT:6;
    } B;
  } CINT; /* DMA Clear Interrupt Request Register @baseaddress + 0x1C */

  union {
    vuint8_t R;
    struct {
      vuint8_t NOP:1;
      vuint8_t CAEI:1;
      vuint8_t CERR:6;
    } B;
  } CERR; /* DMA Clear error Register @baseaddress + 0x1D */

  union {
    vuint8_t R;
    struct {
      vuint8_t NOP:1;
      vuint8_t SAST:1;
      vuint8_t SSRT:6;
    } B;
  } SSRT; /* DMA Set Start Bit Register @baseaddress + 0x1E */

  union {
    vuint8_t R;
    struct {
      vuint8_t NOP:1;
      vuint8_t CADN:1;
      vuint8_t CDNE:6;
    } B;
  } CDNE; /* DMA Clear Done Status Bit Register @baseaddress + 0x1F */

  union {
    vuint32_t R;
    struct {
      vuint32_t INT63:1;
      vuint32_t INT62:1;
      vuint32_t INT61:1;
      vuint32_t INT60:1;
      vuint32_t INT59:1;
      vuint32_t INT58:1;
      vuint32_t INT57:1;
      vuint32_t INT56:1;
      vuint32_t INT55:1;
      vuint32_t INT54:1;
      vuint32_t INT53:1;
      vuint32_t INT52:1;
      vuint32_t INT51:1;
      vuint32_t INT50:1;
      vuint32_t INT49:1;
      vuint32_t INT48:1;
      vuint32_t INT47:1;
      vuint32_t INT46:1;
      vuint32_t INT45:1;
      vuint32_t INT44:1;
      vuint32_t INT43:1;
      vuint32_t INT42:1;
      vuint32_t INT41:1;
      vuint32_t INT40:1;
      vuint32_t INT39:1;
      vuint32_t INT38:1;
      vuint32_t INT37:1;
      vuint32_t INT36:1;
      vuint32_t INT35:1;
      vuint32_t INT34:1;
      vuint32_t INT33:1;
      vuint32_t INT32:1;
    } B;
  } INTH; /* DMA Interrupt Request High @baseaddress + 0x20 */

  union {
    vuint32_t R;
    struct {
      vuint32_t INT31:1;
      vuint32_t INT30:1;
      vuint32_t INT29:1;
      vuint32_t INT28:1;
      vuint32_t INT27:1;
      vuint32_t INT26:1;
      vuint32_t INT25:1;
      vuint32_t INT24:1;
      vuint32_t INT23:1;
      vuint32_t INT22:1;
      vuint32_t INT21:1;
      vuint32_t INT20:1;
      vuint32_t INT19:1;
      vuint32_t INT18:1;
      vuint32_t INT17:1;
      vuint32_t INT16:1;
      vuint32_t INT15:1;
      vuint32_t INT14:1;
      vuint32_t INT13:1;
      vuint32_t INT12:1;
      vuint32_t INT11:1;
      vuint32_t INT10:1;
      vuint32_t INT9:1;
      vuint32_t INT8:1;
      vuint32_t INT7:1;
      vuint32_t INT6:1;
      vuint32_t INT5:1;
      vuint32_t INT4:1;
      vuint32_t INT3:1;
      vuint32_t INT2:1;
      vuint32_t INT1:1;
      vuint32_t INT0:1;
    } B;
  } INTL; /* DMA Interrupt Request Low @baseaddress + 0x24 */

  union {
    vuint32_t R;
    struct {
      vuint32_t ERR63:1;
      vuint32_t ERR62:1;
      vuint32_t ERR61:1;
      vuint32_t ERR60:1;
      vuint32_t ERR59:1;
      vuint32_t ERR58:1;
      vuint32_t ERR57:1;
      vuint32_t ERR56:1;
      vuint32_t ERR55:1;
      vuint32_t ERR54:1;
      vuint32_t ERR53:1;
      vuint32_t ERR52:1;
      vuint32_t ERR51:1;
      vuint32_t ERR50:1;
      vuint32_t ERR49:1;
      vuint32_t ERR48:1;
      vuint32_t ERR47:1;
      vuint32_t ERR46:1;
      vuint32_t ERR45:1;
      vuint32_t ERR44:1;
      vuint32_t ERR43:1;
      vuint32_t ERR42:1;
      vuint32_t ERR41:1;
      vuint32_t ERR40:1;
      vuint32_t ERR39:1;
      vuint32_t ERR38:1;
      vuint32_t ERR37:1;
      vuint32_t ERR36:1;
      vuint32_t ERR35:1;
      vuint32_t ERR34:1;
      vuint32_t ERR33:1;
      vuint32_t ERR32:1;
    } B;
  } ERRH; /* DMA Error Register High @baseaddress + 0x28 */

  union {
    vuint32_t R;
    struct {
      vuint32_t ERR31:1;
      vuint32_t ERR30:1;
      vuint32_t ERR29:1;
      vuint32_t ERR28:1;
      vuint32_t ERR27:1;
      vuint32_t ERR26:1;
      vuint32_t ERR25:1;
      vuint32_t ERR24:1;
      vuint32_t ERR23:1;
      vuint32_t ERR22:1;
      vuint32_t ERR21:1;
      vuint32_t ERR20:1;
      vuint32_t ERR19:1;
      vuint32_t ERR18:1;
      vuint32_t ERR17:1;
      vuint32_t ERR16:1;
      vuint32_t ERR15:1;
      vuint32_t ERR14:1;
      vuint32_t ERR13:1;
      vuint32_t ERR12:1;
      vuint32_t ERR11:1;
      vuint32_t ERR10:1;
      vuint32_t ERR9:1;
      vuint32_t ERR8:1;
      vuint32_t ERR7:1;
      vuint32_t ERR6:1;
      vuint32_t ERR5:1;
      vuint32_t ERR4:1;
      vuint32_t ERR3:1;
      vuint32_t ERR2:1;
      vuint32_t ERR1:1;
      vuint32_t ERR0:1;
    } B;
  } ERRL; /* DMA Error Register Low @baseaddress + 0x2C */

  union {
    vuint32_t R;
    struct {
      vuint32_t HRS63:1;
      vuint32_t HRS62:1;
      vuint32_t HRS61:1;
      vuint32_t HRS60:1;
      vuint32_t HRS59:1;
      vuint32_t HRS58:1;
      vuint32_t HRS57:1;
      vuint32_t HRS56:1;
      vuint32_t HRS55:1;
      vuint32_t HRS54:1;
      vuint32_t HRS53:1;
      vuint32_t HRS52:1;
      vuint32_t HRS51:1;
      vuint32_t HRS50:1;
      vuint32_t HRS49:1;
      vuint32_t HRS48:1;
      vuint32_t HRS47:1;
      vuint32_t HRS46:1;
      vuint32_t HRS45:1;
      vuint32_t HRS44:1;
      vuint32_t HRS43:1;
      vuint32_t HRS42:1;
      vuint32_t HRS41:1;
      vuint32_t HRS40:1;
      vuint32_t HRS39:1;
      vuint32_t HRS38:1;
      vuint32_t HRS37:1;
      vuint32_t HRS36:1;
      vuint32_t HRS35:1;
      vuint32_t HRS34:1;
      vuint32_t HRS33:1;
      vuint32_t HRS32:1;
    } B;
  } HRSH; /* DMA Hardware Request Status Register High @baseaddress + 0x30 */

  union {
    vuint32_t R;
    struct {
      vuint32_t HRS31:1;
      vuint32_t HRS30:1;
      vuint32_t HRS29:1;
      vuint32_t HRS28:1;
      vuint32_t HRS27:1;
      vuint32_t HRS26:1;
      vuint32_t HRS25:1;
      vuint32_t HRS24:1;
      vuint32_t HRS23:1;
      vuint32_t HRS22:1;
      vuint32_t HRS21:1;
      vuint32_t HRS20:1;
      vuint32_t HRS19:1;
      vuint32_t HRS18:1;
      vuint32_t HRS17:1;
      vuint32_t HRS16:1;
      vuint32_t HRS15:1;
      vuint32_t HRS14:1;
      vuint32_t HRS13:1;
      vuint32_t HRS12:1;
      vuint32_t HRS11:1;
      vuint32_t HRS10:1;
      vuint32_t HRS9:1;
      vuint32_t HRS8:1;
      vuint32_t HRS7:1;
      vuint32_t HRS6:1;
      vuint32_t HRS5:1;
      vuint32_t HRS4:1;
      vuint32_t HRS3:1;
      vuint32_t HRS2:1;
      vuint32_t HRS1:1;
      vuint32_t HRS0:1;
    } B;
  } HRSL; /* DMA Hardware Request Status Register Low @baseaddress + 0x34 */

  vuint8_t eDMA_reserved0[200]; /* 0x0038-0x00FF */

  union {
    vuint8_t R;
    struct {
      vuint8_t ECP:1;
      vuint8_t DPA:1;
      vuint8_t GRPPRI:2;
      vuint8_t CHPRI:4;
    } B;
  } DCHPRI[64]; /* DMA Channel n Priority Register @baseaddress + 0x100 */

  union {
    vuint8_t R;
    struct {
      vuint8_t EMI:1;
      vuint8_t PAL:1;
      vuint8_t unused_0:2;
      vuint8_t MID:4;
    } B;
  } DCHMID[64]; /* DMA Channel n Master ID Register @baseaddress + 0x140 */

  vuint8_t eDMA_reserved1[3712]; /* 0x0180-0x0FFF */

  edma_tcd_t TCD[64];
} spc5_edma_t;

/**
 * @brief   Type of a DMA-MUX peripheral.
 */
typedef struct {
  union {
    vuint8_t R;
    struct {
      vuint8_t ENBL:1;
      vuint8_t TRIG:1;
      vuint8_t SOURCE:6;
    } B;
  } CHCONFIG[16];
} spc5_dmamux_t;
/*lint +e46 */

/**
 * @brief   DMA callback type.
 *
 * @param[in] channel   the channel number
 * @param[in] p         parameter for the registered function
 */
typedef void (*edma_callback_t)(edma_channel_t channel, void *p);

/**
 * @brief   DMA error callback type.
 *
 * @param[in] channel   the channel number
 * @param[in] p         parameter for the registered function
 * @param[in] esr       content of the ESR register
 */
typedef void (*edma_error_callback_t)(edma_channel_t channel,
                                      void *p,
                                      uint32_t esr);

/**
 * @brief   Type of an EDMA channel configuration structure.
 */
typedef struct {
  edma_channel_t        dma_channel;    /**< @brief Channel to be allocated.*/
  uint8_t               dma_periph;     /**< @brief Peripheral to be
                                             associated to the channel.     */
#if (SPC5_EDMA_NUM_OF_MUX > 1U) || defined(__DOXYGEN__)
  uint8_t               dma_mux;        /**< @brief Mux associated to the
                                             peripheral.                    */
#endif /* (SPC5_EDMA_NUM_OF_MUX > 1U) */
  dma_irq_prio_t        dma_irq_prio;   /**< @brief IRQ priority level for
                                             this channel.                  */
  edma_callback_t       dma_func;       /**< @brief Channel callback,
                                             can be NULL if not required.   */
  edma_error_callback_t dma_error_func; /**< @brief Channel error callback,
                                             can be NULL if not required.   */
  void                  *dma_param;     /**< @brief Channel callback param. */
} edma_channel_config_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Returns the TCD address associated to a channel.
 *
 * @param[in] channel   the channel number
 * @return              A pointer to an @p edma_tcd_t structure.
 *
 * @api
 */
#if (SPC5_EDMA_NCHANNELS <= 64U)
#define edmaGetTCD(channel) ((edma_tcd_t *)&SPC5_EDMA1.TCD[channel])
#else
#define edmaGetTCD(channel) ((channel < 64U)? ((edma_tcd_t *)&SPC5_EDMA1.TCD[channel]) : ((edma_tcd_t *)&SPC5_EDMA0.TCD[channel - 64U]))
#endif /* SPC5_EDMA_NCHANNELS <= 64U */

/**
 * @brief   Sets the word 0 fields into a TCD.
 *
 * @param[in] tcdp      pointer to an @p edma_tcd_t structure
 * @param[in] src       the source address
 *
 * @api
 */
#define edmaTCDSetWord0(tcdp, src)                                          \
  ((tcdp)->word[0] = (uint32_t)(src))

/**
 * @brief   Sets the word 1 fields into a TCD.
 *
 * @param[in] tcdp      pointer to an @p edma_tcd_t structure
 * @param[in] ssize     the source width
 * @param[in] dsize     the destination width
 * @param[in] soff      the source increment value
 *
 * @api
 */
#define edmaTCDSetWord1(tcdp, ssize, dsize, soff)                           \
  ((tcdp)->word[1] = (((uint32_t)(ssize) << 24) |                           \
                      ((uint32_t)(dsize) << 16) |                           \
                      ((uint32_t)(soff) << 0)))

/**
 * @brief   Sets the word 2 fields into a TCD.
 *
 * @param[in] tcdp      pointer to an @p edma_tcd_t structure
 * @param[in] nbytes    the inner counter value
 *
 * @api
 */
#define edmaTCDSetWord2(tcdp, nbytes)                                       \
  ((tcdp)->word[2] = (uint32_t)(nbytes))

/**
 * @brief   Sets the word 3 fields into a TCD.
 *
 * @param[in] tcdp      pointer to an @p edma_tcd_t structure
 * @param[in] slast      the adjustment value
 *
 * @api
 */
#define edmaTCDSetWord3(tcdp, slast)                                        \
  ((tcdp)->word[3] = (uint32_t)(slast))

/**
 * @brief   Sets the word 4 fields into a TCD.
 *
 * @param[in] tcdp      pointer to an @p edma_tcd_t structure
 * @param[in] dst       the destination address
 *
 * @api
 */
#define edmaTCDSetWord4(tcdp, dst)                                          \
  ((tcdp)->word[4] = (uint32_t)(dst))

/**
 * @brief   Sets the word 5 fields into a TCD.
 *
 * @param[in] tcdp      pointer to an @p edma_tcd_t structure
 * @param[in] citer     the current outer counter value
 * @param[in] doff      the destination increment value
 *
 * @api
 */
#define edmaTCDSetWord5(tcdp, citer, doff)                                  \
  ((tcdp)->word[5] = (((uint32_t)(citer) << 16) |                           \
                      ((uint32_t)(doff) << 0)))

/**
 * @brief   Sets the word 5 fields into a TCD.
 * @note    Transfers are limited to 512 operations using this modality
 *          (citer parameter).
 *
 * @param[in] tcdp      pointer to an @p edma_tcd_t structure
 * @param[in] linkch    channel linked on minor loop counter
 * @param[in] citer     the current outer counter value
 * @param[in] doff      the destination increment value
 *
 * @api
 */
#define edmaTCDSetWord5Linked(tcdp, linkch, citer, doff)                    \
  ((tcdp)->word[5] = (((uint32_t)0x80000000U) |                             \
                      ((uint32_t)(linkch) << 25) |                          \
                      ((uint32_t)(citer) << 16) |                           \
                      ((uint32_t)(doff) << 0)))

/**
 * @brief   Sets the word 6 fields into a TCD.
 *
 * @param[in] tcdp      pointer to an @p edma_tcd_t structure
 * @param[in] dlast     the adjustment value
 *
 * @api
 */
#define edmaTCDSetWord6(tcdp, dlast)                                        \
  ((tcdp)->word[6] = (uint32_t)(dlast))

/**
 * @brief   Sets the word 7 fields into a TCD.
 *
 * @param[in] tcdp      pointer to an @p edma_tcd_t structure
 * @param[in] biter     the base outer counter value
 * @param[in] mode      the mode value
 *
 * @api
 */
#define edmaTCDSetWord7(tcdp, biter, mode)                                  \
  ((tcdp)->word[7] = (((uint32_t)(biter) << 16) |                           \
                      ((uint32_t)(mode) << 0)))

/**
 * @brief   Sets the word 7 fields into a TCD.
 * @note    Transfers are limited to 512 operations using this modality
 *          (biter parameter).
 *
 * @param[in] tcdp      pointer to an @p edma_tcd_t structure
 * @param[in] linkch    channel linked on minor loop counter
 * @param[in] biter     the base outer counter value
 * @param[in] mode      the mode value
 *
 * @api
 */
#define edmaTCDSetWord7Linked(tcdp, linkch, biter, mode)                    \
  ((tcdp)->word[7] = (((uint32_t)0x80000000U) |                             \
                      ((uint32_t)(linkch) << 25) |                          \
                      ((uint32_t)(biter) << 16) |                           \
                      ((uint32_t)(mode) << 0)))

/**
 * @brief   EDMA channel setup.
 *
 * @param[in] channel   eDMA channel number
 * @param[in] src       source address
 * @param[in] dst       destination address
 * @param[in] soff      source address offset
 * @param[in] doff      destination address offset
 * @param[in] ssize     source transfer size
 * @param[in] dsize     destination transfer size
 * @param[in] nbytes    minor loop count
 * @param[in] iter      major loop count
 * @param[in] dlast     last destination address adjustment
 * @param[in] slast     last source address adjustment
 * @param[in] mode      LSW of TCD register 7
 *
 * @api
 */
#define edmaChannelSetup(channel, src, dst, soff, doff, ssize, dsize,       \
                         nbytes, iter, slast, dlast, mode) {                \
  edma_tcd_t *tcdp = edmaGetTCD(channel);                                   \
  edmaTCDSetWord0(tcdp, src);                                               \
  edmaTCDSetWord1(tcdp, ssize, dsize, soff);                                \
  edmaTCDSetWord2(tcdp, nbytes);                                            \
  edmaTCDSetWord3(tcdp, slast);                                             \
  edmaTCDSetWord4(tcdp, dst);                                               \
  edmaTCDSetWord5(tcdp, iter, doff);                                        \
  edmaTCDSetWord6(tcdp, dlast);                                             \
  edmaTCDSetWord7(tcdp, iter, mode);                                        \
}

/**
 * @brief   EDMA channel setup with linked channel on both minor and major
 *          loop counters.
 * @note    Transfers are limited to 512 operations using this modality
 *          (iter parameter).
 *
 * @param[in] channel   eDMA channel number
 * @param[in] linkch    channel linked on minor loop counter
 * @param[in] src       source address
 * @param[in] dst       destination address
 * @param[in] soff      source address offset
 * @param[in] doff      destination address offset
 * @param[in] ssize     source transfer size
 * @param[in] dsize     destination transfer size
 * @param[in] nbytes    minor loop count
 * @param[in] iter      major loop count
 * @param[in] dlast     last destination address adjustment
 * @param[in] slast     last source address adjustment
 * @param[in] mode      LSW of TCD register 7
 *
 * @api
 */
#define edmaChannelSetupLinked(channel, linkch, src, dst, soff,             \
                               doff, ssize, dsize, nbytes, iter,            \
                               slast, dlast, mode) {                        \
  edma_tcd_t *tcdp = edmaGetTCD(channel);                                   \
  edmaTCDSetWord0(tcdp, src);                                               \
  edmaTCDSetWord1(tcdp, ssize, dsize, soff);                                \
  edmaTCDSetWord2(tcdp, nbytes);                                            \
  edmaTCDSetWord3(tcdp, slast);                                             \
  edmaTCDSetWord4(tcdp, dst);                                               \
  edmaTCDSetWord5Linked(tcdp, linkch, iter, doff);                          \
  edmaTCDSetWord6(tcdp, dlast);                                             \
  edmaTCDSetWord7Linked(tcdp, linkch, iter, (mode) |                        \
                                            EDMA_TCD_MODE_MELINK |          \
                                            EDMA_TCD_MODE_MLINKCH(linkch)); \
}

/**
 * @brief   Enables the request signal related to the specified channel.
 *
 * @param[in] channel   the channel number
 *
 * @api
 */
#if (SPC5_EDMA_NCHANNELS <= 64U)
#define edmaEnableRequest(channel) (SPC5_EDMA1.SERQ.R = (uint8_t)(channel))
#else
#define edmaEnableRequest(channel) ((channel < 64U)? (SPC5_EDMA1.SERQ.R = (uint8_t)(channel)) : (SPC5_EDMA0.SERQ.R = (uint8_t)(channel - 64U)))
#endif /* SPC5_EDMA_NCHANNELS <= 64U */

/**
 * @brief   Disables the request signal related to the specified channel.
 *
 * @param[in] channel   the channel number
 *
 * @api
 */
#if (SPC5_EDMA_NCHANNELS <= 64U)
#define edmaDisableRequest(channel) (SPC5_EDMA1.CERQ.R = (uint8_t)(channel))
#else
#define edmaDisableRequest(channel) ((channel < 64U)? (SPC5_EDMA1.CERQ.R = (uint8_t)(channel)) : (SPC5_EDMA0.CERQ.R = (uint8_t)(channel - 64U)))
#endif /* SPC5_EDMA_NCHANNELS <= 64U */

/**
 * @brief   Sets Start bit in TCD for a specified channel.
 *
 * @param[in] channel   the channel number
 *
 * @api
 */
#if (SPC5_EDMA_NCHANNELS <= 64U)
#define edmaSetStart(channel) (SPC5_EDMA1.SSRT.R = (uint8_t)(channel))
#else
#define edmaSetStart(channel) ((channel < 64U)? (SPC5_EDMA1.SSRT.R = (uint8_t)(channel)) : (SPC5_EDMA0.SSRT.R = (uint8_t)(channel - 64U)))
#endif /* SPC5_EDMA_NCHANNELS <= 64U */

/**
 * @brief   Clears Done bit related to the specified channel.
 *
 * @param[in] channel   the channel number
 *
 * @api
 */
#if (SPC5_EDMA_NCHANNELS <= 64U)
#define edmaClearDone(channel) (SPC5_EDMA1.CDNE.R = (uint8_t)(channel))
#else
#define edmaClearDone(channel) ((channel < 64U)? (SPC5_EDMA1.CDNE.R = (uint8_t)(channel)) : (SPC5_EDMA0.CDNE.R = (uint8_t)(channel - 64U)))
#endif /* SPC5_EDMA_NCHANNELS <= 64U */

/**
 * @brief   Starts a specified channel.
 *
 * @param[in] channel   the channel number
 *
 * @api
 */
#define edmaChannelStart(channel) edmaEnableRequest(channel)

/**
 * @brief   Stops a specified channel.
 *
 * @param[in] channel   the channel number
 *
 * @api
 */
#define edmaChannelStop(channel) {                                          \
  edmaDisableRequest(channel);                                              \
  edmaClearDone(channel);                                                   \
}

/*===========================================================================*/
/* IRQ Handlers.                                                             */
/*===========================================================================*/

IRQ_HANDLER(SPC5_EDMA_COMBO_CH_ERR_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH0_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH1_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH2_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH3_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH4_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH5_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH6_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH7_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH8_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH9_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH10_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH11_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH12_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH13_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH14_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH15_HANDLER);
#if (SPC5_EDMA_NCHANNELS > 16U) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_EDMA_CH16_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH17_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH18_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH19_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH20_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH21_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH22_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH23_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH24_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH25_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH26_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH27_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH28_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH29_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH30_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH31_HANDLER);
#if (SPC5_EDMA_NCHANNELS > 32U) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_EDMA_CH32_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH33_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH34_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH35_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH36_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH37_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH38_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH39_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH40_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH41_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH42_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH43_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH44_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH45_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH46_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH47_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH48_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH49_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH50_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH51_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH52_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH53_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH54_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH55_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH56_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH57_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH58_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH59_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH60_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH61_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH62_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH63_HANDLER);
#if (SPC5_EDMA_NCHANNELS > 64U) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_EDMA_CH64_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH65_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH66_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH67_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH68_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH69_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH70_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH71_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH72_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH73_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH74_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH75_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH76_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH77_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH78_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH79_HANDLER);
#if (SPC5_EDMA_NCHANNELS > 80U) || defined(__DOXYGEN__)
IRQ_HANDLER(SPC5_EDMA_CH80_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH81_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH82_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH83_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH84_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH85_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH86_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH87_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH88_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH89_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH90_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH91_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH92_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH93_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH94_HANDLER);
IRQ_HANDLER(SPC5_EDMA_CH95_HANDLER);
#endif /* SPC5_EDMA_NCHANNELS > 80 */
#endif /* SPC5_EDMA_NCHANNELS > 64 */
#endif /* SPC5_EDMA_NCHANNELS > 32 */
#endif /* SPC5_EDMA_NCHANNELS > 16 */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/**
 * @name    Peripherals references
 *
 * @{
 */
#if (SPC5_HAS_EDMA0 == TRUE)
#define SPC5_EDMA0                          (*(spc5_edma_t *)SPC5_EDMA_DMA0_BASE)
#endif

#if (SPC5_HAS_EDMA1 == TRUE)
#define SPC5_EDMA1                          (*(spc5_edma_t *)SPC5_EDMA_DMA1_BASE)
#endif

#if (SPC5_EDMA_NUM_OF_MUX >= 1U) || defined(__DOXYGEN__)
#define SPC5_DMAMUX0                        (*(spc5_dmamux_t *)SPC5_EDMA_MUX0_BASE)
#endif
#if (SPC5_EDMA_NUM_OF_MUX >= 2U) || defined(__DOXYGEN__)
#define SPC5_DMAMUX1                        (*(spc5_dmamux_t *)SPC5_EDMA_MUX1_BASE)
#endif
#if (SPC5_EDMA_NUM_OF_MUX >= 3U) || defined(__DOXYGEN__)
#define SPC5_DMAMUX2                        (*(spc5_dmamux_t *)SPC5_EDMA_MUX2_BASE)
#endif
#if (SPC5_EDMA_NUM_OF_MUX >= 4U) || defined(__DOXYGEN__)
#define SPC5_DMAMUX3                        (*(spc5_dmamux_t *)SPC5_EDMA_MUX3_BASE)
#endif
#if (SPC5_EDMA_NUM_OF_MUX >= 5U) || defined(__DOXYGEN__)
#define SPC5_DMAMUX4                        (*(spc5_dmamux_t *)SPC5_EDMA_MUX4_BASE)
#endif
#if (SPC5_EDMA_NUM_OF_MUX >= 6U) || defined(__DOXYGEN__)
#define SPC5_DMAMUX5                        (*(spc5_dmamux_t *)SPC5_EDMA_MUX5_BASE)
#endif
/** @} */

#ifdef __cplusplus
extern "C" {
#endif
  void edmaInit(void);
  edma_channel_t edmaChannelAllocate(const edma_channel_config_t *ccfg);
  void edmaChannelRelease(const edma_channel_config_t *ccfg);
#ifdef __cplusplus
}
#endif

#endif /* LLD_USE_DMA */

#endif /* _SPC5_EDMA_H_ */

/** @} */
