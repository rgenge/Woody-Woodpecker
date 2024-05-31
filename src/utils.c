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
			printf("%02x ", *(h + o) & 0xFF);
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
			printf("%02x ", *(h + o) & 0xFF);
		for (size_t o = 0; o < 8 && o < amount; o += 1)
		{
			if (isprint(*(h + o)))
				printf("%c", *(h + o));
			else if (!*(h + o))
				printf(" ");
			else
				printf(".");
		}
	}
}

void	lin_dump(void* address, size_t amount)
{
	char*	h; // head
	bool	n; // non-printable printed
	char	c;

	if (!address)
		return (void)printf("Dumping address nil, no dump.");

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
