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
	bool highdpi = false;          /**< enable high-dpi mode */
	bool borderless = false;       /**< whether the window is borderless */
	bool resizable = false;        /**< whether the window is resizable */
	int min_width = 0;             /**< minimum window width if the window is resizable */
	int min_height = 0;            /**< minimum window height if the window is resizable */
	bool fullscreen = false;       /**< whether the window is fullscreen */
	bool opengl = true;            /**< enable OpenGL mode */
	int vsync = 1;                 /**< only used in OpenGL, vertical sync mode */
	int depth = 24;                /**< only used in OpenGL, the size of depth buffer */
	int stencil = 8;               /**< only used in OpenGL, the size of stencil buffer */
	int msaa = 0;                  /**< only used in OpenGL, the number of samples in MSAA */
	int fps = 30;                  /**< the maximum fps */
	bool hide_cursor = false;      /**< whether to hide system cursor */
	bool lock_cursor = false;      /**< whether to lock cursor to the center of window */
	std::string title;             /**< the title of window */
	Ink::Vec3 background_color;    /**< the background color of window */
};

std::vector<Ink::Vec4> frame;

void conf(Settings& t);
void load();
void update(float dt);
void quit();

int run() {
	Settings t;
	conf(t); /* config setting */
	
	/* initialize window */
	Ink::Window::init(t.title, t.x, t.y, t.width, t.height, t.highdpi);
	if (t.opengl) {
		Ink::Window::init_opengl(t.vsync, t.depth, t.stencil, t.msaa);
	} else {
		Ink::Window::init_canvas();
		frame.resize(t.width * t.height);
	}
	if (t.fullscreen) {
		Ink::Window::fullscreen_desktop();
	}
	if (t.hide_cursor) {
		Ink::Window::hide_cursor();
	}
	if (t.lock_cursor) {
		Ink::Window::lock_cursor();
		Ink::Window::move_cursor(t.width / 2, t.height / 2);
	}
	Ink::Window::set_borderless(t.borderless);
	Ink::Window::set_resizable(t.resizable);
	Ink::Window::set_min_size(t.min_width, t.min_height);
	Ink::Window::set_interval(1000 / t.fps);
	
	/* print error from Ink3D */
	Ink::set_error_callback([](const std::string& s) -> void {
		std::cerr << Ink::format_date(Ink::get_date()) << ' ' << s << '\n';
	});
	
	load(); /* load once */
	
	/* main loop */
	while (Ink::Window::is_open()) {
		
		/* update frame with OpenGL */
		if (t.opengl) {
			Ink::Gpu::set_clear_color(t.background_color);
			Ink::Gpu::clear();
			update(Ink::Window::get_deltatime() * 0.001f);
		}
		
		/* update frame without OpenGL */
		else {
			std::fill(frame.begin(), frame.end(), Ink::Vec4(t.background_color, 0));
			update(Ink::Window::get_deltatime() * 0.001f);
			uint32_t* canvas = Ink::Window::get_canvas();
			int pixel = t.width * t.height;
			while (pixel --> 0) {
				canvas[pixel] = roundf(frame[pixel].x * 0xFF) * 0x010000 +
					roundf(frame[pixel].y * 0xFF) * 0x000100 +
					roundf(frame[pixel].z * 0xFF) * 0x000001;
			}
		}
		
		/* update window */
		Ink::Window::update();
		
		/* print error from OpenGL */
		if (t.opengl) {
			std::string error = Ink::Gpu::get_error();
			if (!error.empty()) std::cerr << error;
		}
		
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
