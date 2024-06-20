#include "woody.h"
#include <string.h>

void read_file(t_elf *elf, char *filename)
{
    int fd;
    int filesize;

    fd = open(filename, O_RDONLY);
    filesize = get_filesize(fd);
    elf->data = malloc(filesize + 1);
    read(fd, elf->data, filesize);

    // if ((fd == -1) || (filesize == -1))
    // {
    //     close(fd);
    //     print_error("Invalid file");
    //     free(elf);
    // }
    char buffer[filesize + 1];
    ssize_t bytes_read = read(fd, buffer, filesize);
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

    // Verify ELF header 
    if (ehdr.e_ident[EI_MAG0] != ELFMAG0 ||
        ehdr.e_ident[EI_MAG1] != ELFMAG1 ||
        ehdr.e_ident[EI_MAG2] != ELFMAG2 ||
        ehdr.e_ident[EI_MAG3] != ELFMAG3) {
        printf("Invalid ELF file.\n");
        exit(1) ;
    }
    printf("%d", filesize);
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
            printf("Section Flags: 0x%lx\n", shdr[i].sh_flags);
            printf("Section Type: 0x%x\n", shdr[i].sh_type);
            printf("Section Link: 0x%x\n", shdr[i].sh_link);
            printf("Section Info: 0x%x\n", shdr[i].sh_info);
            printf("Section Alignment: 0x%lx\n", shdr[i].sh_addralign);
            printf("Section Entry Size: 0x%lx\n", shdr[i].sh_entsize);
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
    // Add more relevant fields as needed
}


void get_code_load(t_elf *elf) {
    
    Elf64_Ehdr *ehdr = elf->data;
    Elf64_Phdr *phdr = elf->data + ehdr->e_phoff;

    for (int i = 0; i < ehdr->e_phnum; i++) {

        if (phdr[i].p_vaddr == phdr[i].p_paddr
			&& phdr[i].p_memsz == phdr[i].p_filesz
			&& phdr[i].p_type == 1
			&& ehdr->e_entry >= phdr[i].p_vaddr
			&& ehdr->e_entry < phdr[i].p_vaddr + phdr[i].p_memsz) {

            elf->code = &phdr[i];
            print_code_segment_info(elf->code);
        }
    }
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
 //   decrypt_file(&elf);
}