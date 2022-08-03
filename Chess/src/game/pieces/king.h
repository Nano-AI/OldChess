#pragma once
#include "../piece.h"
#include <vector>
class King : public Piece
{
public:
	King(int x, int y, int side);
	std::vector<Vector2> GetValidMoves();
	Piece* Clone();
};

