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

#include "Ray.h"

namespace Ink {

Ray::Ray(const Vec3& o, const Vec3& d) : origin(o), direction(d) {}

bool Ray::intersect_triangle(const Vec3& a, const Vec3& b, const Vec3& c, Vec3* i) {
	Vec3 ab = b - a;
	Vec3 ac = c - a;
	Vec3 ao = origin - a;
	Vec3 p = direction.cross(ac);
	Vec3 q = ao.cross(ab);
	float inverse = 1 / ab.dot(p);
	float u = direction.dot(q) * inverse;
	float v = ao.dot(p) * inverse;
	float t = ac.dot(q) * inverse;
	if (t < 0 || u < 0 || v < 0 || u + v > 1) return false;
	if (i != nullptr) *i = {1 - u - v, v, u};
	return true;
}

}
