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

#include <cmath>
#include "../core/math.h"
#include "../graphics/camera.h"
#include "../window/window.h"

enum viewer_mode {
	WALK_MODE,
	FLY_MODE,
};

class viewer {
public:
	int mode = FLY_MODE;			/* movement mode */
	float speed = 0;				/* moving speed */
	float sensitivity = .001f;		/* mouse sensitivity */
	
	float axisy = 0;				/* solid angle on y-axis */
	float axisz = 0;				/* solid angle on z-axis */
	
	camera view_camera;				/* viewing camera */
	
	/**
	 * Create a new viewer.
	 */
	viewer() = default;
	
	/**
	 * Create a new viewer.
	 *
	 * \param c camera
	 * \param m moving mode
	 * \param s moving speed
	 */
	viewer(const camera& c, int m = FLY_MODE, float s = .02f);
	
	/**
	 * Remapping keys of camera movement.
	 *
	 * \param u up key
	 * \param d down key
	 * \param l left key
	 * \param r right key
	 */
	void key_mapping(int u, int d, int l, int r);
	
	/**
	 * Update viewing camera.
	 */
	camera& update();
	
private:
	int key_up = SDLK_w;
	int key_down = SDLK_s;
	int key_left = SDLK_a;
	int key_right = SDLK_d;
};
