#include "renderer.h"
#define INNER_PADDING 10

void DrawCircle(SDL_Renderer* renderer, int x, int y, int radius, SDL_Color color);

// Just colors for the board
// I'll switch these to SDL_Color eventually.
static int light_square_rgb[3] = { 243, 242, 242 };
static int dark_square_rgb[3] = { 125, 135, 150 };
static int background_rgb[3] = { 100, 100, 100 };
static int move_color[4] = { 180, 200, 155, 200 };

const char* c_white_dir = "./resources/pieces/white";
const char* c_black_dir = "./resources/pieces/black";

int FilterEvent(void* userdata, SDL_Event* event);

bool CoordEqual(Vector2* point1, Vector2* point2) {
	return point1->x == point2->x && point1->y == point2->y;
}

RenderMathValues CalculateDetails(Window* win) {
	RenderMathValues board;
	// Find shortest side and find size of each square
	board.shortest = (win->g_height > win->g_width) ? win->g_width : win->g_height;
	board.size = (int)(board.shortest / 8);
	// Offset for resizing
	board.x_offset = (board.shortest == win->g_width) ? 0 : (win->g_width - board.shortest) / 2;
	board.y_offset = (board.shortest == win->g_height) ? 0 : (win->g_height - board.shortest) / 2;
	return board;
}

Renderer::Renderer(Window* win, Board* board) : win(this->win), board(this->board) {
	// I need this cause it doesn't work without it :/
	this->win = win;
	this->board = board;
	// If it should be drawed from white's point of view.
	SDL_SetEventFilter(FilterEvent, this);
	Image image;
	image.win = this->win;
	image.LoadPieces(c_white_dir, c_black_dir);
	this->images = image;

	// Get sizes of images
	for (auto& [piece, texture] : this->images.g_pieces_images) {
		int x = 0;
		int y = 0;
		int p = piece;
		SDL_QueryTexture(texture, NULL, NULL, &x, &y);
		this->sizes.insert(std::pair<int, Vector2>(p, { x, y }));
	}


	auto values = CalculateDetails(this->win);
	auto size = values.size;
	auto x_offset = values.x_offset;
	auto y_offset = values.y_offset;

	this->empty_spots = std::vector<std::vector<Empty*>>(
		8,
		std::vector<Empty*>(8)
		);

	this->moves = std::vector<std::vector<std::vector<Vector2>>>(
		8,
		std::vector<std::vector<Vector2>>(8)
		);

	// Setup the empty spaces
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			this->empty_spots[x][y] = new Empty(x, y, EMPTY);
		}
	}

	this->sound = new Sound();

	LOG_F(INFO, "Setup renderer.");
}

Renderer::~Renderer() {
	delete(this->sound);
}

void Renderer::Render(bool filter_event) {
	if (filter_event) {
		SDL_GetWindowSize(win->g_window, &win->g_width, &win->g_height);
	}
	SDL_RenderClear(this->win->g_renderer);
	DrawBoard();
	DrawMoves();
	DrawPieces();
	SDL_SetRenderDrawColor(this->win->g_renderer, background_rgb[0], background_rgb[1], background_rgb[2], 255);
	//DrawPossibleMoves(this->pov_white);
	SDL_RenderPresent(this->win->g_renderer);
}

void Renderer::DrawBoard() {
	//LOG_F(ERROR, "RAAHHHHHH");
	// Get the renderer and clear it
	SDL_Renderer* render = win->g_renderer;
	auto values = CalculateDetails(this->win);
	auto size = values.size;
	auto x_offset = values.x_offset;
	auto y_offset = values.y_offset;
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			SDL_Rect rect;
			rect.x = (x * (size + 1)) + x_offset;
			rect.y = (y * (size + 1)) + y_offset;
			rect.w = size + 1;
			rect.h = size + 1;
			this->empty_spots[x][y]->g_box = rect;
			if (this->board->g_board_colors[x][y] == WHITE) {
				SDL_SetRenderDrawColor(render, light_square_rgb[0], light_square_rgb[1], light_square_rgb[2], 255);
			}
			else {
				SDL_SetRenderDrawColor(render, dark_square_rgb[0], dark_square_rgb[1], dark_square_rgb[2], 255);
			}
			SDL_RenderFillRect(render, &rect);
		}
	}
	SDL_SetRenderDrawColor(render, background_rgb[0], background_rgb[1], background_rgb[2], 255);
}

