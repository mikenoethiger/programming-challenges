#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

#define MAX_M 250
#define MAX_N 250

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

void skip_char() {
	char buf;
	read(STDIN_FILENO, &buf, sizeof(buf));
}

void skip_to_endl() {
	char buf;
	while (read(STDIN_FILENO, &buf, sizeof(buf)) && buf != '\n') {}
}

int read_char(char *c) {
	return read(STDIN_FILENO, c, sizeof(*c));
}

char bitmap[MAX_N][MAX_M];
int cache[MAX_N][MAX_M];
int N, M;


void C() {
	int i, j;
	for (i = 0; i < N; i++) {
		for (j = 0; j < M; j++) {
			bitmap[i][j] = 'O';
		}
	}
}

void L(int x, int y, char c) {
	bitmap[y][x] = c;
}

void K(int x1, int x2, int y1, int y2, char c) {
	int i, j;
	for (i = x1; i <= x2; i++) {
		for (j = y1; j <= y2; j++) {
			L(i, j, c);
		}
	}
}

bool valid_coords(int x, int y) {
	return x >= 0 && x < M && y >= 0 && y < N;
}

void F(int x, int y, char c) {
	cache[y][x] = 1;
	int x1, y1, x2, y2, x3, y3, x4, y4;
	x1 = x; y1 = y+1;
	x2 = x; y2 = y-1;
	x3 = x+1; y3 = y;
	x4 = x-1; y4 = y;
	if (valid_coords(x1, y1) && cache[y1][x1] == 0 && bitmap[y1][x1] == bitmap[y][x]) F(x1, y1, c);
	if (valid_coords(x2, y2) && cache[y2][x2] == 0 && bitmap[y2][x2] == bitmap[y][x]) F(x2, y2, c);
	if (valid_coords(x3, y3) && cache[y3][x3] == 0 && bitmap[y3][x3] == bitmap[y][x]) F(x3, y3, c);
	if (valid_coords(x4, y4) && cache[y4][x4] == 0 && bitmap[y4][x4] == bitmap[y][x]) F(x4, y4, c);
	bitmap[y][x] = c;
}

void clear_cache() {
	int i, j;
	for (i = 0; i < N; i++) {
		for (j = 0; j < M; j++) {
			cache[i][j] = 0;
		}
	}
}

void S() {
	char c;
	while (read_char(&c) && c != '\n') {
		printf("%c", c);
	}
	printf("\n");
	int i, j;
	for (i = 0; i < N; i++) {
		for (j = 0; j < M; j++) {
			printf("%c", bitmap[i][j]);
		}
		printf("\n");
	}
}

void swap(int *n1, int *n2) {
	int n3 = *n1;
	*n1 = *n2;
	*n2 = n3;
}

int main() {
	char cmd;
	int x, y, x2, y2;
	char c;
	while (read_char(&cmd)) {
		switch (cmd) {
			case 'I':
				M = read_num(NULL);
				N = read_num(NULL);
				C();
				break;
			case 'C':
				C();
				skip_to_endl();
				break;
			case 'L':
				x = read_num(NULL)-1;
				y = read_num(NULL)-1;
				read_char(&c);
				skip_to_endl();
				L(x, y, c);
				break;
			case 'V':
				x = read_num(NULL)-1;
				y = read_num(NULL)-1;
				y2 = read_num(NULL)-1;
				if (y2 < y) swap(&y2, &y);
				read_char(&c);
				skip_to_endl();
				K(x, x, y, y2, c);
				break;
			case 'H':
				x = read_num(NULL)-1;
				x2 = read_num(NULL)-1;
				if (x2 < x) swap(&x2, &x);
				y = read_num(NULL)-1;
				read_char(&c);
				skip_to_endl();
				K(x, x2, y, y, c);
				break;
			case 'K':
				x = read_num(NULL)-1;
				y = read_num(NULL)-1;
				x2 = read_num(NULL)-1;
				y2 = read_num(NULL)-1;
				if (x2 < x) swap(&x2, &x);
				if (y2 < y) swap(&y2, &y);
				read_char(&c);
				skip_to_endl();
				K(x, x2, y, y2, c);
				break;
			case 'F':
				x = read_num(NULL)-1;
				y = read_num(NULL)-1;
				read_char(&c);
				skip_to_endl();
				clear_cache();
				F(x, y, c);
				break;
			case 'S':
				skip_char();
				S();
				break;
			case 'X':
				exit(0);
			default:
				skip_to_endl();
		}
	}
	return 0;
}