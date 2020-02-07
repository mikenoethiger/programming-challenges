#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define LIMIT 10000000
#define PRIME 0
#define COMPOSITE 1

/* data types */
typedef unsigned long long_t;
typedef unsigned char sieve_t;

/* data structures */

// un     := upper limit to which primes should be found
// ep     := estimated number of primes based on the
//           prime number theorem: n/ln(n)
// np     := exact number of primes from 2..n
// sieve  := prime number information of every number
//           0..n
// sieve[p] = PRIME if p is prime
// sieve[p] = COMPOSITE if p is composite
// primes := array of consecutive primes from 2..n
long_t un;
long_t ep;
long_t np;
sieve_t *sieve;
long_t *primes;

/* algorithms */

void sieve_of_eratosthenes() {
	long_t i, k;
	sieve[0] = COMPOSITE;
	sieve[1] = COMPOSITE;
	primes[np++] = 2;
	for (i = 3; i < sqrt(un)+1; i += 2) {
		while (sieve[i] == COMPOSITE && i < sqrt(un)+1) i += 2;
		if (i > sqrt(un)) break;
		primes[np++] = i;
		for (k = i; k*i <= un; k += 2) {
			sieve[k*i] = COMPOSITE;
		}
	}
	while (i <= un) {
		if (sieve[i] != COMPOSITE && i & 1) primes[np++] = i;
		i++;
	}
}

void solve(long_t n) {
	// Case n=8 and n=9 would require special condition
	// checking on the later algorithm, thus potentially
	// slow down the runtime which is why they are handled here.
	if (n < 8) {
		printf("Impossible.\n");
		return;
	} else if (n == 8) {
		printf("2 2 2 2\n");
		return;
	} else if (n == 9) {
		printf("2 2 2 3\n");
		return;
	}
	// Idea:
	// n is even (even + even = even):
	//   x + 2 + 2 = n
	// n is odd (odd + odd = even):
	//   x + 3 + 2 = n
	// in both cases x is even and can be expressed as sum of 2 primes (Goldbach's Conjecture)
	long_t x, p1, p2;

	p1 = 2;
	p2 = 2;
	// if n is odd
	if (n & 1) p2 = 3;
	x = n - p1 - p2;

	// Apply Goldbach's Conjecture:
	// pi is the ith prime number.
	// p0 = 2
	// pk = last prime <= x/2
	// for p = p0 to pk
	//   if x-p is prime: we have a prime pair that add up to x
	// endfor
	// if the if was never entered in the loop, then x can not be expressed
	// as the sum of two primes (which would violate Goldbach's conjecture,
	// thus very likely doesn't happen)
	long_t i, p;

	for (i = 0; primes[i] <= x/2; i++) {
		p = x-primes[i];
		if (p & 1 && sieve[p] == PRIME) {
			printf("%ld %ld %ld %ld\n", p1, p2, primes[i], p);
			return;
		}
	}
	printf("Goldbach's conjecture is wrong\n");
}

int main() {
	un = LIMIT;
	ep = 2*(un/log(un));
	np = 0;

	sieve = malloc(sizeof(sieve_t)*(un+1));
	primes = malloc(sizeof(long_t)*ep);
	memset(sieve, 0, sizeof(sieve_t)*un);

	sieve_of_eratosthenes();

	long_t n;
	while (scanf("%ld", &n) != EOF) {
		solve(n);
	}
	return 0;
}