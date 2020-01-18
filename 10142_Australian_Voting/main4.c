#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <cstring>
#include <stdbool.h>

#define WINNER 1
#define ELIMINATED 2
#define ELIMINATED_PROCESSED 3

char candidate_names[20][81];
int candidates[20][2];
int candidates_b[20][1001];
int ballots[1000][20];

int cn, bn, l, wi;

void print_votes() {
	int i;
	printf("candidate: ");
	for (i = 0; i < cn; i++) {
		printf("%2d ", i);
	}
	printf("\n");
	printf("status:    ");
	for (i = 0; i < cn; i++) {
		printf("%2d ", candidates[i][1]);
	}
	printf("\n");
	printf("votes:     ");
	for (i = 0; i < cn; i++) {
		printf("%2d ", candidates[i][0]);
	}
	printf("\n");
	sleep(1);
}

void vote(int b, int c) {
	candidates[c][0]++;
	candidates_b[c][candidates_b[c][1000]++] = b;
	if (candidates[c][0] >= l) {
		wi++;
		candidates[c][1] = WINNER;
	}
}

void check_tied() {
	int i;
	int tied_score = -1;
	bool tied = true;
	for (i = 0; i < cn; i++) {
		if (candidates[i][1] < ELIMINATED) {
			if (tied_score < 0) tied_score = candidates[i][0];
			else tied = tied_score == candidates[i][0];
			if (!tied) break;
		}
	}
	if (tied) {
		for (i = 0; i < cn; i++) {
			if (candidates[i][1] < ELIMINATED) {
				candidates[i][1] = WINNER;
				wi++;
			}
		}
	}
}

void solve() {
	int i, j;
	memset(candidates, 0, sizeof(int) * 20 * 2);
	memset(candidates_b, 0, sizeof(int) * 20 * 1001);

	for (i = 0; i < bn; i++) {
		vote(i, ballots[i][0]);
	}
	check_tied();

	int min;
	while (wi <= 0) {
		min = INT_MAX;
		for (i = 0; i < cn; i++) {
			if (candidates[i][1] < ELIMINATED && candidates[i][0] < min) min = candidates[i][0];
		}
		for (i = 0; i < cn; i++) {
			if (candidates[i][0] == min) candidates[i][1] = ELIMINATED;
		}
		for (i = 0; i < cn; i++) {
			if (candidates[i][1] == ELIMINATED) {
				for (j = 0; j < candidates_b[i][1000]; j++) {
					int k = 1;
					int b = candidates_b[i][j];
					while (candidates[ballots[b][k]][1] >= ELIMINATED && k < cn) k++;
					if (k < cn) vote(b, ballots[b][k]);
				}
				candidates[i][1] = ELIMINATED_PROCESSED;
			}
		}
		check_tied();
	}

	for (i = 0; i < cn; i++) {
		if (candidates[i][1] == WINNER) {
			printf("%s\n", candidate_names[i]);
		}
	}
}

int scan_ballots() {
	int i, bi, n;
	bi = 0;
	n = 1;
	while (n == 1) {
		for (i = 0; i < cn; i++) {
			if (scanf("%d", ballots[bi] + i) == EOF) {
				bi--; n = 2;
				break;
			}
			scanf(" %n", &n);
			if (n > 1) break;
			ballots[bi][i]--;
		}
		bi++;
	}
	return bi;
}

int main() {
	int cases, i, j, ni;
	scanf("%d\n", &cases);

	for (i = 0; i < cases; i++) {
		scanf("%d\n", &cn);
		for (j = 0; j < cn; j++) {
			ni = 0;
			scanf("%c", candidate_names[j]);
			while (candidate_names[j][ni] != '\n') {
				ni++;
				scanf("%c", candidate_names[j] + ni);
			}
			candidate_names[j][ni] = '\0';
		}
		bn = scan_ballots();
		l = bn / 2 + 1;
		wi = 0;
		solve();
		if (i < cases-1) printf("\n");
	}
	return 0;
}