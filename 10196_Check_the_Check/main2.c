#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

#define BOARD_WIDTH 8
#define BOARD_HEIGHT 8
#define BOARD_SIZE 64
#define BLACK 0
#define WHITE 1

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


char board[BOARD_SIZE];
const int m_up = -BOARD_WIDTH;
const int m_down = BOARD_WIDTH;
const int m_left = -1;
const int m_right = 1;

int moves[8] = {m_up, m_right, m_down, m_left,
				m_up + m_left, m_up + m_right,
				m_down + m_right, m_down + m_left};
char *move_names[8] = {"up", "right", "down", "left", "up-left", "up-right", "down-right", "down-left"};

int m_p[2][2] = {{7, 6},
				 {4, 5}};
int m_n[8][3] = {{0, 0, 3},
				 {0, 0, 1},
				 {1, 1, 0},
				 {1, 1, 2},
				 {2, 2, 3},
				 {2, 2, 1},
				 {3, 3, 0},
				 {3, 3, 2}};

int m_b[4] = {4, 5, 6, 7};
int m_r[4] = {0, 1, 2, 3};
int m_q[8] = {0, 1, 2, 3, 4, 5, 6, 7};


bool read_board() {
	int i, j, k;
	char buf;
	bool dot_only = true;
	for (i = 0; i < BOARD_HEIGHT; i++) {
		for (j = 0; j < BOARD_WIDTH; j++) {
			k = i * BOARD_WIDTH + j;
			read(STDIN_FILENO, &board[k], sizeof(char));
			if (dot_only && board[k] != '.') dot_only = false;
		}
		read(STDIN_FILENO, &buf, sizeof(char));
	}
	return !dot_only;
}

void print_board() {
	int i;
	for (i = 0; i < BOARD_SIZE; i++) {
		if (i != 0 && i % BOARD_WIDTH == 0) printf("\n");
		printf("%c", board[i]);
	}
	printf("\n");
}

int move(int i, int m, int *j) {
	int vn[8] = {1,1,1,1,1,1,1,1};
	int w = BOARD_WIDTH;
	int h = BOARD_HEIGHT;

	if (i % w == 0) {
		vn[4] = 0;
		vn[3] = 0;
		vn[7] = 0;
	}
	if ((i+1) % w == 0) {
		vn[5] = 0;
		vn[1] = 0;
		vn[6] = 0;
	}
	if (i/w == 0) {
		vn[4] = 0;
		vn[0] = 0;
		vn[5] = 0;
	}
	if (i/w == (h-1)) {
		vn[7] = 0;
		vn[2] = 0;
		vn[6] = 0;
	}
	*j = vn[m] ? i + moves[m] : i;
	return vn[m];
}

int move_n(int i, int *m_arr, int m_n, int *j) {
	int k;
	*j = i;
	for (k = 0; k < m_n; k++) {
		if (!move(*j, m_arr[k], j)) return 0;
	}
	return 1;
}

int move_r(int i, int m, int *j) {
	*j = i;
	int moves = 0;
	while (move(*j, m, j) && board[*j] == '.') {
		moves++;
	}
	return moves;
}

int can_attack(int i, char a, char t, int p) {
	int ca = 0;
	int j, k;
	switch (a) {
		case 'p':
			if (move(i, m_p[p][0], &j) && board[j] == t) ca = 1;
			if (move(i, m_p[p][1], &j) && board[j] == t) ca = 1;
			break;
		case 'n':
			for (k = 0; k < 8; k++) {
				if (move_n(i, m_n[k], 3, &j) && board[j] == t) {
					ca = 1;
					break;
				}
			}
			break;
		case 'b':
			for (k = 0; k < 4; k++) {
				move_r(i, m_b[k], &j);
				if (board[j] == t) ca = 1;
			}
			break;
		case 'r':
			for (k = 0; k < 4; k++) {
				move_r(i, m_r[k], &j);
				if (board[j] == t) ca = 1;
			}
			break;
		case 'q':
			for (k = 0; k < 8; k++) {
				move_r(i, m_q[k], &j);
				if (board[j] == t) ca = 1;
			}
			break;
		case 'k':
			for (k = 0; k < 8; k++) {
				if (move(i, m_q[k], &j) && board[j] == t) {
					ca = 1;
					break;
				}
			}
			break;
		default:
			printf("unknown figure '%c'\n", a);
			exit(1);
	}
	return ca;
}

void check() {
	int i;
	int check[2] = {0, 0};
	int pa, pd;
	char a, t;
	for (i = 0; i < BOARD_SIZE; i++) {
		if (board[i] != '.') {
			a = board[i];
			pa = 0; pd = 1;
			t = 'K';
			if (a < 97) {
				pa = 1; pd = 0;
				a += 32;
				t = 'k';
			}
			if (!check[pd]) {
				if (can_attack(i, a, t, pa)) check[pd] = 1;
			}
		}
	}
	if (check[0]) {
		printf("black king is in check.\n");
	} else if (check[1]) {
		printf("white king is in check.\n");
	} else {
		printf("no king is in check.\n");
	}
}

int main() {
	int i = 1;
	char buf;
	while (read_board()) {
		printf("Game #%d: ", i++);
		check();
		read(STDIN_FILENO, &buf, sizeof(buf));
	}
	return 0;
}