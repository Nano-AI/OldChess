#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <loguru.hpp>
#include <map>
#include "window.h"
#include "../game/pieces.h"

namespace fs = std::filesystem;

class Image
{
public:
	Window* win;
	std::map<int, SDL_Texture*> g_pieces_images;
	//Image(Window* window);
	Image();
	static SDL_Texture* LoadImage(SDL_Renderer* renderer, const char* file);
	std::map<int, SDL_Texture*> LoadDirectory(const char* directory, int side = WHITE);
	void LoadPieces(const char* white_dir, const char* black_dir);
	//SDL_Surface* g_loaded_images[12];
};