// SPDX-License-Identifier: MIT
/* NOTE: Struct members/comments pulled from rp2040 datasheet. */
#ifndef _CORE_PPB_H
#define _CORE_PPB_H
#include "types.h"

struct core_ppb {
  u32 SYST_CSR;   /* SysTick Control and Status Register */
  u32 SYST_RVR;   /* SysTick Reload Value Register */
  u32 SYST_CVR;   /* SysTick Current Value Register */
  u32 SYST_CALIB; /* SysTick Calibration Value Register */
  u32 __pad7[56];
  u32 NVIC_ISER; /* Interrupt Set-Enable Register */
  u32 __pad0[31];
  u32 NVIC_ICER; /* Interrupt Clear-Enable Register */
  u32 __pad1[31];
  u32 NVIC_ISPR; /* Interrupt Set-Pending Register */
  u32 __pad2[31];
  u32 NVIC_ICPR; /* Interrupt Clear-Pending Register */
  u32 __pad3[95];
  u32 NVIC_IPR[8];
  u32 __pad4[568];
  u32 CPUID; /* CPUID Base Register */
  u32 ICSR;  /* Interrupt Control and State Register */
  u32 VTOR;  /* Vector Table Offset Register */
  u32 AIRCR; /* Application Interrupt and Reset Control Register */
  u32 SCR;   /* System Control Register */
  u32 CCR;   /* Configuration and Control Register */
  u32 __pad5;
  u32 SHPR2; /* System Handler Priority Register 2 */
  u32 SHPR3; /* System Handler Priority Register 3 */
  u32 SHCSR; /* System Handler Control and State Register */
  u32 __pad6[26];
  u32 MPU_TYPE; /* MPU Type Register */
  u32 MPU_CTRL; /* MPU Control Register */
  u32 MPU_RNR;  /* MPU Region Number Register */
  u32 MPU_RBAR; /* MPU Region Base Address Register */
  u32 MPU_RASR; /* MPU Region Attribute and Size Register */
};

#define PPB_BASE 0xe0000000
#define SYST_CSR_OFFSET 0xe010
/* Sanity checks. */
_Static_assert(sizeof(struct core_ppb) == (0xeda4 - SYST_CSR_OFFSET), "");
#define CORE_PPB ((volatile struct core_ppb *)(PPB_BASE + SYST_CSR_OFFSET))

static inline void core_ppb_set_vtable(u32 addr) {
  CORE_PPB->VTOR = addr;
  /* TODO: Barriers needed? We only do this once per core but still. */
  asm("dsb");
  asm("isb");
}

#endif /* _CORE_PPB_H */
