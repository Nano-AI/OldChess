#include "rook.h"

Rook::Rook(int x, int y, int side) : Piece(x, y, side) {
	this->g_is_king = false;
	this->g_is_white = side == WHITE;
	this->g_first_move = true;
	this->g_piece = ROOK | side;
}

std::vector<Vector2> Rook::GetValidMoves(std::vector<std::vector<Piece*>> board) {
	std::vector<Vector2> moves;
	int x = X();
	int y = Y();
	for (int d = -1; d <= 1; d += 2) {
		int i = x + d;
		while (i >= 0 && i < 8) {
			Piece* current = board[i][y];
			if (current->g_side == this->g_side && current->g_piece != EMPTY) {
				break;
			}
			if (current->g_is_king) {
				((King*)current)->g_in_check = true;
				break;
			}
			moves.push_back({ i, y });
			if (current->g_piece != EMPTY) {
				break;
			}
			i += d;
		}
	}
	for (int d = -1; d <= 1; d += 2) {
		int i = y + d;
		while (i >= 0 && i < 8) {
			Piece* current = board[x][i];
			if (current->g_side == this->g_side && current->g_piece != EMPTY) {
				break;
			}
			if (current->g_is_king) {
				((King*)current)->g_in_check = true;
				break;
			}
			moves.push_back({ x, i });
			if (current->g_piece != EMPTY) {
				break;
			}
			i += d;
		}
	}
	return moves;
}

Piece* Rook::Clone() {
	return new Rook(X(), Y(), this->g_side);
}
