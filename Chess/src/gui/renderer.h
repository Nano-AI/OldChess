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
#include "./sound.h"

namespace fs = std::filesystem;

struct RenderMathValues {
    int shortest;
    int size;
    int x_offset;
    int y_offset;
};

class Renderer
{
public:
	bool pov_white;
	Renderer(Window* win, Board* board, json settings);
	~Renderer();
	void UpdateMoves();
	void ClearMoves();
	void Render(bool filter_event = false);
	void Update();
	void DrawBoard();
	void DrawPieces();
	void DrawMoves();
	void DrawCheck();
	void DrawPiece(Piece* piece, SDL_Texture* piece_texture, Vector2 image_size, RenderMathValues values);
	int HandleInput(SDL_Event* event);
private:
	bool mouse_down;
	Sound* sound;
	Image* images;
	Window* win;
	Board* board;
	SDL_Point mouse_pos;
	Piece* selected_piece;
	json settings;
	std::vector<std::vector<Empty*>> empty_spots;
	std::vector<std::vector<
		std::vector<Vector2>
	>> moves;
	std::map<int, Vector2> sizes;
	void MouseUp(SDL_Event* event);
	//int FilterEvent(void* userdata, SDL_Event* event);
};

