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


class Board
{
public:
	Board();
	void PrintBoard();
	std::vector<std::vector<Piece*>> GetBoard();
private:
	std::vector<std::vector<Piece*>> game_board;
	//int game_board[8][8];
};

