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

#include <iostream>

#include "Everything.h"

struct Settings {
	int x = -1;                    /**< the x-coordinate of the window's position */
	int y = -1;                    /**< the y-coordinate of the window's position */
	int width = 960;               /**< the window width */
	int height = 540;              /**< the height window */
	bool highdpi = false;          /**< whether to enable high-dpi mode */
	bool borderless = false;       /**< whether the window is borderless */
	bool resizable = false;        /**< whether the window is resizable */
	int min_width = 0;             /**< minimum window width if the window is resizable */
	int min_height = 0;            /**< minimum window height if the window is resizable */
	bool fullscreen = false;       /**< whether the window is fullscreen */
	int vsync = 1;                 /**< the vertical sync mode in OpenGL */
	int depth = 24;                /**< the size of depth buffer in OpenGL */
	int stencil = 8;               /**< the size of stencil buffer in OpenGL */
	int msaa = 0;                  /**< the number of samples in MSAA in OpenGL */
	int fps = 30;                  /**< the maximum fps */
	bool show_cursor = true;       /**< whether to hide system cursor */
	bool lock_cursor = false;      /**< whether to lock cursor to the center of window */
	std::string title;             /**< the title of window */
	Ink::Vec3 background_color;    /**< the background color of window */
};

void conf(Settings& t);
void load();
void update(float dt);
void quit();

int run() {
	Settings t; /* create settings */
	conf(t); /* config settings */
	
	/* initialize window with parameters */
	Ink::Window::init(t.title, t.x, t.y, t.width, t.height, t.highdpi);
	Ink::Window::init_opengl(t.vsync, t.depth, t.stencil, t.msaa);
	Ink::Window::set_fullscreen(t.fullscreen);
	Ink::Window::set_borderless(t.borderless);
	Ink::Window::set_resizable(t.resizable);
	Ink::Window::set_min_size(t.min_width, t.min_height);
	Ink::Window::set_interval(1000 / t.fps);
	Ink::Window::set_cursor_visible(t.show_cursor);
	Ink::Window::set_cursor_locked(t.lock_cursor);
	if (t.lock_cursor) Ink::Window::set_cursor_position(t.width / 2, t.height / 2);
	
	/* print error from Ink3D */
	Ink::Error::set_callback([](const std::string& s) -> void {
		std::cerr << Ink::Date::get_local().format() << ' ' << s << '\n';
	});
	
	load(); /* load once */
	
	/* main loop */
	uint32_t time = 0;
	while (Ink::Window::is_open()) {
		
		/* clear default frame buffer */
		Ink::Gpu::FrameBuffer::activate(nullptr);
		Ink::Gpu::set_clear_color(t.background_color, 0);
		Ink::Gpu::clear();
		
		/* update window with delta time */
		uint32_t delta_time = Ink::Window::get_time() - time;
		time = Ink::Window::get_time();
		update(delta_time * 0.001f);
		Ink::Window::update();
		
		/* print error from OpenGL */
		std::string error = Ink::Gpu::get_error();
		if (!error.empty()) std::cerr << error;
		
#ifdef DEBUG
		/* close window when pressing escape key */
		if (Ink::Window::is_down(SDLK_ESCAPE)) {
			Ink::Window::close();
		}
#endif
	}
	
	quit(); /* quit window */
	
	return 0;
}

int main(int argc, char** argv) {
	return run();
}
