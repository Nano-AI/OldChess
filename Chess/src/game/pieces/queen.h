#pragma once
#include "../piece.h"

class Queen : public Piece
{
public:
	Queen(int x, int y, int side);
	std::vector<Vector2> GetValidMoves();
	Piece* Clone();
};

