#include "woody.h"

extern elf_t			*elf;
extern injector		*inj;
extern bool				elf_alloc;
extern bool				elf_data_alloc;
extern bool				inj_alloc;
extern bool				inj_data_alloc;

void	close_all_fds()
{
	for (size_t i = 3; i < 1024; i++)
		close(i);
}

void	die(char* message)
{
	close_all_fds();
	free_all();
	printf("Controlled error:\n");
	perror(message);
	exit(-1);
}

int		free_all()
{
	if (inj_data_alloc) free(inj->data);
	if (inj_alloc) free(inj);
	if (elf_data_alloc) free(elf->data);
	if (elf_alloc) free(elf);
	return 0;
}

void	validate_file()
{
	___die	(		elf->data[EI_MAG0] != ELFMAG0
					||	elf->data[EI_MAG1] != ELFMAG1
					||	elf->data[EI_MAG2] != ELFMAG2
					||	elf->data[EI_MAG3] != ELFMAG3,
					"Invalid ELF file.");
	___die	(		elf->data[EI_CLASS] != ELFCLASS32
					&&	elf->data[EI_CLASS] != ELFCLASS64,
					"File is not of suitable bit class.");
}

void  read_original(char *filename, elf_t** elf_ptr)
{
	int				fd;
	int				filesize;
	long int	bytes_read;

	*elf_ptr = (elf_t*)malloc(sizeof(elf_t));
	___die (!elf_ptr, "Failed to alloc elf_ptr.");
	elf_alloc = true;
	fd = open(filename, O_RDONLY);
	___die (fd == -1, "Failed to open file");
	filesize = get_filesize(fd);
	___die (filesize == -1,
					"Failed to get file size");
	(*elf_ptr)->data_size = (uint32_t)filesize;
	(*elf_ptr)->data = malloc((*elf_ptr)->data_size);
	___die (!(*elf_ptr)->data, "Could not allocate.");
	elf_data_alloc = true;
	bytes_read = read(fd, (*elf_ptr)->data, filesize);
	___die (!bytes_read,
					"No bytes read. Is file empty?");
	___die (bytes_read == -1,
					"Error reading file");
	validate_file();
}
