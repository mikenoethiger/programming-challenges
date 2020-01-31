#include <stdio.h>
#include <math.h>

typedef unsigned long long_t;


void solve(long_t n) {
	double p_square = sqrt(n);
	if (p_square - (long_t) p_square > 0) {
		printf("no\n");
	} else {
		printf("yes\n");
	}
}

int main() {
	long_t n;

	while (scanf("%ld", &n) != EOF && n) {
		solve(n);
	}
	return 0;
}