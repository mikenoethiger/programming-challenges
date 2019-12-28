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

/**
 * Reads a floating point number from STDIN as follows:
 * - Leading white spaces, line breaks and tabs will be skipped.
 * - A leading '0' will be returned immediately.
 * - A leading '-' will multiply the subsequent number with -1.
 * - The first '.' will mark the decimal point
 * - The first character from STDIN that is not '0' <= c <= '9'
 *   and not '.' will terminate the algorithm. This character
 *   will be written to the out parameter t.
 *
 * @param t (nullable) out parameter that will hold the terminating
 *          character.
 * @return the number or zero if not a number
 */
double read_double(char *t) {
	char buf;
	double num = 0;
	int it = -1;
	int coeff = 1;
	while (read(STDIN_FILENO, &buf, sizeof(buf))) {
		it++;
		if (t != NULL) *t = buf;
		if (num == 0 && (buf == ' ' || buf == '\n' || buf == '\t')) continue;
		if (it == 0 && buf == '-') {
			coeff = -1;
			continue;
		}
		if (!('0' <= buf && buf <= '9')) break;
		num *= 10;
		num += buf - '0';
	}
	if (buf == '.') {
		int divisor = 10;
		while (read(STDIN_FILENO, &buf, sizeof(buf))) {
			if (t != NULL) *t = buf;
			if (!('0' <= buf && buf <= '9')) break;
			num += (double)(buf - '0') / divisor;
			divisor *= 10;
		}
	}
	return num * coeff;
}

double s[1000];
double total, avg, diff, neg, pos;
int i;

void solve(int n) {
	// e   := expenses
	// te  := total expenses
	// avg := per person expenses
	total = .0;

	for (i = 0; i < n; i++) {
		s[i] = read_double(NULL);
		total += s[i];
	}

	avg = total / n;

	neg = 0;
	pos = 0;
	for (i = 0; i < n; i++) {
		diff = (double) (long) ((s[i]-avg) * 100.0) / 100.0;
		if (diff > 0) {
			pos += diff;
		} else {
			neg += diff;
		}
	}

	// fixes weird bug when neg is negative 0
	if (neg == -0) neg = 0;

	printf("$%.2f\n", pos >= -neg ? pos : (-neg));
}

int main() {
	int n;
	while ((n = read_num(NULL))) {
		solve(n);
	}

	return 0;
}