#include <stdio.h>
#include <math.h>

typedef unsigned long long_t;


void solve(long_t n) {
	// the bulb will be off if n has an even number of divisors
	// and on if it has an odd number of divisors.
	// Every divisor that is smaller than sqrt(n) implies another
	// divisor bigger than sqrt(n).
	// The matchmaker is sqrt(n); if it is a natural number, n has
	// an odd number of divisors, otherwise the number is even.
	double p_square = sqrt(n);
	if (p_square - (long_t) p_square > 0) {
		printf("no\n");
	} else {
		printf("yes\n");
	}
}

int main() {
	long_t n;

	while (scanf("%ld", &n) != EOF && n) {
		solve(n);
	}
	return 0;
}