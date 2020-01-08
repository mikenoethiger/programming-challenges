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
	scanf("%d%d%d%d%d%d", buf, buf+1, buf+2, buf+3, buf+4, buf+5);
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
PartSolution *towers[SIZE][6];
int towers_c[COLORS][3];

void print_reverse(int i, int s, PartSolution *towers[SIZE][6]) {
	if (towers[i][s]->cube != i) {
		print_reverse(towers[i][s]->cube, towers[i][s]->side, towers);
		printf("%d %s\n", towers[i][s]->cube + 1, faces[towers[i][s]->side]);
	}
}

void calc_tallest_tower(int N) {
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