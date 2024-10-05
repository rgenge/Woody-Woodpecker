#include "woody.h"
<<<<<<< HEAD
#include <string.h>

#define DATALOAD_SIZE (_loadend - _dataload)
#define TOTAL_DATALOAD_SIZE (DATALOAD_SIZE)
=======
#include "buzz_buzzard.h"
>>>>>>> last-minute

extern unsigned char src_buzz_buzzard_bin[];
extern unsigned int src_buzz_buzzard_bin_len;

elf_t				*elf;
injector		*inj;
bool				elf_alloc = false;
bool				elf_data_alloc = false;
bool				inj_alloc = false;
bool				inj_data_alloc = false;

void	elf_init(char *vict, elf_t** elf_ptr)
{
<<<<<<< HEAD
    int fd;

    fd = open(filename, O_RDONLY);
    elf->filesize = get_filesize(fd);
    elf->data = malloc(elf->filesize + 1);
    read(fd, elf->data,  elf->filesize);

    char buffer[elf->filesize + 1];
    ssize_t bytes_read = read(fd, buffer,  elf->filesize);
    if (bytes_read == -1) {
        perror("Error reading file");
    } else {
        buffer[bytes_read] = '\0'; // Null-terminate the buffer
        printf("File contents:\n%s\n", buffer);
    }
    FILE* input_elf = fopen(filename, "rb");

    if (!input_elf) {
        printf("Error opening files.\n");
        exit(1);
    }

    Elf64_Ehdr ehdr;
    fread(&ehdr, sizeof(Elf64_Ehdr), 1, input_elf);
    elf->ehdr = ehdr;

    //    Elf64_Ehdr ehdr;
    //fread(&ehdr, sizeof(Elf64_Ehdr), 1, elf->data);
   // elf->ehdr = ehdr;

    // Verify ELF header 
    if (ehdr.e_ident[EI_MAG0] != ELFMAG0 ||
        ehdr.e_ident[EI_MAG1] != ELFMAG1 ||
        ehdr.e_ident[EI_MAG2] != ELFMAG2 ||
        ehdr.e_ident[EI_MAG3] != ELFMAG3) {
        printf("Invalid ELF file.\n");
        exit(1) ;
    }
    printf("%ld", elf->filesize);
}

void get_text(t_elf *elf) {
    
    Elf64_Ehdr *ehdr = elf->data;
    Elf64_Shdr *shdr = elf->data + ehdr->e_shoff;

    size_t name_length = shdr[ehdr->e_shstrndx].sh_offset;
    char *name = malloc(name_length + 1);
    if (name == NULL) {
        fprintf(stderr, "Failed to allocate memory for section name\n");
        exit (-1);  // Or handle allocation failure appropriately
    }
    // Copy section header string table data
    ft_memcpy(name, elf->data + shdr[ehdr->e_shstrndx].sh_offset, name_length);
    name[name_length] = '\0';  

    for (int i = 0; i < ehdr->e_shnum; i++) {
        char *section_name = name + shdr[i].sh_name;

        // Search  ".text" section
        if (!ft_strncmp(section_name, ".text", 6) && shdr[i].sh_type == SHT_PROGBITS
			&& (shdr[i].sh_flags & SHF_EXECINSTR)) {
            printf("\nProgram Header %d for section '%s':\n", i, section_name);
            printf("Section Name: %s\n", section_name);
            printf("Section Offset: 0x%lx\n", shdr[i].sh_offset);
            elf->text = &shdr[i];
        }
    }

    free(name);  // Release allocated memory
}


void print_code_segment_info(const Elf64_Phdr *segment) {
    printf("Load Segment Information:\n");
    printf("Type: %d\n", segment->p_type);
    printf("Virtual Address: 0x%lx\n", segment->p_vaddr);
}

void *ft_memmove(void *dst, const void *src, size_t len) {
	if (!dst && !src)
		return NULL;
	unsigned char *dst_ptr = (unsigned char*)dst;
	unsigned char *src_ptr = (unsigned char*)src;
	size_t cntr = 0;
	if (dst > src) {
		while (cntr < len) {
			dst_ptr[(len - 1) - cntr] = src_ptr[(len - 1) - cntr];
			++cntr;
		}
	} else {
		while (len--) {
			dst_ptr[cntr] = src_ptr[cntr];
			++cntr;
		}
	}
	return dst;
}

