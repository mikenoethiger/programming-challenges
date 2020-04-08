#include <stdio.h>
#include <math.h>
#include <stdbool.h>

typedef unsigned long long_t;

void solve(long_t a, long_t b) {
	long_t exp = 1;
	long_t tmp = a;
	long_t result = 1;

	while (b > 0) {
		if (b & 1) {
			result *= tmp;
			result %= 10;
		}
		tmp *= tmp;
		tmp %= 10;
		b = b >> 1;
	}

	printf("%ld\n", result);
}

int main() {

	long_t cases, i, n;
	scanf("%ld\n", &cases);
	long_t a,b;

	for (i = 0; i < cases; i++) {
		scanf("%ld", &a);
		scanf("%ld", &b);
		solve(a, b);
	}

	return 0;
}