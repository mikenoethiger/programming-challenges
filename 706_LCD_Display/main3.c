#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

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

int printn(char c, int n) {
	int i;
	for (i = 0; i < n; i++) {
		printf("%c", c);
	}
	return n;
}

int print_horizontal(char sign, int s) {

}

/**
 *
 * @param num_index
 * @param row
 * @param s digit size
 * @param l last column index (of latest print call)
 * @param c current column index (where this function call
 *          should start printing, if it does print)
 * @return
 */
int print_num_row(int num_index, int row, int s, int l, int c) {
	char sign;
	int i;
	// b := buffer of currently printed chars
	// d := difference of last column index and current column index
	int b = 0;
	int d = c-l;
	if (row == 0) {
		// first row
		sign = nums[num_index][0] ? '-' : ' ';
		if (sign == '-') {
			b += printn(' ', d+1);
			b += printn(sign, s);
			b += printn(' ', 1);
		}
	} else if (row >= 1 && row <= s) {
		// first vertical section
		sign = nums[num_index][1] ? '|' : ' ';
		if (sign == '|') {
			b += printn(' ', d);
			printf("%c", sign);
			b++;
		}
		sign = nums[num_index][2] ? '|' : ' ';
		if (sign == '|') {
			b += printn(' ', s+(c-(l+b))+1);
			printf("%c", sign);
			b++;
		}
	} else if (row == s+1) {
		// mid section
		sign = nums[num_index][3] ? '-' : ' ';
		if (sign == '-') {
			b += printn(' ', d+1);
			b += printn(sign, s);
		}
	} else if (row >= s+2 && row <= 2*s+1) {
		// second vertical section
		sign = nums[num_index][4] ? '|' : ' ';
		if (sign == '|') {
			b += printn(' ', d);
			printf("%c", sign);
			b++;
		}
		sign = nums[num_index][5] ? '|' : ' ';
		if (sign == '|') {
			b += printn(' ', s+(c-(l+b))+1);
			printf("%c", sign);
			b++;
		}
	} else if (row == 2*s+2) {
		// last row
		sign = nums[num_index][6] ? '-' : ' ';
		if (sign == '-') {
			b += printn(' ', d+1);
			b += printn(sign, s);
		}
	} else {
		printf("could not detect row section\n");
		exit(1);
	}
	return b;
}

void print_num(int s) {
	int size = 0;
	while (read(STDIN_FILENO, &buf, sizeof(buf)) && buf != '\n') {
		if (buf == ' ' || buf == '\t') continue;
		n[size++] = buf - '0';
	}

	int cols = size*(s+2) + s -1;
	int rows = 2*s+3;
	// w := width in characters of a single digit relative to size s
	int w = s+2;
	char grid[rows][cols];

	int pivot_num = 0;

	int i, j, curr_col;
	for (i = 0; i < rows; i++) {
		curr_col = 0;
		for (j = 0; j < size; j++) {
			curr_col += print_num_row(n[j], i, s, curr_col, j*(w+1));
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