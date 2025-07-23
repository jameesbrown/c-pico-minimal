// SPDX-License-Identifier: MIT
/* NOTE: Struct members/comments pulled from rp2040 datasheet. */
#ifndef _XOSC_H
#define _XOSC_H
#include "types.h"

struct xosc {
  u32 ctrl;    /*Crystal Oscillator Control*/
  u32 status;  /*Crystal Oscillator Status*/
  u32 dormant; /*Crystal Oscillator pause control*/
  u32 startup; /*Controls the startup delay*/
  u32 _pad[3];
  u32 count; /*A down counter running at the XOSC frequency which counts to
                   zero and stops.*/
};

#define XOSC_BASE 0x40024000
#define XOSC ((volatile struct xosc *)(XOSC_BASE))
#define XOSC_ENABLE_BITS 0xFAB << 12
/* NOTE: this is the only valid value.
 * Implies 1-15MHZ range. Pico is 12MHz.
 */
#define XOSC_MHZ 12
#define XOSC_FREQ_RANGE_BITS 0xAA0
/* FIXME: Find optimal startup delay (see section 2.16.3 pp 217). */
#define XOSC_STARTUP_DELAY 0x2F
/* Stable and enabled. */
#define XOSC_STATUS_READY (u32)((0x1 << 31) | (0x1 << 12))

static inline void xosc_init_enable() {
  /* > before starting the XOSC the programmer must ensure the
   * > STARTUP_DELAY register is correctly configured.
   * Source: rp2040 datasheet.
   */
  XOSC->startup = XOSC_STARTUP_DELAY;
  XOSC->ctrl = XOSC_ENABLE_BITS | XOSC_FREQ_RANGE_BITS;
  /* Spin until it is ready. */
  while ((XOSC->status & XOSC_STATUS_READY) != XOSC_STATUS_READY)
    ;
}

static inline void xosc_count_down(u8 val) {
  XOSC->count = val;
  while (XOSC->count)
    ;
}

#endif /*_XOSC_H*/
