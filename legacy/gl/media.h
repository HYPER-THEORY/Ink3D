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

#pragma once

#include <cmath>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include "gl.h"

class window {
public:
	static int x;
	static int y;
	static int width;
	static int height;
	static int mousex;
	static int mousey;
	static bool opening;
	static bool opengl;
	static bool borderless;
	static bool resizable;
	static bool highdpi;
	static bool lock_mouse;
	static bool keydown[512];
	static bool keypressed[512];
	static bool keyreleased[512];
	static uint32_t delay;
	static uint32_t time;
	static uint32_t deltatime;
	static std::string title;
	static std::vector<uint32_t> canvas;
	
	static void init() {
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
			SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
			flags |= SDL_WINDOW_OPENGL;
			sdl_window = SDL_CreateWindow(title.c_str(), x, y, width, height, flags);
			context = SDL_GL_CreateContext(sdl_window);
		} else {
			flags |= SDL_WINDOW_SHOWN;
			sdl_window = SDL_CreateWindow(title.c_str(), x, y, width, height, flags);
			surface = SDL_GetWindowSurface(sdl_window);
			canvas = std::vector<uint32_t>(width * height);
		}
		std::fill(keydown, keydown + 128, false);
	}
	
	static void close() {
		opening = false;
		if (opengl) {
			SDL_GL_DeleteContext(context);
		} else {
			SDL_FreeSurface(surface);
		}
		SDL_DestroyWindow(sdl_window);
		SDL_Quit();
	}
	
	static void update() {
		/* init key pressed & released array */
		std::fill(keypressed, keypressed + 512, false);
		std::fill(keyreleased, keyreleased + 512, false);
		/* deal with events, low performance on Mac or Linux */
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			int32_t keycode = event.key.keysym.sym;
			switch (event.type) {
				case SDL_QUIT:
					close();
					return;
				case SDL_KEYDOWN:
					if (keycode < 512) {
						keypressed[keycode] = !keydown[keycode];
						keydown[keycode] = true;
					}
					break;
				case SDL_KEYUP:
					if (keycode < 512) {
						keyreleased[keycode] = true;
						keydown[keycode] = false;
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_LEFT) {
						keypressed[1] = true;
						keydown[1] = true;
					} else if (event.button.button == SDL_BUTTON_RIGHT) {
						keypressed[2] = true;
						keydown[2] = true;
					}
					break;
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT) {
						keypressed[1] = true;
						keydown[1] = false;
					} else if (event.button.button == SDL_BUTTON_RIGHT) {
						keypressed[2] = true;
						keydown[2] = false;
					}
					break;
				case SDL_MOUSEMOTION:
					mousex = event.motion.x;
					mousey = event.motion.y;
					break;
				default:
					break;
			}
		}
		/* lock mouse to the center of window */
		if (lock_mouse && SDL_GetKeyboardFocus()) {
			/* if use mousemove here, program can not receive mouse movement */
			SDL_WarpMouseInWindow(sdl_window, width / 2, height / 2);
		}
		/* refresh window */
		if (opengl) {
			SDL_GL_SwapWindow(sdl_window);
		} else {
			copy(canvas.begin(), canvas.end(), static_cast<uint32_t*>(surface->pixels));
			SDL_UpdateWindowSurface(sdl_window);
		}
		/* calcuate delta time */
		deltatime = SDL_GetTicks() - time;
		if (deltatime <= delay) SDL_Delay(delay - deltatime);
		time = SDL_GetTicks();
		deltatime = std::max(delay, deltatime);
	}
	
	static void move_mouse(int x, int y) {
		SDL_WarpMouseInWindow(sdl_window, x, y);
		mousex = x;
		mousey = y;
	}
	
	static void show_cursor() {
		SDL_ShowCursor(SDL_ENABLE);
	}
	
	static void hide_cursor() {
		SDL_ShowCursor(SDL_DISABLE);
	}
	
	static void fullscreen(bool d = true) {
		SDL_SetWindowFullscreen(sdl_window, d ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_FULLSCREEN);
	}
	
	static void exit_fullscreen() {
		SDL_SetWindowFullscreen(sdl_window, 0);
	}
	
	static void maximize() {
		SDL_MaximizeWindow(sdl_window);
	}
	
private:
	static SDL_Window* sdl_window;
	static SDL_Surface* surface;
	static SDL_GLContext context;
};

inline int window::x = SDL_WINDOWPOS_CENTERED;
inline int window::y = SDL_WINDOWPOS_CENTERED;
inline int window::width = 0;
inline int window::height = 0;
inline int window::mousex = 0;
inline int window::mousey = 0;
inline bool window::opening = true;
inline bool window::opengl = false;
inline bool window::borderless = false;
inline bool window::resizable = false;
inline bool window::highdpi = false;
inline bool window::lock_mouse = false;
inline bool window::keydown[512];
inline bool window::keypressed[512];
inline bool window::keyreleased[512];
inline uint32_t window::delay = 0;
inline uint32_t window::time = 0;
inline uint32_t window::deltatime = 0;
inline std::string window::title;
inline std::vector<uint32_t> window::canvas;
inline SDL_Window* window::sdl_window = nullptr;
inline SDL_Surface* window::surface = nullptr;
inline SDL_GLContext window::context;

class image {
public:
	int width;
	int height;
	int bytes;
	std::vector<uint8_t> data;
	
	image() = default;
	
