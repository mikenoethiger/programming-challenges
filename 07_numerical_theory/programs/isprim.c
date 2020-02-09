#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * Primality test program O(sqrt n).
 */

/* data types */
typedef unsigned long long_t;

/* options */
bool o_help = false;
bool o_human_readable = false;
bool o_range = false;

void print_usage() {
	printf("Usage: isprim [-h] [-H] n1 [n2 ...]\n");
	printf("    ni  Primality test numbers. Alternatively these numbers can be provided via stdin.\n");
	printf("    -h  Show usage\n");
	printf("    -H  Print results in a human readable format\n");
	printf("\n");
	printf("AUTHORS\n");
	printf("    Mike Noethiger (noethiger.mike@gmail.com)\n");
	printf("\n");
	printf("DESCRIPTION\n");
	printf("    Primality test for a given number with 100%% certainty.\n");
	printf("    Time Complexity: O(sqrt n).\n");
}

/* algorithm */

bool is_prime(long_t n) {
	if (n == 0 || n == 1) return false;
	if (n == 2) return true;
	if (!(n & 1)) return false;

	long_t i;

	// first divisor below sqrt(n) implies a second divisor above sqrt(n) which implies n is not prime
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

/* program glue/shell */

void prim_test(long_t n) {
	if (is_prime(n)) {
		printf("%ld is prim\n", n);
	} else {
		printf("%ld is normal\n", n);
	}
}

void print_results(long_t n, bool prime) {
	if (o_human_readable) {
		if (prime) {
			printf("%ld=prim\n", n);
		} else {
			printf("%ld=comp\n", n);
		}
	} else {
		printf("%d ", prime ? 1 : 0);
	}
}

void parse_opt(int argc, char *argv[]) {
	int opt;
	while ((opt = getopt(argc, argv, "hH")) != -1) {
		switch (opt) {
			case 'h':
				o_help = true;
				break;
			case 'H':
				o_human_readable = true;
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

	// n     := primality test number
	// argc2 := number of arguments after options have been processed
	// sr    := scanf function result
	// buf   := char buffer used for scanf
	long_t n;
	int argc2, sr;
	char buf;

	argc2 = argc-optind;

	if (argc2 < 1) {
		// read arg(s) from stdin
		do {
			sr = scanf("%ld%c", &n, &buf);
			if (sr < 2) break; // indicates no number matched
			print_results(n, is_prime(n));
		} while (sr != EOF && buf != '\n');
	} else {
		while (optind < argc) {
			n = atol(argv[optind]);
			print_results(n, is_prime(n));
			optind++;
		}
	}

	printf("\n");

	return 0;
}