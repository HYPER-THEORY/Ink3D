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

#include "vec3.h"

class ray {
public:
	vec3 origin;
	vec3 direction;
	
	/**
	 * Create a new ray.
	 */
	ray() = default;
	
	/**
	 * Create a new ray.
	 *
	 * \param o origin of ray
	 * \param d direction of ray
	 */
	ray(const vec3& o, const vec3& d);
	
	/**
	 * Intersect with traingle.
	 *
	 * \param a triangle vertex A
	 * \param b triangle vertex B
	 * \param c triangle vertex C
	 * \param i intersect point
	 */
	bool intersect_triangle(const vec3& a, const vec3& b, const vec3& c, vec3* i = nullptr);
};
