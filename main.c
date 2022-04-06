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

unsigned long long node_hits = 0;
unsigned long long cache_hits = 0;

#define MAX(a,b) (a > b ? a : b)
#define MIN(a,b) (a < b ? a : b)

#define CACHE_SIZE 123458ULL
#define HASH(red, yellow) ((7*red + 13*yellow)%CACHE_SIZE)
#define SCORE_OFFSET 50
#define EXACT 1
#define LOWERBOUND 2
#define UPPERBOUND 3
int check_cache(unsigned long long *cache, int *alpha, int *beta) {
	unsigned long long hash = HASH(board.red, board.yellow);
	if ((cache[2*hash] & ALL_SQUARES) == board.red && (cache[2*hash + 1] & ALL_SQUARES) == board.yellow) {
		cache_hits++;
		int score = NEG_INFINITY + (int)((cache[2*hash] & (63ULL<<SCORE_OFFSET))>>SCORE_OFFSET);
		int type  = (int)(cache[2*hash + 1] >> SCORE_OFFSET);
		if (type == LOWERBOUND)
			*alpha = MAX(*alpha, score);
		else if (type == UPPERBOUND)
			*beta = MIN(*beta, score);
		else if (type == EXACT)
			*alpha = *beta;
		return score;
	}
	return NEG_INFINITY;
}
void put_cache(unsigned long long *cache, int val, int type) {
	unsigned long long hash = HASH(board.red, board.yellow);
	cache[2*hash] = board.red | (((unsigned long long)(val - NEG_INFINITY)) << SCORE_OFFSET);
	cache[2*hash + 1] = board.yellow | (((unsigned long long)type) << SCORE_OFFSET);
}

int negamax(int isRed, int alpha, int beta, unsigned long long *cache) {
	node_hits++;
	//check for instant wins/draws:
	unsigned long long *moves = gen_moves();
	unsigned long long *curBoard = (isRed ? &board.red : &board.yellow);
	for (int i=0; i<7; i++) {
		if (moves[i] && IS_WIN(*curBoard | moves[i])) {
			return SCORE(*curBoard | moves[i]);
		}
	}
	if (!isRed && PIECE_COUNT(*curBoard)==20)	//yellow placing its 21st piece without a winning move means the board is full, and the game is a draw
		return 0;
	//If we got here, we need to search, so check the cache:
	int inputAlpha = alpha;
	int cacheVal = check_cache(cache, &alpha, &beta);
	if (cacheVal > NEG_INFINITY && alpha >= beta)
		return cacheVal;
	//If we got here, the cache didn't have the exact value, so perform a search:
	int best = NEG_INFINITY;
	for (int i=0; i<7; i++) {
		if (moves[i]) {
			*curBoard ^= moves[i];
			best = MAX(best, -negamax(!isRed, -beta, -alpha, cache));
			*curBoard ^= moves[i];
			alpha = MAX(alpha, best);
			if (alpha >= beta)	//alpha-beta cutoff
				break;
		}
	}
	if (best <= inputAlpha)
		put_cache(cache, best, UPPERBOUND);
	else if (best >= beta)
		//an alpha-beta cuttoff occured (or would have occured if there was another move to examine)
		//so we know we can get at least this score
		put_cache(cache, best, LOWERBOUND);
	else	//nothing special happened and we explored the whole gametree below us
		put_cache(cache, best, EXACT);
	return best;
}

#ifndef TESTING
int main() {
	init_board();
	board.red    = 23478420589 | AT(38);
	board.yellow = 217039747986 | AT(35) | AT(39);
	print_board();
	unsigned long long cache[2*CACHE_SIZE];
	printf("%d\n", IS_WIN(board.red | AT(40)));
	printf("%d\n", IS_WIN(1397867955309));
	//printf("%d\n", negamax(RED_TURN(board), NEG_INFINITY, POS_INFINITY, cache));
	return 0;
}
#endif
