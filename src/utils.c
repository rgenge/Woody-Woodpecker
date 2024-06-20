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

int	ft_strncmp(const char *s1, const char *s2, size_t size)
{
	if (size == 0)
		return (0);
	while (*s1 != '\0' && *s2 != '\0' && size - 1 > 0)
	{
		if (*s1 != *s2)
		{
			break ;
		}
		s1++;
		s2++;
		size--;
	}	
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

void	*ft_memcpy(void *dest, const void *src, size_t size)
{
	unsigned char	*dest_aux;
	unsigned char	*src_aux;
	size_t			index;

	if (!dest && !src)
		return (NULL);
	dest_aux = (unsigned char *)dest;
	src_aux = (unsigned char *)src;
	index = 0;
	while (index < size)
	{
		*dest_aux = *src_aux;
		dest_aux++;
		src_aux++;
		index++;
	}
	return (dest);
}