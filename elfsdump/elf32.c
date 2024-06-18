#include "elfsdump.h"

extern dumpster				elf;
extern unsigned char	*h;
extern uint16_t				u16;
extern uint32_t				u32;

void	pretty_print32()
{
	Elf32_Ehdr *e = elf.ehdr._32; // for alias only.
	Elf32_Phdr *p = elf.phdr._32;
	Elf32_Shdr *s = elf.shdr._32;

	// ELF Header
	printf("|==================================================|\n");
	printf("[ Elf32_Ehdr   %p\n", e);
	printf("|--------------------------------------------------|\n");

	printf("|\\ e_ident    [%02d]\n", 0);

	printf("|| EI_MAG       : ");
	h = (unsigned char*)e;
	hex_byte(h, 4);
	printf("||       \\_______ 7f 45 4c 46 .ELF <- check.\n");

	printf("|| EI_CLASS     : ");
	h = (unsigned char*)e + EI_CLASS;
	hex_pure(h, 1);
	byte_is(h, ELFCLASSNONE, "None/invalid architecture.");
	byte_is(h, ELFCLASS32, "32-bit architecture.");
	byte_is(h, ELFCLASS64, "64-bit architecture.");
	___br;

	printf("|| EI_DATA      : ");
	h = (unsigned char*)e + EI_DATA;
	hex_pure(h, 1);
	byte_is(h, ELFDATANONE, "Unknown data format.");
	byte_is(h, ELFDATA2LSB, "2-complement, litte-endian.");
	byte_is(h, ELFDATA2MSB, "2-complement, big-endian.");
	___br;

	printf("|| EI_VERSION   : ");
	h = (unsigned char*)e + EI_VERSION;
	if (h == EV_NONE)
		hex_msg(h, EV_CURRENT, "Ivalid version.");
	else
		hex_msg(h, EV_CURRENT, "<- Current version (hex).");
	___br;

	printf("|| EI_OSABI     : ");
	h = (unsigned char*)e + EI_OSABI;
	hex_pure(h, 1);
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

	printf("|| EI_ABIVERSION: ");
	h = (unsigned char*)e + EI_ABIVERSION;
	hex_msg(h, 1, "<- (Hex) target version.");
	___br;

	printf("|\\__padding__ (%02ld) ", sizeof(e));
	h = (unsigned char*)e + EI_PAD;
	hex_byte(h, (void*)&e->e_type - (void*)h);

	// End e_ident

	printf("\\ e_type      (%ld) ", (void*)&e->e_type - (void*)e);
	u16 = e->e_type;
	hex_pure(&u16, sizeof(u16));
	true_is(u16, ET_NONE, "Unkown type.");
	true_is(u16, ET_REL, "A relocatable file.");
	true_is(u16, ET_EXEC, "An executable file.");
	true_is(u16, ET_DYN, "A shared object.");
	true_is(u16, ET_CORE, "A core file.");
	___br;

	printf("\\ e_machine   (%ld) ", (void *)&e->e_machine - (void*)e);
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

	printf("\\ e_version   (%ld) ", (void*)&e->e_version - (void*)e);
	u32 = e->e_version;
	if (u32 == EV_NONE) 
		hex_msg((void*)&u32, 4, " Invalid version.");
	else
	{
		hex_pure((void*)&u32, 4);
		printf("File version: %d.", u32);
	}
	___br;

	printf("\\ e_entry     (%ld) ", (void*)&e->e_entry - (void*)e);
	if (e->e_entry)
	{
		hex_pure((void*)&e->e_entry, sizeof(e->e_entry));
		printf("Entry point address: %d", e->e_entry);
	}
	else
		hex_msg((void*)&e->e_entry, sizeof(e->e_entry),
			"No associated entry point.");
	___br;

	printf("\\ e_phoff     (%ld) ", (void*)&e->e_phoff - (void*)e);
	if (e->e_phoff)
	{
		hex_msg((void*)&e->e_phoff, sizeof(e->e_phoff),
			"PH offset: ");
		printf("%d B", e->e_phoff);
	}
	else
	{
		hex_msg((void*)&e->e_phoff, sizeof(e->e_phoff),
			"PH zero, no prog. table");
	}
	___br;

	printf("\\ e_shoff     (%ld) ", (void*)&e->e_shoff - (void*)e);
	hex_msg((void*)&e->e_shoff, sizeof(e->e_shoff),
		"SH offset: ");
	if (e->e_shoff)
		printf("%d B", e->e_shoff);
	else
		printf("(zero:) no section header.");
	___br;

	printf("\\ e_flags     (%ld) ", (void*)&e->e_flags - (void*)e);
	hex_msg((void*)&e->e_flags, sizeof(e->e_flags),
		"CPU-specific flags.");
	___br;

	printf("\\ e_ehsize    (%ld) ", (void*)&e->e_ehsize - (void*)e);
	hex_msg((void*)&e->e_ehsize, sizeof(e->e_ehsize),
		"ELF Header size: ");
	printf("%d B", e->e_ehsize);
	___br;

	printf("\\ e_phentsize (%ld) ", (void*)&e->e_phentsize - (void*)e);
	hex_msg((void*)&e->e_phentsize, sizeof(e->e_phentsize),
		"PH entry size: ");
	printf("%d B", e->e_phentsize);
	___br;

	printf("\\ e_phnum     (%ld) ", (void*)&e->e_phnum - (void*)e);
	hex_msg((void*)&e->e_phnum, sizeof(e->e_phnum),
		"PH entries: ");
	if (e->e_phnum != PN_XNUM) // 0xffff
		printf("%d", e->e_phnum);
	else
		printf("see SH[0]->sh_info.");
	___br;

	printf("\\ e_shentsize (%ld) ", (void*)&e->e_shentsize - (void*)e);
	hex_msg((void*)&e->e_shentsize, sizeof(e->e_shentsize),
		"SH entry size: ");
	printf("%d B", e->e_shentsize);
	___br;

	printf("\\ e_shnum     (%ld) ", (void*)&e->e_shnum - (void*)e);
	hex_msg((void*)&e->e_shnum, sizeof(e->e_shnum),
		"SH entries: ");
	if (e->e_shnum < SHN_LORESERVE) // 0xff00
		printf("%d", e->e_shnum);
	else
		printf("see SH[0]->sh_size.");
	___br;

	printf("\\ e_shstrndx  (%ld) ", (void*)&e->e_shstrndx - (void*)e);
	hex_msg((void*)&e->e_shstrndx, sizeof(e->e_shstrndx),
		"SH string table index: ");
	if (e->e_shnum < SHN_LORESERVE) // 0xff00
		printf("%d", e->e_shstrndx);
	else
		printf("see SH[0]->sh_link.");
	___br;

	printf("] -------------------------------------------------/\n");

	// Program Header	

	printf("[ Elf32_Phdr     %p\n", p);
	printf("|--------------------------------------------------|\n");

	uint32_t pi = -1; // same type for 32 and 64-bit.

	while (++pi < elf.phnum)
	{
		printf("/--------- [%03ld] %02d/%02d phdr segment ---------------\\\n",
			(void*)&p[pi] - (void*)e, pi, e->e_phnum);
		printf("\\ p_type   (%03ld) ", (void*)&p[pi] - (void*)e);
		hex_pure(&p[pi].p_type, sizeof(p[pi].p_type));
		byte_is(&p[pi].p_type, PT_NULL, "Null: ignore.");
		byte_is(&p[pi].p_type, PT_LOAD, "Load.");
		byte_is(&p[pi].p_type, PT_DYNAMIC, "Dynamic link info.");
		byte_is(&p[pi].p_type, PT_INTERP, "Interpreter loc+size.");
		byte_is(&p[pi].p_type, PT_NOTE, "Nhdr location.");
		byte_is(&p[pi].p_type, PT_SHLIB, "Reserved/unused/non-ABI.");
		byte_is(&p[pi].p_type, PT_PHDR, "Phdr table loc+size.");
		if (p[pi].p_type >= PT_LOPROC && p[pi].p_type <= PT_HIPROC)
			printf("Reserved CPU-specific.");
		byte_is(&p[pi].p_type, PT_GNU_STACK, "GNU kernel-controled state.");
		___br;

		printf("\\ p_offset (%03ld) ", (void*)&p[pi].p_offset - (void*)e);
		hex_msg(&p[pi].p_offset, sizeof(p[pi].p_offset),
			"Section offset: ");
		printf("%d B", p[pi].p_offset);
		___br;

		printf("\\ p_vaddr  (%03ld) ", (void*)&p[pi].p_vaddr - (void*)e);
		hex_msg(&p[pi].p_vaddr, sizeof(p[pi].p_vaddr),
			"Seg virt address. ");
		___br;

		printf("\\ p_paddr  (%03ld) ", (void*)&p[pi].p_paddr - (void*)e);
		hex_msg(&p[pi].p_paddr, sizeof(p[pi].p_paddr),
			"Seg physical addr.");
		___br;

		printf("\\ p_filesz (%03ld) ", (void*)&p[pi].p_filesz - (void*)e);
		hex_msg(&p[pi].p_filesz, sizeof(p[pi].p_filesz),
			"Seg file img: ");
		printf("%d B", p[pi].p_filesz);
		___br;

		printf("\\ p_memsz  (%03ld) ", (void*)&p[pi].p_memsz - (void*)e);
		hex_msg(&p[pi].p_memsz, sizeof(p[pi].p_memsz),
			"Seg mem size: ");
		printf("%d B", p[pi].p_memsz);
		___br;

		printf("\\ p_flags  (%03ld) ",
			(void*)&p[pi].p_flags - (void*)e);
		hex_pure(&p[pi].p_flags, sizeof(p[pi].p_flags));
		flag_is(p[pi].p_flags, PF_R, "+r");
		flag_is(p[pi].p_flags, PF_W, "+w");
		flag_is(p[pi].p_flags, PF_X, "+x");
		___br;

		printf("\\ p_align  (%03ld) ", (void*)&p[pi].p_align - (void*)e);
		hex_msg(&p[pi].p_align, sizeof(p[pi].p_align),
			"Seg mem align: ");
		printf("%d", p[pi].p_align);
		___br;

		hex_byte((void*)e + p[pi].p_offset, p[pi].p_filesz);
	}
	printf("] -------------------------------------------------/\n");

	// Section Header	
	pi = -1;

	printf("[ Elf32_Shdr %p (each: %d B)\n", s, e->e_shentsize);
	printf("| -------------------------------------------------|\n");

	while (++pi < elf.shnum)
	{
		printf("\\= [%04ld] %02d/%02d =============",
				(void*)&s[pi] - (void*)e, pi, e->e_shnum);
		if (pi == 0)
			printf(":special data 0 ======\\\n");
		else if (pi == elf.shstrndx)
			printf(":name string table ===\\\n");
		else
			printf("======================\\\n");

		if (pi == 0)
		{
			if (___nonzero s[pi].sh_info)
			{
				printf("|--(%04ld) ", (void*)&s[pi].sh_info - (void*)e);
				hex_pure(&s[pi].sh_info, sizeof (s[pi].sh_info));
				printf("sh_info, PH entries: %d\n", s[pi].sh_info);
			}
			if (___nonzero s[pi].sh_size)
			{
				printf("|--(%04ld) ", (void*)&s[pi].sh_size - (void*)e);
				hex_pure(&s[pi].sh_size, sizeof (s[pi].sh_size));
				printf("sh_size, SH entries: %d\n", s[pi].sh_size);
			}
			if (___nonzero s[pi].sh_link)
			{
				printf("|--(%04ld) ", (void*)&s[pi].sh_link - (void*)e);
				hex_pure(&s[pi].sh_link, sizeof (s[pi].sh_link));
				printf(___spc "sh_link, name_string table: SH[%d]\n", s[pi].sh_link);
			}
			if (!s[pi].sh_info && !s[pi].sh_size && !s[pi].sh_link)
			{
				printf("|- empty/unused.\n");
			}
		}

		if (pi >= SHN_LORESERVE && pi <= SHN_HIRESERVE)
			printf("|- reserved.\n");
		if (pi == SHN_LORESERVE)
			printf("|- lower bound of reserved.\n");
		if (pi >= SHN_LOPROC && pi <= SHN_HIPROC)
			printf("|- CPU-specific.\n");
		if (pi == SHN_ABS)
			printf("|- absolute corresponding reference.\n");
		if (pi == SHN_COMMON)
			printf("|- common-relatives reference.\n");
		if (pi == SHN_HIRESERVE)
			printf("|- upper bound of reserved.\n");

		if (pi != 0
				&& (pi < SHN_LORESERVE || pi > SHN_HIRESERVE)
				&& pi != SHN_UNDEF
				&& (pi < SHN_LORESERVE || pi > SHN_HIRESERVE)
				&& pi != SHN_ABS
				&& pi != SHN_COMMON
				&& pi != SHN_HIRESERVE)
		{
			if (___nonzero s[pi].sh_name)
			{
				printf("|\\ name      (%03ld) ", (void*)&s[pi].sh_name - (void*)e);
				hex_msg(&s[pi].sh_name, sizeof(s[pi].sh_name),
						___spc "on str_table[");
				printf("%d]", s[pi].sh_name);
				___br;
			}

			if (___nonzero s[pi].sh_type)
			{
				printf("|\\ sh_type   (%03ld) ", (void*)&s[pi].sh_type - (void*)e);
				hex_pure(&s[pi].sh_type, sizeof(s[pi].sh_type));
				true_is(s[pi].sh_type, SHT_NULL,     "Undefined/unused.");
				true_is(s[pi].sh_type, SHT_PROGBITS, "Prog-defined sect.");
				true_is(s[pi].sh_type, SHT_SYMTAB,   "Link symbol table.");
				true_is(s[pi].sh_type, SHT_STRTAB,   "String table.");
				true_is(s[pi].sh_type, SHT_RELA,     "Reloc w/ addends.");
				true_is(s[pi].sh_type, SHT_HASH,     "Symbol hash table.");
				true_is(s[pi].sh_type, SHT_DYNAMIC,  "Dyn-linking infos.");
				true_is(s[pi].sh_type, SHT_NOTE,     "Notes.");
				true_is(s[pi].sh_type, SHT_NOBITS,   "No-bits mem.");
				true_is(s[pi].sh_type, SHT_REL,      "Reloc w/o addends.");
				true_is(s[pi].sh_type, SHT_SHLIB,    "Unespecified.");
				true_is(s[pi].sh_type, SHT_DYNSYM,   "Dynmic links.");
				if (s[pi].sh_type >= SHT_LOPROC && s[pi].sh_type <= SHT_HIUSER)
					printf("CPU-specific.");
				true_is(s[pi].sh_type, SHT_LOUSER,   "Low i for app.");
				true_is(s[pi].sh_type, SHT_HIUSER,   "High i for app.");
				___br;
			}

			if (___nonzero s[pi].sh_flags)
			{
				printf("|\\ sh_flags  (%03ld) ", (void*)&s[pi].sh_flags - (void*)e);
				hex_pure(&s[pi].sh_flags, sizeof(s[pi].sh_flags));
				condition_msg(s[pi].sh_flags & SHF_WRITE,     "\n|                 - Writable data.");
				condition_msg(s[pi].sh_flags & SHF_ALLOC,     "\n|                 - Alloc'd on exec.");
				condition_msg(s[pi].sh_flags & SHF_EXECINSTR, "\n|                 - Machine language.");
				condition_msg(s[pi].sh_flags & SHF_MASKPROC,  "\n|                 - CPU-specific.");
				___br;
			}

			if (___nonzero s[pi].sh_addr)
			{
				printf("|\\ sh_addr   (%03ld) ", (void*)&s[pi].sh_addr - (void*)e);
				hex_msg(&s[pi].sh_addr, sizeof(s[pi].sh_addr),
						"First byte addr.");
				___br;
			}

			if (___nonzero s[pi].sh_offset)
			{
				printf("|\\ sh_offset (%03ld) ", (void*)&s[pi].sh_offset - (void*)e);
				hex_msg(&s[pi].sh_offset, sizeof(s[pi].sh_offset),
						"Offset: ");
				printf("%d B", s[pi].sh_offset);
				___br;
			}

			if (___nonzero s[pi].sh_size)
			{
				printf("|\\ sh_size   (%03ld) ", (void*)&s[pi].sh_size - (void*)e);
				hex_msg(&s[pi].sh_size, sizeof(s[pi].sh_size),
						"Size: ");
				printf("%d B", s[pi].sh_size);
				___br;
			}

			if (___nonzero s[pi].sh_addralign)
			{
				printf("|\\sh_addralign(%03ld) ", (void*)&s[pi].sh_addralign - (void*)e);
				hex_msg(&s[pi].sh_addralign, sizeof(s[pi].sh_addralign),
						"");
				printf("%d", s[pi].sh_addralign);
				___br;
			}

			if (___nonzero s[pi].sh_entsize)
			{
				printf("|\\ sh_entsize(%03ld) ", (void*)&s[pi].sh_entsize - (void*)e);
				hex_msg(&s[pi].sh_entsize, sizeof(s[pi].sh_entsize),
						"Fixed-size pad.: ");
				printf("%d", s[pi].sh_entsize);
				___br;
			}

			// Sections:

			char	*ss;

			ss = (char*)((void*)e + s[pi].sh_offset);
			printf("\\/¨¨¨¨¨¨¨¨¨¨¨(%04d) ", s[pi].sh_offset);
			___br;

			if (s[pi].sh_type == SHT_NOBITS
					&& (s[pi].sh_flags & SHF_ALLOC
						|| s[pi].sh_flags & SHF_WRITE))
			{
				printf("SHT_NOBITS ");
				if (s[pi].sh_flags & SHF_ALLOC)
					printf("SHF_ALLOC ");
				if (s[pi].sh_flags & SHF_WRITE)
					printf("WRITE");
				___br;
				printf(".bss Uninit wiped data area.\n");
			}

			if (s[pi].sh_type == SHT_PROGBITS
					&& !s[pi].sh_flags)
			{
				printf("SHT_PROGBITS, no flags\n");
				printf(".comment Version control info.\n");
				printf(".debug Symbolic info.\n");
				lin_dump(ss, s[pi].sh_size, 51);
			}

			if (s[pi].sh_type == SHT_PROGBITS
					&& (s[pi].sh_flags & SHF_ALLOC
						|| s[pi].sh_flags & SHF_WRITE))
			{
				printf("SHT_PROGBITS ");
				if (s[pi].sh_flags & SHF_ALLOC)
					printf("SHF_ALLOC ");
				if (s[pi].sh_flags & SHF_WRITE)
					printf("WRITE");
				___br;
				printf(".ctor Pointers to constructors.\n");
				printf(".data Initialized data.\n");
				printf(".data1 Initialized data.\n");
				printf(".dtors Pointers to descructors.\n");
			}

			if (s[pi].sh_type == SHT_DYNAMIC
					&& s[pi].sh_flags & SHF_ALLOC)
			{
				printf("SHT_DYNAMIC SHF_ALLOC");
				if (s[pi].sh_flags & SHF_WRITE)
					printf("WRITE");
				___br;
				printf(".dynamic Dynamic linking info.\n");
			}

			if (s[pi].sh_type == SHT_STRTAB
					&& s[pi].sh_flags & SHF_ALLOC)
			{
				printf("SHT_STRTAB SHF_ALLOC\n");
				printf(".dynstr Strings for dynamic linking.\n");
			}

			if (s[pi].sh_type == SHT_DYNSYM
					&& s[pi].sh_flags & SHF_ALLOC)
			{
				printf("SHT_DYNSYM SHF_ALLOC\n");
				printf(".dynsym Dynamic symbol table.\n");
			}

			if (s[pi].sh_type == SHT_PROGBITS
					&& (s[pi].sh_flags & SHF_ALLOC
						|| s[pi].sh_flags & SHF_EXECINSTR))
			{
				printf("SHT_PROGBITS ");
				if (s[pi].sh_flags & SHF_ALLOC)
					printf("SHF_ALLOC ");
				if (s[pi].sh_flags & SHF_EXECINSTR)
					printf("EXECINSTR");
				___br;
				printf(".fini Finish exec code.\n");
			}

			if (s[pi].sh_type == SHT_GNU_versym
					&& (s[pi].sh_flags & SHF_ALLOC))
			{

				printf("SHT_GNU_versym SHF_ALLOC\n");
				printf(".gnu.version Version symbol table.\n");
				printf("\\ Array of Elf32_Half elements.\n");
				Elf32_Half *half = (Elf32_Half*)ss;
				printf("[");
				for (size_t i = 0; i < s[pi].sh_size; i += sizeof(Elf32_Half))
				{
					printf("%d", *(half + i));
					if (i + sizeof(Elf32_Half) < s[pi].sh_size)
						printf(", ");
					else
						printf("]\n");
				}
				printf(".gnu.version_r Needed elements\n");

				Elf32_Verneed *v_need = (Elf32_Verneed*)ss;
				printf("\\ Array of Elf32_Verneed elements.\n");
				printf("[");
				for (size_t i = 0; i < s[pi].sh_size; i += sizeof(Elf32_Verneed))
				{
					printf("%d", *((uint*)v_need + i)); // uint16 on man
					if (i + sizeof(Elf32_Verneed) < s[pi].sh_size)
						printf(", ");
					else
						printf("]\n");
				}

			}

			if (s[pi].sh_type == SHT_GNU_verdef
					&& (s[pi].sh_flags & SHF_ALLOC))
			{
				printf("SHT_GNU_verdef SHF_ALLOC\n");
				printf(".gnu.version_d Version symbol definitions.\n");
				Elf32_Verdef *v_def = (Elf32_Verdef*)ss;
				printf("[");
				for (size_t i = 0; i < s[pi].sh_size; i += sizeof(Elf32_Verdef))
				{
					printf("%d", *((uint*)v_def + i)); // uint16 on man
					if (i + sizeof(Elf32_Verdef) < s[pi].sh_size)
						printf(", ");
					else
						printf("]\n");
				}
			}

			___br
			hex_byte(ss, s[pi].sh_size);
		}
	}

	printf("___________/\\(%04d)\n", s[pi].sh_offset + s[pi].sh_size);

//	printf("] -------------------------------------------------/\n");

//	___br;
//	int elf = 2645008368;
//	printf("%s\n", (char*)&elf);

//	return ;
}
