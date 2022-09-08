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

#pragma once

#include <string>
#include <SDL2/SDL.h>

class audio {
public:
	bool loop = false;		/* audio is looped */
	float volume = 1;		/* audio volume from 0 to 1 */
	int position = 0;		/* the length of data audio has been playing */
	
	/**
	 * Init audio.
	 */
	static void init();
	
	/**
	 * Create a new audio.
	 *
	 * \param v volume
	 * \param l loop
	 */
	audio(float v = 1, bool l = false);
	
	/**
	 * Load audio data from file.
	 *
	 * \param f file url
	 */
	bool load(const std::string& f);
	
	/**
	 * Play audio.
	 */
	void play();
	
	/**
	 * Stop audio. The position will not been reseted.
	 */
	void stop();
	
	~audio();
	
	audio(const audio&) = delete;
	
	audio& operator=(const audio&) = delete;
	
private:
	SDL_AudioSpec spec;
	SDL_AudioDeviceID device;
	uint8_t* buffer = nullptr;
	uint32_t length = 0;
};
