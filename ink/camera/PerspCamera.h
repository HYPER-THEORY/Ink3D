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

#include "Camera.h"

namespace Ink {

class PerspCamera : public Camera {
public:
	float fov_y = 0;     /**< the field of view in y-axis */
	float aspect = 0;    /**< the aspect ratio of field of view */
	
	/**
	 * Creates a new PerspCamera object.
	 */
	explicit PerspCamera() = default;
	
	/**
	 * Creates a new PerspCamera and initializes it with FOV, aspect, near and
	 * far.
	 *
	 * \param fov the field of view in y-axis
	 * \param a the aspect ratio of field of view
	 * \param n distance to the nearer clipping plane
	 * \param f distance to the farther clipping plane
	 */
	explicit PerspCamera(float fov, float a, float n, float f);
	
	/**
	 * Sets the FOV, aspect, near and far. Updates the projection matrix of
	 * camera.
	 *
	 * \param fov the field of view in y-axis
	 * \param a the aspect ratio of field of view
	 * \param n distance to the nearer clipping plane
	 * \param f distance to the farther clipping plane
	 */
	void set(float fov, float a, float n, float f);
};

}
