#pragma once
#include <functional>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include <loguru.hpp>

class Window
{
public:
	int g_width, g_height;
	SDL_Window* g_window;
	SDL_Surface* g_surface;
	SDL_Renderer* g_renderer;

	Window(int height, int width, std::string title);
	void Quit(int error_num);
	void HandleInput(SDL_Event* event);
	void ResizeWindow(int width, int height);
};

