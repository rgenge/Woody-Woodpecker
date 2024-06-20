#include "woody.h"

extern dumpster		*elf;
extern injector		*inj;
extern void*			file_out;
extern bool				elf_alloc;
extern bool				elf_data_alloc;
extern bool				inj_alloc;
extern bool				inj_data_alloc;
extern bool				inj_bin_alloc;
extern bool				file_out_alloc;

off_t	get_filesize(int fd)
{
	off_t result = lseek(fd, 0, SEEK_END);
	if (result == -1 || lseek(fd, 0, SEEK_SET) == -1) {
		return -1;
	}
	return result;
}

void	*ft_memset(void *s, unsigned char c, size_t size)
{
	unsigned char	*p;
	p = s;
	while (size--)
		*p++ = c;
	return (s);
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
				printf("\033[38;5;240m");
			printf("%02x ", *(h + o) & 0xFF);
			if (!(*((char*)h + o) & 0xFF))
				printf("\033[0m");
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
}

void	hex_byte(void* address, size_t amount)
{
	char*	h; // head
	if (!address)
		return (void)printf("Dumping address nil, no dump.");
	for (size_t i = 0; i < amount; i += 8)
	{
		h = (char*)(address + i);
		for (size_t o = 0; o < 8 && o < amount; o++) // offset
		{
			if (!(*((char*)h + o) & 0xFF))
				printf("\033[38;5;240m");
			printf("%02x ", *(h + o) & 0xFF);
			if (!(*((char*)h + o) & 0xFF))
				printf("\033[0m");
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
}

void	byte_is(void* h, long long test, const char* msg)
{	if (*(unsigned char*)h == test) printf("%s", msg); }

void	true_is(long long a, long long b, const char* msg)
{	if (a == b) printf("%s", msg); }

void	condition_msg(long long cond, const char* msg)
{	if (cond) printf("%s", msg); }

void	flag_is(long long a, long long b, const char* msg)
{	if (a & b) printf("%s", msg); }

void	hex_pure(void* h, size_t amount)
{
	if (!h)
		return (void)printf("Dumping address nil, no dump.");
	for (size_t i = 0; i < amount; i += 8)
	{
		h = (char*)(h + i);
		for (size_t o = 0; o < 8 && o < amount; o++) // offset
		{
			if (!(*((char*)h + o) & 0xFF))
				printf("\033[38;5;240m");
			printf("%02x ", *((char*)h + o) & 0xFF);
			if (!(*((char*)h + o) & 0xFF))
				printf("\033[0m");
		}
	}
}

void	hex_msg(void* h, size_t amount, const char* msg)
{
	hex_pure(h, amount);
	printf("%s", msg);
}

void	lin_dump(void* address, size_t amount, size_t line_break)
{
	char*	h; // head
	bool	n; // non-printable printed
	char	c;
	size_t	col;

	if (!address)
		return (void)printf("Dumping address nil, no dump.");
	col = 0;
	for (size_t i = 0; i < amount; i += 8)
	{
		h = (char*)(address + i);
		for (size_t o = 0; o < 8; o += 1) // offset
		{
			c = *(h + o);
			if (!isprint(c) || c == ' ')
			{
				if (!n)
				{
					printf(" ");
					n = true;
					col++;
				}
			}
			else
			{
				printf("%c", c);
				n = false;
				col++;
			}
			if (line_break && col >= line_break)
			{
				col = 0;
				printf("\n");
			}
		}
	}
}

void say(char* msg, void* data, char type)
{
	if (type == 'i') // int
		printf("%s %d\n", msg, *(int*)data);	
	if (type == 'u') // utin32_t
		printf("%s %d\n", msg, *(uint32_t*)data);	
	if (type == 's') // short unsigned int
		printf("%s %d\n", msg, *(short unsigned int*)data);	
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

void	M(size_t offset, size_t c)
{
	void *h;
	h = ft_memcpy(file_out + offset, elf->data + offset, c);
	___die(!h, "Failed to copy memory chunk.");
}

void	file_out_to_file(const char *woody)
{
	int		fd;
	fd = open(woody, O_WRONLY | O_CREAT, 00755);
	___die (fd == -1, "Failed to create `woody`. File exists?");
	___die (write(fd, file_out, elf->data_size) == -1, "Could not write to file.");
	close(fd);
}
