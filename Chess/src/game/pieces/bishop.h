#pragma once
#include "../piece.h"
#include "./king.h"

class Bishop : public Piece
{
public:
	Bishop(int x, int y, int side);
	std::vector<Vector2> GetValidMoves(std::vector<std::vector<Piece*>> board);
	Piece* Clone();
};
