#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#define N_FACTORS 1000000

/*
 * A program that calculates the prime factorization
 * of a given integer n.
 */

/* options */
bool o_help = false;
bool o_range = false;
char o_delimiter = ' ';

/* data types */
typedef unsigned long long_t;

void print_usage() {
	printf("Usage: fact [-h] [-r] n1 [n2 ...]\n");
	printf("    ni  number(s) to factorize\n");
	printf("    -h  show usage\n");
	printf("    -r  (range) factorize all numbers from 1..n1 or n1..n2\n");
	printf("    -d  delimiter to print between factors (default is white space).\n");
	printf("        Some delimiters need to be escaped otherwise they have specific meaning to bash, e.g '*'.\n");
	printf("\n");
	printf("DESCRIPTION\n");
	printf("    Prints the prime factorization of a given number n.\n");
	printf("    Runs in O(sqrt(n)).\n");
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
void factorize(long_t n, long_t *factors, size_t *n_factors) {
	int k = 0;
	long_t i;

	// find even divisors
	// O(log n)
	while (n % 2 == 0) {
		factors[k++] = 2;
		n /= 2;
	}

	// find odd divisors
	// sqrt(n)/2 => O(sqrt(n))
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

void print_factors(long_t n, long_t *factors, size_t n_factors) {
	long_t i;
	if (n_factors == 1) {
		printf("%ld is prime\n", n);
	} else {
		printf("%ld = ", n);
		for (i = 0; i < n_factors; i++) {
			printf("%ld", factors[i]);
			if (i < n_factors-1) printf("%c", o_delimiter);
		}
		printf("\n");
	}
}

void parse_opt(int argc, char *argv[]) {
	int opt;
	while ((opt = getopt(argc, argv, "hrd:")) != -1) {
		switch (opt) {
			case 'h':
				o_help = true;
				break;
			case 'r':
				o_range = true;
				break;
			case 'd':
				o_delimiter = optarg[0];
				break;
			default:
				print_usage();
				exit(EXIT_FAILURE);
		}
	}
}

int main(int argc, char *argv[]) {
	parse_opt(argc, argv);
	if (o_help) {
		print_usage();
		exit(0);
	}
	if (argc-optind < 1) {
		print_usage();
		exit(EXIT_FAILURE);
	}

	long_t n;
	long_t factors[N_FACTORS];
	long_t n_factors, i;
	clock_t start, end;
	long_t r_start, r_end;
	int r_offset;
	int opt = optind;

	if (argc-opt == 1) {
		r_start = 1;
		r_end = atol(argv[opt]);
		r_offset = 1;
	} else {
		r_start = atol(argv[opt]);
		r_end = atol(argv[opt+1]);
		r_offset = 2;
	}

	start = clock();

	if (o_range) {
		for (i = r_start; i <= r_end; i++) {
			factorize(i, factors, &n_factors);
			print_factors(i, factors, n_factors);
		}
		opt += r_offset;
	}

	for (i = opt; i < argc; i++) {
		n = atol(argv[i]);
		factorize(n, factors, &n_factors);
		print_factors(n, factors, n_factors);
	}

	end = clock();

	printf("%f ms\n", ((double)end-start)/CLOCKS_PER_SEC*1000);

	return 0;
}