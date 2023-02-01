/**
 * Copyright (C) 2021-2023 Hypertheory
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

#include "../math/Vector.h"
#include "../math/Matrix.h"

namespace Ink {

class Camera {
public:
	float near = 0;     /**< distance to the nearer clipping plane */
	float far = 0;      /**< distance to the farther clipping plane */
	
	Vec3 position;      /**< the position of camera */
	Vec3 direction;     /**< the viewing direction of camera */
	Vec3 up;            /**< the view-up vector of camera */
	
	Mat4 viewing;       /**< the matrix of viewing transform */
	Mat4 projection;    /**< the matrix of projection transform */
	
	/**
	 * Creates a new Camera object.
	 */
	explicit Camera() = default;
	
	/**
	 * Returns true if this camera is perspective.
	 */
	bool is_perspective() const;
	
	/**
	 * Sets the position, viewing direction, view-up vector of camera. Updates
	 * the viewing matrix and projection matrix.
	 *
	 * \param p the position of camera
	 * \param d viewing direction (from object to camera)
	 * \param u view-up vector
	 */
	void lookat(const Vec3& p, const Vec3& d, const Vec3& u);
};

}
