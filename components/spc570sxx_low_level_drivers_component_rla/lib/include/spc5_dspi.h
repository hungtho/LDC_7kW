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
* EVALUATION ONLY - NOT FOR USE IN PRODUCTION
*****************************************************************************/
/**
 * @file    spc5_dspi.h
 * @brief   SPC5xx DSPI header file.
 *
 * @addtogroup SPI
 * @{
 */

#ifndef _SPC5_DSPI_H_
#define _SPC5_DSPI_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    MCR register definitions
 * @{
 */
#define SPC5_MCR_MSTR                       (1UL << 31)
#define SPC5_MCR_CONT_SCKE                  (1UL << 30)
#define SPC5_MCR_DCONF_MASK                 (3UL << 28)
#define SPC5_MCR_FRZ                        (1UL << 27)
#define SPC5_MCR_MTFE                       (1UL << 26)
#define SPC5_MCR_PCSSE                      (1UL << 25)
#define SPC5_MCR_ROOE                       (1UL << 24)
#define SPC5_MCR_PCSIS7                     (1UL << 23)
#define SPC5_MCR_PCSIS6                     (1UL << 22)
#define SPC5_MCR_PCSIS5                     (1UL << 21)
#define SPC5_MCR_PCSIS4                     (1UL << 20)
#define SPC5_MCR_PCSIS3                     (1UL << 19)
#define SPC5_MCR_PCSIS2                     (1UL << 18)
#define SPC5_MCR_PCSIS1                     (1UL << 17)
#define SPC5_MCR_PCSIS0                     (1UL << 16)
#define SPC5_MCR_DOZE                       (1UL << 15)
#define SPC5_MCR_MDIS                       (1UL << 14)
#define SPC5_MCR_DIS_TXF                    (1UL << 13)
#define SPC5_MCR_DIS_RXF                    (1UL << 12)
#define SPC5_MCR_CLR_TXF                    (1UL << 11)
#define SPC5_MCR_CLR_RXF                    (1UL << 10)
#define SPC5_MCR_SMPL_PT_MASK               (3UL << 8)
#define SPC5_MCR_SMPL_PT(n)                 ((uint32_t)(n) << 8)
#define SPC5_MCR_FCPCS                      (1UL << 2)
#define SPC5_MCR_PES                        (1UL << 1)
#define SPC5_MCR_HALT                       (1UL << 0)
/** @} */

/**
 * @name    RSER register definitions
 * @{
 */
#define SPC5_RSER_TCF_RE                    (1UL << 31)
#define SPC5_RSER_DSITCF_RE                 (1UL << 29)
#define SPC5_RSER_EOQF_RE                   (1UL << 28)
#define SPC5_RSER_TFUF_RE                   (1UL << 27)
#define SPC5_RSER_SPITCF_RE                 (1UL << 26)
#define SPC5_RSER_TFFF_RE                   (1UL << 25)
#define SPC5_RSER_TFFF_DIRS                 (1UL << 24)
#define SPC5_RSER_DPEF_RE                   (1UL << 22)
#define SPC5_RSER_SPEF_RE                   (1UL << 21)
#define SPC5_RSER_DDIF_RE                   (1UL << 20)
#define SPC5_RSER_RFOF_RE                   (1UL << 19)
#define SPC5_RSER_RFDF_RE                   (1UL << 17)
#define SPC5_RSER_RFDF_DIRS                 (1UL << 16)
/** @} */

/**
 * @name    CTAR registers definitions
 * @{
 */
#define SPC5_CTAR_DBR                       (1UL << 31)
#define SPC5_CTAR_FMSZ_MASK                 (15UL << 27)
#define SPC5_CTAR_FMSZ(n)                   (((uint32_t)(n) - 1U) << 27)
#define SPC5_CTAR_CPOL                      (1UL << 26)
#define SPC5_CTAR_CPHA                      (1UL << 25)
#define SPC5_CTAR_LSBFE                     (1UL << 24)
#define SPC5_CTAR_PCSSCK_MASK               (3UL << 22)
#define SPC5_CTAR_PCSSCK_PRE1               (0UL << 22)
#define SPC5_CTAR_PCSSCK_PRE3               (1UL << 22)
#define SPC5_CTAR_PCSSCK_PRE5               (2UL << 22)
#define SPC5_CTAR_PCSSCK_PRE7               (3UL << 22)
#define SPC5_CTAR_PASC_MASK                 (3UL << 20)
#define SPC5_CTAR_PASC_PRE1                 (0UL << 20)
#define SPC5_CTAR_PASC_PRE3                 (1UL << 20)
#define SPC5_CTAR_PASC_PRE5                 (2UL << 20)
#define SPC5_CTAR_PASC_PRE7                 (3UL << 20)
#define SPC5_CTAR_PDT_MASK                  (3UL << 18)
#define SPC5_CTAR_PDT_PRE1                  (0UL << 18)
#define SPC5_CTAR_PDT_PRE3                  (1UL << 18)
#define SPC5_CTAR_PDT_PRE5                  (2UL << 18)
#define SPC5_CTAR_PDT_PRE7                  (3UL << 18)
#define SPC5_CTAR_PBR_MASK                  (3UL << 16)
#define SPC5_CTAR_PBR_PRE2                  (0UL << 16)
#define SPC5_CTAR_PBR_PRE3                  (1UL << 16)
#define SPC5_CTAR_PBR_PRE5                  (2UL << 16)
#define SPC5_CTAR_PBR_PRE7                  (3UL << 16)
#define SPC5_CTAR_CSSCK_MASK                (15UL << 12)
#define SPC5_CTAR_CSSCK_DIV2                (0UL << 12)
#define SPC5_CTAR_CSSCK_DIV4                (1UL << 12)
#define SPC5_CTAR_CSSCK_DIV8                (2UL << 12)
#define SPC5_CTAR_CSSCK_DIV16               (3UL << 12)
#define SPC5_CTAR_CSSCK_DIV32               (4UL << 12)
#define SPC5_CTAR_CSSCK_DIV64               (5UL << 12)
#define SPC5_CTAR_CSSCK_DIV128              (6UL << 12)
#define SPC5_CTAR_CSSCK_DIV256              (7UL << 12)
#define SPC5_CTAR_CSSCK_DIV512              (8UL << 12)
#define SPC5_CTAR_CSSCK_DIV1024             (9UL << 12)
#define SPC5_CTAR_CSSCK_DIV2048             (10UL << 12)
#define SPC5_CTAR_CSSCK_DIV4096             (11UL << 12)
#define SPC5_CTAR_CSSCK_DIV8192             (12UL << 12)
#define SPC5_CTAR_CSSCK_DIV16384            (13UL << 12)
#define SPC5_CTAR_CSSCK_DIV32768            (14UL << 12)
#define SPC5_CTAR_CSSCK_DIV65536            (15UL << 12)
#define SPC5_CTAR_ASC_MASK                  (15UL << 8)
#define SPC5_CTAR_ASC_DIV2                  (0UL << 8)
#define SPC5_CTAR_ASC_DIV4                  (1UL << 8)
#define SPC5_CTAR_ASC_DIV8                  (2UL << 8)
#define SPC5_CTAR_ASC_DIV16                 (3UL << 8)
#define SPC5_CTAR_ASC_DIV32                 (4UL << 8)
#define SPC5_CTAR_ASC_DIV64                 (5UL << 8)
#define SPC5_CTAR_ASC_DIV128                (6UL << 8)
#define SPC5_CTAR_ASC_DIV256                (7UL << 8)
#define SPC5_CTAR_ASC_DIV512                (8UL << 8)
#define SPC5_CTAR_ASC_DIV1024               (9UL << 8)
#define SPC5_CTAR_ASC_DIV2048               (10UL << 8)
#define SPC5_CTAR_ASC_DIV4096               (11UL << 8)
#define SPC5_CTAR_ASC_DIV8192               (12UL << 8)
#define SPC5_CTAR_ASC_DIV16384              (13UL << 8)
#define SPC5_CTAR_ASC_DIV32768              (14UL << 8)
#define SPC5_CTAR_ASC_DIV65536              (15UL << 8)
#define SPC5_CTAR_DT_MASK                   (15UL << 4)
#define SPC5_CTAR_DT_DIV2                   (0UL << 4)
#define SPC5_CTAR_DT_DIV4                   (1UL << 4)
#define SPC5_CTAR_DT_DIV8                   (2UL << 4)
#define SPC5_CTAR_DT_DIV16                  (3UL << 4)
#define SPC5_CTAR_DT_DIV32                  (4UL << 4)
#define SPC5_CTAR_DT_DIV64                  (5UL << 4)
#define SPC5_CTAR_DT_DIV128                 (6UL << 4)
#define SPC5_CTAR_DT_DIV256                 (7UL << 4)
#define SPC5_CTAR_DT_DIV512                 (8UL << 4)
#define SPC5_CTAR_DT_DIV1024                (9UL << 4)
#define SPC5_CTAR_DT_DIV2048                (10UL << 4)
#define SPC5_CTAR_DT_DIV4096                (11UL << 4)
#define SPC5_CTAR_DT_DIV8192                (12UL << 4)
#define SPC5_CTAR_DT_DIV16384               (13UL << 4)
#define SPC5_CTAR_DT_DIV32768               (14UL << 4)
#define SPC5_CTAR_DT_DIV65536               (15UL << 4)
#define SPC5_CTAR_BR_MASK                   (15UL << 0)
#define SPC5_CTAR_BR_DIV2                   (0UL << 0)
#define SPC5_CTAR_BR_DIV4                   (1UL << 0)
#define SPC5_CTAR_BR_DIV6                   (2UL << 0)
#define SPC5_CTAR_BR_DIV8                   (3UL << 0)
#define SPC5_CTAR_BR_DIV16                  (4UL << 0)
#define SPC5_CTAR_BR_DIV32                  (5UL << 0)
#define SPC5_CTAR_BR_DIV64                  (6UL << 0)
#define SPC5_CTAR_BR_DIV128                 (7UL << 0)
#define SPC5_CTAR_BR_DIV256                 (8UL << 0)
#define SPC5_CTAR_BR_DIV512                 (9UL << 0)
#define SPC5_CTAR_BR_DIV1024                (10UL << 0)
#define SPC5_CTAR_BR_DIV2048                (11UL << 0)
#define SPC5_CTAR_BR_DIV4096                (12UL << 0)
#define SPC5_CTAR_BR_DIV8192                (13UL << 0)
#define SPC5_CTAR_BR_DIV16384               (14UL << 0)
#define SPC5_CTAR_BR_DIV32768               (15UL << 0)
/** @} */

/**
 * @name    PUSHR register definitions
 * @{
 */
#define SPC5_PUSHR_CONT                     (1UL << 31)
#define SPC5_PUSHR_CTAS_MASK                (3UL << 28)
#define SPC5_PUSHR_CTAS(n)                  ((uint32_t)(n) << 29)
#define SPC5_PUSHR_EOQ                      (1UL << 27)
#define SPC5_PUSHR_CTCNT                    (1UL << 26)
#define SPC5_PUSHR_MASC                     (1UL << 25)
#define SPC5_PUSHR_MCSC                     (1UL << 24)
#define SPC5_PUSHR_PCS_MASK                 (255UL << 16)
#define SPC5_PUSHR_PCS(n)                   ((1UL << (n)) << 16)
#define SPC5_PUSHR_TXDATA_MASK              (0xFFFFUL << 0)
/** @} */

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
struct spc5_dspi {
  union {
    vuint32_t R;
    struct {
      vuint32_t MSTR :1;
      vuint32_t CONT_SCKE :1;
      vuint32_t DCONF :2;
      vuint32_t FRZ :1;
      vuint32_t MTFE :1;
      vuint32_t PCSSE :1;
      vuint32_t ROOE :1;
      vuint32_t PCSIS7 :1;
      vuint32_t PCSIS6 :1;
      vuint32_t PCSIS5 :1;
      vuint32_t PCSIS4 :1;
      vuint32_t PCSIS3 :1;
      vuint32_t PCSIS2 :1;
      vuint32_t PCSIS1 :1;
      vuint32_t PCSIS0 :1;
      vuint32_t :1;
      vuint32_t MDIS :1;
      vuint32_t DIS_TXF :1;
      vuint32_t DIS_RXF :1;
      vuint32_t CLR_TXF :1;
      vuint32_t CLR_RXF :1;
      vuint32_t SMPL_PT :2;
      vuint32_t :7;
      vuint32_t HALT :1;
    } B;
  } MCR; /* Module Configuration Register */

  uint32_t dspi_reserved1;

  union {
    vuint32_t R;
    struct {
      vuint32_t TCNT :16;
      vuint32_t :16;
    } B;
  } TCR;

  union {
    vuint32_t R;
    struct {
      vuint32_t DBR :1;
      vuint32_t FMSZ :4;
      vuint32_t CPOL :1;
      vuint32_t CPHA :1;
      vuint32_t LSBFE :1;
      vuint32_t PCSSCK :2;
      vuint32_t PASC :2;
      vuint32_t PDT :2;
      vuint32_t PBR :2;
      vuint32_t CSSCK :4;
      vuint32_t ASC :4;
      vuint32_t DT :4;
      vuint32_t BR :4;
    } B;
  } CTAR[8]; /* Clock and Transfer Attributes Registers */

  union {
    vuint32_t R;
    struct {
      vuint32_t TCF :1;
      vuint32_t TXRXS :1;
      vuint32_t :1;
      vuint32_t EOQF :1;
      vuint32_t TFUF :1;
      vuint32_t :1;
      vuint32_t TFFF :1;
      vuint32_t :5;
      vuint32_t RFOF :1;
      vuint32_t :1;
      vuint32_t RFDF :1;
      vuint32_t :1;
      vuint32_t TXCTR :4;
      vuint32_t TXNXTPTR :4;
      vuint32_t RXCTR :4;
      vuint32_t POPNXTPTR :4;
    } B;
  } SR; /* Status Register */

  union {
    vuint32_t R;
    struct {
      vuint32_t TCFRE :1;
      vuint32_t :2;
      vuint32_t EOQFRE :1;
      vuint32_t TFUFRE :1;
      vuint32_t :1;
      vuint32_t TFFFRE :1;
      vuint32_t TFFFDIRS :1;
      vuint32_t :4;
      vuint32_t RFOFRE :1;
      vuint32_t :1;
      vuint32_t RFDFRE :1;
      vuint32_t RFDFDIRS :1;
      vuint32_t :16;
    } B;
  } RSER; /* DMA/Interrupt Request Select and Enable Register */

  union {
    vuint32_t R;
    struct {
      vuint32_t CONT :1;
      vuint32_t CTAS :3;
      vuint32_t EOQ :1;
      vuint32_t CTCNT :1;
      vuint32_t :2;
      vuint32_t PCS7 :1;
      vuint32_t PCS6 :1;
      vuint32_t PCS5 :1;
      vuint32_t PCS4 :1;
      vuint32_t PCS3 :1;
      vuint32_t PCS2 :1;
      vuint32_t PCS1 :1;
      vuint32_t PCS0 :1;
      vuint32_t TXDATA :16;
    } B;
  } PUSHR; /* PUSH TX FIFO Register */

  union {
    vuint32_t R;
    struct {
      vuint32_t :16;
      vuint32_t RXDATA :16;
    } B;
  } POPR; /* POP RX FIFO Register */

  union {
    vuint32_t R;
    struct {
      vuint32_t TXCMD :16;
      vuint32_t TXDATA :16;
    } B;
  } TXFR[5]; /* Transmit FIFO Registers */

  vuint32_t DSPI_reserved_txf[11];

  union {
    vuint32_t R;
    struct {
      vuint32_t :16;
      vuint32_t RXDATA :16;
    } B;
  } RXFR[5]; /* Receive FIFO Registers */

  vuint32_t DSPI_reserved_rxf[12];

  union {
    vuint32_t R;
    struct {
      vuint32_t MTOE :1;
      vuint32_t :1;
      vuint32_t MTOCNT :6;
      vuint32_t :4;
      vuint32_t TXSS :1;
      vuint32_t TPOL :1;
      vuint32_t TRRE :1;
      vuint32_t CID :1;
      vuint32_t DCONT :1;
      vuint32_t DSICTAS :3;
      vuint32_t :6;
      vuint32_t DPCS5 :1;
      vuint32_t DPCS4 :1;
      vuint32_t DPCS3 :1;
      vuint32_t DPCS2 :1;
      vuint32_t DPCS1 :1;
      vuint32_t DPCS0 :1;
    } B;
  } DSICR; /* DSI Configuration Register */

  union {
    vuint32_t R;
    struct {
      vuint32_t :16;
      vuint32_t SER_DATA :16;
    } B;
  } SDR; /* DSI Serialization Data Register */

  union {
    vuint32_t R;
    struct {
      vuint32_t :16;
      vuint32_t ASER_DATA :16;
    } B;
  } ASDR; /* DSI Alternate Serialization Data Register */

  union {
    vuint32_t R;
    struct {
      vuint32_t :16;
      vuint32_t COMP_DATA :16;
    } B;
  } COMPR; /* DSI Transmit Comparison Register */

  union {
    vuint32_t R;
    struct {
      vuint32_t :16;
      vuint32_t DESER_DATA :16;
    } B;
  } DDR; /* DSI deserialization Data Register */

};
/*lint +e46 */

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    DSPI units references
 * @{
 */
#if (SPC5_HAS_DSPI0 == TRUE) || defined(__DOXYGEN__)
#define SPC5_DSPI0      (*(struct spc5_dspi *)SPC5_DSPI0_BASE)
#endif

#if (SPC5_HAS_DSPI1 == TRUE) || defined(__DOXYGEN__)
#define SPC5_DSPI1      (*(struct spc5_dspi *)SPC5_DSPI1_BASE)
#endif

#if (SPC5_HAS_DSPI2 == TRUE) || defined(__DOXYGEN__)
#define SPC5_DSPI2      (*(struct spc5_dspi *)SPC5_DSPI2_BASE)
#endif

#if (SPC5_HAS_DSPI3 == TRUE) || defined(__DOXYGEN__)
#define SPC5_DSPI3      (*(struct spc5_dspi *)SPC5_DSPI3_BASE)
#endif

#if (SPC5_HAS_DSPI4 == TRUE) || defined(__DOXYGEN__)
#define SPC5_DSPI4      (*(struct spc5_dspi *)SPC5_DSPI4_BASE)
#endif

#if (SPC5_HAS_DSPI5 == TRUE) || defined(__DOXYGEN__)
#define SPC5_DSPI5      (*(struct spc5_dspi *)SPC5_DSPI5_BASE)
#endif

#if (SPC5_HAS_DSPI6 == TRUE) || defined(__DOXYGEN__)
#define SPC5_DSPI6      (*(struct spc5_dspi *)SPC5_DSPI6_BASE)
#endif

#if (SPC5_HAS_DSPI7 == TRUE) || defined(__DOXYGEN__)
#define SPC5_DSPI7      (*(struct spc5_dspi *)SPC5_DSPI7_BASE)
#endif

#if (SPC5_HAS_DSPI8 == TRUE) || defined(__DOXYGEN__)
#define SPC5_DSPI8      (*(struct spc5_dspi *)SPC5_DSPI8_BASE)
#endif

#if (SPC5_HAS_DSPI9 == TRUE) || defined(__DOXYGEN__)
#define SPC5_DSPI9      (*(struct spc5_dspi *)SPC5_DSPI9_BASE)
#endif
/** @} */

#endif /* _SPC5_DSPI_H_ */

/** @} */
