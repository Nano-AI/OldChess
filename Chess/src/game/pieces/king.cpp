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
			// Man this is going to take some time
			// for (int i = 0; i < 8; i++) {
			// 	for (int j = 0; j < 8; j++) {
			// 		for (auto move : this->moves[i][j]) {
			// 			if (move.x == x && move.y == y && board[i][j]->g_side != this->g_side) {
			// 				illegal_move = true;
			// 				break;
			// 			}
			// 		}
			// 	}
			// }
			for (auto move : compressed) {
			}
			if (illegal_move) {
				std::cout << "IOLEGLAL " << x << " " << y << std::endl;
				continue;
			}
			Piece* current = board[x][y];
			if (current->g_side == this->g_side && current->g_piece != EMPTY) {
				continue;
			}
			if (current->g_is_king) {
				break;
			}
			std::cout << "is ok! " << x << " " << y << std::endl;
			moves.push_back(current->GetCoord());
		}
	}
	return moves;

}

Piece* King::Clone() {
	return new King(X(), Y(), this->g_side);
}
