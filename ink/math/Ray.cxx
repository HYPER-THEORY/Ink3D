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

#include "Ray.h"

namespace Ink {

Ray::Ray(const Vec3& o, const Vec3& d) : origin(o), direction(d) {}

float Ray::intersect_box(const Vec3& l, const Vec3& u) const {
	Vec3 inv_dir = 1 / direction;
	Vec3 tmin = (l - origin) * inv_dir;
	Vec3 tmax = (u - origin) * inv_dir;
	if (tmin.x > tmax.x) std::swap(tmin.x, tmax.x);
	if (tmin.y > tmax.y) std::swap(tmin.y, tmax.y);
	if (tmin.z > tmax.z) std::swap(tmin.z, tmax.z);
	float t0 = fmax(fmax(tmin.x, tmin.y), tmin.z);
	float t1 = fmin(fmin(tmax.x, tmax.y), tmax.z);
	return t0 > t1 ? -1 : t0 > 0 ? t0 : t1 > 0 ? t1 : -1;
}

float Ray::intersect_plane(const Vec3& n, float d) const {
	float t = n.dot(n * d - origin) / n.dot(direction);
	return t < 0 ? -1 : t;
}

float Ray::intersect_shpere(const Vec3& c, float r) const {
	Vec3 oc = c - origin;
	float t = direction.dot(oc);
	float d2 = oc.magnitude() - t * t;
	float r2 = r * r;
	if (d2 > r2) return -1;
	float dt = sqrtf(r2 - d2);
	float t0 = t - dt;
	float t1 = t + dt;
	return t1 < 0 ? -1 : t0 < 0 ? t1 : t0;
}

float Ray::intersect_triangle(const Vec3& a, const Vec3& b, const Vec3& c) const {
	Vec3 ab = b - a;
	Vec3 ac = c - a;
	Vec3 ao = origin - a;
	Vec3 p = direction.cross(ac);
	Vec3 q = ao.cross(ab);
	float inv = 1 / ab.dot(p);
	float u = direction.dot(q) * inv;
	float v = ao.dot(p) * inv;
	float t = ac.dot(q) * inv;
	return t < 0 || u < 0 || u > 1 || v < 0 || u + v > 1 ? -1 : t;
}

}
