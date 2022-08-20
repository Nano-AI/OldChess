#pragma once
#include "../piece.h"
#include <vector>
class King : public Piece
{
public:
	bool g_in_check;
	King(int x, int y, int side);
	std::vector<Vector2> GetValidMoves(std::vector<std::vector<Piece*>> board);
	std::vector<std::vector<std::vector<Vector2>>> moves;
	Piece* Clone();
};

