/*
 * Copyright (C) 2021-2022 Hypertheory
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

#include "audio.h"

void audio::init() {
	SDL_Init(SDL_INIT_AUDIO);
}

audio::audio(float v, bool l) : volume(v), loop(l) {}

bool audio::load(const std::string& f) {
	if (SDL_LoadWAV(f.c_str(), &spec, &buffer, &length) == nullptr) return false;
	spec.userdata = this;
	spec.callback = [](void* ud, uint8_t* s, int l) -> void {
		SDL_memset(s, 0, l);
		audio* data = static_cast<audio*>(ud);
		if (data->position + l < data->length) {
			SDL_MixAudioFormat(s, data->buffer + data->position, data->spec.format,
							   l, SDL_MIX_MAXVOLUME * data->volume);
			data->position += l;
		} else {
			SDL_MixAudioFormat(s, data->buffer + data->position, data->spec.format,
							   data->length - data->position, SDL_MIX_MAXVOLUME * data->volume);
			data->position = 0;
			if (!data->loop) data->stop();
		}
	};
	device = SDL_OpenAudioDevice(nullptr, 0, &spec, nullptr, 0);
	return true;
}

void audio::play() {
	SDL_PauseAudioDevice(device, 0);
}

void audio::stop() {
	SDL_PauseAudioDevice(device, 1);
}

audio::~audio() {
	SDL_CloseAudioDevice(device);
	SDL_FreeWAV(buffer);
}
