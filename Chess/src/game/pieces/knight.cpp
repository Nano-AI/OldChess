#include "knight.h"

Knight::Knight(int x, int y, int side) : Piece(x, y, side) {
	this->g_is_king = false;
	this->g_is_white = side == WHITE;
	this->g_first_move = true;
	this->g_piece = KNIGHT | side;
}

std::vector<Vector2> Knight::GetValidMoves(std::vector<std::vector<Piece*>> board, bool xray) {
	std::vector<Vector2> moves;
	int x = X();
	int y = Y();
	for (int i = -2; i <= 2; i += 4) {
		for (int j = -1; j <= 1; j += 2) {
			int x1 = x + i, y1 = y + j;
			int x2 = x + j, y2 = y + i;
			if (x1 >= 0 && x1 <=7 && y1 >= 0 && y1 <=7) {
				Piece* current1 = board[x1][y1];
				if (current1->g_is_king && current1->g_side != this->g_side) {
					((King*)current1)->g_in_check = true;
					continue;
				}
				//if (current1->g_piece == BLANK || (current1->g_side != this->g_side && current1->g_piece != BLANK)) {
					moves.push_back({ x1, y1 });
				//}
			}
			if (x2 >= 0 && x2 <=7 && y2 >= 0 && y2 <=7) {
				Piece* current2 = board[x2][y2];
				if (current2->g_is_king && current2->g_side != this->g_side) {
					((King*)current2)->g_in_check = true;
					continue;
				}
				//if (current2->g_piece == BLANK || (current2->g_side != this->g_side && current2->g_piece != BLANK)) {
					moves.push_back({ x2, y2 });
				//}
			}
		}
	}
	return moves;
}

std::vector<Vector2> Knight::MovesToPiece(Piece* end, std::vector<std::vector<Piece*>> board) {
	return std::vector<Vector2>();
}

Piece* Knight::Clone() {
	return new Knight(X(), Y(), this->g_side);
}
