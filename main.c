#include <stdio.h>

#include "connect4.h"

struct board {
	unsigned long long red;
	unsigned long long yellow;
} board;
unsigned long long allMoves[294];

void init_board() {
	board.red = 0;
	board.yellow = 0;
}

#define CHAR(N) (board.red & AT(N) ? 'R' : board.yellow & AT(N) ? 'Y' : '.')
void print_board() {
	for (int i=5; i>-1; i--) {
		printf("%c%c%c%c%c%c%c\n", CHAR(i), CHAR(i+6), CHAR(i+12), CHAR(i+18), CHAR(i+24), CHAR(i+30), CHAR(i+36));
	}
	printf("0123456\n\n");
}

#define PIECE_COUNT(pieces) (pieces ? __builtin_popcountll(pieces) : 0)
#define MOVE(col) (CAN_DROP(board, col) ? NEXT_DROP(board, col) : 0)
unsigned long long *gen_moves() {
	unsigned long long *base = &allMoves[7*(PIECE_COUNT(board.red) + PIECE_COUNT(board.yellow))];
	base[0] = MOVE(3);	//examine central moves first
	base[1] = MOVE(2);
	base[2] = MOVE(4);
	base[3] = MOVE(1);
	base[4] = MOVE(5);
	base[5] = MOVE(0);
	base[6] = MOVE(6);
	return base;
}

#define MAX(a,b) (a > b ? a : b)
#define MIN(a,b) (a < b ? a : b)

int yellow_search(int alpha, int beta);
int red_search(int alpha, int beta) {
	int best = NEG_INFINITY;
	unsigned long long *moves = gen_moves();
	for (int i=0; i<7; i++) { //check for winning moves
		if (moves[i] && IS_WIN(board.red | moves[i])) {
			return RED_SCORE(board.red | moves[i]);
		}
	}
	for (int i=0; i<7; i++) {
		if(moves[i]) {
			board.red ^= moves[i];
			best = MAX(best, yellow_search(alpha, beta));
			if (best >= beta) {	//alphabeta cutoff
				board.red ^= moves[i];
				break;
			}
			alpha = MAX(alpha, best);
			board.red ^= moves[i];
		}
	}
	return best;
}
int yellow_search(int alpha, int beta) {
	int best = POS_INFINITY;
	unsigned long long *moves = gen_moves();
	for (int i=0; i<7; i++) { //check for winning moves
		if (moves[i] && IS_WIN(board.yellow | moves[i])) {
			return YELLOW_SCORE(board.yellow | moves[i]);
		}
	}
	if (PIECE_COUNT(board.yellow)==20)	//if we don't have a winning move and are placing our 21st piece, it's a full board draw
		return 0;
	for (int i=0; i<7; i++) {
		if(moves[i]) {
			board.yellow ^= moves[i];
			best = MIN(best, red_search(alpha, beta));
			if (best <= alpha) {
				board.yellow ^= moves[i];
				break;
			}
			beta = MIN(beta, best);
			board.yellow ^= moves[i];
		}
	}
	return best;
}

#ifndef TESTING
int main() {
	init_board();
	board.red    = 17571840;
	board.yellow = 68719480897;
	print_board();
	return 0;
}
#endif
