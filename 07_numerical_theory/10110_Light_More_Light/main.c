#include <stdio.h>
#include <math.h>

void solve(int n) {
	int c;
	int i;
	c = 0;
	for (i = 1; i <= n; i++) {
		if (n % i == 0) c++;
	}
	if (c % 2 == 0) printf("no\n");
	else printf("yes\n");
}

void solve2(unsigned long n) {
	unsigned long initial_n = n;
	int d = 0;
	size_t i;

	// according to https://primes.utm.edu/glossary/page.php?sort=tau
	// the number of divisors tau(n) is:
	// tau(n) = (e1+1)(e2+1)(e3+1) ... (ek+1)
	// with e1, e1, ..., ek being the exponent of each
	// prime factor of n
	// So we can simply count the prime factors and multiply
	// these (counts+1) with each other.
	int divisors = 1;
	int cnt = 0;
	int pivot;


	// find even divisors
	// O(log n)
	while (n % 2 == 0) {
		n /= 2;
		cnt++;
	}
	divisors *= (cnt+1);

	i = 3;
	cnt = 0;
	pivot = i;
	while (i < sqrt(n)+1) {
		for (;n % i == 0; cnt++, n/= i){}
		if (cnt > 0) {
			divisors *= (cnt+1);
		}
		i += 2;
		cnt = 0;
	}

	// i.e. remaining n is prime
	if (n > 1) divisors *= 2;

//	printf("tau(%ld)=%d\n", initial_n, divisors);
	if (divisors % 2 == 0) printf("no\n");
	else printf("yes\n");
}

int main() {
	unsigned long n;

	while (scanf("%ld", &n) && n) {
		solve2(n);
	}
}