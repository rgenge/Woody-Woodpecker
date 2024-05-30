#include "woody.h"

t_elf	elf; // Yes, global.

void	validate_file()
{
	___die (elf.ehdr->e_ident[EI_MAG0] != ELFMAG0 ||
		elf.ehdr->e_ident[EI_MAG1] != ELFMAG1 ||
		elf.ehdr->e_ident[EI_MAG2] != ELFMAG2 ||
		elf.ehdr->e_ident[EI_MAG3] != ELFMAG3,
		"Invalid ELF file."); // Verify ELF header 
	___die (elf.ehdr->e_machine == EM_X86_64, 
		"File is not a x86_64 binary");
//	___die (elf.ehdr[EI_CLASS] != ELFCLASS32 &&
//		elf.ehdr[EI_CLASS != ELFCLASS64,
//		"Unknown data format.");
}

void  read_file(char *filename)
{
	int			fd;
	int			filesize;
	long int	bytes_read;

	fd = open(filename, O_RDONLY);
	___die (fd == -1, "Failed to open file");
	filesize = get_filesize(fd);
	___die (filesize == -1, "Failed to get file size");
	elf.data = malloc(filesize + 1);
	___die (!elf.data, "Could not allocate.");
	bytes_read = read(fd, elf.data, filesize);
	___die (!bytes_read, "Is file empty?");
	___die (bytes_read == -1, "Error reading file");
	elf.data[bytes_read] = 0; // Null-terminate.
//	___deb lin_dump(elf.data, bytes_read);
//	___deb hex_dump(elf.ehdr, sizeof(elf.ehdr));
	elf.ehdr = (typeof(elf.ehdr))elf.data;
}

void	cast_data_32_or_64()
{
//	elf.bit_class = elf.ehdr[EI_CLASS];
	pretty_print(&elf);
}

void	decrypt()
{
	//  t_dv        *dv;
	//  void        *ptr; 

//	char *h;
//
//	h = elf.phdr;
//	while (h - elf.phdr < elf.e_shentsize)
//	{
//		h++;
//	}

//	while (++i < elf.ehdr->e_phnum)
//	{
//		if(elf.phdr[i].p_vaddr == elf.phdr[i].p_paddr && elf.phdr[i].p_memsz == elf.phdr[i].p_filesz)
//		{
//			elf.phdr = &elf.phdr[i];
//		} 
//	}
//	printf("\n%ld", elf.phdr->p_memsz);
}

int		main(int ac, char **av)
{
	ft_memset(&elf, 0, sizeof(elf));
	if (ac != 2)
		die("Usage: `woody_woodpacker binary_file`");
	read_file(av[1]);
	validate_file();
	cast_data_32_or_64();
	decrypt();
	free(elf.data);
	return (0);
}
