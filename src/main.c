#include "woody.h"

dumpster		*elf;
injector		*inj;
bool				elf_alloc = false;
bool				elf_data_alloc = false;
bool				inj_alloc = false;
bool				inj_data_alloc = false;
bool				inj_bin_alloc = false;

void	elf_init(char *vict, dumpster** xxx)
{
	read_original(vict, &(*xxx));
	(*xxx)->bit_class =
		(*xxx)->data[EI_CLASS] == ELFCLASS64 ? 64
		: (*xxx)->data[EI_CLASS] == ELFCLASS32 ? 32
		: 0;
	if ((*xxx)->bit_class == 64)
	{
		_E64 = (Elf64_Ehdr*)(*xxx)->data;
		_P64 = (Elf64_Phdr*)((*xxx)->data + _E64->e_phoff);
		_S64 = (Elf64_Shdr*)((*xxx)->data + _E64->e_shoff);
		(*xxx)->phnum = _E64->e_phnum == PN_XNUM ?
			_S64->sh_info : _E64->e_phnum;
		(*xxx)->shnum = _E64->e_shnum == 0 ?
			_S64[0].sh_size : _E64->e_shnum;
		(*xxx)->shstrndx = _E64->e_shstrndx == SHN_XINDEX ?
			_S64[0].sh_link : _E64->e_shstrndx;
	}
}

void	inject(const char *woody, const char *buzz_filename)
{
	size_t i;

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
	int32_t original_filesz;
	(void)EE; (void)IE; (void)IP; (void)IPX; (void)IS;
	(void)original_entry; (void)original_filesz;

	// Find IPX, the exec Load Phdr responsible for .text.
	i = 0;
	while (i < elf->phnum)
	{
		IP[i].p_flags |= PF_W;
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
		IS[i].sh_flags |= SHF_WRITE;
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
	IPX->p_filesz += inj->bin_size; // Not necessary.
	IPX->p_memsz += inj->bin_size;  // Not necessary.
	ISX->sh_size += inj->bin_size;  // ISX is unused, not necessary.
	ISX->sh_flags |= SHF_WRITE;
	// ^ not implementing these will hide the inject from objdump.

	// Validate enough padding space.
	char* s = (char*)IE + IE->e_entry;
	char* h = s;
	while (h++ - s < inj->bin_size) ;
	___die(h - s != inj->bin_size + 1, "Not enough padding space for injection.");

	// The jump:
	int32_t *last_jump;
	last_jump = (int32_t*)(inj->bin + inj->bin_size - sizeof(int32_t));
	*last_jump = original_entry - IE->e_entry - inj->bin_size;

	// Inject. (Not checking available padding space.)
	ft_memcpy((void*)IE + IE->e_entry, (void*)inj->bin, inj->bin_size);

	// Lastly.
	file_out_to_file(woody, inj->data, inj->data_size);

}

int		main(int argc, char **argv)
{
	___die(argc != 2, "Usage: `woody_woodpacker binary_file`");
	elf_init(argv[1], &elf);
	inject("woody", "material.bin");
	return free_all();
}
