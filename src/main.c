// SPDX-License-Identifier: MIT
#include "atomic_access.h"
#include "clocks.h"
#include "core_ppb.h"
#include "resets.h"
#include "sio.h"
#include "timer.h"
#include "types.h"
#include "watchdog.h"
#include "xip.h"
#include "xosc.h"
/* WARN: Change this to your led pin (usually 25). */
#define LED_PIN 25
/* One second. */
#define BLINK_DELAY_US 1000000

/* TODO: Should these go in blink.ld? */
#define LOAD_BASE 0x10000100
#define RUNTIME_BASE 0x20000000

/* NOTE: From blink.ld. */
extern void *__copy_len;

void timer0_irq(void) {
  clear_bits(&TIMER->intr, 0x1);
  sio_toggle_gpio(LED_PIN);
  return;
}
/**
 * Vector Table
 *
 * NOTE: Reset is the only time Vectors[0] (initial stack pointer) and
 * Vectors[1] (reset) would be relevant. However, reset is handled by the boot
 * rom.
 *
 * TODO: Confirm that the above statement (regarding entries [0] and [1]) is
 * correct.
 */
__attribute__((section(".vectors"))) void (*const Vectors[])(void) = {
    [2] = 0,           /* NMI */
    [3] = 0,           /* HardFault */
    [11] = 0,          /* SVCall */
    [14] = 0,          /* PendSV */
    [15] = 0,          /* SysTick */
    [16] = timer0_irq, /* IRQ 0: TIMER_IRQ_0 */
};

__attribute__((section(".boot"), noreturn)) void start(void) {
  /* Set the stack pointer. */
  asm volatile("ldr r0, =__sram_striped_end");
  asm volatile("mov sp, r0");
  /* FIXME: Disable ROSC? */
  xosc_init_enable();
  /* Set the reference clock to XOSC. */
  clocks_set_ref_to_xosc();
  /* Start the watchdog tick. */
  watchdog_tick_start();
  /* Copy from flash to ram. */
  xip_enable_for_reading();
  volatile u32 *src = (u32 *)LOAD_BASE;
  volatile u32 *dest = (u32 *)RUNTIME_BASE;
  u32 len = ((u32)(&__copy_len));
  while (len--) {
    *dest++ = *src++;
  }
  /* Update the vtable with ours. By setting the vtor. */
  core_ppb_set_vtable((u32)&Vectors);
  /* Done with XIP (we are in sram) so disable. */
  xip_disable();
  /* Set thumb bit. */
  asm volatile(".thumb");
  /* Load _main into r0. */
  asm volatile("ldr r0, =_main");
  /* Jump to _main. */
  asm volatile("bx r0");
  __builtin_unreachable();
}

/* NOTE: We use `_main` instead of `main` to avoid special treatment by the
 * compiler and linker. GCC may place `main` in `.text.startup` when using
 * `-Os`, which can interfere with our custom memory layout and size
 * constraints. The real entry point is `start()`, which sets up memory and
 * explicitly jumps here.
 */
__attribute__((noreturn)) void _main(void) {
  /* Take IO_BANK0 and TIMER out of reset. */
  resets_clear_blocking(RESET_IO_BANK0 | RESET_TIMER);
  /* Enable timer peripheral interrupt for alarm 0. */
  timer_enable_alarm(ALARM0);
  /* Setup LED_PIN for use. */
  sio_enable_gpio(LED_PIN);
  for (;;) {
    timer_alarm_in_us(ALARM0, BLINK_DELAY_US);
    /* TODO: Is this better than "wfi"? */
    asm volatile("wfe"); /* Wait for event. */
  }
}
