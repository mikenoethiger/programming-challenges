#include <stdio.h>

int main(int argc, char *argv[]) {
	int i, k, m;
	char c1, c2;
	i = k = 0;
//	m = scanf("%d%c%d%c", &i, &c1, &k, &c2);
	scanf("%d%c", &i, &c1);
	if (c1 == '\n') {
		printf("%d\n", i);
		return 0;
	}
	scanf("%d", &k);
	printf("%d %d\n", i, k);
	return 0;
}