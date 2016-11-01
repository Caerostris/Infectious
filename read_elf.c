#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "elf.h"

int main(int argc, char *argv[])
{
	struct stat sb;
	struct Elf64_Header *header;
	int fd;
	const char *elf_blob;

	if(argc != 2)
	{
		printf("Usage: %s elf_file\n", argv[0]);
		return 1;
	}

	fd = open(argv[1], O_RDONLY);
	if(fd == -1)
	{
		fprintf(stderr, "%s: open(%s): %s\n", argv[0], argv[1], strerror(errno));
		return 1;
	}

	if(fstat(fd, &sb) == -1)
	{
		fprintf(stderr, "%s: fstat(%s): %s\n", argv[0], argv[1], strerror(errno));
		return 1;
	}

	elf_blob = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if(elf_blob == MAP_FAILED)
	{
		fprintf(stderr, "%s: mmap(%s): %s\n", argv[0], argv[1], strerror(errno));
		return 1;
	}

	header = (struct Elf64_Header *)elf_blob;

	// attempt to match magic header
	if
	(
		header->e_ident[EI_MAG0] != 0x7F ||
		header->e_ident[EI_MAG1] != 0x45 ||
		header->e_ident[EI_MAG2] != 0x4C ||
		header->e_ident[EI_MAG3] != 0x46
	)
	{
		fprintf(stderr, "%s: Not an ELF binary\n", argv[1]);
		return 1;
	}

	printf(
		"Magic Number: 0x%x %c%c%c\n",
		header->e_ident[EI_MAG0],
		header->e_ident[EI_MAG1],
		header->e_ident[EI_MAG2],
		header->e_ident[EI_MAG3]
	);

	printf("Program entry point: 0x%x\n", header->e_entry);
	printf("Section table at offset: 0x%x \n", header->e_shoff);

	// Check Section Header String Table exists
	if(header->e_shstrndx == SHN_UNDEF)
	{
		fprintf(stderr, "No Section Header String Table defined in binary\n");
		return 1;
	}

	// Get Section Header String Table (for looking up section names)
	struct Elf64_Section_Header *shstrtab =
		GET_SECTION_HEADER(elf_blob, header, header->e_shstrndx);
	char *section_header_string_table = elf_blob + shstrtab->sh_offset;

	for(int i = 0; i < header->e_shnum; i++)
	{
		struct Elf64_Section_Header *section_header =
			GET_SECTION_HEADER(elf_blob, header, i);
		char section_name[SH_NAME_LENGTH];
		char *name_start =
			section_header_string_table + section_header->sh_name;
		strncpy(section_name, name_start, SH_NAME_LENGTH);
		section_name[SH_NAME_LENGTH - 1] = '\0';

		printf("Section\n");
		printf("\tName: %s", section_name);
		printf("\tType: 0x%x\n", section_header->sh_type);
		printf("\tContent offset: 0x%x\n", section_header->sh_offset);
		printf("\tSize: %lu bytes\n", section_header->sh_size);

		if(strncmp(section_name, ".text", strlen(".text")) == 0)
		{
			printf("TEXT!!\n");
			if(section_header->sh_offset != header->e_entry)
			{
				printf("Doh, wrong text: %x vs %x\n", section_header->sh_offset, header->e_entry);
			}
		}
	}

	close(fd);
}
