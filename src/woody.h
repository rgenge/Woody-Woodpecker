#ifndef WOODY_H
# define WOODY_H

# include <elf.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <ctype.h>
# include <sys/mman.h>

# define __debug true

typedef struct	elf_is_such_a
{
	char					*data; // raw bin content
	uint32_t			data_size;
	unsigned char	bit_class; // 32 || 64 bit
	uint32_t			phnum; // variable loc
	uint32_t			shnum; // variable loc
	uint32_t			shstrndx; // variable loc
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

typedef struct	injection
{
	dumpster			troll;
	char					*bin;
	uint32_t			bin_size;
	Elf64_Addr		prev_entry;
//	union {
//		Elf32_Phdr	*_32;
//		Elf64_Phdr	*_64;
//	}							ph;
}								injector;


off_t	get_filesize(int fd);
void	read_original_elf(char *filename);
void	*ft_memset(void *s, unsigned char c, size_t size);
void	hex_dump(void* address, size_t amount);
void	hex_byte(void* address, size_t amount);
void	lin_dump(void* address, size_t offset, size_t line_break);
void	say(char* msg, void* data, char type);
void	byte_is(void* h, long long test, const char* msg);
void	true_is(long long a, long long b, const char* msg);
void	condition_msg(long long, const char* msg);
void	flag_is(long long a, long long b, const char* msg);
void	hex_pure(void* h, size_t amount);
void	hex_msg(void* h, size_t amount, const char* msg);
void	*ft_memcpy(void *dst, const void *src, size_t n);

# define bool char
# define false 0
# define true !false
# define ___die(_bool, _msg) if (_bool) die(_msg)
void	die(char* message);

# define ___deb if (__debug) 
# define ___br printf("\n")
# define ___debp(x, y) if (__debug) { printf(x "", y); };

# define _E32 elf->ehdr._32
# define _P32 elf->phdr._32
# define _S32 elf->shdr._32
# define _E64 elf->ehdr._64
# define _P64 elf->phdr._64
# define _S64 elf->shdr._64

#endif
