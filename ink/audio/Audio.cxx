/**
 * Copyright (C) 2021-2023 Hypertheory
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "Audio.h"

#include "../core/Error.h"

namespace Ink {

void Audio::init() {
	SDL_Init(SDL_INIT_AUDIO);
}

Audio::Audio(const std::string& p) {
	if (SDL_LoadWAV(p.c_str(), &spec, &buffer, &length) == nullptr) {
		auto error = SDL_GetError();
		printf("%s\n", error);
		Error::set("Audio", "Failed to read from WAVE file");
	}
	ratio = spec.channels * spec.freq * (spec.format >> 3 & 0x1F);
	spec.userdata = this;
	spec.callback = [](void* u, uint8_t* s, int l) -> void {
		auto* a = static_cast<Audio*>(u);
		uint8_t* buffer = a->buffer + a->position;
		uint32_t length = 0;
		uint32_t volume = SDL_MIX_MAXVOLUME * a->volume;
		if (a->position + l < a->length) {
			length = l;
			a->position += l;
		} else {
			length = a->length - a->position;
			a->position = 0;
			if (!a->loop) SDL_PauseAudioDevice(a->device, 1);
		}
		SDL_memset(s, 0, l);
		SDL_MixAudioFormat(s, buffer, a->spec.format, length, volume);
	};
	device = SDL_OpenAudioDevice(nullptr, 0, &spec, nullptr, 0);
}

Audio::~Audio() {
	SDL_CloseAudioDevice(device);
	SDL_FreeWAV(buffer);
}

void Audio::play() const {
	SDL_PauseAudioDevice(device, 0);
}

void Audio::pause() const {
	SDL_PauseAudioDevice(device, 1);
}

void Audio::stop() {
	SDL_PauseAudioDevice(device, 1);
	position = 0;
}

float Audio::get_duration() const {
	return length / ratio;
}

bool Audio::get_loop() const {
	return loop;
}

void Audio::set_loop(bool l) {
	loop = l;
}

float Audio::get_volume() const {
	return volume;
}

void Audio::set_volume(float v) {
	volume = v;
}

float Audio::get_position() const {
	return position / ratio;
}

void Audio::set_position(float p) {
	position = p * ratio;
}

}
