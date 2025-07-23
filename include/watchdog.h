// SPDX-License-Identifier: MIT
/* NOTE: Struct members/comments pulled from rp2040 datasheet. */
#ifndef _WATCHDOG_H
#define _WATCHDOG_H
#include "types.h"
#include "xosc.h"
struct watchdog {
  u32 ctrl;       /* Watchdog control */
  u32 load;       /* Load the watchdog timer. */
  u32 reason;     /* Logs the reason for the last reset.*/
  u32 scratch[8]; /* Scratch registers 0-7 */
  u32 tick;       /* Controls the tick generator. */
};
#define WATCHDOG_BASE 0x40058000
#define WATCHDOG ((volatile struct watchdog *)(WATCHDOG_BASE))
#define WATCHDOG_TICK_ENABLE_BITS 0x1 << 9
/* Enable watchdog tick and set cycles to 12. */
static inline void watchdog_tick_start() {
  WATCHDOG->tick = XOSC_MHZ | WATCHDOG_TICK_ENABLE_BITS;
}
#endif /*_WATCHDOG_H*/
