#ifndef WOODY_H
#define WOODY_H

# include <elf.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <ctype.h>

typedef struct	s_elf
{
	uint64_t	*key;
	char		*data; // raw bin content
	Elf64_Phdr	*code;
	Elf64_Shdr	*text;
	ssize_t		filesize;
	int			decrypt;
}               t_elf;	  

off_t	get_filesize(int fd);
void	read_file(t_elf *elf, char *filename);
void	die(char* message);
void	*ft_memset(void *s, unsigned char c, size_t size);
void	hex_dump(void* address, size_t offset);
void	lin_dump(void* address, size_t offset);
void	say(char* msg, void* data, char type);

# define ___die(_bool, _msg) if (_bool) die(_msg)
# define bool char
# define true 1
# define false 0

#endif
