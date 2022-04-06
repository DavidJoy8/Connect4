#define TOP_ROW 2233928386592ULL
#define BOTTOM_ROW 69810262081ULL
#define ALL_SQUARES 4398046511103ULL

#define AT(n) ((unsigned long long int)1<<(n))
#define COL(n) (63ULL << (6*n))
//bitmap of the filled squares
#define FILLED(board) (board.red | board.yellow)
//bitmap of the non-filled squares
#define EMPTY(board) (ALL_SQUARES ^ FILLED(board))
#define CAN_DROP(board, col) (EMPTY(board) & TOP_ROW & COL(col))
//red goes first
#define RED_TURN(board) (board.red==0 || __builtin_popcountll(board.red)==__builtin_popcountll(board.yellow))
#define YELLOW_TURN(board) (board.red!=0 && __builtin_popcountll(board.red)>__builtin_popcountll(board.yellow))
//the bit to set for the next piece dropped in column n
#define NEXT_DROP(board, n) ((BOTTOM_ROW & COL(n)) + (FILLED(board) & COL(n)))

//win checking:
#define TOP3 3909374676536ULL
#define TOP4 4188615724860ULL
#define TOP5 4328236249022ULL
#define BOT3 488671834567ULL
#define BOT4 1047153931215ULL
#define BOT5 2164118124511ULL

#define VERT_WIN(pieces) ((TOP3 & (pieces))>>3 & (TOP4 & (pieces))>>2 & (TOP5 & (pieces))>>1 & (pieces) != 0)
#define HORI_WIN(pieces) ((pieces)>>18 & (pieces)>>12 & (pieces) >> 6 & (pieces) != 0)
#define DIAG_A_WIN(pieces) ((TOP3 & (pieces))>>21 & (TOP4 & (pieces))>>14 & (TOP5 & (pieces))>>7 & (pieces) != 0)
#define DIAG_B_WIN(pieces) ((BOT3 & (pieces))>>15 & (BOT4 & (pieces))>>10 & (BOT5 & (pieces))>>5 & (pieces) != 0)
#define IS_WIN(pieces) (VERT_WIN(pieces) || HORI_WIN(pieces) || DIAG_A_WIN(pieces) || DIAG_B_WIN(pieces))

//scoring:
//If red wins, their score is (22 - number of red pieces).  This encourages the CPU to win quickly and lose slowly.
//If yellow wins, their score is (number of yellow pieces - 22).  The idea that negative scores represent the second player winning is borrowed from chess
//These values are used to start the search.  They are outside the range of obtainable scores, which is [-18, 18] (since a win takes a minimum of 4 pieces)
#define NEG_INFINITY -25
#define POS_INFINITY 25
#define RED_SCORE(pieces) 22 - __builtin_popcountll(pieces)
#define YELLOW_SCORE(pieces) __builtin_popcountll(pieces) - 22
