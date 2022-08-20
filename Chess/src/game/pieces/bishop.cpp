#include "bishop.h"

Bishop::Bishop(int x, int y, int side) : Piece(x, y, side) {
	this->g_is_king = false;
	this->g_is_white = side == WHITE;
	this->g_first_move = true;
	this->g_piece = BISHOP | side;
}

std::vector<Vector2> Bishop::GetValidMoves(std::vector<std::vector<Piece*>> board) {
	std::vector<Vector2> moves;
	int x = this->X();
	int y = this->Y();
	bool top_right = true, top_left = true, bottom_right = true, bottom_left = true;
	// Would've been better to just use 4 while loops...
	for (int j = -1; j <= 1; j += 2) {
		for (int i = -1; i <= 1; i += 2) {
			for (int k = 1; k < 8; k++) {
				int new_x = x + (k * i), new_y = y + (k * j);
				if (new_x < 0 || new_x > 7 || new_y < 0 || new_y > 7) {
					break;
				}
				if (board[new_x][new_y]->g_side == this->g_side && board[new_x][new_y]->g_piece != BLANK) {
					break;
				}
				// TODO: Check if it's the same side or not
				if (board[new_x][new_y]->g_is_king && board[new_x][new_y]->g_side != this->g_side) {
					((King*)board[new_x][new_y])->g_in_check = true;
					break;
				}
				Piece* at = board[new_x][new_y];
				moves.push_back({ new_x, new_y });
				if (board[new_x][new_y]->g_piece != BLANK) {
					break;
				}
			}
		}
	}
	return moves;
}

Piece* Bishop::Clone() {
	return new Bishop(X(), Y(), this->g_side);
}
