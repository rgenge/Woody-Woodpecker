#include "woody.h"
#include <string.h>

#define DATALOAD_SIZE (_loadend - _dataload)
#define TOTAL_DATALOAD_SIZE (DATALOAD_SIZE+1)


void read_file(t_elf *elf, char *filename)
{
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

void decrypt_file(t_elf *elf) {

    
    printf("%ld", elf->code->p_offset);
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

    unsigned char *char_data = (unsigned char *)data;
    printf("Original data:\n");
    for (size_t i = 0; i < size; ++i) {
        printf("%c", char_data[i]);
        char_data[i] = char_data[i] + 1;
    }
    printf("\n");
    // Print the encoded data for debugging purposes
    printf("Encoded data, %ld::\n", key);
    for (size_t i = 0; i < size; ++i) {
        printf("%c", char_data[i]);
    }
    printf("\n");

    // Decoding: Decrement each character by 1
    for (size_t i = 0; i < size; ++i) {
        char_data[i] = char_data[i] - 1;
    }

    // Print the decoded data for debugging purposes
    printf("Decoded data:\n");
    for (size_t i = 0; i < size; ++i) {
        printf("%c", char_data[i]);
    }
    printf("\n");
}

void save_encrypt_data(t_elf *elf, t_encrypt *encrypt)
{
    Elf64_Ehdr *ehdr = elf->data;

    encrypt->key = 0;
    encrypt->sh_addr = elf->text->sh_addr;
    encrypt->sh_size = elf->text->sh_size;
    encrypt->original_entry = ehdr->e_entry;

    Elf64_Ehdr *tmp = (Elf64_Ehdr*)elf->data;
  //  tmp->e_entry = elf->code->p_vaddr + elf->code->p_memsz + 17;

   // encrypt->e_entry = tmp->e_entry;
   printf("key_value: %ld %p\n\n",  tmp->e_entry, elf->data);

   void *pos = elf->data + elf->text->sh_offset;
   encrypt_text_section(elf->text->sh_size, pos, encrypt->key);
}


void insert_woody(t_elf *elf, t_encrypt *encrypt) {




 //   Elf64_Ehdr *or_ehdr = elf->data;
    Elf64_Ehdr *ehdr = elf->data;
    Elf64_Ehdr *or_ehdr = malloc(sizeof(Elf64_Ehdr));
    if (or_ehdr == NULL) {
        // Handle allocation error
        return;
    }

    memcpy(or_ehdr, elf->data, sizeof(Elf64_Ehdr));

    uint64_t original_entrypoint = or_ehdr->e_entry; // Save the original entry point

    void *tmp_ptr = elf->data + elf->code->p_offset + elf->code->p_filesz; // Move to the pointer where the load section is 
    ehdr->e_entry = elf->code->p_vaddr + elf->code->p_memsz + 17; // Change the entry point of the file based on the load + size of message

    elf->code->p_flags |= PF_X | PF_W ;
 //   int x = DATALOAD_SIZE - sizeof(t_encrypt);
    ft_memcpy(tmp_ptr, g_decryptor,  DATALOAD_SIZE);
//    tmp_ptr +=  x;
    printf("key_value: %ld %ld\n\n",  encrypt->key, encrypt->sh_size);
  //  ft_memcpy(tmp_ptr+x, &encrypt, sizeof(t_encrypt));
 //   tmp_ptr += sizeof(t_encrypt);
    tmp_ptr += DATALOAD_SIZE;
    // Calculate the offset to the original entry point
    int offset = original_entrypoint - (elf->code->p_vaddr + elf->code->p_filesz + TOTAL_DATALOAD_SIZE);
    elf->code->p_memsz += DATALOAD_SIZE;
    elf->code->p_filesz += DATALOAD_SIZE;
    // Overwrite the instruction at the end of _dataload to jump to the original entry point
    *((char *)tmp_ptr) = 0xe9; // Jump instruction
    *((int *)(tmp_ptr + 1)) = offset; // Offset to jump

  


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