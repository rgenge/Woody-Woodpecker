#ifndef WOODY_H
#define WOODY_H

# include <elf.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <ctype.h>

# define __debug true

typedef struct		s_elf
{
	char			*data; // raw bin content
	Elf64_Ehdr*		ehdr;
	uint64_t		*key;
	ssize_t			filesize;
	int				decrypt;
	unsigned char	bit_class;
}					t_elf;	  

off_t	get_filesize(int fd);
void	read_file(char *filename);
void	die(char* message);
void	*ft_memset(void *s, unsigned char c, size_t size);
void	hex_dump(void* address, size_t amount);
void	hex_byte(void* address, size_t amount);
void	lin_dump(void* address, size_t offset);
void	say(char* msg, void* data, char type);
void	*ft_calloc(size_t count, size_t size);
void	ft_bzero(void *s, size_t n);
void	pretty_print(t_elf*);

# define ___die(_bool, _msg) if (_bool) die(_msg)
# define bool char
# define false 0
# define true !false
# define ___deb if (__debug) 
# define ___br printf("\n")

#endif
