#include "king.h"

King::King(int x, int y, int side) : Piece(x, y, side) {
	this->g_is_king = true;
	this->g_is_white = side == WHITE;
	this->g_first_move = true;
	this->g_piece = KING | side;
}

std::vector<Vector2> King::GetValidMoves(std::vector<std::vector<Piece*>> board) {
	std::vector<Vector2> moves;
	return moves;
}

Piece* King::Clone() {
	return new King(X(), Y(), this->g_side);
}
