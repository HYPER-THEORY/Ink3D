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

#include "Vector3.h"

namespace Ink {

class Ray {
public:
	Vec3 origin;		/**< the origin of ray */
	Vec3 direction;		/**< the direction of ray */
	
	/**
	 * Creates a new Ray object.
	 */
	explicit Ray() = default;
	
	/**
	 * Creates a new Ray object and initializes it with origin and direction.
	 *
	 * \param o the origin of ray
	 * \param d the direction of ray
	 */
	explicit Ray(const Vec3& o, const Vec3& d);
	
	/**
	 * Returns whether the ray will intersect with the triangle ABC.
	 *
	 * \param a vertex A of triangle
	 * \param b vertex B of triangle
	 * \param c vertex C of triangle
	 */
	bool intersect_triangle(const Vec3& a, const Vec3& b, const Vec3& c) const;
};

}
