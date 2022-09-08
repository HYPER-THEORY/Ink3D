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

#include <cmath>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include "../core/error.h"

class window {
public:
	static int x;					/* x position of window, center window by default */
	static int y;					/* y position of window, center window by default */
	static int width;				/* width of window */
	static int height;				/* height of window */
	static bool borderless;			/* no window decoration */
	static bool resizable;			/* window can be resized */
	static bool highdpi;			/* allow high-dpi mode */
	static bool lock_cursor;		/* lock the position of cursor*/
	static uint32_t interval;		/* interval time of update */
	static std::string title;		/* title of window */
	
	static bool opengl;				/* OpenGL mode */
	static bool msaa;				/* available if OpenGL is enabled, turn on multisample anti-aliasing */
	static int msaa_samples;		/* available if OpenGL is enabled, number of samples in msaa */
	static bool accelerated;		/* available if OpenGL is enabled, allow hardware acceleration */
	
	static int cursorx;				/* x-coordinate of cursor position */
	static int cursory;				/* x-coordinate of cursor position */
	static bool opening;			/* window is opening */
	static uint32_t time;			/* the time after window is created */
	static uint32_t deltatime;		/* the time between current frame and previous frame */
	static uint32_t* canvas;		/* available if OpenGL is disabled, canvas of window */
	
	/**
	 * Initialize window.
	 */
	static void init();
	
	/**
	 * Close window.
	 */
	static void close();
	
	/**
	 * Update window every interval time.
	 */
	static void update();
	
	/**
	 * Move cursor to specific position.
	 *
	 * \param x x-coordinate of cursor position
	 * \param y y-coordinate of cursor position
	 */
	static void move_cursor(int x, int y);
	
	/**
	 * Show system cursor.
	 */
	static void show_cursor();
	
	/**
	 * Hide system cursor.
	 */
	static void hide_cursor();
	
	/**
	 * Fullscreen window.
	 *
	 * \param d desktop mode
	 */
	static void fullscreen(bool d = true);
	
	/**
	 * Exit fullscreen.
	 */
	static void exit_fullscreen();
	
	/**
	 * Maximize window.
	 */
	static void maximize();
	
	/**
	 * Determines whether the specified key is down.
	 *
	 * \param k key code
	 */
	static bool is_down(unsigned int k);
	
	/**
	 * Determines whether the specified key is pressed.
	 *
	 * \param k key code
	 */
	static bool is_pressed(unsigned int k);
	
	/**
	 * Determines whether the specified key is released.
	 *
	 * \param k key code
	 */
	static bool is_released(unsigned int k);
	
private:
	static SDL_Window* sdl_window;
	static SDL_Surface* surface;
	static SDL_GLContext context;
	static bool keydown[512];
	static bool keypressed[512];
	static bool keyreleased[512];
	static std::vector<uint32_t> canvas_vector;
};
