#pragma once
#include "../piece.h"
#include "./king.h"
#include <loguru.hpp>

class Pawn : public Piece
{
public:
	Pawn(int x, int y, int side, int direction);
	std::vector<Vector2> GetValidMoves(std::vector<std::vector<Piece*>> board, bool xray = false);
	std::vector<Vector2> MovesToPiece(Piece* end, std::vector<std::vector<Piece*>> board);
	Piece* Clone();
private:
	int direction;
};

