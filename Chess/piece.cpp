#include "piece.h"

Piece::Piece(int x, int y, int side) {
	this->coord = { x, y };
	this->side = side;
}

Piece::Piece(Vector2 coord, int side) {
	this->coord = coord;
	this->side = side;
}

int Piece::GetSide() {
	this->side;
}

bool Piece::IsWhite() {
	return this->side == WHITE;
}

int Piece::X() {
	return this->coord.x;
}

int Piece::Y() {
	return this->coord.y;
}

Vector2 Piece::GetCoord() {
	return this->coord;
}
