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
	std::vector<Vector2> g_moves;
	std::vector<Vector2> g_protected_spots; // This includes pieces the same side
	std::vector<Vector2> g_xray;
	void GenerateMoves(std::vector<std::vector<Piece*>> board);
	int GetSide();
	bool IsWhite();
	int X();
	int Y();
	Piece(Vector2 coord, int side);
	Piece(int x, int y, int side);
	Vector2 GetCoord();
	//virtual bool Move(int x, int y) = 0;
	bool Move(int x, int y, std::vector<std::vector<Piece*>> board);
	virtual Piece* Clone() = 0;
	virtual std::vector<Vector2> GetValidMoves(std::vector<std::vector<Piece*>> board, bool xray = false) = 0;
	virtual std::vector<Vector2> MovesToPiece(Piece* end, std::vector<std::vector<Piece*>> board) = 0;
	//virtual std::vector<Vector2> GetValidMoves(std::vector<std::vector<Piece*>> board, bool discovered_check = false) = 0;
};

