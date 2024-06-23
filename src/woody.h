#ifndef WOODY_H
#define WOODY_H
# include <elf.h>
# include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

typedef struct	s_elf
{
	uint64_t	*key;
	void		*data;
	Elf64_Phdr	*code;
	Elf64_Shdr	*text;
	Elf64_Ehdr	ehdr;
	long unsigned int		filesize;
	int			decrypt;
}               t_elf;	  

off_t    		get_filesize(int fd);
void            read_file(t_elf *elf, char *filename);
void            print_error(char* message);
void            *ft_memset(void *s, int c, size_t size);
int				ft_strncmp(const char *s1, const char *s2, size_t size);
void			*ft_memcpy(void *dest, const void *src, size_t size);
void 			insert_woody(t_elf *elf, Elf64_Phdr *phdr) ;
#endif