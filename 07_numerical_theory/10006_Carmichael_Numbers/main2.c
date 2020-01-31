#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define FACTORS_SIZE 1000

typedef unsigned long long_t;

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

int ggT(int a, int b) {
	if (a == 0) return b;
	else if (b == 0) return a;
	if (a > b) return ggT(a%b, b);
	else return ggT(a, b%a);
}

bool korselt_test(long_t n) {
	// k  := running number
	// on := original n
	int k, kp;
	long_t on, i, lp;

	k = 0;
	kp = 0;
	lp = 0;
	on = n;

	// find even divisors
	// O(log n)
	while (n % 2 == 0) {
		k++; kp++;
		n /= 2;
	}
	// n is not square free
	if (k > 1) return false;

	// find odd divisors
	// O(sqrt(n)/2) = O(sqrt(n))
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
	// check again, otherwise we might miss the lass prime number
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

	// n is prime
	if (kp == 1) return false;

	return true;
}

long_t factors[FACTORS_SIZE];
long_t factors_n;

bool is_carmichael(long_t n) {
	// Satz von Korselt https://de.wikipedia.org/wiki/Carmichael-Zahl#Satz_von_Korselt
	// Eine natürliche Zahl n ist genau dann eine Carmichael-Zahl,
	// wenn sie nicht prim und quadratfrei ist und für alle ihre
	// Primteiler p gilt, dass p-1 die Zahl n-1 teilt.
	long_t i;

//	factorize(n, factors, &factors_n);
//	// prime numbers are not carmichael
//	if (factors_n == 1) return false;
//	for (i = 0; i < factors_n; i++) {
//		if ((n-1) % (factors[i]-1) != 0) return false;
//	}
	return true;
}

void solve(long_t n) {
	if (korselt_test(n)) {
		printf("The number %ld is a Carmichael number.\n", n);
	} else {
		printf("%ld is normal.\n", n);
	}
}

int main() {
	int n;
	while (scanf("%d", &n) && n) {
		solve(n);
	}

	return 0;
}