#pragma once
#include "../piece.h"
#include <loguru.hpp>

class Pawn : public Piece
{
public:
	Pawn(int x, int y, int side, int direction);
	std::vector<Vector2> GetValidMoves(std::vector<std::vector<Piece*>> board, bool discovered_check = false);
	Piece* Clone();
private:
	int direction;
};

