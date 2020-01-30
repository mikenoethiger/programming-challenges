#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <strings.h>
#include <stdbool.h>
#include <time.h>

#define WINNER 1
#define ELIMINATED 2
#define ELIMINATED_PROCESSED 3

/* I tried to analyze the execution time of this algorithm.
 * Experiments showed, that for an input of 800 votings:
 * - Time to read input: 0.089923s
 * - Time to solve and print solution: 0.006333s
 * Which means reading the input makes 90% of the time consumption.
 * Unfortunately I could not find a more efficient way to read the input.
 */

/* DATA STRUCTURE */

// ballots       := ballot containing candidate indexes
// ballots[20]   := cache last used candidate for each ballet (i.e. a candidate index)
//                  PROBABLY OBSOLETE WITH candidates_b?
// candidates    := information about each candidate
// candidates[0] := number of votes so far
// candidates[1] := candidate status:
//                   0 means still in the game
//                   1 means winner
//                   2 means eliminated but ballots voting for him are not yet recounted
//                   3 means eliminated and ballots voting for him are already recounted
// candidates_b       := reverse cache; store's indexes of ballots who voted for a candidate
// candidates_b[1000] := next free index
char candidate_names[20][81]; // 81 for the terminating null byte
int candidates[20][2];
int candidates_b[20][1001];
int ballots[1000][20];

// cn := total candidates
// bn := total ballots
// l  := winner limit, i.e. number of votes to reach in order to win election
// wi := number of winners
int cn, bn, l, wi;

clock_t start, end;
clock_t start1, end1;
double time_read_names = 0;
double time_read_numbers = 0;
double time_algo = 0;

/* UTILITY */

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

/* ALGORITHM */

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

/**
 * Finds the winning candidate(s).
 */
void solve() {
	start = clock();
	int i, j;

	// clear candidates and ballots_c cache
	memset(candidates, 0, sizeof(int) * 20 * 2);
	memset(candidates_b, 0, sizeof(int) * 20 * 1001);

	// 1. voting round
	for (i = 0; i < bn; i++) {
		vote(i, ballots[i][0]);
	}
	check_tied();

	int min;
	while (wi <= 0) {
		min = INT_MAX;
		// evaluate min votes
		for (i = 0; i < cn; i++) {
			if (candidates[i][1] < ELIMINATED && candidates[i][0] < min) min = candidates[i][0];
		}
		// eliminated min candidate(s)
		for (i = 0; i < cn; i++) {
			if (candidates[i][0] == min) candidates[i][1] = ELIMINATED;
		}
		// re-vote for eliminated candidates
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
			printf("%s", candidate_names[i]);
		}
	}
	end = clock();
	time_algo += end-start;
}

int scan_ballots() {
	start = clock();
	// i  := running index
	// bi := ballot index
	// n  := line breaks counter
	int i, bi, n;
	bi = 0;
	n = 1;
	while (n == 1) {
		for (i = 0; i < cn; i++) {
			if (scanf("%d", ballots[bi] + i) == EOF) {
				bi--; n = 2;
				break;
			}
			// count white spaces / line breaks
			scanf(" %n", &n);
			if (n > 1) break;
			ballots[bi][i]--;
		}
		bi++;
	}
	end = clock();
	time_read_numbers += end-start;
	return bi;
}

int main() {
	int cases, i, j, ni;
	scanf("%d\n", &cases);
	for (i = 0; i < cases; i++) {
		start = clock();
		scanf("%d\n", &cn);
		for (j = 0; j < cn; j++) {
			fgets(candidate_names[j], 81, stdin);
		}
		end = clock();
		time_read_names += end-start;
		bn = scan_ballots();
		l = bn / 2 + 1;
		wi = 0;
		solve();
		if (i < cases-1) printf("\n");
	}
	printf("time_read_names=%f\n", (double) time_read_names / CLOCKS_PER_SEC);
	printf("time_read_numbers=%f\n", (double) time_read_numbers / CLOCKS_PER_SEC);
	printf("time_algo=%f\n", (double) time_algo / CLOCKS_PER_SEC);
	return 0;
}