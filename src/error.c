#include "woody.h"

extern dumpster		*elf;
extern injector		*inj;
extern void*			file_out;
extern bool				elf_alloc;
extern bool				elf_data_alloc;
extern bool				inj_alloc;
extern bool				inj_data_alloc;
extern bool				inj_bin_alloc;
extern bool				file_out_alloc;

void close_all_fds()
{
	for (size_t i = 3; i < 1024; i++)
		close(i);
}

void die(char* message)
{
	close_all_fds();
	free_all();
	printf("Controlled error:\n");
	perror(message);
	exit(-1);
}

int	free_all()
{
	if (inj_bin_alloc) free(inj->bin);
	if (inj_data_alloc) free(inj->data);
	if (inj_alloc) free(inj);
	if (elf_data_alloc) free(elf->data);
	if (elf_alloc) free(elf);
	if (file_out_alloc) free(file_out);
	return 0;
}

void	validate_file()
{
	___die (elf->data[EI_MAG0] != ELFMAG0 ||
		elf->data[EI_MAG1] != ELFMAG1 ||
		elf->data[EI_MAG2] != ELFMAG2 ||
		elf->data[EI_MAG3] != ELFMAG3,
		"Invalid ELF file.");
	___die (elf->data[EI_CLASS] != ELFCLASS32 &&
		elf->data[EI_CLASS] != ELFCLASS64,
		"File is not of suitable bit class.");
}

void  read_original_elf(char *filename)
{
	int				fd;
	int				filesize;
	long int	bytes_read;
	elf = calloc(sizeof(dumpster), 1);
	___die (!elf, "Failed to alloc elf.");
	elf_alloc = true;
	fd = open(filename, O_RDONLY);
	___die (fd == -1, "Failed to open file");
	filesize = get_filesize(fd);
	___die (filesize == -1, "Failed to get file size");
	elf->data_size = (uint32_t)filesize;
	elf->data = calloc(1, elf->data_size);
	___die (!elf->data, "Could not allocate.");
	elf_data_alloc = true;
	bytes_read = read(fd, elf->data, filesize);
	___die (!bytes_read, "No bytes read. Is file empty?");
	___die (bytes_read == -1, "Error reading file");
	validate_file();
}

void	read_blob(char *filename)
{
	int				fd;
	off_t			filesize;
	long int	bytes_read;
	fd = open(filename, O_RDONLY);
	___die (fd == -1, "Failed to open file");
	filesize = get_filesize(fd);
	___die ((int)filesize == -1, "Failed to get file size");
	inj = calloc(sizeof(injector), 1);
	___die (!inj, "Failed to create injector.");
	inj_alloc = true;
	inj->bin_size = (uint32_t)filesize;
	inj->bin = calloc(1, inj->bin_size);
	___die (!inj->bin, "Could not allocate injection portion.");
	inj_bin_alloc = true;
	bytes_read = read(fd, inj->bin, filesize);
	___die (!bytes_read, "No bytes read. Is file empty?");
	___die (bytes_read == -1, "Error reading file");
}
