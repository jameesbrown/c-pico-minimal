// SPDX-License-Identifier: MIT
/* NOTE: Struct members/comments pulled from rp2040 datasheet. */
#ifndef _SIO_H
#define _SIO_H

#include "types.h"
/* FIXME: Remove and create shared gpio header. */
#include "io_bank0.h"

struct sio {
  u32 cpuid;      /* Processor core identifier */
  u32 gpio_in;    /* Input value for GPIO pins */
  u64 gpio_hi_in; /* Input value for QSPI pins */

  u32 gpio_out;     /* GPIO output value */
  u32 gpio_out_set; /* GPIO output value set */
  u32 gpio_out_clr; /* GPIO output value clear */
  u32 gpio_out_xor; /* GPIO output value XOR */

  u32 gpio_oe;     /* GPIO output enable */
  u32 gpio_oe_set; /* GPIO output enable set */
  u32 gpio_oe_clr; /* GPIO output enable clear */
  u32 gpio_oe_xor; /* GPIO output enable XOR */

  u32 gpio_hi_out;     /* QSPI output value */
  u32 gpio_hi_out_set; /* QSPI output value set */
  u32 gpio_hi_out_clr; /* QSPI output value clear */
  u32 gpio_hi_out_xor; /* QSPI output value XOR */

  u32 gpio_hi_oe;     /* QSPI output enable */
  u32 gpio_hi_oe_set; /* QSPI output enable set */
  u32 gpio_hi_oe_clr; /* QSPI output enable clear */
  u32 gpio_hi_oe_xor; /* QSPI output enable XOR */

  u32 fifo_st; /* FIFO status register */
  u32 fifo_wr; /* TX FIFO write */
  u32 fifo_rd; /* RX FIFO read */

  u32 spinlock_st; /* Spinlock status */

  u32 div_udividend; /* Divider unsigned dividend */
  u32 div_udivisor;  /* Divider unsigned divisor */
  u32 div_sdividend; /* Divider signed dividend */
  u32 div_sdivisor;  /* Divider signed divisor */
  u32 div_quotient;  /* Divider quotient result */
  u32 div_remainder; /* Divider remainder result */
  u64 div_csr;       /* Divider control/status register */

  /* Interpolator 0 */
  u32 interp0_accum0;
  u32 interp0_accum1;
  u32 interp0_base0;
  u32 interp0_base1;
  u32 interp0_base2;
  u32 interp0_pop_lane0; /* Pop: write both accumulators. */
  u32 interp0_pop_lane1;
  u32 interp0_pop_full;
  u32 interp0_peek_lane0; /* Peek: read without side effects. */
  u32 interp0_peek_lane1;
  u32 interp0_peek_full;
  u32 interp0_ctrl_lane0;
  u32 interp0_ctrl_lane1;
  u32 interp0_accum0_add; /* Atomic add to accum0 */
  u32 interp0_accum1_add;
  u32 interp0_base_1and0; /* Lower bits to base0, upper to base1 */

  /* Interpolator 1 */
  u32 interp1_accum0;
  u32 interp1_accum1;
  u32 interp1_base0;
  u32 interp1_base1;
  u32 interp1_base2;
  u32 interp1_pop_lane0;
  u32 interp1_pop_lane1;
  u32 interp1_pop_full;
  u32 interp1_peek_lane0;
  u32 interp1_peek_lane1;
  u32 interp1_peek_full;
  u32 interp1_ctrl_lane0;
  u32 interp1_ctrl_lane1;
  u32 interp1_accum0_add;
  u32 interp1_accum1_add;
  u32 interp1_base_1and0;

  /* Spinlocks 0–31 */
  u32 spinlock[32];
};

#define SIO_BASE 0xd0000000
#define SIO ((volatile struct sio *)SIO_BASE)

// FIXME: Make this typed? (In a way that is no less efficient.)
static inline void sio_toggle_gpio(u32 gpio_num) {
  SIO->gpio_out_xor = 1 << gpio_num;
}

// FIXME: Put this and related functions (like above) in a gpio header.
static inline void sio_enable_gpio(u32 gpio_num) {
  IO_BANK0->gpio[gpio_num].ctrl = GPIO_FUNC_SIO;
  SIO->gpio_oe_set = 1 << gpio_num;
}

#endif /* _SIO_H */
