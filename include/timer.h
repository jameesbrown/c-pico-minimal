// SPDX-License-Identifier: MIT
/* NOTE: Struct members/comments pulled from rp2040 datasheet. */
#ifndef _TIMER_H
#define _TIMER_H

#include "atomic_access.h"
#include "core_ppb.h"
#include "types.h"

struct timer {
  u32 timehw; /* Write to bits 63:32 of time. */
  u32 timelw; /* Write to bits 31:0 of time writes do not get copied to time
                 until timehw is written. */
  u32 timehr; /* Read from bits 63:32 of time always read timelr before timehr.
               */
  u32 timelr; /* Read from bits 31:0 of time. */
  u32 alarms[4]; /* Arm and configure fire time of alarms 0-3. */
  u32 armed;     /* Indicates the armed/disarmed status of each alarm.*/
  u32 timerawh;  /* Raw read from bits 63:32 of time (no side effects). */
  u32 timerawl;  /* Raw read from bits 31:0 of time (no side effects). */
  u32 dbgpause;  /* Set bits high to enable pause when the corresponding debug
                    ports are active. */
  u32 pause;     /* Set high to pause the timer. */
  u32 intr;      /* Raw Interrupts */
  u32 inte;      /* Interrupt Enable */
  u32 intf;      /* Interrupt Force */
  u32 ints;      /* Interrupt status after masking & forcing. */
};

enum timer_alarm {
  ALARM0 = 0,
  ALARM1,
  ALARM2,
  ALARM3,
};

/* Sanity checks. */
#define TIMER_BASE 0x40054000
#define TIMER ((volatile struct timer *)(TIMER_BASE))
_Static_assert(sizeof(struct timer) == (0x44 - 0x00), "");
_Static_assert(((u32)&TIMER->intr - 0x40054000) == 0x34, "");

static inline void spin_for_us(u32 us) {
  u32 start_time = TIMER->timerawl;
  while (TIMER->timerawl - start_time < us)
    ;
}

static inline void timer_enable_alarm(enum timer_alarm alarm) {
  /* NOTE: 1 << alarm works b/c IRQ0-3 are for alarms 0-3.
   * TODO: Double-check that this is not _more_ expensive then rmw sequence.
   */

  /* Enable timer peripheral interrupt for alarm.*/
  set_bits(&TIMER->inte, 1 << alarm);
  /* Clear any pending interrupt. */
  CORE_PPB->NVIC_ICPR = 1 << alarm;
  /* Enable IRQ in NVIC. */
  CORE_PPB->NVIC_ISER = 1 << alarm;
}

static inline void timer_alarm_in_us(enum timer_alarm alarm, u32 us) {
  u32 target = TIMER->timerawl + us;
  /* Set the alarm. */
  TIMER->alarms[alarm] = target;
}

#endif /* _TIMER_H */
