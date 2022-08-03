#pragma once
#include <iostream>
#include <vector>
#include <stdio.h>
#include "./pieces.h"

class Board
{
public:
	Board();
	void PrintBoard();
	std::vector<std::vector<int>> GetBoard();
private:
	std::vector<std::vector<int>> game_board;
	//int game_board[8][8];
};

