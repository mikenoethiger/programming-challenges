#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

/*
 * A program to calculate the number of divisors of a
 * given number n, also called tau(n).
 */

void print_usage() {
	printf("Usage: tau n1 [n2 ...]\n");
	printf("    ni := number to calculate tau(n)\n");
	printf("\n");
	printf("DESCRIPTION\n");
	printf("    Calculates the number of divisors of n, also called the tau(n) function.\n");
	printf("    tau(p)=2 if p is a prime number.\n");
	printf("    Otherwise tau(n) = (e1+1)(e2+1)(e3+1) ... (ek+1) with e1, e2, ..., ek being the exponents of the prime factors of n\n");
	printf("    Perfectly explained at https://primes.utm.edu/glossary/page.php?sort=tau\n");
	printf("\n");
	printf("AUTHORS\n");
	printf("    Mike Nöthiger (noethiger.mike@gmail.com)\n");
}

size_t tau(unsigned long n) {
	// according to https://primes.utm.edu/glossary/page.php?sort=tau
	// the number of divisors tau(n) is:
	// tau(n) = (e1+1)(e2+1)(e3+1) ... (ek+1)
	// with e1, e1, ..., ek being the exponent of each
	// prime factor of n
	// So we can simply count the prime factors and multiply
	// these (counts+1) with each other.
	int d = 1;
	size_t i;
	int cnt = 0;


	// count even divisors
	// O(log n)
	for(;n%2 == 0; n/=2, cnt++) {}
	d *= (cnt+1);

	i = 3;
	cnt = 0;
	// count odd divisors
	// O(sqrt(n))
	while (i < sqrt(n)+1) {
		for (;n % i == 0; cnt++, n/= i){}
		if (cnt > 0) {
			d *= (cnt+1);
		}
		i += 2;
		cnt = 0;
	}

	// i.e. remaining n is prime
	if (n > 1) d *= 2;

	return d;
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		print_usage();
		return -1;
	}

	int i;
	unsigned long t, n;
	clock_t start, end;
	start = clock();
	for (i = 1; i < argc; i++) {
		n = atol(argv[i]);
		t = tau(n);
		printf("tau(%ld)=%ld\n", n, t);
	}
	end = clock();
	printf("(%fs)\n", (double)(end-start)/CLOCKS_PER_SEC);
	return 0;
}