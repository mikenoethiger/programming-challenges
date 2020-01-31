#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define FACTORS_SIZE 10000

typedef unsigned long long_t;

/**
 * Calculates the number of occurrences of p
 * in the prime factorization of n!
 *
 * O(logp(n))
 *
 * @return
 */
long_t n_factorial_powers(n, p) {
	// Given n, p, count all numbers from 1..n which are
	// divisible by p:
	//     floor(n/p)
	// which is basically just the series:
	//     p
	//     p+p
	//     p+p+p
	// up to the point where p is bigger than n.
	// All of these numbers are obviously divisible
	// by p, and there are floor(n/p) of these.
	// Now we do this for p, p^2, p^3, ..., until
	// p^k > n and sum them all up
	long_t sum = 0;
	long_t power = p;
	while (power <= n) {
		sum += n / power;
		power *= p;
	}
	return sum;
}

/* O(sqrt(n)) */
void factorize(long_t n, long_t *factors, long_t *n_factors) {
	long_t k = 0;
	long_t i;

	// find even divisors
	while (n % 2 == 0) {
		factors[k++] = 2;
		n /= 2;
	}

	// find odd divisors
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
	if (n == 0) n = 1; // 0! = 1, 1! = 1 => 0! <=> 1!
	if (m == 0) return true; // 0 divides no number
	if (m == 1) return true; // 1 divides every number
	if (m < n) return true; // m must be in the sequence of n!
	if (n % m == 0) return true; // at least one factor of n! is divisible by m (namely n)

	// ensure that no prime factor of m occurs more
	// in the prime factorization of m than in the
	// prime factorization of n!
	factorize(m, factors, &n_factors);
	long_t lp, i, cnt;
	lp = factors[0];
	cnt = 1;
//	printf("%ld ", factors[0]);
	for (i = 1; i < n_factors; i++) {
//		printf("%ld ", factors[i]);
		if (factors[i] == lp) {
			cnt++;
		} else {
			// factor change
			if (cnt > n_factorial_powers(n, lp)) return false;
			lp = factors[i];
			cnt = 1;
		}
	}
//	printf("\n");
//	printf("i=%ld\n", i);
//	printf("n_factors=%ld\n", n_factors);
//		printf("nfp(%ld,%ld)=%ld\n", n, lp, n_factorial_powers(n, lp));
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