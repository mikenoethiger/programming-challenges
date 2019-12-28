#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

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

bool is_adjacent(int a, int b, int n, int m, int moves[8]) {
	int invalid_positions[8] = {0,0,0,0,0,0,0,0};
	int pos = 0;
	if (a % m == 0) {
		invalid_positions[4] = 1;
		invalid_positions[0] = 1;
		invalid_positions[7] = 1;
	}
	if ((a+1) % m == 0) {
		invalid_positions[5] = 1;
		invalid_positions[2] = 1;
		invalid_positions[6] = 1;
	}
	if (a/m == 0) {
		invalid_positions[4] = 1;
		invalid_positions[1] = 1;
		invalid_positions[5] = 1;
	}
	if (a/m == (n-1)) {
		invalid_positions[7] = 1;
		invalid_positions[3] = 1;
		invalid_positions[6] = 1;
	}
	int i;
	for (i = 0; i < 8; i++) {
		if (invalid_positions[i] && a+moves[i] == b) return false;
	}
	return true;
}

void print_field(char field[][2], int n, int m) {
	int s = m*n;
	int i;
	for (i = 0; i < s; i++) {
		if (i != 0 && i % m == 0) printf("\n");
		printf("%c", field[i][1]);
	}
	printf("\n");
}

void solve(int n, int m) {
	int s = n*m;
	int i;
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
	int moves[8] = { m_l, m_u, m_r, m_d, m_l_u, m_r_u, m_r_d, m_l_d };

	for (i = 0; i < s; i++) {
		field[i][0] = read_char();
		field[i][1] = '0';
	}

	int j, k, mines;
	for (i = 0; i < s; i++) {
		if (i > 0 && i % m == 0) printf("\n");
		if (field[i][0] == '*') {
			printf("*");
		} else {
			mines = 0;
			for (j = 0; j < 8; j++) {
				k = i + moves[j];
				if (is_adjacent(i, k, n, m, moves) && field[k][0] == '*') mines++;
			}
			printf("%d", mines);
		}
	}
}

int main() {
	int n, m, f = 1;
	while ((n = read_num(NULL))) {
		m = read_num(NULL);
		printf("Field #%d:\n", f++);
		solve(n,m);
		printf("\n\n");
	}
	return 0;
}