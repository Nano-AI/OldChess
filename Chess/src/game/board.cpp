#include "board.h"

Board::Board() {
	// Create a vector with 8 rows and 8 columns, with the default value BLANK.
	this->game_board = std::vector<std::vector<int>>(
		8,
		std::vector<int>(8, BLANK)
	);
	int backrank[8] = {WROOK, WKNIGHT, WBISHOP, WQUEEN, WKING, WBISHOP, WKNIGHT, WROOK};
	for (int i = 0; i < 8; i++) {
		game_board[7][i] = backrank[i];
		game_board[6][i] = WPAWN;
		game_board[0][i] = backrank[i] | 0x1000;
		game_board[1][i] = BPAWN;
	}
}

void Board::PrintBoard() {
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			printf("|0x%0.4x|\t", game_board[x][y]);
		}
		printf("\n");
	}
}

std::vector<std::vector<int>> Board::GetBoard() {
	return this->game_board;
}

