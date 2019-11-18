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

#include "board.h"
#include "clock.h"

/*
 * Initial setup of all defined pads, the list is terminated
 * by a {-1, 0, 0}.
 */
static const spc_mscr_io_init_t spc_mscr_io_init[] = {
  {(int16_t)MSCR_IO_INDEX(PORT_A, SBC_SCK), PAL_LOW,    (iomode_t)(PAL_SPC5_SSS(0) | PAL_MODE_OUTPUT_PUSHPULL)},
  {(int16_t)MSCR_IO_INDEX(PORT_A, POWER_LATCH), PAL_HIGH,   (iomode_t)(PAL_SPC5_SSS(0) | PAL_MODE_OUTPUT_PUSHPULL)},
  {(int16_t)MSCR_IO_INDEX(PORT_B, AN9), PAL_LOW,    (iomode_t)(PAL_SPC5_SSS(0) | PAL_MODE_INPUT_ANALOG)},
  {(int16_t)MSCR_IO_INDEX(PORT_B, AN7), PAL_LOW,    (iomode_t)(PAL_SPC5_SSS(0) | PAL_MODE_INPUT_ANALOG)},
  {(int16_t)MSCR_IO_INDEX(PORT_B, AN6), PAL_LOW,    (iomode_t)(PAL_SPC5_SSS(0) | PAL_MODE_INPUT_ANALOG)},
  {(int16_t)MSCR_IO_INDEX(PORT_B, AN5), PAL_LOW,    (iomode_t)(PAL_SPC5_SSS(0) | PAL_MODE_INPUT_ANALOG)},
  {(int16_t)MSCR_IO_INDEX(PORT_B, AN3), PAL_LOW,    (iomode_t)(PAL_SPC5_SSS(0) | PAL_MODE_INPUT_ANALOG)},
  {(int16_t)MSCR_IO_INDEX(PORT_B, AN2), PAL_LOW,    (iomode_t)(PAL_SPC5_SSS(0) | PAL_MODE_INPUT_ANALOG)},
  {(int16_t)MSCR_IO_INDEX(PORT_C, CAN1TX), PAL_LOW,    (iomode_t)(PAL_SPC5_SSS(3) | PAL_MODE_OUTPUT_PUSHPULL)},
  {(int16_t)MSCR_IO_INDEX(PORT_D, PWM_A0), PAL_LOW,    (iomode_t)(PAL_SPC5_SSS(1) | PAL_MODE_OUTPUT_PUSHPULL)},
  {(int16_t)MSCR_IO_INDEX(PORT_D, PWM_A1), PAL_LOW,    (iomode_t)(PAL_SPC5_SSS(1) | PAL_MODE_OUTPUT_PUSHPULL)},
  {(int16_t)MSCR_IO_INDEX(PORT_D, CANTX), PAL_LOW,    (iomode_t)(PAL_SPC5_SSS(1) | PAL_MODE_OUTPUT_PUSHPULL)},
  {(int16_t)MSCR_IO_INDEX(PORT_D, PWM_B0), PAL_LOW,    (iomode_t)(PAL_SPC5_SSS(1) | PAL_MODE_OUTPUT_PUSHPULL)},
  {(int16_t)MSCR_IO_INDEX(PORT_D, PWM_B1), PAL_LOW,    (iomode_t)(PAL_SPC5_SSS(3) | PAL_MODE_OUTPUT_PUSHPULL)},
  {(int16_t)MSCR_IO_INDEX(PORT_A, STANDBY_CAN1), PAL_LOW,    (iomode_t)(PAL_SPC5_SSS(0) | PAL_MODE_OUTPUT_PUSHPULL)},
  {(int16_t)MSCR_IO_INDEX(PORT_E, TEST_PIN), PAL_LOW,    (iomode_t)(PAL_SPC5_SSS(0) | PAL_MODE_OUTPUT_PUSHPULL)},
  {(int16_t)MSCR_IO_INDEX(PORT_A, STANDBY_CAN2), PAL_LOW,    (iomode_t)(PAL_SPC5_SSS(0) | PAL_MODE_OUTPUT_PUSHPULL)},
  {(int16_t)MSCR_IO_INDEX(PORT_A, INH_PIN), PAL_LOW,    (iomode_t)(PAL_SPC5_SSS(0) | PAL_MODE_OUTPUT_PUSHPULL)},
  {-1, 0, 0}
};

/*
 * Initial setup of MSCR_MUX registers, the list is terminated by
 * a {0, -1, 0}.
 */
static const spc_mscr_mux_init_t spc_mscr_mux_init[] = {
  {(int16_t)MSCR_IO_INDEX(PORT_C, 2U), (int16_t)MSCR_MUX_CAN1_RX_CFG, 3},
  {(int16_t)MSCR_IO_INDEX(PORT_D, 9U), (int16_t)MSCR_MUX_CAN0_RX_CFG, 1},
  {0, -1, 0}
};

/**
 * @brief   PAL setup.
 */
static const PALConfig pal_default_config = {
  spc_mscr_io_init,
  spc_mscr_mux_init
};

/*
 * Board-specific initialization code.
 */
void boardInit(void) {

  pal_init(&pal_default_config);
}
