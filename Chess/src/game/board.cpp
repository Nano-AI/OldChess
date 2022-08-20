#include "board.h"

// I have no clue what this guy from stack overflow did
// https://stackoverflow.com/questions/236129/how-do-i-iterate-over-the-words-of-a-string
template <typename Out>
void split(const std::string &s, char delim, Out result) {
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        *result++ = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

Piece* Board::GetPiece(char value, int x, int y, int playing_side) {
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
	case 'k': {
		King* k = new King(x, y, side);
		if (side == WHITE) {
			this->white_king = k;
		}
		else {
			this->black_king = k;
		}
		return k;
	}
	default:
		LOG_F(ERROR, "Unkown piece %d", value);
		return NULL;
	}
}

Board::Board(int side) {
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

	std::vector<std::string> sfen = split(fen, ' ');

	for (char piece : sfen[0]) {
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
	
	if (sfen[1] != "w" && sfen[1] != "b") {
		LOG_F(ERROR, "Unknown side %c.", sfen[1]);
		sfen = split(this->default_fen, ' ');
	}

	if (sfen[1] == "w") {
		this->g_active_color = WHITE;
	} else if (sfen[1] == "b") {
		this->g_active_color = BLACK;
	}

	this->white_castle = { false, false };
	this->black_castle = { false, false };

	for (char side : sfen[2]) {
		switch (side) {
		case 'K':
			this->white_castle.king_side = true;
			break;
		case 'k':
			this->black_castle.king_side = true;
			break;
		case 'Q':
			this->white_castle.queen_side = true;
			break;
		case 'q':
			this->black_castle.queen_side = true;
		default:
			LOG_F(ERROR, "Unknown side for castle: %c.", side);
		}
	}
	// Other values are not needed for now
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

int Board::OppositeSide(int side) {
	return (side == WHITE) ? BLACK : WHITE;
}

bool Board::IsTurn(int side) {
	return side == this->g_active_color;
}

int Board::Move(int startX, int startY, int toX, int toY) {
	if (At(startX, startY)->g_side != this->g_active_color) {
		LOG_F(ERROR, "Not your turn.");
		return DIFFERENT_SIDE_TURN;
	}
	LOG_F(INFO, "Moving piece from (%i, %i) to (%i, %i).", startX, startY, toX, toY);
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
	g_game_board[startX][startY]->g_first_move = false;

	bool is_white_king = false, is_black_king = false;

	if (this->g_game_board[startX][startY]->g_is_king) {
		if (this->g_game_board[startX][startY]->IsWhite()) {
			is_white_king = true;
		}
		else {
			is_black_king = true;
		}
	}

	// Clear out memory
	delete(this->g_game_board[toX][toY]);
	this->g_game_board[toX][toY] = start->Clone();
	this->g_game_board[toX][toY]->g_coord = { toX, toY };
	delete(this->g_game_board[startX][startY]);
	this->g_game_board[startX][startY] = new Empty(startX, startY, EMPTY);
	
	if (is_white_king) {
		this->white_king = (King*) this->g_game_board[toX][toY];
	} if (is_black_king) {
		this->black_king = (King*) this->g_game_board[toX][toY];
	}

	this->g_active_color = OppositeSide(this->g_active_color);

	return SUCCESS;
}
