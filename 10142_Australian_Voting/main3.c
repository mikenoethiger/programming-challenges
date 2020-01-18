#include <stdio.h>

int main() {
	int a, b;
	a = scanf("%d", &b);
	printf("a=%d\n", a);
	printf("a==EOF: %d\n", a == EOF);
}