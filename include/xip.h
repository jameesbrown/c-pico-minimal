// SPDX-License-Identifier: MIT
/* NOTE: Struct members/comments pulled from rp2040 datasheet. */
#ifndef _XIP_H
#define _XIP_H

#include "types.h"
struct xip {
  u32 ctrlr0;         /* Control register 0 */
  u32 ctrlr1;         /* Master Control register 1 */
  u32 ssienr;         /* SSI Enable */
  u32 mwcr;           /* Microwire Control */
  u32 ser;            /* Slave enable */
  u32 baudr;          /* Baud rate */
  u32 txftlr;         /* TX FIFO threshold level */
  u32 rxftlr;         /* RX FIFO threshold level */
  u32 txflr;          /* TX FIFO level */
  u32 rxflr;          /* RX FIFO level */
  u32 sr;             /* Status register */
  u32 imr;            /* Interrupt mask */
  u32 isr;            /* Interrupt status */
  u32 risr;           /* Raw interrupt status */
  u32 txoicr;         /* TX FIFO overflow interrupt clear */
  u32 rxoicr;         /* RX FIFO overflow interrupt clear */
  u32 rxuicr;         /* RX FIFO underflow interrupt clear */
  u32 msticr;         /* Multi-master interrupt clear */
  u32 icr;            /* Interrupt clear */
  u32 dmacr;          /* DMA control */
  u32 dmatdlr;        /* DMA TX data level */
  u32 dmardlr;        /* DMA RX data level */
  u32 idr;            /* Identification register */
  u32 ssi_version_id; /* Version ID */
  u32 dr0[36];        /* Data Register 0-36 */
  u32 rx_sample_dly;  /* RX sample delay */
  u32 spi_ctrlr0;     /* SPI control 0xf8 TXD_DRIVE_EDGE TX drive edge */
};

#define XIP_BASE 0x18000000
#define XIP ((volatile struct xip *)(XIP_BASE))

/**
 * xip_enable_for_reading - Basic XIP setup for reading from flash.
 */
static inline void xip_enable_for_reading() {
  XIP->ssienr = 0;
  XIP->ctrlr0 = (0x3 << 8) | (0x1F << 16);
  XIP->ssienr = 0x1; /* Enable */
}

static inline void xip_disable() { XIP->ssienr = 0x0; }

#endif /* _XIP_H */
