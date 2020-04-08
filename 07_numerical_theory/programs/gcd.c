#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

/* options */
bool o_help = false;
bool o_human_readable = false;

/* data types */
typedef unsigned long long_t;

void print_usage() {
	printf("Usage: gcd [-h] [-H] n1 n2 [n3 n4 ...]\n");
	printf("    ni, ni+1  Sequence pair(s) to find greatest common divisor. The number of arguments provided must be even.\n");
	printf("              n1 and n2 is the first pair, n3 and n4 the second and so forth. Alternatively the arguments\n");
	printf("              can be provided via stdin.\n");
	printf("    -h        Show usage.\n");
	printf("    -H        Print results in a human readable format.\n");
	printf("\n");
	printf("AUTHORS\n");
	printf("    Mike Noethiger (noethiger.mike@gmail.com)\n");
	printf("\n");
	printf("DESCRIPTION\n");
	printf("    Calculates the greatest common divisor of two numbers n1 and n2.\n");
	printf("    Time Complexity: O(log n).\n");
}

/* algorithm */

long_t gcd(long_t a, long_t b, long_t *f1, long_t *f2) {
	long_t ai, bi, a1, a2, b1, b2, f, d;
	ai = a;
	bi = b;
	a1 = 1; a2 = 0;
	b1 = 0; b2 = 1;
	while (a > 0 && b > 0) {
		if (a < b) {
			f = (b/a);
			b -= (f*a1*ai + f*a2*bi);
			b1 -= f*a1;
			b2 -= f*a2;
		} else {
			f = (a/b);
			a -= (f*b1*ai + f*b2*bi);
			a1 -= f*b1;
			a2 -= f*b2;
		}
	}
	if (a > b) {
		*f1 = a1;
		*f2 = a2;
		return a;
	} else {
		*f1 = b1;
		*f2 = b2;
		return b;
	}
}

/* program glue/shell */

void print_result(long_t a, long_t b, long_t gcd, long_t f1, long_t f2) {
	if (o_human_readable) {
		printf("gcd(%ld,%ld) = %ld = %ld %ld + %ld %ld\n", a, b, gcd, f1, a, f2, b);
	} else {
		printf("%ld ", gcd);
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

void exit_odd_arguments() {
	printf("even amount of arguments expected.\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
	parse_opt(argc, argv);
	if (o_help) {
		print_usage();
		exit(0);
	}
	// a   := first number for gcd
	// b   := second number for gcd
	// c   := gcd(a,b)
	// i   := running index
	// m   := number of input items matched by scanf
	// buf := buffer used to scan a character
	long_t a, b, c, f1, f2;
	size_t i;
	int m;
	char buf;
	if (argc-optind > 0) {
		if ((argc-optind) & 1) exit_odd_arguments();
		// input via program arguments
		for (i = optind; i < argc; i += 2) {
			a = atoi(argv[i]);
			b = atoi(argv[i+1]);
			c = gcd(a, b, &f1, &f2);
			print_result(a, b, c, f1, f2);
		}
	} else {
		// input via stdin
		do {
			m = scanf("%ld%ld%c", &a, &b, &buf);
			c = gcd(a, b, &f1, &f2);
			print_result(a, b, c, f1, f2);
		} while (m != EOF && buf != '\n');
	}
	printf("\n");
	return 0;
}