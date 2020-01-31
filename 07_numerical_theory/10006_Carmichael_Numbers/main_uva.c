#include <stdio.h>
#include <math.h>
#include <stdbool.h>

typedef unsigned long long_t;

bool korselt_test(int n) {
	int k, kp;
	int on, i, lp;

	k = 0;
	kp = 0;
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

int main() {
	int n;
	while (scanf("%d", &n) && n) {
		if (korselt_test(n)) {
			printf("The number %d is a Carmichael number.\n", n);
		} else {
			printf("%d is normal.\n", n);
		}
	}

	return 0;
}