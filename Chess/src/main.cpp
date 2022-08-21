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

Board* board;
Window* win;
Renderer* render;
SDL_Event event;

int FilterEvent(void* userdata, SDL_Event* event) {
	if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED) {
		// convert userdata pointer to yours and trigger your own draw function
		// this is called very often now
		// IMPORTANT: Might be called from a different thread, see SDL_SetEventFilter docs
		// ((Renderer*)userdata)->Render(true);
		render->Render(true);
		//static_cast<Renderer*>(userdata)->Render(true);
		//return 0 if you don't want to handle this event twice
		return 0;
	}
	//important to allow all events, or your SDL_PollEvent doesn't get any event
	return 1;
}


int main(int argc, char* argv[]) {
	loguru::init(argc, argv);
	loguru::add_file("Chess.log", loguru::Append, loguru::Verbosity_MAX);

	LOG_F(INFO, "Reading settings");

	std::ifstream f("settings.json");
	json data = json::parse(f);

	// Play as white or black
	board = new Board(data);
	win = new Window(800, 800, "Chess");
	render = new Renderer(win, board, data);
	
	SDL_SetEventFilter(FilterEvent, NULL);
	bool done = false;

	while ((!done) && (SDL_WaitEvent(&event))) {
		//render.Update();
		render->HandleInput(&event);
		render->Render();
		win->HandleInput(&event);
		// Render handles window resize
		switch (event.type) {
		case SDL_QUIT:
			done = true;
			break;
		default:
			break;
		}
	}

	win->Quit(0);
	return 0;
}
