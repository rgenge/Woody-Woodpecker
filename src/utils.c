#include "woody.h"

extern elf_t			*elf;
extern injector		*inj;
extern bool				elf_alloc;
extern bool				elf_data_alloc;
extern bool				inj_alloc;
extern bool				inj_data_alloc;

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

bool ft_isprint(const char c)
{
	return c >= 31 && c <= 126;
}

void	hex_dump(void* address, size_t amount)
{
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
			if (ft_isprint(*(h + o)))
				printf("%c", *(h + o));
			else if (!*(h + o))
				printf(" ");
			else
				printf(".");
		}
		printf("\r");
		fflush(stdout);
		if (FUN)
			usleep(168);
	}
	printf("\n");
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
