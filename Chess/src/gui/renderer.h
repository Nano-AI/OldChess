#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <loguru.hpp>
#include <string>
#include <iostream>
#include <filesystem>
#include <stdlib.h>
#include "window.h"
#include "image.h"
#include "../game/board.h"

namespace fs = std::filesystem;

class Renderer
{
public:
	bool pov_white;
	Renderer(Window* win, Board* board);
	void Render(bool filter_event = false);
	void Update();
	void DrawBoard();
	void DrawPieces();
	int HandleInput(SDL_Event* event);
private:
	Image images;
	Window* win;
	Board* board;
	SDL_Point mouse_pos;
	bool mouse_down;
	Piece* selected_piece;
	std::vector<std::vector<Empty*>> empty_spots;
	std::map<int, Vector2> sizes;
	void MouseUp(SDL_Event* event);
	//int FilterEvent(void* userdata, SDL_Event* event);
};

