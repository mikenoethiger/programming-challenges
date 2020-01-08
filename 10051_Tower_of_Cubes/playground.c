#include <stdio.h>
#include <string.h>

int main() {
	int i, k;
	int towers_c[100][3];
	printf("%ld\n", sizeof(towers_c));
	memset(&i, -1, sizeof(i));
	printf("%d\n", i);

	memset(towers_c, -1, sizeof(towers_c));

	for (i = 0; i < 100; i++) {
		printf("%d %d %d ", towers_c[i][0], towers_c[i][1], towers_c[i][2]);
	}
	printf("\n");

	int cube[6];
	scanf("%d%d%d%d%d%d", cube, cube+1, cube+2, cube+3, cube+4, cube+5);
	printf("%d %d %d %d %d %d\n", cube[0], cube[1], cube[2], cube[3], cube[4], cube[5]);

	return 0;
}