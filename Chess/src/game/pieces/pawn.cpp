#include "pawn.h"

// 1 is up, -1 is down.
Pawn::Pawn(int x, int y, int side, int direction) : Piece(x, y, side) {
	this->g_is_king = false;
	this->g_is_white = side == WHITE;
	this->g_first_move = true;
	this->g_piece = PAWN | side;
	this->direction = direction;
}

std::vector<Vector2> Pawn::GetValidMoves(std::vector<std::vector<Piece*>> board) {
	std::vector<Vector2> moves;
	int x = this->g_coord.x;
	int y = this->g_coord.y;
	Piece* current;
	if (x > 0 && x < 8) {
		if (board[x + this->direction][y]->g_piece == EMPTY) {
			moves.push_back({ x + this->direction, y });
			if (this->g_first_move && board[x + 2 * this->direction][y]->g_piece == EMPTY) {
				moves.push_back({ x + (2 * direction), y});
				// This doesn't work here so it's been moved to the board's move function.
				// this->g_first_move = false;
			}
		}
		if (y >= 0 && y < 8) {
			for (int i = -1; i <= 1; i += 2) {
				if (x + this->direction < 0 || x + this->direction > 7 || y + i < 0 || y + i > 7) {
					continue;
				}
				Piece* current = board[x + this->direction][y + i];
				if (!current) {
					LOG_F(ERROR, "Current was null.");
					continue;
				}
				if (current->g_is_king && current->g_side != current->g_side) {
					((King*)current)->g_in_check = true;
					continue;
				}
				if (current->g_piece != EMPTY && current->g_side != this->g_side) {
					moves.push_back(current->GetCoord());
				}
			}
		}
	}
	return moves;
}

Piece* Pawn::Clone() {
	Pawn* copy = new Pawn(X(), Y(), this->g_side, this->direction);
	copy->g_first_move = this->g_first_move;
	return copy;
}
