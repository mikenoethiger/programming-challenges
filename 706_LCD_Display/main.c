#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

#define HORIZONTAL '-'
#define VERTICAL '|'

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

// structures of a digit labelled with indexes:
//             height
// x0x  row 0: 0
// 1x2  row 1: 1 to s
// x3x  row 2: s+1
// 4x5  row 3: s+2 to 2s+1
// x6x  row 4: 2s+2
// index     0  1  2  3  4  5  6
int n0[7] = {1, 1, 1, 0, 1, 1, 1};
int n1[7] = {0, 0, 1, 0, 0, 1, 0};
int n2[7] = {1, 0, 1, 1, 1, 0, 1};
int n3[7] = {1, 0, 1, 1, 0, 1, 1};
int n4[7] = {0, 1, 1, 1, 0, 1, 0};
int n5[7] = {1, 1, 0, 1, 0, 1, 1};
int n6[7] = {1, 1, 0, 1, 1, 1, 1};
int n7[7] = {1, 0, 1, 0, 0, 1, 0};
int n8[7] = {1, 1, 1, 1, 1, 1, 1};
int n9[7] = {1, 1, 1, 1, 0, 1, 1};
int *structures[10] = {n0, n1, n2, n3, n4, n5, n6, n7, n8, n9};
// bounds := defines the height boundaries for each row relative to size s
//           bounds are defined per structure index, thus row 1 and 3 are
//           covered twice by structure index 1,2 and 4,5
// digits := the digits to print on console
int bounds[7];
int digits[10];
char buf;

int printn(char c, int n) {
	int i;
	for (i = 0; i < n; i++) {
		printf("%c", c);
	}
	return n;
}

/**
 * Print a horizontal section of a digit.
 * In the following diagram, the index 0, 3 and 6
 * represent horizontal sections of a digit:
 * x0x
 * 1x2
 * x3x
 * 4x5
 * x6x
 *
 * @param sign character used for printing
 * @param s digit size
 * @param l last column index
 * @param c current column index
 * @return number of printed characters
 */
int print_horizontal_row(char sign, int s, int l, int c) {
	// n := number of printed characters
	// d := difference of last column index and current column index
	int n = 0;
	int d = c - l;
	n += printn(' ', d + 1);
	n += printn(sign, s);
	n += printn(' ', 1);
	return n;
}

/**
 * Print a vertical section of a digit.
 * In the following diagram, the indexes 1,2,4 and 5
 * represent vertical sections of a digit:
 * x0x
 * 1x2
 * x3x
 * 4x5
 * x6x
 *
 * @param sign character used for printing
 * @param s digit size
 * @param l last column index
 * @param c current column index
 * @return
 */
int print_vertical_row(char sign1, char sign2, int s, int l, int c) {
	// n := buffer of currently printed chars
	// d := difference of last column index and current column index
	int n = 0;
	int d = c - l;
	n += printn(' ', d);
	if (sign1 != ' ') {
		printf("%c", sign1);
		n++;
	}
	n += printn(' ', s + (c - (l + n)) + 1);
	printf("%c", sign2);
	n++;
	return n;
}

/**
 *
 * @param si structure index
 * @param row
 * @param s digit size
 * @param l last column index
 * @param c current column index
 * @return the number of printed characters
 */
int print_row(int si, int row, int s, int l, int c) {
	char sign, sign2;
	// n := number of printed characters
	// b := boundary index to
	// d := difference of last column index and current column index
	int n = 0;
	int b;
	int d = c - l;

	int i;
	for (i = 0; i < 7; i++) {
		if (row <= bounds[i]) {
			b = i;
			break;
		}
	}
	if (b % 3 == 0) {
		sign = structures[si][b] ? HORIZONTAL : ' ';
		n += print_horizontal_row(sign, s, l, c);
	} else {
		sign = structures[si][b] ? VERTICAL : ' ';
		sign2 = structures[si][b+1] ? VERTICAL : ' ';
		n += print_vertical_row(sign, sign2, s, l, c);
	}

	return n;
}

void print_num(int s) {
	// n := number of digits
	int n = 0;
	while (read(STDIN_FILENO, &buf, sizeof(buf)) && buf != '\n') {
		if (buf == ' ' || buf == '\t') continue;
		digits[n++] = buf - '0';
	}

	// w := width in characters of a single digit relative to size s
	// h := height in characters of a single digit relative to size s
	// l := last column index (i.e. index of last print statement)
	int w = s + 2;
	int h = 2 * s + 3;
	int l;
	int i, j;

	bounds[0] = 0;
	bounds[1] = s;
	bounds[2] = bounds[1];
	bounds[3] = s+1;
	bounds[4] = 2 * s + 1;
	bounds[5] = bounds[4];
	bounds[6] = 2 * s + 2;

	// write digits row by row
	for (i = 0; i < h; i++) {
		l = 0;
		for (j = 0; j < n; j++) {
			l += print_row(digits[j], i, s, l, j * (w + 1));
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