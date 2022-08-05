#pragma once
#include <iostream>
#include <vector>
#include <stdio.h>
#include <loguru.hpp>
#include "./pieces.h"
#include "./piece.h"
#include "./pieces/empty.h"
#include "./pieces/pawn.h"
#include "./pieces/knight.h"
#include "./pieces/bishop.h"
#include "./pieces/rook.h"
#include "./pieces/queen.h"
#include "./pieces/king.h"
#include "./errors.h"


class Board
{
public:
	Board();
	void PrintBoard();
	std::vector<std::vector<Piece*>> GetBoard();
	std::vector<std::vector<Piece*>> g_game_board;
	int Move(int startX, int startY, int toX, int toY);
	/*int Move(Vector2 start, Vector2 end);
	int Move(Piece* start, Piece* end);*/
	Piece* At(int x, int y);
	//int game_board[8][8];
};

