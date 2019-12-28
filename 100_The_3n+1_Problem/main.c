#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

/**
 * Read a positive number (excluding 0) from STDIN.
 * Stops at the first position which is not a digit
 * between 1..9
 *
 * @return the number or 0 if nothing to read from STDIN.
 */
int read_num() {
	char buf;
	int num = 0;
	while (read(STDIN_FILENO, &buf, sizeof(buf))) {
		/* skip all leading white spaces / new lines */
		if ((buf == ' ' | buf == '\n') && num == 0) continue;
		if (!('0' <= buf && buf <= '9')) return num;
		num *= 10;
		num += buf - '0';
	}
	return num;
}

int calc_cycle_length(int cycle, int num) {
	if (num == 1) return cycle;
	if (num%2 == 0) {
		return calc_cycle_length(++cycle, num/2);
	} else {
		return calc_cycle_length(++cycle, 3*num+1);
	}
}

int main() {
	char buf;
	int a = read_num();
	int b = read_num();
	int longest_cycle = 0;
	int cycle;
	while (a > 0) {
		int i;
		int start = a < b ? a : b;
		int end = a > b ? a : b;
		for (i = start; i <= end; i++) {
			cycle = calc_cycle_length(1, i);
			if (cycle > longest_cycle) longest_cycle = cycle;
		}
		printf("%d %d %d\n", a, b, longest_cycle);
		longest_cycle = 0;
		a = read_num();
		b = read_num();
	}
	return 0;
}