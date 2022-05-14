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

#ifndef __APPLE__
#include "../glad/glad.h"
#endif

class window {
public:
	/**
	 * Init window without OpenGL, call get_canvas to.
	 *
	 * \param t the title of window
	 * \param x the x-coordinate of the window's position
	 * \param y the y-coordinate of the window's position
	 * \param w the width of window
	 * \param h the height of window
	 * \param highdpi enable high-dpi mode
	 */
	static void init(const std::string& t, int x, int y, int w, int h, bool highdpi = false);
	
	/**
	 * Init window with OpenGL.
	 *
	 * \param t the title of window
	 * \param x the x-coordinate of the window's position
	 * \param y the y-coordinate of the window's position
	 * \param w the width of window
	 * \param h the height of window
	 * \param highdpi enable high-dpi mode
	 * \param depth the size of depth buffer
	 * \param stencil the size of stencil buffer
	 * \param msaa the number of samples in msaa
	 * \param accelerated enable hardware acceleration
	 */
	static void init_gl(const std::string& t, int x, int y, int w, int h, bool highdpi = false,
						int depth = 24, int stencil = 8, int msaa = 0, bool accelerated = true);
	
	/**
	 * Close window.
	 */
	static void close();
	
	/**
	 * Update window in interval time.
	 */
	static void update();
	
	/**
	 * Determines whether the window is open.
	 */
	static bool is_open();
	
	/**
	 * Returns the window width.
	 */
	static int get_width();
	
	/**
	 * Returns the window height.
	 */
	static int get_height();
	
	/**
	 * Returns the x-coordinate of the cursor's position.
	 */
	static int get_cursor_x();
	
	/**
	 * Returns the y-coordinate of the cursor's position.
	 */
	static int get_cursor_y();
	
	/**
	 * Returns the time scene the window is created.
	 */
	static int get_time();
	
	/**
	 * Returns the time between the current frame and the previous frame.
	 */
	static int get_deltatime();
	
	/**
	 * Returns the canvas displayed in the window.
	 */
	static uint32_t* get_canvas();
	
	/**
	 * Returns the interval time of update.
	 */
	static uint32_t get_interval();
	
	/**
	 * Sets the interval time of update.
	 *
	 * \param i interval time
	 */
	static void set_interval(uint32_t i);
	
	/**
	 * Determines whether whether the window is borderless.
	 *
	 * \param b whether the window is borderless
	 */
	static void set_borderless(bool b);
	
	/**
	 * Determines whether whether the window is resizable.
	 *
	 * \param r whether the window is resizable
	 */
	static void set_resizable(bool r);
	
	/**
	 * Sets minimum window size if the window is resizable.
	 *
	 * \param w minimum window width
	 * \param h minimum window height
	 */
	static void set_min_size(int w, int h);
	
	/**
	 * Lock cursor to the center of window.
	 */
	static void lock_cursor();
	
	/**
	 * Unlock cursor.
	 */
	static void unlock_cursor();
	
	/**
	 * Move cursor to the specified position.
	 *
	 * \param x the x-coordinate of position
	 * \param y the y-coordinate of position
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
	 * Enter fullscreen.
	 */
	static void fullscreen();
	
	/**
	 * Enter fullscreen desktop.
	 */
	static void fullscreen_desktop();
	
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
	static int width;
	static int height;
	static bool opengl;
	static bool open;
	
	static int cursor_x;
	static int cursor_y;
	static bool cursor_locked;
	
	static uint32_t interval;
	static uint32_t time;
	static uint32_t deltatime;
	
	static uint32_t* canvas;
	
	static SDL_Window* sdl_window;
	static SDL_Surface* surface;
	static SDL_GLContext context;
	
	static bool keydown[512];
	static bool keypressed[512];
	static bool keyreleased[512];
	
	static void create_window(const std::string& t, int x, int y, int w, int h, bool o, bool highdpi);
};
