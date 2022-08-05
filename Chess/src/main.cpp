#include <SDL2/SDL.h>
#include <iostream>
#include <loguru.hpp>
#include "./gui/window.h"
#include "./game/board.h"
#include "./gui/renderer.h"
#include "./gui/image.h"
#include "./game/piece.h"
#include "./game/pieces/pawn.h"

int main(int argc, char* argv[]) {
	loguru::init(argc, argv);
	loguru::add_file("Chess.log", loguru::Append, loguru::Verbosity_MAX);

	Board board;
	Window win(800, 800, "Chess");
	// Render from white's perspective
	Renderer render(&win, &board, WHITE);
	SDL_Event event;

	bool done = false;

	board.PrintBoard();	

	while ((!done) && (SDL_WaitEvent(&event))) {
		//render.Update();
		render.HandleInput(&event);
		render.Render();
		win.HandleInput(&event);
		// Render handles window resize
		// :/
		//LOG_F(ERROR, "RAAHHHHH");
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
