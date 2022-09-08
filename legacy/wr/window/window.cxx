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
	
void window::init() {
	opening = true;
	SDL_Init(SDL_INIT_VIDEO);
	uint32_t flags = 0;
	if (borderless) flags |= SDL_WINDOW_BORDERLESS;
	if (resizable) flags |= SDL_WINDOW_RESIZABLE;
	if (highdpi) flags |= SDL_WINDOW_ALLOW_HIGHDPI;
	if (opengl) {
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, msaa);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, msaa_samples);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, accelerated);
		flags |= SDL_WINDOW_OPENGL;
		sdl_window = SDL_CreateWindow(title.c_str(), x, y, width, height, flags);
		context = SDL_GL_CreateContext(sdl_window);
	} else {
		flags |= SDL_WINDOW_SHOWN;
		sdl_window = SDL_CreateWindow(title.c_str(), x, y, width, height, flags);
		surface = SDL_GetWindowSurface(sdl_window);
		canvas_vector = std::vector<uint32_t>(width * height);
		canvas = canvas_vector.data();
	}
	std::fill(keydown, keydown + 128, false);
}
	
void window::close() {
	opening = false;
	if (opengl) {
		SDL_GL_DeleteContext(context);
	} else {
		SDL_FreeSurface(surface);
	}
	SDL_DestroyWindow(sdl_window);
	SDL_Quit();
}
	
void window::update() {
	/* init key pressed & released array */
	std::fill(keypressed, keypressed + 512, false);
	std::fill(keyreleased, keyreleased + 512, false);
	/* deal with events, low performance on Mac or Linux */
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
			cursorx = event.motion.x;
			cursory = event.motion.y;
		}
	}
	/* lock cursor to the center of window */
	if (lock_cursor && SDL_GetKeyboardFocus()) {
		SDL_WarpMouseInWindow(sdl_window, width / 2, height / 2);
	}
	/* refresh window */
	if (opengl) {
		SDL_GL_SwapWindow(sdl_window);
	} else {
		std::copy(canvas_vector.begin(), canvas_vector.end(), static_cast<uint32_t*>(surface->pixels));
		SDL_UpdateWindowSurface(sdl_window);
	}
	/* calcuate delta time */
	deltatime = SDL_GetTicks() - time;
	if (deltatime <= interval) SDL_Delay(interval - deltatime);
	time = SDL_GetTicks();
	deltatime = std::max(interval, deltatime);
}
	
void window::move_cursor(int x, int y) {
	SDL_WarpMouseInWindow(sdl_window, x, y);
	cursorx = x;
	cursory = y;
}

void window::show_cursor() {
	SDL_ShowCursor(SDL_ENABLE);
}
	
void window::hide_cursor() {
	SDL_ShowCursor(SDL_DISABLE);
}
	
void window::fullscreen(bool d) {
	SDL_SetWindowFullscreen(sdl_window, d ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_FULLSCREEN);
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

int window::x = SDL_WINDOWPOS_CENTERED;
int window::y = SDL_WINDOWPOS_CENTERED;
int window::width = 0;
int window::height = 0;
bool window::borderless = false;
bool window::resizable = false;
bool window::highdpi = false;
bool window::lock_cursor = false;
uint32_t window::interval = 0;
std::string window::title;

bool window::opengl = false;
bool window::msaa = false;
int window::msaa_samples = 0;
bool window::accelerated = true;

int window::cursorx = 0;
int window::cursory = 0;
bool window::opening = true;
uint32_t window::time = 0;
uint32_t window::deltatime = 0;
uint32_t* window::canvas = nullptr;

SDL_Window* window::sdl_window = nullptr;
SDL_Surface* window::surface = nullptr;
SDL_GLContext window::context;
bool window::keydown[512];
bool window::keypressed[512];
bool window::keyreleased[512];
std::vector<uint32_t> window::canvas_vector;
