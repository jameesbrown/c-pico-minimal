// SPDX-License-Identifier: MIT
/*
 * BASE + 0x1000: XOR
 * BASE + 0x2000: Bitmask SET
 * BASE + 0x3000: Bitmask CLEAR
 *
 * See RP2040 Datasheet, Section 2.1.2. Atomic Register Access
 */
#ifndef _ATOMIC_ACCESS_H
#define _ATOMIC_ACCESS_H

#include "types.h"

static inline void xor_bits(volatile u32 *addr, u32 bits) {
  *((volatile u32 *)((u32)(addr) + 0x1000)) = bits;
}

static inline void set_bits(volatile u32 *addr, u32 bits) {
  *((volatile u32 *)((u32)(addr) + 0x2000)) = bits;
}

static inline void clear_bits(volatile u32 *addr, u32 bits) {
  *((volatile u32 *)((u32)(addr) + 0x3000)) = bits;
}

#endif /*_ATOMIC_ACCESS_H*/
