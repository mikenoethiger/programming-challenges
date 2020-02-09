#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#define PROG_NAME "stretch"

/* data types */
typedef unsigned long long_t;

/* options */
bool o_help = false;
long_t o_steps = 1;

void print_usage() {
	printf("Usage: %s [-h] [-s num] n1 [n2]\n", PROG_NAME);
	printf("    n1  Lower limit (if n2 provided), otherwise upper limit\n");
	printf("    n2  Upper limit (optional)\n");
	printf("        If n2 is not provided, n1 will be taken as upper limit and 1 as lower limit\n");
	printf("    -h  Show usage\n");
	printf("    -s  Increment steps (default 1)\n");
	printf("\n");
	printf("AUTHORS\n");
	printf("    Mike Noethiger (noethiger.mike@gmail.com)\n");
	printf("\n");
	printf("DESCRIPTION\n");
	printf("    Creates a sequence of integers from lower limit to upper limit using the specified step size.\n");
	printf("    Time Complexity: O(n).\n");
}

/* algorithm */

void stretch(long_t l, long_t u, long_t s) {
	long_t n;
	for (n = l; n <= u; n += s) {
		printf("%ld ", n);
	}
}

/* program glue/shell */

void parse_opt(int argc, char *argv[]) {
	int opt;
	while ((opt = getopt(argc, argv, "hs:")) != -1) {
		switch (opt) {
			case 'h':
				o_help = true;
				break;
			case 's':
				o_steps = atol(optarg);
				break;
			default:
				print_usage();
				exit(EXIT_FAILURE);
		}
	}
}

int main(int argc, char *argv[]) {
	parse_opt(argc, argv);
	if (o_help) {
		print_usage();
		exit(0);
	}
	// l     := lower limit
	// u     := upper limit
	// argc2 := number of arguments after options have been processed
	// bug   := char buffer used for scanf
	long_t l, u;
	int argc2;
	char buf;

	argc2 = argc-optind;
	l = 1;

	// following if chain is simply to determine lower and upper limits which can be provided
	// via program arguments or stdin and upper limit being an optional value.
	if (argc2 > 2) {
		printf("exactly 1 or 2 arguments expected\n");
		exit(EXIT_FAILURE);
	} else if (argc2 < 1) {
		// read arg(s) from stdin
		scanf("%ld%c", &u, &buf);
		if (buf != '\n') {
			// two one arguments provided (lower and upper limit)
			l = u;
			scanf("%ld", &u);
		}
	} else {
		u = atol(argv[optind]);
		if (argc2 == 2) {
			l = u;
			u = atol(argv[optind+1]);
		}
	}
	stretch(l, u, o_steps);
	printf("\n");
	return 0;
}