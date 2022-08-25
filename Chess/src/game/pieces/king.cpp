#include "king.h"

King::King(int x, int y, int side) : Piece(x, y, side) {
	this->g_is_king = true;
	this->g_is_white = side == WHITE;
	this->g_first_move = true;
	this->g_piece = KING | side;
	this->g_in_check = false;
}

std::vector<Vector2> King::GetValidMoves(std::vector<std::vector<Piece*>> board, bool xray) {
	// TODO: Store moves in one array and only update it every move
	std::vector<Vector2> moves;
	std::vector<Vector2> compressed;

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
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					for (auto move : board[i][j]->g_protected_spots) {
						if (move.x == x && move.y == y && board[i][j]->g_side != this->g_side) {
							illegal_move = true;
						}
					}
					for (auto move : board[i][j]->g_xray) {
						//Empty* empty = new Empty(move.x, move.y, EMPTY);
						//Piece* at = board[i][j];
						//Piece* to = board[move.x][move.y];
						//board[i][j] = empty;
						//board[move.x][move.y] = at->Clone();
						//// Do stuff
						//Piece* clone = board[move.x][move.y];
						//clone->GenerateMoves(board);
						//if (!this->g_in_check) {
						//	
						//}
						//// Finish stuff
						//delete(board[move.x][move.y]);
						//board[i][j] = at;
						//board[move.x][move.y] = to;
						//delete(empty);
						std::vector<Vector2> moves_to_piece = board[i][j]->MovesToPiece(board[move.x][move.y], board);
						if (moves_to_piece.size() != 0) {
							board[move.x][move.y]->g_moves.clear();
							board[move.x][move.y]->g_moves = moves_to_piece;
						}
					}
				}
			}
			if (illegal_move) {
				continue;
			}
			Piece* current = board[x][y];
			//if (current->g_side == this->g_side && current->g_piece != EMPTY) {
			//	continue;
			//}
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
