#include "knight.h"

Knight::Knight(int x, int y, int side) : Piece(x, y, side) {
	this->g_is_king = false;
	this->g_is_white = side == WHITE;
	this->g_first_move = true;
	this->g_piece = KNIGHT | side;
}

std::vector<Vector2> Knight::GetValidMoves() {
	std::vector<Vector2> moves;
	return moves;
}

Piece* Knight::Clone() {
	return new Knight(X(), Y(), this->g_side);
}
