#include <stdio.h>
#include <stdlib.h>

int
main() {
	int *pointer = malloc(sizeof(int));
	*pointer = 42;
	printf("--> %d <--", *pointer);
	return (*pointer);
}
