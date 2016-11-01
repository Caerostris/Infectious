#ifndef _ELF_H_SD9F8W9045
#define _ELF_H_SD9F8W9045

#include <inttypes.h>

// Source: https://uclibc.org/docs/elf-64-gen.pdf

#define EI_MAG0 0 // 0x7F
#define EI_MAG1 1 // 0x45 (E)
#define EI_MAG2 2 // 0x4C (L)
#define EI_MAG3 3 // 0x46 (F)
#define EI_CLASS 4 // 0x1 for 32-bit, 0x2 for 64-bit
#define EI_DATA 5 // 0x1 for little endian, 0x2 for big
#define EI_VERSION 6 // 0x1 for ELF v1
#define EI_OSABI 7 // OS target ABI
#define EI_ABIVERSION 8 // ABI version, ABI specific
#define EI_PAD 9 // Padding, unused

/*
64-bit ELF Header
================
16-byte: e_ident
(see above)
----------------
2-byte: e_type
0x1: relocatable
0x2: executable
0x3: shared
0x4: core
----------------
2-byte: e_machine
(examples)
0x03: x86
0x08: MIPS
0x28: ARM
0x3E: x86-64
0xB7: AArch64
----------------
4-byte: e_version
0x1: Version 1
----------------
8-byte: e_entry
Address of program entry point
----------------
8-byte: e_phoff
Start of program header table
Usually 0x40 for 64-bit ELF
----------------
8-byte: e_shoff
Start of section header table
----------------
...
*/

typedef uint16_t Elf64_Half;
typedef uint32_t Elf64_Word;
typedef uint64_t Elf64_Xword;
typedef uint64_t Elf64_Addr;
typedef uint64_t Elf64_Off;

struct Elf64_Header {
	unsigned char e_ident[16];
	Elf64_Half e_type;
	Elf64_Half e_machine;
	Elf64_Word e_version;
	Elf64_Addr e_entry;
	Elf64_Off e_phoff;
	Elf64_Off e_shoff;
	Elf64_Word e_flags;
	Elf64_Half e_ehsize;
	Elf64_Half e_phentsize;
	Elf64_Half e_phnum;
	Elf64_Half e_shentsize;
	Elf64_Half e_shnum;
	Elf64_Half e_shstrndx;
};

#define SH_NAME_LENGTH 256
#define SHN_UNDEF 0 // Undefined section

#define GET_SECTION_HEADER_FILE_OFFSET(elf_header, section_index) \
	(elf_header->e_shoff + sizeof(struct Elf64_Section_Header) * section_index)
#define GET_SECTION_HEADER(elf_blob, elf_header, section_index) \
	((struct Elf64_Section_Header *)(elf_blob + GET_SECTION_HEADER_FILE_OFFSET(elf_header, section_index)))

struct Elf64_Section_Header {
	Elf64_Word sh_name;
	Elf64_Word sh_type;
	Elf64_Xword sh_flags;
	Elf64_Addr sh_addr;
	Elf64_Off sh_offset;
	Elf64_Xword sh_size;
	Elf64_Word sh_link;
	Elf64_Word sh_info;
	Elf64_Xword sh_addralign;
	Elf64_Xword sh_entsize;
};

#endif // _ELF_H_SD9F8W9045
