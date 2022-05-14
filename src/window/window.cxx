/*
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

#include "window.h"
	
void window::init(const std::string& t, int x, int y, int w, int h, bool highdpi) {
	/* set up the window */
	create_window(t, x, y, w, h, false, highdpi);
	/* create a surface of the window */
	surface = SDL_GetWindowSurface(sdl_window);
	canvas = static_cast<uint32_t*>(surface->pixels);
}

void window::init_gl(const std::string& t, int x, int y, int w, int h, bool highdpi,
					 int depth, int stencil, int msaa, bool accelerated) {
	/* set up the window */
	create_window(t, x, y, w, h, true, highdpi);
	/* set the OpenGL attributes */
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, depth);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, stencil);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, msaa != 0);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, msaa);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, accelerated);
	/* create an OpenGL context */
	context = SDL_GL_CreateContext(sdl_window);
#ifndef __APPLE__
	gladLoadGL();
#endif
}

void window::close() {
	open = false;
	opengl ? SDL_GL_DeleteContext(context) : SDL_FreeSurface(surface);
	SDL_DestroyWindow(sdl_window);
	SDL_Quit();
}

void window::update() {
	/* init key pressed & released array */
	std::fill_n(keypressed, 512, false);
	std::fill_n(keyreleased, 512, false);
	/* handle events, low performance on Mac or Linux */
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
		SDL_WarpMouseInWindow(sdl_window, width / 2, height / 2);
	}
	/* refresh window */
	if (opengl) {
		SDL_GL_SwapWindow(sdl_window);
	} else {
		SDL_UpdateWindowSurface(sdl_window);
	}
	/* calcuate delta time */
	deltatime = SDL_GetTicks() - time;
	if (deltatime <= interval) SDL_Delay(interval - deltatime);
	time = SDL_GetTicks();
	deltatime = std::max(interval, deltatime);
}

bool window::is_open() {
	return open;
}

int window::get_width() {
	return width;
}

int window::get_height() {
	return height;
}

int window::get_cursor_x() {
	return cursor_x;
}

int window::get_cursor_y() {
	return cursor_y;
}

int window::get_time() {
	return time;
}

int window::get_deltatime() {
	return deltatime;
}

uint32_t* window::get_canvas() {
	return canvas;
}

uint32_t window::get_interval() {
	return interval;
}

void window::set_interval(uint32_t i) {
	interval = i;
}

void window::set_borderless(bool b) {
	SDL_SetWindowBordered(sdl_window, b ? SDL_FALSE : SDL_TRUE);
}

void window::set_resizable(bool r) {
	SDL_SetWindowResizable(sdl_window, r ? SDL_TRUE : SDL_FALSE);
}

void window::set_min_size(int w, int h) {
	SDL_SetWindowMinimumSize(sdl_window, w, h);
}

void window::lock_cursor() {
	cursor_locked = true;
}

void window::unlock_cursor() {
	cursor_locked = false;
}

void window::move_cursor(int x, int y) {
	SDL_WarpMouseInWindow(sdl_window, x, y);
	cursor_x = x;
	cursor_y = y;
}

void window::show_cursor() {
	SDL_ShowCursor(SDL_ENABLE);
}

void window::hide_cursor() {
	SDL_ShowCursor(SDL_DISABLE);
}

void window::fullscreen() {
	SDL_SetWindowFullscreen(sdl_window, SDL_WINDOW_FULLSCREEN);
}

void window::fullscreen_desktop() {
	SDL_SetWindowFullscreen(sdl_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
}

void window::exit_fullscreen() {
	SDL_SetWindowFullscreen(sdl_window, 0);
}

void window::maximize() {
	SDL_MaximizeWindow(sdl_window);
}

bool window::is_down(unsigned int k) {
	if (k < 0 || k >= 512) return !set_error("[Window Error] Illegal key code");
	return keydown[k];
}

bool window::is_pressed(unsigned int k) {
	if (k < 0 || k >= 512) return !set_error("[Window Error] Illegal key code");
	return keypressed[k];
}

bool window::is_released(unsigned int k) {
	if (k < 0 || k >= 512) return !set_error("[Window Error] Illegal key code");
	return keyreleased[k];
}

//void window::init_setup(int w, int h, bool o) {
//	width = w;
//	height = h;
//	opengl = o;
//	open = true;
//	SDL_Init(SDL_INIT_VIDEO);
//	std::fill_n(keydown, 128, false);
//}

void window::create_window(const std::string& t, int x, int y, int w, int h, bool o, bool highdpi) {
	width = w;
	height = h;
	opengl = o;
	open = true;
	std::fill_n(keydown, 128, false);
	SDL_Init(SDL_INIT_VIDEO);
	uint32_t flags = 0;
	flags |= highdpi * SDL_WINDOW_ALLOW_HIGHDPI;
	flags |= o ? SDL_WINDOW_OPENGL : SDL_WINDOW_SHOWN;
	sdl_window = SDL_CreateWindow(t.c_str(), x, y, w, h, flags);
}

int window::width = 0;
int window::height = 0;
bool window::opengl = false;
bool window::open = true;

int window::cursor_x = 0;
int window::cursor_y = 0;
bool window::cursor_locked = false;

uint32_t window::interval = 0;
uint32_t window::time = 0;
uint32_t window::deltatime = 0;

uint32_t* window::canvas = nullptr;

SDL_Window* window::sdl_window = nullptr;
SDL_Surface* window::surface = nullptr;
SDL_GLContext window::context = nullptr;

bool window::keydown[512];
bool window::keypressed[512];
bool window::keyreleased[512];
