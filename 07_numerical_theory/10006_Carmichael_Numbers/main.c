#include <stdio.h>
#include <math.h>
#include <stdbool.h>

typedef unsigned long long_t;

bool korselt_test(int n) {
	// Satz von Korselt https://de.wikipedia.org/wiki/Carmichael-Zahl#Satz_von_Korselt
	// Eine natürliche Zahl n ist genau dann eine Carmichael-Zahl,
	// wenn sie nicht prim und quadratfrei ist und für alle ihre
	// Primteiler p gilt, dass p-1 die Zahl n-1 teilt.

	// k  := temp counter
	// kp := prime numbers counter
	// on := original n
	// i  := running number
	// lp := last prime number (cache)
	int k, kp;
	int on, i, lp;

	k = 0;
	kp = 0;
	on = n;

	// find even divisors
	while (n % 2 == 0) {
		k++; kp++;
		n /= 2;
	}
	// n is not square free
	if (k > 1) return false;

	// find odd divisors
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
	// check again, otherwise we might miss the last prime number
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

	// n is prime thus by definition not carchmichael
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