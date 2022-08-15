#include "window.h"

Window::Window(int width, int height, std::string title) {
	g_width = width;
	g_height = height;
	LOG_F(INFO, "Initializing SDL2 window. %ix%i, \"%s\"", width, height, title.c_str());
	// Setup video and audio
	if ((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)) {
		LOG_F(ERROR, "Error initializing SDL2. Error:");
		LOG_F(FATAL, "%s", SDL_GetError());
		exit(-1);
	}
	LOG_F(INFO, "Creating window.");
	// Creating a window
	SDL_Window* window = window = SDL_CreateWindow(
		title.c_str(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height, 
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	SDL_Surface* screenSurface = SDL_GetWindowSurface(window);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (window == NULL) {
		LOG_F(ERROR, "Error creating window. Error:");
		LOG_F(FATAL, "%s", SDL_GetError());
		exit(-1);
	}
	// Window surface, renderer, and window set
	this->g_window = window;
	this->g_surface = screenSurface;
	this->g_renderer = renderer;
	LOG_F(INFO, "SDL initialized.");
}

void Window::Quit(int error_num) {
	SDL_FreeSurface(g_surface);
	g_surface = NULL;
	SDL_DestroyWindow(g_window);
	g_window = NULL;
	SDL_Quit();
	LOG_F(INFO, "Quitting...");
	exit(error_num);
}

void Window::HandleInput(SDL_Event* event) {
	
}

void Window::ResizeWindow(int width, int height) {
	SDL_SetWindowSize(this->g_window, width, height);
	this->g_width = width;
	this->g_height = height;
	LOG_F(INFO, "Window resized to %dx%d.", width, height);
}
