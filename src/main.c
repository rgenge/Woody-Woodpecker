#include "woody.h"

char debug = 1;

void  read_file(t_elf *elf, char *filename)
{
	int			fd;
	int			filesize;
	long int	bytes_read;

	fd = open(filename, O_RDONLY);
	___die (fd == -1, "Failed to open file.");
	filesize = get_filesize(fd);
	___die (filesize == -1, "Failed to get file size.");
	elf->data = malloc(filesize + 1);
	___die (!elf->data, "Could not allocate.");
	bytes_read = read(fd, elf->data, filesize);
	___die (!bytes_read, "Is file empty?");
	___die (bytes_read == -1, "Error reading file");
	elf->data[bytes_read] = 0; // Null-terminate.
	___deb printf("Bytes read: %ld\n", bytes_read);
	___deb hex_dump(elf->data, bytes_read);
	___deb lin_dump(elf->data, bytes_read);

	//    FILE* input_elf = fopen(filename, "rb");
	//	___die (!input_elf, "Error opening file.\n");
	//    Elf64_Ehdr ehdr;
	//    fread(&ehdr, sizeof(Elf64_Ehdr), 1, input_elf);
	//
	//    // Verify ELF header 
	//    if (ehdr.e_ident[EI_MAG0] != ELFMAG0 ||
	//        ehdr.e_ident[EI_MAG1] != ELFMAG1 ||
	//        ehdr.e_ident[EI_MAG2] != ELFMAG2 ||
	//        ehdr.e_ident[EI_MAG3] != ELFMAG3) {
	//        printf("Invalid ELF file.\n");
	//        exit(1) ;
	//    }
	//    printf("%d", filesize);
}

void  decrypt_file(t_elf *elf)
{
	Elf64_Ehdr  *ehdr;
	Elf64_Phdr  *phdr;
	//  t_dv        *dv;
	//  void        *ptr; 
	int i = -1;

	ehdr = (Elf64_Ehdr*)elf->data;
	phdr = (Elf64_Phdr*)elf->data + ehdr->e_phoff;
	while (++i < ehdr->e_phnum)
	{
		if(phdr[i].p_vaddr == phdr[i].p_paddr && phdr[i].p_memsz == phdr[i].p_filesz)
		{
			elf->code = &phdr[i];
		} 
	}
	printf("\n%ld", elf->code->p_memsz);
	return;

	printf("decypt: %s", (char *)elf->data);
}

int	main(int ac, char **av)
{
	t_elf   elf;

	ft_memset(&elf, 0, sizeof(elf));
	if (ac != 2)
		die("Usage: `woody_woodpacker binary_file`");
	read_file(&elf, av[1]);
	//    decrypt_file(&elf);
	free(elf.data);
}
