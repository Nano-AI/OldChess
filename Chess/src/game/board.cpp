#include "board.h"

Piece* GetPiece(int value, int x, int y, int side) {
	switch (value) {
	case PAWN:
		return new Pawn(x, y, side);
	case BISHOP:
		return new Bishop(x, y, side);
	case KNIGHT:
		return new Knight(x, y, side);
	case ROOK:
		return new Rook(x, y, side);
	case QUEEN:
		return new Queen(x, y, side);
	case KING:
		return new King(x, y, side);
	default:
		LOG_F(ERROR, "Unkown piece |0x%0.4x|", value);
	}
}

Board::Board() {
	// Create a vector with 8 rows and 8 columns, with the default value BLANK.
	this->game_board = std::vector<std::vector<Piece*>>(
		8,
		std::vector<Piece*>(8)
	);
	int backrank[8] = {ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK};
	for (int i = 0; i < 8; i++) {
		this->game_board[7][i] = GetPiece(backrank[i], i, 7, BLACK);
		this->game_board[0][i] = GetPiece(backrank[i], i, 0, WHITE);
		this->game_board[1][i] = new Pawn(i, 1, WHITE);
		this->game_board[6][i] = new Pawn(i, 6, BLACK);
	}
	for (int x = 2; x < 7; x++) {
		for (int y = 0; y < 8; y++) {
			this->game_board[x][y] = new Empty(y, x, EMPTY);
		}
	}
}

void Board::PrintBoard() {
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			printf("|0x%0.4x|\t", this->game_board[x][y]->g_piece);
		}
		printf("\n");
	}
}

std::vector<std::vector<Piece*>> Board::GetBoard() {
	return this->game_board;
}

