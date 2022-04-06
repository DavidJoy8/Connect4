#include <assert.h>
#include <time.h>

#include "main.c"

void test_basics() {
	init_board();
	//setup a board with 41/42 placed
	board.red = 2359940629613;	board.yellow = 2003746143122;
	assert(EMPTY(board) == AT(35));
	assert(YELLOW_TURN(board));
	assert(!IS_WIN(board.red));
	assert(!IS_WIN(board.yellow));
	board.red ^= AT(34);	//undo a red move
	assert(RED_TURN(board));

	assert(IS_WIN(1397867955309));
}

void test_ai(char *filename, int positions) {
	FILE *test_file = fopen(filename, "r");
	int expected, actual;
	for (int i=0; i<positions; i++) {
		fscanf(test_file, "%llu %llu %d", &board.red, &board.yellow, &expected);
		unsigned long long cache[2*CACHE_SIZE];
		actual = negamax(RED_TURN(board), NEG_INFINITY, POS_INFINITY, cache);
		if (expected != actual) {
			printf("Expected %d, got %d on board (%llu, %llu)\n", expected, actual, board.red, board.yellow);
			assert(expected == actual);
		}
	}
	fclose(test_file);
}

#ifdef TESTING
int main() {
	test_basics();
	test_ai("tests/endgame_easy.txt", 1000);
	printf("All tests passed\n");
	return 0;
}
#endif
