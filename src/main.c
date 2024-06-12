#include "woody.h"

dumpster	elf; // Yes, global.

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
		elf.shdr._32 = (Elf32_Shdr*)(elf.data + elf.ehdr._32->e_shoff);
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
		elf.shdr._64 = (Elf64_Shdr*)(elf.data + elf.ehdr._64->e_shoff);
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
	elf.data_size = filesize;
	elf.data = malloc(elf.data_size);
	___die (!elf.data, "Could not allocate.");
	bytes_read = read(fd, elf.data, filesize);
	___die (!bytes_read, "No bytes read. Is file empty?");
	___die (bytes_read == -1, "Error reading file");
}

ssize_t write_offset(int fd, unsigned int offset, void* src, size_t c)
{
	lseek(fd, offset, SEEK_SET);
	return write(fd, src, c);
}

void	write_file(const char *woody)
{
//	The intent here is to write the same ELF file
//	based on the read structure.
//	int fd;
	int	fd;
	ssize_t bytes_written = 0;

#ifndef _W
# define _W ___ok; bytes_written = write_offset
# define _E32 elf.ehdr._32
# define _P32 elf.phdr._32
# define _S32 elf.shdr._32
#endif

	fd = open(woody, O_WRONLY | O_CREAT, 00755);
	___die (fd == -1, "Failed to create `woody`. File exists?");
	_W(fd, 0, _E32, elf.data_size);
	if (elf.bit_class == 32)
	{
		_W(fd, 0, _E32, sizeof(Elf32_Ehdr));
//		_W(fd, _E32->e_phoff, _P32,
//			_E32->e_phentsize * elf.phnum);
		for (size_t i = 0; i < elf.phnum; i++)
		{
			printf("fd %d, off %d, add %p, size %d\n",
						fd,
						_P32[i].p_offset,
						&_E32 + _P32[i].p_paddr,
						_P32[i].p_filesz		); 
			_W(		fd,
						_P32[i].p_offset,
						&_E32 + _P32[i].p_paddr,
						_P32[i].p_filesz		); 
		}

		_W(fd, _E32->e_shoff, _S32, \
			_E32->e_shentsize * elf.shnum);
		___ok
	}
	else
	{
		bytes_written = write_offset(fd, 0, elf.ehdr._64, sizeof(Elf64_Ehdr));
		___ok
	}
	close(fd);
}

int		main(int argc, char **argv)
{
	___die(argc != 2, "Usage: `woody_woodpacker binary_file`");
	read_file(argv[1]);
	validate_file();
	elf_init();
	write_file("woody");
	free(elf.data);
	return (0);
}
