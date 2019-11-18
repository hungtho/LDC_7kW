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

#ifndef _BOARD_H_
#define _BOARD_H_

#include "pal.h"

/*
 * Setup for a generic SPC570Sxx board.
 */

/*
 * Board identifiers.
 */
#define BOARD_GENERIC_SPC570SXX
#define BOARD_NAME                  "Generic SPC570Sxx"

/*
 * I/O definitions.
 */
#define SBC_SCK                     0U
#define POWER_LATCH                 11U
#define AN9                         3U
#define AN7                         5U
#define AN6                         6U
#define AN5                         7U
#define AN3                         11U
#define AN2                         14U
#define CAN1TX                      3U
#define PWM_A0                      7U
#define PWM_A1                      8U
#define CANTX                       10U
#define PWM_B0                      11U
#define PWM_B1                      14U
#define STANDBY_CAN1                7U
#define TEST_PIN                    15U
#define STANDBY_CAN2                8U
#define INH_PIN                     9U

/*
 * MSCR_MUX definitions.
 */
#define MSCR_MUX_CAN1_RX_CFG        1U
#define MSCR_MUX_CAN0_RX_CFG        0U

/*
 * Support macros.
 */
#define MSCR_IO_INDEX(port, pin)  (((port) * 16U) + (pin))

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
