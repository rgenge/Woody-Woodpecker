#include "woody.h"

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

void	hex_dump(void* address, size_t offset)
{
	char*	h; // head

	printf("%p\n", address);
	for (size_t i = 0; i < offset; i += 8)
	{
		h = (char*)(address + i);
		printf("%05ld ", (void *)h - address);
		for (size_t o = 0; o < 8; o++) // offset
			printf("%02x ", *(h + o) & 0xFF);
		for (size_t o = 0; o < 8; o += 1)
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

void	lin_dump(void* address, size_t offset)
{
	char*	h; // head
	bool	n; // non-printable printed
	char	c;

	for (size_t i = 0; i < offset; i += 8)
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
				}
			}
			else
			{
				printf("%c", c);
				n = false;
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

void	*ft_calloc(size_t count, size_t size)
{
	void	*c;

	c = malloc(count * size);
	if (!c)
		return (NULL);
	ft_bzero(c, count * size);
	return (c);
}

void	ft_bzero(void *s, size_t n)
{
	char	*p;

	p = s;
	while (n--)
	{
		*p = 0;
		p++;
	}
	return ;
}
