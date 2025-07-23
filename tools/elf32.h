/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 1996
 *	The Regents of the University of California. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _ELF32_H_
#define _ELF32_H_

/* ELF Identification Indexes */
#define EI_CLASS 4
#define ELFCLASS32 1

/* Magic number */
#define ELF_MAG "\177ELF"
#define SELFMAG 4

/* Segment types */
#define PT_LOAD 1

/* Segment permissions */
#define PF_X (1 << 0) /* Executable */
#define PF_W (1 << 1) /* Writable */
#define PF_R (1 << 2) /* Readable */

/* ELF32 File Header */
struct Elf32_Ehdr {
  unsigned char e_ident[16];
  unsigned short e_type;
  unsigned short e_machine;
  unsigned int e_version;
  unsigned int e_entry;
  unsigned int e_phoff;
  unsigned int e_shoff;
  unsigned int e_flags;
  unsigned short e_ehsize;
  unsigned short e_phentsize;
  unsigned short e_phnum;
  unsigned short e_shentsize;
  unsigned short e_shnum;
  unsigned short e_shstrndx;
};

/* ELF32 Program Header */
struct Elf32_Phdr {
  unsigned int p_type;
  unsigned int p_offset;
  unsigned int p_vaddr;
  unsigned int p_paddr;
  unsigned int p_filesz;
  unsigned int p_memsz;
  unsigned int p_flags;
  unsigned int p_align;
};

#endif /* _ELF32_H_ */
