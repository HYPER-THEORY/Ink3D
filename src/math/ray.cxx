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

#include "ray.h"

ray::ray(const vec3& o, const vec3& d) : origin(o), direction(d) {}

bool ray::intersect_triangle(const vec3& a, const vec3& b, const vec3& c, vec3* i) {
	vec3 ab = b - a;
	vec3 ac = c - a;
	vec3 ao = origin - a;
	vec3 p = direction ^ ac;
	vec3 q = ao ^ ab;
	float inverse = 1 / (ab * p);
	float u = direction * q * inverse;
	float v = ao * p * inverse;
	float t = ac * q * inverse;
	if (t < 0 || u < 0 || v < 0 || u + v > 1) return false;
	if (i) *i = {1 - u - v, v, u};
	return true;
}
