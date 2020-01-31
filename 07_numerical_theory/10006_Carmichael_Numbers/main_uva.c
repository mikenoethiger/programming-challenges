#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define FACTORS_SIZE 1000

typedef unsigned long long_t;

long_t modexp(long_t a, long_t x, long_t m) {
	long_t res = 1;
	while (x > 0) {
		if (x & 1) res = (res * a) % m;
		x >>= 1;
		a = (a * a) % m;
	}
	return res;
}

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
	int k, kp;
	long_t on, i, lp;

	k = 0;
	kp = 0;
	lp = 0;
	on = n;

	while (n % 2 == 0) {
		k++; kp++;
		n /= 2;
	}
	if (k > 1) return false;

	i = 3;
	k = 0;
	while (i < sqrt(n)+1) {
		if (k > 0) {
			if (k > 1) return false;
			if ((on-1) % (i-1) != 0) return false;
		}
		if (n % i == 0) {
			k++; kp++; lp = i;
			n /= i;
		} else {
			i += 2;
			k = 0;
		}
	}
	if (k > 0) {
		if (k > 1) return false;
		if ((on-1) % (i-1) != 0) return false;
	}

	if (n > 1) {
		if (n == lp) return false;
		if ((on-1) % (n-1) != 0) return false;
		kp++;
	}

	if (kp == 1) return false;

	return true;
}

long_t factors[FACTORS_SIZE];
long_t factors_n;

bool is_carmichael(long_t n) {
	long_t i;
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