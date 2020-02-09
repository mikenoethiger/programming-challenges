#include <stdio.h>
#include <math.h>

// Numbers up to 10^9 have to be handled.
// A prime factorization of a number of
// that size would in worst case only consist
// of two's, therefore there will never be
// more than log2(10^9) prime factors.
// log2(10^9) = 29.89735
#define FACTORS_N 30

/* data types */
typedef unsigned long long_t;

/* data structure */

// factors   := prime factors of the factorized number
// factors_n := length of factors
long_t factors[FACTORS_N];
long_t factors_n;

/* Factorizes x into it's prime factors.
 * Factors will be stored in global variable `factors`.
 * `factors` length will be stored in `factors_n`.
 * Time Complexity: O(sqrt(n)).
 */
void factorize(long_t n) {
	int k = 0;
	long_t i;

	// find even divisors
	// O(log n)
	while (!(n & 1)) {
		factors[k++] = 2;
		n /= 2;
	}

	// find odd divisors
	// sqrt(n)/2 => O(sqrt(n))
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

/* Calculates the sum of all digits of n.
 * Time Complexity: O(log10(n))
 * Or in other words, the amount of steps
 * involved into the algorithm grows equally
 * to the number of digits added to n
 */
long_t sum_digits(long_t n) {
	// sum := sum of the digits of n
	// rem := remainder (temporary variable)
	long_t sum, rem;
	sum = 0;
	while (n != 0) {
		rem = n % 10;
		sum += rem;
		n = (n-rem)/10;
	}
	return sum;
}

/* Sums up the digits of all numbers in the
 * global `factors` array and returns this sum.
 */
long_t sum_prime_factors() {
	long_t sum, i;
	sum = 0;
	for (i = 0; i < factors_n; i++) {
		sum += sum_digits(factors[i]);
	}
	return sum;
}

void next_smith_number(long_t n) {
	// sum_f := sum of prime factorization
	// sum_n := sum of input number n
	long_t sum_f, sum_n;

	// increment, because find NEXT bigger smith number
	// is the task.
	n++;
	factorize(n);
	sum_f = sum_prime_factors();
	sum_n = sum_digits(n);
	// factors_n == 1 is due to the fact the prime numbers
	// are excluded from the definition of a smith number
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
	// skip cases number
	scanf("%ld", &n);

	while (scanf("%ld", &n) != EOF) {
		next_smith_number(n);
	}

//	int i;
//	long_t n = 6666666;
//	factorize(n);
//	for (i = 0; i < factors_n; i++) {
//		printf("%ld ", factors[i]);
//	}
//	printf("\n");
//	printf("%ld\n", sum_prime_factors());
//	printf("%ld\n", sum_digits(n));
	return 0;
}