#ifndef WOODY_H
# define WOODY_H

# include <elf.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <ctype.h>
# include <sys/mman.h>

# define __debug false

# ifndef FUN
#  define FUN 1
# endif
# ifndef COLORMODE
#  define COLORMODE 1
# endif

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
	char					*data;
	char					*bin;
	uint32_t			data_size;
	uint32_t			bin_size;
}								injector;

off_t	get_filesize(int fd);
void	read_original(char *filename, dumpster**);
void	*ft_memcpy(void *dst, const void *src, size_t n);
void	validate_file();
int		free_all();
void  read_original_elf(char *filename);
void	read_blob(const char *filename);
void	M(size_t offset, size_t c);
void	file_out_to_file(const char*, const char*, uint32_t);
void	hex_dump(void* address, size_t amount);
int		ft_stridentical(const char *s1, const char *s2);

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
