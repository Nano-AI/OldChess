#pragma once
#include "../piece.h"
#include "rook.h"
#include "empty.h"
#include <vector>
class King : public Piece
{
public:
	bool g_in_check;
	King(int x, int y, int side);
	std::vector<Vector2> GetValidMoves(std::vector<std::vector<Piece*>> board, bool xray = false);
	std::vector<std::vector<std::vector<Vector2>>> moves;
	std::vector<Vector2> MovesToPiece(Piece* end, std::vector<std::vector<Piece*>> board);
	Piece* Clone();
};

