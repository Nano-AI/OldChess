#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <loguru.hpp>
#include <map>
#include <json.hpp>
#include "window.h"
#include "../game/pieces.h"
#include "../file.h"

namespace fs = std::filesystem;
using json = nlohmann::json;

class Image
{
public:
	Window* win;
	std::map<int, SDL_Texture*> g_pieces_images;
	//Image(Window* window);
	Image(Window* win);
	static SDL_Texture* LoadImage(SDL_Renderer* renderer, const char* file);
	static SDL_Texture* LoadSVG(SDL_Renderer* renderer, const char* file);
	std::map<int, SDL_Texture*> LoadDirectory(std::string directory, int side = WHITE);
	void LoadPieces(json settings);
	//SDL_Surface* g_loaded_images[12];
protected:
	json settings;
};