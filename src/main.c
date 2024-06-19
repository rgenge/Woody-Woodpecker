#include "woody.h"

dumpster		*elf;
injector		inj;
void*				memo;
void*				zero;
size_t			size;
dumpster*		elf_alloc;

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

void	elf_init()
{
	elf->bit_class =
		elf->data[EI_CLASS] == ELFCLASS64 ? 64
		: elf->data[EI_CLASS] == ELFCLASS32 ? 32
		: 0;

	if (elf->bit_class == 64)
	{
		_E64 = (Elf64_Ehdr*)elf->data;
		_P64 = (Elf64_Phdr*)(elf->data + _E64->e_phoff);
		_S64 = (Elf64_Shdr*)(elf->data + _E64->e_shoff);
		elf->phnum = _E64->e_phnum == PN_XNUM ?
			_S64->sh_info : _E64->e_phnum;
		elf->shnum = _E64->e_shnum == 0 ?
			_S64[0].sh_size : _E64->e_shnum;
		elf->shstrndx = _E64->e_shstrndx == SHN_XINDEX ?
			_S64[0].sh_link : _E64->e_shstrndx;
	}
}

void  read_original_elf(char *filename)
{
	int				fd;
	int				filesize;
	long int	bytes_read;

	fd = open(filename, O_RDONLY);
	___die (fd == -1, "Failed to open file");
	filesize = get_filesize(fd);
	___die (filesize == -1, "Failed to get file size");
	elf->data_size = (uint32_t)filesize;
	elf->data = malloc(elf->data_size);
	___die (!elf->data, "Could not allocate.");
	bytes_read = read(fd, elf->data, filesize);
	___die (!bytes_read, "No bytes read. Is file empty?");
	___die (bytes_read == -1, "Error reading file");
}

void	read_blob(char *filename)
{
	int				fd;
	int				filesize;
	long int	bytes_read;

	fd = open(filename, O_RDONLY);
	___die (fd == -1, "Failed to open file");
	filesize = get_filesize(fd);
	___die (filesize == -1, "Failed to get file size");
	inj.bin_size = filesize;
	inj.bin = malloc(inj.bin_size);
	___die (!inj.bin, "Could not allocate.");
	bytes_read = read(fd, inj.bin, filesize);
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
	memo = calloc(elf->data_size, 1);
	___die(!memo, "Failed to prepare alloc block.");
	zero = elf->data;
	size = elf->data_size;

	if (elf->bit_class == 64)
	{
		_E64 = (Elf64_Ehdr*)elf->data;
		_P64 = (Elf64_Phdr*)(elf->data + _E64->e_phoff);
		_S64 = (Elf64_Shdr*)(elf->data + _E64->e_shoff);
		M (0, _E64->e_ehsize);
		M (_E64->e_phoff, _E64->e_phentsize * elf->phnum);
		for (size_t i = 0; i < elf->phnum; i++)
		{
			M (		_P64[i].p_offset,
						_P64[i].p_filesz		); 
		}
		M (_E64->e_shoff, _E64->e_shentsize * elf->shnum);
		for (size_t i = 0; i < elf->shnum; i++) // unused 0
		{
			M (		_S64[i].sh_offset,
						_S64[i].sh_size			); 
		}
	}

	int		fd;
	fd = open(woody, O_WRONLY | O_CREAT, 00755);
	___die (fd == -1, "Failed to create `woody`. File exists?");
	___die (write(fd, memo, size) == -1, "Could not write to file.");
	free(memo);
	close(fd);
}

/*
 * For now, will inject by insertion.
 * Only for 64bit little endian.
 * Without criptography.
 */
void	inject()
{
	void*	ih;
	void* eh;
	ft_memcpy(&inj.troll, &elf, sizeof(elf));
	read_blob("WOODY_blob_64.bin");
	size_t extra_size = inj.bin_size;
	inj.troll.data = calloc(elf->data_size + extra_size, 1);
	ih = inj.troll.data;
	eh = elf->data;

	size_t txt_p_align;
	for (size_t i = 0; i < elf->phnum; i++)
	{
		if (_P64[i].p_type == PT_LOAD)
		{
			txt_p_align = _P64[i].p_align;
		}
		if (_P64[i].p_paddr == _E64->e_entry)
		{
			_P64[i].p_filesz += inj.bin_size;
			_P64[i].p_memsz += inj.bin_size;
		}
	}

	for (size_t i = 0; i < elf->shnum; i++)
	{
		if (_S64[i].sh_offset == _E64->e_entry)
		{
			_S64[i].sh_size += inj.bin_size;
		}
	}

	ft_memcpy(ih, eh, elf->data_size);

	ft_memcpy(
		ih + _E64->e_entry + extra_size,
		eh + _E64->e_entry,
		txt_p_align - extra_size);
	ft_memcpy(
		ih + _E64->e_entry,
		inj.bin,
		inj.bin_size);

	free(inj.bin);
	free(elf->data);
	elf->data = inj.troll.data;
}

int		main(int argc, char **argv)
{
	___die(argc != 2, "Usage: `woody_woodpacker binary_file`");
	elf = calloc(sizeof(dumpster), 1);
	read_original_elf(argv[1]);
	validate_file();
	elf_init();
	inject();
	write_file("woody");
	free(elf->data);
	free(elf);
	return (0);
}

