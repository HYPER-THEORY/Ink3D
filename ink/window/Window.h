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
#include <vector>
#include <SDL2/SDL.h>

namespace Ink {

class Window {
public:
	/**
	 * Initialize Window with parameters.
	 *
	 * \param t the title of window
	 * \param x the x-coordinate of the window's position
	 * \param y the y-coordinate of the window's position
	 * \param w the width of window
	 * \param h the height of window
	 * \param dpi whether to enable high-dpi mode
	 */
	static void init(const std::string& t, int x, int y, int w, int h, bool dpi = false);
	
	/**
	 * Initialize canvas after the window is initialized.
	 */
	static void init_canvas();
	
	/**
	 * Initialize OpenGL after the window is initialized.
	 *
	 * \param v vertical sync mode
	 * \param d the size of depth buffer
	 * \param s the size of stencil buffer
	 * \param m the number of samples in MSAA
	 * \param a whether to enable hardware acceleration
	 */
	static void init_opengl(int v = 1, int d = 24, int s = 8, int m = 0, bool a = true);
	
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
	 * Returns the size of the window.
	 */
	static std::pair<int, int> get_size();
	
	/**
	 * Returns the coordinates of the cursor's position.
	 */
	static std::pair<int, int> get_cursor_position();
	
	/**
	 * Returns the time scene the window is created.
	 */
	static unsigned int get_time();
	
	/**
	 * Returns the time between the current and previous frame.
	 */
	static unsigned int get_deltatime();
	
	/**
	 * Returns the canvas displayed in the window.
	 */
	static uint32_t* get_canvas();
	
	/**
	 * Returns the title of the window.
	 */
	static std::string get_title();
	
	/**
	 * Sets the title of the window.
	 */
	static void set_title(const std::string& t);
	
	/**
	 * Returns the coordinates of the window's position.
	 */
	static std::pair<int, int> get_position();
	
	/**
	 * Sets the coordinate of the window's position.
	 *
	 * \param x the x-coordinate of position
	 * \param y the t-coordinate of position
	 */
	static void set_position(int x, int y);
	
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
};

}
