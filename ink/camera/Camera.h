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

#include "../math/Vector.h"
#include "../math/Matrix.h"

namespace ink {

class Camera {
public:
	float near = 0;                 /**< the nearer clipping plane */
	float far = 0;                  /**< the farther clipping plane */
	
	Vec3 position = {0, 0, 0};      /**< the position of the camera */
	Vec3 direction = {0, 0, -1};    /**< the viewing direction of the camera */
	Vec3 up = {0, 1, 0};            /**< the view-up vector of the camera */
	
	Mat4 viewing;                   /**< the matrix of viewing transform */
	Mat4 projection;                /**< the matrix of projection transform */
	
	/**
	 * Creates a new Camera object.
	 */
	Camera() = default;
	
	/**
	 * Returns true if this camera is perspective.
	 */
	bool is_perspective() const;
	
	/**
	 * Sets the position, viewing direction, view-up vector of the camera.
	 * Updates the viewing matrix and the projection matrix.
	 *
	 * \param p position of the camera
	 * \param d viewing direction (from target to the camera)
	 * \param u view-up vector
	 */
	void lookat(const Vec3& p, const Vec3& d, const Vec3& u);
};

}
