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

void	file_out_to_file(const char *woody, const char* data_block, uint32_t size)
{
	int		fd;
	fd = open(woody, O_WRONLY | O_CREAT, 00755);
	___die (fd == -1, "Failed to create `woody`. File exists?");
	___die (write(fd, file_out, size) == -1, "Could not write to file.");
	close(fd);
}

void	read_blob(char *filename)
{
	int				fd;
	off_t			filesize;
	long int	bytes_read;
	fd = open(filename, O_RDONLY);
	___die (fd == -1, "Failed to open file");
	filesize = get_filesize(fd);
	___die ((int)filesize == -1, "Failed to get file size");
	inj = calloc(sizeof(injector), 1);
	___die (!inj, "Failed to create injector.");
	inj_alloc = true;
	inj->bin_size = (uint32_t)filesize;
	inj->bin = calloc(1, inj->bin_size);
	___die (!inj->bin, "Could not allocate injection portion.");
	inj_bin_alloc = true;
	bytes_read = read(fd, inj->bin, filesize);
	___die (!bytes_read, "No bytes read. Is file empty?");
	___die (bytes_read == -1, "Error reading file");
}
