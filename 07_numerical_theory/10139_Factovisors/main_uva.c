#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define FACTORS_SIZE 10000

typedef unsigned long long_t;

long_t n_factorial_powers(n, p) {
	long_t sum = 0;
	long_t power = p;
	while (power <= n) {
		sum += n / power;
		power *= p;
	}
	return sum;
}

void factorize(long_t n, long_t *factors, long_t *n_factors) {
	long_t k = 0;
	long_t i;
	while (n % 2 == 0) {
		factors[k++] = 2;
		n /= 2;
	}

	i = 3;
	while (i < sqrt(n)+1) {
		if (n % i == 0) {
			factors[k++] = i;
			n /= i;
		} else {
			i += 2;
		}
	}

	if (n > 1) factors[k++] = n;

	*n_factors = k;
}

long_t factors[10000];
long_t n_factors;

bool factovisor(long_t n, long_t m) {
	if (n == 0) n = 1;
	if (m == 0) return true;
	if (m == 1) return true;
	if (m < n) return true;
	if (n % m == 0) return true;

	factorize(m, factors, &n_factors);
	long_t lp, i, cnt;
	lp = factors[0];
	cnt = 1;
	for (i = 1; i < n_factors; i++) {
		if (factors[i] == lp) {
			cnt++;
		} else {
			if (cnt > n_factorial_powers(n, lp)) return false;
			lp = factors[i];
			cnt = 1;
		}
	}
	if (cnt > n_factorial_powers(n, lp)) return false;

	return true;
}

void solve(long_t n, long_t m) {
	if (factovisor(n, m)) {
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