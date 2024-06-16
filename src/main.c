#include "woody.h"

dumpster	elf; // Yes, global.
void*			memo;
void*			zero;
size_t		size;

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

void	M(size_t offset, size_t c)
{
	void *h;
	h = ft_memcpy(memo + offset, zero + offset, c);
	___die(!h, "Failed to copy memory chunk.");
}

void	write_file(const char *woody)
{
	memo = calloc(elf.data_size, 1);
	___die(!memo, "Failed to prepare alloc block.");

	zero = elf.data;
	size = elf.data_size;

	if (elf.bit_class == 32)
	{
		M (0, sizeof(*_E32));

		for (size_t i = 0; i < elf.phnum; i++)
		{

//			___deb printf( ">> %d : %d\n",
//						_P32[i].p_offset,
//						_P32[i].p_filesz		); 

			M (		_P32[i].p_offset,
						_P32[i].p_filesz		); 

		}

		for (size_t i = 1; i < elf.shnum; i++) // unused 0
		{

//			___deb printf( ">> %d : %d\n",
//						_S32[i].sh_addr,
//						_S32[i].sh_size		); 

			M (		_S32[i].sh_addr,
						_S32[i].sh_size		); 

		}

	} // elseif (elf.bit_class == 64) {...}
	else { ___die(true, "Unknown or not implemented e_type.");}

	int		fd;
	fd = open(woody, O_WRONLY | O_CREAT, 00755);
	___die (fd == -1, "Failed to create `woody`. File exists?");
	___die (write(fd, memo, size) == -1, "Could not write to file.");
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
