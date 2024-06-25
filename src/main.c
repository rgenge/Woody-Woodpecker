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

void	inject(const char *woody, const char *buzz)
{
	read_blob(buzz);

	// Positions.
	Elf64_Ehdr* EE = (Elf64_Ehdr*)elf->data;
	Elf64_Ehdr* IE = (Elf64_Ehdr*)inj->data;
	Elf64_Phdr* EP = (Elf64_Phdr*)(elf->data + EE->e_phoff);
	Elf64_Phdr* IP = (Elf64_Phdr*)(inj->data + EE->e_phoff);
	Elf64_Phdr* IPX = 0;
	Elf64_Shdr* IS = (Elf64_Shdr*)(inj->data + EE->e_shoff);
	(void)EE; (void)IE; (void)EP; (void)IP; (void)IPX; (void)IS;

	// Inject blob.
	size_t inj_offset = 0;
	ft_memcpy((void*)IE + inj_offset, (void*)inj->bin, inj->bin_size);

	// Find IPX, Phdr responible for .text.
	size_t i = 0;
	while (i < elf->phnum)
	{
		if (IP[i].p_flags & PF_X && 
			(IE->e_entry >= IP[i].p_offset &&
			 IE->e_entry < IP[i].p_offset + IP[i].p_filesz))
		{
			IPX = &IP[i];
		}
		i++;
	}
	___die(!IPX, "Did not fina a `.text` section.");

	// Full clone.
//	size_t data_offset = inj->bin_size;
//	ft_memcpy(inj->data + data_offset, elf->data, elf->data_size);
//	return;

//	// Clean .text area (for clarity).
//	char *h = (void*)IE + IPX->p_offset;
//	for (size_t i = 0; i < IPX->p_filesz; i++)
//		*h++ = 0;	

//	// Hard ajust last jump
//	size_t org_offset = inj->bin_size;

//	int32_t* jump = (void*)inj->bin + (inj->bin_size - sizeof(int32_t));
//	*jump = 0x10;
//
//	// Translate e_entry portion (assuming there is padding room).
//	ft_memcpy((void*)IE + IPX->p_offset + org_offset, (void*)EE + IPX->p_offset, IPX->p_filesz);

//	// Adjust parameters.
//	IPX->p_filesz += inj->bin_size;
//	IPX->p_memsz += inj->bin_size;

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
