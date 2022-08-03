#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <string>

#define WHITE   0x0000
#define BLACK   0x1000

#define BLANK   0x0000
#define WPAWN   0x0001
#define WKNIGHT 0x0010
#define WBISHOP 0x0011
#define WROOK   0x0100
#define WQUEEN  0x0101
#define WKING   0x0111
#define BPAWN   0x1001
#define BKNIGHT 0x1010
#define BBISHOP 0x1011
#define BROOK   0x1100
#define BQUEEN  0x1101
#define BKING   0x1111

class Pieces {
public:
	inline static std::map<std::string, int> c_name_binary = {
		{ "Pawn"  , WPAWN   },
		{ "Bishop", WKNIGHT },
		{ "Knight", WBISHOP },
		{ "Rook"  , WROOK   },
		{ "Queen" , WQUEEN  },
		{ "King"  , WKING   }
	};
};
