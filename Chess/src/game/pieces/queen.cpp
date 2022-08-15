#include "queen.h"

Queen::Queen(int x, int y, int side) : Piece(x, y, side) {
	this->g_is_king = false;
	this->g_is_white = side == WHITE;
	this->g_first_move = true;
	this->g_piece = QUEEN | side;
}

std::vector<Vector2> Queen::GetValidMoves(std::vector<std::vector<Piece*>> board) {
	std::vector<Vector2> moves;
	return moves;
}

Piece* Queen::Clone() {
	return new Queen(X(), Y(), this->g_side);
}
