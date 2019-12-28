#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

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

char read_char() {
	char buf;
	while (read(STDIN_FILENO, &buf, sizeof(buf))) {
		if (buf == '\n' || buf == '\t' || buf == ' ') continue;
		break;
	}
	return buf;
}

bool is_neighbour(int a, int p, int n, int m, int permuations[8]) {
	int neighbours[8] = {1,1,1,1,1,1,1,1};
	// left edge
	if (a % m == 0) {
		neighbours[4] = 0;
		neighbours[0] = 0;
		neighbours[7] = 0;
	}
	// right edge
	if ((a+1) % m == 0) {
		neighbours[5] = 0;
		neighbours[2] = 0;
		neighbours[6] = 0;
	}
	// top edge
	if (a/m == 0) {
		neighbours[4] = 0;
		neighbours[1] = 0;
		neighbours[5] = 0;
	}
	// bottom edge
	if (a/m == (n-1)) {
		neighbours[7] = 0;
		neighbours[3] = 0;
		neighbours[6] = 0;
	}
	return neighbours[p];
}

void solve(int n, int m) {
	int s = n*m;
	int i, j;
	char buf;
	char field[s];

	// Move permutations to move a given point
	// in a two dimensional grid to one of its
	// eight adjacence points. The permuations
	// must be applied to two dimensional data
	// stored in a one dimensional array
	int m_l = -1;
	int m_r = +1;
	int m_u = -m;
	int m_d = m;
	// access a permutation value using the index
	// from the following diagram.
	// (e.g. 0 is the left permuation)
	int permuations[8] = { m_l, m_u, m_r, m_d, m_l + m_u, m_r + m_u, m_r + m_d, m_l + m_d };

	// read field input data
	for (i = 0; i < s; i++) {
		field[i] = read_char();
	}

	int k, p, mines;
	for (i = 0; i < s; i++) {
		if (i > 0 && i % m == 0) printf("\n");
		if (field[i] == '*') {
			printf("*");
		} else {
			mines = 0;
			for (p = 0; p < 8; p++) {
				k = i + permuations[p];
				if (is_neighbour(i, p, n, m, permuations) && field[k] == '*') {
					mines++;
				}
			}
			printf("%d", mines);
		}
	}
}

int main() {
	int n, m, f = 1;
	while ((n = read_num(NULL))) {
		m = read_num(NULL);
		if (f > 1) printf("\n");
		printf("Field #%d:\n", f++);
		solve(n,m);
		printf("\n");
	}
	return 0;
}