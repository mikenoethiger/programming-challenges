#include <stdio.h>
#include <math.h>

typedef unsigned long long_t;


void solve(long_t n) {
	// according to https://primes.utm.edu/glossary/page.php?sort=tau
	// the number of divisors tau(n) is:
	// tau(n) = (e1+1)(e2+1)(e3+1) ... (ek+1)
	// with e1, e1, ..., ek being the exponent of each
	// prime factor of n
	// So we can simply count the prime factors and multiply
	// these (counts+1) with each other.
	long_t i;
	long_t d = 1;
	long_t cnt = 0;

	// find even divisors
	// O(log n)
	while (n % 2 == 0) {
		n /= 2;
		cnt++;
	}
	d *= (cnt+1);

	i = 3;
	cnt = 0;
	while (i < sqrt(n)+1) {
		while (n % i == 0) {
			cnt++;
			n /= i;
		}
		if (cnt > 0) {
			d *= (cnt+1);
		}
		i += 2;
		cnt = 0;
	}

	// i.e. remaining n is prime
	if (n > 1) d *= 2;

	if (d % 2 == 0) printf("no\n");
	else printf("yes\n");
}

int main() {
	long_t n;

	while (scanf("%ld", &n) != EOF && n) {
		solve(n);
	}
	return 0;
}