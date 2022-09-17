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

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "gl.h"
#include "media.h"

void conf();
void load();
void update(float dt);
void quit();

inline int width = 960;
inline int height = 540;
inline int fps = 30;
inline bool opengl = false;
inline bool depth_test = true;
inline bool stencil_test = true;
inline bool hide_cursor = false;
inline bool lock_cursor = false;
inline std::string title;
inline std::vector<gl::vec3> frame;
inline gl::vec3 background_color;

inline void run() {
	/* config setting */
	conf();
	/* initialize window */
	window::opengl = opengl;
	window::title = title;
	window::width = width;
	window::height = height;
	window::delay = 1000 / fps;
	window::init();
	if (hide_cursor) {
		window::hide_cursor();
	}
	if (lock_cursor) {
		window::lock_mouse = true;
		window::move_mouse(width / 2, height / 2);
	}
	if (!opengl) {
		frame = std::vector<gl::vec3>(width * height);
	}
	/* load once */
	load();
	/* main loop */
	while (window::opening) {
		if (opengl) {
			glClearColor(background_color.x, background_color.y, background_color.z, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			depth_test ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
			stencil_test ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
			update(window::deltatime * .001f);
		} else {
			std::fill(frame.begin(), frame.end(), background_color);
			update(window::deltatime * .001f);
			for (int p = width * height; p --> 0;) {
				gl::vec3& color = frame[p];
				window::canvas[p] = floorf(color.z * 0xff) +
					floorf(color.y * 0xff) * 0x100 +
					floorf(color.x * 0xff) * 0x10000;
			}
		}
		/* update window */
		window::update();
#ifdef DEBUG
		/* close window when pressing escape key */
		if (window::keydown[SDLK_ESCAPE]) window::close();
		/* output fps */
		std::cout << "FPS: " << 1000 / window::deltatime << '\n';
#endif
	}
	/* quit window */
	quit();
}
