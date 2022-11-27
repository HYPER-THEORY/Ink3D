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
	/**
	 * Initializes SDL audio system before using this class.
	 */
	static void init();
	
	/**
	 * Creates a new Audio object and loads audio data from the specified WAVE
	 * file into the audio.
	 *
	 * \param p the path to the file
	 */
	explicit Audio(const std::string& p);
	
	/**
	 * Deletes this Audio object and releases resources.
	 */
	~Audio();
	
	/**
	 * The copy constructor is deleted.
	 */
	Audio(const Audio&) = delete;
	
	/**
	 * The copy assignment operator is deleted.
	 */
	Audio& operator=(const Audio&) = delete;
	
	/**
	 * Starts the audio playback.
	 */
	void play() const;
	
	/**
	 * Pauses the audio playback.
	 */
	void pause() const;
	
	/**
	 * Stops the audio playback. The position of the audio will be set to 0.
	 */
	void stop();
	
	/**
	 * Returns the duration of the audio in seconds.
	 */
	float get_duration() const;
	
	/**
	 * Returns true if the audio must be replayed when the end of the audio is
	 * reached.
	 */
	bool get_loop() const;
	
	/**
	 * Determines whether the audio must be replayed when the end of the audio
	 * is reached. Default is false.
	 *
	 * \param l looping
	 */
	void set_loop(bool l);
	
	/**
	 * Returns the volume of the audio. Range is 0 to 1.
	 */
	float get_volume() const;
	
	/**
	 * Sets the specified volume of the audio. Range is 0 to 1. Default is 1.
	 *
	 * \param v volume
	 */
	void set_volume(float v);
	
	/**
	 * Returns the position indicates in seconds the current point that is being
	 * played in the audio.
	 */
	float get_position() const;
	
	/**
	 * Sets the position indicates in seconds the current point that is being
	 * played in the audio.
	 *
	 * \param p position
	 */
	void set_position(float p);
	
private:
	bool loop = false;
	float volume = 1;
	float ratio = 0;
	uint32_t position = 0;
	uint32_t length = 0;
	uint8_t* buffer = nullptr;
	
	SDL_AudioSpec spec;
	SDL_AudioDeviceID device;
};

}
