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

#include "Ray.h"

namespace ink {

Ray::Ray(const Vec3& o, const Vec3& d) : origin(o), direction(d) {}

float Ray::intersect_box(const Vec3& l, const Vec3& u) const {
	Vec3 inv = 1 / direction;
	Vec3 tmin = (l - origin) * inv;
	Vec3 tmax = (u - origin) * inv;
	if (inv.x < 0) std::swap(tmin.x, tmax.x);
	if (inv.y < 0) std::swap(tmin.y, tmax.y);
	if (inv.z < 0) std::swap(tmin.z, tmax.z);
	float tmin_v = tmin.x;
	float tmax_v = tmax.x;
	if (tmin_v > tmax.y || tmin.y > tmax_v) return -1;
	if (tmin.y > tmin_v || std::isnan(tmin_v)) tmin_v = tmin.y;
	if (tmax.y < tmax_v || std::isnan(tmax_v)) tmax_v = tmax.y;
	if (tmin_v > tmax.z || tmin.z > tmax_v) return -1;
	if (tmin.z > tmin_v || std::isnan(tmin_v)) tmin_v = tmin.z;
	if (tmax.z < tmax_v || std::isnan(tmax_v)) tmax_v = tmax.z;
	return tmax_v < 0 ? -1 : tmin_v >= 0 ? tmin_v : tmax_v;
}

float Ray::intersect_plane(const Vec3& n, float d) const {
	float cos_nd = n.dot(direction);
	if (cos_nd == 0) {
		return n.dot(origin) - d == 0 ? 0 : -1;
	}
	float t = -(n.dot(origin) - d) / cos_nd;
	return t < 0 ? -1 : t;
}

float Ray::intersect_sphere(const Vec3& c, float r) const {
	Vec3 ray_to_sphere = c - origin;
	float t = ray_to_sphere.dot(direction);
	float dt_2 = r * r - ray_to_sphere.dot(ray_to_sphere) + t * t;
	if (dt_2 < 0) return -1;
	float dt = sqrtf(dt_2);
	return t + dt < 0 ? -1 : t - dt < 0 ? t + dt : t - dt;
}

float Ray::intersect_triangle(const Vec3& a, const Vec3& b, const Vec3& c) const {
	Vec3 ab = b - a;
	Vec3 ac = c - a;
	Vec3 ao = origin - a;
	Vec3 p = direction.cross(ac);
	Vec3 q = ao.cross(ab);
	float d = ab.dot(p);
	if (d == 0) return -1;
	float inv = 1 / d;
	float u = ao.dot(p) * inv;
	float v = direction.dot(q) * inv;
	float t = ac.dot(q) * inv;
	return t < 0 || u < 0 || u > 1 || v < 0 || u + v > 1 ? -1 : t;
}

}
