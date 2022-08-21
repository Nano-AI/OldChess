#include <SDL2/SDL.h>
#include <iostream>
#include <loguru.hpp>
#include <fstream>
#include "./gui/window.h"
#include "./game/board.h"
#include "./gui/renderer.h"
#include "./gui/image.h"
#include "./game/piece.h"
#include "./game/pieces/pawn.h"

int main(int argc, char* argv[]) {
	loguru::init(argc, argv);
	loguru::add_file("Chess.log", loguru::Append, loguru::Verbosity_MAX);

	LOG_F(INFO, "Reading settings");

	std::ifstream f("settings.json");
	json data = json::parse(f);

	// Play as white or black
	Board board(data);
	Window win(800, 800, "Chess");
	Renderer render(&win, &board, data);
	SDL_Event event;
	
	bool done = false;

	while ((!done) && (SDL_WaitEvent(&event))) {
		//render.Update();
		render.HandleInput(&event);
		render.Render();
		win.HandleInput(&event);
		// Render handles window resize
		switch (event.type) {
		case SDL_QUIT:
			done = true;
			break;
		default:
			break;
		}
	}

	win.Quit(0);
	return 0;
}
