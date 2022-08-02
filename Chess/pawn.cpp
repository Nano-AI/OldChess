#include "pawn.h"

Pawn::Pawn(int x, int y, int side) : Piece(x, y, side) {

}

std::vector<Vector2> Pawn::GetValidMoves() {
	std::vector<Vector2> moves;
	if (this->g_first_move) {
		moves.push_back({ X(), Y() + 2});
	}
}
