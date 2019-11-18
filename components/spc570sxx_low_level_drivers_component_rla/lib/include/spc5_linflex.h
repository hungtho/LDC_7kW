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
 * @file    spc5_linflex.h
 * @brief   LINFlex helper driver header.
 *
 * @addtogroup SPC5xx_LINFLEX
 * @{
 */

#ifndef _SPC5_LINFLEX_H_
#define _SPC5_LINFLEX_H_

#include "spc5_lld.h"
#include "lldconf.h"

#if (LLD_USE_SERIAL == TRUE) ||(LLD_USE_LIN == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    LINIER register bits definitions
 * @{
 */
#define SPC5_LINIER_HRIE                    (1UL << 0)
#define SPC5_LINIER_DTIE                    (1UL << 1)
#define SPC5_LINIER_DRIE                    (1UL << 2)
#define SPC5_LINIER_DBEIE                   (1UL << 3)
#define SPC5_LINIER_DBFIE                   (1UL << 4)
#define SPC5_LINIER_WUIE                    (1UL << 5)
#define SPC5_LINIER_LSIE                    (1UL << 6)
#define SPC5_LINIER_BOIE                    (1UL << 7)
#define SPC5_LINIER_FEIE                    (1UL << 8)
#define SPC5_LINIER_HEIE                    (1UL << 11)
#define SPC5_LINIER_CEIE                    (1UL << 12)
#define SPC5_LINIER_BEIE                    (1UL << 13)
#define SPC5_LINIER_OCIE                    (1UL << 14)
#define SPC5_LINIER_SZIE                    (1UL << 15)
/** @} */

/**
 * @name    LINSR register bits definitions
 * @{
 */
#define SPC5_LINSR_HRF                      (1UL << 0)
#define SPC5_LINSR_DTF                      (1UL << 1)
#define SPC5_LINSR_DRF                      (1UL << 2)
#define SPC5_LINSR_DBEF                     (1UL << 3)
#define SPC5_LINSR_DBFF                     (1UL << 4)
#define SPC5_LINSR_WUF                      (1UL << 5)
#define SPC5_LINSR_RDI                      (1UL << 6)
#define SPC5_LINSR_RXBUSY                   (1UL << 7)
#define SPC5_LINSR_DRBNE                    (1UL << 8)
#define SPC5_LINSR_RMB                      (1UL << 9)
#define SPC5_LINSR_LINS                     (15UL << 12)
/** @} */

/**
 * @name    LINSR State register bit definitions
 * @{
 */
#define SPC5_LINSR_LINS_SLEEP               0U
#define SPC5_LINSR_LINS_INIT                1U
#define SPC5_LINSR_LINS_IDLE                2U
#define SPC5_LINSR_LINS_SYNCBREAK           3U
#define SPC5_LINSR_LINS_SYNCDEL             4U
#define SPC5_LINSR_LINS_SYNCFIELD           5U
#define SPC5_LINSR_LINS_IDFIELD             6U
#define SPC5_LINSR_LINS_HEADERRXTX          7U
#define SPC5_LINSR_LINS_DATARXTX            8U
#define SPC5_LINSR_LINS_CHECKSUM            9U
/** @} */

/**
 * @name    LINESR register bits definitions
 * @{
 */
#define SPC5_LINESR_NF                      (1UL << 0)
#define SPC5_LINESR_BOF                     (1UL << 7)
#define SPC5_LINESR_FEF                     (1UL << 8)
#define SPC5_LINESR_IDPEF                   (1UL << 9)
#define SPC5_LINESR_SDEF                    (1UL << 10)
#define SPC5_LINESR_SFEF                    (1UL << 11)
#define SPC5_LINESR_CEF                     (1UL << 12)
#define SPC5_LINESR_BEF                     (1UL << 13)
#define SPC5_LINESR_OCF                     (1UL << 14)
#define SPC5_LINESR_SZF                     (1UL << 15)
/** @} */

/**
 * @name    UARTSR register bits definitions
 * @{
 */
#define SPC5_UARTSR_NF                      (1UL << 0)
#define SPC5_UARTSR_DTF                     (1UL << 1)
#define SPC5_UARTSR_DRF                     (1UL << 2)
#define SPC5_UARTSR_WUF                     (1UL << 5)
#define SPC5_UARTSR_RPS                     (1UL << 6)
#define SPC5_UARTSR_BOF                     (1UL << 7)
#define SPC5_UARTSR_FEF                     (1UL << 8)
#define SPC5_UARTSR_RMB                     (1UL << 9)
#define SPC5_UARTSR_PE0                     (1UL << 10)
#define SPC5_UARTSR_PE1                     (1UL << 11)
#define SPC5_UARTSR_PE2                     (1UL << 12)
#define SPC5_UARTSR_PE3                     (1UL << 13)
#define SPC5_UARTSR_OCF                     (1UL << 14)
#define SPC5_UARTSR_SZF                     (1UL << 15)
/** @} */

/**
 * @name    UARTCR register bits definitions
 * @{
 */
#define SPC5_UARTCR_UART                    (1U << 0)
#define SPC5_UARTCR_WL                      (1U << 1)
#define SPC5_UARTCR_PCE                     (1U << 2)
#define SPC5_UARTCR_OP                      (1U << 3)
#define SPC5_UARTCR_TXEN                    (1U << 4)
#define SPC5_UARTCR_RXEN                    (1U << 5)
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   LINFlex-0 driver enable switch.
 * @details It is set to the device use the LINFlex-0, one of the following choice:
 *          @p SPC5_LINFLEX_MODE_SERIAL for serial device.
 *          @p SPC5_LINFLEX_MODE_LIN    for lin device.
 *          @p SPC5_LINFLEX_MODE_NONE   no device using LINFlex-0.
 */
#if !defined(SPC5_LINFLEX0_SETTING) || defined(__DOXYGEN__)
#define SPC5_LINFLEX0_SETTING               SPC5_LINFLEX_MODE_NONE
#endif

/**
 * @brief   LINFlex-1 driver enable switch.
 * @details It is set to the device use the LINFlex-1, one of the following choice:
 *          @p SPC5_LINFLEX_MODE_SERIAL for serial device.
 *          @p SPC5_LINFLEX_MODE_LIN    for lin device.
 *          @p SPC5_LINFLEX_MODE_NONE   no device using LINFlex-1.
 */
#if !defined(SPC5_LINFLEX1_SETTING) || defined(__DOXYGEN__)
#define SPC5_LINFLEX1_SETTING               SPC5_LINFLEX_MODE_NONE
#endif

/**
 * @brief   LINFlex-2 driver enable switch.
 * @details It is set to the device use the LINFlex-2, one of the following choice:
 *          @p SPC5_LINFLEX_MODE_SERIAL for serial device.
 *          @p SPC5_LINFLEX_MODE_LIN    for lin device.
 *          @p SPC5_LINFLEX_MODE_NONE   no device using LINFlex-2.
 */
#if !defined(SPC5_LINFLEX2_SETTING) || defined(__DOXYGEN__)
#define SPC5_LINFLEX2_SETTING               SPC5_LINFLEX_MODE_NONE
#endif

/**
 * @brief   LINFlex-3 driver enable switch.
 * @details It is set to the device use the LINFlex-3, one of the following choice:
 *          @p SPC5_LINFLEX_MODE_SERIAL for serial device.
 *          @p SPC5_LINFLEX_MODE_LIN    for lin device.
 *          @p SPC5_LINFLEX_MODE_NONE   no device using LINFlex-3.
 */
#if !defined(SPC5_LINFLEX3_SETTING) || defined(__DOXYGEN__)
#define SPC5_LINFLEX3_SETTING               SPC5_LINFLEX_MODE_NONE
#endif

/**
 * @brief   LINFlex-4 driver enable switch.
 * @details It is set to the device use the LINFlex-4, one of the following choice:
 *          @p SPC5_LINFLEX_MODE_SERIAL for serial device.
 *          @p SPC5_LINFLEX_MODE_LIN    for lin device.
 *          @p SPC5_LINFLEX_MODE_NONE   no device using LINFlex-4.
 */
#if !defined(SPC5_LINFLEX4_SETTING) || defined(__DOXYGEN__)
#define SPC5_LINFLEX4_SETTING               SPC5_LINFLEX_MODE_NONE
#endif

/**
 * @brief   LINFlex-5 driver enable switch.
 * @details It is set to the device use the LINFlex-5, one of the following choice:
 *          @p SPC5_LINFLEX_MODE_SERIAL for serial device.
 *          @p SPC5_LINFLEX_MODE_LIN    for lin device.
 *          @p SPC5_LINFLEX_MODE_NONE   no device using LINFlex-5.
 */
#if !defined(SPC5_LINFLEX5_SETTING) || defined(__DOXYGEN__)
#define SPC5_LINFLEX5_SETTING               SPC5_LINFLEX_MODE_NONE
#endif

/**
 * @brief   LINFlex-6 driver enable switch.
 * @details It is set to the device use the LINFlex-6, one of the following choice:
 *          @p SPC5_LINFLEX_MODE_SERIAL for serial device.
 *          @p SPC5_LINFLEX_MODE_LIN    for lin device.
 *          @p SPC5_LINFLEX_MODE_NONE   no device using LINFlex-6.
 */
#if !defined(SPC5_LINFLEX6_SETTING) || defined(__DOXYGEN__)
#define SPC5_LINFLEX6_SETTING               SPC5_LINFLEX_MODE_NONE
#endif

/**
 * @brief   LINFlex-7 driver enable switch.
 * @details It is set to the device use the LINFlex-7, one of the following choice:
 *          @p SPC5_LINFLEX_MODE_SERIAL for serial device.
 *          @p SPC5_LINFLEX_MODE_LIN    for lin device.
 *          @p SPC5_LINFLEX_MODE_NONE   no device using LINFlex-7.
 */
#if !defined(SPC5_LINFLEX7_SETTING) || defined(__DOXYGEN__)
#define SPC5_LINFLEX7_SETTING               SPC5_LINFLEX_MODE_NONE
#endif

/**
 * @brief   LINFlex-8 driver enable switch.
 * @details It is set to the device use the LINFlex-8, one of the following choice:
 *          @p SPC5_LINFLEX_MODE_SERIAL for serial device.
 *          @p SPC5_LINFLEX_MODE_LIN    for lin device.
 *          @p SPC5_LINFLEX_MODE_NONE   no device using LINFlex-8.
 */
#if !defined(SPC5_LINFLEX8_SETTING) || defined(__DOXYGEN__)
#define SPC5_LINFLEX8_SETTING               SPC5_LINFLEX_MODE_NONE
#endif

/**
 * @brief   LINFlex-9 driver enable switch.
 * @details It is set to the device use the LINFlex-9, one of the following choice:
 *          @p SPC5_LINFLEX_MODE_SERIAL for serial device.
 *          @p SPC5_LINFLEX_MODE_LIN    for lin device.
 *          @p SPC5_LINFLEX_MODE_NONE   no device using LINFlex-9.
 */
#if !defined(SPC5_LINFLEX9_SETTING) || defined(__DOXYGEN__)
#define SPC5_LINFLEX9_SETTING               SPC5_LINFLEX_MODE_NONE
#endif

/**
 * @brief   LINFlex-10 driver enable switch.
 * @details It is set to the device use the LINFlex-10, one of the following choice:
 *          @p SPC5_LINFLEX_MODE_SERIAL for serial device.
 *          @p SPC5_LINFLEX_MODE_LIN    for lin device.
 *          @p SPC5_LINFLEX_MODE_NONE   no device using LINFlex-10.
 */
#if !defined(SPC5_LINFLEX10_SETTING) || defined(__DOXYGEN__)
#define SPC5_LINFLEX10_SETTING              SPC5_LINFLEX_MODE_NONE
#endif

/**
 * @brief   LINFlex-11 driver enable switch.
 * @details It is set to the device use the LINFlex-11, one of the following choice:
 *          @p SPC5_LINFLEX_MODE_SERIAL for serial device.
 *          @p SPC5_LINFLEX_MODE_LIN    for lin device.
 *          @p SPC5_LINFLEX_MODE_NONE   no device using LINFlex-11.
 */
#if !defined(SPC5_LINFLEX11_SETTING) || defined(__DOXYGEN__)
#define SPC5_LINFLEX11_SETTING              SPC5_LINFLEX_MODE_NONE
#endif

/**
 * @brief   LINFlex-12 driver enable switch.
 * @details It is set to the device use the LINFlex-12, one of the following choice:
 *          @p SPC5_LINFLEX_MODE_SERIAL for serial device.
 *          @p SPC5_LINFLEX_MODE_LIN    for lin device.
 *          @p SPC5_LINFLEX_MODE_NONE   no device using LINFlex-12.
 */
#if !defined(SPC5_LINFLEX12_SETTING) || defined(__DOXYGEN__)
#define SPC5_LINFLEX12_SETTING              SPC5_LINFLEX_MODE_NONE
#endif

/**
 * @brief   LINFlex-13 driver enable switch.
 * @details It is set to the device use the LINFlex-13, one of the following choice:
 *          @p SPC5_LINFLEX_MODE_SERIAL for serial device.
 *          @p SPC5_LINFLEX_MODE_LIN    for lin device.
 *          @p SPC5_LINFLEX_MODE_NONE   no device using LINFlex-13.
 */
#if !defined(SPC5_LINFLEX13_SETTING) || defined(__DOXYGEN__)
#define SPC5_LINFLEX13_SETTING              SPC5_LINFLEX_MODE_NONE
#endif

/**
 * @brief   LINFlex-14 driver enable switch.
 * @details It is set to the device use the LINFlex-14, one of the following choice:
 *          @p SPC5_LINFLEX_MODE_SERIAL for serial device.
 *          @p SPC5_LINFLEX_MODE_LIN    for lin device.
 *          @p SPC5_LINFLEX_MODE_NONE   no device using LINFlex-14.
 */
#if !defined(SPC5_LINFLEX14_SETTING) || defined(__DOXYGEN__)
#define SPC5_LINFLEX14_SETTING              SPC5_LINFLEX_MODE_NONE
#endif

/**
 * @brief   LINFlex-15 driver enable switch.
 * @details It is set to the device use the LINFlex-15, one of the following choice:
 *          @p SPC5_LINFLEX_MODE_SERIAL for serial device.
 *          @p SPC5_LINFLEX_MODE_LIN    for lin device.
 *          @p SPC5_LINFLEX_MODE_NONE   no device using LINFlex-15.
 */
#if !defined(SPC5_LINFLEX15_SETTING) || defined(__DOXYGEN__)
#define SPC5_LINFLEX15_SETTING              SPC5_LINFLEX_MODE_NONE
#endif

/**
 * @brief   LINFlex-16 driver enable switch.
 * @details It is set to the device use the LINFlex-16, one of the following choice:
 *          @p SPC5_LINFLEX_MODE_SERIAL for serial device.
 *          @p SPC5_LINFLEX_MODE_LIN    for lin device.
 *          @p SPC5_LINFLEX_MODE_NONE   no device using LINFlex-16.
 */
#if !defined(SPC5_LINFLEX16_SETTING) || defined(__DOXYGEN__)
#define SPC5_LINFLEX16_SETTING              SPC5_LINFLEX_MODE_NONE
#endif

/**
 * @brief   LINFlex-17 driver enable switch.
 * @details It is set to the device use the LINFlex-17, one of the following choice:
 *          @p SPC5_LINFLEX_MODE_SERIAL for serial device.
 *          @p SPC5_LINFLEX_MODE_LIN    for lin device.
 *          @p SPC5_LINFLEX_MODE_NONE   no device using LINFlex-17.
 */
#if !defined(SPC5_LINFLEX17_SETTING) || defined(__DOXYGEN__)
#define SPC5_LINFLEX17_SETTING              SPC5_LINFLEX_MODE_NONE
#endif

/**
 * @brief   LINFlex-18 driver enable switch.
 * @details It is set to the device use the LINFlex-18, one of the following choice:
 *          @p SPC5_LINFLEX_MODE_SERIAL for serial device.
 *          @p SPC5_LINFLEX_MODE_LIN    for lin device.
 *          @p SPC5_LINFLEX_MODE_NONE   no device using LINFlex-18.
 */
#if !defined(SPC5_LINFLEX18_SETTING) || defined(__DOXYGEN__)
#define SPC5_LINFLEX18_SETTING              SPC5_LINFLEX_MODE_NONE
#endif

/**
 * @brief   LINFlex-19 driver enable switch.
 * @details It is set to the device use the LINFlex-19, one of the following choice:
 *          @p SPC5_LINFLEX_MODE_SERIAL for serial device.
 *          @p SPC5_LINFLEX_MODE_LIN    for lin device.
 *          @p SPC5_LINFLEX_MODE_NONE   no device using LINFlex-19.
 */
#if !defined(SPC5_LINFLEX19_SETTING) || defined(__DOXYGEN__)
#define SPC5_LINFLEX19_SETTING              SPC5_LINFLEX_MODE_NONE
#endif

/**
 * @brief   LINFlex-20 driver enable switch.
 * @details It is set to the device use the LINFlex-20, one of the following choice:
 *          @p SPC5_LINFLEX_MODE_SERIAL for serial device.
 *          @p SPC5_LINFLEX_MODE_LIN    for lin device.
 *          @p SPC5_LINFLEX_MODE_NONE   no device using LINFlex-20.
 */
#if !defined(SPC5_LINFLEX20_SETTING) || defined(__DOXYGEN__)
#define SPC5_LINFLEX20_SETTING              SPC5_LINFLEX_MODE_NONE
#endif

/**
 * @brief   LINFlex-21 driver enable switch.
 * @details It is set to the device use the LINFlex-21, one of the following choice:
 *          @p SPC5_LINFLEX_MODE_SERIAL for serial device.
 *          @p SPC5_LINFLEX_MODE_LIN    for lin device.
 *          @p SPC5_LINFLEX_MODE_NONE   no device using LINFlex-21.
 */
#if !defined(SPC5_LINFLEX21_SETTING) || defined(__DOXYGEN__)
#define SPC5_LINFLEX21_SETTING              SPC5_LINFLEX_MODE_NONE
#endif

/**
 * @brief   LINFlex-22 driver enable switch.
 * @details It is set to the device use the LINFlex-22, one of the following choice:
 *          @p SPC5_LINFLEX_MODE_SERIAL for serial device.
 *          @p SPC5_LINFLEX_MODE_LIN    for lin device.
 *          @p SPC5_LINFLEX_MODE_NONE   no device using LINFlex-22.
 */
#if !defined(SPC5_LINFLEX22_SETTING) || defined(__DOXYGEN__)
#define SPC5_LINFLEX22_SETTING              SPC5_LINFLEX_MODE_NONE
#endif

/**
 * @brief   LINFlex-23 driver enable switch.
 * @details It is set to the device use the LINFlex-23, one of the following choice:
 *          @p SPC5_LINFLEX_MODE_SERIAL for serial device.
 *          @p SPC5_LINFLEX_MODE_LIN    for lin device.
 *          @p SPC5_LINFLEX_MODE_NONE   no device using LINFlex-23.
 */
#if !defined(SPC5_LINFLEX23_SETTING) || defined(__DOXYGEN__)
#define SPC5_LINFLEX23_SETTING              SPC5_LINFLEX_MODE_NONE
#endif

/**
 * @brief   LINFlex-0 interrupt priority level setting.
 */
#if !defined(SPC5_LINFLEX0_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_LINFLEX0_PRIORITY              INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#endif

/**
 * @brief   LINFlex-1 interrupt priority level setting.
 */
#if !defined(SPC5_LINFLEX1_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_LINFLEX1_PRIORITY              INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#endif

/**
 * @brief   LINFlex-2 interrupt priority level setting.
 */
#if !defined(SPC5_LINFLEX2_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_LINFLEX2_PRIORITY              INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#endif

/**
 * @brief   LINFlex-3 interrupt priority level setting.
 */
#if !defined(SPC5_LINFLEX3_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_LINFLEX3_PRIORITY              INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#endif

/**
 * @brief   LINFlex-4 interrupt priority level setting.
 */
#if !defined(SPC5_LINFLEX4_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_LINFLEX4_PRIORITY              INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#endif

/**
 * @brief   LINFlex-5 interrupt priority level setting.
 */
#if !defined(SPC5_LINFLEX5_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_LINFLEX5_PRIORITY              INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#endif

/**
 * @brief   LINFlex-6 interrupt priority level setting.
 */
#if !defined(SPC5_LINFLEX6_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_LINFLEX6_PRIORITY              INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#endif

/**
 * @brief   LINFlex-7 interrupt priority level setting.
 */
#if !defined(SPC5_LINFLEX7_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_LINFLEX7_PRIORITY              INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#endif

/**
 * @brief   LINFlex-8 interrupt priority level setting.
 */
#if !defined(SPC5_LINFLEX8_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_LINFLEX8_PRIORITY              INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#endif

/**
 * @brief   LINFlex-9 interrupt priority level setting.
 */
#if !defined(SPC5_LINFLEX9_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_LINFLEX9_PRIORITY              INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#endif

/**
 * @brief   LINFlex-10 interrupt priority level setting.
 */
#if !defined(SPC5_LINFLEX10_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_LINFLEX10_PRIORITY             INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#endif

/**
 * @brief   LINFlex-11 interrupt priority level setting.
 */
#if !defined(SPC5_LINFLEX11_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_LINFLEX11_PRIORITY             INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#endif

/**
 * @brief   LINFlex-12 interrupt priority level setting.
 */
#if !defined(SPC5_LINFLEX12_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_LINFLEX12_PRIORITY             INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#endif

/**
 * @brief   LINFlex-13 interrupt priority level setting.
 */
#if !defined(SPC5_LINFLEX13_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_LINFLEX13_PRIORITY             INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#endif

/**
 * @brief   LINFlex-14 interrupt priority level setting.
 */
#if !defined(SPC5_LINFLEX14_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_LINFLEX14_PRIORITY             INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#endif

/**
 * @brief   LINFlex-15 interrupt priority level setting.
 */
#if !defined(SPC5_LINFLEX15_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_LINFLEX15_PRIORITY             INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#endif

/**
 * @brief   LINFlex-16 interrupt priority level setting.
 */
#if !defined(SPC5_LINFLEX16_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_LINFLEX16_PRIORITY             INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#endif

/**
 * @brief   LINFlex-17 interrupt priority level setting.
 */
#if !defined(SPC5_LINFLEX17_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_LINFLEX17_PRIORITY             INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#endif

/**
 * @brief   LINFlex-18 interrupt priority level setting.
 */
#if !defined(SPC5_LINFLEX18_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_LINFLEX18_PRIORITY             INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#endif

/**
 * @brief   LINFlex-19 interrupt priority level setting.
 */
#if !defined(SPC5_LINFLEX19_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_LINFLEX19_PRIORITY             INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#endif

/**
 * @brief   LINFlex-20 interrupt priority level setting.
 */
#if !defined(SPC5_LINFLEX20_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_LINFLEX20_PRIORITY             INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#endif

/**
 * @brief   LINFlex-21 interrupt priority level setting.
 */
#if !defined(SPC5_LINFLEX21_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_LINFLEX21_PRIORITY             INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#endif

/**
 * @brief   LINFlex-22 interrupt priority level setting.
 */
#if !defined(SPC5_LINFLEX22_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_LINFLEX22_PRIORITY             INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#endif

/**
 * @brief   LINFlex-23 interrupt priority level setting.
 */
#if !defined(SPC5_LINFLEX23_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_LINFLEX23_PRIORITY             INTC_PSR_ENABLE(INTC_PSR_MAINCORE, 8)
#endif

/**
 * @brief   LINFlex-0 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode. 
 */
#if !defined(SPC5_LINFLEX0_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX0_START_PCTL            (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-0 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode. 
 */
#if !defined(SPC5_LINFLEX0_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX0_STOP_PCTL             (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   LINFlex-1 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode. 
 */
#if !defined(SPC5_LINFLEX1_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX1_START_PCTL            (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-1 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_LINFLEX1_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX1_STOP_PCTL             (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   LINFlex-2 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_LINFLEX2_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX2_START_PCTL            (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-2 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_LINFLEX2_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX2_STOP_PCTL             (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   LINFlex-3 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode. 
 */
#if !defined(SPC5_LINFLEX3_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX3_START_PCTL            (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-3 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_LINFLEX3_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX3_STOP_PCTL             (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   LINFlex-4 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_LINFLEX4_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX4_START_PCTL            (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-4 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_LINFLEX4_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX4_STOP_PCTL             (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   LINFlex-5 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_LINFLEX5_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX5_START_PCTL            (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-5 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_LINFLEX5_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX5_STOP_PCTL             (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   LINFlex-6 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_LINFLEX6_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX6_START_PCTL            (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-6 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_LINFLEX6_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX6_STOP_PCTL             (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   LINFlex-7 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_LINFLEX7_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX7_START_PCTL            (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-7 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_LINFLEX7_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX7_STOP_PCTL             (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   LINFlex-8 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_LINFLEX8_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX8_START_PCTL            (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-8 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_LINFLEX8_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX8_STOP_PCTL             (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   LINFlex-9 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_LINFLEX9_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX9_START_PCTL            (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-9 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_LINFLEX9_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX9_STOP_PCTL             (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   LINFlex-10 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_LINFLEX10_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX10_START_PCTL           (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-10 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_LINFLEX10_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX10_STOP_PCTL            (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif


/**
 * @brief   LINFlex-11 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_LINFLEX11_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX11_START_PCTL           (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-11 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_LINFLEX11_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX11_STOP_PCTL            (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   LINFlex-12 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_LINFLEX12_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX12_START_PCTL           (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-12 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_LINFLEX12_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX12_STOP_PCTL            (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   LINFlex-13 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_LINFLEX13_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX13_START_PCTL           (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-13 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_LINFLEX13_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX13_STOP_PCTL            (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   LINFlex-14 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_LINFLEX14_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX14_START_PCTL           (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-14 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_LINFLEX14_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX14_STOP_PCTL            (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   LINFlex-15 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_LINFLEX15_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX15_START_PCTL           (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-15 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_LINFLEX15_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX15_STOP_PCTL            (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))

#endif

/**
 * @brief   LINFlex-16 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_LINFLEX16_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX16_START_PCTL           (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-16 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_LINFLEX16_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX16_STOP_PCTL            (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))

#endif

/**
 * @brief   LINFlex-17 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_LINFLEX17_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX17_START_PCTL           (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-17 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_LINFLEX17_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX17_STOP_PCTL            (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))

#endif

/**
 * @brief   LINFlex-18 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_LINFLEX18_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX18_START_PCTL           (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-18 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_LINFLEX18_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX18_STOP_PCTL            (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))

#endif

/**
 * @brief   LINFlex-19 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_LINFLEX19_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX19_START_PCTL           (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-19 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_LINFLEX19_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX19_STOP_PCTL            (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))

#endif

/**
 * @brief   LINFlex-20 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_LINFLEX20_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX20_START_PCTL           (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-20 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_LINFLEX20_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX20_STOP_PCTL            (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))

#endif

/**
 * @brief   LINFlex-21 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_LINFLEX21_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX21_START_PCTL           (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-21 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_LINFLEX21_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX21_STOP_PCTL            (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))

#endif

/**
 * @brief   LINFlex-22 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_LINFLEX22_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX22_START_PCTL           (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-22 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_LINFLEX22_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX22_STOP_PCTL            (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))

#endif

/**
 * @brief   LINFlex-23 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode.
 */
#if !defined(SPC5_LINFLEX23_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX23_START_PCTL           (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   LINFlex-23 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode.
 */
#if !defined(SPC5_LINFLEX23_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_LINFLEX23_STOP_PCTL            (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))

#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/**
 * @brief  API mode.
 * @{
 */
#define SPC5_LIN_API_MODE_SYNCHRONOUS       0U /**< @brief Synchronous API.  */
#define SPC5_LIN_API_MODE_ASYNCHRONOUS      1U /**< @brief Asynchronous API. */
#define SPC5_LIN_API_MODE_BUFFERED_IO       2U /**< @brief Buffered IO API (only for Serial). */
/** @} */

/**
 * @brief  Error interrupts (common to TX and RX).
 * @{
 */
#define SPC5_LIN_INTERRUPTS                 (SPC5_LINIER_BOIE |             \
                                             SPC5_LINIER_FEIE |             \
                                             SPC5_LINIER_SZIE)
/** @} */

/**
 * @brief  LinFlex RX/TX status.
 * @{
 */
#define SPC5_LIN_RX_READY                   0U
#define SPC5_LIN_RX_BUSY                    1U
#define SPC5_LIN_TX_READY                   0U
#define SPC5_LIN_TX_BUSY                    1U
/** @} */

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/* PPC register definitions not compatible with MISRA rules on bit-fields 
 (Rules 6.1). */
/*lint -e46 */
struct spc5_linflexl {

  int16_t LINFLEX_reserved1;

  union {
    vuint16_t R;
    struct {
      vuint16_t CCD :1;
      vuint16_t CFD :1;
      vuint16_t LASE :1;
      vuint16_t AWUM :1;
      vuint16_t MBL :4;
      vuint16_t BF :1;
      vuint16_t SFTM :1;
      vuint16_t LBKM :1;
      vuint16_t MME :1;
      vuint16_t SBDT :1;
      vuint16_t RBLM :1;
      vuint16_t SLEEP :1;
      vuint16_t INIT :1;
    } B;
  } LINCR1;

  int16_t LINFLEX_reserved2;

  union {
    vuint16_t R;
    struct {
      vuint16_t SZIE :1;
      vuint16_t OCIE :1;
      vuint16_t BEIE :1;
      vuint16_t CEIE :1;
      vuint16_t HEIE :1;
      vuint16_t :2;
      vuint16_t FEIE :1;
      vuint16_t BOIE :1;
      vuint16_t LSIE :1;
      vuint16_t WUIE :1;
      vuint16_t DBFIE :1;
      vuint16_t DBEIE :1;
      vuint16_t DRIE :1;
      vuint16_t DTIE :1;
      vuint16_t HRIE :1;
    } B;
  } LINIER;

  int16_t LINFLEX_reserved3;

  union {
    vuint16_t R;
    struct {
      vuint16_t LINS :4;
      vuint16_t :2;
      vuint16_t RMB :1;
      vuint16_t :1;
      vuint16_t RBSY :1;
      vuint16_t RPS :1;
      vuint16_t WUF :1;
      vuint16_t DBFF :1;
      vuint16_t DBEF :1;
      vuint16_t DRF :1;
      vuint16_t DTF :1;
      vuint16_t HRF :1;
    } B;
  } LINSR;

  int16_t LINFLEX_reserved4;

  union {
    vuint16_t R;
    struct {
      vuint16_t SZF :1;
      vuint16_t OCF :1;
      vuint16_t BEF :1;
      vuint16_t CEF :1;
      vuint16_t SFEF :1;
      vuint16_t BDEF :1;
      vuint16_t IDPEF :1;
      vuint16_t FEF :1;
      vuint16_t BOF :1;
      vuint16_t :6;
      vuint16_t NF :1;
    } B;
  } LINESR;

  int16_t LINFLEX_reserved5;

  union {
    vuint16_t R;
    struct {
      vuint16_t TDFL :3;
      vuint16_t RDFL :3;
      vuint16_t RFBM :1;
      vuint16_t TFBM :1;
      vuint16_t WL1 :1;
      vuint16_t PC1 :1;
      vuint16_t RXEN :1;
      vuint16_t TXEN :1;
      vuint16_t PC0 :1;
      vuint16_t PCE :1;
      vuint16_t WL0 :1;
      vuint16_t UART :1;
    } B;
  } UARTCR;

  int16_t LINFLEX_reserved6;

  union {
    vuint16_t R;
    struct {
      vuint16_t SZF :1;
      vuint16_t OCF :1;
      vuint16_t PE :4;
      vuint16_t RMB :1;
      vuint16_t FEF :1;
      vuint16_t BOF :1;
      vuint16_t RPS :1;
      vuint16_t WUF :1;
      vuint16_t :2;
      vuint16_t DRF :1;
      vuint16_t DTF :1;
      vuint16_t NF :1;
    } B;
  } UARTSR;

  int16_t LINFLEX_reserved7;

  union {
    vuint16_t R;
    struct {
      vuint16_t :5;
      vuint16_t MODE :1;
      vuint16_t IOT :1;
      vuint16_t TOCE :1;
      vuint16_t CNT :8;
    } B;
  } LINTCSR;

  int16_t LINFLEX_reserved8;

  union {
    vuint16_t R;
    struct {
      vuint16_t OC2 :8;
      vuint16_t OC1 :8;
    } B;
  } LINOCR;

  int16_t LINFLEX_reserved9;

  union {
    vuint16_t R;
    struct {
      vuint16_t :4;
      vuint16_t RTO :4;
      vuint16_t :1;
      vuint16_t HTO :7;
    } B;
  } LINTOCR;

  int16_t LINFLEX_reserved10;

  union {
    vuint16_t R;
    struct {
      vuint16_t :12;
      vuint16_t DIV_F :4;
    } B;
  } LINFBRR;

  int16_t LINFLEX_reserved11;

  union {
    vuint16_t R;
    struct {
      vuint16_t :3;
      vuint16_t DIV_M :13;
    } B;
  } LINIBRR;

  int16_t LINFLEX_reserved12;

  union {
    vuint16_t R;
    struct {
      vuint16_t :8;
      vuint16_t CF :8;
    } B;
  } LINCFR;

  int16_t LINFLEX_reserved13;

  union {
    vuint16_t R;
    struct {
      vuint16_t :1;
      vuint16_t IOBE :1;
      vuint16_t IOPE :1;
      vuint16_t WURQ :1;
      vuint16_t DDRQ :1;
      vuint16_t DTRQ :1;
      vuint16_t ABRQ :1;
      vuint16_t HTRQ :1;
      vuint16_t :8;
    } B;
  } LINCR2;

  int16_t LINFLEX_reserved14;

  union {
    vuint16_t R;
    struct {
      vuint16_t DFL :6;
      vuint16_t DIR :1;
      vuint16_t CCS :1;
      vuint16_t :2;
      vuint16_t ID :6;
    } B;
  } BIDR;

  union {
    vuint32_t R;
    struct {
      vuint32_t DATA3 :8;
      vuint32_t DATA2 :8;
      vuint32_t DATA1 :8;
      vuint32_t DATA0 :8;
    } B;
  } BDRL;

  union {
    vuint32_t R;
    struct {
      vuint32_t DATA7 :8;
      vuint32_t DATA6 :8;
      vuint32_t DATA5 :8;
      vuint32_t DATA4 :8;
    } B;
  } BDRM;

  int16_t LINFLEX_reserved15;

  union {
    vuint16_t R;
    struct {
      vuint16_t :8;
      vuint16_t FACT :8;
    } B;
  } IFER;

  int16_t LINFLEX_reserved16;

  union {
    vuint16_t R;
    struct {
      vuint16_t :12;
      vuint16_t IFMI :4;
    } B;
  } IFMI;

  int16_t LINFLEX_reserved17;

  union {
    vuint16_t R;
    struct {
      vuint16_t :12;
      vuint16_t IFM :4;
    } B;
  } IFMR;
};

struct spc5_linflexh {

  union {
    vuint32_t R;
    struct {
      vuint32_t:26;
      vuint32_t TDFBM:1;
      vuint32_t RDFBM:1;
      vuint32_t TDLIS:1;
      vuint32_t RDLIS:1;
      vuint32_t STOP:1;
      vuint32_t SR:1;
    } B;
  } GCR;

  union {
    vuint32_t R;
    struct {
      vuint32_t:20;
      vuint32_t PTO:12;
    } B;
  } UARTPTO;

  union {
    vuint32_t R;
    struct {
      vuint32_t:20;
      vuint32_t CTO:12;
    } B;
  } UARTCTO;

  union {
    vuint32_t R;
    struct {
      vuint32_t:16;
      vuint32_t DTE:16;
    } B;
  } DMATXE;

  union {
    vuint32_t R;
    struct {
      vuint32_t:16;
      vuint32_t DRE:16;
    } B;
  } DMARXE;
};
/*lint +e46 */

/**
 * @brief   Linflex  callback type.
 *
 * @param[in] device    pointer to the configured linflex device
 *
 */
typedef void (*lincallback_t)(uint32_t device);

/**
 * @brief   Type of a structure representing a Linflex driver.
 */
typedef struct LinflexDriver LinflexDriver;

/**
 * @brief   Structure representing an LinflexDriver driver.
 */
struct LinflexDriver{
  uint32_t device; /* must be private */
  /*linflex rx interrupt call back */
  lincallback_t rxi_lincallback;
  /*linflex rx interrupt call back */
  lincallback_t txi_lincallback;
  /*linflex rx interrupt call back */
  lincallback_t err_lincallback;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Sets TX busy flag.
 * @details This function sets the TX busy flag when a TX transfer starts.
 *
 * @param[in] ddp      pointer to the @p [Serial,Lin]Driver object
 *
 * @notapi
 */
#define SPC5_LIN_TX_IN_PROGRESS(ddp)                                        \
  (ddp)->tx_busy = SPC5_LIN_TX_BUSY

  /**
 * @brief   Sets RX busy flag.
 * @details This function sets the RX busy flag when a RX transfer starts.
 *
 * @param[in] ddp      pointer to the @p [Serial,Lin]Driver object
 *
 * @notapi
 */
#define SPC5_LIN_RX_IN_PROGRESS(ddp)                                        \
  (ddp)->rx_busy = SPC5_LIN_RX_BUSY

/**
 * @brief   Clears TX busy flag.
 * @details This function clears the TX busy flag when a TX transfer is
 *          completed.
 *
 * @param[in] ddp      pointer to the @p [Serial,Lin]Driver object
 *
 * @notapi
 */
#define SPC5_LIN_TX_DONE(ddp)                                               \
  (ddp)->tx_busy = SPC5_LIN_TX_READY

/**
 * @brief   Clears RX busy flag.
 * @details This function clears the RX busy flag when a RX transfer is
 *          completed.
 *
 * @param[in] ddp      pointer to the @p [Serial,Lin]Driver object
 *
 * @notapi
 */
#define SPC5_LIN_RX_DONE(ddp)                                               \
  (ddp)->rx_busy = SPC5_LIN_RX_READY

/**
 * @brief   Waits for TX completion.
 * @details This function waits till the current TX transfer is completed.
 *
 * @param[in] ddp      pointer to the @p [Serial,Lin]Driver object
 *
 * @notapi
 */
#define SPC5_LIN_WAIT_FOR_TX_COMPLETION(ddp)                                \
  if ((ddp)->config->api_mode == SPC5_LIN_API_MODE_SYNCHRONOUS ||           \
      (ddp)->config->api_mode == SPC5_LIN_API_MODE_BUFFERED_IO) {           \
    while ((ddp)->tx_busy == SPC5_LIN_TX_BUSY) { ; }                        \
  }

/**
 * @brief   Waits for RX completion.
 * @details This function waits till the current RX transfer is completed.
 *
 * @param[in] ddp      pointer to the @p [Serial,Lin]Driver object
 *
 * @notapi
 */
#define SPC5_LIN_WAIT_FOR_RX_COMPLETION(ddp)                                \
  if ((ddp)->config->api_mode == SPC5_LIN_API_MODE_SYNCHRONOUS) {           \
    while ((ddp)->rx_busy == SPC5_LIN_RX_BUSY) { ; }                        \
  }

/*===========================================================================*/
/* IRQ Handlers                                                              */
/*===========================================================================*/

#if (SPC5_LINFLEX0_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX0_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX0_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX0_ERR_HANDLER);
#endif
#if (SPC5_LINFLEX1_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX1_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX1_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX1_ERR_HANDLER);
#endif
#if (SPC5_LINFLEX2_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX2_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX2_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX2_ERR_HANDLER);
#endif
#if (SPC5_LINFLEX3_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX3_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX3_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX3_ERR_HANDLER);
#endif
#if (SPC5_LINFLEX4_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX4_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX4_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX4_ERR_HANDLER);
#endif
#if (SPC5_LINFLEX5_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX5_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX5_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX5_ERR_HANDLER);
#endif
#if (SPC5_LINFLEX6_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX6_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX6_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX6_ERR_HANDLER);
#endif
#if (SPC5_LINFLEX7_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX7_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX7_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX7_ERR_HANDLER);
#endif
#if (SPC5_LINFLEX8_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX8_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX8_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX8_ERR_HANDLER);
#endif
#if (SPC5_LINFLEX9_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX9_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX9_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX9_ERR_HANDLER);
#endif
#if (SPC5_LINFLEX10_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX10_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX10_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX10_ERR_HANDLER);
#endif
#if (SPC5_LINFLEX11_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX11_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX11_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX11_ERR_HANDLER);
#endif
#if (SPC5_LINFLEX12_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX12_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX12_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX12_ERR_HANDLER);
#endif
#if (SPC5_LINFLEX13_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX13_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX13_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX13_ERR_HANDLER);
#endif
#if (SPC5_LINFLEX14_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX14_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX14_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX14_ERR_HANDLER);
#endif
#if (SPC5_LINFLEX15_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX15_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX15_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX15_ERR_HANDLER);
#endif
#if (SPC5_LINFLEX16_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX16_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX16_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX16_ERR_HANDLER);
#endif
#if (SPC5_LINFLEX17_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX17_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX17_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX17_ERR_HANDLER);
#endif
#if (SPC5_LINFLEX18_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX18_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX18_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX18_ERR_HANDLER);
#endif
#if (SPC5_LINFLEX19_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX19_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX19_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX19_ERR_HANDLER);
#endif
#if (SPC5_LINFLEX20_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX20_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX20_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX20_ERR_HANDLER);
#endif
#if (SPC5_LINFLEX21_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX21_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX21_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX21_ERR_HANDLER);
#endif
#if (SPC5_LINFLEX22_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX22_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX22_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX22_ERR_HANDLER);
#endif
#if (SPC5_LINFLEX23_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
IRQ_HANDLER (SPC5_LINFLEX23_RXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX23_TXI_HANDLER);
IRQ_HANDLER (SPC5_LINFLEX23_ERR_HANDLER);
#endif

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/**
 * @name    LINFlex units references
 * @{
 */
#if (SPC5_HAS_LINFLEX0 == TRUE) || defined(__DOXYGEN__)
#define SPC5_LINFLEX0L  (*(struct spc5_linflexl *)SPC5_LINFLEX0L_BASE)
#define SPC5_LINFLEX0H  (*(struct spc5_linflexh *)SPC5_LINFLEX0H_BASE)
#endif

#if (SPC5_HAS_LINFLEX1 == TRUE) || defined(__DOXYGEN__)
#define SPC5_LINFLEX1L  (*(struct spc5_linflexl *)SPC5_LINFLEX1L_BASE)
#define SPC5_LINFLEX1H  (*(struct spc5_linflexh *)SPC5_LINFLEX1H_BASE)
#endif

#if (SPC5_HAS_LINFLEX2 == TRUE) || defined(__DOXYGEN__)
#define SPC5_LINFLEX2L  (*(struct spc5_linflexl *)SPC5_LINFLEX2L_BASE)
#define SPC5_LINFLEX2H  (*(struct spc5_linflexh *)SPC5_LINFLEX2H_BASE)
#endif

#if (SPC5_HAS_LINFLEX3 == TRUE) || defined(__DOXYGEN__)
#define SPC5_LINFLEX3L  (*(struct spc5_linflexl *)SPC5_LINFLEX3L_BASE)
#define SPC5_LINFLEX3H  (*(struct spc5_linflexh *)SPC5_LINFLEX3H_BASE)
#endif

#if (SPC5_HAS_LINFLEX4 == TRUE) || defined(__DOXYGEN__)
#define SPC5_LINFLEX4L  (*(struct spc5_linflexl *)SPC5_LINFLEX4L_BASE)
#define SPC5_LINFLEX4H  (*(struct spc5_linflexh *)SPC5_LINFLEX4H_BASE)
#endif

#if (SPC5_HAS_LINFLEX5 == TRUE) || defined(__DOXYGEN__)
#define SPC5_LINFLEX5L  (*(struct spc5_linflexl *)SPC5_LINFLEX5L_BASE)
#define SPC5_LINFLEX5H  (*(struct spc5_linflexh *)SPC5_LINFLEX5H_BASE)
#endif

#if (SPC5_HAS_LINFLEX6 == TRUE) || defined(__DOXYGEN__)
#define SPC5_LINFLEX6L  (*(struct spc5_linflexl *)SPC5_LINFLEX6L_BASE)
#define SPC5_LINFLEX6H  (*(struct spc5_linflexh *)SPC5_LINFLEX6H_BASE)
#endif

#if (SPC5_HAS_LINFLEX7 == TRUE) || defined(__DOXYGEN__)
#define SPC5_LINFLEX7L  (*(struct spc5_linflexl *)SPC5_LINFLEX7L_BASE)
#define SPC5_LINFLEX7H  (*(struct spc5_linflexh *)SPC5_LINFLEX7H_BASE)
#endif

#if (SPC5_HAS_LINFLEX8 == TRUE) || defined(__DOXYGEN__)
#define SPC5_LINFLEX8L  (*(struct spc5_linflexl *)SPC5_LINFLEX8L_BASE)
#define SPC5_LINFLEX8H  (*(struct spc5_linflexh *)SPC5_LINFLEX8H_BASE)
#endif

#if (SPC5_HAS_LINFLEX9 == TRUE) || defined(__DOXYGEN__)
#define SPC5_LINFLEX9L  (*(struct spc5_linflexl *)SPC5_LINFLEX9L_BASE)
#define SPC5_LINFLEX9H  (*(struct spc5_linflexh *)SPC5_LINFLEX9H_BASE)
#endif

#if (SPC5_HAS_LINFLEX10 == TRUE) || defined(__DOXYGEN__)
#define SPC5_LINFLEX10L (*(struct spc5_linflexl *)SPC5_LINFLEX10L_BASE)
#define SPC5_LINFLEX10H (*(struct spc5_linflexh *)SPC5_LINFLEX10H_BASE)
#endif

#if (SPC5_HAS_LINFLEX11 == TRUE) || defined(__DOXYGEN__)
#define SPC5_LINFLEX11L (*(struct spc5_linflexl *)SPC5_LINFLEX11L_BASE)
#define SPC5_LINFLEX11H (*(struct spc5_linflexh *)SPC5_LINFLEX11H_BASE)
#endif

#if (SPC5_HAS_LINFLEX12 == TRUE) || defined(__DOXYGEN__)
#define SPC5_LINFLEX12L (*(struct spc5_linflexl *)SPC5_LINFLEX12L_BASE)
#define SPC5_LINFLEX12H (*(struct spc5_linflexh *)SPC5_LINFLEX12H_BASE)
#endif

#if (SPC5_HAS_LINFLEX13 == TRUE) || defined(__DOXYGEN__)
#define SPC5_LINFLEX13L (*(struct spc5_linflexl *)SPC5_LINFLEX13L_BASE)
#define SPC5_LINFLEX13H (*(struct spc5_linflexh *)SPC5_LINFLEX13H_BASE)
#endif

#if (SPC5_HAS_LINFLEX14 == TRUE) || defined(__DOXYGEN__)
#define SPC5_LINFLEX14L (*(struct spc5_linflexl *)SPC5_LINFLEX14L_BASE)
#define SPC5_LINFLEX14H (*(struct spc5_linflexh *)SPC5_LINFLEX14H_BASE)
#endif

#if (SPC5_HAS_LINFLEX15 == TRUE) || defined(__DOXYGEN__)
#define SPC5_LINFLEX15L (*(struct spc5_linflexl *)SPC5_LINFLEX15L_BASE)
#define SPC5_LINFLEX15H (*(struct spc5_linflexh *)SPC5_LINFLEX15H_BASE)
#endif

#if (SPC5_HAS_LINFLEX16 == TRUE) || defined(__DOXYGEN__)
#define SPC5_LINFLEX16L (*(struct spc5_linflexl *)SPC5_LINFLEX16L_BASE)
#define SPC5_LINFLEX16H (*(struct spc5_linflexh *)SPC5_LINFLEX16H_BASE)
#endif

#if (SPC5_HAS_LINFLEX17 == TRUE) || defined(__DOXYGEN__)
#define SPC5_LINFLEX17L (*(struct spc5_linflexl *)SPC5_LINFLEX17L_BASE)
#define SPC5_LINFLEX17H (*(struct spc5_linflexh *)SPC5_LINFLEX17H_BASE)
#endif

#if (SPC5_HAS_LINFLEX18 == TRUE) || defined(__DOXYGEN__)
#define SPC5_LINFLEX18L (*(struct spc5_linflexl *)SPC5_LINFLEX18L_BASE)
#define SPC5_LINFLEX18H (*(struct spc5_linflexh *)SPC5_LINFLEX18H_BASE)
#endif

#if (SPC5_HAS_LINFLEX19 == TRUE) || defined(__DOXYGEN__)
#define SPC5_LINFLEX19L (*(struct spc5_linflexl *)SPC5_LINFLEX19L_BASE)
#define SPC5_LINFLEX19H (*(struct spc5_linflexh *)SPC5_LINFLEX19H_BASE)
#endif

#if (SPC5_HAS_LINFLEX20 == TRUE) || defined(__DOXYGEN__)
#define SPC5_LINFLEX20L (*(struct spc5_linflexl *)SPC5_LINFLEX20L_BASE)
#define SPC5_LINFLEX20H (*(struct spc5_linflexh *)SPC5_LINFLEX20H_BASE)
#endif

#if (SPC5_HAS_LINFLEX21 == TRUE) || defined(__DOXYGEN__)
#define SPC5_LINFLEX21L (*(struct spc5_linflexl *)SPC5_LINFLEX21L_BASE)
#define SPC5_LINFLEX21H (*(struct spc5_linflexh *)SPC5_LINFLEX21H_BASE)
#endif

#if (SPC5_HAS_LINFLEX22 == TRUE) || defined(__DOXYGEN__)
#define SPC5_LINFLEX22L (*(struct spc5_linflexl *)SPC5_LINFLEX22L_BASE)
#define SPC5_LINFLEX22H (*(struct spc5_linflexh *)SPC5_LINFLEX22H_BASE)
#endif

#if (SPC5_HAS_LINFLEX23 == TRUE) || defined(__DOXYGEN__)
#define SPC5_LINFLEX23L (*(struct spc5_linflexl *)SPC5_LINFLEX23L_BASE)
#define SPC5_LINFLEX23H (*(struct spc5_linflexh *)SPC5_LINFLEX23H_BASE)
#endif
/** @} */

#if (SPC5_LINFLEX0_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
extern LinflexDriver LinflexD1;
#endif
#if (SPC5_LINFLEX1_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
extern LinflexDriver LinflexD2;
#endif
#if (SPC5_LINFLEX2_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
extern LinflexDriver LinflexD3;
#endif
#if (SPC5_LINFLEX3_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
extern LinflexDriver LinflexD4;
#endif
#if (SPC5_LINFLEX4_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
extern LinflexDriver LinflexD5;
#endif
#if (SPC5_LINFLEX5_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
extern LinflexDriver LinflexD6;
#endif
#if (SPC5_LINFLEX6_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
extern LinflexDriver LinflexD7;
#endif
#if (SPC5_LINFLEX7_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
extern LinflexDriver LinflexD8;
#endif
#if (SPC5_LINFLEX8_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
extern LinflexDriver LinflexD9;
#endif
#if (SPC5_LINFLEX9_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
extern LinflexDriver LinflexD10;
#endif
#if (SPC5_LINFLEX10_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
extern LinflexDriver LinflexD11;
#endif
#if (SPC5_LINFLEX11_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
extern LinflexDriver LinflexD12;
#endif
#if (SPC5_LINFLEX12_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
extern LinflexDriver LinflexD13;
#endif
#if (SPC5_LINFLEX13_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
extern LinflexDriver LinflexD14;
#endif
#if (SPC5_LINFLEX14_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
extern LinflexDriver LinflexD15;
#endif
#if (SPC5_LINFLEX15_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
extern LinflexDriver LinflexD16;
#endif
#if (SPC5_LINFLEX16_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
extern LinflexDriver LinflexD17;
#endif
#if (SPC5_LINFLEX17_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
extern LinflexDriver LinflexD18;
#endif
#if (SPC5_LINFLEX18_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
extern LinflexDriver LinflexD19;
#endif
#if (SPC5_LINFLEX19_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
extern LinflexDriver LinflexD20;
#endif
#if (SPC5_LINFLEX20_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
extern LinflexDriver LinflexD21;
#endif
#if (SPC5_LINFLEX21_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
extern LinflexDriver LinflexD22;
#endif
#if (SPC5_LINFLEX22_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
extern LinflexDriver LinflexD23;
#endif
#if (SPC5_LINFLEX23_SETTING != SPC5_LINFLEX_MODE_NONE) || defined(__DOXYGEN__)
extern LinflexDriver LinflexD24;
#endif


#ifdef __cplusplus
extern "C" {
#endif
  void linflex_lld_init(void);
#ifdef __cplusplus
}
#endif

#endif /* LLD_USE_SERIAL || LLD_USE_LIN */

#endif /* _SPC5_LINFLEX_H_ */

/** @} */
