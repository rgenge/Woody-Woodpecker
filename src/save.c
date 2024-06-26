void insert_woody(t_elf *elf, Elf64_Phdr *current_phdr) {
    size_t new_size = elf->filesize; // Adjusting the new size to include the payload

  //  Elf64_Ehdr *eehdr = elf->data;
   // Elf64_Phdr *pphdr = elf->data + eehdr->e_phoff;
  //  uint64_t original_entrypoint = eehdr->e_entry; // Save the original entry point

    char *new_data = malloc(new_size);
    if (!new_data) {
        print_error("Failed to allocate memory");
    }

    // Copy original ELF file data to new memory
    memcpy(new_data, elf->data, elf->filesize);

    // Find the insertion point (end of the first PT_LOAD segment)
    uint64_t g_code_entrypoint = current_phdr->p_offset + current_phdr->p_filesz;
    
  //  eehdr->e_entry = current_phdr->p_vaddr + current_phdr->p_memsz + MSG_SIZE;

    // Insert the new code at the calculated position
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)new_data;
    Elf64_Phdr *phdr = (Elf64_Phdr *)new_data + ehdr->e_phoff;
    ehdr->e_entry = current_phdr->p_vaddr + current_phdr->p_memsz + MSG_SIZE;
    char *loader = new_data + g_code_entrypoint;
    memcpy(loader, _dataload, DATALOAD_SIZE);
    loader += DATALOAD_SIZE;

    // Add the jump instruction to return to the original entry point
 //   char jmp = 0xe9;
  //  memcpy(loader, &jmp, sizeof(jmp));
 //   loader += sizeof(jmp);

  //  int jmp_addr = original_entrypoint - (phdr->p_vaddr + phdr->p_memsz + TOTAL_DATALOAD_SIZE);
 //   memcpy(loader, &jmp_addr, sizeof(int));

    // Update the code segment size in the ELF program header
    phdr->p_filesz += TOTAL_DATALOAD_SIZE;
    phdr->p_memsz += TOTAL_DATALOAD_SIZE;
      *((uint32_t *)(new_data + 0x108)) = 0x01a9;
    // Offset 0x110: changing 0x0179 to 0x01a9
    *((uint32_t *)(new_data + 0x110)) = 0x01a9;

    // Write the modified ELF file to a new file
    FILE *output = fopen("modified_file", "wb");
    if (!output) {
        print_error("Failed to open output file");
    }

    fwrite(new_data, 1, new_size, output);
    fclose(output);

    free(new_data);
}
