#include <stdio.h>
#include <limits.h>


void solve(int a, int b) {
	// Vorgehen:
	// 1. kleinere zahl so oft wie möglich von der grösseren abziehen
	// 2. schritt 1 wiederholen bis eine zahl 0 ist
	// 3. die Zahl ungleich null ist ggt und gleichzeitig d
	int ai, bi, a1, a2, b1, b2, f, d;
	ai = a;
	bi = b;
	a1 = 1; a2 = 0;
	b1 = 0; b2 = 1;
	while (a > 0 && b > 0) {
		if (a < b) {
			f = (b/a);
			b -= (f*a1*ai + f*a2*bi);
			b1 -= f*a1;
			b2 -= f*a2;
		} else {
			f = (a/b);
			a -= (f*b1*ai + f*b2*bi);
			a1 -= f*b1;
			a2 -= f*b2;
		}
	}
	if (a > b) {
		printf("%d %d %d\n", a1, a2, a);
	} else {
		printf("%d %d %d\n", b1, b2, b);
	}
}

int main() {
	int a, b;

	while (scanf("%d %d", &a, &b) != EOF) {
		solve(a, b);
	}
	return 0;
}