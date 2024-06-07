#ifndef ELFSDUMP_H
# define ELFSDUMP_H

# include <elf.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <ctype.h>

# define bool char
# define false 0
# define true !false
# define ___die(_bool, _msg) if (_bool) die(_msg)
# define ___br printf("\n");

# define BAD_ARGUMENTS \
	"Just point me towards the nearest mushroom ring, would you? " \
	"This isn't the time for riddles. Just tell me, birch bark or glowing moss?\n:::::"
# define NO_ADDRESS \
  "An elf toilet with no address! Perhaps they expect telepathic directions?"
# define ELF_DUMP \
	"\U0001F9DD\U0001F4A9 "

typedef struct	elf_is_such_a
{
	char					*data; // raw bin content
	unsigned char	bit_class; // 32 || 64 bit
	union {
		Elf32_Ehdr	*_32;
		Elf64_Ehdr	*_64;
	}							ehdr;
	union {
		Elf32_Phdr	*_32;
		Elf64_Phdr	*_64;
	}							phdr;
	union {
		Elf32_Shdr	*_32;
		Elf64_Shdr	*_64;
	}							shdr;
}								dumpster;

void	hex_byte(void* address, size_t amount);
void	byte_is(void* h, long long test, const char* msg);
void	true_is(long long a, long long b, const char* msg);
void	condition_msg(long long cond, const char* msg);
void	flag_is(long long a, long long b, const char* msg);
void	hex_pure(void* h, size_t amount);
void	hex_msg(void* h, size_t amount, const char* msg);
void	hex_dump(void* address, size_t amount);
void	hex_byte(void* address, size_t amount);
void	byte_is(void* h, long long test, const char* msg);
void	true_is(long long a, long long b, const char* msg);
void	condition_msg(long long cond, const char* msg);
void	flag_is(long long a, long long b, const char* msg);
void	hex_pure(void* h, size_t amount);
void	hex_msg(void* h, size_t amount, const char* msg);
void	hex_dump(void* address, size_t amount);















#endif
