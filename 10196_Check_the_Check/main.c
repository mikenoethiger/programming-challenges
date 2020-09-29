#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#define BOARD_WIDTH 8
#define BOARD_HEIGHT 8
#define BOARD_SIZE 64
#define BLACK 0
#define WHITE 1

#define DEBUG_TIMEOUT 1

/*
 * UTILITY
 */

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

/*
 * DATA STRUCTURE
 */

char board[BOARD_SIZE];
const int m_up = -BOARD_WIDTH;
const int m_down = BOARD_WIDTH;
const int m_left = -1;
const int m_right = 1;
// m_p := movements pawn
// m_n := movements knight
// m_b := movements bishop
// m_r := movements rook
// m_q := movements queen
// m_k := movements king (equal to m_q)
// IDEA: Store single moves in an array
//       instead of summing them up.
// 		 Then these moves can be processed
// 		 sequentially which allows to check
// 		 for adjacency

// Moves indexed from 0 to 7:
// 405
// 3x1
// 726
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

bool debug = false;

/*
 * ALGORITHM
 */

/**
 * Read board data from STDIN and write to
 * global board variable.
 *
 * @return true if the board does not consist
 *         of '.' only.
 */
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
		// skip new lines
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

/**
 * Executes move m from position i if and
 * only if it is a valid movement i.e. the
 * new position is adjacent to i.
 *
 * @param i the position on the board (0..63)
 * @param m the move index (0..7):
 *          405
 *          3x1
 *          726
 * @param j out parameter that will contain the new position
 *          (if the move is invalid, j will be set to i)
 * @return 1 if a valid move, otherwise 0
 */
int move(int i, int m, int *j) {
	// vn := valid neighbour positions
	// w  := board width
	// h  := board height
	int vn[8] = {1,1,1,1,1,1,1,1};
	int w = BOARD_WIDTH;
	int h = BOARD_HEIGHT;

	// left edge
	if (i % w == 0) {
		vn[4] = 0;
		vn[3] = 0;
		vn[7] = 0;
	}
	// right edge
	if ((i+1) % w == 0) {
		vn[5] = 0;
		vn[1] = 0;
		vn[6] = 0;
	}
	// top edge
	if (i/w == 0) {
		vn[4] = 0;
		vn[0] = 0;
		vn[5] = 0;
	}
	// bottom edge
	if (i/w == (h-1)) {
		vn[7] = 0;
		vn[2] = 0;
		vn[6] = 0;
	}
	*j = vn[m] ? i + moves[m] : i;
	return vn[m];
}

/**
 * Execute multiple moves.
 *
 * @param i the start position on the board (0..63)
 * @param m_arr array of move indexes
 * @param m_n size of m
 * @param j out parameter that will contain the new position
 *          (if the move is invalid, j will be set to i)
 * @return 1 if a valid sequence of moves, otherwise 0
 */
int move_n(int i, int *m_arr, int m_n, int *j) {
	// k := iterator
	int k;
	*j = i;
	for (k = 0; k < m_n; k++) {
		if (!move(*j, m_arr[k], j)) return 0;
	}
	return 1;
}

/**
 * Repeat a move starting from i until it gets invalid
 * (i.e. out of board) or you land on a field that is
 * not a '.'
 *
 * @param i the start position on the board (0..63)
 * @param m the move index (0..7)
 * @param j out parameter that will contain the final position
 * @return the number of moves executed
 */
int move_r(int i, int m, int *j) {
	*j = i;
	int moves = 0;
	while (move(*j, m, j) && board[*j] == '.') {
		moves++;
	}
	return moves;
}

/**
 *
 * @param i field position of attacker
 * @param a the attacking figure (always lower case)
 * @param t target figure ('k' or 'K')
 * @param p player in charge (0=black, 1=white)
 * @return
 */
int can_attack(int i, char a, char t, int p) {
	// ca := can attack
	// j  := field to attack (board index)
	// k  := iterating index
	int ca = 0;
	int j, k;
	switch (a) {
		case 'p':
			// check 2 pawn moves
			if (move(i, m_p[p][0], &j) && board[j] == t) ca = 1;
			if (move(i, m_p[p][1], &j) && board[j] == t) ca = 1;
			break;
		case 'n':
			// check 8 knight moves
			for (k = 0; k < 8; k++) {
				if (move_n(i, m_n[k], 3, &j) && board[j] == t) {
					ca = 1;
					break;
				}
			}
			break;
		case 'b':
			// check 4 recursive bishop moves
			for (k = 0; k < 4; k++) {
				move_r(i, m_b[k], &j);
				if (board[j] == t) ca = 1;
			}
			break;
		case 'r':
			// check 4 recursive rook moves
			for (k = 0; k < 4; k++) {
				move_r(i, m_r[k], &j);
				if (board[j] == t) ca = 1;
			}
			break;
		case 'q':
			// check 8 recursive queen moves
			for (k = 0; k < 8; k++) {
				move_r(i, m_q[k], &j);
				if (board[j] == t) ca = 1;
			}
			break;
		case 'k':
			// check 8 king moves
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
	// 'A' to 'Z' = 65 to 90
	// 'a' to 'z' = 97 to 122
	// 'A'+32     = 'a'
	// Black, top,	 	lower case letters
	// White, bottom,	upper case letters

	// check[0] := black is in check
	// check[1] := white is in check
	// pa       := attacking player (0=black, 1=white)
	// pd       := defending player (0=black, 1=white)
	// a		:= attacking figure char
	// t		:= target figure char ('k' or 'K')
	int i;
	int check[2] = {0, 0};
	int pa, pd;
	char a, t;
	for (i = 0; i < BOARD_SIZE; i++) {
		if (board[i] != '.') {
			// assuming black is attacking player
			a = board[i];
			pa = 0; pd = 1;
			t = 'K';
			if (a < 97) {
				// white is attacking player
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
		// skip line break
		read(STDIN_FILENO, &buf, sizeof(buf));
	}
	return 0;
}