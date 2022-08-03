#include "pawn.h"

Pawn::Pawn(int x, int y, int side) : Piece(x, y, side) {
	this->g_is_king = false;
	this->g_is_white = side == WHITE;
	this->g_first_move = true;
	this->g_piece = PAWN | side;
}

std::vector<Vector2> Pawn::GetValidMoves() {
	std::vector<Vector2> moves;
	return moves;
}

Piece* Pawn::Clone() {
	return new Pawn(X(), Y(), this->g_side);
}
