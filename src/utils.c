#include "woody.h"

extern dumpster		*elf;
extern injector		*inj;
extern bool				elf_alloc;
extern bool				elf_data_alloc;
extern bool				inj_alloc;
extern bool				inj_data_alloc;
extern bool				inj_bin_alloc;

off_t	get_filesize(int fd)
{
	off_t result = lseek(fd, 0, SEEK_END);
	if (result == -1 || lseek(fd, 0, SEEK_SET) == -1) {
		return -1;
	}
	return result;
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char				*d;
	const char	*s;
	if ((!dst) && (!src))
		return (NULL);
	d = dst;
	s = src;
	while (n--)
		*d++ = *s++;
	return (dst);
}

void	file_out_to_file(const char *woody, const char* data_block, uint32_t size)
{
	int		fd;
	fd = open(woody, O_WRONLY | O_CREAT, 00755);
	___die (fd == -1, "Failed to create `woody`. File exists?");
	___die (write(fd, data_block, size) == -1,
				  "Could not write to file.");
	close(fd);
}

void	read_blob(const char *filename)
{
	int				fd;
	off_t			filesize;
	long int	bytes_read;
	fd = open(filename, O_RDONLY);
	___die (fd == -1, "Failed to open file");
	filesize = get_filesize(fd);
	___die ((int)filesize == -1, "Failed to get file size");
	inj = calloc(sizeof(injector), 1);
	___die (!inj, "Failed to create injector.");
	inj_alloc = true;
	inj->bin_size = (uint32_t)filesize;
	inj->bin = calloc(1, inj->bin_size);
	___die (!inj->bin, "Could not allocate injection portion.");
	inj_bin_alloc = true;
	bytes_read = read(fd, inj->bin, filesize);
	___die (!bytes_read, "No bytes read. Is file empty?");
	___die (bytes_read == -1, "Error reading file");
//	inj->data_size = elf->data_size + inj->bin_size;
	inj->data_size = elf->data_size; // Dont'd expand space, use padding.
	inj->data = calloc(inj->data_size, 1);
	___die(!inj->data, "Failed to prepare injected alloc block.");
	inj_data_alloc = true;
}

void	hex_dump(void* address, size_t amount)
{
	char* COLORMODE = getenv("COLORMODE");
	char*	h; // head
	if (!address)
		return (void)printf("Dumping address nil, no dump.");
	printf("%p\n", address);
	for (size_t i = 0; i < amount; i += 8)
	{
		h = (char*)(address + i);
		printf("%05ld ", (void *)h - address);
		for (size_t o = 0; o < 8 && o < amount; o++) // offset
		{
			if (!(*((char*)h + o) & 0xFF))
				if (COLORMODE) printf("\033[38;5;240m");
			printf("%02x ", *(h + o) & 0xFF);
			if (!(*((char*)h + o) & 0xFF))
				if (COLORMODE) printf("\033[0m");
		}
		for (size_t o = 0; o < 8 && o < amount; o += 1)
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
};

int	ft_stridentical(const char *s1, const char *s2)
{
	char	*a;
	char	*b;
	a = (char *)s1;
	b = (char *)s2;
	while (a && b && *a && *b)
	{
		if (*a != *b)
			return (0);
		a++;
		b++;
	}
	if (!*a && !*b)
		return (1);
	return (0);
}
