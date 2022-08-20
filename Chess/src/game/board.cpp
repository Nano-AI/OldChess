#include "board.h"

Piece* GetPiece(char value, int x, int y, int playing_side) {
	int side = isupper(value) ? WHITE : BLACK;
	int direction = (side == playing_side) ? -1 : 1;
	switch (tolower(value)) {
	case 'p':
		return new Pawn(x, y, side, direction);
	case 'b':
		return new Bishop(x, y, side);
	case 'n':
		return new Knight(x, y, side);
	case 'r':
		return new Rook(x, y, side);
	case 'q':
		return new Queen(x, y, side);
	case 'k':
		return new King(x, y, side);
	default:
		LOG_F(ERROR, "Unkown piece %s", value);
	}
	return NULL;
}

Board::Board(int side) {
	this->white_king = new King(0, 0, WHITE);
	this->black_king = new King(0, 0, WHITE);
	// Use the board.fen in resources some day
	// Create a vector with 8 rows and 8 columns, with the default value BLANK.
	this->g_game_board = std::vector<std::vector<Piece*>>(
		8,
		std::vector<Piece*>(8)
	);
	this->g_board_colors = std::vector<std::vector<int>>(
		8,
		std::vector<int>(8)
	);
	LoadFEN(side);

	int playing = side;
	int other = (playing == WHITE) ? BLACK : WHITE;

	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			this->g_board_colors[x][y] = ((x + y) % 2 == 0) ? other : playing;
		}
	}
}

void Board::LoadFEN(int side) {
	int playing = side;
	int other = (playing == WHITE) ? BLACK : WHITE;
	LOG_F(INFO, "Loading board using FEN...");
	std::string fen = File::ReadFile("./resources/board.fen");
	if (std::regex_match(fen, std::regex(this->validate_fen_regex))) {
		LOG_F(INFO, "Valid FEN: %s", fen.c_str());
	}
	else {
		LOG_F(ERROR, "Invalid FEN: %s! Using default FEN.", fen.c_str());
		fen = this->default_fen;
	}
	int start, end, direction;
	if (side == WHITE) {
		start = 0;
		end = 7;
		direction = 1;
	}
	else {
		start = 7;
		end = 0;
		direction = -1;
	}
	int x = start, y = 0;
	for (char piece : fen) {
		// If reached end of FEN
		if (piece == ' ') {// || (y == 8 && (x == 0 || x == 7))) {
			break;
		}
		// Reset x value and go down a row
		if (piece == '/') {
			x += direction;
			y = 0;
			continue;
		}
		if (isdigit(piece)) {
			int inc = (int)piece - 48;
			int stop = y + inc;
			while (y < stop) {
				this->g_game_board[x][y] = new Empty(x, y, EMPTY);
				y++;
			}
			continue;
		}
		else {
			this->g_game_board[x][y] = GetPiece(piece, x, y, side);
			this->g_game_board[x][y]->g_coord = { x, y };
		}
		y++;
	}
}

void Board::PrintBoard() {
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			printf("|0x%0.4x|\t", this->g_game_board[x][y]->g_piece);
		}
		printf("\n");
	}
}

std::vector<std::vector<Piece*>> Board::GetBoard() {
	return this->g_game_board;
}

Piece* Board::At(int x, int y) {
	if (x > 8 || y > 8 || x < 0 || y < 0) {
		LOG_F(ERROR, "Trying to access (%i, %i) which is out of bounds.", x, y);
		return NULL;
	}
	return this->g_game_board[x][y];
}

int Board::Move(int startX, int startY, int toX, int toY) {
	Piece* start = At(startX, startY);
	Piece* end = At(toX, toY);

	if (start == NULL || end == NULL) {
		LOG_F(ERROR, "You tried to place a piece outside of the board. How is this possible?");
		return OUT_OF_BOUNDS;
	}
	if (start->g_side == end->g_side && end->g_piece != EMPTY) {
		LOG_F(ERROR, "You can't capture your own piece you idiot.");
		return SAME_SIDE_CAPTURE;
	}
	if (!start->Move(end->X(), end->Y(), this->GetBoard())) {
		LOG_F(ERROR, "Do you know how to play chess?");
		return ILLEGAL_PIECE_MOVE;
	}

	// Because we don't discriminate in the game of chess!
	if (this->g_game_board[startX][startY]->g_piece == WPAWN || 
		this->g_game_board[startX][startY]->g_piece == BPAWN) {
		g_game_board[startX][startY]->g_first_move = false;
	}

	// Clear out memory
	delete(this->g_game_board[toX][toY]);
	this->g_game_board[toX][toY] = start->Clone();
	this->g_game_board[toX][toY]->g_coord = { toX, toY };
	delete(this->g_game_board[startX][startY]);
	this->g_game_board[startX][startY] = new Empty(startX, startY, EMPTY);
	
	return SUCCESS;
}