void get_code_load(t_elf *elf, t_encrypt *encrypt) {
    
    Elf64_Ehdr *ehdr = elf->data;
    Elf64_Phdr *phdr = elf->data + ehdr->e_phoff;
    // Iterate through headers looking for the PT_LOAD header where the code is
    for (int i = 0; i < ehdr->e_phnum; i++) {
        if (phdr[i].p_vaddr == phdr[i].p_paddr
			&& phdr[i].p_memsz == phdr[i].p_filesz
			&& phdr[i].p_type == 1
			&& ehdr->e_entry >= phdr[i].p_vaddr
			&& ehdr->e_entry < phdr[i].p_vaddr + phdr[i].p_memsz) {           
            elf->code = &phdr[i];
            print_code_segment_info(elf->code);
            save_encrypt_data(elf, encrypt);
            insert_woody(elf, encrypt);
            break;
        }
    }
}

void encrypt_text_section(uint64_t size, void *data, uint64_t key)
{
   // int64_t tmp_key = key;

    uint8_t *char_data = (uint8_t *)data;
    printf("Original data:\n");
    for (size_t i = 0; i < size; ++i) {
       // printf("%c", char_data[i]);
        ;char_data[i] = char_data[i] + 1;
    }
    // Print the encoded data for debugging purposes
    printf("Encoded data, %ld::\n", key);
    for (size_t i = 0; i < size; ++i) {
     //   printf("%c", char_data[i]);
    }

    // // Decoding: Decrement each character by 1
    // for (size_t i = 0; i < size; ++i) {
    //     char_data[i] = char_data[i] - 1;
    // }

    // Print the decoded data for debugging purposes
    // printf("Decoded data:\n");
    // for (size_t i = 0; i < size; ++i) {
    //     printf("%c", char_data[i]);
    // }
    // printf("\n");
}

void save_encrypt_data(t_elf *elf, t_encrypt *encrypt)
{
  //  Elf64_Ehdr *ehdr = elf->data;

    encrypt->key = 0;

  //  Elf64_Ehdr *tmp = (Elf64_Ehdr*)elf->data;
  //  tmp->e_entry = elf->code->p_vaddr + elf->code->p_memsz + 17;

   // encrypt->e_entry = tmp->e_entry;
 //  printf("key_value: %ld %p\n\n",  tmp->e_entry, elf->data);

   void *pos = elf->data + elf->text->sh_offset;
   encrypt_text_section(elf->text->sh_size, pos, encrypt->key);
}


void insert_woody(t_elf *elf, t_encrypt *encrypt) {

    char jmp = 0xe9;
 //   Elf64_Ehdr *or_ehdr = elf->data;
    Elf64_Ehdr *ehdr = elf->data;
    Elf64_Ehdr *or_ehdr = malloc(sizeof(Elf64_Ehdr));
    if (or_ehdr == NULL) {
        // Handle allocation error
        return;
    }
    // uint8_t *char_data = (uint8_t *)elf->data + elf->text->sh_offset;
    // size_t size= elf->text->sh_size;
    // printf("encoded data 2:");

    // for (size_t i = 0; i < size; ++i) {
    //     printf("%c", char_data[i]);
    // }
    // printf("\n");
    memcpy(or_ehdr, elf->data, sizeof(Elf64_Ehdr));

    uint64_t original_entrypoint = or_ehdr->e_entry; // Save the original entry point
    void *tmp_ptr = elf->data + elf->code->p_offset + elf->code->p_filesz; // Move to the pointer where the load section is 
    ehdr->e_entry = elf->code->p_vaddr + elf->code->p_memsz + 17; // Change the entry point of the file based on the load + size of message

    elf->code->p_flags |= PF_X | PF_W ;
    ft_memcpy(tmp_ptr, g_decryptor,  DATALOAD_SIZE);
    printf("key_value: %ld %ld\n\n",  encrypt->key, encrypt->sh_size);
    tmp_ptr += DATALOAD_SIZE;
    // Calculate the offset to the original entry point
    int offset = original_entrypoint - (elf->code->p_vaddr + elf->code->p_memsz + DATALOAD_SIZE + 5);
    // Overwrite the instruction at the end of _dataload to jump to the original entry point
    *((char *)tmp_ptr) = jmp; // Jump instruction
    printf("%d %p %d", offset, &offset, jmp);
    *((int *)(tmp_ptr + 1)) = offset; // Offset to jump

    elf->code->p_memsz += DATALOAD_SIZE ;
    elf->code->p_filesz += DATALOAD_SIZE;


    int fd = open("woody", O_TRUNC | O_CREAT | O_WRONLY, 0777);
	write(fd, elf->data, elf->filesize);
	close(fd);
}

