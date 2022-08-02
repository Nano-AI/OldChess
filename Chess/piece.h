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
protected:
	bool g_is_white;
	bool g_is_king = false;
	bool g_first_move = true;
	Vector2 coord;
	int side;
public:
	// SDL rectangle
	SDL_Rect box;
	Piece(Vector2 coord, int side);
	Piece(int x, int y, int side);
	int GetSide();
	bool IsWhite();
	int X();
	int Y();
	Vector2 GetCoord();
	virtual bool Move(int x, int y);
	virtual Piece* Clone();
	virtual std::vector<Vector2> GetValidMoves();
	virtual bool IsKing();
};

