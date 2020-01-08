#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define SIZE 500
#define COLORS 100

char *faces[] = {"front", "back", "left", "right", "top", "bottom"};

typedef struct part_solution {
	/* if cube above is equal to the cube represented in this solution,
	 * the end of tower is reached */
	int cube; // the cube above this solution
	int side; // the face up side of the cube above
	int height; // the height of this solution
} PartSolution;

PartSolution *part_solution_create(int cube, int side, int height) {
	PartSolution *part_solution = malloc(sizeof(PartSolution));
	part_solution->cube = cube;
	part_solution->side = side;
	part_solution->height = height;
	return part_solution;
}

/*
 * UTILITY FUNCTIONS
 */

int opposite_side(int s) {
	return s % 2 == 0 ? s + 1 : s - 1;
}

/**
 * Read cube data from STDIN and write
 * the color value of each side to buf
 * which has to be an int array of size 6.
 *
 * @param buf
 * @return
 */
void read_cube(int *buf) {
	scanf("%d%d%d%d%d%d", buf, buf+1, buf+2, buf+3, buf+4, buf+5);
}

/**
 * Prints tower matrix, each part solution will be
 * represented by its cube and side index.
 *
 * Can be used for debugging purposes for example
 * to trace the cubes of a tower by following the
 * matrix path.
 *
 * @param towers
 */
void print_towers(PartSolution *towers[SIZE][6]) {
	int i, s;
	for (i = 0; i < SIZE; i++) {
		printf("%d. ", i);
		for (s = 0; s < 6; s++) {
			if (towers[i][s] == NULL) {
				printf("0|0|0 ");
			} else {
				printf("%d|%d|%d ", towers[i][s]->cube, towers[i][s]->side, towers[i][s]->height);
			}
		}
		printf("\n");
	}
	printf("\n");
}

/*
 * ALGORITHM
 */

// cube        := cube color configuration
// towers      := tower backtracking
// towers_c    := color cache; which gives information on the
//                currently tallest tower (identified by the
//                cube index) for any given color
// towers_c[c]    := tallest tower for color c (0..99)
//                   c is the color of the face down side of the cube
// towers_c[c][0] := cube index
// towers_c[c][1] := side index (face up side)
// towers_c[c][2] := absolute height of tower
int cube[6];
PartSolution *towers[SIZE][6];
int towers_c[COLORS][3];

void print_reverse(int i, int s, PartSolution *towers[SIZE][6]) {
	if (towers[i][s]->cube != i) {
		print_reverse(towers[i][s]->cube, towers[i][s]->side, towers);
		printf("%d %s\n", towers[i][s]->cube + 1, faces[towers[i][s]->side]);
	}
}

/**
 * Seconds version for the tower of cubes algorithm, that uses more
 * extensive caching, thus reducing the time complexity from O(n^2)
 * to O(n).
 *
 * The additional cache is maintained in the towers_c array. This
 * array stores a reference to a cube that currently forms the largest
 * tower for a given color c.
 *
 * Whenever we process a new cube, we just consult the color cache for
 * all colors of this cube and extend existing solutions or introduce
 * new if no solution does exist for a color yet.
 *
 * The tower structures are still maintained in the towers array but
 * we now only have to make a single call to the cache in order to
 * find the largest tower that should be extended by a cube.
 *
 * Time Complexity: O(n)
 *
 * @param N the number of cubes
 */
void calc_tallest_tower(int N) {
	// s   := side index
	// ops := opposite side index of s
	// c   := color value of s (0..99)
	// opc := color value of ops (0..99)
	// m   := max tower height
	// mi  := cube index of max tower
	// ms  := side index of cube mi (face up side)
	int s, ops, c, opc;
	int m, mi, ms;
	int i;
	m = 0; mi = 0; ms = 0;

	memset(towers_c, -1, sizeof(towers_c));

	for (i = 0; i < N; i++) {
		read_cube(cube);
		for (s = 0; s < 6; s++) {
			ops = opposite_side(s);
			c = cube[s]-1;
			opc = cube[ops]-1;
			towers[i][s] = part_solution_create(i, s, 1);
			if (towers_c[c][0] == -1) {
				towers_c[c][0] = i;
				towers_c[c][1] = ops;
				towers_c[c][2] = 1;
			} else if (towers_c[c][0] != i) {
				towers[i][s]->cube = towers_c[c][0];
				towers[i][s]->side = towers_c[c][1];
				towers[i][s]->height = towers_c[c][2] + 1;
			}
		}
		// turned out we have to update the towers_c cache in a separate loop
		// otherwise we might lose solutions. If the first side of a cube forms
		// a new tower for any color c, we won't be able to extend the opc by any
		// other side of the same cube who'd have this color, since a cube can't
		// be placed on top of himself.
		for (s = 0; s < 6; s++) {
			c = cube[s]-1;
			ops = opposite_side(s);
			opc = cube[ops]-1;
			if (towers_c[opc][2] < towers[i][s]->height) {
				towers_c[opc][0] = i;
				towers_c[opc][1] = s;
				towers_c[opc][2] = towers[i][s]->height;
			}
			if (towers[i][s]->height > m) {
				mi = i;
				ms = s;
				m = towers[i][s]->height;
			}
		}
	}

	printf("%d\n", m);
	print_reverse(mi, ms, towers);
	printf("%d %s\n", mi+1, faces[ms]);
}

int main() {
	int N;
	int c = 1;
	while (scanf("%d", &N) != EOF) {
		printf("Case #%d\n", c++);
		calc_tallest_tower(N);
		printf("\n");
	}
	return 0;
}