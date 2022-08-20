#pragma once
#include <iostream>
#include <map>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <loguru.hpp>

#define SOUND_MOVE      "move"
#define SOUND_CAPTURE   "capture"
#define SOUND_CASTLE    "castle"
#define SOUND_CHECK     "check"
#define SOUND_CHECKMATE "checkmate"
#define SOUND_STALEMATE "stalemate"

class Sound
{
public:
	inline static std::string sound_dir = "./resources/sounds/";
	inline static std::map<std::string, std::string> c_sound_name = {
		{ "move"      , "Move.mp3"},
		{ "capture"   , "Capture.mp3"},
		{ "castle"    , "Castle.mp3" },
		{ "check"     , "Check.mp3"},
		{ "checkmate" , "Checkmate.mp3"},
		{ "stalemate" , "Stalemate.mp3"}
	};
	Sound();
	~Sound();
	void PlaySound(std::string event);
	std::map<std::string, Mix_Music*> sounds;
};

