#pragma once
#include "../piece.h"
#include <vector>
class King : public Piece
{
public:
	King(int x, int y, int side);
	std::vector<Vector2> GetValidMoves(std::vector<std::vector<Piece*>> board, bool discovered_check = false);
	std::vector<std::vector<std::vector<Vector2>>> moves;
	Piece* Clone();
};

