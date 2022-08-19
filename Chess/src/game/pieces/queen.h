#pragma once
#include "../piece.h"
#include "./bishop.h"
#include "./rook.h"

class Queen : public Piece
{
public:
	Queen(int x, int y, int side);
	std::vector<Vector2> GetValidMoves(std::vector<std::vector<Piece*>> board, bool discovered_check = false);
	Piece* Clone();
};

