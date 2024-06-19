#include "woody.h"

extern dumpster	elf; // Yes, global.
extern injector inj;

void close_all_fds()
{
	for (size_t i = 3; i < 1024; i++)
		close(i);
}

void die(char* message)
{
	close_all_fds();
	free(elf.data);
	free(inj.bin);
	printf("Controlled error:\n");
	perror(message);
	exit(1);
}
