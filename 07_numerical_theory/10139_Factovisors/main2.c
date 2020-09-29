#include <stdio.h>

typedef unsigned long long_t;

/*
 * O(n)
 */
long_t factovisor(long_t n, long_t m) {
	if (m == 0) return 0;
	if (n == 0) n = 1;
	// calculates n! mod m
	long_t res = n;
	size_t i;
	for (i = n-1; i > 1; i--) {
		n = (n * i) % m;
	}
	return n % m;
}

void solve(long_t n, long_t m) {
	if (factovisor(n, m) == 0) {
		printf("%ld divides %ld!\n", m, n);
	} else {
		printf("%ld does not divide %ld!\n", m, n);
	}
}

int main() {
	long_t n, m;

	while (scanf("%ld %ld", &n, &m) != EOF) {
		solve(n, m);
	}
	return 0;
}