#include "woody.h"

off_t    get_filesize(int fd) {

    off_t result = lseek(fd, 0, SEEK_END);

    if (result == -1 || lseek(fd, 0, SEEK_SET) == -1) {
        return -1;
    }
    return result;
}

void	*ft_memset(void *s, int c, size_t size)
{
	unsigned char	*p;

	p = s;
	while (size--)
		*p++ = (unsigned char)c;
	return (s);
}