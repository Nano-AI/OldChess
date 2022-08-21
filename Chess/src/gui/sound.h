#pragma once
#include <iostream>
#include <map>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <loguru.hpp>
#include <json.hpp>
#include <vector>
#include <map>
#include <filesystem>

namespace fs = std::filesystem;


#define SOUND_MOVE      "Move"
#define SOUND_CAPTURE   "Capture"
#define SOUND_CASTLE    "Castle"
#define SOUND_CHECK     "Check"
#define SOUND_CHECKMATE "Checkmate"
#define SOUND_STALEMATE "Stalemate"

using json = nlohmann::json;

class Sound
{
public:
	inline static std::string sound_dir = "./resources/sounds/";
	Sound(json settings);
	~Sound();
	void PlaySound(std::string event);
	std::map<std::string, Mix_Music*> sounds;
protected:
	json settings;
};

