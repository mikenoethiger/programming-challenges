#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define N 10000000

typedef unsigned long long_t;

long_t cache_prime[N];
long_t cache[N];
long_t sieve[N];

long_t first_prime(long_t n) {
	if (cache_prime[n] != -1) return cache_prime[n];
	if (!(n & 1)) return 2;
	if (sieve[n] == 0) return n;
	return sieve[n];

	long_t p = 3;
	while (n % p != 0 && p*p < n) {
		p += 2;
	}
	if (n % p == 0) cache_prime[n] = p;
	else cache_prime[n] = n;
	return cache_prime[n];
}

long_t fib(long_t n) {
	if (n == 0 || n == 1) return 0;
	if (cache[n-1] == -1) {
		cache[n-1] = fib(n-1);
	}
	return cache[n-1] + first_prime(n);
}

void solve(long_t n) {
	long_t res = fib(n);
	printf("%ld\n", res);
}

void do_sieve() {
	long_t i;
	long_t j;
	for (i = 3; i*i < N; i += 2) {
		while (sieve[i] != 0 && i*i < N) i += 2;
		for (j = 2; j*i < N; j++) {
			if (sieve[j*i] == 0) sieve[j*i] = i;
		}

	}
}

int main() {
	long_t src = -1;
	memset(cache, -1, N* sizeof(long_t));
	memset(cache_prime, -1, N* sizeof(long_t));
	memset(sieve, 0, N * sizeof(long_t));

	long_t cases, n;
	do_sieve();
	scanf("%ld\n", &cases);

	size_t i;
	for (i = 0; i < cases; i++) {
		scanf("%ld\n", &n);
		solve(n);
	}

	return 0;
}