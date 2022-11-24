#include "king.h"

King::King(int x, int y, int side) : Piece(x, y, side) {
	this->g_is_king = true;
	this->g_is_white = side == WHITE;
	this->g_first_move = true;
	this->g_piece = KING | side;
	this->g_in_check = false;
}

bool operator==(const Vector2& one, const Vector2& two) {
	return one.x == two.x && one.y == two.y;
}

std::vector<Vector2> King::GetValidMoves(std::vector<std::vector<Piece*>> board, bool xray) {
	// TODO: Store moves in one array and only update it every move
	std::vector<Vector2> moves;
	std::vector<Vector2> compressed;

	bool default_check = this->g_in_check;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			int x = X() + i;
			int y = Y() + j;
			bool illegal_move = false;
			if (x < 0 || x > 7 || y < 0 || y > 7) {
				continue;
			}
			// To lazy to implement a proper method that won't iterate through 400+ moves 64 times.
			// Now that I've chnaged this, it's going to loop over a thousands of times.
			// Man this is going to take some time
			std::vector<Vector2> checkedPieces;
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					for (auto move : board[i][j]->g_protected_spots) {
						if (move.x == x && move.y == y && board[i][j]->g_side != this->g_side) {
							checkedPieces.push_back({ move.x, move.y });
							illegal_move = true;
						}
					}
					for (auto move : board[i][j]->g_moves) {
						Piece* at = board[i][j];
						Piece* to = board[move.x][move.y];
						if (at->g_is_king || at->g_side != this->g_side) {
							continue;
						}
						Piece* clone = at->Clone();
						Empty* empty = new Empty(move.x, move.y, EMPTY);
						board[i][j] = empty;
						board[move.x][move.y] = clone;
						// Do stuff
						clone->GenerateMoves(board);
						if (this->g_in_check) {
							auto m = &at->g_moves;
							m->erase(std::remove(m->begin(), m->end(), move), m->end());
							printf("Illegal for (%i, %i) to move (%i, %i).\n", i, j, move.x, move.y);
							// std::cout << "Illegal move for (): " << move.x << " " << move.y << std::endl;
						}
						// Finish stuff
						delete(clone);
						delete(empty);
						board[i][j] = at;
						board[move.x][move.y] = to;
					}
				}
			}
			this->g_in_check = default_check;
			if (illegal_move) {
				continue;
			}
			Piece* current = board[x][y];

			if (current->g_is_king) {
				break;
			}

			moves.push_back(current->GetCoord());
		}
	}
	for (auto move : moves) {
		Piece* current = board[move.x][move.y];
		if (move.x != this->X() && move.y != this->Y()) {
			continue;
		}
	}
	return moves;
}

std::vector<Vector2> King::MovesToPiece(Piece* end, std::vector<std::vector<Piece*>> board) {
	return std::vector<Vector2>();
}

Piece* King::Clone() {
	return new King(X(), Y(), this->g_side);
}