void Renderer::DrawPieces() {
	auto board = this->board->GetBoard();
	auto loaded_pieces = this->images.g_pieces_images;
	auto render = this->win->g_renderer;

	auto values = CalculateDetails(this->win);

	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			if (board[x][y]->g_piece == BLANK) {
				continue;
			}
			if (this->selected_piece && x == this->selected_piece->X() && y == this->selected_piece->Y()) {
				continue;
			}
			Piece* current = board[x][y];
			SDL_Texture* texture = loaded_pieces.find(current->g_piece)->second;
			Vector2 size = this->sizes.find(current->g_piece)->second;
			DrawPiece(current, texture, size, values);
		}
	}

	// Makes sure that the selected piece renders on top of others.
	if (this->selected_piece) {
		SDL_Texture* texture = loaded_pieces.find(this->selected_piece->g_piece)->second;
		Vector2 size = this->sizes.find(this->selected_piece->g_piece)->second;
		DrawPiece(this->selected_piece, texture, size, values);
	}
}

void Renderer::DrawPiece(Piece* piece, SDL_Texture* piece_texture, Vector2 image_size, RenderMathValues values) {
	int size = values.size, x_offset = values.x_offset, y_offset = values.y_offset, x = piece->X(), y = piece->Y();

	SDL_Rect rect;
	// Correct scaling to maintain aspect ratio of images
	if (image_size.x > image_size.y) {
		float ratio = (float)image_size.x / (float)image_size.y;
		rect.w = size - 2 * INNER_PADDING;
		rect.h = rect.w / ratio;

	}
	else {
		float ratio = (float)image_size.y / (float)image_size.x;
		rect.h = size - 2 * INNER_PADDING;
		rect.w = rect.h / ratio;
	}
	// Set the size of the images
	// Set the offset of the piece (not centered, but at the top left of the square)
	if (this->selected_piece &&
		piece->g_coord.x == this->selected_piece->g_coord.x &&
		piece->g_coord.y == this->selected_piece->g_coord.y) {
		// Move selected piece to a coordinate and center it on the mouse
		SDL_Point clickOffset;
		rect.x = this->mouse_pos.x - (rect.w) / 2;
		rect.y = this->mouse_pos.y - (rect.h) / 2;
	}
	else {
		// Swap so we render up down instead of left right
		rect.x = (y * (size + 1)) + x_offset;
		rect.y = (x * (size + 1)) + y_offset;
		// Center the piece in the box
		rect.x += (size - rect.w) / 2;
		rect.y += (size - rect.h) / 2;
	}
	piece->g_box = rect;
	// TODO: Error being thrown here after playing too many moves
	/*
	Exception thrown at 0x00007FFECCDA48BF (SDL2.dll) in Chess.exe: 0xC0000005: Access violation reading location 0x0000024D00001011.
	*/
	if (!piece_texture || !&rect) {
		LOG_F(ERROR, "Piece texture and/or rect is NULL.");
		return;
	}
	SDL_RenderCopy(this->win->g_renderer, piece_texture, NULL, &rect);
}

bool Renderer::UpdateMoves() {
	if (!this->selected_piece) {
		return false;
	}

	int sx = this->selected_piece->X();
	int sy = this->selected_piece->Y();

	std::vector<Vector2> piece_moves = this->moves[sx][sy];

	if (piece_moves.size() == 0) {
		std::vector<Vector2> mvs = this->selected_piece->GetValidMoves(this->board->g_game_board);
		if (mvs.size() == 0) {
			mvs.push_back({ NULL, NULL });
		}
		this->moves[sx][sy] = mvs;
		return true;
	}
	else if (piece_moves.size() == 1 && piece_moves[0].x == NULL && piece_moves[0].y == NULL) {
		return false;
	}
}

void Renderer::ClearMoves() {
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			this->moves[x][y].clear();
		}
	}
}

void Renderer::DrawMoves() {
	bool should_render = UpdateMoves();
	if (!should_render) {
		return;
	}

	int sx = this->selected_piece->X(), sy = this->selected_piece->Y();

	RenderMathValues values = CalculateDetails(this->win);
	int size = values.size;
	int x_offset = values.x_offset;
	int y_offset = values.y_offset;

	SDL_Renderer* render = this->win->g_renderer;
	SDL_Surface* surface = this->win->g_surface;

	int circle_size = size / 8;

	for (Vector2 move : this->moves[sx][sy]) {
		int x = move.x;
		int y = move.y;
		if (x >= 8 || x < 0 || y >= 8 || y < 0) {
			continue;
		}
		SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);
		if (this->board->At(x, y)->g_piece == EMPTY) {
			DrawCircle(render,
				(y * (size + 1)) + x_offset + (size / 2),
				(x * (size + 1)) + y_offset + (size / 2),
				circle_size,
				{ 180, 200, 155, 200 }
			);
		}
		else {
			SDL_Rect rect;
			rect.x = (y * (size + 1) + x_offset);
			rect.y = (x * (size + 1) + y_offset);
			rect.w = size + 1;
			rect.h = size + 1;
			SDL_SetRenderDrawColor(render, move_color[0], move_color[1], move_color[2], move_color[3]);
			SDL_RenderFillRect(render, &rect);
			SDL_RenderDrawRect(render, &rect);
		}
		SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_NONE);
	}
}

