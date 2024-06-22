#include "woody.h"

dumpster		*elf;
injector		*inj;
bool				elf_alloc = false;
bool				elf_data_alloc = false;
bool				inj_alloc = false;
bool				inj_data_alloc = false;
bool				inj_bin_alloc = false;

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

void	inject(const char *woody, const char *buzz)
{
	read_blob(buzz);
	_E64 = (Elf64_Ehdr*)elf->data;
	_P64 = (Elf64_Phdr*)(elf->data + _E64->e_phoff);
	_S64 = (Elf64_Shdr*)(elf->data + _E64->e_shoff);
	M (0, _E64->e_ehsize);
	M (_E64->e_phoff, _E64->e_phentsize * elf->phnum);
	for (size_t i = 0; i < elf->phnum; i++)
	{
		if (_P64[i].p_offset > _E64->e_entry + inj->bin_size
		|| _P64[i].p_offset + _P64[i].p_filesz < _E64->e_entry)
		{
			M (		_P64[i].p_offset,
						_P64[i].p_filesz		); 
		}
		else
		{
			uint32_t asz = _E64->e_entry - _P64[i].p_offset;
			uint32_t bsz = _P64[i].p_filesz - _E64->e_entry;
			M (_P64[i].p_offset, asz);
			___die(!ft_memcpy(inj->data + _E64->e_entry,
												inj->bin, inj->bin_size),
							"Unable to inject.");
			___die(!ft_memcpy(inj->data + _E64->e_entry +
												inj->bin_size, _E64 +
												_P64[i].p_offset + inj->bin_size,
												bsz),
							"Could not copy right side.");
		}
	}

//	M (_E64->e_shoff, _E64->e_shentsize * elf->shnum);
//	for (size_t i = 0; i < elf->shnum; i++)
//	{
//		M (		_S64[i].sh_offset,
//					_S64[i].sh_size			); 
//	}

	file_out_to_file(woody, inj->data, inj->data_size);
}

int		main(int argc, char **argv)
{
	___die(argc != 2, "Usage: `woody_woodpacker binary_file`");
	elf_init(argv[1]);
	inject("woody", "WOODY_blob.bin");
	return free_all();
}
