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
 * @file    lldconf.h
 * @brief   LLD configuration header.
 * @details LLD configuration file, this file allows to enable or disable the
 *          various device drivers from your application. You may also use
 *          this file in order to override the device drivers default settings.
 *
 * @addtogroup LLD_CONF
 * @{
 */

#ifndef _LLDCONF_H_
#define _LLDCONF_H_

/**
 * @name    Drivers enable switches
 */
#define LLD_USE_DMA                 TRUE
#define LLD_USE_PAL                 TRUE
#define LLD_USE_PIT                 FALSE
#define LLD_USE_STM                 FALSE
#define LLD_USE_SERIAL              FALSE
#define LLD_USE_PWM                 FALSE
#define LLD_USE_ICU                 FALSE
#define LLD_USE_CAN                 FALSE
#define LLD_USE_SPI                 FALSE
#define LLD_USE_ADC                 FALSE
#define LLD_USE_CTU                 FALSE
#define LLD_USE_I2C                 FALSE
#define LLD_USE_RTC                 FALSE
#define LLD_USE_SWT                 FALSE
#define LLD_USE_SARADC              FALSE
#define LLD_USE_FCCU                FALSE
#define LLD_USE_CRC                 FALSE
#define LLD_USE_WKPU                FALSE
#define LLD_USE_LIN                 FALSE
#define LLD_USE_I2S                 FALSE
/** @} */

/*===========================================================================*/
/**
 * @name SERIAL driver related setting
 * @{
 */
/*===========================================================================*/

/**
 * @name    SERIAL DMA modes
 * @{
 */
#define SPC5_SERIAL_DMA_OFF                 0
#define SPC5_SERIAL_DMA_ON                  1
/** @} */

/**
 * @name    LIN DMA modes
 * @{
 */
#define SPC5_LIN_DMA_OFF                    0
#define SPC5_LIN_DMA_ON                     1
/** @} */

/**
 * @name    LINFlex Mode Settings
 * @{
 */
#define SPC5_LINFLEX_MODE_NONE              0
#define SPC5_LINFLEX_MODE_SERIAL            1
#define SPC5_LINFLEX_MODE_LIN               2
/** @} */

/*===========================================================================*/
/**
 * @name SPI driver related setting
 * @{
 */
/*===========================================================================*/

/**
 * @brief   Enables synchronous APIs.
 * @note    Disabling this option saves both code and data space.
 */
#if !defined(SPI_USE_WAIT) || defined(__DOXYGEN__)
#define SPI_USE_WAIT                TRUE
#endif

/**
 * @brief   Enables the @p spiAcquireBus() and @p spiReleaseBus() APIs.
 * @note    Disabling this option saves both code and data space.
 */
#if !defined(SPI_USE_MUTUAL_EXCLUSION) || defined(__DOXYGEN__)
#define SPI_USE_MUTUAL_EXCLUSION    FALSE
#endif
/** @} */

/**
 * @name    SPI DMA modes
 * @{
 */
#define SPC5_SPI_DMA_NONE                   0
#define SPC5_SPI_DMA_RX_ONLY                1
#define SPC5_SPI_DMA_RX_AND_TX              2
/** @} */

/*
 * EDMA driver settings.
 */
#define SPC5_EDMA_CR_SETTING                (EDMA_CR_GRP1PRI(1) |           \
                                             EDMA_CR_GRP0PRI(0) |           \
                                             EDMA_CR_EMLM       |           \
                                             EDMA_CR_ERGA)
#define SPC5_EDMA1_GROUP0_PRIORITIES        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
#define SPC5_EDMA_COMBO_CH_ERR_IRQ_PRIO     10

/*
 * SERIAL driver system settings.
 */
#define SPC5_LINFLEX0_SETTING               SPC5_LINFLEX_MODE_NONE
#define SPC5_LINFLEX1_SETTING               SPC5_LINFLEX_MODE_NONE
#define SPC5_LINFLEX0_PRIORITY              INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#define SPC5_LINFLEX1_PRIORITY              INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#define SPC5_SERIAL_DMA_MODE                SPC5_SERIAL_DMA_OFF
#define SPC5_LIN_DMA_MODE                   SPC5_LIN_DMA_OFF

