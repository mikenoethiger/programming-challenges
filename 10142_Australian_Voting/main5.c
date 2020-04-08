#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WINNER 1
#define ELIMINATED 2
#define ELIMINATED_PROCESSED 3

char candidate_names[21][81];
char ballots[1001][42];
int candidates[21][2];
int votes_consumed = 0;
int total_ballots;
int total_candidates;
int winners;

// do not append line delimiter (but still count it in return value)
int getline2(char **lineptr, size_t *n) {
	int res = getline(lineptr, n, stdin);
	if ((*lineptr)[res-1] == '\n') {
		(*lineptr)[res-1] = '\0';
	}
	return res;
}

void print_winners() {
	for (i = 1; i < 21; i++) {
		if (candidates[i][1] == WINNER) {
			printf("%s\n", candidate_names[i]);
		}
	}
}

void winner() {
	// 1. check for winner (i.e. more than 50% votes)
	// 2. eliminate losers
	// 3. recount votes of losers
	// repeat

	int i;

	for (i = 1; i < 21; i++) {
		if (candidates[i][0]*2 > total_candidates) {
			winners++;
			candidates[i][1] = WINNER;
		}
	}

	if (w > 0) {
		print_winners();
		return;
	}

	int min;

	while (w < 1) {
		// eliminate
		// 1. find min
		// 2. eliminate all with min
		for (i = 0; i < total_ballots; i++) {

		}

		// recount votes

		// check for winners
	}
}

int main() {
	// vorgehen:
	// 1. Candidates lesen
	// 2. Ballots als string lesen, sowie die erste stimmabgabe von jedem ballot parsen
	// 3. Nun schrittweise die weiteren stimmen parsen und evaluieren

	// allgemein:
	// - Sobald einer mehr als 50% dere stimmen hat gewinnt er
	// -

	int cases;
	int n_candidates;
	int i, c, b;
	char *line;
	int vote;
	size_t size;

	for (i = 0; i < 21; i++) {
		candidates[i][0] = 0;
		candidates[i][1] = 0;
	}

	getline2(&line, &size);
	sscanf(line, "%d\n", &cases);
	printf("cases: %d\n", cases);

	// skip empty line
	getline2(&line, &size);

	for (i = 0; i < cases; i++) {
		getline2(&line, &size);
		sscanf(line, "%d\n", &n_candidates);
		total_candidates = n_candidates;

		for (c = 1; c <= n_candidates; c++) {
			getline2(&line, &size);
			strcpy(candidate_names[c], line);
			printf("%s\n", line);
		}

		b = 0;
		while (getline2(&line, &size) > 1) {
			strcpy(ballots[b], line);
			printf("%s\n", ballots[b]);
			b++;
			sscanf(ballots[b], "%d", &vote);
			candidates[vote][0]++;
		}
		total_ballots = b;
		votes_consumed++;
		winners = 0;
		winner();
	}

	return 0;
}