// SPDX-License-Identifier: MIT
/* NOTE: Struct members/comments pulled from rp2040 datasheet. */
#ifndef _CLOCKS_H
#define _CLOCKS_H
#include "types.h"
struct clocks {
  u32 clk_gpout0_ctrl; /* Clock control, can be changed on-the-fly (except for
                          auxsrc). */

  u32 clk_gpout0_div; /* Clock divisor, can be changed on-the-fly. */

  u32 clk_gpout0_selected; /* Indicates which SRC is currently selected by the
                              glitchless mux (one-hot). */

  u32 clk_gpout1_ctrl; /* Clock control, can be changed on-the-fly (except for
                          auxsrc). */

  u32 clk_gpout1_div; /* Clock divisor, can be changed on-the-fly. */

  u32 clk_gpout1_selected; /* Indicates which SRC is currently selected by the
                              glitchless mux (one-hot). */

  u32 clk_gpout2_ctrl; /* Clock control, can be changed on-the-fly (except for
                          auxsrc). */

  u32 clk_gpout2_div; /* Clock divisor, can be changed on-the-fly */

  u32 clk_gpout2_selected; /* Indicates which SRC is currently selected by the
                              glitchless mux (one-hot). */

  u32 clk_gpout3_ctrl; /* Clock control, can be changed on-the-fly (except for
                          auxsrc) */

  u32 clk_gpout3_div; /* Clock divisor, can be changed on-the-fly k*/

  u32 clk_gpout3_selected; /* Indicates which SRC is currently selected by the
                              glitchless mux (one-hot). */

  u32 clk_ref_ctrl; /* Clock control, can be changed on-the-fly (except for
                       auxsrc) */
  u32 clk_ref_div;  /* Clock divisor, can be changed on-the-fly */

  u32 clk_ref_selected; /* Indicates which SRC is currently selected by the
                           glitchless mux (one-hot). */

  u32 clk_sys_ctrl; /* Clock control, can be changed on-the-fly (except for
                       auxsrc) */

  u32 clk_sys_div; /* Clock divisor, can be changed on-the-fly. */

  u32 clk_sys_selected; /* Indicates which SRC is currently selected by the
                           glitchless mux (one-hot). */

  u32 clk_peri_ctrl; /* Clock control, can be changed on-the-fly (except for
                        auxsrc). */
  u32 _pad;
  u32 clk_peri_selected; /* Indicates which SRC is currently selected by the
                            glitchless mux (one-hot). */

  u32 clk_usb_ctrl; /* Clock control, can be changed on-the-fly (except for
                       auxsrc) */
  u32 clk_usb_div;  /* Clock divisor, can be changed on-the-fly. */

  u32 clk_usb_selected; /* Indicates which SRC is currently selected by the
                           glitchless mux (one-hot). */

  u32 clk_adc_ctrl; /* Clock control, can be changed on-the-fly (except for
                       auxsrc) */

  u32 clk_adc_div; /* Clock divisor, can be changed on-the-fly. */

  u32 clk_adc_selected; /* Indicates which SRC is currently selected by the
                           glitchless mux (one-hot). */

  u32 clk_rtc_ctrl; /* Clock control, can be changed on-the-fly (except for
                       auxsrc) */
  u32 clk_rtc_div;  /* Clock divisor, can be changed on-the-fly */

  u32 clk_rtc_selected; /* Indicates which SRC is currently selected by the
                           glitchless mux (one-hot). */
  u32 clk_sys_resus_ctrl;
  u32 clk_sys_resus_status;
  u32 fc0_ref_khz; /* Reference clock frequency in kHz. */

  u32 fc0_min_khz; /* Minimum pass frequency in kHz. This is optional. Set to 0
                      if you are not using the pass/fail flags. */

  u32 fc0_max_khz; /* Maximum pass frequency in kHz. This is optional. Set to
                      0x1ffffff if you are not using the pass/fail flags. */

  u32 fc0_delay; /* Delays the start of frequency counting to allow the mux to
                    settle. Delay is measured in multiples of the reference
                    clock period. */

  u32 fc0_interval; /* The test interval is 0.98us * 2**interval, but let’s call
                       it 1us * 2**interval The default gives a test interval of
                       250us */
  u32 fc0_src; /*Clock sent to frequency counter, set to 0 when not required
                  Writing to this register initiates the frequency count*/

  u32 fc0_status; /* Frequency counter status. */

  u32 fc0_result; /* Result of frequency measurement, only valid when
                     status_done=1. */

  u32 wake_en0;  /* Enable clock in wake mode */
  u32 wake_en1;  /* Enable clock in wake mode */
  u32 sleep_en0; /* Enable clock in sleep mode */
  u32 sleep_en1; /* Enable clock in sleep mode */
  u32 enabled0;  /* Indicates the state of the clock enable. */
  u32 enabled1;  /* Indicates the state of the clock enable. */
  u32 intr;      /* Raw Interrupts */
  u32 inte;      /* Interrupt Enable */
  u32 intf;      /* Interrupt Force */
  u32 ints;      /* Interrupt status after masking & forcing. */
};

#define CLOCKS_BASE 0x40008000
#define CLOCKS ((volatile struct clocks *)(CLOCKS_BASE))
/*
 * 0x0 → ROSC_CLKSRC_PH
 * 0x1 → CLKSRC_CLK_REF_AUX
 * 0x2 → XOSC_CLKSRC
 */
#define CLK_REF_XOSC_CLKSRC 0x2
#define CLK_SYS_REF_CLKSRC 0x0

/* NOTE: clk_ref_selected will have at most one _decoded_ bit corresponding to
 * the enumerated value. E.g, XOSC_CLKSRC (0x2) corresponds to the second bit
 * 0-indexed, i.e. 1 << 2.
 */
#define CLK_REF_SELECTED_XOSC_CLKSRC 1 << 2
#define CLK_SYS_SELECTED_REF_CLKSRC 1 << 0

/*
 * > To switch the glitchless mux:
 * > Switch the glitchless mux to an alternate source
 * > poll the SELECTED register until the switch is completed.
 * Source: rp2040 datasheet.
 */
static inline void clocks_set_ref_to_xosc() {
  /* Set clock source glitchlessly to XOSC. */
  CLOCKS->clk_ref_ctrl = CLK_REF_XOSC_CLKSRC;
  /* Spin until we see our new source selected. */
  while (!(CLOCKS->clk_ref_selected & CLK_REF_SELECTED_XOSC_CLKSRC))
    ;
  /* TODO: Is this needed? */
  /* Make sure the system clock source is the reference clock. */
  CLOCKS->clk_sys_ctrl = CLK_SYS_REF_CLKSRC;
  while ((CLOCKS->clk_sys_selected) != CLK_SYS_SELECTED_REF_CLKSRC)
    ;
}

#endif /*_CLOCKS_H*/
