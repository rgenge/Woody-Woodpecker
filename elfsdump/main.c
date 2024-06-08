#include "elfsdump.h"

dumpster			elf;
unsigned char	*h;
uint16_t			u16;
uint32_t			u32;
//	Elf32_Phdr		*p;
//	Elf32_Shdr		*s;
//	Elf32_Half		*half;
//	Elf32_Verdef	*v_def;
//	Elf32_Verneed	*v_need;
//	size_t			pi;
//	size_t			sh_entries;
//	size_t			sh_string_table_i;
//
//	e = (Elf32_Ehdr*)ex->ehdr;
//	p = (Elf32_Phdr*)ex->phdr;
//	s = (Elf32_Shdr*)((void*)e + e->e_shoff);
//	sh_entries = e->e_shnum == 0 ? s[0].sh_size : e->e_shnum;
//	sh_string_table_i = e->e_shstrndx == SHN_XINDEX ? s[0].sh_link : e->e_shstrndx;

void close_all_fds()
{
	for (size_t i = 3; i < 1024; i++)
	  close(i);
}

void die(char* message)
{
	close_all_fds();
	perror(message);
	free(elf.data);
	exit(1);
}

void	take_a_dump()
{
	if (elf.bit_class == 32)
		pretty_print32();
	else if (elf.bit_class == 64)
		pretty_print64();
	else
		die(BAD_ARGUMENTS);
}

off_t	get_filesize(int fd)
{
	off_t result = lseek(fd, 0, SEEK_END);

	if (result == -1 || lseek(fd, 0, SEEK_SET) == -1) {
		return -1;
	}
	return result;
}

void	validate_file()
{
	___die (elf.data[EI_MAG0] != ELFMAG0 ||
			elf.data[EI_MAG1] != ELFMAG1 ||
			elf.data[EI_MAG2] != ELFMAG2 ||
			elf.data[EI_MAG3] != ELFMAG3,
			"Invalid ELF file."); // Verify ELF header 
	___die (elf.data[EI_CLASS] != ELFCLASS32 &&
			elf.data[EI_CLASS] != ELFCLASS64,
			"File is not suitable bit class.");
}

void	elf_init()
{
	elf.bit_class =
		elf.data[EI_CLASS] == ELFCLASS64 ? 64
		: elf.data[EI_CLASS] == ELFCLASS32 ? 32
		: 0;
	if (elf.bit_class == 32)
	{
		elf.ehdr._32 = (Elf32_Ehdr*)elf.data;
		elf.phdr._32 = (Elf32_Phdr*)(elf.data + elf.ehdr._32->e_phoff);
		elf.shdr._32 = (Elf32_Shdr*)(elf.data + elf.ehdr._32->e_phoff);
		elf.phnum = elf.ehdr._32->e_phnum == PN_XNUM ?
			elf.shdr._32->sh_info : elf.ehdr._32->e_phnum;
		elf.shnum = elf.ehdr._32->e_shnum == 0 ?
			elf.shdr._32[0].sh_size : elf.ehdr._32->e_shnum;
		elf.shstrndx = elf.ehdr._32->e_shstrndx == SHN_XINDEX ?
			elf.shdr._32[0].sh_link : elf.ehdr._32->e_shstrndx;
	}
	else if (elf.bit_class == 64)
	{
		elf.ehdr._64 = (Elf64_Ehdr*)elf.data;
		elf.phdr._64 = (Elf64_Phdr*)(elf.data + elf.ehdr._64->e_phoff);
		elf.shdr._64 = (Elf64_Shdr*)(elf.data + elf.ehdr._64->e_phoff);
		elf.phnum = elf.ehdr._64->e_phnum == PN_XNUM ?
			elf.shdr._64->sh_info : elf.ehdr._64->e_phnum;
		elf.shnum = elf.ehdr._64->e_shnum == 0 ?
			elf.shdr._64[0].sh_size : elf.ehdr._64->e_shnum;
		elf.shstrndx = elf.ehdr._64->e_shstrndx == SHN_XINDEX ?
			elf.shdr._64[0].sh_link : elf.ehdr._64->e_shstrndx;
	}
}

void  read_file(char *filename)
{
	int				fd;
	int				filesize;
	long int	bytes_read;

	fd = open(filename, O_RDONLY);
	___die (fd == -1, "Failed to open file");
	filesize = get_filesize(fd);
	___die (filesize == -1, "Failed to get file size");
	elf.data = malloc(filesize + 1);
	___die (!elf.data, "Could not allocate.");
	bytes_read = read(fd, elf.data, filesize);
	___die (!bytes_read, "No bytes read. Is file empty?");
	___die (bytes_read == -1, "Error reading file");
	elf.data[bytes_read] = 0; // Null-terminate.
}

int main(int argc, char **argv) {
	___die(argc != 2, BAD_ARGUMENTS);
	read_file(argv[1]);
	validate_file();
	elf_init();
	take_a_dump();
	free(elf.data);
	printf(ELF_DUMP);
}
