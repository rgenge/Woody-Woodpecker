#include "woody.h"
#include <string.h>

#define PRINT_WOODY_FUNC "\x48\xC7\xC0\x01\x00\x00\x00" /* mov rax, 1 (sys_write) */ \
                         "\x48\xC7\xC7\x01\x00\x00\x00" /* mov rdi, 1 (stdout) */ \
                         "\x48\x8D\x35\x0C\x00\x00\x00" /* lea rsi, [rip+0x0c] (address of 'woody\n') */ \
                         "\x48\xC7\xC2\x06\x00\x00\x00" /* mov rdx, 6 (length of 'woody\n') */ \
                         "\x0F\x05"                     /* syscall */ \
                         "\xC3"                         /* ret */ \
                         "woody\n"                      /* data: 'woody\n' */

#define XOR_KEY 0xFF


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

    // Dynamic memory allocation for section name
    size_t name_length = shdr[ehdr->e_shstrndx].sh_offset;
    char *name = malloc(name_length + 1);  // Allocate space for data and null terminator
    if (name == NULL) {
        fprintf(stderr, "Failed to allocate memory for section name\n");
        exit (-1);  // Or handle allocation failure appropriately
    }
    // Copy section header string table data
    ft_memcpy(name, elf->data + shdr[ehdr->e_shstrndx].sh_offset, name_length);
    name[name_length] = '\0';  // Explicitly add null terminator

    // Process section names and program headers
    for (int i = 0; i < ehdr->e_shnum; i++) {
        // Access section name using offset from shdr table
        char *section_name = name + shdr[i].sh_name;

        // Search for ".text" section (adjust condition as needed)
        if (!ft_strncmp(section_name, ".text", 6)) {
            // ... (process program header for .text section)
            printf("\nProgram Header %d for section '%s':\n", i, section_name);
            printf("Section Name: %s\n", section_name);
            printf("Section Offset: 0x%lx\n", shdr[i].sh_offset);
            printf("Section Size: 0x%lx\n", shdr[i].sh_size);
            // printf("Section Flags: 0x%lx\n", shdr[i].sh_flags);
            // printf("Section Type: 0x%x\n", shdr[i].sh_type);
            // printf("Section Link: 0x%x\n", shdr[i].sh_link);
            // printf("Section Info: 0x%x\n", shdr[i].sh_info);
            // printf("Section Alignment: 0x%lx\n", shdr[i].sh_addralign);
            // printf("Section Entry Size: 0x%lx\n", shdr[i].sh_entsize);
            elf->text = &shdr[i];
        }
    }

    free(name);  // Release allocated memory
}


void print_code_segment_info(const Elf64_Phdr *segment) {
    printf("Load Segment Information:\n");
    printf("Type: %d\n", segment->p_type);
    printf("Virtual Address: 0x%lx\n", segment->p_vaddr);
    printf("Physical Address: 0x%lx\n", segment->p_paddr);
    printf("Size in Memory: %lu bytes\n", (unsigned long)segment->p_memsz);
    printf("Size in File: %lu bytes\n", (unsigned long)segment->p_filesz);
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

            insert_woody(elf, elf->code);
            break;
        }
    }
}

void insert_woody(t_elf *elf, Elf64_Phdr *current_phdr) {
    size_t func_size = sizeof(PRINT_WOODY_FUNC) - 1;
    size_t new_size = elf->filesize;

    Elf64_Ehdr *ehdr = elf->data;
    Elf64_Phdr *phdr = elf->data + ehdr->e_phoff;
    printf("%d %d" , current_phdr->p_type, phdr->p_type);
    uint64_t original_entrypoint = ehdr->e_entry;

    char *new_data = malloc(new_size);
    if (!new_data) {
        print_error("Failed to allocate memory");
    }

    // Copy original ELF file data to new memory
    memcpy(new_data, elf->data, elf->filesize);

    // Find the insertion point (end of the first PT_LOAD segment)
    size_t insertion_point = elf->code->p_offset + elf->code->p_filesz ;

    // Insert the new code at the calculated position
    memcpy(new_data + insertion_point, PRINT_WOODY_FUNC, func_size);
    Elf64_Ehdr *eehdr = (Elf64_Ehdr *)new_data;
    Elf64_Phdr *pphdr = (Elf64_Phdr *)new_data;

    // Update the entry point to the new function
    eehdr->e_entry = elf->code->p_vaddr + elf->code->p_memsz + func_size;

    // Patch the return address of the print function to jump to the original entry point
    *(Elf64_Addr *)(new_data + insertion_point + 24) = original_entrypoint;

    // Update the code segment size in the ELF program header
    pphdr->p_filesz += func_size;
    pphdr->p_memsz += func_size;

    // Write the modified ELF file to a new file
    FILE *output = fopen("modified_file", "wb");
    if (!output) {
        print_error("Failed to open output file");
    }

    fwrite(new_data, 1, new_size, output);
    fclose(output);

    free(new_data);
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
}