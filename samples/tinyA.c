#include <unistd.h>

int main() {
	char A;
	A = 'A';
	write(1, &A, 1);
}
