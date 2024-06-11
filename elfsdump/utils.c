#include "elfsdump.h"

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

void	hex_byte(void* address, size_t amount)
{
	char*	h; // head
	size_t a = amount;
	if (a > 80)
		a = 80;
	if (!address)
		return (void)printf(NO_ADDRESS);
	for (size_t i = 0; i < a; i += 8)
	{
		h = (char*)(address + i);
		for (size_t o = 0; o < 8 && o < a; o++) // offset
		{
			if (!(*((char*)h + o) & 0xFF))
				printf("\033[38;5;240m");
			printf("%02x ", *(h + o) & 0xFF);
			if (!(*((char*)h + o) & 0xFF))
				printf("\033[0m");
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
		if (amount > 100)
			printf("...(%ld)\n", amount);
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
