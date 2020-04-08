#include <stdio.h>

typedef long long_t;

void ggt(long_t n1, long_t n2, long_t *f1, long_t *f2, long_t *ggt) {
	int ai, bi, a1, a2, b1, b2, f, d;
	ai = n1;
	bi = n2;
	a1 = 1;
	a2 = 0;
	b1 = 0;
	b2 = 1;
	while (n1 > 0 && n2 > 0) {
		if (n1 < n2) {
			f = (n2 / n1);
			n2 -= (f * a1 * ai + f * a2 * bi);
			b1 -= f * a1;
			b2 -= f * a2;
		} else {
			f = (n1 / n2);
			n1 -= (f * b1 * ai + f * b2 * bi);
			a1 -= f * b1;
			a2 -= f * b2;
		}
	}
	if (n1 > n2) {
		*f1 = a1;
		*f2 = a2;
		*ggt = n1;
	} else {
		*f1 = b1;
		*f2 = b2;
		*ggt = n2;
	}
}

long_t a_k(long_t a0, long_t k, long_t n2, long_t ggT) {
	return a0+k*n2/ggT;
}

long_t b_k(long_t b0, long_t k, long_t n1, long_t ggT) {
	return b0-k*n1/ggT;
}

void solve(long_t n, long_t n1, long_t n2, long_t c1, long_t c2) {
	if (n1 == n2 && n2 == n) {
		printf("%ld 0\n", n1);
		return;
	}

	long_t alpha, beta, ggT;
	ggt(n1, n2, &alpha, &beta, &ggT);

	if (n % ggT != 0) {
		printf("failed\n");
		return;
	}

	long_t a0 = alpha * n / ggT;
	long_t b0 = beta * n / ggT;
	long_t m = (n2 * c1 - c2 * n1) / ggT;
	long_t k;

	if (m > 0) {
		k = -a0 * ggT / n2;
		if (a_k(a0, k, n2, ggT) < 0) {
			k += 1;
		}
	}
	else {
		k = b0 * ggT / n1;
		if (b_k(b0, k, n1, ggT) < 0) {
			k -= 1;
		}
	}

	long_t ak = a_k(a0, k, n2, ggT);
	long_t bk = b_k(b0, k, n1, ggT);

	if (ak < 0 || bk < 0) {
		printf("failed\n");
		return;
	}

	printf("%ld %ld\n", ak, bk);
}

int main() {
	long_t n, n1, n2, c1, c2;

	while (scanf("%ld", &n) && n != 0) {
		scanf("%ld%ld%ld%ld", &c1, &n1, &c2, &n2);
		solve(n, n1, n2, c1, c2);
	}
	return 0;
}