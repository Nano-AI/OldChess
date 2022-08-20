#pragma once
#include <iostream>
#include <vector>
#include <stdio.h>
#include <loguru.hpp>
#include <cctype>
#include <regex>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
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
#include "./file.h"

struct Castle {
	bool queen_side;
	bool king_side;
};

class Board
{
public:
	int g_active_color = NULL;
	Castle white_castle;
	Castle black_castle;
	std::vector<std::vector<Piece*>> GetBoard();
	std::vector<std::vector<Piece*>> g_game_board;
	std::vector<std::vector<int>> g_board_colors;
	King* white_king, * black_king;
	Board(int side);
	bool IsTurn(int side);
	int OppositeSide(int side);
	void LoadFEN(int side);
	Piece* GetPiece(char value, int x, int y, int playing_side);
	void PrintBoard();
	int Move(int startX, int startY, int toX, int toY);
	Piece* At(int x, int y);
protected:
	const std::string validate_fen_regex = ("\\s*([rnbqkpRNBQKP1-"
		"8]+\\/){7}([rnbqkpRN"
		"BQKP1-8]+)\\s[bw-]\\"
		"s(([a-hkqA-HKQ]{1,4}"
		")|(-))\\s(([a-h][36]"
		")|(-))\\s\\d+\\s\\d+"
		"\\s*");
	const std::string default_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
};
