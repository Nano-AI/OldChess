#include "queen.h"

Queen::Queen(int x, int y, int side) : Piece(x, y, side) {
	this->g_is_king = false;
	this->g_is_white = side == WHITE;
	this->g_first_move = true;
	this->g_piece = QUEEN | side;
}

std::vector<Vector2> Queen::GetValidMoves(std::vector<std::vector<Piece*>> board) {
	std::vector<Vector2> moves = {};
	int x = this->X(), y = this->Y();
	Bishop* b = new Bishop(x, y, this->g_side);
	std::vector<Vector2> b_moves = b->GetValidMoves(board);
	Rook* r = new Rook(x, y, this->g_side);
	std::vector<Vector2> r_moves = r->GetValidMoves(board);
	for (auto move : b_moves) {
		moves.push_back(move);
	}
	for (auto move : r_moves) {
		moves.push_back(move);
	}
	// There's a memory error that gets thrown every now and then.
	// No clue what's happening.
	delete(b);
	delete(r);
	return moves;
}

Piece* Queen::Clone() {
	return new Queen(X(), Y(), this->g_side);
}
