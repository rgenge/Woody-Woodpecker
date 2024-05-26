#include "woody.h"


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

void    decrypt_file(t_elf *elf)
{
    Elf64_Ehdr  *ehdr;
    Elf64_Phdr  *phdr;
  //  t_dv        *dv;
  //  void        *ptr; 
    int i = -1;

    ehdr = elf->data;
    phdr = elf->data + ehdr->e_phoff;
    while (++i < ehdr->e_phnum)
    {
        if(phdr[i].p_vaddr == phdr[i].p_paddr && phdr[i].p_memsz == phdr[i].p_filesz)
        {
           elf->code = &phdr[i];
        } 
    }
    printf("\n%ld",elf->code->p_memsz);
    return;


    printf("decypt: %s", (char *)elf->data);
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
    decrypt_file(&elf);
}