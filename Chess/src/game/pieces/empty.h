#pragma once
#include "../piece.h"
#include <vector>
class Empty : public Piece
{
public:
	Empty(int x, int y, int side);
	std::vector<Vector2> GetValidMoves(std::vector<std::vector<Piece*>> board, bool discovered_check = false);
	Piece* Clone();
};

