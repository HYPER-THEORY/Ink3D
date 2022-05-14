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
	/**
	 * Init audio.
	 */
	static void init();
	
	/**
	 * Create a new audio.
	 *
	 * \param v volume level
	 */
	audio(float v = 1);
	
	~audio();
	
	audio(const audio&) = delete;
	
	audio& operator=(const audio&) = delete;
	
	/**
	 * Returns the current volume level, range is [0, 1].
	 */
	float get_volume() const;
	
	/**
	 * Sets the volume level, range is [0, 1].
	 *
	 * \param v volume level
	 */
	void set_volume(float v);
	
	/**
	 * Determines whether the audio should play consecutively.
	 */
	bool get_loop() const;
	
	/**
	 * Sets whether the audio should play consecutively.
	 *
	 * \param l audio loop
	 */
	void set_loop(bool l);
	
	/**
	 * Returns the current length the audio has been playing.
	 */
	int get_position() const;
	
	/**
	 * Sets the position where the audio starts playback.
	 *
	 * \param p audio position
	 */
	void set_position(int p);
	
	/**
	 * Load a WAV file into audio.
	 *
	 * \param f the URL to the file
	 */
	bool load(const std::string& f);
	
	/**
	 * Start playback.
	 */
	void play();
	
	/**
	 * Pause playback.
	 */
	void pause();
	
	/**
	 * Stop playback.
	 */
	void stop();
	
private:
	float volume = 1;
	bool loop = false;
	int position = 0;
	
	SDL_AudioSpec spec;
	SDL_AudioDeviceID device;
	uint8_t* buffer = nullptr;
	uint32_t length = 0;
};
