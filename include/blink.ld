/* SPDX-License-Identifier: MIT */
SECTIONS {
  /*
   * NOTE: Boot stage 2 starts at 0x20041F00 (VMA = 0x20041F00).
   * See https://github.com/raspberrypi/pico-bootrom-rp2040
   */
  .boot 0x20041F00 : AT(0x10000000) {
    __boot_start = .;
    KEEP(*(.boot)) /* Keep startup code even if "unused". */
    __boot_end = .;
    . = ALIGN(0x100); /* Pad to 256 bytes (final 4 are CRC32). */
  }

  /* Ensure boot code/data fits in first 252 bytes (CRC32 uses last 4) */
  ASSERT(__boot_end - __boot_start <= 0xFC,
         "Boot section must fit within 252 bytes")

  .text 0x20000000 : AT(0x10000000 + SIZEOF(.boot)) {
    KEEP(*(.vectors));
    KEEP(*(.text))
    . = ALIGN(0x100);
  }
}
/* TODO: Should we limit to striped ram? */
__sram_striped_end = 0x20040000;
/* The number of bytes we have to copy into ram. */
__copy_len = SIZEOF(.text);
