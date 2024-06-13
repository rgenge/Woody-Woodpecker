#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <fcntl.h>

# define bool char
# define false 0
# define true !false
# define ___die(_bool, _msg) if (_bool) die(_msg)

void	lin_dump(void* address, size_t amount, size_t print_width)
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
			if (print_width && col >= print_width)
			{
				col = 0;
				printf("\n");
			}
		}
	}
}

void close_all_fds()
{
	for (size_t i = 3; i < 1024; i++)
		close(i);
}

void die(char* message)
{
	close_all_fds();
	perror(message);
	exit(1);
}

off_t	get_filesize(int fd)
{
	off_t result = lseek(fd, 0, SEEK_END);
	return (result == -1 || lseek(fd, 0, SEEK_SET) == -1) ?
		 -1 : result;
}

int		main(int argc, char** argv)
{
	___die(argc != 2, "Use `lindump file`.");
	int fd = open(argv[1], O_RDONLY);
	___die (fd == -1, "Failed to open file");
	off_t size = get_filesize(fd);
	___die (size == -1, "Failed to get file size");
	void*	h = malloc(size);
	___die (!h, "Malloc went wrong.");
	ssize_t bytes = read(fd, h, size);
	___die (!bytes, "No bytes read. Is file empty?");
	___die (bytes == -1, "Error reading file");
	close_all_fds();	
	lin_dump(h, size, 0);
	return 0;
}
