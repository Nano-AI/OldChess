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
	this->g_game_board = std::vector<std::vector<Piece*>>(
		8,
		std::vector<Piece*>(8)
	);
	int backrank[8] = {ROOK, BISHOP, KNIGHT, QUEEN, KING, KNIGHT, BISHOP, ROOK};
	for (int i = 0; i < 8; i++) {
		this->g_game_board[1][i] = new Pawn(1, i, WHITE);
		this->g_game_board[6][i] = new Pawn(6, i, BLACK);
		this->g_game_board[7][i] = GetPiece(backrank[i], 7, i, BLACK);
		this->g_game_board[0][i] = GetPiece(backrank[i], 0, i, WHITE);
	}
	for (int x = 2; x < 6; x++) {
		for (int y = 0; y < 8; y++) {
			this->g_game_board[x][y] = new Empty(x, y, EMPTY);
		}
	}
}

void Board::PrintBoard() {
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			printf("|0x%0.4x|\t", this->g_game_board[x][y]->g_piece);
		}
		printf("\n");
	}
}

std::vector<std::vector<Piece*>> Board::GetBoard() {
	return this->g_game_board;
}

Piece* Board::At(int x, int y) {
	if (x > 8 || y > 8 || x < 0 || y < 0) {
		LOG_F(ERROR, "Trying to access (%i, %i) which is out of bounds.", x, y);
		return NULL;
	}
	return this->g_game_board[x][y];
}

int Board::Move(int startX, int startY, int toX, int toY) {
	Piece* start = At(startX, startY);
	Piece* end = At(toX, toY);
	LOG_F(INFO, "Moving from (%i, %i): (0x%0.4x) to (%i, %i): (0x%0.4x).", startX, startY, start->g_piece, toX, toY, end->g_piece);
	if (start == NULL || end == NULL) {
		LOG_F(ERROR, "Trying to access out of board boundary!");
		return OUT_OF_BOUNDS;
	}
	if (start->g_side == end->g_side && end->g_piece != EMPTY) {
		LOG_F(WARNING, "Trying to capture same sided piece from (%i, %i) to (%i, %i)!", startX, startY, toX, toY);
		return SAME_SIDE_CAPTURE;
	}
	if (!start->Move(end->X(), end->Y())) {
		LOG_F(WARNING, "Moving piece illegally from (%i, %i) to (%i, %i)!", startX, startY, toX, toY);
		return ILLEGAL_PIECE_MOVE;
	}
	delete(this->g_game_board[toX][toY]);
	this->g_game_board[toX][toY] = start->Clone();
	this->g_game_board[toX][toX]->g_coord = { toX, toY };
	delete(this->g_game_board[startX][startY]);
	this->g_game_board[startX][startY] = new Empty(startX, startY, EMPTY);
	return SUCCESS;
}

// Just made these incase I wanted to make it easier on myself
// I could make this more efficient but who cares
//int Board::Move(Vector2 start, Vector2 end) {
//	return Move(start.x, start.y, end.x, end.y);
//}
//
//int Board::Move(Piece* start, Piece* end) {
//	return Move(start->g_coord, end->g_coord);
//}
