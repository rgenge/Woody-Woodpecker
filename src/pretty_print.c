#include "woody.h"

void	byte_is(void* h, long long test, const char* msg)
{	if (*(unsigned char*)h == test) printf("%s", msg); }

void	true_is(long long a, long long b, const char* msg)
{	if (a == b) printf("%s", msg); }

void	flag_is(long long a, long long b, const char* msg)
{	if (a & b) printf("%s", msg); }

void	hex_pure(void* h, size_t amount)
{
	if (!h)
		return (void)printf("Dumping address nil, no dump.");
	for (size_t i = 0; i < amount; i += 8)
	{
		h = (char*)(h + i);
		for (size_t o = 0; o < 8 && o < amount; o++) // offset
			printf("%02x ", *((char*)h + o) & 0xFF);
	}
}

void	hex_msg(void* h, size_t amount, const char* msg)
{
	hex_pure(h, amount);
	printf("%s", msg);
}

void	pretty_print(t_elf *e)
{
	if (e->bit_class == 32)
		pretty_print32(e);
	else if (e->bit_class == 64)
		pretty_print64(e);
}

void	pretty_print64(t_elf* ex)
{
	___deb printf("pretty_print64() not implemented.");
	(void) ex;
}

void	pretty_print32(t_elf* ex)
{
	Elf32_Ehdr		*e;
	Elf32_Phdr		*p;
	unsigned char	*h;
	uint16_t		u16;
	uint32_t		u32;

	// ELF Header
	e = (Elf32_Ehdr*)ex->ehdr;

	printf("[ Elf32_Ehdr  %p\n|\\ e_ident    [+%02d]\n", e, 0);
	h = (unsigned char *)e;

	printf("|| magic    ");
	hex_byte(h, 4);
	printf(" <- Must be the same as");
	___br;
	___deb printf("||      \\__ 7f 45 4c 46 .ELF <- this to validate.\n");

	printf("|| class    ");
	h += 4;
	hex_pure(h, 1);
	byte_is(h, ELFCLASSNONE, "None/invalid architecture.");
	byte_is(h, ELFCLASS32, "32-bit architecture.");
	byte_is(h, ELFCLASS64, "64-bit architecture.");
	___br;

	printf("|| data enc ");
	hex_pure(++h, 1);
	byte_is(h, ELFDATANONE, "Unknown data format.");
	byte_is(h, ELFDATA2LSB, "2-complement, litte-endian.");
	byte_is(h, ELFDATA2MSB, "2-complement, big-endian.");
	___br;

	printf("|| elf ver  ");
	if (++h == EV_NONE)
		hex_msg(h, EV_CURRENT, "Ivalid version.");
	else
		hex_msg(h, EV_CURRENT, "<- Represents current version (hex).");
	___br;

	printf("|| OS ABI   ");
	hex_pure(++h, 1);
	byte_is(h, ELFOSABI_NONE | ELFOSABI_SYSV, "UNIX System VABI.");
	byte_is(h, ELFOSABI_HPUX, "HP-UX.");
	byte_is(h, ELFOSABI_NETBSD, "NetBSD.");
	byte_is(h, ELFOSABI_LINUX, "Linux.");
	byte_is(h, ELFOSABI_SOLARIS, "Solaris.");
	byte_is(h, ELFOSABI_IRIX, "IRIX.");
	byte_is(h, ELFOSABI_FREEBSD, "FreeBSD.");
	byte_is(h, ELFOSABI_TRU64, "TRU64 UNIX.");
	byte_is(h, ELFOSABI_ARM, "ARM architecture ABI.");
	byte_is(h, ELFOSABI_STANDALONE, "Stand-alone (embedded).");
	___br;

	printf("|| abi ver  ");
	hex_msg(++h, 1, "<- Represents target object version.");
	___br;

	h++;
	printf("|\\___________ (+%02ld) padding from here.\n",
		h - e->e_ident);
	printf("|                   ");
	hex_byte(h,
		((unsigned char *)&e->e_type - e->e_ident)
		- (h - e->e_ident));
	___br;

	printf("\\ e_type      (+%ld) ",
		(unsigned char *)&e->e_type - e->e_ident);
	u16 = e->e_type;
	hex_pure(&u16, sizeof(e->e_type));
	true_is(u16, ET_NONE, "Unkown type.");
	true_is(u16, ET_REL, "A relocatable file.");
	true_is(u16, ET_EXEC, "An executable file.");
	true_is(u16, ET_DYN, "A shared object.");
	true_is(u16, ET_CORE, "A core file.");
	___br;

	printf("\\ e_machine   (+%ld) ", (void *)&e->e_machine - (void*)e);
	u16 = e->e_machine;
	hex_pure(&u16, sizeof(e->e_type));
	true_is(u16, EM_NONE, "Unkown machine.");
	true_is(u16, EM_M32, "AT&T WE 32100.");
	true_is(u16, EM_SPARC, "Sun Microsystems SPARC.");
	true_is(u16, EM_386, "Intel 80386.");
	true_is(u16, EM_68K, "Motorola 68000.");
	true_is(u16, EM_88K, "Motorola 88000.");
	true_is(u16, EM_860, "Intel 80860.");
	true_is(u16, EM_MIPS, "MIPS RS3000 big-endian.");
	true_is(u16, EM_PARISC, "HP/PA.");
	true_is(u16, EM_SPARC32PLUS, "SPARC enhanced.");
	true_is(u16, EM_PPC, "PowerPC 32-bit.");
	true_is(u16, EM_PPC64, "PowerPC 64-bit.");
	true_is(u16, EM_S390, "IBM S/390.");
	true_is(u16, EM_ARM, "Advanced RISC Machines.");
	true_is(u16, EM_SH, "Renesas SuperH.");
	true_is(u16, EM_SPARCV9, "SPARC v9 64-bit.");
	true_is(u16, EM_IA_64, "Intel Itanium.");
	true_is(u16, EM_X86_64, "AMD x86-64.");
	true_is(u16, EM_VAX, "DEC Vax.");
	___br;

	printf("\\ e_version   (+%ld) ", (void*)&e->e_version - (void*)e);
	u32 = e->e_version;
	if (u32 == EV_NONE) 
		hex_msg((void*)&u32, 4, " Invalid version.");
	else
	{
		hex_pure((void*)&u32, 4);
		printf("= %d <- File version.", u32);
	}
	___br;

	printf("\\ e_entry     (+%ld) ", (void*)&e->e_entry - (void*)e);
	if (e->e_entry)
		hex_msg((void*)&e->e_entry, sizeof(e->e_entry),
			"Entry point address.");
	else
		hex_msg((void*)&e->e_entry, sizeof(e->e_entry),
			"No associated entry point.");
	___br;

	printf("\\ e_phoff     (+%ld) ", (void*)&e->e_phoff - (void*)e);
	if (e->e_phoff)
	{
		hex_msg((void*)&e->e_phoff, sizeof(e->e_phoff),
			"PH offset: ");
		printf("%d B", e->e_phoff);
	}
	else
	{
		hex_msg((void*)&e->e_phoff, sizeof(e->e_phoff),
			"PH zero, no header table");
	}
	___br;

	printf("\\ e_shoff     (+%ld) ", (void*)&e->e_shoff - (void*)e);
	hex_msg((void*)&e->e_shoff, sizeof(e->e_shoff),
		"SH offset: ");
	if (e->e_shoff)
		printf("%d B", e->e_shoff);
	else
		printf("(zero:) no section header.");
	___br;

	printf("\\ e_flags     (+%ld) ", (void*)&e->e_flags - (void*)e);
	hex_msg((void*)&e->e_flags, sizeof(e->e_flags),
		"CPU-specific flags.");
	___br;

	printf("\\ e_ehsize    (+%ld) ", (void*)&e->e_ehsize - (void*)e);
	hex_msg((void*)&e->e_ehsize, sizeof(e->e_ehsize),
		"ELF Header size: ");
	printf("%d B", e->e_ehsize);
	___br;

	printf("\\ e_phentsize (+%ld) ", (void*)&e->e_phentsize - (void*)e);
	hex_msg((void*)&e->e_phentsize, sizeof(e->e_phentsize),
		"PH entry size: ");
	printf("%d B", e->e_phentsize);
	___br;

	printf("\\ e_phnum     (+%ld) ", (void*)&e->e_phnum - (void*)e);
	hex_msg((void*)&e->e_phnum, sizeof(e->e_phnum),
		"PH entries: ");
	if (e->e_phnum != PN_XNUM) // 0xffff
		printf("%d", e->e_phnum);
	else
		printf("see sh_info.");
	___br;

	printf("\\ e_shentsize (+%ld) ", (void*)&e->e_shentsize - (void*)e);
	hex_msg((void*)&e->e_shentsize, sizeof(e->e_shentsize),
		"SH entry size: ");
	printf("%d B", e->e_shentsize);
	___br;

	printf("\\ e_shnum     (+%ld) ", (void*)&e->e_shnum - (void*)e);
	hex_msg((void*)&e->e_shnum, sizeof(e->e_shnum),
		"SH entries: ");
	if (e->e_shnum < SHN_LORESERVE) // 0xff00
		printf("%d", e->e_shnum);
	else
		printf("see SH[0]->sh_size.");
	___br;

	printf("\\ e_shstrndx  (+%ld) ", (void*)&e->e_shstrndx - (void*)e);
	hex_msg((void*)&e->e_shstrndx, sizeof(e->e_shstrndx),
		"SH string table index: ");
	if (e->e_shnum < SHN_LORESERVE) // 0xff00
		printf("%d", e->e_shstrndx);
	else
		printf("see sh_link.");
	___br;

	printf("] -------------------------------------------------/\n");

	// Program Header	
	p = (Elf32_Phdr*)ex->phdr;
	size_t pi = -1;

	printf("[ Elf32_Phdr     %p\n", p);

	while (++pi < e->e_phnum)
	{
		printf("|/-------- [+%02ld] %02ld/%02d --- phdr sec. --------------\\\n", (void*)&p[pi] - (void*)e, pi, e->e_phnum) ;
		printf("\\ p_type   (+%02ld) ", (void*)&p[pi] - (void*)e);
		hex_pure(&p[pi].p_type, sizeof(p[pi].p_type));
		byte_is(&p[pi].p_type, PT_NULL, "Null: ignore.");
		byte_is(&p[pi].p_type, PT_LOAD, "Load.");
		byte_is(&p[pi].p_type, PT_DYNAMIC, "Dynamic link info.");
		byte_is(&p[pi].p_type, PT_INTERP, "Loc+size of interpreter.");
		byte_is(&p[pi].p_type, PT_NOTE, "Nhdr location.");
		byte_is(&p[pi].p_type, PT_SHLIB, "Reserved/unused/non-ABI.");
		byte_is(&p[pi].p_type, PT_PHDR, "Phdr loc+size.");
		byte_is(&p[pi].p_type, PT_LOPROC | PT_HIPROC, "Reserved CPU-specific.");
		byte_is(&p[pi].p_type, PT_GNU_STACK, "Kernel-controled, state of stack.");
		___br;

		printf("\\ p_offset (+%02ld) ", (void*)&p[pi].p_offset - (void*)e);
		hex_msg(&p[pi].p_offset, sizeof(p[pi].p_offset),
			"Section offset: ");
		printf("%d B", p[pi].p_offset);
		___br;

		printf("\\ p_vaddr  (+%02ld) ", (void*)&p[pi].p_vaddr - (void*)p);
		hex_msg(&p[pi].p_vaddr, sizeof(p[pi].p_vaddr),
			"Segment v. address. ");
		___br;

		printf("\\ p_paddr  (+%02ld) ", (void*)&p[pi].p_paddr - (void*)p);
		hex_msg(&p[pi].p_paddr, sizeof(p[pi].p_paddr),
			"Seg physical addr.");
		___br;

		printf("\\ p_filesz (+%02ld) ", (void*)&p[pi].p_filesz - (void*)p);
		hex_msg(&p[pi].p_filesz, sizeof(p[pi].p_filesz),
			"Seg file img: ");
		printf("%d B", p[pi].p_filesz);
		___br;

		printf("\\ p_memsz  (+%02ld) ", (void*)&p[pi].p_memsz - (void*)p);
		hex_msg(&p[pi].p_memsz, sizeof(p[pi].p_memsz),
			"Seg mem size: ");
		printf("%d B", p[pi].p_memsz);
		___br;

		printf("\\ p_flags  (+%02ld) ", (void*)&p[pi].p_flags - (void*)p);
		hex_pure(&p[pi].p_flags, sizeof(p[pi].p_flags));
		flag_is(p[pi].p_flags, PF_R, "+r");
		flag_is(p[pi].p_flags, PF_W, "+w");
		flag_is(p[pi].p_flags, PF_X, "+x");
		___br;

		printf("\\ p_align  (+%02ld) ", (void*)&p[pi].p_align - (void*)p);
		hex_msg(&p[pi].p_align, sizeof(p[pi].p_align),
			"Seg mem align: ");
		printf("%d", p[pi].p_align);
		___br;
	}



	___br;
	return ;
}