int Renderer::HandleInput(SDL_Event* event) {
	// Window event handling
	switch (event->type) {
	case SDL_WINDOWEVENT:
		if (event->type == SDL_WINDOWEVENT && (event->window.event == SDL_WINDOWEVENT_RESIZED || event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED)) {
			LOG_F(INFO, "Resizing window...");
			DrawBoard();
			this->win->ResizeWindow(event->window.data1, event->window.data2);
		}
		break;
	case SDL_MOUSEMOTION:
		this->mouse_pos = { event->motion.x, event->motion.y };
	case SDL_MOUSEBUTTONDOWN:
		if (!this->mouse_down && event->button.button == SDL_BUTTON_LEFT) {
			this->mouse_down = true;
			for (int x = 0; x < 8; x++) {
				for (int y = 0; y < 8; y++) {
					if (this->board->g_game_board[x][y] == BLANK)
						break;
					Piece* piece = this->board->g_game_board[x][y];
					if (SDL_PointInRect(&this->mouse_pos, &piece->g_box)) {
						this->selected_piece = piece;
						break;
					}
				}
			}
		}
		break;
	case SDL_MOUSEBUTTONUP:
		if (this->mouse_down && event->button.button == SDL_BUTTON_LEFT) {
			MouseUp(event);
			//this->board->PrintBoard();
			this->mouse_down = false;
			this->selected_piece = NULL;
		}
		break;
	default:
		break;
	}
	return 1;
}

void Renderer::MouseUp(SDL_Event* event) {
	if (!this->mouse_down || this->selected_piece == NULL) {
		return;
	}
	Piece* dropped_box = NULL;
	for (int x = 0; x < 8; x++) {
		if (dropped_box != NULL) {
			break;
		}
		for (int y = 0; y < 8; y++) {
			/* Make sure that the piece isn't the same as the selected piece that the piece was in */
			//if (CoordEqual(&this->selected_piece->g_coord, &this->board->g_game_board[x][y]->g_coord) ||
			//    /* Check for empty boxes as well */
			//    CoordEqual(&this->selected_piece->g_coord, &this->empty_spots[x][y]->g_coord)) {
			//    continue;
			//}
			if (/* Check if player dropped it on a piece */
				//SDL_PointInRect(&this->mouse_pos, &this->board->g_game_board[x][y]->g_box) ||
				/* Check if player dropped it on an empty box */
				SDL_PointInRect(&this->mouse_pos, &this->empty_spots[x][y]->g_box)) {
				dropped_box = this->empty_spots[y][x];
				break;
			}
		}
	}
	if (dropped_box == NULL) {
		return;
	}
	Vector2 start = this->selected_piece->g_coord;
	Vector2 end = dropped_box->g_coord;
	Piece* dropped = this->board->At(end.x, end.y);
	bool capture = (dropped->g_piece != EMPTY);
	// Honestly, I'm not sure why this is the way it is, but it works and I'm too lazy to make a proper implementation
	int msg = this->board->Move(start.x, start.y, end.x, end.y);
	if (msg == SUCCESS) {
		if (capture) {
			this->sound->PlaySound("capture");
		}
		else {
			this->sound->PlaySound("move");
		}
		ClearMoves();
	}
}


int FilterEvent(void* userdata, SDL_Event* event) {
	if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED) {
		// convert userdata pointer to yours and trigger your own draw function
		// this is called very often now
		// IMPORTANT: Might be called from a different thread, see SDL_SetEventFilter docs	
		((Renderer*)userdata)->Render(true);
		//return 0 if you don't want to handle this event twice
		return 0;
	}
	//important to allow all events, or your SDL_PollEvent doesn't get any event
	return 1;
}

void DrawCircle(SDL_Renderer* renderer, int x, int y, int radius, SDL_Color color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	for (int w = 0; w < radius * 2; w++)
	{
		for (int h = 0; h < radius * 2; h++)
		{
			int dx = radius - w; // horizontal offset
			int dy = radius - h; // vertical offset
			if ((dx * dx + dy * dy) <= (radius * radius))
			{
				SDL_RenderDrawPoint(renderer, x + dx, y + dy);
			}
		}
	}
}
