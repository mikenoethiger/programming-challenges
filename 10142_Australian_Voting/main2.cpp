#include <iostream>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <cstring>

#define WINNER 1
#define ELIMINATED 2
#define ELIMINATED_PROCESSED 3


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
	ballots[b][20] = c; // propably obsolete
	candidates_b[c][candidates_b[c][1000]] = b;
	candidates_b[c][1000]++;
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


//	for (i = 0; i < ct; i++) {
//		candidates[i][0] = 0;
//		candidates[i][1] = 0;
//	}
	// clear candidates and ballots_c cache
	memset(candidates, 0, sizeof(int) * 20 * 2);
	memset(candidates_b, 0, sizeof(int) * 20 * 1001);

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
		// eliminated min candidate(s)
		for (i = 0; i < ct; i++) {
			if (candidates[i][0] == min) candidates[i][1] = ELIMINATED;
		}
		// re-vote for eliminated candidates
		for (i = 0; i < ct; i++) {
			if (candidates[i][1] == ELIMINATED) {
				for (j = 0; j < candidates_b[i][1000]; j++) {
					int k = 1;
					int b = candidates_b[i][j];
					while (candidates[ballots[b][k]][1] >= ELIMINATED && k < ct) k++;
					if (k < ct) vote(b, ballots[b][k]);
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
			if (scanf("%d", ballots[b] + i) == EOF) {
				b--; n = 2;
				break;
			}
			// count white spaces / line breaks
			scanf(" %n", &n);
			if (n > 1) break;
			ballots[b][i]--;
		}
		b++;
	}
	return b;
}

void print_ballots() {
	int i, j;
	for (i = 0; i < bt; i++) {
		for (j = 0; j < ct; j++) {
			printf("%d ", ballots[i][j]);
		}
		printf("\n");
	}
}

int main() {
	int cases, i, j, ni;
	scanf("%d\n", &cases);

	for (i = 0; i < cases; i++) {
		scanf("%d\n", &ct);
		for (j = 0; j < ct; j++) {
			ni = 0;
			scanf("%c", candidate_names[j]);
			while (candidate_names[j][ni] != '\n') {
				ni++;
				scanf("%c", candidate_names[j] + ni);
			}
			candidate_names[j][ni] = '\0';
		}
		bt = scan_ballots(ct);
		l = bt / 2 + 1;
		w = 0;
		solve();
		if (i < cases-1) printf("\n");
	}
	return 0;
}