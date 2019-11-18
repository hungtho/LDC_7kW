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
 * @file    saradc_lld.h
 * @brief   SPC5xx SARADC low level driver header.
 *
 * @addtogroup SARADC
 * @{
 */

#ifndef _SARADC_LLD_H_
#define _SARADC_LLD_H_

#include "spc5_lld.h"
#include "lldconf.h"
#if (SPC5_SARADC_USE_DMA == TRUE)
#include "spc5_edma.h"
#endif

#if (LLD_USE_SARADC == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    SARADC constant definitions
 * @{
 */
#define SARADC_MODE_ONESHOT                0U
#define SARADC_MODE_SCAN                   1U
#define SARADC_CHANNEL_INTERNAL            0U
#define SARADC_CHANNEL_EXTERNAL            1U
#define SARADC_CHANNEL_TEST                2U
#define SARADC_REFERENCE_DEFAULT           0U
#define SARADC_REFERENCE_ALTERNATE         1U
#define SARADC_PRECHARGE_DISABLED          0U
#define SARADC_PRECHARGE_ENABLED           1U
#define SARADC_CTR0                        0U
#define SARADC_CTR1                        1U
#define SARADC_CTR2                        2U
#define SARADC_CTR3                        3U
#define SARADC_WATCHDOG_REGISTER_0         0U
#define SARADC_WATCHDOG_REGISTER_1         1U
#define SARADC_WATCHDOG_REGISTER_2         2U
#define SARADC_WATCHDOG_REGISTER_3         3U
#define SARADC_WATCHDOG_REGISTER_NONE      0xFFU
#define SARADC_CRES_HIGH                   0U
#define SARADC_CRES_LOW                    1U
#define SARADC_DMA_BUFFER_LINEAR		   0U
#define SARADC_DMA_BUFFER_CIRCULAR		   1U
#define SARADC_TRIGGER_DISABLED			   0xFFU
#define SARADC_TRIGGER_ENABLED			   1U
#define SARADC_TRIGGER_FALLING_EDGE		   0U
#define SARADC_TRIGGER_RISING_EDGE		   1U
#define SARADC_TRIGGER_BOTH_EDGES		   3U		   

/** @} */

/**
 * @name    SARADC error definitions
 * @{
 */
#define SARADC_INVALID_DATA                0x2000U

/** @} */
 
/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   SARADCD12_1 driver enable switch.
 * @details If set to @p TRUE the support for SARADC12_0 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_SARADC_USE_SARADC12_0) || defined(__DOXYGEN__)
#define SPC5_SARADC_USE_SARADC12_0                   FALSE
#endif

/**
 * @brief   SARADCD12_2 driver enable switch.
 * @details If set to @p TRUE the support for SARADC12_1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_SARADC_USE_SARADC12_1) || defined(__DOXYGEN__)
#define SPC5_SARADC_USE_SARADC12_1                   FALSE
#endif

/**
 * @brief   SARADCD12_3 driver enable switch.
 * @details If set to @p TRUE the support for SARADC12_2 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_SARADC_USE_SARADC12_2) || defined(__DOXYGEN__)
#define SPC5_SARADC_USE_SARADC12_2                   FALSE
#endif

/**
 * @brief   SARADCD12_4 driver enable switch.
 * @details If set to @p TRUE the support for SARADC12_3 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_SARADC_USE_SARADC12_3) || defined(__DOXYGEN__)
#define SPC5_SARADC_USE_SARADC12_3                   FALSE
#endif

/**
 * @brief   SARADCD12_5 driver enable switch.
 * @details If set to @p TRUE the support for SARADC12_4 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_SARADC_USE_SARADC12_4) || defined(__DOXYGEN__)
#define SPC5_SARADC_USE_SARADC12_4                   FALSE
#endif

/**
 * @brief   SARADCD12_6 driver enable switch.
 * @details If set to @p TRUE the support for SARADC12_5 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_SARADC_USE_SARADC12_5) || defined(__DOXYGEN__)
#define SPC5_SARADC_USE_SARADC12_5                   FALSE
#endif

/**
 * @brief   SARADCD12_7 driver enable switch.
 * @details If set to @p TRUE the support for SARADC12_6 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_SARADC_USE_SARADC12_6) || defined(__DOXYGEN__)
#define SPC5_SARADC_USE_SARADC12_6                   FALSE
#endif

/**
 * @brief   SARADCD12_SV driver enable switch.
 * @details If set to @p TRUE the support for SARADC12_SV is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_SARADC_USE_SARADC12_SV) || defined(__DOXYGEN__)
#define SPC5_SARADC_USE_SARADC12_SV                   FALSE
#endif
/**
 * @brief   SARADCD12_SV_1 driver enable switch.
 * @details If set to @p TRUE the support for SARADC12_SV_1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_SARADC_USE_SARADC12_SV_1) || defined(__DOXYGEN__)
#define SPC5_SARADC_USE_SARADC12_SV_1                 FALSE
#endif
/**
 * @brief   SARADCD10_1 driver enable switch.
 * @details If set to @p TRUE the support for SARADC10_0 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_SARADC_USE_SARADC10_0) || defined(__DOXYGEN__)
#define SPC5_SARADC_USE_SARADC10_0                   FALSE
#endif

/**
 * @brief   SARADCD10_2 driver enable switch.
 * @details If set to @p TRUE the support for SARADC10_1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_SARADC_USE_SARADC10_1) || defined(__DOXYGEN__)
#define SPC5_SARADC_USE_SARADC10_1                   FALSE
#endif

/**
 * @brief   SARADCD10_STANDBY driver enable switch.
 * @details If set to @p TRUE the support for SARADC10_1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_SARADC_USE_SARADC10_STANDBY) || defined(__DOXYGEN__)
#define SPC5_SARADC_USE_SARADC10_STANDBY              FALSE
#endif

/**
 * @brief   SARADC CHECK VALID BYPASS
 * @details If set to @p TRUE if VALID bit in ICDR register should not be checked before converted value read.
 * @note    The default is @p FALSE.
 */
#if !defined(SPC5_SARADC_BYPASS_VALID_CHECK) || defined(__DOXYGEN__)
#define SPC5_SARADC_BYPASS_VALID_CHECK              FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/
#if (SPC5_HAS_SARADC12_0 == FALSE) && (SPC5_SARADC_USE_SARADC12_0 == TRUE)
#error "SARADC12_0 not present in the selected device"
#endif
#if (SPC5_HAS_SARADC12_1 == FALSE) && (SPC5_SARADC_USE_SARADC12_1 == TRUE)
#error "SARADC12_1 not present in the selected device"
#endif
#if (SPC5_HAS_SARADC12_2 == FALSE) && (SPC5_SARADC_USE_SARADC12_2 == TRUE)
#error "SARADC12_2 not present in the selected device"
#endif
#if (SPC5_HAS_SARADC12_3 == FALSE) && (SPC5_SARADC_USE_SARADC12_3 == TRUE)
#error "SARADC12_3 not present in the selected device"
#endif
#if (SPC5_HAS_SARADC12_4 == FALSE) && (SPC5_SARADC_USE_SARADC12_4 == TRUE)
#error "SARADC12_4 not present in the selected device"
#endif
#if (SPC5_HAS_SARADC12_5 == FALSE) && (SPC5_SARADC_USE_SARADC12_5 == TRUE)
#error "SARADC12_5 not present in the selected device"
#endif
#if (SPC5_HAS_SARADC12_6 == FALSE) && (SPC5_SARADC_USE_SARADC12_6 == TRUE)
#error "SARADC12_6 not present in the selected device"
#endif
#if (SPC5_HAS_SARADC12_SV == FALSE) && (SPC5_SARADC_USE_SARADC12_SV == TRUE)
#error "SARADC12_SV not present in the selected device"
#endif
#if (SPC5_HAS_SARADC12_SV_1 == FALSE) && (SPC5_SARADC_USE_SARADC12_SV_1 == TRUE)
#error "SARADC12_SV_1 not present in the selected device"
#endif
#if (SPC5_HAS_SARADC10_0 == FALSE) && (SPC5_SARADC_USE_SARADC10_0 == TRUE)
#error "SARADC10_0 not present in the selected device"
#endif
#if (SPC5_HAS_SARADC10_1 == FALSE) && (SPC5_SARADC_USE_SARADC10_1 == TRUE)
#error "SARADC10_1 not present in the selected device"
#endif
#if (SPC5_HAS_SARADC10_STANDBY == FALSE) && (SPC5_SARADC_USE_SARADC10_STANDBY == TRUE)
#error "SARADC10_STANDBY not present in the selected device"
#endif
#if    (SPC5_SARADC_USE_SARADC12_0 == FALSE) && (SPC5_SARADC_USE_SARADC12_1 == FALSE)  \
	&& (SPC5_SARADC_USE_SARADC12_2 == FALSE) && (SPC5_SARADC_USE_SARADC12_3 == FALSE)  \
	&& (SPC5_SARADC_USE_SARADC12_4 == FALSE) && (SPC5_SARADC_USE_SARADC12_5 == FALSE)  \
	&& (SPC5_SARADC_USE_SARADC12_6 == FALSE) && (SPC5_SARADC_USE_SARADC12_SV == FALSE) \
	&& (SPC5_SARADC_USE_SARADC10_0 == FALSE) && (SPC5_SARADC_USE_SARADC10_1 == FALSE)  \
	&& (SPC5_SARADC_USE_SARADC10_STANDBY == FALSE) && (SPC5_SARADC_USE_SARADC12_SV_1 == FALSE)
#error "SARADC driver activated but no SARADC peripheral assigned"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a structure representing an SARADC driver.
 */
typedef struct SARADCDriver SARADCDriver;

/**
 * @brief   SARADC notification callback type.
 *
 * @param[in] saradcp      pointer to the @p SARADCDriver object triggering the
 *                         callback
 */
typedef void (*saradccallback_t)(SARADCDriver *saradcp);

/**
 * @brief   SARADC Threshold notification callback type.
 *
 * @param[in] saradcp   pointer to the @p SARADCDriver object triggering the
 *                      callback
 * @param[in] buffer    pointer to the most recent samples data
 * @param[in] len       number of buffer rows available starting from @p buffer
 */
typedef void (*saradccallbackdma_t)(SARADCDriver *saradcp,uint16_t* buffer, uint16_t len);

/**
 * @brief   Conversion timing register definition.
 * @note
 */
typedef struct {
  /* resolution */
  uint8_t cres;

  /* precharge value*/
  uint8_t prechg;

  /* sampling phase duration*/
  uint8_t inpsamp;
} CTRregister;

/**
 * @brief   Channel configuration
 * @note
 */
typedef struct {
  /* Channel Type */
  uint16_t type;

  /* Channel Number*/
  uint16_t number;

  /* reference selection*/
  uint8_t reference;

  /*precharge enable*/
  uint8_t precharge;

  /*conversion timing register selection*/
  uint8_t ctr;

  /*Watchdog Threshold register associated*/
  uint8_t thr;
  
  /*out of range callback */
  saradccallback_t oorcallback;
} channelConfig;

/**
 * @brief   DMA conversion configuration
 * @note
 */
typedef struct {
  /* Channel Type */
  uint16_t type;

  /* Channel Number*/
  uint16_t number;
    
  /* reference selection*/
  uint8_t reference;

  /*precharge enable*/
  uint8_t precharge;

  /*conversion timing register selection*/
  uint8_t ctr;
  
  /* Number of consecutive channels*/
  uint16_t noofchannels;
  
  /* Number of conversions*/
  uint16_t noofconversions;
  
} dmaConfig;

/**
 * @brief   Threshold configuration
 * @note
 */
typedef struct {
	/* Low Threshold */
	float low;
	
	/* High Threshold */
	float high;

} thresholdConfig;
/**
 * @brief   Driver configuration structure.
 * @note
 */
typedef struct {
  /*Conversion mode */
  uint8_t mode;

  /* Trigger Mode */
  uint8_t trigger;
  
  /* CTRx register configuration */
  CTRregister ctrx[4];
  
  /* DMA enabled */
  uint8_t dma_enabled;

  /* DMA configuration */
  dmaConfig dmacfg;
  
  /*DMA callback */
  saradccallbackdma_t dmaconvcallback;

#if (SPC5_SARADC_MAX_NUMOFCHANNELS > 0U)
  /*channel configuration*/
  channelConfig ch[SPC5_SARADC_MAX_NUMOFCHANNELS];
#endif

  /*number of channel configured */
  uint16_t numofchannels;

  /*conversion callback */
  saradccallback_t convcallback;

  /* Threshold configuration */
  thresholdConfig thr[SPC5_SARADC_MAX_NUMOFTHRESHOLDS];
  
  /* SARADC voltage reference */
  float vref;
 
  /* number of threshold configured */
  uint8_t numofthresholds;
  
} SARADCConfig;

/**
 * @brief   Structure representing a SARADC driver.
 */
struct SARADCDriver {
  /**
   * @brief   Current configuration data.
   */
  SARADCConfig *config;
  
  /**
   * @brief Pointer to the SARADCx registers block.
   */
  volatile struct spc5_saradc *saradc;
#if (SPC5_SARADC_USE_DMA  == TRUE)
  /**
   * @brief   DMA Channel associated to Driver
   */
  uint8_t dma_channel;
  
  /**
   * @brief Pointer to the SARADCx registers block.
   */
  uint16_t dma_buffer[SPC5_SARADC_DMA_BUFFER_LENGHT];
#endif
  /**
   * @brief Number of driver instances running (only SUPERVISOR)
   */
   uint8_t noofinstances;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* IRQ Handlers                                                              */
/*===========================================================================*/

#if (SPC5_SARADC_USE_SARADC12_0 == TRUE)
IRQ_HANDLER(SPC5_SARADC12_0_HANDLER);
#endif
#if (SPC5_SARADC_USE_SARADC12_1 == TRUE)
IRQ_HANDLER(SPC5_SARADC12_1_HANDLER);
#endif
#if (SPC5_SARADC_USE_SARADC12_2 == TRUE)
IRQ_HANDLER(SPC5_SARADC12_2_HANDLER);
#endif
#if (SPC5_SARADC_USE_SARADC12_3 == TRUE)
IRQ_HANDLER(SPC5_SARADC12_3_HANDLER);
#endif
#if (SPC5_SARADC_USE_SARADC12_4 == TRUE)
IRQ_HANDLER(SPC5_SARADC12_4_HANDLER);
#endif
#if (SPC5_SARADC_USE_SARADC12_5 == TRUE)
IRQ_HANDLER(SPC5_SARADC12_5_HANDLER);
#endif
#if (SPC5_SARADC_USE_SARADC12_6 == TRUE)
IRQ_HANDLER(SPC5_SARADC12_6_HANDLER);
#endif
#if (SPC5_SARADC_USE_SARADC12_SV == TRUE)
IRQ_HANDLER(SPC5_SARADC12_SV_HANDLER);
#endif
#if (SPC5_SARADC_USE_SARADC12_SV_1 == TRUE)
IRQ_HANDLER(SPC5_SARADC12_SV_1_HANDLER);
#endif
#if (SPC5_SARADC_USE_SARADC10_0 == TRUE)
IRQ_HANDLER(SPC5_SARADC10_0_HANDLER);
#endif
#if (SPC5_SARADC_USE_SARADC10_1 == TRUE)
IRQ_HANDLER(SPC5_SARADC10_1_HANDLER);
#endif
#if (SPC5_SARADC_USE_SARADC10_STANDBY == TRUE)
IRQ_HANDLER(SPC5_SARADC10_STANDBY_HANDLER);
#endif

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if (SPC5_SARADC_USE_SARADC12_0 == TRUE) || defined(__DOXYGEN__)
extern SARADCDriver SARADC12D1;
#endif

#if (SPC5_SARADC_USE_SARADC12_1 == TRUE) || defined(__DOXYGEN__)
extern SARADCDriver SARADC12D2;
#endif

#if (SPC5_SARADC_USE_SARADC12_2 == TRUE) || defined(__DOXYGEN__)
extern SARADCDriver SARADC12D3;
#endif

#if (SPC5_SARADC_USE_SARADC12_3 == TRUE) || defined(__DOXYGEN__)
extern SARADCDriver SARADC12D4;
#endif

#if (SPC5_SARADC_USE_SARADC12_4 == TRUE) || defined(__DOXYGEN__)
extern SARADCDriver SARADC12D5;
#endif

#if (SPC5_SARADC_USE_SARADC12_5 == TRUE) || defined(__DOXYGEN__)
extern SARADCDriver SARADC12D6;
#endif

#if (SPC5_SARADC_USE_SARADC12_6 == TRUE) || defined(__DOXYGEN__)
extern SARADCDriver SARADC12D7;
#endif

#if (SPC5_SARADC_USE_SARADC12_SV == TRUE) || defined(__DOXYGEN__)
extern SARADCDriver SARADC12DSV;
#endif

#if (SPC5_SARADC_USE_SARADC12_SV_1 == TRUE) || defined(__DOXYGEN__)
extern SARADCDriver SARADC12DSV1;
#endif

#if (SPC5_SARADC_USE_SARADC10_0 == TRUE) || defined(__DOXYGEN__)
extern SARADCDriver SARADC10D1;
#endif

#if (SPC5_SARADC_USE_SARADC10_1 == TRUE) || defined(__DOXYGEN__)
extern SARADCDriver SARADC10D2;
#endif

#if (SPC5_SARADC_USE_SARADC10_STANDBY == TRUE) || defined(__DOXYGEN__)
extern SARADCDriver SARADC10DSB;
#endif

#ifdef __cplusplus
extern "C" {
#endif
void saradc_lld_init(void);
void saradc_lld_start(SARADCDriver *saradcp, SARADCConfig *config);
void saradc_lld_stop(SARADCDriver *saradcp);
void saradc_lld_start_conversion(SARADCDriver *saradcp);
void saradc_lld_stop_conversion(SARADCDriver *saradcp);
uint16_t saradc_lld_readchannel(SARADCDriver *saradcp, uint16_t channel);
#ifdef __cplusplus
}
#endif

#endif /* LLD_USE_SARADC */

#endif /* _SARADC_LLD_H_ */

/** @} */
