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
	return NULL;
}

Board::Board(int side) {
	// Create a vector with 8 rows and 8 columns, with the default value BLANK.
	this->g_game_board = std::vector<std::vector<Piece*>>(
		8,
		std::vector<Piece*>(8)
	);
	this->g_board_colors = std::vector<std::vector<int>>(
		8,
		std::vector<int>(8)
	);
	int backrank[8] = {ROOK, BISHOP, KNIGHT, QUEEN, KING, KNIGHT, BISHOP, ROOK};

	int playing = side;
	int other = (playing == WHITE) ? BLACK : WHITE;

	for (int i = 0; i < 8; i++) {
		this->g_game_board[1][i] = new Pawn(1, i, playing);
		this->g_game_board[6][i] = new Pawn(6, i, other);
		this->g_game_board[7][i] = GetPiece(backrank[i], 7, i, other);
		this->g_game_board[0][i] = GetPiece(backrank[i], 0, i, playing);
	}
	for (int x = 2; x < 6; x++) {
		for (int y = 0; y < 8; y++) {
			this->g_game_board[x][y] = new Empty(x, y, EMPTY);
		}
	}
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			this->g_game_board[x][y]->g_coord = { x, y };
		}
	}

	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			this->g_board_colors[x][y] = ((x + y) % 2 == 0) ? other : playing;
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

	if (start == NULL || end == NULL) {
		LOG_F(ERROR, "You tried to place a piece outside of the board. How is this possible?");
		return OUT_OF_BOUNDS;
	}
	if (start->g_side == end->g_side && end->g_piece != EMPTY) {
		LOG_F(ERROR, "You can't capture your own piece you idiot.");
		return SAME_SIDE_CAPTURE;
	}
	if (!start->Move(end->X(), end->Y(), this->GetBoard())) {
		LOG_F(ERROR, "Do you know how to play chess?");
		return ILLEGAL_PIECE_MOVE;
	}

	// Clear out memory
	delete(this->g_game_board[toX][toY]);
	this->g_game_board[toX][toY] = start->Clone();
	this->g_game_board[toX][toY]->g_coord = { toX, toY };
	delete(this->g_game_board[startX][startY]);
	this->g_game_board[startX][startY] = new Empty(startX, startY, EMPTY);

	return SUCCESS;
}
