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

long_t lazy(long_t n) {
	long_t sum = 0;
	long_t i;

	for (i = 1; i < n; i++) {
		if (n % i == 0) sum += i;
	}
	return sum;
}

long_t sum_divisors(long_t n) {
	if (n == 2) return 1;
	if (n == 1) return 0;

	di = 0;

	long_t init_n = n;
	long_t sum = 1;
	long_t i;

	for (i = 2; i*i <= n; i++) {
		if (n % i == 0) {
			//printf("add %ld\n", i);
			sum += i;
			if (n/i != i) {
				sum += n/i;
			//	printf("add %ld\n", n/i);
			}
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

//	long_t s1, s2;
//	for (i = 1; i < 10; i++) {
//		s1 = sum_divisors(i);
//		s2 = lazy(i);
//		if (s1 != s2) {
//			printf("i = %ld\n", i);
//			printf("s1 = %ld\n", s1);
//			printf("s2 = %ld\n", s2);
//		}
//	}

//	printf("%ld\n", sum_divisors(4));

	return 0;
}