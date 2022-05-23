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

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "../math/maths.h"
#include "../graphics/gpu.h"
#include "../window/window.h"

struct settings {
	int x = -1;						/* the x-coordinate of the window's position */
	int y = -1;						/* the y-coordinate of the window's position */
	int width = 960;				/* the window width */
	int height = 540;				/* the height window */
	bool highdpi = false;			/* enable high-dpi mode */
	bool borderless = false;		/* whether the window is borderless */
	bool resizable = false;			/* whether the window is resizable */
	int min_width = 0;				/* minimum window width if the window is resizable */
	int min_height = 0;				/* minimum window height if the window is resizable */
	bool fullscreen = false;		/* whether the window is fullscreen */
	bool opengl = true;				/* enable OpenGL mode */
	int depth = 24;					/* only used in OpenGL, the size of depth buffer */
	int stencil = 8;				/* only used in OpenGL, the size of stencil buffer */
	int msaa = 0;					/* only used in OpenGL, the number of samples in msaa */
	int fps = 30;					/* the maximum fps */
	bool hide_cursor = false;		/* whether to hide system cursor */
	bool lock_cursor = false;		/* whether to lock cursor to the center of window */
	std::string title;				/* the title of window */
	vec3 background_color;			/* the background color of window */
};

std::vector<vec3> frame;

void conf(settings& t);
void load();
void update(float dt);
void quit();

void init() {
	loader::init();
	cubemap::init();
	renderpass::init();
	copypass::init();
	blendpass::init();
	shadowpass::init();
	ssaopass::init();
	fxaapass::init();
	audio::init();
}

int run() {
	/* config setting */
	settings t;
	conf(t);
	/* initialize window */
	window::init(t.title, t.x, t.y, t.width, t.height, t.highdpi);
	if (t.opengl) {
		window::init_opengl(t.depth, t.stencil, t.msaa);
	} else {
		window::init_canvas();
		frame = std::vector<vec3>(t.width * t.height);
	}
	if (t.fullscreen) {
		window::fullscreen_desktop();
	}
	if (t.hide_cursor) {
		window::hide_cursor();
	}
	if (t.lock_cursor) {
		window::lock_cursor();
		window::move_cursor(t.width / 2, t.height / 2);
	}
	window::set_borderless(t.borderless);
	window::set_resizable(t.resizable);
	window::set_min_size(t.min_width, t.min_height);
	window::set_interval(1000 / t.fps);
	/* init libraries */
	init();
	/* load once */
	load();
	/* main loop */
	while (window::is_open()) {
		if (t.opengl) {
			gpu::set_clear_color(t.background_color);
			gpu::clear();
			update(window::get_deltatime() * .001f);
		} else {
			std::fill(frame.begin(), frame.end(), t.background_color);
			update(window::get_deltatime() * .001f);
			uint32_t* canvas = window::get_canvas();
			for (int p = t.width * t.height; p --> 0;) {
				vec3& color = frame[p];
				canvas[p] = floorf(color.z * 0xff) +
					floorf(color.y * 0xff) * 0x100 +
					floorf(color.x * 0xff) * 0x10000;
			}
		}
		/* update window */
		window::update();
#ifdef DEBUG
		/* print error from OpenGL */
		if (t.opengl) gpu::print_error();
		/* print error from program */
		std::cerr << get_error();
		clear_error();
		/* close window when pressing escape key */
		if (window::is_down(SDLK_ESCAPE)) window::close();
#endif
	}
	/* quit window */
	quit();
	return 0;
}

int main(int argc, char** argv) {
	return run();
}
