#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "pieces.h"

struct Vector2 {
	int x;
	int y;
};

class Piece
{
public:
	// I'd put it private but I'm to lazy to make get functions.
	// SDL rectangle
	SDL_Rect g_box;
	bool g_is_white;
	bool g_is_king = false;
	bool g_first_move = true;
	Vector2 g_coord;
	int g_side;
	int g_piece;
	Piece(Vector2 coord, int side);
	Piece(int x, int y, int side);
	int GetSide();
	bool IsWhite();
	int X();
	int Y();
	Vector2 GetCoord();
	//virtual bool Move(int x, int y) = 0;
	bool Move(int x, int y);
	virtual Piece* Clone() = 0;
	virtual std::vector<Vector2> GetValidMoves() = 0;
};

