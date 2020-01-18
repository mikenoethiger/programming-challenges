#include <stdio.h>

int main() {

	int i, n;
	scanf(" %n", &n);
	printf("n=%d\n", n);
	scanf("%d%n", &i, &n);
	printf("i=%d n=%d\n", i, n);
	scanf("%d%n", &i, &n);
	printf("i=%d n=%d\n", i, n);

//	int k, n, i;
//	scanf("%d", &k);
//
//	for (i = 0; i < k; i++) {
//		scanf("\n");
//		scanf("%d", &n);
//
//	}
//
//	printf("hello world\n");
	return 0;
}