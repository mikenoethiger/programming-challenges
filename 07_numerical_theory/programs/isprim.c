#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

typedef unsigned long long_t;

/*
 * A program that checks whether a given number
 * n is prime.
 */

void print_usage() {
	printf("Usage: isprim [-h] [-r] n1 [n2 ...]\n");
	printf("    ni  number(s) to check for primality\n");
	printf("    -h  show usage\n");
	printf("    -r  check all numbers between n1 and n2 for primality\n");
	printf("\n");
	printf("DESCRIPTION\n");
	printf("    Checks whether a given number is prime in O(sqrt(n)).\n");
	printf("    Certainty is 100 percent.\n");
}

bool is_prime(long_t n) {
	if (n == 0 || n == 1) return false;
	if (n == 2) return true;
	if (n % 2 == 0) return false;

	long_t i;

	// first divisor below sqrt(n) implies
	// a second divisor above sqrt(n) which
	// implies n is not prime
	i = 3;
	while (i < sqrt(n)+1) {
		if (n % i == 0) {
			return false;
		} else {
			i += 2;
		}
	}
	return true;
}

void prim_test(long_t n) {
	if (is_prime(n)) {
		printf("%ld is prim\n", n);
	} else {
		printf("%ld is normal\n", n);
	}
}

bool o_help = false;
bool o_range = false;

void parse_opt(int argc, char *argv[]) {
	int opt;
	while ((opt = getopt(argc, argv, "hr")) != -1) {
		switch (opt) {
			case 'h':
				o_help = true;
				break;
			case 'r':
				o_range = true;
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
		return -1;
	}
	if (o_range && argc-optind < 2) {
		print_usage();
		return -1;
	}

	long_t i1,i2;
	long_t p;

	if (o_range) {
		i1 = atol(argv[optind]);
		i2 = atol(argv[optind+1]);
		for (; i1 <= i2; i1++) {
			prim_test(i1);
		}
	} else {
		for (i1 = optind; i1 < argc; i1++) {
			prim_test(atol(argv[i1]));
		}
	}

	return 0;
}