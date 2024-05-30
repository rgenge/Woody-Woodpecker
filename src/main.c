#include "woody.h"

t_elf	elf; // Yes, global.

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
	elf.ehdr = (typeof(elf.ehdr))elf.data;
//	___deb hex_dump(elf.ehdr, sizeof(elf.ehdr));
	___die (elf.ehdr->e_ident[EI_MAG0] != ELFMAG0 ||
		elf.ehdr->e_ident[EI_MAG1] != ELFMAG1 ||
		elf.ehdr->e_ident[EI_MAG2] != ELFMAG2 ||
		elf.ehdr->e_ident[EI_MAG3] != ELFMAG3,
		"Invalid ELF file."); // Verify ELF header 
	___die (elf.ehdr->e_machine == EM_X86_64, 
		"File is not a x86_64 binary");

//	___deb say("elf.ehdr->e_shentsize", &elf.ehdr->e_shentsize, 's');

	elf.phdr = (typeof(elf.phdr))elf.data \
		+ sizeof(elf.ehdr);
	if ((void*)elf.phdr == (void*)elf.ehdr)
		elf.phdr = 0;
//	___deb hex_dump(elf.phdr, sizeof(elf.phdr));

	pretty_print(&elf);
}

void  decrypt()
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

int	main(int ac, char **av)
{
	ft_memset(&elf, 0, sizeof(elf));
	if (ac != 2)
		die("Usage: `woody_woodpacker binary_file`");
	read_file(av[1]);
	decrypt();
	free(elf.data);
	return (0);
}
