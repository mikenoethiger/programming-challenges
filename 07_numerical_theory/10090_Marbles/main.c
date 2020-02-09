#include <stdio.h>

typedef unsigned int long_t;

void gcd_lin_combination(long_t a, long_t b, long_t *f1, long_t *f2) {

}

void solve(long_t n, long_t n1, long_t n2, long_t c1, long_t c2) {
	// Underlying Idea:
	// n1*x + n2*y = n with n1,n2,n given and unknown integers x,y is called
	// a linear diophantine equation. The uva challenge adds a constraint for
	// x and y being positive integers (i.e. x,y >= 0).
	// A linear diophantine equation is said to have solutions if n is divisible
	// by gcd(n1,n2), because then n can be expressed as a linear combination of
	// n1 and n2.

	// Mathematical Aspects:
	// 1. x*n1 + y*n2 = n (linear diophantine equation)
	// 2. g = n/gcd(n1,n2)  (if g is an integer, solutions for above equation exist)
	// 3. gcd(n1,n2) = f1*n1 + f2*n2 (using extended euclid, gcd can be expressed
	//                                as a linear combination of n1 and n2 using
	//                                co-factors f1 and f2)
	// 4. n = g*(f1*n1 + f2*n2)
	// 5. A partial solution would now be g*f1 of the first box n1 and g*f2 of the
	//    second box n2. The problem is that as of now, one of the two solutions
	//    will be negative, because gcd(n1,n2) is smaller than n1 and n2 so in order
	//    to express it as a linear combination of n1 and n2, one has to be subtracted
	//    from the other one. This is why more solutions need to be found from here on.
	// 6. Let x0 and y0 be the partial solutions obtained from step 4. For any integer k
	//    the following equation produces more solutions:
	//    xk = x0 + (k*n2)/gcd(n1,n2) and yk = y0 - (k*n1)/gcd(n1,n2)
	//    It does not matter which of both uses subtraction but they have to complement
	//    each other in order to maintain the balance.

	// Algorithm:
	// 1. Find partial solution by applying step 1-5 previously explained.
	// 2. Apply step 6; generate new solutions by adding the term to the negative solution
	//    and subtracting from the positive solution up to that point where the negative
	//    solution turns positive or the positive solutions turns negative. If now both
	//    are positive we have a winner, otherwise no solution exists.


}

int main() {
	long_t n, n1, n2, c1, c2;

	while (scanf("%d", &n) && n != 0) {
		scanf("%d%d%d%d", &n1, &n2, &c1, &c2);
		solve(n, n1, n2, c1, c2);
	}
	return 0;
}