#pragma once
#include "../piece.h"

class Pawn : public Piece
{
public:
	Pawn(int x, int y, int side);
	std::vector<Vector2> GetValidMoves(std::vector<std::vector<Piece*>> board);
	Piece* Clone();
};

