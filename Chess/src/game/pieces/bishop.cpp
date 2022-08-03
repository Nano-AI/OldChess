#include "bishop.h"

Bishop::Bishop(int x, int y, int side) : Piece(x, y, side) {
	this->g_is_king = false;
	this->g_is_white = side == WHITE;
	this->g_first_move = true;
	this->g_piece = BISHOP | side;
}

std::vector<Vector2> Bishop::GetValidMoves() {
	std::vector<Vector2> moves;
	if (this->g_first_move) {
		moves.push_back({ X(), Y() + 2 });
	}
	return moves;
}

Piece* Bishop::Clone() {
	return new Bishop(X(), Y(), this->g_side);
}
