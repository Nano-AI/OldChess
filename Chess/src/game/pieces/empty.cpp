#include "empty.h"

Empty::Empty(int x, int y, int side) : Piece(x, y, side) {
	this->g_is_king = false;
	this->g_is_white = false;
	this->g_first_move = false;
	this->g_piece = EMPTY;
}

std::vector<Vector2> Empty::GetValidMoves() {
	std::vector<Vector2> moves;
	return moves;
}

Piece* Empty::Clone() {
	return new Empty(0, 0, 0);
}
