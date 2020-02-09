#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define PRIME 0
#define COMPOSITE 1

/*
 * A program that finds prime numbers from 2..n by
 * applying sieve of eratosthenes.
 */

/* options */
bool o_help = false;
bool o_quiet = false;

/* data types */
typedef unsigned long long_t;
typedef unsigned char sieve_t;

/* data structures */

// n      := upper limit to which primes should be found
// ep     := estimated number of primes based on the
//           prime number theorem: n/ln(n)
// np     := exact number of primes from 2..n
// sieve  := prime number information of every number
//           0..n
// sieve[n]  = 0 if n is prime
// sieve[n] != 0 if n is composite
// primes := array of consecutive primes from 2..n
long_t n;
long_t ep;
long_t np;
sieve_t *sieve;
long_t *primes;

void print_usage() {
	printf("Usage: sieve1 [-h] [-q] n\n");
	printf("    n   upper limit\n");
	printf("    -h  show usage\n");
	printf("    -q  quiet, do not list the primes\n");
	printf("\n");
	printf("DESCRIPTION\n");
	printf("    Finds prime numbers from 2..n by using\n");
	printf("    sieve of eratosthenes.\n");
}

void parse_opt(int argc, char *argv[]) {
	int opt;
	while ((opt = getopt(argc, argv, "hq")) != -1) {
		switch (opt) {
			case 'h':
				o_help = true;
				break;
			case 'q':
				o_quiet = true;
				break;
			default:
				print_usage();
				exit(EXIT_FAILURE);
		}
	}
}

/*
 * Apply sieve of eratosthenes.
 *
 * After this function was called, `sieve[p]` will be set to
 * PRIME if p is prime, otherwise it will be set to COMPOSITE.
 * (with PRIME and COMPOSITE being #defined values)
 *
 * Furthermore, the global variable `primes` will contain all
 * consecutive prime numbers from 2..n and `np` will be set
 * to the amount of primes present in `primes`.
 *
 * Following optimization were applied to the sieve algorithm:
 * 1. Even numbers are ignored, thus even numbers are falsely marked prime in `sieve`.
 *    Even numbers can be easily checked with logical and: if (n&1) n is odd.
 * 2. Sieve (i.e. multiplying and "cross out" mechanism) is only applied to numbers <= sqrt(n).
 *    All composite numbers { c | sqrt(n) <= c <= n } need to have a prime factor <= sqrt(n),
 *    consequently they have all been detected at the point where we reach sqrt(n).
 * 3. When multiplying prime numbers to cross out composite numbers, the multiply factor starts
 *    at the current prime number and increases by 2 consequently for the same reason as #1:
 *    Multiplying an odd number with an even number results in an even number.
 *
 * Runtime analysis:
 * 8.661000 ms   for n=1'000'000
 * 92.081000 ms  for n=10'000'000
 * 977.944000 ms for n=100'000'000
 */
void apply_sieve() {
	long_t i, k;
	sieve[0] = COMPOSITE;
	sieve[1] = COMPOSITE;
	primes[np++] = 2;
	for (i = 3; i < sqrt(n)+1; i += 2) {
		while (sieve[i] == COMPOSITE && i < sqrt(n)+1) i += 2;
		if (i > sqrt(n)) break;
		primes[np++] = i;
		for (k = i; k*i <= n; k += 2) {
			sieve[k*i] = COMPOSITE;
		}
	}
	while (i <= n) {
		if (sieve[i] != COMPOSITE && i & 1) primes[np++] = i;
		i++;
	}
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		print_usage();
		exit(EXIT_FAILURE);
	}
	parse_opt(argc, argv);
	if (o_help) {
		print_usage();
		exit(0);
	}

	int i;
	clock_t begin, end;

	n = atol(argv[optind]);
	ep = 2*(n/log(n));
	np = 0;

	sieve = malloc(sizeof(sieve_t)*(n+1));
	primes = malloc(sizeof(long_t)*ep);

	memset(sieve, 0, sizeof(sieve_t)*n);

	if (n > 1) {
		begin = clock();
		apply_sieve();
		end = clock();
	}

	printf("%ld primes found in %f ms\n", np, ((double)end-begin)/CLOCKS_PER_SEC*1000);

	if (!o_quiet && np > 0) {
		for (i = 0; i < np; i ++) {
			printf("%ld ", primes[i]);
		}
		printf("\n");
	}

	free(sieve);
	free(primes);

	return 0;
}