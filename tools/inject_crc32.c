// SPDX-License-Identifier: MIT
#include "crc32table.h"
#include "elf32.h"
#include <stdio.h>
#include <string.h>

#define SIZE 256

/**
 * crc32_be - Compute a big-endian CRC32 checksum.
 */
static unsigned int crc32_be(const unsigned char *bytes, size_t len) {
  unsigned int crc = 0xFFFFFFFF;
  while (len--)
    crc = (crc << 8) ^ crc32table[((crc >> 24) ^ *bytes++)];
  return crc;
}

static int validate_file(FILE *f) {
  struct Elf32_Ehdr ehdr = {0};

  if (!f) {
    perror("fopen");
    return 0;
  }

  if (!fread(&ehdr, sizeof(ehdr), 1, f)) {
    perror("fread");
    return 0;
  }

  if (memcmp(ehdr.e_ident, ELF_MAG, SELFMAG) != 0) {
    fprintf(stderr, "Not an ELF file.\n");
    fclose(f);
    return 0;
  }

  if (ehdr.e_ident[EI_CLASS] != ELFCLASS32) {
    fprintf(stderr, "Not a 32-bit ELF file.\n");
    return 0;
  }

  /* FIXME: We really just need the first program header to be boot 2.
   * we should validate that that is the case.
   */
  if (ehdr.e_phnum < 1) {
    fprintf(stderr, "Expected a program segment, but found none.\n");
    return 0;
  }

  /* Seek to the program segment header. */
  if (fseek(f, ehdr.e_phoff, SEEK_SET)) {
    perror("fseek");
    return 0;
  }

  struct Elf32_Phdr phdr = {0};

  if (!fread(&phdr, sizeof(phdr), 1, f)) {
    perror("fread");
    return 0;
  }

  if (phdr.p_memsz != SIZE) {
    fprintf(stderr, "Have p_memsz %u but expected %u\n", phdr.p_memsz, SIZE);
    return 0;
  }

  if (phdr.p_type != PT_LOAD) {
    fprintf(stderr, "The program segment is not loadable.\n");
    return 0;
  }

  /* Make sure executable and readable. */
  if ((phdr.p_flags & (PF_X | PF_R)) != (PF_X | PF_R)) {
    fprintf(stderr, "The program segment is not executable and readable.\n");
    return 0;
  }

  /* Seek to the program segment. */
  if (fseek(f, phdr.p_offset, SEEK_SET)) {
    perror("fseek");
    return 0;
  }

  return 1;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <elf-file>\n", argv[0]);
    return 1;
  }

  const char *filename = argv[1];
  FILE *f = fopen(filename, "rb+");

  if (!validate_file(f)) {
    return 1;
  }

  unsigned char buffer[SIZE - CRC32_SIZE] = {};
  _Static_assert(sizeof(buffer) == SIZE - CRC32_SIZE, "Incorrect buffer size");

  /* From: Raspberry Pi RP2040 Datasheet (rev 1.4)
   * Section: 2.8.1.3.1 - Flash boot stage 2 format
   * URL: https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf
   * Summary:
   *   - The last 4 bytes of the image loaded from flash are a CRC32 checksum
   *     of the first 252 bytes.
   *   - The checksum is stored as a little-endian 32-bit integer.
   */
  if (!fread(buffer, SIZE - CRC32_SIZE, 1, f)) {
    perror("fread");
    return 1;
  }

  unsigned int crc = crc32_be(buffer, SIZE - CRC32_SIZE);

  unsigned char crc_le[CRC32_SIZE] = {(crc >> 0) & 0xFF, (crc >> 8) & 0xFF,
                                      (crc >> 16) & 0xFF, (crc >> 24) & 0xFF};

  /* Required by C standard when switching from reading to writing. */
  if (fseek(f, 0, SEEK_CUR)) {
    perror("fseek");
    return 1;
  }

  if (!fwrite(&crc_le, CRC32_SIZE, 1, f)) {
    perror("fwrite");
    return 1;
  }

  printf("CRC32 0x%08X injected into %s\n", crc, filename);

  fclose(f);
  return 0;
}