#define SPC5_SERIAL_LINFLEX0_TX_DMA_CH_ID   0
#define SPC5_SERIAL_LINFLEX0_RX_DMA_CH_ID   1
#define SPC5_SERIAL_LINFLEX1_TX_DMA_CH_ID   2
#define SPC5_SERIAL_LINFLEX1_RX_DMA_CH_ID   3
#define SPC5_SERIAL_LINFLEX0_DMA_IRQ_PRIO   INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#define SPC5_SERIAL_LINFLEX1_DMA_IRQ_PRIO   INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)

#define SPC5_LINFLEX0_RX_DMA_MUX            0U
#define SPC5_LINFLEX0_RX_DMA_DEV_ID         SPC5_LINFLEX0_RX_DMA_MUX0_DEV_ID
#define SPC5_LINFLEX0_TX_DMA_MUX            0U
#define SPC5_LINFLEX0_TX_DMA_DEV_ID         SPC5_LINFLEX0_TX_DMA_MUX0_DEV_ID
#define SPC5_LINFLEX1_RX_DMA_MUX            0U
#define SPC5_LINFLEX1_RX_DMA_DEV_ID         SPC5_LINFLEX1_RX_DMA_MUX0_DEV_ID
#define SPC5_LINFLEX1_TX_DMA_MUX            0U
#define SPC5_LINFLEX1_TX_DMA_DEV_ID         SPC5_LINFLEX1_TX_DMA_MUX0_DEV_ID

/*
 * CAN driver system settings.
 */
#define SPC5_CAN_USE_FILTERS                FALSE
#define SPC5_CAN_NUM_RX_MAILBOXES			8U
#define SPC5_CAN_NUM_TX_MAILBOXES			24U

#define SPC5_CAN_USE_FLEXCAN0               FALSE
#define SPC5_CAN_FLEXCAN0_USE_EXT_CLK       FALSE
#define SPC5_CAN_FLEXCAN0_IRQ_PRIORITY      INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 12)
#define SPC5_CAN_FLEXCAN0_START_PCTL        (SPC5_ME_PCTL_RUN(1) |  SPC5_ME_PCTL_LP(2))                                                   
#define SPC5_CAN_FLEXCAN0_STOP_PCTL         (SPC5_ME_PCTL_RUN(0) |  SPC5_ME_PCTL_LP(0))

#define SPC5_CAN_USE_FLEXCAN1               FALSE
#define SPC5_CAN_FLEXCAN1_USE_EXT_CLK       FALSE
#define SPC5_CAN_FLEXCAN1_IRQ_PRIORITY      INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 12)
#define SPC5_CAN_FLEXCAN1_START_PCTL        (SPC5_ME_PCTL_RUN(1) |  SPC5_ME_PCTL_LP(2))                                                   
#define SPC5_CAN_FLEXCAN1_STOP_PCTL         (SPC5_ME_PCTL_RUN(0) |  SPC5_ME_PCTL_LP(0))


/*
* PIT driver system settings.
*/
#define PIT_CHANNEL_CH0_ENABLED				FALSE
#define PIT_CHANNEL_CH1_ENABLED				FALSE
#define PIT_CHANNEL_CH2_ENABLED				FALSE
#define PIT_CHANNEL_CH3_ENABLED				FALSE
#define PIT_CHANNEL_CH4_ENABLED				FALSE
#define PIT_CHANNEL_CH5_ENABLED				FALSE
#define PIT_CHANNEL_CH6_ENABLED				FALSE
#define PIT_CHANNEL_CH7_ENABLED				FALSE
#define SPC5_PIT_START_PCTL                 (SPC5_ME_PCTL_RUN(2) |          \
                                             SPC5_ME_PCTL_LP(2))
#define SPC5_PIT_STOP_PCTL                  (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#define SPC5_PIT_CH0_IRQ_PRIO               INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 4)
#define SPC5_PIT_CH1_IRQ_PRIO               INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 12)
#define SPC5_PIT_CH2_IRQ_PRIO               INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 12)
#define SPC5_PIT_CH3_IRQ_PRIO               INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 12)
/*
* STM driver system settings.
*/
#define SPC5_STM_USE_STM0                   FALSE
#define SPC5_STM0_SYSCLOCK_PRE              1
#define SPC5_STM0_CH0_ENABLED               FALSE
#define SPC5_STM0_CH0_IRQ_PRIORITY          INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 11)  
#define SPC5_STM0_CH1_ENABLED               FALSE
#define SPC5_STM0_CH1_IRQ_PRIORITY          INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 11)  
#define SPC5_STM0_CH2_ENABLED               FALSE
#define SPC5_STM0_CH2_IRQ_PRIORITY          INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 11)  
#define SPC5_STM0_CH3_ENABLED               FALSE
#define SPC5_STM0_CH3_IRQ_PRIORITY          INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 12)  
  


