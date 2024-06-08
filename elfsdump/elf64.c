#include "elfsdump.h"

extern dumpster				elf;
extern unsigned char	*h;
extern uint16_t				u16;
extern uint32_t				u32;

void	pretty_print64()
{
	Elf64_Ehdr *e = elf.ehdr._64; // for alias only.
	Elf64_Phdr *p = elf.phdr._64;

	// ELF Header
	printf("|==================================================|\n");
	printf("[ Elf64_Ehdr   %p\n", e);
	printf("|--------------------------------------------------|\n");

	printf("|\\ e_ident    [%02d]\n", 0);

	printf("|| EI_MAG 4b: ");
	h = (unsigned char*)e;
	hex_byte(h, 4);
	printf("||       \\___ 7f 45 4c 46 .ELF <- eq to validate.\n");

	printf("|| EI_CLASS 1b: ");
	h = (unsigned char*)e + EI_CLASS;
	hex_pure(h, 1);
	byte_is(h, ELFCLASSNONE, "None/invalid architecture.");
	byte_is(h, ELFCLASS32, "32-bit architecture.");
	byte_is(h, ELFCLASS64, "64-bit architecture.");
	___br;

	printf("|| EI_DATA 1b: ");
	h = (unsigned char*)e + EI_DATA;
	hex_pure(h, 1);
	byte_is(h, ELFDATANONE, "Unknown data format.");
	byte_is(h, ELFDATA2LSB, "2-complement, litte-endian.");
	byte_is(h, ELFDATA2MSB, "2-complement, big-endian.");
	___br;

	printf("|| EI_VERSION 1b: ");
	h = (unsigned char*)e + EI_VERSION;
	if (h == EV_NONE)
		hex_msg(h, EV_CURRENT, "Ivalid version.");
	else
		hex_msg(h, EV_CURRENT, "<- Current version (hex).");
	___br;

	printf("|| EI_OSABI 1b: ");
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

	printf("|| EI_ABIVERSION 1b: ");
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
		hex_msg((void*)&e->e_entry, sizeof(e->e_entry),
			___spc64 "Entry point address.");
	else
		hex_msg((void*)&e->e_entry, sizeof(e->e_entry),
			___spc64 "No associated entry point.");
	___br;

	printf("\\ e_phoff     (%ld) ", (void*)&e->e_phoff - (void*)e);
	if (e->e_phoff)
	{
		hex_msg((void*)&e->e_phoff, sizeof(e->e_phoff),
			___spc64 "PH offset: ");
		printf("%ld B", e->e_phoff);
	}
	else
	{
		hex_msg((void*)&e->e_phoff, sizeof(e->e_phoff),
			___spc64 "PH zero, no prog. table");
	}
	___br;

	printf("\\ e_shoff     (%ld) ", (void*)&e->e_shoff - (void*)e);
	hex_msg((void*)&e->e_shoff, sizeof(e->e_shoff),
		___spc64 "SH offset: ");
	if (e->e_shoff)
		printf("%ld B", e->e_shoff);
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

	printf("[ Elf64_Phdr     %p\n", p);
	printf("|--------------------------------------------------|\n");

	uint32_t pi = -1; // same type for 32 and 64-bit.

	while (++pi < elf.phnum)
	{
		printf("/--------- [%03ld] %02d/%02d phdr segment ---------------\\\n", (void*)&p[pi] - (void*)e, pi, e->e_phnum) ;
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

		printf("\\ p_flags  (%03ld) ",
			(void*)&p[pi].p_flags - (void*)e);
		hex_pure(&p[pi].p_flags, sizeof(p[pi].p_flags));
		flag_is(p[pi].p_flags, PF_R, "+r");
		flag_is(p[pi].p_flags, PF_W, "+w");
		flag_is(p[pi].p_flags, PF_X, "+x");
		___br;

		printf("\\ p_offset (%03ld) ", (void*)&p[pi].p_offset - (void*)e);
		hex_msg(&p[pi].p_offset, sizeof(p[pi].p_offset),
			___spc64 "Section offset: ");
		printf("%ld B", p[pi].p_offset);
		___br;

		printf("\\ p_vaddr  (%03ld) ", (void*)&p[pi].p_vaddr - (void*)e);
		hex_msg(&p[pi].p_vaddr, sizeof(p[pi].p_vaddr),
			___spc64 "Seg virt address. ");
		___br;

		printf("\\ p_paddr  (%03ld) ", (void*)&p[pi].p_paddr - (void*)e);
		hex_msg(&p[pi].p_paddr, sizeof(p[pi].p_paddr),
			___spc64 "Seg physical addr.");
		___br;

		printf("\\ p_filesz (%03ld) ", (void*)&p[pi].p_filesz - (void*)e);
		hex_msg(&p[pi].p_filesz, sizeof(p[pi].p_filesz),
			___spc64 "Seg file img: ");
		printf("%ld B", p[pi].p_filesz);
		___br;

		printf("\\ p_memsz  (%03ld) ", (void*)&p[pi].p_memsz - (void*)e);
		hex_msg(&p[pi].p_memsz, sizeof(p[pi].p_memsz),
			___spc64 "Seg mem size: ");
		printf("%ld B", p[pi].p_memsz);
		___br;

		printf("\\ p_align  (%03ld) ", (void*)&p[pi].p_align - (void*)e);
		hex_msg(&p[pi].p_align, sizeof(p[pi].p_align),
			___spc64 "Seg mem align: ");
		printf("%ld", p[pi].p_align);
		___br;
	}
	printf("] -------------------------------------------------/\n");

}
