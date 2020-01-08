#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>

#define SIZE 500
#define COLORS 100

char *faces[] = {"front", "back", "left", "right", "top", "bottom"};

/*
 * DATA STRUCTURES
 */

typedef struct part_solution {
	/* if cube above is equal to the cube represented in this solution,
	 * the end of tower is reached */
	int cube; // the cube above this solution
	int side; // the face up side of the cube above
	int height; // the height of this solution
} PartSolution;

typedef struct stack_element {
	void *value;
	struct stack_element *prev;
} StackElement;

typedef struct stack {
	StackElement *top;
} Stack;

PartSolution *part_solution_create(int cube, int side, int height) {
	PartSolution *part_solution = malloc(sizeof(PartSolution));
	part_solution->cube = cube;
	part_solution->side = side;
	part_solution->height = height;
	return part_solution;
}

StackElement *stack_element_create(void *value) {
	StackElement *stack_element = malloc(sizeof(StackElement));
	stack_element->value = value;
	stack_element->prev = NULL;
	return stack_element;
}

void stack_element_delete(StackElement *stack_element) {
	free(stack_element);
}

Stack *stack_create() {
	Stack *stack = malloc(sizeof(Stack));
	stack->top = NULL;
	return stack;
}

void stack_delete(Stack *stack) {
	StackElement *stack_element = stack->top;
	while (stack_element) {
		stack_element_delete(stack_element);
		stack_element = stack_element->prev;
	}
	free(stack);
}

void stack_push(Stack *stack, void *value) {
	StackElement *stack_element = stack_element_create(value);
	if (stack->top != NULL) {
		stack_element->prev = stack->top;
	}
	stack->top = stack_element;
}

void *stack_pop(Stack *stack) {
	if (stack->top == NULL) return NULL;
	StackElement *stack_element = stack->top;
	void *value = stack_element->value;
	stack->top = stack_element->prev;
	stack_element_delete(stack_element);
	return value;
}

/*
 * UTILITY FUNCTIONS
 */

int opposite_side(int s) {
	return s % 2 == 0 ? s + 1 : s - 1;
}

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
 * Read cube data from STDIN and write
 * the color value of each side to buf
 * which has to be an int array of size 6.
 *
 * @param buf
 * @return
 */
void read_cube(int *buf) {
	int i;
	for (i = 0; i < 6; i++) {
		buf[i] = read_num(NULL);
	}
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
 * all colors of the cube and extend existing solutions or introduce
 * new if there's doesn't exist a solution for a color yet.
 *
 * The tower structures are still maintained in the towers array.
 *
 * Time Complexity: O(n^2)
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
	for (i = 0; i < COLORS; i++) {
		towers_c[i][0] = -1;
		towers_c[i][1] = -1;
		towers_c[i][2] = -1;
	}
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
		// otherwise we might lose solutions. Because if the first side of a cube
		// builds a new biggest tower for any color c, we won't be able to extend
		// this color c by any other side of the same cube who'd have this color,
		// since a cube can't be placed onto himself.
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


	int tmp_i;
	Stack *stack = stack_create();
	stack_push(stack, part_solution_create(mi, ms, m));
	while (towers[mi][ms]->cube != mi) {
		stack_push(stack, towers[mi][ms]);
		tmp_i = mi;
		mi = towers[mi][ms]->cube;
		ms = towers[tmp_i][ms]->side;
	}

	PartSolution *p;
	printf("%d\n", m);
	for (i = 0; i < m; i++) {
		p = (PartSolution*) stack_pop(stack);
		printf("%d %s\n", p->cube + 1, faces[p->side]);
	}
}

int main() {
	int N;
	int c = 1;
	while ((N = read_num(NULL))) {
		printf("Case #%d\n", c++);
		calc_tallest_tower(N);
		printf("\n");
	}
	return 0;
}