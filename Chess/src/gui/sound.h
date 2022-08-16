#pragma once
#include <iostream>
#include <map>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <loguru.hpp>

class Sound
{
public:
	inline static std::string sound_dir = "./resources/sounds/";
	inline static std::map<std::string, std::string> c_sound_name = {
		{ "capture"   , "Capture.mp3"},
		{ "castle"    , "Castle.mp3" },
		{ "check"     , "Check.mp3"},
		{ "checkmate" , "Checkmate.mp3"},
		{ "move"      , "Move.mp3"},
		{ "stalemate" , "Stalemate.mp3"}
	};
	Sound();
	~Sound();
	void PlaySound(std::string event);
	std::map<std::string, Mix_Music*> sounds;
};