int			main(int ac, char **av)
{
    t_elf   elf;
    t_encrypt encrypt;
    ft_memset(&elf, 0, sizeof(elf));
    ft_memset(&encrypt, 0, sizeof(encrypt));
    if (ac != 2){
        print_error("Type the program with one argument");
        printf("%s", av[1]);
    }
    read_file(&elf, av[1]);
    get_text(&elf);
    get_code_load(&elf, &encrypt);
    free(elf.data);
}
=======
	read_original(vict, &(*elf_ptr));
	(*elf_ptr)->bit_class =
		(*elf_ptr)->data[EI_CLASS] == ELFCLASS64 ? 64
		: (*elf_ptr)->data[EI_CLASS] == ELFCLASS32 ? 32
		: 0;
	___die((*elf_ptr)->bit_class == 32, "Not accepting 32bit files");
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

void	read_blob()
{
	off_t			filesize;
	filesize = src_buzz_buzzard_bin_len;
	___die ((int)filesize == 0, "Blob size not declared.");
	inj = malloc(sizeof(injector));
	___die (!inj, "Failed to create injector.");
	inj_alloc = true;
	inj->bin_size = (uint32_t)filesize;
	inj->bin = (char*)src_buzz_buzzard_bin;
	___die (!inj->bin, "Could not locate extern injection binary.");
	inj->data_size = elf->data_size;
	inj->data = malloc(inj->data_size);
	___die(!inj->data, "Failed to prepare injected alloc block.");
	inj_data_alloc = true;
}

void	inject(const char *woody)
{
	size_t	i;
	int32_t	original_filesz;
	char		*h;
	char		*s;

	read_blob();

	/*
	 * Full clone.
	 */
	ft_memcpy(inj->data, elf->data, elf->data_size);

	/*
	 * Positions.
	 */
	Elf64_Ehdr* EE = (Elf64_Ehdr*)elf->data;
	Elf64_Ehdr* IE = (Elf64_Ehdr*)inj->data;
	Elf64_Phdr* IP = (Elf64_Phdr*)(inj->data + EE->e_phoff);
	Elf64_Shdr* IS = (Elf64_Shdr*)(inj->data + EE->e_shoff);
	Elf64_Phdr* IPX = 0;
	Elf64_Shdr* ISX = 0;
	Elf64_Addr original_entry;

	/*
	 * Find IPX, the exec Load Phdr responsible for .text.
	 */
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

	/*
	 * Find string table.
	 */
	Elf64_Shdr* SHST = &IS[EE->e_shstrndx];
	Elf64_Off stoff = SHST->sh_offset;
	char* ST = (char*)(inj->data + stoff);

	/*
	 * Find ISX, the exec Load Shdr responsible for .text.
	 */
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

	/*
	 * Adjust parameters.
	 */
	original_entry = IE->e_entry;
	original_filesz = IPX->p_filesz;
	IE->e_entry = IPX->p_vaddr + IPX->p_memsz;
	IPX->p_flags |= PF_W;
	IPX->p_filesz += inj->bin_size;
	IPX->p_memsz += inj->bin_size;
	ISX->sh_size += inj->bin_size;
	ISX->sh_flags |= SHF_WRITE;

	/*
	 * Validate enough padding space.
	 */
	s = (char*)IE + IE->e_entry;
	h = s;
	while (!*h && h++ - s < inj->bin_size) ;
	___die(h - s != inj->bin_size + 1, "Not enough padding space for injection.");

	/*
	 * The jump:
	 */
	int32_t *last_jump;
	last_jump = (int32_t*)(inj->bin + inj->bin_size - sizeof(int32_t));
	*last_jump = original_entry - IE->e_entry - inj->bin_size;

	/*
	 * Generate key
	 */
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

	/*
	 * Write key
	 */
	h = (char*)(inj->bin + inj->bin_size - 15);
	*(long int*)h = key;

	/*
	 * Encript.
	 */
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

	/*
	 * Inject.
	 */
	ft_memcpy((void*)IE + IE->e_entry, (void*)inj->bin, inj->bin_size);

	/*
	 * Lastly.
	 */
	file_out_to_file(woody, inj->data, inj->data_size);
}

int		main(int argc, char **argv)
{
	___die(argc != 2, "Usage: `woody_woodpacker binary_file`");
	elf_init(argv[1], &elf);
	inject("woody");
	return free_all();
}
>>>>>>> last-minute
