#include <SDL2/SDL.h>
#include <iostream>
#include <loguru.hpp>
#include "window.h"
#include "board.h"
#include "renderer.h"
#include "image.h"
#include "piece.h"
#include "pawn.h"

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

	Pawn p = Pawn(0, 1, WHITE);
	

	while ((!done) && (SDL_WaitEvent(&event))) {
		//render.Update();
		render.Render();
		win.HandleInput(&event);
		// Render handles window resize
		// :/
		render.HandleInput(&event);
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
