#include "woody.h"

elf_t				*elf;
injector		*inj;
bool				elf_alloc = false;
bool				elf_data_alloc = false;
bool				inj_alloc = false;
bool				inj_data_alloc = false;
bool				inj_bin_alloc = false;

void	elf_init(char *vict, elf_t** elf_ptr)
{
	read_original(vict, &(*elf_ptr));
	(*elf_ptr)->bit_class =
		(*elf_ptr)->data[EI_CLASS] == ELFCLASS64 ? 64
		: (*elf_ptr)->data[EI_CLASS] == ELFCLASS32 ? 32
		: 0;
	if ((*elf_ptr)->bit_class == 64)
	{
		_E64 = (Elf64_Ehdr*)(*elf_ptr)->data;
		_P64 = (Elf64_Phdr*)((*elf_ptr)->data + _E64->e_phoff);
		_S64 = (Elf64_Shdr*)((*elf_ptr)->data + _E64->e_shoff);
		(*elf_ptr)->phnum = _E64->e_phnum == PN_XNUM ?
			_S64->sh_info : _E64->e_phnum;
		(*elf_ptr)->shnum = _E64->e_shnum == 0 ?
			_S64[0].sh_size : _E64->e_shnum;
		(*elf_ptr)->shstrndx = _E64->e_shstrndx == SHN_XINDEX ?
			_S64[0].sh_link : _E64->e_shstrndx;
	}
}

void	inject(const char *woody, const char *buzz_filename)
{
	size_t	i;
	int32_t	original_filesz;
	char		*h;
	char		*s;

	read_blob(buzz_filename);

	// Full clone.
	ft_memcpy(inj->data, elf->data, elf->data_size);

	// Positions.
	Elf64_Ehdr* EE = (Elf64_Ehdr*)elf->data;
	Elf64_Ehdr* IE = (Elf64_Ehdr*)inj->data;
	Elf64_Phdr* IP = (Elf64_Phdr*)(inj->data + EE->e_phoff);
	Elf64_Shdr* IS = (Elf64_Shdr*)(inj->data + EE->e_shoff);
	Elf64_Phdr* IPX = 0;
	Elf64_Shdr* ISX = 0;
	Elf64_Addr original_entry;

	// Find IPX, the exec Load Phdr responsible for .text.
	i = 0;
	while (i < elf->phnum)
	{
		if (IP[i].p_flags & PF_X && 
			(IE->e_entry >= IP[i].p_offset &&
			 IE->e_entry < IP[i].p_offset + IP[i].p_filesz))
			IPX = &IP[i];
		i++;
	}
	___die(!IPX, "Did not find a `.text` section.");

	// Find string table.
	Elf64_Shdr* SHST = &IS[EE->e_shstrndx];
	Elf64_Off stoff = SHST->sh_offset;
	char* ST = (char*)(inj->data + stoff);

	// Find ISX, the exec Load Shdr responsible for .text.
	char* name;
	i = 0;
	while (i < elf->shnum)
	{
		name = (char*)(ST + IS[i].sh_name);
		if (ft_stridentical(name, ".text")
		&& IS[i].sh_type & SHT_PROGBITS)
			ISX = &IS[i];
		i++;
	}
	___die(!ISX, "Did not find a `.text` section.");

	// Adjust parameters.
	original_entry = IE->e_entry;
	original_filesz = IPX->p_filesz;
	IE->e_entry = IPX->p_vaddr + IPX->p_memsz;
	IPX->p_flags |= PF_W;
	IPX->p_filesz += inj->bin_size;
	IPX->p_memsz += inj->bin_size;
	ISX->sh_size += inj->bin_size;
	ISX->sh_flags |= SHF_WRITE;

	// Validate enough padding space.
	s = (char*)IE + IE->e_entry;
	h = s;
	while (!*h && h++ - s < inj->bin_size) ;
	___die(h - s != inj->bin_size + 1, "Not enough padding space for injection.");

	// The jump:
	int32_t *last_jump;
	last_jump = (int32_t*)(inj->bin + inj->bin_size - sizeof(int32_t));
	*last_jump = original_entry - IE->e_entry - inj->bin_size;

	// Generate key
	int n;
	char k_buffer[8];
	long int key;
	int fd = open("/dev/random", O_RDONLY);
	___die(fd == -1, "Could not access /dev/random.");
	n = read(fd, k_buffer, sizeof(k_buffer));
	___die(n != sizeof(k_buffer), "Key generation failed.");
	close(fd);
	key = *(long int*)k_buffer;
	printf("64-bit key: ==== %8lx ====\n", (long int)key);

	// Write key
	h = (char*)(inj->bin + inj->bin_size - 15);
	*(long int*)h = key;

	// Encript.
	h = (char*)IE + original_entry;
	s = (char*)IE + IE->e_entry;
	s -= 8;
	while (h < s)
	{
		*(long int*)h ^= key;
		h += 8;
	}
	if (__debug || FUN)
	{
		h = (char*)IE + original_entry;
		hex_dump(h, original_filesz);
	}

	// Inject.
	ft_memcpy((void*)IE + IE->e_entry, (void*)inj->bin, inj->bin_size);

	// Lastly.
	file_out_to_file(woody, inj->data, inj->data_size);
}

int		main(int argc, char **argv)
{
	___die(argc != 2, "Usage: `woody_woodpacker binary_file`");
	elf_init(argv[1], &elf);
	inject("woody", "buzz_buzzard.bin");
	return free_all();
}
