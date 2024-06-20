#include "woody.h"

dumpster		*elf;
injector		*inj;
void*				file_out;
bool				elf_alloc = false;
bool				elf_data_alloc = false;
bool				inj_alloc = false;
bool				inj_data_alloc = false;
bool				inj_bin_alloc = false;
bool				file_out_alloc = false;

void	elf_init(char *vict)
{
	read_original_elf(vict);
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

void	write_file(const char *woody)
{
	file_out = calloc(elf->data_size, 1);
	___die(!file_out, "Failed to prepare alloc block.");
	file_out_alloc = true;
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
	file_out_to_file(woody);
}

void	inject()
{
//	void*	ih;
//	void* eh;
	uint64_t old_offset = 0;
	read_blob("WOODY_blob_64.bin");
	inj->data = calloc(elf->data_size + inj->bin_size, 1);
	___die(!inj->data, "Failed to alloc injection memory clone.");
	inj_data_alloc = true;

	for (size_t i = 0; i < elf->phnum; i++)
	{
		if (_P64[i].p_paddr == _E64->e_entry)
		{
//			_P64[i].p_filesz += inj->bin_size;
//			_P64[i].p_memsz += inj->bin_size;
//			old_offset = _P64[i].p_offset;
		}
		if (old_offset && _P64[i].p_offset > old_offset)
		{
//			_P64[i].p_offset += inj->bin_size;
//			_P64[i].p_paddr += inj->bin_size;
//			_P64[i].p_vaddr += inj->bin_size;
		}
	}

//	for (size_t i = 0; i < elf->shnum; i++)
//	{
//		if (_S64[i].sh_addr == _E64->e_entry)
//			_S64[i].sh_size += inj->bin_size;
//		if (_S64[i].sh_offset > old_offset)
//		{
//			_S64[i].sh_offset += inj->bin_size;
////			_S64[i].sh_addr += inj->bin_size;
//		}
//	}

//	ih = inj->troll.data;
//	eh = elf->data;
//	ft_memcpy(ih, eh, old_offset);
//	ih += old_offset;
//	ft_memcpy(ih, inj->bin, inj->bin_size);
//	ih += inj->bin_size;
//	eh += old_offset;
//	ft_memcpy(ih, eh, elf->data_size - old_offset);

}

int		main(int argc, char **argv)
{
	___die(argc != 2, "Usage: `woody_woodpacker binary_file`");
	elf_init(argv[1]);
	inject();
	write_file("woody");
	return free_all();
}
