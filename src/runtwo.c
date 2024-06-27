#include "woody.h"
#include <errno.h>
#include <string.h>

void	hex_byte(void* address, size_t amount)
{
	char* COLORMODE = getenv("COLORMODE");
	char*	h; // head
	size_t a = amount;
	if (a > 80)
		a = 80;
	if (!address)
		return (void)printf("No address.");
	for (size_t i = 0; i < a; i += 8)
	{
		h = (char*)(address + i);
		for (size_t o = 0; o < 8 && o < a; o++) // offset
		{
			if (!(*((char*)h + o) & 0xFF))
				if (COLORMODE) printf("\033[38;5;240m");
			printf("%02x ", *(h + o) & 0xFF);
			if (!(*((char*)h + o) & 0xFF))
				if (COLORMODE) printf("\033[0m");
		}
		for (size_t o = 0; o < 8 && o < a; o += 1)
		{
			if (isprint(*(h + o)))
				printf("%c", *(h + o));
			else if (!*(h + o))
				printf(" ");
			else
				printf(".");
		}
		printf("\n");
	}
	if (amount > 100)
		printf("...(%ld)\n", amount);
}

size_t	get_filesize(int fd)
{
	size_t result = lseek(fd, 0, SEEK_END);
	if (result == -1
			|| lseek(fd, 0, SEEK_SET) == -1) {
		return -1;
	}
	return result;
}

//#define MYELF "/bin/ls"
#define MYELF "../samples/sample"

int main(int argc, char** argv)
{
	int fd = open(MYELF, O_RDONLY);
	if (fd == -1)
	{
		printf("1st instance, %d\n", errno);
		return 1;
	}
	size_t size1 = get_filesize(fd);
	printf("size1 %d\n", size1);

	void* file1 = malloc(size1);
	read(fd, file1, size1);
	printf("file1\n");
	hex_byte(file1, size1);

	Elf64_Ehdr* elf_header = (Elf64_Ehdr*)file1;
	Elf64_Addr entry_point = elf_header->e_entry;

	void* cpu_vm = mmap(NULL, size1,
		PROT_READ | PROT_WRITE | PROT_EXEC,
		MAP_PRIVATE | MAP_ANONYMOUS, fd, 0);

	if (cpu_vm == MAP_FAILED)
	{
		int out = errno;
		printf("%p) errno %d\n", cpu_vm, out);
		munmap(file1, size1);
		free(file1);
		return out;
	}

	printf("Anon map done: %p -> %p\n", cpu_vm, (long long)&cpu_vm);
	hex_byte(cpu_vm, size1); // Empty

	printf("memcpy(%p, %p, %ld):\n", cpu_vm, file1, size1);
	memcpy(cpu_vm, file1, size1);
	hex_byte(cpu_vm, size1);

	size_t entry = ((Elf64_Ehdr*)cpu_vm)->e_entry;
	printf("Entry %d\n", entry);
	hex_byte(cpu_vm + entry, size1);

	void* (*e_point)(int, char**);
	e_point = ((void*)cpu_vm) + entry_point;
	printf("e_point %p\n", e_point);
	hex_byte(e_point, 80);

//	argv[0] = MYELF;
//	void* return_from_exec = (*e_point)(argc, argv);

	free(file1);
	close(fd);
	return 0;
}
