#pragma once
#include "../piece.h"
#include "./king.h"

class Rook : public Piece
{
public:
	Rook(int x, int y, int side);
	std::vector<Vector2> GetValidMoves(std::vector<std::vector<Piece*>> board, bool xray = false);
	std::vector<Vector2> MovesToPiece(Piece* end, std::vector<std::vector<Piece*>> board);
	Piece* Clone();
};
