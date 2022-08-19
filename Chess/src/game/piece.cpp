#include "piece.h"

Piece::Piece(int x, int y, int side) {
	this->g_coord = { x, y };
	this->g_side = side;
}

Piece::Piece(Vector2 coord, int side) {
	this->g_coord = coord;
	this->g_side = side;
}

int Piece::GetSide() {
	return this->g_side;
}

bool Piece::IsWhite() {
	return this->g_side == WHITE;
}

int Piece::X() {
	return this->g_coord.x;
}

int Piece::Y() {
	return this->g_coord.y;
}

Vector2 Piece::GetCoord() {
	return this->g_coord;
}

bool Piece::Move(int x, int y, std::vector<std::vector<Piece*>> board, bool discovered_check) {
	std::vector<Vector2> valid_moves = GetValidMoves(board, discovered_check);
	for (Vector2 move : valid_moves) {
		if (move.x == x && move.y == y) {
			return true;
		}
	}
	return false;
}

