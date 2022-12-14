#include "piece.h"

Piece::Piece(int x, int y, int side) {
	this->g_coord = { x, y };
	this->g_side = side;
	this->g_moves = std::vector<Vector2>();
}

Piece::Piece(Vector2 coord, int side) {
	this->g_coord = coord;
	this->g_side = side;
	this->g_moves = std::vector<Vector2>();
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
void Piece::GenerateMoves(std::vector<std::vector<Piece*>> board) {
	this->g_moves.clear();
	this->g_protected_spots.clear();
	this->g_xray.clear();
	std::vector<Vector2> moves = GetValidMoves(board);
	this->g_xray = GetValidMoves(board, true);
	for (auto move : moves) {
		Piece* spot = board[move.x][move.y];
		if (spot->g_side != this->g_side || (spot->g_piece == EMPTY && spot->g_side == EMPTY)) {
			g_moves.push_back(move);
		}
		this->g_protected_spots.push_back(move);
	}
	for (auto move : this->g_xray) {
		Piece* spot = board[move.x][move.y];
		if (spot->g_piece == BLANK) {
			continue;
		}
	}
}

bool Piece::Move(int x, int y, std::vector<std::vector<Piece*>> board) {
	for (Vector2 move : this->g_moves) {
		if (move.x == x && move.y == y) {
			return true;
		}
	}
	return false;
}