/*
 * SPI driver system settings.
 */
#define SPC5_SPI_USE_DSPI0                  FALSE
#define SPC5_SPI_USE_DSPI1                  FALSE
#define SPC5_SPI_USE_DSPI2                  FALSE
#define SPC5_SPI_DMA_MODE                   SPC5_SPI_DMA_NONE
#define SPC5_SPI_DSPI0_MCR                  (0U | SPC5_MCR_PCSIS0 | SPC5_MCR_PCSIS1 | SPC5_MCR_PCSIS2 | SPC5_MCR_PCSIS3 | SPC5_MCR_PCSIS4 | SPC5_MCR_PCSIS5 | SPC5_MCR_PCSIS6 | SPC5_MCR_PCSIS7)
#define SPC5_SPI_DSPI1_MCR                  (0U | SPC5_MCR_PCSIS0 | SPC5_MCR_PCSIS1 | SPC5_MCR_PCSIS2 | SPC5_MCR_PCSIS3 | SPC5_MCR_PCSIS4 | SPC5_MCR_PCSIS5 | SPC5_MCR_PCSIS6 | SPC5_MCR_PCSIS7)
#define SPC5_SPI_DSPI2_MCR                  (0U | SPC5_MCR_PCSIS0 | SPC5_MCR_PCSIS1 | SPC5_MCR_PCSIS2 | SPC5_MCR_PCSIS3 | SPC5_MCR_PCSIS4 | SPC5_MCR_PCSIS5 | SPC5_MCR_PCSIS6 | SPC5_MCR_PCSIS7)

#define SPC5_SPI_DSPI0_IRQ_PRIO             INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 10) 
#define SPC5_SPI_DSPI1_IRQ_PRIO             INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 10)
#define SPC5_SPI_DSPI2_IRQ_PRIO             INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 10)

/*
 * SARADC driver system settings.
 */ 
#define SPC5_SARADC_USE_SARADC12_0			FALSE
#define SPC5_SARADC12_0_PRIORITY			INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 10)

#define SPC5_SARADC_USE_SARADC12_SV			FALSE
#define SPC5_SARADC12_SV_PRIORITY			INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 10)


#define SPC5_SARADC_USE_DMA                 FALSE
#define SPC5_SARADC_DMA_BUFFER_LENGHT       0U
#define SPC5_SARADC_MAX_NUMOFCHANNELS	    0U
#define SPC5_SARADC_MAX_NUMOFTHRESHOLDS     1U
#define SPC5_SARADC_DMA_ERROR_HOOK(saradcp) irqSysHalt()

/*
 * eTIMER (PWM/ICU) driver system settings.
 */
#define SPC5_PWM_USE_ETIMER0_CH0            FALSE
#define SPC5_PWM_USE_ETIMER0_CH1            FALSE
#define SPC5_PWM_USE_ETIMER0_CH2            FALSE
#define SPC5_PWM_USE_ETIMER0_CH3            FALSE
#define SPC5_PWM_USE_ETIMER0_CH4            FALSE
#define SPC5_PWM_USE_ETIMER0_CH5            FALSE
#define SPC5_PWM_USE_ETIMER1_CH0            FALSE
#define SPC5_PWM_USE_ETIMER1_CH1            FALSE
#define SPC5_PWM_USE_ETIMER1_CH2            FALSE
#define SPC5_PWM_USE_ETIMER1_CH3            FALSE
#define SPC5_PWM_USE_ETIMER1_CH4            FALSE
#define SPC5_PWM_USE_ETIMER1_CH5            FALSE
#define SPC5_PWM_USE_ETIMER2_CH0            FALSE
#define SPC5_PWM_USE_ETIMER2_CH1            FALSE
#define SPC5_PWM_USE_ETIMER2_CH2            FALSE
#define SPC5_PWM_USE_ETIMER2_CH3            FALSE
#define SPC5_PWM_USE_ETIMER2_CH4            FALSE
#define SPC5_PWM_USE_ETIMER2_CH5            FALSE
#define SPC5_PWM_USE_ETIMER3_CH0            FALSE
#define SPC5_PWM_USE_ETIMER3_CH1            FALSE
#define SPC5_PWM_USE_ETIMER3_CH2            FALSE
#define SPC5_PWM_USE_ETIMER3_CH3            FALSE
#define SPC5_PWM_USE_ETIMER3_CH4            FALSE
#define SPC5_PWM_USE_ETIMER3_CH5            FALSE

