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

#pragma once

#include <string>
#include <vector>
#include <SDL2/SDL.h>

namespace Ink {

class Window {
public:
	/**
	 * Initializes Window with title, positions, size and DPI.
	 *
	 * \param t the title of window
	 * \param x the x-coordinate of the window's position
	 * \param y the y-coordinate of the window's position
	 * \param w the width of window
	 * \param h the height of window
	 * \param d whether to enable high-dpi mode
	 */
	static void init(const std::string& t, int x, int y, int w, int h, bool d = false);
	
	/**
	 * Initializes OpenGL after the window is initialized.
	 *
	 * \param v vertical sync mode
	 * \param d the size of depth buffer
	 * \param s the size of stencil buffer
	 * \param m the number of samples in MSAA
	 * \param a whether to enable hardware acceleration
	 */
	static void init_opengl(int v = 1, int d = 24, int s = 8, int m = 0, bool a = true);
	
	/**
	 * Closes window and releases all resources.
	 */
	static void close();
	
	/**
	 * Handles the window's events and refreshes the display of window.
	 */
	static void update();
	
	/**
	 * Returns true if the window is opening.
	 */
	static bool is_open();
	
	/**
	 * Returns the number of milliseconds scene the window is initialized.
	 */
	static unsigned int get_time();
	
	/**
	 * Returns the title of the window.
	 */
	static std::string get_title();
	
	/**
	 * Sets the specified title of the window.
	 */
	static void set_title(const std::string& t);
	
	/**
	 * Returns the size of a window's client area.
	 */
	static std::pair<int, int> get_size();
	
	/**
	 * Returns the coordinates represents the position of the window.
	 */
	static std::pair<int, int> get_position();
	
	/**
	 * Sets the coordinates represents the position of the window.
	 *
	 * \param x the X coordinate of position
	 * \param y the Y coordinate of position
	 */
	static void set_position(int x, int y);
	
	/**
	 * Returns the interval time. If the time update function spends is less
	 * than interval time, the program will wait for the time difference.
	 */
	static uint32_t get_interval();
	
	/**
	 * Sets the specified interval time. If the time update function spends is
	 * less than interval time, the program will wait for the time difference.
	 *
	 * \param i interval time
	 */
	static void set_interval(uint32_t i);
	
	/**
	 * Sets the border state of the window.
	 *
	 * \param b borderless
	 */
	static void set_borderless(bool b);
	
	/**
	 * Sets the user-resizable state of the window.
	 *
	 * \param r resizable
	 */
	static void set_resizable(bool r);
	
	/**
	 * Sets the minimum size of the window's client area.
	 *
	 * \param w minimum width
	 * \param h minimum height
	 */
	static void set_min_size(int w, int h);
	
	/**
	 * Enters or exits fullscreen state. The fullscreen mode is desktop.
	 *
	 * \param f fullscreen state
	 */
	static void set_fullscreen(bool f);
	
	/**
	 * Makes a window as large as possible.
	 */
	static void maximize();
	
	/**
	 * Minimizes a window to an iconic representation.
	 */
	static void minimize();
	
	/**
	 * Returns the coordinates represents the position of cursor.
	 */
	static std::pair<int, int> get_cursor_position();
	
	/**
	 * Sets the coordinates represents the position of cursor.
	 *
	 * \param x the x coordinate of cursor
	 * \param y the y coordinate of cursor
	 */
	static void set_cursor_position(int x, int y);
	
	/**
	 * Sets the specified visibility of the cursor.
	 *
	 * \param v visibility
	 */
	static void set_cursor_visible(bool v);
	
	/**
	 * Sets the lock state of the cursor. The cursor will be fixed at the center
	 * of window if lock is enabled.
	 *
	 * \param l lock
	 */
	static void set_cursor_locked(bool l);
	
	/**
	 * Returns true if the specified key is down. The key code of left mouse
	 * button is 1 and right mouse button is 2.
	 *
	 * \param k key code
	 */
	static bool is_down(unsigned int k);
	
	/**
	 * Returns true if the specified key is pressed. The key code of left mouse
	 * button is 1 and right mouse button is 2.
	 *
	 * \param k key code
	 */
	static bool is_pressed(unsigned int k);
	
	/**
	 * Returns true if the specified key is released. The key code of left mouse
	 * button is 1 and right mouse button is 2.
	 *
	 * \param k key code
	 */
	static bool is_released(unsigned int k);
	
private:
	static bool open;
	static int cursor_x;
	static int cursor_y;
	static bool cursor_locked;
	static uint32_t time;
	static uint32_t interval;
	
	static SDL_Window* sdl_window;
	static SDL_GLContext context;
	
	static bool keydown[512];
	static bool keypressed[512];
	static bool keyreleased[512];
};

}
