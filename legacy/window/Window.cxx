/**
 * Copyright (C) 2021-2022 Hypertheory
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

#include "../../libs/glad/glad.h"
#include "../../ink/core/Error.h"

namespace Ink::Legacy {

void Window::init(const std::string& t, int x, int y, int w, int h, bool opengl, bool dpi) {
	/* whether to center the window */
	if (x == -1) x = SDL_WINDOWPOS_CENTERED;
	if (y == -1) y = SDL_WINDOWPOS_CENTERED;
	
	/* initialize keydown with false */
	std::fill_n(keydown, 512, false);
	
	/* check whether to enable OpenGL */
	Window::opengl = opengl;
	
	/* create a SDL window */
	SDL_Init(SDL_INIT_VIDEO);
	uint32_t flags = SDL_WINDOW_SHOWN;
	flags |= SDL_WINDOW_OPENGL * opengl;
	flags |= SDL_WINDOW_ALLOW_HIGHDPI * dpi;
	sdl_window = SDL_CreateWindow(t.c_str(), x, y, w, h, flags);
}

void Window::init_canvas() {
	surface = SDL_GetWindowSurface(sdl_window);
	canvas = static_cast<uint32_t*>(surface->pixels);
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
	opengl ? SDL_GL_DeleteContext(context) : SDL_FreeSurface(surface);
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
		if (event.type == SDL_QUIT) {
			close();
			return;
		} else if (event.type == SDL_KEYDOWN) {
			if (keycode < 512) {
				keypressed[keycode] = !keydown[keycode];
				keydown[keycode] = true;
			}
		} else if (event.type == SDL_KEYUP) {
			if (keycode < 512) {
				keyreleased[keycode] = true;
				keydown[keycode] = false;
			}
		} else if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (event.button.button == SDL_BUTTON_LEFT) {
				keypressed[1] = true;
				keydown[1] = true;
			} else if (event.button.button == SDL_BUTTON_RIGHT) {
				keypressed[2] = true;
				keydown[2] = true;
			}
		} else if (event.type == SDL_MOUSEBUTTONUP) {
			if (event.button.button == SDL_BUTTON_LEFT) {
				keypressed[1] = true;
				keydown[1] = false;
			} else if (event.button.button == SDL_BUTTON_RIGHT) {
				keypressed[2] = true;
				keydown[2] = false;
			}
		} else if (event.type == SDL_MOUSEMOTION) {
			cursor_x = event.motion.x;
			cursor_y = event.motion.y;
		}
	}
	
	/* lock cursor to the center of window */
	if (cursor_locked && SDL_GetKeyboardFocus()) {
		int dimensions[2];
		SDL_GetWindowSize(sdl_window, &dimensions[0], &dimensions[1]);
		SDL_WarpMouseInWindow(sdl_window, dimensions[0] / 2, dimensions[1] / 2);
	}
	
	/* refresh window */
	if (opengl) {
		SDL_GL_SwapWindow(sdl_window);
	} else {
		SDL_UpdateWindowSurface(sdl_window);
	}
	
	/* calculate delta time */
	deltatime = SDL_GetTicks() - time;
	if (deltatime <= interval) SDL_Delay(interval - deltatime);
	time = SDL_GetTicks();
	deltatime = std::max(interval, deltatime);
}

bool Window::is_open() {
	return open;
}

std::pair<int, int> Window::get_size() {
	int dimensions[2];
	SDL_GetWindowSize(sdl_window, dimensions, dimensions + 1);
	return {dimensions[0], dimensions[1]};
}

std::pair<int, int> Window::get_cursor_position() {
	return {cursor_x, cursor_y};
}

unsigned int Window::get_time() {
	return SDL_GetTicks();
}

unsigned int Window::get_deltatime() {
	return deltatime;
}

uint32_t* Window::get_canvas() {
	return canvas;
}

std::string Window::get_title() {
	return SDL_GetWindowTitle(sdl_window);
}

void Window::set_title(const std::string& t) {
	SDL_SetWindowTitle(sdl_window, t.c_str());
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

void Window::lock_cursor() {
	cursor_locked = true;
}

void Window::unlock_cursor() {
	cursor_locked = false;
}

void Window::move_cursor(int x, int y) {
	SDL_WarpMouseInWindow(sdl_window, x, y);
	cursor_x = x;
	cursor_y = y;
}

void Window::show_cursor() {
	SDL_ShowCursor(SDL_ENABLE);
}

void Window::hide_cursor() {
	SDL_ShowCursor(SDL_DISABLE);
}

void Window::fullscreen() {
	SDL_SetWindowFullscreen(sdl_window, SDL_WINDOW_FULLSCREEN);
}

void Window::fullscreen_desktop() {
	SDL_SetWindowFullscreen(sdl_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
}

void Window::exit_fullscreen() {
	SDL_SetWindowFullscreen(sdl_window, 0);
}

void Window::maximize() {
	SDL_MaximizeWindow(sdl_window);
}

bool Window::is_down(unsigned int k) {
	return keydown[k];
}

bool Window::is_pressed(unsigned int k) {
	return keypressed[k];
}

bool Window::is_released(unsigned int k) {
	return keyreleased[k];
}

bool Window::opengl = false;
bool Window::open = true;

int Window::cursor_x = 0;
int Window::cursor_y = 0;
bool Window::cursor_locked = false;

uint32_t Window::interval = 0;
uint32_t Window::time = 0;
uint32_t Window::deltatime = 0;

uint32_t* Window::canvas = nullptr;

SDL_Window* Window::sdl_window = nullptr;
SDL_Surface* Window::surface = nullptr;
SDL_GLContext Window::context = nullptr;

bool Window::keydown[512];
bool Window::keypressed[512];
bool Window::keyreleased[512];

}
