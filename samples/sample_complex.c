#include <stdio.h>
#include <stdlib.h>

int
main() {
	int *pointer = malloc(sizeof(int));
	*pointer = 42;
	printf("--> %p %d <--", pointer, *pointer);
	return (*pointer);
}
