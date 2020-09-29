#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define N 1000

typedef unsigned long long_t;

long_t divisors[N];
long_t di;

bool contains(long_t *arr, long_t size, long_t el) {
	long_t i;
	for (i = 0; i < size; i++) {
		if (arr[i] == el) return true;
	}
	return false;
}

void print_divisors() {
	printf("[");
	long_t i;
	for (i = 0; i < di; i++) {
		printf("%ld, ", divisors[i]);
	}
	printf("]\n");
}

long_t sum_divisors(long_t n) {
	if (n == 2) return 1;

	di = 0;

	long_t init_n = n;
	long_t sum = 1;
	long_t i;

	long_t repeat = 1;
	long_t cofactor;
	while (n % 2 == 0) {
//		printf("%ld divisible by %ld\n", n, i);
		n /= 2;
		divisors[di++] = n;
		cofactor = (long_t) pow(2, repeat);
		if (!contains(divisors, di, cofactor)) divisors[di++] = cofactor;
		repeat++;
	}

	for (i = 3; 2*i < n; i += 2) {
		if (n % i == 0) {
			repeat = 1;
			while (n % i == 0) {
//				print_divisors();
				//printf("%ld divisible by %ld\n", n, i);
				//printf("%ld += %ld\n", sum, (long_t) pow(i,repeat));
				n /= i;
				cofactor = (long_t) pow(i, repeat);
				if (!contains(divisors, di, n)) divisors[di++] = n;
				if (!contains(divisors, di, cofactor)) divisors[di++] = cofactor;
				repeat++;
			}
		}
	}
	printf("n=%ld\n", n);
	print_divisors();

	for (i = 0; i < di; i++) {
		if (init_n % divisors[i] == 0 && !contains(divisors, di, init_n / divisors[i])) {

		}
	}

	for (i = 0; i < di; i++) {
		if (divisors[i] != init_n && divisors[i] != 1) {
			sum += divisors[i];
		}
	}

	return sum;
}

int main() {

	long_t cases, i, n;
	scanf("%ld\n", &cases);

	long_t sum;
	for (i = 0; i < cases; i++) {
		scanf("%ld\n", &n);
		sum = sum_divisors(n);
		printf("%ld\n", sum);
	}

	return 0;
}