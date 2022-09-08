/**
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

namespace Ink {

class Audio {
public:
	bool loop = false;    /**< whether the audio play consecutively */
	float volume = 1;     /**< the current volume level, range is 0 to 1 */
	
	/**
	 * Initialize SDL Audio system.
	 */
	static void init();
	
	/**
	 * Create a new Audio with its volume.
	 *
	 * \param v volume level
	 */
	Audio(float v = 1);
	
	/**
	 * Load a WAV file into audio.
	 *
	 * \param f the URL to the file
	 */
	bool load(const std::string& f);
	
	/**
	 * Unload audio and release recourse.
	 */
	void unload() const;
	
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
	 * Start playback from the position.
	 */
	void play();
	
	/**
	 * Pause playback and record the position.
	 */
	void pause();
	
	/**
	 * Stop playback and set the position to 0.
	 */
	void stop();
	
private:
	int position = 0;
	
	uint32_t length = 0;
	
	uint8_t* buffer = nullptr;
	
	SDL_AudioSpec spec;
	
	SDL_AudioDeviceID device;
	
	Audio(const Audio&) = delete;
	
	Audio& operator=(const Audio&) = delete;
};

}
