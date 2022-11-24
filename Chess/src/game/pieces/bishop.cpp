#include "bishop.h"

Bishop::Bishop(int x, int y, int side) : Piece(x, y, side) {
	this->g_is_king = false;
	this->g_is_white = side == WHITE;
	this->g_first_move = true;
	this->g_piece = BISHOP | side;
}

std::vector<Vector2> Bishop::GetValidMoves(std::vector<std::vector<Piece*>> board, bool xray) {
	std::vector<Vector2> moves;
	int x = this->X();
	int y = this->Y();
	bool top_right = true, top_left = true, bottom_right = true, bottom_left = true;
	// Would've been better to just use 4 while loops...
	bool pinned_piece = false;
	for (int j = -1; j <= 1; j += 2) {
		for (int i = -1; i <= 1; i += 2) {
			bool passed = false;
			for (int k = 1; k < 8; k++) {
				int new_x = x + (k * i), new_y = y + (k * j);
				if (new_x < 0 || new_x > 7 || new_y < 0 || new_y > 7) {
					break;
				}
				if (xray && board[new_x][new_y]->g_side == this->g_side && board[new_x][new_y]->g_piece != BLANK) {
					break;
				}
				// TODO: Check if it's the same side or not
				if (board[new_x][new_y]->g_is_king && board[new_x][new_y]->g_side != this->g_side) {
					if (!xray) {
						((King*)board[new_x][new_y])->g_in_check = true;
						break;
					}
				}
				Piece* at = board[new_x][new_y];
				if (!xray) {
					moves.push_back({ new_x, new_y });
				}
				if (at->g_piece != BLANK && xray) {
					if (!passed && at->g_piece != BLANK && at->g_side != this->g_side) {
						passed = true;
						moves.push_back({ new_x, new_y });
					}
					else if (passed && at->g_is_king) {
						pinned_piece = true;
						break;
					}
					else if (passed && !at->g_is_king) {
						break;
					}
				}
				if (at->g_piece != BLANK && !xray) {
					break;
				}
			}
			if (!pinned_piece && xray) {
				moves.clear();
			}
		}
	}
	return moves;
}

std::vector<Vector2> Bishop::MovesToPiece(Piece* end, std::vector<std::vector<Piece*>> board) {
	std::vector<Vector2> spots = {};
	int m = (end->Y() - Y()) / (end->X() - X());
	int x = (end->Y() - Y()), y = (end->X() - X());
	for (auto move : end->g_moves) {
		// WE'RE TALKING ABOUT AN ARRAY, NOT A GRAPH!
		int dy = (move.y - Y());
		int dx = (move.x - X());
		if (move.x == end->X() &&  move.y == end->Y()) {
			spots.push_back(move);
		}
		if (dx == 0) {
			continue;
		}
		if ((dy / dx) == m)
			spots.push_back(move);
	}
	return spots;

}

Piece* Bishop::Clone() {
	return new Bishop(X(), Y(), this->g_side);
}
