#include "../../ink/utils/Mainloop.h"

Ink::Audio* music;

void conf(Settings& t) {
	t.title = "Audio Test";
	t.width = 960;
	t.height = 540;
}

void load() {
	Ink::Audio::init();
	music = new Ink::Audio("test/audio/tilbury_fair.wav");
	music->play();
	
	int duration = music->get_duration();
	int minutes = duration / 60;
	int seconds = duration % 60;
	std::string min_sec;
	min_sec += (minutes < 10 ? "0" : "") + std::to_string(minutes) + ":";
	min_sec += (seconds < 10 ? "0" : "") + std::to_string(seconds);
	std::cout << "Duration  " << min_sec << "\n";
}

void update(float dt) {}

void quit() {}
