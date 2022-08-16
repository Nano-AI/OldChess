#include "king.h"

King::King(int x, int y, int side) : Piece(x, y, side) {
	this->g_is_king = true;
	this->g_is_white = side == WHITE;
	this->g_first_move = true;
	this->g_piece = KING | side;
}

std::vector<Vector2> King::GetValidMoves(std::vector<std::vector<Piece*>> board) {
	// TODO: Store moves in one array and only update it every move
	std::vector<Vector2> moves;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			int x = X() + i;
			int y = Y() + j;
			if (x < 0 || x > 7 || y < 0 || y > 7) {
				continue;
			}
			Piece* current = board[x][y];
			if (current->g_side == this->g_side && current->g_piece != EMPTY) {
				continue;
			}
			moves.push_back(current->GetCoord());
		}
	}
	return moves;
}

Piece* King::Clone() {
	return new King(X(), Y(), this->g_side);
}
