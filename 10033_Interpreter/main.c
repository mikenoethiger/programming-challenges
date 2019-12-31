#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

#define REG_SIZE 10
#define RAM_SIZE 1000

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

void skip_char() {
	char buf;
	read(STDIN_FILENO, &buf, sizeof(buf));
}

int read_char(char *buf) {
	return read(STDIN_FILENO, buf, sizeof(*buf));
}

int reg[REG_SIZE];
char ram[RAM_SIZE][3];

void read_ram() {
	char c;
	int i = 0;
	while (read_char(&c) && c != '\n') {
		ram[i][0] = c;
		read_char(&c);
		ram[i][1] = c;
		read_char(&c);
		ram[i++][2] = c;
		read_char(&c);
	}
	while (i < RAM_SIZE) {
		ram[i][0] = '0';
		ram[i][1] = '0';
		ram[i++][2] = '0';
	}
}

void num_char(int num, char *out) {
	int h = num / 100;
	int z = (num / 10) % 10;
	int e = num % 10;
	out[0] = '0' + h;
	out[1] = '0' + z;
	out[2] = '0' + e;
}

int char_num(char *c) {
	int num = (c[0]-'0') * 100;
	num += (c[1]-'0') * 10;
	num += c[2]-'0';
	return num;
}

void clear_register() {
	int i;
	for (i = 0; i < 10; i++) {
		reg[i] = 0;
	}
}

int run() {
	// n := number of executed instructions
	// p := current ram position (index)
	// d := first instruction parameter
	// s := second instruction parameter
	int n = 0;
	int p = 0;
	int d, s;
	while (char_num(ram[p]) != 100) {
		d = ram[p][1] - '0';
		s = ram[p][2] - '0';

		switch (ram[p][0]) {
			case '2':
				reg[d] = s;
				break;
			case '3':
				reg[d] = (reg[d] + s) % 1000;
				break;
			case '4':
				reg[d] = (reg[d] * s) % 1000;
				break;
			case '5':
				reg[d] = reg[s];
				break;
			case '6':
				reg[d] = (reg[d] + reg[s]) % 1000;
				break;
			case '7':
				reg[d] = (reg[d] * reg[s]) % 1000;
				break;
			case '8':
				reg[d] = char_num(ram[reg[s]]);
				break;
			case '9':
				num_char(reg[d], ram[reg[s]]);
				break;
			case '0':
				if (reg[s] != 0) {
					p = reg[d];
					// p-- is a hack to equalize the later p++
					p--;
				}
				break;
			default:
				printf("unknown command %c\n", ram[p][0]);
				exit(1);
		}
		n++; p++;
	}
	n++;

	return n;
}

int main() {
	int n = read_num(NULL);
	int i;
	skip_char();
	for (i = 0; i < n; i++) {
		if (i > 0) printf("\n");
		clear_register();
		read_ram();
		printf("%d\n", run());
	}
	return 0;
}