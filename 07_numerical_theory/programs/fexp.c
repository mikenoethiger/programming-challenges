#include <stdio.h>
#include <stdlib.h>

/*
 * A program that calculate a^x or a^x mod m in O(log n)
 */

void print_usage() {
	printf("Usage: fexp a x [m]\n");
	printf("\n");
	printf("    a := base\n");
	printf("    x := exponent\n");
	printf("    b := modular divisor(optional)\n");
	printf("DESCRIPTION\n");
	printf("    Calculates a^x [mod m] in O(log x)\n");
}

long fexp(int a, unsigned int x, int *n) {
	long res = 1;
	*n = 0;
	while (x > 0) {
		// if LSB is a 1
		if (x & 1) {
			res *= a;
			(*n)++;
		}
		x >>= 1; // shift one bit right / divide by 2
		a *= a;
		(*n)++;
	}
	return res;
}

long fexpm(int a, int x, int m, int *n) {
	long res = 1;
	*n = 0;
	while (x > 0) {
		// if LSB is a 1
		if (x & 1) {
			res = (res * a) % m;
			(*n)++;
		}
		x >>= 1; // shift one bit right / divide by 2
		a = (a * a) % m;
		(*n)++;
	}
	return res;
}

int main(int argc, char *argv[]) {
	if (argc < 3 || argc > 4) {
		print_usage();
		return -1;
	}

	int a, x, m, n;
	size_t res;
	a = atoi(argv[1]);
	x = atoi(argv[2]);
	m = 0;
	if (argc == 4) m = atoi(argv[3]);

	if (m == 0) {
		res = fexp(a, x, &n);
	} else {
		res = fexpm(a, x, m, &n);
	}

	printf("%d multiplications executed\n", n);
	if (m == 0) {
		printf("%d^%d = %ld\n", a, x, res);
	} else {
		printf("%d^%d mod %d = %ld\n", a, x, m, res);
	}


	return 0;
};