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

/**
 * Check neighbourship of a with s, s being
 *
 * @param a
 * @param s
 * @param n
 * @param m
 * @param moves
 * @return
 */
bool is_neighbour(int a, int s, int n, int m, int moves[8]) {
	// neighbour indexes of x:
	// 4 1 5
	// 0 x 2
	// 7 3 6
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
	return neighbours[s];
}

void solve(int n, int m) {
	int s = n*m;
	int i, j;
	char buf;
	char field[s][2];

	int m_l = -1;
	int m_r = +1;
	int m_u = -m;
	int m_d = m;
	int m_l_u = m_l + m_u;
	int m_r_u = m_r + m_u;
	int m_l_d = m_l+ m_d;
	int m_r_d = m_r + m_d;
	// adjacence indexes relative to index x:
	// 4 1 5
	// 0 x 2
	// 7 3 6
	int moves[8] = { m_l, m_u, m_r, m_d, m_l_u, m_r_u, m_r_d, m_l_d };

	// read field input data
	for (i = 0; i < s; i++) {
		field[i][0] = read_char();
		field[i][1] = '0';
	}

	int k, mines;
	for (i = 0; i < s; i++) {
		if (i > 0 && i % m == 0) printf("\n");
		if (field[i][0] == '*') {
			printf("*");
		} else {
			mines = 0;
			for (j = 0; j < 8; j++) {
				k = i + moves[j];
				if (is_neighbour(i, j, n, m, moves) && field[k][0] == '*') {
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