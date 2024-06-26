#include "woody.h"
#include <string.h>

#define DATALOAD_SIZE (_loadend - _dataload)
#define TOTAL_DATALOAD_SIZE (DATALOAD_SIZE + 5)

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
        if (!ft_strncmp(section_name, ".text", 6)) {
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


void get_code_load(t_elf *elf) {
    
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
            insert_woody(elf);
            break;
        }
    }
}

void insert_woody(t_elf *elf) {

    Elf64_Ehdr *or_ehdr = elf->data;
   // Elf64_Phdr *or_phdr = elf->data + or_ehdr->e_phoff;
    uint64_t original_entrypoint = or_ehdr->e_entry; // Save the original entry point

    void *tmp_ptr = elf->data + elf->code->p_offset + elf->code->p_filesz; // Move to the pointer where the load section is 
    or_ehdr->e_entry = elf->code->p_vaddr + elf->code->p_memsz + 17; // Change the entry point of the file based on the load + size of message
   
    elf->code->p_flags = PF_R | PF_X ;
    ft_memcpy(tmp_ptr, _dataload, DATALOAD_SIZE);
    tmp_ptr += DATALOAD_SIZE;

    // Calculate the offset to the original entry point
    int offset = original_entrypoint - (elf->code->p_vaddr + elf->code->p_memsz + TOTAL_DATALOAD_SIZE);

    // Overwrite the instruction at the end of _dataload to jump to the original entry point
    *((char *)tmp_ptr) = 0xe9; // Jump instruction
    *((int *)(tmp_ptr + 1)) = offset; // Offset to jump

  
    elf->code->p_memsz += TOTAL_DATALOAD_SIZE;
    elf->code->p_filesz += TOTAL_DATALOAD_SIZE;

//uint16_t *target = (uint16_t *)elf->data + (0x00000010/2);
//*(target + 4) = 0x118a;
//     target = (uint16_t *)elf->data + (0x000000e0/2);
// *(target +6 ) = 0x0005;



//     target = (uint16_t *)elf->data + (0x000011a0/2);
// *(target +2 ) = 0xb7e9;
//     target = (uint16_t *)elf->data + (0x000011a0/2);
// *(target +3 ) = 0xfffe;
//     target = (uint16_t *)elf->data + (0x000011a0/2);
// *(target +4 ) = 0x00ff;

    int fd = open("woody", O_TRUNC | O_CREAT | O_WRONLY, 0777);
	write(fd, elf->data, elf->filesize);
	close(fd);
}



int			main(int ac, char **av)
{
    t_elf   elf;
    ft_memset(&elf, 0, sizeof(elf));
    if (ac != 2){
        print_error("Type the program with one argument");
        printf("%s", av[1]);
    }
    read_file(&elf, av[1]);
    get_text(&elf);
    get_code_load(&elf);
    free(elf.data);
}