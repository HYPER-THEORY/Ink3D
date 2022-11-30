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

#include "Matrix.h"

namespace Ink {

enum EulerOrder {
	EULER_XYZ,
	EULER_XZY,
	EULER_YXZ,
	EULER_YZX,
	EULER_ZXY,
	EULER_ZYX,
};

class Euler {
public:
	float x = 0;              /* the rotation angle of X axis */
	float y = 0;              /* the rotation angle of Y axis */
	float z = 0;              /* the rotation angle of Z axis */
	int order = EULER_XYZ;    /* the order of rotations */
	
	/**
	 * Creates a new Euler object.
	 */
	explicit Euler() = default;
	
	/**
	 * Creates a new Euler object and initializes it with rotations and order.
	 *
	 * \param x the rotation angle of X axis
	 * \param y the rotation angle of Y axis
	 * \param z the rotation angle of Z axis
	 * \param o the order of rotations
	 */
	explicit Euler(float x, float y, float z, int o = EULER_XYZ);
	
	/**
	 * Creates a new Euler object and initializes it with rotations and order.
	 *
	 * \param r the rotation vector
	 * \param o the order of rotations
	 */
	explicit Euler(Vec3 r, int o = EULER_XYZ);
	
	/**
	 * Transforms the Euler angles to rotation matrix.
	 */
	Mat3 to_rotation_matrix() const;
};

}
