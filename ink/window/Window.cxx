/**
 * Copyright (C) 2021-2023 Hypertheory
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "Window.h"

#include <algorithm>

#include "../../libs/opengl/glad.h"
#include "../core/Error.h"

namespace Ink {

void Window::init(const std::string& t, int x, int y, int w, int h, bool d) {
	/* whether to center the window */
	if (x == -1) x = SDL_WINDOWPOS_CENTERED;
	if (y == -1) y = SDL_WINDOWPOS_CENTERED;
	
	/* initialize keydown with false */
	std::fill_n(keydown, 512, false);
	
	/* create a window in SDL system */
	SDL_Init(SDL_INIT_VIDEO);
	uint32_t flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
	if (d) flags |= SDL_WINDOW_ALLOW_HIGHDPI;
	sdl_window = SDL_CreateWindow(t.c_str(), x, y, w, h, flags);
}

void Window::init_opengl(int v, int d, int s, int m, bool a) {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, d);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, s);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, m != 0);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, m);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, a);
	context = SDL_GL_CreateContext(sdl_window);
	SDL_GL_SetSwapInterval(v);
	if (gladLoadGL() == 0) Error::set("Window: Failed to load OpenGL");
}

void Window::close() {
	open = false;
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(sdl_window);
	SDL_Quit();
}

void Window::update() {
	/* init key pressed & released array */
	std::fill_n(keypressed, 512, false);
	std::fill_n(keyreleased, 512, false);
	
	/* handle events */
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		int32_t keycode = event.key.keysym.sym;
		if (keycode > 127) keycode -= 1073741696;
		if (event.type == SDL_QUIT) {
			return close();
		} else if (event.type == SDL_KEYDOWN) {
			keypressed[keycode] = !keydown[keycode];
			keydown[keycode] = true;
		} else if (event.type == SDL_KEYUP) {
			keyreleased[keycode] = keydown[keycode];
			keydown[keycode] = false;
		} else if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (event.button.button == SDL_BUTTON_LEFT) {
				keypressed[1] = !keydown[1];
				keydown[1] = true;
			} else if (event.button.button == SDL_BUTTON_RIGHT) {
				keypressed[2] = !keydown[2];
				keydown[2] = true;
			}
		} else if (event.type == SDL_MOUSEBUTTONUP) {
			if (event.button.button == SDL_BUTTON_LEFT) {
				keyreleased[1] = keydown[1];
				keydown[1] = false;
			} else if (event.button.button == SDL_BUTTON_RIGHT) {
				keyreleased[2] = keydown[2];
				keydown[2] = false;
			}
		} else if (event.type == SDL_MOUSEMOTION) {
			cursor_x = event.motion.x;
			cursor_y = event.motion.y;
		}
	}
	
	/* lock cursor to the center of window */
	if (cursor_locked && SDL_GetKeyboardFocus()) {
		std::pair<int, int> size;
		SDL_GetWindowSize(sdl_window, &size.first, &size.second);
		SDL_WarpMouseInWindow(sdl_window, size.first / 2, size.second / 2);
	}
	
	/* refresh window */
	SDL_GL_SwapWindow(sdl_window);
	
	/* calculate delta time */
	uint32_t deltatime = SDL_GetTicks() - time;
	if (deltatime <= interval) SDL_Delay(interval - deltatime);
	time = SDL_GetTicks();
}

bool Window::is_open() {
	return open;
}

unsigned int Window::get_time() {
	return SDL_GetTicks();
}

std::string Window::get_title() {
	return SDL_GetWindowTitle(sdl_window);
}

void Window::set_title(const std::string& t) {
	SDL_SetWindowTitle(sdl_window, t.c_str());
}

std::pair<int, int> Window::get_size() {
	std::pair<int, int> size;
	SDL_GetWindowSize(sdl_window, &size.first, &size.second);
	return size;
}

std::pair<int, int> Window::get_position() {
	int position[2];
	SDL_GetWindowPosition(sdl_window, position, position + 1);
	return {position[0], position[1]};
}

void Window::set_position(int x, int y) {
	SDL_SetWindowPosition(sdl_window, x, y);
}

uint32_t Window::get_interval() {
	return interval;
}

void Window::set_interval(uint32_t i) {
	interval = i;
}

void Window::set_borderless(bool b) {
	SDL_SetWindowBordered(sdl_window, b ? SDL_FALSE : SDL_TRUE);
}

void Window::set_resizable(bool r) {
	SDL_SetWindowResizable(sdl_window, r ? SDL_TRUE : SDL_FALSE);
}

void Window::set_min_size(int w, int h) {
	SDL_SetWindowMinimumSize(sdl_window, w, h);
}

void Window::set_fullscreen(bool f) {
	SDL_SetWindowFullscreen(sdl_window, f ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

void Window::maximize() {
	SDL_MaximizeWindow(sdl_window);
}

void Window::minimize() {
	SDL_MinimizeWindow(sdl_window);
}

std::pair<int, int> Window::get_cursor_position() {
	return {cursor_x, cursor_y};
}

void Window::set_cursor_position(int x, int y) {
	SDL_WarpMouseInWindow(sdl_window, x, y);
	cursor_x = x;
	cursor_y = y;
}

void Window::set_cursor_visible(bool v) {
	SDL_ShowCursor(v ? SDL_ENABLE : SDL_DISABLE);
}

void Window::set_cursor_locked(bool l) {
	cursor_locked = l;
}

bool Window::is_down(unsigned int k) {
	if (k > 127) k -= 1073741696;
	return keydown[k];
}

bool Window::is_pressed(unsigned int k) {
	if (k > 127) k -= 1073741696;
	return keypressed[k];
}

bool Window::is_released(unsigned int k) {
	if (k > 127) k -= 1073741696;
	return keyreleased[k];
}

bool Window::open = true;
int Window::cursor_x = 0;
int Window::cursor_y = 0;
bool Window::cursor_locked = false;
uint32_t Window::time = 0;
uint32_t Window::interval = 0;

SDL_Window* Window::sdl_window = nullptr;
SDL_GLContext Window::context = nullptr;

bool Window::keydown[512];
bool Window::keypressed[512];
bool Window::keyreleased[512];

}
