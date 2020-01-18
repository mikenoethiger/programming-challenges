#include <iostream>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>

#define WINNER 1
#define ELIMINATED 2
#define ELIMINATED_PROCESSED 3


// ballots       := candidate indexes ordered by voting priority
// ballots[20]   := last vote of this ballot (i.e. a candidate)
// candidates    := information about each candidate
// candidates[0] := candidate's votes so far
// candidates[1] := status for a single candidate:
//                   0 means still in the game
//                   1 means winner
//                   2 means eliminated but ballots voting for him are not yet recounted
//                   3 means eliminated and ballots voting for him are already recounted
char candidate_names[20][81]; // 81 for the terminating null byte
int candidates[20][2];
int ballots[1000][21];

// ct := total candidates
// bt := total ballots
// l  := winner limit, i.e. number of votes to reach in order to win election
int ct;
int bt;
int l;
int w;

void vote(int b, int c) {
	candidates[c][0]++;
	ballots[b][20] = c;
	if (candidates[c][0] >= l) {
		w++;
		candidates[c][1] = WINNER;
	}
}

void print_votes() {
	int i;
	printf("candidate: ");
	for (i = 0; i < ct; i++) {
		printf("%2d ", i);
	}
	printf("\n");
	printf("status:    ");
	for (i = 0; i < ct; i++) {
		printf("%2d ", candidates[i][1]);
	}
	printf("\n");
	printf("votes:     ");
	for (i = 0; i < ct; i++) {
		printf("%2d ", candidates[i][0]);
	}
	printf("\n");
	sleep(1);
}

void check_tied() {
	int i;
	int tied_score = -1;
	bool tied = true;
	for (i = 0; i < ct; i++) {
		if (candidates[i][1] < ELIMINATED) {
			if (tied_score < 0) tied_score = candidates[i][0];
			else tied = tied_score == candidates[i][0];
			if (!tied) break;
		}
	}
	if (tied) {
		for (i = 0; i < ct; i++) {
			if (candidates[i][1] < ELIMINATED) {
				candidates[i][1] = WINNER;
				w++;
			}
		}
	}
}

/**
 * Finds the winning candidate(s).
 */
void solve() {
	int i, j;

	// clear candidates cache
	for (i = 0; i < ct; i++) {
		candidates[i][0] = 0;
		candidates[i][1] = 0;
	}

	// 1. voting round
	for (i = 0; i < bt; i++) {
		vote(i, ballots[i][0]);
	}
	check_tied();

	int min;
	int it = 2;
	while (w <= 0) {
		min = INT_MAX;
		// evaluate min votes
		for (i = 0; i < ct; i++) {
			if (candidates[i][1] < ELIMINATED && candidates[i][0] < min) min = candidates[i][0];
		}
//		printf("round=%d ct=%d bt=%d min=%d\n", it, ct, bt, min);
//		print_votes();
//		printf("\n");
		// eliminated min candidate(s)
		for (i = 0; i < ct; i++) {
			if (candidates[i][0] == min) candidates[i][1] = ELIMINATED;
		}
		// re-vote for eliminated candidates
		for (i = 0; i < ct; i++) {
			if (candidates[i][1] == ELIMINATED) {
				for (j = 0; j < bt; j++) {
					if (ballots[j][20] == i) {
						int k = 1;
						while (candidates[ballots[j][k]][1] >= ELIMINATED && k < ct) k++;
						if (k < ct) vote(j, ballots[j][k]);
					}
				}
				candidates[i][1] = ELIMINATED_PROCESSED;
			}
		}
		check_tied();
		it++;
	}

	for (i = 0; i < ct; i++) {
		if (candidates[i][1] == WINNER) {
			printf("%s\n", candidate_names[i]);
		}
	}

	// Idea (naive):
	// for (0..c-1):
	//   add votes of current round to candidates (while incrementing, always check
	//       whether a candidate is above l, if yes, add him to the winners array, increment w)
	//   for each round, keep a reference to the min amount of votes
	//
	//   determine losers and eliminate them
	//   if (w > 0) break (we have at least one winner)
	//
}

int scan_ballots(int n_candidates) {
	// i := running index
	// b := ballot index
	// n := line breaks counter
	int i, b, n;
	b = 0;
	n = 1;
	while (n == 1) {
		for (i = 0; i < n_candidates; i++) {
			scanf("%d", ballots[b] + i);
			scanf(" %n", &n); // count white spaces / line breaks
			if (n > 1) break;
			ballots[b][i]--;
		}
		b++;
	}
	return b;
}

int main() {
	int cases, i, j;
	scanf("%d\n", &cases);

	for (i = 0; i < cases; i++) {
		scanf("%d\n", &ct);
		for (j = 0; j < ct; j++) {
			scanf("%[a-zA-Z .]\n", candidate_names[j]);
		}
		bt = scan_ballots(ct);
//		l = bt % 2 == 0 ? bt / 2 : bt / 2 + 1;
		l = bt / 2 + 1;
		w = 0;
		solve();
		if (i < cases-1) printf("\n");
	}
	return 0;
}