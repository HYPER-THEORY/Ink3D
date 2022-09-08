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

#include "../math/Maths.h"
#include "../camera/PerspCamera.h"
#include "../window/Window.h"

namespace Ink {

enum ViewerMode {
	VIEWER_WALK,
	VIEWER_FLY,
};

class Viewer {
public:
	int mode = VIEWER_FLY;        /**< movement mode */
	float speed = 1;              /**< the speed of movement */
	float sensitivity = .001f;    /**< the sensitivity of mouse */
	
	int key_up = SDLK_w;          /**< the key controls camera move forward */
	int key_down = SDLK_s;        /**< the key controls camera move back */
	int key_left = SDLK_a;        /**< the key controls camera move left */
	int key_right = SDLK_d;       /**< the key controls camera move right */
	
	/**
	 * Create a new Viewer.
	 *
	 * \param c camera
	 * \param s moving speed
	 */
	Viewer(const Camera& c = {}, float s = 1);
	
	/**
	 * Update the viewing camera.
	 *
	 * \param dt delta time
	 */
	void update(float dt);
	
	/**
	 * Return the viewing camera.
	 */
	const Camera& get_camera() const;
	
	/**
	 * Return the viewing camera.
	 */
	Camera& get_camera();
	
	/**
	 * Sets the viewing camera.
	 *
	 * \param c camera
	 */
	void set_camera(const Camera& c);
	
private:
	Camera camera;
	float axis_y = 0;
	float axis_z = 0;
};

}
