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
#include "../core/math.h"
#include "../graphics/gpu.h"
#include "../window/window.h"

void conf();
void load();
void update(float dt);
void quit();

int width = 960;
int height = 540;
int fps = 30;
bool highdpi = false;
bool opengl = false;
bool hide_cursor = false;
bool lock_cursor = false;
bool msaa = false;
int msaa_samples = 0;
std::string title;
std::vector<vec3> frame;
vec3 background_color;

int run() {
	/* config setting */
	conf();
	/* initialize window */
	window::opengl = opengl;
	window::title = title;
	window::width = width;
	window::height = height;
	window::highdpi = highdpi;
	window::interval = 1000 / fps;
	window::msaa = msaa;
	window::msaa_samples = msaa_samples;
	window::init();
	if (hide_cursor) {
		window::hide_cursor();
	}
	if (lock_cursor) {
		window::lock_cursor = true;
		window::move_cursor(width / 2, height / 2);
	}
	if (!opengl) {
		frame = std::vector<vec3>(width * height);
	}
	/* load once */
	load();
	/* main loop */
	while (window::opening) {
		if (opengl) {
			gpu::clear_color(background_color);
			gpu::clear();
			update(window::deltatime * .001f);
		} else {
			std::fill(frame.begin(), frame.end(), background_color);
			update(window::deltatime * .001f);
			for (int p = width * height; p --> 0;) {
				vec3& color = frame[p];
				window::canvas[p] = floorf(color.z * 0xff) +
					floorf(color.y * 0xff) * 0x100 +
					floorf(color.x * 0xff) * 0x10000;
			}
		}
		/* update window */
		window::update();
#ifdef DEBUG
		/* print error from OpenGL */
		gpu::print_error();
		/* print error from program */
		std::cerr << get_error();
		clear_error();
		/* print fps */
		std::cout << "FPS: " << 1000 / window::deltatime << "\n";
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
