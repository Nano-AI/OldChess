#include "sound.h"


Sound::Sound(json settings) {
	LOG_F(INFO, "Initializing sounds...");
	this->settings = settings;
	int result = 0;
	int flags = MIX_INIT_MP3;
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		LOG_F(FATAL, "Failed to init SDL. Error:\n%s", SDL_GetError());
	}
	if (flags != (result = Mix_Init(flags))) {
		LOG_F(FATAL, "Could not initialize mixer: %d. Error:\n%s", result, SDL_GetError());
	}
	Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
	std::cout << settings << "\n" << settings["events"] << std::endl;
	for (auto const& item : settings["events"].items()) {
		std::string event = item.key();
		// Have to do this stupid thing because the guy who made SDL_Mixer thought that audio files didn't need to be relative
		std::string filename = (std::string) settings["directory"] + (std::string)item.value() + (std::string)settings["extension"];
		Mix_Music* music = Mix_LoadMUS((SDL_GetBasePath() + filename).c_str());
		sounds.insert(std::pair(event, music));
		LOG_F(INFO, "Loaded event \"%s\" from file \"%s\".", event.c_str(), filename.c_str());
	}
}

Sound::~Sound() {
	for (auto const& [event, audio] : this->sounds) {
		Mix_FreeMusic(audio);
	}
}

void Sound::PlaySound(std::string event) {
	LOG_F(INFO, "Playing sound %s.", this->sounds.find(event)->first.c_str());
	Mix_PlayMusic(this->sounds.find(event)->second, 1);
}

