#include "rook.h"

Rook::Rook(int x, int y, int side) : Piece(x, y, side) {
	this->g_is_king = false;
	this->g_is_white = side == WHITE;
	this->g_first_move = true;
	this->g_piece = ROOK | side;
}

std::vector<Vector2> Rook::GetValidMoves() {
	std::vector<Vector2> moves;
	return moves;
}

Piece* Rook::Clone() {
	return new Rook(X(), Y(), this->g_side);
}
