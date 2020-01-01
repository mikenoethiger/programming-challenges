/*
 * Algorithms to manipulate a 2-dimensional board,
 * stored in a 1-dimensional array.
 */

/* ====================
 * GENERAL TERMS
 * ====================
 *
 * n  := board height
 * m  := board width
 * ai := adjacency index (0..7)
 *
 * Adjacency Indexes:
 * 0 := up
 * 1 := right
 * 2 := down
 * 3 := left
 * 4 := up-left
 * 5 := up-right
 * 6 := down-right
 * 7 := down-left
 * 405
 * 3x1
 * 726
*/

/**
 * Check a given board index for it's edge adjacency.
 * More precisely, this function checks whether an index
 * i is on any of the 4 edges top, right, bottom, left.
 *
 * Examples:
 * ...i
 * ....
 * ....
 * edges = {1, 1, 0, 0}
 *
 * ....
 * i...
 * ....
 * edges = {0, 0, 0, 1}
 *
 * ....
 * ..i.
 * ....
 * edges = {0, 0, 0, 0}
 *
 * @param i board index.
 * @param n board height.
 * @param m board width.
 * @param edges out parameter containing edge adjacency.
 *              must be of size 4*int.
 */
void b_check_edges(size_t i, size_t n, size_t m, size_t *edges) {
	edges[0] = 0;
	edges[1] = 0;
	edges[2] = 0;
	edges[3] = 0;
	// left edge
	if (i % m == 0) edges[3] = 1;
	// right edge
	if ((i+1) % m == 0) edges[1] = 1;
	// top edge
	if (i/m == 0) edges[0] = 1;
	// bottom edge
	if (i/m == (n-1)) edges[2] = 1;
}

/**
 * Move from position i to an adjacent place a
 * if and only if this move stays inside the board.
 *
 * Example 1:
 * move(2, 1, 2, 4, &j)
 * before  after
 * ..i.    ...j
 * ....    ....
 * returns 1
 *
 * Example 2:
 * move(3, 1, 2, 4, &j)
 * before  after
 * ...i    ...j
 * ....    ....
 * returns 0, j=3
 *
 * @param i the starting position
 * @param a the adjacent index to which you want to move (relative to i)
 * @param n the board height
 * @param m the board width
 * @param j (out parameter) the ending position after the move was performed.
 *          j will be equal to i if no move happened.
 * @return 1 if a move happened, 0 otherwise.
 */
int b_move(size_t i, int a, size_t n, size_t m, size_t *j) {
	// edges  := edge adjacency of i
	// moves  := amounts to add to index i in
	//           order to move to an adjacent
	//           position (0..7)
	// ia     := invalid adjacency indexes when i
	//           is adjacent to an edge (0..3)
	// va     := valid adjacency indexes for i
	int edges[4];
	int m_up = -m; int m_down = m;
	int m_left = -1; int m_right = 1;
	int moves[8] = {m_up, m_right, m_down, m_left,
					m_up + m_left, m_up + m_right,
					m_down + m_right, m_down + m_left};
	int ia[4][3] = {{4,0,5},  // taboo moves if top edge
				    {5,1,6},  // taboo moves if right edge
					{7,2,6},  // taboo moves if bottom edge
					{4,3,7}}; // taboo moves if left edge
	int va[8] = {1,1,1,1,1,1,1,1};
	int k;

	b_check_edges(i, n, m, edges);
	for (k = 0; k < 4; k++) {
		if (edges[k]) {
			va[ia[k][0]] = 0;
			va[ia[k][1]] = 0;
			va[ia[k][2]] = 0;
		}
	}
	*j = va[a] ? i + moves[a] : i;
	return va[a];
}

/**
 * Execute multiple adjacent moves.
 * Move from position i to an adjacent place a_arr[0],
 * then from that place to a_arr[1] up to a_arr[a_n-1].
 *
 * The sequence of moves will be intercepted by the first
 * move that leaves the board.
 *
 * @param i the starting position
 * @param a_arr an array of adjacent indexes that represent the single moves
 * @param a_n size of a_arr
 * @param n board height
 * @param m board width
 * @param j (out parameter) the ending position after the moves were performed.
*          j will be equal to i if no move happened.
 * @return 1 if the move sequence could be accomplished, 0 otherwise.
 */
int b_move_n(size_t i, int *a_arr, size_t a_n, size_t n, size_t m, size_t *j) {
	int k;
	*j = i;
	for (k = 0; k < m_n; k++) {
		if (!b_move(*j, m_arr[k], j)) return 0;
	}
	return 1;
}