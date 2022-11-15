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

class OrthoCamera : public Camera {
public:
	float left = 0;     /**< the left vertical clipping plane */
	float right = 0;    /**< the right vertical clipping plane */
	float lower = 0;    /**< the lower horizontal clipping plane */
	float upper = 0;    /**< the upper horizontal clipping plane */
	
	/**
	 * Creates a new OrthoCamera.
	 */
	explicit OrthoCamera() = default;
	
	/**
	 * Creates a new OrthoCamera and initializes it with left, right, lower,
	 * upper, near, far.
	 *
	 * \param lv the left vertical clipping plane
	 * \param rv the right vertical clipping plane
	 * \param lh the lower horizontal clipping plane
	 * \param uh the upper horizontal clipping plane
	 * \param n distance to the nearer clipping plane
	 * \param f distance to the farther clipping plane
	 */
	explicit OrthoCamera(float lv, float rv, float lh, float uh, float n, float f);
	
	/**
	 * Sets the left, right, lower, upper, near, far. Updates the projection
	 * matrix of camera.
	 *
	 * \param lv the left vertical clipping plane
	 * \param rv the right vertical clipping plane
	 * \param lh the lower horizontal clipping plane
	 * \param uh the upper horizontal clipping plane
	 * \param n distance to the nearer clipping plane
	 * \param f distance to the farther clipping plane
	 */
	void set(float lv, float rv, float lh, float uh, float n, float f);
};

}
