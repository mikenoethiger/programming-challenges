#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <time.h>

#define N_FACTORS 1000000

/*
 * A program that calculates the prime factorization
 * of a given integer n.
 */

void print_usage() {
	printf("Usage: fact n\n");
	printf("    n := number to factorize\n");
	printf("DESCRIPTION\n");
	printf("    Prints the prime factorization of a given number n.\n");
}

/**
 * Make prime factorization for an.
 *
 * Runtime Complexity: O(sqrt(n))
 *
 * Runtime complexity was reduced based on two main ideas:
 * 1. Only sqrt(n) numbers need to be checked, if n is divisible
 *    by a number > sqrt(n), it's co-factor needs to be smaller
 *    than sqrt(n) i.e. it has already been checked previously
 * 2. If n is odd, only odd numbers have to be checked
 * 3. If n is even, the number of even divisors (which is the
 *    number of times it can be divided by 2) can be counted
 *    in O(log n) steps.
 *
 * @param n the number to factorize
 * @param factors array that will be filled with the factors
 * @param n_factors will contain the number of factors
 */
void factorize(unsigned long n, long *factors, size_t *n_factors) {
	int k = 0;
	long i;

	// find even divisors
	// O(log n)
	while (n % 2 == 0) {
		factors[k++] = 2;
		n /= 2;
	}

	// find odd divisors
	// O(sqrt(n)/2) = O(sqrt(n))
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

int main(int argc, char *argv[]) {

	if (argc != 2) {
		print_usage();
		return -1;
	}

	long n;

	long factors[N_FACTORS];
	size_t n_factors, i;
	clock_t start, end;

	n = atol(argv[1]);
	printf("factorize %ld\n", n);

	start = clock();
	factorize(n, factors, &n_factors);
	end = clock();

	if (n_factors == 1) {
		printf("%ld is prime\n", n);
	} else {
		printf("%ld prime factors\n", n_factors);
		for (i = 0; i < n_factors; i++) {
			printf("%ld ", factors[i]);
		}
		printf("\n");
	}

	printf("(%f seconds)\n", ((double)end-start)/CLOCKS_PER_SEC);

	return 0;
}