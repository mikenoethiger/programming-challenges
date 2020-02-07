#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define LIMIT 10000000
#define PRIME 0
#define COMPOSITE 1

typedef unsigned long long_t;
typedef unsigned char sieve_t;

long_t un;
long_t ep;
long_t np;
sieve_t *sieve;
long_t *primes;

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
	long_t x, p1, p2;

	p1 = 2;
	p2 = 2;
	if (n & 1) p2 = 3;
	x = n - p1 - p2;

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