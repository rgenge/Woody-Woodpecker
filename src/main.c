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
	read_blob(buzz_filename);

	// Full clone.
	ft_memcpy(inj->data, elf->data, elf->data_size);

	// Positions.
	Elf64_Ehdr* EE = (Elf64_Ehdr*)elf->data;
	Elf64_Ehdr* IE = (Elf64_Ehdr*)inj->data;
	Elf64_Phdr* EP = (Elf64_Phdr*)(elf->data + EE->e_phoff);
	Elf64_Phdr* IP = (Elf64_Phdr*)(inj->data + EE->e_phoff);
	Elf64_Phdr* IPX = 0;
	Elf64_Shdr* IS = (Elf64_Shdr*)(inj->data + EE->e_shoff);
	int32_t original_entry;
	int32_t original_filesz;
	(void)EE; (void)IE; (void)EP; (void)IP; (void)IPX; (void)IS;
	(void)original_entry; (void)original_filesz;

	// Find IPX, the exec Load Phdr responsible for .text.
	size_t i = 0;
	while (i < elf->phnum)
	{
		if (IP[i].p_flags & PF_X && 
			(IE->e_entry >= IP[i].p_offset &&
			 IE->e_entry < IP[i].p_offset + IP[i].p_filesz))
			IPX = &IP[i];
		i++;
	}
	___die(!IPX, "Did not find a `.text` section.");

	// Adjust parameters: augment load area.

	original_entry = EE->e_entry;
	original_filesz = IPX->p_filesz;
	IE->e_entry += original_filesz;
	IPX->p_filesz += inj->bin_size;
	IPX->p_memsz += inj->bin_size;
	IE->e_shoff += inj->bin_size;

	*(int32_t*)(inj->bin + inj->bin_size - sizeof(int32_t)) = 
		-IPX->p_filesz; 
//	- (int32_t)(inj->bin_size + original_filesz);
//		IE->e_entry;
//		((int)inj->bin_size * -1 - (int)original_filesz);
//		(int)(IE->e_entry - original_entry) * -1;

	// Inject.
	ft_memcpy((void*)IE + IE->e_entry, (void*)inj->bin, inj->bin_size);

	// Copy append original data block.
//	size_t org_offset = inj->bin_size;
//	ft_memcpy((void*)IE + org_offset, (void*)EE, elf->data_size);

//	// Clean .text area (for clarity).
//	char *h = (void*)IE + IPX->p_offset;
//	for (size_t i = 0; i < IPX->p_filesz; i++)
//		*h++ = 0;	

//	// Hard ajust last jump
//	int32_t* jump = (void*)inj->bin + (inj->bin_size - sizeof(int32_t));
//	*jump = 0x10;

//	i = 0;
//	while (i < elf->shnum)
//	{
//		if (IS[i].sh_offset == IE->e_entry)
//		{
//			IS[i].sh_size += inj->bin_size;
//		}
//		else if (IS[i].sh_offset > IE->e_entry)
//		{
//			IS[i].sh_offset += inj->bin_size;
//		}
//		i++;
//	}

	file_out_to_file(woody, inj->data, inj->data_size);
}

int		main(int argc, char **argv)
{
	___die(argc != 2, "Usage: `woody_woodpacker binary_file`");
	elf_init(argv[1], &elf);
	inject("woody", "WOODY_blob.bin");
	return free_all();
}
