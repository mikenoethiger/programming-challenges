#include <stdio.h>
#include <math.h>

void solve(int n) {
	int c;
	int i;
	c = 0;
	for (i = 1; i <= n; i++) {
		if (n % i == 0) c++;
	}
	if (c % 2 == 0) printf("no\n");
	else printf("yes\n");
}

void solve2(unsigned long n) {
	unsigned long initial_n = n;
	int d = 0;
	size_t i;

	int divisors = 1;
	int cnt = 0;
	int pivot;

	while (n % 2 == 0) {
		n /= 2;
		cnt++;
	}
	divisors *= (cnt+1);

	i = 3;
	cnt = 0;
	pivot = i;
	while (i < sqrt(n)+1) {
		for (;n % i == 0; cnt++, n/= i){}
		if (cnt > 0) {
			divisors *= (cnt+1);
		}
		i += 2;
		cnt = 0;
	}

	// i.e. remaining n is prime
	if (n > 1) divisors *= 2;

	if (divisors % 2 == 0) printf("no\n");
	else printf("yes\n");
}

int main() {
	unsigned long n;

	while (scanf("%ld", &n) && n) {
		solve2(n);
	}
}