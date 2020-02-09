#include <stdio.h>
#include <math.h>

#define FACTORS_N 30

typedef unsigned long long_t;

long_t factors[FACTORS_N];
long_t factors_n;

void factorize(long_t n) {
	int k = 0;
	long_t i;

	while (!(n & 1)) {
		factors[k++] = 2;
		n /= 2;
	}

	i = 3;
	while (i < sqrt(n)+1) {
		if (n % i == 0) {
			factors[k++] = i;
			n /= i;
		} else {
			i += 2;
		}
	}

	if (n > 1) factors[k++] = n;

	factors_n = k;
}

long_t sum_digits(long_t n) {
	long_t sum, rem;
	sum = 0;
	while (n != 0) {
		rem = n % 10;
		sum += rem;
		n = (n-rem)/10;
	}
	return sum;
}

long_t sum_prime_factors() {
	long_t sum, i;
	sum = 0;
	for (i = 0; i < factors_n; i++) {
		sum += sum_digits(factors[i]);
	}
	return sum;
}

void next_smith_number(long_t n) {
	long_t sum_f, sum_n;

	n++;
	factorize(n);
	sum_f = sum_prime_factors();
	sum_n = sum_digits(n);
	while (sum_f != sum_n || factors_n == 1) {
		n++;
		factorize(n);
		sum_f = sum_prime_factors();
		sum_n = sum_digits(n);
	}
	printf("%ld\n", n);
}

int main() {
	long_t n;
	scanf("%ld", &n);

	while (scanf("%ld", &n) != EOF) {
		next_smith_number(n);
	}
	return 0;
}