#define SPC5_ICU_USE_ETIMER0_CH0            FALSE
#define SPC5_ICU_USE_ETIMER0_CH1            FALSE
#define SPC5_ICU_USE_ETIMER0_CH2            FALSE
#define SPC5_ICU_USE_ETIMER0_CH3            FALSE
#define SPC5_ICU_USE_ETIMER0_CH4            FALSE
#define SPC5_ICU_USE_ETIMER0_CH5            FALSE
#define SPC5_ICU_USE_ETIMER1_CH0            FALSE
#define SPC5_ICU_USE_ETIMER1_CH1            FALSE
#define SPC5_ICU_USE_ETIMER1_CH2            FALSE
#define SPC5_ICU_USE_ETIMER1_CH3            FALSE
#define SPC5_ICU_USE_ETIMER1_CH4            FALSE
#define SPC5_ICU_USE_ETIMER1_CH5            FALSE
#define SPC5_ICU_USE_ETIMER2_CH0            FALSE
#define SPC5_ICU_USE_ETIMER2_CH1            FALSE
#define SPC5_ICU_USE_ETIMER2_CH2            FALSE
#define SPC5_ICU_USE_ETIMER2_CH3            FALSE
#define SPC5_ICU_USE_ETIMER2_CH4            FALSE
#define SPC5_ICU_USE_ETIMER2_CH5            FALSE
#define SPC5_ICU_USE_ETIMER3_CH0            FALSE
#define SPC5_ICU_USE_ETIMER3_CH1            FALSE
#define SPC5_ICU_USE_ETIMER3_CH2            FALSE
#define SPC5_ICU_USE_ETIMER3_CH3            FALSE
#define SPC5_ICU_USE_ETIMER3_CH4            FALSE
#define SPC5_ICU_USE_ETIMER3_CH5            FALSE

#define SPC5_ETIMER0_CH0_PRIORITY           INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#define SPC5_ETIMER0_CH1_PRIORITY           INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#define SPC5_ETIMER0_CH2_PRIORITY           INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#define SPC5_ETIMER0_CH3_PRIORITY           INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#define SPC5_ETIMER0_CH4_PRIORITY           INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#define SPC5_ETIMER0_CH5_PRIORITY           INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#define SPC5_ETIMER1_CH0_PRIORITY           INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#define SPC5_ETIMER1_CH1_PRIORITY           INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#define SPC5_ETIMER1_CH2_PRIORITY           INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#define SPC5_ETIMER1_CH3_PRIORITY           INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#define SPC5_ETIMER1_CH4_PRIORITY           INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#define SPC5_ETIMER1_CH5_PRIORITY           INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#define SPC5_ETIMER2_CH0_PRIORITY           INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#define SPC5_ETIMER2_CH1_PRIORITY           INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#define SPC5_ETIMER2_CH2_PRIORITY           INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#define SPC5_ETIMER2_CH3_PRIORITY           INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#define SPC5_ETIMER2_CH4_PRIORITY           INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#define SPC5_ETIMER2_CH5_PRIORITY           INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#define SPC5_ETIMER3_CH0_PRIORITY           INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#define SPC5_ETIMER3_CH1_PRIORITY           INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#define SPC5_ETIMER3_CH2_PRIORITY           INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#define SPC5_ETIMER3_CH3_PRIORITY           INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#define SPC5_ETIMER3_CH4_PRIORITY           INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#define SPC5_ETIMER3_CH5_PRIORITY           INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)

#endif /* _LLDCONF_H_ */

/** @} */
