#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

typedef unsigned long long_t;

/*
 * A program that checks whether a given number
 * n is prime.
 */

void print_usage() {
	printf("Usage: carmichael [-h] [-r] n1 [n2 ...]\n");
	printf("    ni  number(s) to check\n");
	printf("    -h  show usage\n");
	printf("    -r  range check: check all numbers between 1..n1 or n1..n2\n");
	printf("\n");
	printf("DESCRIPTION\n");
	printf("    Checks whether a given number is carmichael in O(sqrt(n)).\n");
	printf("    Certainty is 100 percent.\n");
}

bool korselt_test(long_t n) {
	// Satz von Korselt https://de.wikipedia.org/wiki/Carmichael-Zahl#Satz_von_Korselt
	// Eine natürliche Zahl n ist genau dann eine Carmichael-Zahl,
	// wenn sie nicht prim und quadratfrei ist und für alle ihre
	// Primteiler p gilt, dass p-1 die Zahl n-1 teilt.

	// k  := temp counter
	// kp := prime numbers counter
	// on := original n
	// i  := running number
	// lp := last prime number (cache)
	long_t k, kp;
	long_t on, i, lp;

	k = 0;
	kp = 0;
	on = n;

	// find even divisors
	while (n % 2 == 0) {
		k++; kp++;
		n /= 2;
	}
	// n is not square free
	if (k > 1) return false;

	// find odd divisors
	i = 3;
	k = 0;
	while (i < sqrt(n)+1) {
		if (k > 0) {
			// i is multiple prime factor of n
			// thus n is not square free
			if (k > 1) return false;
			// Satz von Korselt
			if ((on-1) % (i-1) != 0) return false;
		}
		if (n % i == 0) {
			k++; kp++; lp = i;
			n /= i;
		} else {
			// i is prime factor of n
			i += 2;
			k = 0;
		}
	}
	// check again, otherwise we might miss the last prime number
	// from the loop above
	if (k > 0) {
		if (k > 1) return false;
		if ((on-1) % (i-1) != 0) return false;
	}

	// n > 1 => remaining number is a prime number too
	if (n > 1) {
		// n is not square free
		if (n == lp) return false;
		if ((on-1) % (n-1) != 0) return false;
		kp++;
	}

	// n is prime thus by definition not carchmichael
	if (kp == 1) return false;

	return true;
}

void is_carmichael(long_t n) {
	if (korselt_test(n)) {
		printf("%ld is carmichael\n", n);
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

/* get the number of args, considering already processed options */
int n_args(int argc) {
	return argc-optind;
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

	int opt = optind;
	long_t i1,i2;
	long_t p;

	if (o_range) {
		if (n_args(argc) > 1) {
			i1 = atol(argv[optind]);
			i2 = atol(argv[optind+1]);
		} else {
			i1 = 1;
			i2 = atol(argv[optind]);
		}
		for (; i1 <= i2; i1++) {
			is_carmichael(i1);
		}
	} else {
		for (i1 = optind; i1 < argc; i1++) {
			is_carmichael(atol(argv[i1]));
		}
	}

	return 0;
}