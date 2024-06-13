#include "woody.h"

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
