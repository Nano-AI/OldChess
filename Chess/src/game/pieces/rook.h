#pragma once
#include "../piece.h"
class Rook : public Piece
{
public:
	Rook(int x, int y, int side);
	std::vector<Vector2> GetValidMoves(std::vector<std::vector<Piece*>> board, bool discovered_check = false);
	Piece* Clone();
};
