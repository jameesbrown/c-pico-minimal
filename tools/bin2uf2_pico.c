// SPDX-License-Identifier: MIT
#include "crc32table.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Flash start address on RP2040. */
#define BASE_ADDRESS 0x10000000
#define RP2040_FAM_ID 0xE48BFF56
/* 256 bytes. */
#define BLOCK_SIZE 256u
#define UF2_MAG0 0x0A324655
#define UF2_MAG1 0x9E5D5157
#define UF2_MAG2 0x0AB16F30
#define UF2_FLAG_FAM_ID_PRESENT 0x00002000

/* See https://github.com/microsoft/uf2?tab=readme-ov-file#file-format. */
struct uf2_block {
  /* 32 byte header */
  uint32_t magic_start0;
  uint32_t magic_start1;
  uint32_t flags;
  uint32_t target_addr;
  uint32_t payload_size;
  uint32_t block_num;
  uint32_t num_blocks;
  uint32_t family_id;
  uint8_t data[476];
  uint32_t magic_end;
};
/**
 * init_uf2_block - Initialize a uf2 block from a 256-byte aligned source.
 * Parameters that are specific to RP2040 (eg. family_id) are defaulted.
 */
static inline void init_uf2_block(struct uf2_block *block, const uint8_t *src,
                                  uint32_t target_addr, uint32_t block_num,
                                  uint32_t num_blocks) {
  block->magic_start0 = UF2_MAG0;
  block->magic_start1 = UF2_MAG1;
  block->flags = UF2_FLAG_FAM_ID_PRESENT;
  block->target_addr = target_addr;
  block->payload_size = BLOCK_SIZE;
  block->block_num = block_num;
  block->num_blocks = num_blocks;
  block->family_id = RP2040_FAM_ID;
  memset(block->data, 0, sizeof(block->data));
  memcpy(block->data, src, BLOCK_SIZE);
  block->magic_end = UF2_MAG2;
}
/**
 * write_uf2 - Either create or replace a uf2 file with dest_name.
 */
static int write_uf2(const char *dest_name, struct uf2_block *src,
                     uint32_t num_blocks) {
  FILE *f = fopen(dest_name, "wb");
  if (!f) {
    perror("fopen");
    return 1;
  }
  if (fwrite(src, sizeof(src[0]), num_blocks, f) < num_blocks) {
    perror("fwrite");
    return 1;
  }
  fclose(f);
  return 0;
}

/**
 * copy_to_uf2 - For each 256 byte segment of src, copy that to a \c uf2_block
 * and store it at dest, incrementing the pointers each time.
 */
static void copy_to_uf2(struct uf2_block *dest, const uint8_t *src,
                        const uint32_t num_blocks) {
  uint32_t address = BASE_ADDRESS;
  uint32_t block_num = 0;
  while (block_num < num_blocks) {
    init_uf2_block(/*block=*/dest, /*src=*/src, /*target_addr=*/address,
                   /*block_num=*/block_num++,
                   /*num_blocks=*/num_blocks);
    dest++;
    src += BLOCK_SIZE;
    printf("Placed uf2 block %u @<0x%04x>.\n", block_num, address);
    address += BLOCK_SIZE;
  }
}

/*
 * Converts from .bin to an RP2040-specific .uf2.
 */
int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Usage: %s <bin-file> <uf2-file>\n", argv[0]);
    return (1);
  }

  const char *src_name = argv[1];
  const char *dest_name = argv[2];

  FILE *f = fopen(src_name, "rb");
  if (!f) {
    perror("fopen");
    return 1;
  }

  /* Get file size. */
  fseek(f, 0, SEEK_END);
  const size_t size = ftell(f);
  printf("%lu bytes read from %s\n", size, src_name);
  if (size <= 0) {
    fprintf(stderr, "Invalid file size %lu.\n", size);
    return 1;
  }

  if (size > 0xFFFFFFFF) {
    fprintf(stderr, "File is too large.\n");
    return 1;
  }

  uint32_t num_blocks = size >> 8;
  if ((num_blocks << 8) != size) {
    fprintf(stderr, "Not 256 byte aligned!\n");
    return 1;
  }
  /* Go back to the beginning. */
  rewind(f);
  uint8_t *buffer = malloc(size);
  if (!buffer) {
    perror("malloc");
    return 1;
  }
  struct uf2_block *blocks = malloc(sizeof(struct uf2_block) * num_blocks);
  if (!blocks) {
    perror("malloc");
    free(buffer);
    return 1;
  }
  if (fread(buffer, /*size=*/sizeof(buffer[0]), /*nitems=*/size, f) != size) {
    perror("fread");
    return 1;
  }
  fclose(f);
  copy_to_uf2(blocks, buffer, num_blocks);
  printf("Produced %u uf2 blocks.\n", num_blocks);
  return write_uf2(dest_name, blocks, num_blocks);
}
