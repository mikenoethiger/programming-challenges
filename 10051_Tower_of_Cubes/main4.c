#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>

#define SIZE 500
#define COLORS 100

char *faces[] = {"front", "back", "left", "right", "top", "bottom"};

typedef struct part_solution {
	int cube;
	int side;
	int height;
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

int opposite_side(int s) {
	return s % 2 == 0 ? s + 1 : s - 1;
}

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

void read_cube(int *buf) {
	int i;
	for (i = 0; i < 6; i++) {
		buf[i] = read_num(NULL);
	}
}

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

int cube[6];
int cubes[SIZE][6];
PartSolution *towers[SIZE][6];
int towers_c[COLORS][3];

void calc_tallest_tower(int N) {
	int c, ops, opc, i, s, mc;
	int max[3] = {0,0,0};
	PartSolution *ps;
	for (i = 0; i < COLORS; i++) {
		towers_c[i][0] = -1;
		towers_c[i][1] = -1;
		towers_c[i][2] = -1;
	}
	for (i = 0; i < N; i++) {
		read_cube(cube);
		for (s = 0; s < 6; s++) {
			c = cube[s]-1;
			ops = opposite_side(s);
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
		for (s = 0; s < 6; s++) {
			c = cube[s]-1;
			ops = opposite_side(s);
			opc = cube[ops]-1;
			if (towers_c[opc][2] < towers[i][s]->height) {
				towers_c[opc][0] = i;
				towers_c[opc][1] = s;
				towers_c[opc][2] = towers[i][s]->height;
			}
			if (towers[i][s]->height > max[2]) {
				max[0] = i;
				max[1] = s;
				max[2] = towers[i][s]->height;
			}
		}
	}


	int m, mi, ms, tmp_i;
	mi = max[0];
	ms = max[1];
	m = max[2];
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