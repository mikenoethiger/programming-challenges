#include <stdio.h>
#include <math.h>

typedef unsigned long long_t;


/**
 * O(log n)
 *
 * @param a
 * @param x
 * @param m
 * @return
 */
long_t modexp(long_t a, long_t x, long_t m) {
	long_t res = 1;
	while (x > 0) {
		// if LSB is a 1
		if (x & 1) res = (res * a) % m;
		// check next bit
		x >>= 1;
		a = (a * a) % m;
	}
	return res;
}

// from libs/tau.c
int tau(int n) {
	int d, i, cnt;
	d = 1;
	cnt = 0;

	for(;n%2 == 0; n/=2, cnt++) {}
	d *= (cnt+1);

	i = 3;
	cnt = 0;
	while (i < sqrt(n)+1) {
		for (;n % i == 0; cnt++, n/= i){}
		if (cnt > 0) d *= (cnt+1);
		i += 2;
		cnt = 0;
	}
	if (n > 1) d *= 2;
	return d;
}

void solve(int n) {
	long_t i;
	int is_carmichael = 1;
	int eq_true = 1;
	for (i = 2; i < n; i++) {
		long_t res = modexp(i, n, n);
		if (res != i) {
			eq_true = 0;
			break;
		}
	}
	if (eq_true && tau(n) > 2) {
		printf("The number %d is a Carmichael number.\n", n);
	} else {
		printf("%d is normal.\n", n);
	}
}

int main() {
	int n;
	while (scanf("%d", &n) && n) {
		solve(n);
	}
	return 0;
}