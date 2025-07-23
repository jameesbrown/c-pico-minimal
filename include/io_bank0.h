// SPDX-License-Identifier: MIT
/* NOTE: Struct members/comments pulled from rp2040 datasheet. */
#ifndef _IO_BANK0_H
#define _IO_BANK0_H

#include "types.h"

struct gpio_ctrl {
  u32 status; /* GPIOx_STATUS */
  u32 ctrl;   /* GPIOx_CTRL */
};

struct io_bank0 {
  struct gpio_ctrl gpio[30]; /* GPIO0 to GPIO29. */
  /*
   * Interrupt and PROC/DORMANT registers.
   */
  u32 intr[4];              /* Raw interrupts */
  u32 proc0_inte[4];        /* Interrupt enable for proc0 */
  u32 proc0_intf[4];        /* Interrupt force for proc0 */
  u32 proc0_ints[4];        /* Interrupt status for proc0 */
  u32 proc1_inte[4];        /* Interrupt enable for proc1 */
  u32 proc1_intf[4];        /* Interrupt force for proc1 */
  u32 proc1_ints[4];        /* Interrupt status for proc1 */
  u32 dormant_wake_inte[4]; /* Interrupt enable for dormant wake */
  u32 dormant_wake_intf[4]; /* Interrupt force for dormant wake */
  u32 dormant_wake_ints[4]; /* Interrupt status for dormant wake */
};

#define GPIO_FUNC_SIO 5
#define IO_BANK0_BASE 0x40014000
#define IO_BANK0 ((volatile struct io_bank0 *)IO_BANK0_BASE)

#endif /* _IO_BANK0_H */
