#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#define PROG_NAME "fact"
#define N_FACTORS 1000000
#define NEW_LINE 'n'

/*
 * A program that calculates the prime factorization
 * of a given integer n.
 */

/* options */

bool o_help = false;
bool o_human_readable = false;
char o_delimiter = ' ';

/* data types */

typedef unsigned long long_t;

void print_usage() {
	printf("Usage: %S [-h] [-H] [-d char] n1 [n2 ...]\n", PROG_NAME);
	printf("    ni  Numbers to perform prime factorization on. Alternatively these numbers can be provided via stdin.\n");
	printf("    -h  Show usage.\n");
	printf("    -H  Print results in a human readable format.\n");
	printf("    -d  Delimiter (single character) to print between each factorized number ni (default is white space). Does not have any impact if -H is specified.\n");
	printf("        Some delimiters need to be escaped, otherwise they have reserved meaning to the shell, e.g * needs to be escaped '*'.\n");
	printf("        Special delimiter: n (represents new line).\n");
	printf("\n");
	printf("AUTHORS\n");
	printf("    Mike Noethiger (noethiger.mike@gmail.com)\n");
	printf("\n");
	printf("DESCRIPTION\n");
	printf("    Calculates the prime factorization of a given number.\n");
	printf("    Time Complexity: O(sqrt(n)).\n");
}

/* algorithm */

/**
 * Make prime factorization for an.
 *
 * Time Complexity: O(sqrt(n))
 *
 * Time complexity was reduced based on two main ideas:
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
	if (n == 0) {
		factors[0] = 0;
		*n_factors = 1;
		return;
	} else if (n == 1) {
		factors[0] = 1;
		*n_factors = 1;
		return;
	}

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

/* program glue/shell */

void print_factors(long_t n, long_t *factors, size_t n_factors) {
	long_t i;
	for (i = 0; i < n_factors; i++) {
		printf("%ld", factors[i]);
		if (i < n_factors-1) printf(" ");
	}
}

void print_results(long_t n, long_t *factors, size_t n_factors) {
	if (o_human_readable) {
		printf("%ld = ", n);
		if (n_factors == 1 && n != 0 && n != 1) {
			printf("is prime");
		} else {
			print_factors(n, factors, n_factors);
		}
		printf("\n");
	} else {
		print_factors(n, factors, n_factors);
		printf("%c", o_delimiter);
	}
}

void parse_opt(int argc, char *argv[]) {
	int opt;
	while ((opt = getopt(argc, argv, "hHd:")) != -1) {
		switch (opt) {
			case 'h':
				o_help = true;
				break;
			case 'H':
				o_human_readable = true;
				break;
			case 'd':
				o_delimiter = optarg[0];
				if (o_delimiter == NEW_LINE) o_delimiter = '\n';
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

	// n         := number to factorize
	// n_factors := number of prime factors
	// factors   := the actual prime factors
	// argc2     := number of arguments after options have been processed
	// sr        := scanf function result
	long_t n, n_factors;
	long_t factors[N_FACTORS];
	int argc2, sr;
	char buf;

	argc2 = argc-optind;
	if (argc2 < 1) {
		// read args from stdin
		do {
			sr = scanf("%ld%c", &n, &buf);
			if (sr < 2) break; // indicates no number matched
			factorize(n, factors, &n_factors);
			print_results(n, factors, n_factors);
		} while (sr != EOF && buf != '\n');
	} else {
		// process args from argv
		while (optind < argc) {
			n = atol(argv[optind]);
			factorize(n, factors, &n_factors);
			print_results(n, factors, n_factors);
			optind++;
		}
	}
	printf("\n");

	return 0;
}