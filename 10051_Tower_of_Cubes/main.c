#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>

#define SIZE 500

char *faces[] = {"front", "back", "left", "right", "top", "bottom"};

/*
 * DATA STRUCTURES
 */

typedef struct part_solution {
	int cube; // the cube below this solution
	int side; // the face up side of the cube below
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
			printf("%d|%d ", towers[i][s]->cube, towers[i][s]->side);
		}
		printf("\n");
	}
}

/*
 * ALGORITHM
 */

int cube[6];
int cubes[SIZE][6];
PartSolution *towers[SIZE][6];

/**
 * The basic idea for the algorithm is to build bigger and bigger
 * towers by putting new cubes on top of existing towers, thereby
 * generating larger towers.
 *
 * We start off with the first cube which will then form six new
 * towers of height 1, one tower for each side of the cube.
 *
 * Then the second cube will form another six towers of height 1,
 * in fact every subsequent tower will introduce six new towers,
 * which is called the initialization step for a pivot cube.
 *
 * After the initialization step, all existing towers will be
 * checked for extensibility against the current pivot cube.
 *
 * Whenever an existing tower can be extended by the pivot cube,
 * we maintain this relationship, which is cube i with side si
 * extends cube j with side sj. In fact we only ever maintain
 * the largest tower that can be formed for a cube i.
 *
 * Besides holding the information of the "largest buildable
 * tower for a cube i", we also maintain a reference to the
 * largest tower of all towers, which is cube mi with side ms.
 *
 * Those two indexes, mi and ms allow us to trace back the final
 * largest tower and print out the results.
 *
 * Time Complexity: O(n^2)
 *
 * @param N the number of cubes
 */
void calc_tallest_tower(int N) {
	// The towers are built from top to bottom
	// by reading and then immediately processing
	// the input from STDIN.

	// m   := max tower height
	// mi  := cube index of max tower
	// ms  := side index of cube mi (face up side)
	// s   := running number for side index
	// i   := running number for cube index
	// j   := second order running number for cube index
	// si  := side index of cube i (face up side)
	// sj  := side index of cube j (face up side)
	// opj := opposite side index of side sj
	int m = 1;
	int mi = 0;
	int ms = 0;
	int s, i, j;
	int si, sj, opj;
	// The i and j for loop (which are the expensive ones), expand as follows:
	//      i = 0,   j = -    (needed for initialization)
	// -    i = 1,   j = 0..0
	// --   i = 2,   j = 0..1
	// ---  i = 3,   j = 0..2
	// ---- i = 4,   j = 0..3
	//      i = N-1, j = 0..N-2
	// The computational cost is equal to the sum of 1...N
	// which is n(n+1)/2 which is in O(n^2)
	for (i = 0; i < N; i++) {
		read_cube(cube);
		// initialization step; add six towers of height 1,
		// one for each side of the cube i.
		for (s = 0; s < 6; s++) {
			cubes[i][s] = cube[s];
			towers[i][s] = part_solution_create(i, 0, 1);
		}

		for (j = 0; j < i; j++) {
			// for each side of cube i, check whether it fits below any existing tower sj
			for (si = 0; si < 6; si++) for (sj = 0; sj < 6; sj++) {
				opj = opposite_side(sj);
				// if the cube colors match, and the resulting new tower is bigger
				// then the current one stored for cube i, we replace the current
				// one with the new one.
				if (cubes[j][opj] == cubes[i][si] && towers[i][si]->height < towers[j][sj]->height + 1) {
					towers[i][si]->cube = j;
					towers[i][si]->side = sj;
					towers[i][si]->height = towers[j][sj]->height + 1;
					// maintain a reference to the overall largest tower
					if (towers[i][si]->height > m) {
						m = towers[i][si]->height;
						mi = i;
						ms = si;
					}
				}
			}
		}
	}

	// Reverse the bottom to top structure to make it
	// a top to bottom structure which is an output
	// requirement of the programming challenge.
	Stack *stack = stack_create();
	int tmp_i;
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