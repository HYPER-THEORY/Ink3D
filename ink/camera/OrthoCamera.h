/**
 * Copyright (C) 2021-2023 HYPERTHEORY
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

namespace ink {

class OrthoCamera : public Camera {
public:
	float left = 0;     /**< the left clipping plane */
	float right = 0;    /**< the right clipping plane */
	float lower = 0;    /**< the lower clipping plane */
	float upper = 0;    /**< the upper clipping plane */
	
	/**
	 * Creates a new OrthoCamera object.
	 */
	OrthoCamera() = default;
	
	/**
	 * Creates a new OrthoCamera and initializes it with left, right, lower,
	 * upper, near, far clipping planes.
	 *
	 * \param le the left clipping plane
	 * \param ri the right clipping plane
	 * \param lo the lower clipping plane
	 * \param up the upper clipping plane
	 * \param n the nearer clipping plane
	 * \param f the farther clipping plane
	 */
	OrthoCamera(float le, float ri, float lo, float up, float n, float f);
	
	/**
	 * Sets the left, right, lower, upper, near, far clipping planes. Updates
	 * the projection matrix of the camera.
	 *
	 * \param le the left clipping plane
	 * \param ri the right clipping plane
	 * \param lo the lower clipping plane
	 * \param up the upper clipping plane
	 * \param n the nearer clipping plane
	 * \param f the farther clipping plane
	 */
	void set(float le, float ri, float lo, float up, float n, float f);
};

}
