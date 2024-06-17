#include <unistd.h>

int main() {
	char B;
	B = 'B';
	write(1, &B, 1);
}
