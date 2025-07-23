// SPDX-License-Identifier: MIT
/* NOTE: Struct members/comments pulled from rp2040 datasheet. */
#ifndef _RESETS_H
#define _RESETS_H

#include "atomic_access.h"
#include "types.h"

struct resets {
  u32 reset;
  u32 wdsel;
  u32 reset_done;
};

#define RESETS_BASE 0x4000c000
/* TODO: Should these just be an enum instead? */
/* Reset bits for the RESETS_RESET and RESETS_RESET_CLR registers. */
#define RESET_ADC (1u << 0)
#define RESET_BUSCTRL (1u << 1)
#define RESET_DMA (1u << 2)
#define RESET_I2C0 (1u << 3)
#define RESET_I2C1 (1u << 4)
#define RESET_IO_BANK0 (1u << 5)
#define RESET_IO_QSPI (1u << 6)
#define RESET_JTAG (1u << 7)
#define RESET_PADS_BANK0 (1u << 8)
#define RESET_PADS_QSPI (1u << 9)
#define RESET_PIO0 (1u << 10)
#define RESET_PIO1 (1u << 11)
#define RESET_PLL_SYS (1u << 12)
#define RESET_PLL_USB (1u << 13)
#define RESET_PWM (1u << 14)
#define RESET_RTC (1u << 15)
#define RESET_SPI0 (1u << 16)
#define RESET_SPI1 (1u << 17)
#define RESET_SYSCFG (1u << 18)
#define RESET_SYSINFO (1u << 19)
#define RESET_TBMAN (1u << 20)
#define RESET_TIMER (1u << 21)
#define RESET_UART0 (1u << 22)
#define RESET_UART1 (1u << 23)
#define RESET_USBCTRL (1u << 24)

#define RESETS ((volatile struct resets *)RESETS_BASE)
/**
 * resets_clear_blocking - deassert a reset and spin until it is done.
 * @peripherals: The peripherals you need to bring out of reset, ORed
 * together (See above).
 */
static inline void resets_clear_blocking(u32 peripherals) {
  /* Clear reset. */
  clear_bits(&RESETS->reset, peripherals);
  while ((RESETS->reset_done & peripherals) != peripherals)
    ; /* Wait for peripherals to come out of reset. */
}

#endif /* _RESETS_H */
