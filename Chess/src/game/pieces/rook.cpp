#include "rook.h"

Rook::Rook(int x, int y, int side) : Piece(x, y, side) {
	this->g_is_king = false;
	this->g_is_white = side == WHITE;
	this->g_first_move = true;
	this->g_piece = ROOK | side;
}

std::vector<Vector2> Rook::GetValidMoves(std::vector<std::vector<Piece*>> board, bool xray) {
	std::vector<Vector2> moves;
	int x = X();
	int y = Y();
	bool passed;
	bool pinned_piece = false;
	for (int d = -1; d <= 1; d += 2) {
		int i = x + d;
		passed = false;
		while (i >= 0 && i < 8) {
			Piece* current = board[i][y];
			if (xray && current->g_side == this->g_side && current->g_piece != EMPTY) {
				break;
			}
			if (current->g_is_king && current->g_side != this->g_side) {
				if (!xray) {
					((King*)current)->g_in_check = true;
					break;
				}
			}
			if (!xray)
				moves.push_back({ i, y });	
			if (current->g_piece != EMPTY && xray) {
				if (!passed && current->g_piece != BLANK && current->g_side != this->g_side) {
					passed = true;
					moves.push_back({ i, y });
				}
				else if (passed && current->g_is_king) {
					pinned_piece = true;
					// I could just return the pinned piece here because there's only one king
					// But I don't know if there's any situation where it might help.
					break;
				}
				else if (passed && !current->g_is_king) {
					break;
				}
			}
			if (current->g_piece != BLANK && !xray) {
				break;
			}
			i += d;
		}
		if (!pinned_piece && xray) {
			moves.clear();
		}
	}

	for (int d = -1; d <= 1; d += 2) {
		int i = y + d;
		passed = false;
		while (i >= 0 && i < 8) {
			Piece* current = board[x][i];
			if (xray && current->g_side == this->g_side && current->g_piece != EMPTY) {
				break;
			}
			if (current->g_is_king && current->g_side != this->g_side) {
				//if (!xray) {
					((King*)current)->g_in_check = true;
					break;
				//}
			}
			if (!xray)
				moves.push_back({ x, i });	
			// Get pinned piece
			if (current->g_piece != EMPTY && xray) {
				if (!passed && current->g_piece != BLANK && current->g_side != this->g_side) {
					moves.push_back({ x, i });
					passed = true;
				}
				else if (passed && current->g_is_king) {
					pinned_piece = true;
					break;
				}
			}
			else if (current->g_piece != BLANK) {
				break;
			}
			i += d;
		}
		// Clear because we haven't got any pinned pieces
		if (!pinned_piece && xray) {
			moves.clear();
		}
	}
	return moves;
}

std::vector<Vector2> Rook::MovesToPiece(Piece* end, std::vector<std::vector<Piece*>> board) {
	std::vector<Vector2> spots = {};
	for (auto move : end->g_moves) {
		if (X() == move.x || Y() == move.y) {
			spots.push_back(move);
		}
	}
	return spots;
}

Piece* Rook::Clone() {
	return new Rook(X(), Y(), this->g_side);
}
