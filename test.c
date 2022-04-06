#include <assert.h>
#include <time.h>

#include "main.c"

void test_basics() {
	init_board();
	//setup a board with 41/42 placed
	board.red = 2772257490029;	board.yellow = 1591429282706;
	assert(EMPTY(board) == AT(35));
	assert(YELLOW_TURN(board));
	assert(!IS_WIN(board.red));
	assert(!IS_WIN(board.yellow));
	assert(yellow_search(NEG_INFINITY, POS_INFINITY) == 0);
	board.red ^= AT(34);	//undo a red move
	assert(RED_TURN(board));
	assert(red_search(NEG_INFINITY, POS_INFINITY) == 0);
}

void test_ai(char *filename, int positions) {
	FILE *test_file = fopen(filename, "r");
	int expected, actual;
	for (int i=0; i<positions; i++) {
		fscanf(test_file, "%llu %llu %d", &board.red, &board.yellow, &expected);
		actual = RED_TURN(board) ? red_search(NEG_INFINITY, POS_INFINITY) : yellow_search(NEG_INFINITY, POS_INFINITY);
		if (expected != actual)
			printf("Error: Expected %d, got %d on board (%llu, %llu)\n", expected, actual, board.red, board.yellow);
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