	static void init() {
		IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);
	}
	
	bool read(const std::string& f) {
		SDL_Surface* surface = IMG_Load(f.c_str());
		if (surface == nullptr) return false;
		width = surface->w;
		height = surface->h;
		bytes = surface->format->BytesPerPixel;
		data.resize(width * height * bytes);
		SDL_memcpy(data.data(), surface->pixels, data.size());
		SDL_FreeSurface(surface);
		return true;
	}
	
	static bool texture_read(const std::string& f, gl::texture& t) {
		SDL_Surface* surface = IMG_Load(f.c_str());
		if (surface == nullptr) return false;
		t.width = surface->w;
		t.height = surface->h;
		t.bytes = surface->format->BytesPerPixel;
		t.data.resize(t.width * t.height * t.bytes);
		SDL_memcpy(t.data.data(), surface->pixels, t.data.size());
		SDL_FreeSurface(surface);
		return true;
	}
	
	void slice(image& t, int x, int y, int w, int h) {
		t.width = w;
		t.height = h;
		t.bytes = bytes;
		t.data.resize(w * h * bytes);
		x = std::min(x, width - w);
		y = std::min(y, height - h);
		for (int i = 0; i < h; ++i) {
			SDL_memcpy(data.data() + x * bytes + (i + y) * width * bytes,
					   t.data.data() + i * w * bytes, w * bytes);
		}
	}
	
	static void texture_slice(const gl::texture& s, gl::texture& t, int x, int y, int w, int h) {
		t.width = w;
		t.height = h;
		t.bytes = s.bytes;
		t.data.resize(w * h * s.bytes);
		x = std::min(x, s.width - w);
		y = std::min(y, s.height - h);
		for (int i = 0; i < h; ++i) {
			std::copy_n(s.data.data() + x * s.bytes + (i + y) * s.width * s.bytes,
						w * s.bytes, t.data.data() + i * w * s.bytes);
		}
	}
	
	void flip_vertical() {
		int median = height / 2;
		for (int x = 0; x < width; ++x) {
			for (int y = 0; y < median; ++y) {
				for (int b = 0; b < bytes; ++b) {
					std::swap(data[(x + y * width) * bytes + b],
							  data[(x + (height - y - 1) * width) * bytes + b]);
				}
			}
		}
	}
	
	static void texture_flip_vertical(gl::texture& t) {
		int median = t.height / 2;
		for (int x = 0; x < t.width; ++x) {
			for (int y = 0; y < median; ++y) {
				for (int b = 0; b < t.bytes; ++b) {
					std::swap(t.data[(x + y * t.width) * t.bytes + b],
							  t.data[(x + (t.height - y - 1) * t.width) * t.bytes + b]);
				}
			}
		}
	}
	
	void flip_horizontal() {
		int median = width / 2;
		for (int x = 0; x < median; ++x) {
			for (int y = 0; y < height; ++y) {
				for (int b = 0; b < bytes; ++b) {
					std::swap(data[(x + y * width) * bytes + b],
							  data[(width - x - 1 + y * width) * bytes + b]);
				}
			}
		}
	}

	static void texture_flip_horizontal(gl::texture& t) {
		int median = t.width / 2;
		for (int x = 0; x < median; ++x) {
			for (int y = 0; y < t.height; ++y) {
				for (int b = 0; b < t.bytes; ++b) {
					std::swap(t.data[(x + y * t.width) * t.bytes + b],
							  t.data[(t.width - x - 1 + y * t.width) * t.bytes + b]);
				}
			}
		}
	}
	
	void to_bgr() {
		if (bytes < 3) return;
		for (int p = width * height; p --> 0;) {
			std::swap(data[p * bytes], data[p * bytes + 2]);
		}
	}
	
	static void texture_to_bgr(gl::texture& t) {
		if (t.bytes < 3) return;
		for (int p = t.width * t.height; p --> 0;) {
			std::swap(t.data[p * t.bytes], t.data[p * t.bytes + 2]);
		}
	}
};

class audio {
public:
	bool loop = false;
	float volume = 1;
	int position = 0;
	
	static void init() {
		SDL_Init(SDL_INIT_AUDIO);
	}
	
	audio(float v = 1, bool l = false) : volume(v), loop(l) {};
	
	bool read(const std::string& f) {
		if (SDL_LoadWAV(f.c_str(), &spec, &buffer, &length) == nullptr) return false;
		spec.userdata = this;
		spec.callback = [](void* ud, uint8_t* s, int l) -> void {
			SDL_memset(s, 0, l);
			audio* data = static_cast<audio*>(ud);
			if (data->position + l < data->length) {
				SDL_MixAudioFormat(s, data->buffer + data->position, data->spec.format, l,
					SDL_MIX_MAXVOLUME * data->volume);
				data->position += l;
			} else {
				SDL_MixAudioFormat(s, data->buffer + data->position, data->spec.format, data->length - data->position,
					SDL_MIX_MAXVOLUME * data->volume);
				data->position = 0;
				if (!data->loop) data->stop();
			}
		};
		device = SDL_OpenAudioDevice(nullptr, 0, &spec, nullptr, 0);
		return true;
	}
	
	~audio() {
		SDL_CloseAudioDevice(device);
		SDL_FreeWAV(buffer);
	}
	
	void play() {
		SDL_PauseAudioDevice(device, 0);
	}
	
	void stop() {
		SDL_PauseAudioDevice(device, 1);
	}
	
private:
	SDL_AudioSpec spec;
	SDL_AudioDeviceID device;
	uint8_t* buffer = nullptr;
	uint32_t length = 0;
};
