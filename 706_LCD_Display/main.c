#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <math.h>

/**
 * Reads a number from STDIN as follows:
 * - Leading white spaces, line breaks and tabs will be skipped.
 * - A leading '0' will be returned immediately.
 * - A leading '-' will multiply the subsequent number with -1.
 * - The first character from STDIN that is not '0' <= c <= '9'
 *   will terminate the algorithm. This character will be written
 *   to the out parameter t.
 *
 * @param t (nullable) out parameter that will hold the terminating
 *          character.
 * @return the number or zero if not a number
 */
int read_num(char *t) {
	char buf;
	int num = 0;
	int it = -1;
	int coeff = 1;
	while (read(STDIN_FILENO, &buf, sizeof(buf))) {
		it++;
		if (t != NULL) *t = buf;
		if (num == 0 && (buf == ' ' || buf == '\n' || buf == '\t')) continue;
		if (num == 0 && buf == '0') break;
		if (it == 0 && buf == '-') {
			coeff = -1;
			continue;
		}
		if (!('0' <= buf && buf <= '9')) break;
		num *= 10;
		num += buf - '0';
	}
	return num * coeff;
}

// structure:
// x0x  row: 0
// 1x2  row: 1 to s
// x3x  row: s+1
// 4x5  row: s+2 to 2s+1
// x6x  row: 2s+2
//           0 1 2 3 4 5 6
int n0[7] = {1,1,1,0,1,1,1};
int n1[7] = {0,0,1,0,0,1,0};
int n2[7] = {1,0,1,1,1,0,1};
int n3[7] = {1,0,1,1,0,1,1};
int n4[7] = {0,1,1,1,0,1,0};
int n5[7] = {1,1,0,1,0,1,1};
int n6[7] = {1,1,0,1,1,1,1};
int n7[7] = {1,0,1,0,0,1,0};
int n8[7] = {1,1,1,1,1,1,1};
int n9[7] = {1,1,1,1,0,1,1};
int *nums[10] = {n0, n1, n2, n3, n4, n5, n6, n7, n8, n9};
int n[10];
char buf;

void print_num_row(int num_index, int row, int s, int is_last_num, int is_first_num) {
	char sign;
	int i;
	if (row == 0) {
		// first row
		sign = nums[num_index][0] ? '-' : ' ';
		if (is_last_num && sign == ' ') return;
		printf(" ");
		if (!is_first_num) printf("  ");
		for (i = 0; i < s; i++) printf("%c", sign);
		if (!is_last_num) printf(" ");
	} else if (row >= 1 && row <= s) {
		// first vertical section
		if (!is_first_num) printf("  ");
		sign = nums[num_index][1] ? '|' : ' ';
		printf("%c", sign);
		sign = nums[num_index][2] ? '|' : ' ';
		if (sign == ' ' && is_last_num) return;
		for (i = 0; i < s; i++) printf(" ");
		printf("%c", sign);
	} else if (row == s+1) {
		// mid section
		if (!is_first_num) printf("  ");
		printf(" ");
		sign = nums[num_index][3] ? '-' : ' ';
		for (i = 0; i < s; i++) printf("%c", sign);
		if (!is_last_num) printf(" ");
	} else if (row >= s+2 && row <= 2*s+1) {
		// second vertical section
		if (!is_first_num) printf("  ");
		sign = nums[num_index][4] ? '|' : ' ';
		printf("%c", sign);
		sign = nums[num_index][5] ? '|' : ' ';
		if (sign == ' ' && is_last_num) return;
		for (i = 0; i < s; i++) printf(" ");
		printf("%c", sign);
	} else if (row == 2*s+2) {
		// last row
		if (!is_first_num) printf("  ");
		printf(" ");
		sign = nums[num_index][6] ? '-' : ' ';
		for (i = 0; i < s; i++) printf("%c", sign);
		if (!is_last_num) printf(" ");
	}
}

void print_num(int s) {
	int size = 0;
	while (read(STDIN_FILENO, &buf, sizeof(buf)) && buf != '\n') {
		if (buf == ' ' || buf == '\t') continue;
		n[size++] = buf - '0';
	}

	int cols = size*(s+2) + s -1;
	int rows = 2*s+3;
	char grid[rows][cols];

	int pivot_num = 0;

	int i, j;
	for (i = 0; i < rows; i++) {
		for (j = 0; j < size; j++) {
			print_num_row(n[j], i, s, j == size-1, j == 0);
//			if (j < size-1) printf(" ");
		}
		printf("\n");
	}
}

int main() {
	int s;
	while ((s = read_num(NULL))) {
		print_num(s);
		printf("\n");
	}

	return 0;
}