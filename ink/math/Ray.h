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

#include "Vector3.h"

namespace Ink {

class Ray {
public:
	Vec3 origin;		/**< the origin of ray */
	Vec3 direction;		/**< the direction of ray, must be normalized */
	
	/**
	 * Creates a new Ray object.
	 */
	explicit Ray() = default;
	
	/**
	 * Creates a new Ray object and initializes it with origin and direction.
	 *
	 * \param o the origin of ray
	 * \param d the direction of ray, must be normalized
	 */
	explicit Ray(const Vec3& o, const Vec3& d);
	
	/**
	 * Intersects the ray with the AABB box. Returns the distance to the
	 * intersection point if there is an intersection, returns -1 otherwise.
	 *
	 * \param l the lower boundary of box
	 * \param u the upper boundary of box
	 */
	float intersect_box(const Vec3& l, const Vec3& u) const;
	
	/**
	 * Intersects the ray with the plane. Returns the distance to the
	 * intersection point if there is an intersection, returns -1 otherwise.
	 *
	 * \param n the normal of plane, must be normalized
	 * \param d the distance from the origin to the plane
	 */
	float intersect_plane(const Vec3& n, float d) const;
	
	/**
	 * Intersects the ray with the sphere. Returns the distance to the
	 * intersection point if there is an intersection, returns -1 otherwise.
	 *
	 * \param c the center of sphere
	 * \param r the radius of sphere
	 */
	float intersect_shpere(const Vec3& c, float r) const;
	
	/**
	 * Intersects the ray with the triangle ABC. Returns the distance to the
	 * intersection point if there is an intersection, returns -1 otherwise.
	 *
	 * \param a the vertex A of triangle
	 * \param b the vertex B of triangle
	 * \param c the vertex C of triangle
	 */
	float intersect_triangle(const Vec3& a, const Vec3& b, const Vec3& c) const;
};

}
