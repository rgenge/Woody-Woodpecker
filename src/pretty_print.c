#include "woody.h"

void	byte_is(unsigned char* h, char test, const char* msg)
{	if (*h == test) printf("%s", msg); }

void	true_is(long long a, long long b, const char* msg)
{	if (a == b) printf("%s", msg); }

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

void	pretty_print(t_elf* ex)
{
	unsigned char* h;
	t_elf e = *ex;
	uint16_t u16;
	uint32_t u32;

	printf("Elf64_Ehdr: %p\n", ex);

	printf("\\ e_ident   (+%d)\n", 0);
	h = (unsigned char *)e.ehdr;

	printf("|| magic    ");
	hex_byte(h, 4);
	printf(" <- This must be the same as");
	___br;
	___deb printf("||      \\__ 7f 45 4c 46 .ELF <- this to validate.\n");

	printf("|| class    ");
	h += 4;
	hex_pure(h, 1);
	byte_is(h, ELFCLASSNONE, "None architecture/Invalid.");
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
	printf("|\\_________ (+%02ld) padding from here.\n", h - e.ehdr->e_ident);
	
	printf("|                 ");
	hex_byte(h, ((unsigned char *)&e.ehdr->e_type - e.ehdr->e_ident) - (h - e.ehdr->e_ident));
	___br;

	printf("\\ e_type    (+%ld) ", (unsigned char *)&e.ehdr->e_type - e.ehdr->e_ident);
	u16 = e.ehdr->e_type;
	hex_pure(&u16, sizeof(e.ehdr->e_type));
	true_is(u16, ET_NONE, "Unkown type.");
	true_is(u16, ET_REL, "A relocatable file.");
	true_is(u16, ET_EXEC, "An executable file.");
	true_is(u16, ET_DYN, "A shared object.");
	true_is(u16, ET_CORE, "A core file.");
	___br;

	printf("\\ e_machine (+%ld) ", (void *)&e.ehdr->e_machine - (void*)e.ehdr);
	u16 = e.ehdr->e_machine;
	hex_pure(&u16, sizeof(e.ehdr->e_type));
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

	printf("\\ e_version (+%ld) ", (void*)&e.ehdr->e_version - (void*)e.ehdr);
	u32 = e.ehdr->e_version;
	if (u32 == EV_NONE) 
		hex_msg((void*)&u32, 4, " Invalid version.");
	else
	{
		hex_pure((void*)&u32, 4);
		printf("= %d <- File version.", u32);
	}
	___br;

	printf("\\ e_entry   (+%ld) ", (void*)&e.ehdr->e_entry - (void*)e.ehdr);
		hex_pure((void*)&e.ehdr->e_entry, sizeof(e.ehdr->e_entry));
	printf("= %d", (int)e.ehdr->e_entry);
	if (e.ehdr->e_entry)
		printf("\n|         virtual \\ entry point address /");
	else
		hex_msg((void*)&e.ehdr->e_entry, sizeof(e.ehdr->e_entry), "\n|                 ^ No associated entry point.");
	___br;

	printf("\\ e_phoff   (+%ld) ", (void*)&e.ehdr->e_phoff - (void*)e.ehdr);
	hex_msg((void*)&e.ehdr->e_phoff, sizeof(e.ehdr->e_phoff), "\n|  program header:");
	if (e.ehdr->e_phoff)
		printf("\\    %ld B offset    /", e.ehdr->e_phoff);
	else
		printf("\\ (zero:) no header table.");
	___br;

	printf("\\ e_shoff   (+%ld) ", (void*)&e.ehdr->e_shoff - (void*)e.ehdr);
	hex_msg((void*)&e.ehdr->e_shoff, sizeof(e.ehdr->e_shoff), "\n|  section header:");
	if (e.ehdr->e_shoff)
		printf("\\    %ld kB offset   /", e.ehdr->e_shoff / 1024);
	else
		printf("\\ (zero:) no section header.");
	___br;


	___br;
	return ;

//	printf("p_type %d: ", e.phdr->p_type);
//	printf(" PT_NULL %b ", e.phdr->p_type & PT_NULL & true);
//	printf(" PT_LOAD %b ", e.phdr->p_type & PT_LOAD & true);
//	printf(" PT_INTERP %b ", e.phdr->p_type & PT_INTERP & true);
//	printf(" PT_NOTE %b ", e.phdr->p_type & PT_NOTE & true);
//	printf(" PT_SHLIB %b ", e.phdr->p_type & PT_SHLIB & true);
//	printf(" PT_PHDR %b ", e.phdr->p_type & PT_PHDR & true);
//	printf(" PT_LOPROC %b ", e.phdr->p_type & PT_LOPROC & true);
//	printf(" PT_HIPROC %b ", e.phdr->p_type & PT_HIPROC & true);
//	printf(" PT_GNU_STACK %b ", e.phdr->p_type & PT_GNU_STACK & true);
//	printf ("0 %b ", e.phdr->p_type & 0 & true);
}
