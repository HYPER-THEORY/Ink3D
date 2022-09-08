
/*
 * Copyright (C) 2021-2022 Hypertheory
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#ifdef __APPLE__
#define	GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#else
/* TODO */
#endif

#ifndef LIKELY
#define	LIKELY(x)	__builtin_expect((x), 1)
#endif

#ifndef UNLIKELY
#define	UNLIKELY(x)	__builtin_expect((x), 0)
#endif

namespace gl {

inline float random01f() {
	return rand() / (RAND_MAX + 1.f);
}

class vec2f {
public:
	float x = 0;
	float y = 0;
	
	vec2f() = default;
	
	vec2f(float x, float y) : x(x), y(y) {}
	
	vec2f(const vec2f& v) : x(v.x), y(v.y) {}
	
	vec2f operator-() const {
		return {-x, -y};
	}
	
	void operator+=(const vec2f& v) {
		x += v.x;
		y += v.y;
	}
	
	void operator-=(const vec2f& v) {
		x -= v.x;
		y -= v.y;
	}
	
	void operator*=(float v) {
		x *= v;
		y *= v;
	}
	
	void operator/=(float v) {
		x /= v;
		y /= v;
	}
	
	float magnitude() const {
		return sqrtf(x * x + y * y);
	}
	
	float distance(const vec2f& v) const {
		return sqrtf((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y));
	}
	
	vec2f normalize() const {
		float l = sqrtf(x * x + y * y);
		return {x / l, y / l};
	}
	
	vec2f rotate(float a) const {
		return {x * cosf(a) - y * sinf(a), x * sinf(a) + y * cosf(a)};
	}
	
	static vec2f random() {
		float angle = random01f() * M_PI * 2;
		return {cosf(angle), sinf(angle)};
	}
};

inline vec2f operator+(const vec2f& v1, const vec2f& v2) {
	return {v1.x + v2.x, v1.y + v2.y};
}

inline vec2f operator-(const vec2f& v1, const vec2f& v2) {
	return {v1.x - v2.x, v1.y - v2.y};
}

inline vec2f operator*(const vec2f& v1, float v2) {
	return {v1.x * v2, v1.y * v2};
}

inline vec2f operator*(float v1, const vec2f& v2) {
	return {v2.x * v1, v2.y * v1};
}

inline vec2f operator/(const vec2f& v1, float v2) {
	return {v1.x / v2, v1.y / v2};
}

inline float operator*(const vec2f& v1, const vec2f& v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

inline float operator^(const vec2f& v1, const vec2f& v2) {
	return v1.x * v2.y - v1.y * v2.x;
}

class vec3f {
public:
	float x = 0;
	float y = 0;
	float z = 0;
	
	vec3f() = default;
	
	vec3f(float x, float y, float z) : x(x), y(y), z(z) {}
	
	vec3f(const vec2f& v, float z) : x(v.x), y(v.y), z(z) {}
	
	vec3f(float x, const vec2f& v) : x(x), y(v.x), z(v.y) {}
	
	vec3f(const vec3f& v) : x(v.x), y(v.y), z(v.z) {}
	
	vec3f operator-() const {
		return {-x, -y, -z};
	}
	
	void operator+=(const vec3f& v) {
		x += v.x;
		y += v.y;
		z += v.z;
	}
	
	void operator-=(const vec3f& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
	}
	
	void operator*=(float v) {
		x *= v;
		y *= v;
		z *= v;
	}
	
	void operator/=(float v) {
		x /= v;
		y /= v;
		z /= v;
	}
	
	float magnitude() const {
		return sqrtf(x * x + y * y + z * z);
	}
	
	float distance(const vec3f& v) const {
		return sqrtf((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
	}
	
	vec3f normalize() const {
		float l = sqrtf(x * x + y * y + z * z);
		return {x / l, y / l, z / l};
	}
	
	vec3f rotate(const vec3f& v, float a) const {
		return {(cosf(a) + (1 - cosf(a)) * v.x * v.x) * x +
			((1 - cosf(a)) * v.x * v.y - sinf(a) * v.z) * y +
			((1 - cosf(a)) * v.x * v.z + sinf(a) * v.y) * z,
			((1 - cosf(a)) * v.x * v.y + sinf(a) * v.z) * x +
			(cosf(a) + (1 - cosf(a)) * v.y * v.y) * y +
			((1 - cosf(a)) * v.y * v.z - sinf(a) * v.x) * z,
			((1 - cosf(a)) * v.x * v.z - sinf(a) * v.y) * x +
			((1 - cosf(a)) * v.y * v.z + sinf(a) * v.x) * y +
			(cosf(a) + (1 - cosf(a)) * v.z * v.z) * z};
	}
	
	static vec3f random() {
		float angle1 = random01f() * M_PI * 2;
		float angle2 = random01f() * M_PI * 2;
		return {cosf(angle1) * cosf(angle2), sinf(angle2), sinf(angle1) * cosf(angle2)};
	}
};

inline vec3f operator+(const vec3f& v1, const vec3f& v2) {
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

inline vec3f operator-(const vec3f& v1, const vec3f& v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

inline vec3f operator*(const vec3f& v1, float v2) {
	return {v1.x * v2, v1.y * v2, v1.z * v2};
}

inline vec3f operator*(float v1, const vec3f& v2) {
	return {v2.x * v1, v2.y * v1, v2.z * v1};
}

inline vec3f operator/(const vec3f& v1, float v2) {
	return {v1.x / v2, v1.y / v2, v1.z / v2};
}

inline float operator*(const vec3f& v1, const vec3f& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline vec3f operator^(const vec3f& v1, const vec3f& v2) {
	return {v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x};
}

class vec4f {
public:
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 0;
	
	vec4f() = default;
	
	vec4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	
	vec4f(const vec2f& v, float z, float w) : x(v.x), y(v.y), z(z), w(w) {}
	
	vec4f(float x, const vec2f& v, float w) : x(x), y(v.x), z(v.y), w(w) {}
	
	vec4f(float x, float y, const vec2f& v) : x(x), y(y), z(v.x), w(v.y) {}
	
	vec4f(const vec2f& v1, const vec2f& v2) : x(v1.x), y(v1.y), z(v2.x), w(v2.y) {}
	
	vec4f(const vec3f& v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}
	
	vec4f(float x, const vec3f& v) : x(x), y(v.x), z(v.y), w(v.z) {}
	
	vec4f(const vec4f& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
	
	vec4f operator-() const {
		return {-x, -y, -z, -w};
	}
	
	void operator+=(const vec4f& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
	}
	
	void operator-=(const vec4f& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
	}
	
	void operator*=(float v) {
		x *= v;
		y *= v;
		z *= v;
		w *= v;
	}
	
	void operator/=(float v) {
		x /= v;
		y /= v;
		z /= v;
		w /= v;
	}
	
	float magnitude() const {
		return sqrtf(x * x + y * y + z * z + w * w);
	}
	
	float distance(const vec4f& v) const {
		return sqrtf((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z) + (w - v.w) * (w - v.w));
	}
	
	vec4f normalize() const {
		float l = sqrtf(x * x + y * y + z * z + w * w);
		return {x / l, y / l, z / l, w / l};
	}
};

inline vec4f operator+(const vec4f& v1, const vec4f& v2) {
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
}

inline vec4f operator-(const vec4f& v1, const vec4f& v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

inline vec4f operator*(const vec4f& v1, float v2) {
	return {v1.x * v2, v1.y * v2, v1.z * v2, v1.w * v2};
}

inline vec4f operator*(float v1, const vec4f& v2) {
	return {v2.x * v1, v2.y * v1, v2.z * v1, v2.w * v1};
}

inline vec4f operator/(const vec4f& v1, float v2) {
	return {v1.x / v2, v1.y / v2, v1.z / v2, v1.w / v2};
}

inline float operator*(const vec4f& v1, const vec4f& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

template <int r, int c>
class matf {
public:
	float m[r][c];
	
	float* operator[](size_t k) {
		return m[k];
	}
	
	const float* operator[](size_t k) const {
		return m[k];
	}
	
	void operator+=(const matf<r, c>& v) {
		for (int i = r * c; i --> 0;) {
			*(*m + i) += *(*v.m + i);
		}
	}

	void operator-=(const matf<r, c>& v) {
		for (int i = r * c; i --> 0;) {
			*(*m + i) -= *(*v.m + i);
		}
	}

	void operator*=(float v) {
		for (int i = r * c; i --> 0;) {
			*(*m + i) *= v;
		}
	}

	void operator/=(float v) {
		for (int i = r * c; i --> 0;) {
			*(*m + i) /= v;
		}
	}
	
	operator vec2f() {
		return {m[0][0], m[1][0]};
	}
	
	operator vec3f() {
		return {m[0][0], m[1][0], m[2][0]};
	}
	
	operator vec4f() {
		return {m[0][0], m[1][0], m[2][0], m[3][0]};
	}
	
	static matf<r, c> identity(int v) {
		matf<r, c> matrix;
		for (int i = 0; i < v; ++i) {
			matrix[i][i] = 1;
		}
		return matrix;
	}
	
	matf<c, r> transpose() const {
		matf<c, r> matrix;
		for (int i = 0; i < r; ++i) {
			for (int j = 0; j < c; ++j) {
				matrix[j][i] = m[i][j];
			}
		}
		return matrix;
	}
};

template <int r, int c>
matf<r, c> operator+(const matf<r, c>& v1, const matf<r, c>& v2) {
	matf<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) + *(*v2.m + i);
	}
	return matrix;
}

template <int r, int c>
matf<r, c> operator-(const matf<r, c>& v1, const matf<r, c>& v2) {
	matf<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) - *(*v2.m + i);
	}
	return matrix;
}

template <int r, int c>
matf<r, c> operator*(const matf<r, c>& v1, float v2) {
	matf<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) * v2;
	}
	return matrix;
}

template <int r, int c>
matf<r, c> operator*(float v1, const matf<r, c>& v2) {
	matf<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = v1 + *(*v2.m + i);
	}
	return matrix;
}

template <int l1, int l2, int l3>
matf<l1, l3> operator*(const matf<l1, l2>& v1, const matf<l2, l3>& v2) {
	matf<l1, l3> matrix;
	std::fill_n(matrix[0], l1 * l3, 0);
	for (int i = 0; i < l1; ++i) {
		for (int j = 0; j < l2; ++j) {
			for (int k = 0; k < l3; ++k) {
				matrix[i][k] += v1[i][j] * v2[j][k];
			}
		}
	}
	return matrix;
}

template <int r>
matf<r, 1> operator*(const matf<r, 2>& v1, const vec2f& v2) {
	matf<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y;
	}
	return matrix;
}

template <int r>
matf<r, 1> operator*(const matf<r, 3>& v1, const vec3f& v2) {
	matf<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y + v1[i][2] * v2.z;
	}
	return matrix;
}

template <int r>
matf<r, 1> operator*(const matf<r, 4>& v1, const vec4f& v2) {
	matf<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y + v1[i][2] * v2.z + v1[i][3] * v2.w;
	}
	return matrix;
}

template <int r, int c>
matf<r, c> operator/(const matf<r, c>& v1, float v2) {
	matf<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) / v2;
	}
	return matrix;
}

inline float mat2x2_determinant(const matf<2, 2>& m) {
	return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

inline float mat3x3_determinant(const matf<3, 3>& m) {
	float inv0 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
	float inv1 = m[1][2] * m[2][0] - m[1][0] * m[2][2];
	float inv2 = m[1][0] * m[2][1] - m[2][0] * m[1][1];
	return m[0][0] * inv0 + m[0][1] * inv1 + m[0][2] * inv2;
}

inline float mat4x4_determinant(const matf<4, 4>& m) {
	float s00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
	float s01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
	float s02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
	float s03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
	float s04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
	float s05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
	return m[0][0] * (m[1][1] * s00 - m[1][2] * s01 + m[1][3] * s02) -
		m[0][1] * (m[1][0] * s00 - m[1][2] * s03 + m[1][3] * s04) +
		m[0][2] * (m[1][0] * s01 - m[1][1] * s03 + m[1][3] * s05) -
		m[0][3] * (m[1][0] * s02 - m[1][1] * s04 + m[1][2] * s05);
}

inline matf<2, 2> mat2x2_inverse(const matf<2, 2>& m) {
	float inv_det = 1 / (m[0][0] * m[1][1] - m[0][1] * m[1][0]);
	return {
		 inv_det * m[1][1],
		-inv_det * m[0][1],
		-inv_det * m[1][0],
		 inv_det * m[0][0],
	};
}

inline matf<3, 3> mat3x3_inverse(const matf<3, 3>& m) {
	float inv0 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
	float inv1 = m[1][2] * m[2][0] - m[1][0] * m[2][2];
	float inv2 = m[1][0] * m[2][1] - m[2][0] * m[1][1];
	float inv_det = 1 / (m[0][0] * inv0 + m[0][1] * inv1 + m[0][2] * inv2);
	return {
		inv_det * inv0,
		inv_det * (m[0][2] * m[2][1] - m[0][1] * m[2][2]),
		inv_det * (m[0][1] * m[1][2] - m[0][2] * m[1][1]),
		inv_det * inv1,
		inv_det * (m[0][0] * m[2][2] - m[0][2] * m[2][0]),
		inv_det * (m[1][0] * m[0][2] - m[0][0] * m[1][2]),
		inv_det * inv2,
		inv_det * (m[2][0] * m[0][1] - m[0][0] * m[2][1]),
		inv_det * (m[0][0] * m[1][1] - m[1][0] * m[0][1]),
	};
}

inline matf<4, 4> mat4x4_inverse(const matf<4, 4>& m) {
	float s00 = m[2][2] * m[3][3] - m[2][3] * m[3][2];
	float s01 = m[2][1] * m[3][3] - m[2][3] * m[3][1];
	float s02 = m[2][1] * m[3][2] - m[2][2] * m[3][1];
	float s03 = m[2][0] * m[3][3] - m[2][3] * m[3][0];
	float s04 = m[2][0] * m[3][2] - m[2][2] * m[3][0];
	float s05 = m[2][0] * m[3][1] - m[2][1] * m[3][0];
	float s06 = m[1][2] * m[3][3] - m[1][3] * m[3][2];
	float s07 = m[1][1] * m[3][3] - m[1][3] * m[3][1];
	float s08 = m[1][1] * m[3][2] - m[1][2] * m[3][1];
	float s09 = m[1][2] * m[2][3] - m[1][3] * m[2][2];
	float s10 = m[1][1] * m[2][3] - m[1][3] * m[2][1];
	float s11 = m[1][1] * m[2][2] - m[1][2] * m[2][1];
	float s12 = m[1][0] * m[3][3] - m[1][3] * m[3][0];
	float s13 = m[1][0] * m[3][2] - m[1][2] * m[3][0];
	float s14 = m[1][0] * m[2][3] - m[1][3] * m[2][0];
	float s15 = m[1][0] * m[2][2] - m[1][2] * m[2][0];
	float s16 = m[1][0] * m[3][1] - m[1][1] * m[3][0];
	float s17 = m[1][0] * m[2][1] - m[1][1] * m[2][0];
	float inv0 = m[1][1] * s00 - m[1][2] * s01 + m[1][3] * s02;
	float inv1 = m[1][0] * s00 - m[1][2] * s03 + m[1][3] * s04;
	float inv2 = m[1][0] * s01 - m[1][1] * s03 + m[1][3] * s05;
	float inv3 = m[1][0] * s02 - m[1][1] * s04 + m[1][2] * s05;
	float inv_det = 1 / (m[0][0] * inv0 - m[0][1] * inv1 + m[0][2] * inv2 - m[0][3] * inv3);
	return {
		 inv_det * inv0,
		-inv_det * (m[0][1] * s00 - m[0][2] * s01 + m[0][3] * s02),
		 inv_det * (m[0][1] * s06 - m[0][2] * s07 + m[0][3] * s08),
		-inv_det * (m[0][1] * s09 - m[0][2] * s10 + m[0][3] * s11),
		-inv_det * inv1,
		 inv_det * (m[0][0] * s00 - m[0][2] * s03 + m[0][3] * s04),
		-inv_det * (m[0][0] * s06 - m[0][2] * s12 + m[0][3] * s13),
		 inv_det * (m[0][0] * s09 - m[0][2] * s14 + m[0][3] * s15),
		 inv_det * inv2,
		-inv_det * (m[0][0] * s01 - m[0][1] * s03 + m[0][3] * s05),
		 inv_det * (m[0][0] * s07 - m[0][1] * s12 + m[0][3] * s16),
		-inv_det * (m[0][0] * s10 - m[0][1] * s14 + m[0][3] * s17),
		-inv_det * inv3,
		 inv_det * (m[0][0] * s02 - m[0][1] * s04 + m[0][2] * s05),
		-inv_det * (m[0][0] * s08 - m[0][1] * s13 + m[0][2] * s16),
		 inv_det * (m[0][0] * s11 - m[0][1] * s15 + m[0][2] * s17),
	};
}

inline double random01d() {
	return rand() / (RAND_MAX + 1.);
}

class vec2d {
public:
	double x = 0;
	double y = 0;
	
	vec2d() = default;
	
	vec2d(double x, double y) : x(x), y(y) {}
	
	vec2d(const vec2d& v) : x(v.x), y(v.y) {}
	
	vec2d operator-() const {
		return {-x, -y};
	}
	
	void operator+=(const vec2d& v) {
		x += v.x;
		y += v.y;
	}
	
	void operator-=(const vec2d& v) {
		x -= v.x;
		y -= v.y;
	}
	
	void operator*=(double v) {
		x *= v;
		y *= v;
	}
	
	void operator/=(double v) {
		x /= v;
		y /= v;
	}
	
	double magnitude() const {
		return sqrt(x * x + y * y);
	}
	
	double distance(const vec2d& v) const {
		return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y));
	}
	
	vec2d normalize() const {
		double l = sqrt(x * x + y * y);
		return {x / l, y / l};
	}
	
	vec2d rotate(double a) const {
		return {x * cos(a) - y * sin(a), x * sin(a) + y * cos(a)};
	}
	
	static vec2d randomunit() {
		double angle = random01d() * M_PI * 2;
		return {cos(angle), sin(angle)};
	}
};

inline vec2d operator+(const vec2d& v1, const vec2d& v2) {
	return {v1.x + v2.x, v1.y + v2.y};
}

inline vec2d operator-(const vec2d& v1, const vec2d& v2) {
	return {v1.x - v2.x, v1.y - v2.y};
}

inline vec2d operator*(const vec2d& v1, double v2) {
	return {v1.x * v2, v1.y * v2};
}

inline vec2d operator*(double v1, const vec2d& v2) {
	return {v2.x * v1, v2.y * v1};
}

inline vec2d operator/(const vec2d& v1, double v2) {
	return {v1.x / v2, v1.y / v2};
}

inline double operator*(const vec2d& v1, const vec2d& v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

inline double operator^(const vec2d& v1, const vec2d& v2) {
	return v1.x * v2.y - v1.y * v2.x;
}

class vec3d {
public:
	double x = 0;
	double y = 0;
	double z = 0;
	
	vec3d() = default;
	
	vec3d(double x, double y, double z) : x(x), y(y), z(z) {}
	
	vec3d(const vec2d& v, double z) : x(v.x), y(v.y), z(z) {}
	
	vec3d(double x, const vec2d& v) : x(x), y(v.x), z(v.y) {}
	
	vec3d(const vec3d& v) : x(v.x), y(v.y), z(v.z) {}
	
	vec3d operator-() const {
		return {-x, -y, -z};
	}
	
	void operator+=(const vec3d& v) {
		x += v.x;
		y += v.y;
		z += v.z;
	}
	
	void operator-=(const vec3d& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
	}
	
	void operator*=(double v) {
		x *= v;
		y *= v;
		z *= v;
	}
	
	void operator/=(double v) {
		x /= v;
		y /= v;
		z /= v;
	}
	
	double magnitude() const {
		return sqrt(x * x + y * y + z * z);
	}
	
	double distance(const vec3d& v) const {
		return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
	}
	
	vec3d normalize() const {
		double l = sqrt(x * x + y * y + z * z);
		return {x / l, y / l, z / l};
	}
	
	vec3d rotate(const vec3d& v, double a) const {
		return {(cos(a) + (1 - cos(a)) * v.x * v.x) * x +
			((1 - cos(a)) * v.x * v.y - sin(a) * v.z) * y +
			((1 - cos(a)) * v.x * v.z + sin(a) * v.y) * z,
			((1 - cos(a)) * v.x * v.y + sin(a) * v.z) * x +
			(cos(a) + (1 - cos(a)) * v.y * v.y) * y +
			((1 - cos(a)) * v.y * v.z - sin(a) * v.x) * z,
			((1 - cos(a)) * v.x * v.z - sin(a) * v.y) * x +
			((1 - cos(a)) * v.y * v.z + sin(a) * v.x) * y +
			(cos(a) + (1 - cos(a)) * v.z * v.z) * z};
	}
	
	static vec3d randomunit() {
		double angle1 = random01d() * M_PI * 2;
		double angle2 = random01d() * M_PI * 2;
		return {cos(angle1) * cos(angle2), sin(angle2), sin(angle1) * cos(angle2)};
	}
};

inline vec3d operator+(const vec3d& v1, const vec3d& v2) {
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

inline vec3d operator-(const vec3d& v1, const vec3d& v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

inline vec3d operator*(const vec3d& v1, double v2) {
	return {v1.x * v2, v1.y * v2, v1.z * v2};
}

inline vec3d operator*(double v1, const vec3d& v2) {
	return {v2.x * v1, v2.y * v1, v2.z * v1};
}

inline vec3d operator/(const vec3d& v1, double v2) {
	return {v1.x / v2, v1.y / v2, v1.z / v2};
}

inline double operator*(const vec3d& v1, const vec3d& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline vec3d operator^(const vec3d& v1, const vec3d& v2) {
	return {v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x};
}

class vec4d {
public:
	double x = 0;
	double y = 0;
	double z = 0;
	double w = 0;
	
	vec4d() = default;
	
	vec4d(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}
	
	vec4d(const vec2d& v, double z, double w) : x(v.x), y(v.y), z(z), w(w) {}
	
	vec4d(double x, const vec2d& v, double w) : x(x), y(v.x), z(v.y), w(w) {}
	
	vec4d(double x, double y, const vec2d& v) : x(x), y(y), z(v.x), w(v.y) {}
	
	vec4d(const vec2d& v1, const vec2d& v2) : x(v1.x), y(v1.y), z(v2.x), w(v2.y) {}
	
	vec4d(const vec3d& v, double w) : x(v.x), y(v.y), z(v.z), w(w) {}
	
	vec4d(double x, const vec3d& v) : x(x), y(v.x), z(v.y), w(v.z) {}
	
	vec4d(const vec4d& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
	
	vec4d operator-() const {
		return {-x, -y, -z, -w};
	}
	
	void operator+=(const vec4d& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
	}
	
	void operator-=(const vec4d& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
	}
	
	void operator*=(double v) {
		x *= v;
		y *= v;
		z *= v;
		w *= v;
	}
	
	void operator/=(double v) {
		x /= v;
		y /= v;
		z /= v;
		w /= v;
	}
	
	double magnitude() const {
		return sqrt(x * x + y * y + z * z + w * w);
	}
	
	double distance(const vec4d& v) const {
		return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z) + (w - v.w) * (w - v.w));
	}
	
	vec4d normalize() const {
		double l = sqrt(x * x + y * y + z * z + w * w);
		return {x / l, y / l, z / l, w / l};
	}
};

inline vec4d operator+(const vec4d& v1, const vec4d& v2) {
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
}

inline vec4d operator-(const vec4d& v1, const vec4d& v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

inline vec4d operator*(const vec4d& v1, double v2) {
	return {v1.x * v2, v1.y * v2, v1.z * v2, v1.w * v2};
}

inline vec4d operator*(double v1, const vec4d& v2) {
	return {v2.x * v1, v2.y * v1, v2.z * v1, v2.w * v1};
}

inline vec4d operator/(const vec4d& v1, double v2) {
	return {v1.x / v2, v1.y / v2, v1.z / v2, v1.w / v2};
}

inline double operator*(const vec4d& v1, const vec4d& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

template <int r, int c>
class matd {
public:
	double m[r][c];
	
	double* operator[](size_t k) {
		return m[k];
	}
	
	const double* operator[](size_t k) const {
		return m[k];
	}
	
	void operator+=(const matd<r, c>& v) {
		for (int i = r * c; i --> 0;) {
			*(*m + i) += *(*v.m + i);
		}
	}

	void operator-=(const matd<r, c>& v) {
		for (int i = r * c; i --> 0;) {
			*(*m + i) -= *(*v.m + i);
		}
	}

	void operator*=(double v) {
		for (int i = r * c; i --> 0;) {
			*(*m + i) *= v;
		}
	}

	void operator/=(double v) {
		for (int i = r * c; i --> 0;) {
			*(*m + i) /= v;
		}
	}
	
	operator vec2d() const {
		return {m[0][0], m[1][0]};
	}
	
	operator vec3d() const {
		return {m[0][0], m[1][0], m[2][0]};
	}
	
	operator vec4d() const {
		return {m[0][0], m[1][0], m[2][0], m[3][0]};
	}
	
	static matd<r, c> identity(int v) {
		matd<r, c> matrix;
		for (int i = 0; i < v; ++i) {
			matrix[i][i] = 1;
		}
		return matrix;
	}
	
	matd<c, r> transpose() const {
		matd<c, r> matrix;
		for (int i = 0; i < r; ++i) {
			for (int j = 0; j < c; ++j) {
				matrix[j][i] = m[i][j];
			}
		}
		return matrix;
	}
};

template <int r, int c>
matd<r, c> operator+(const matd<r, c>& v1, const matd<r, c>& v2) {
	matd<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) + *(*v2.m + i);
	}
	return matrix;
}

template <int r, int c>
matd<r, c> operator-(const matd<r, c>& v1, const matd<r, c>& v2) {
	matd<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) - *(*v2.m + i);
	}
	return matrix;
}

template <int r, int c>
matd<r, c> operator*(const matd<r, c>& v1, double v2) {
	matd<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) * v2;
	}
	return matrix;
}

template <int r, int c>
matd<r, c> operator*(double v1, const matd<r, c>& v2) {
	matd<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = v1 + *(*v2.m + i);
	}
	return matrix;
}

template <int l1, int l2, int l3>
matd<l1, l3> operator*(const matd<l1, l2>& v1, const matd<l2, l3>& v2) {
	matd<l1, l3> matrix;
	std::fill_n(matrix[0], l1 * l3, 0);
	for (int i = 0; i < l1; ++i) {
		for (int j = 0; j < l2; ++j) {
			for (int k = 0; k < l3; ++k) {
				matrix[i][k] += v1[i][j] * v2[j][k];
			}
		}
	}
	return matrix;
}

template <int r>
matd<r, 1> operator*(const matd<r, 2>& v1, const vec2d& v2) {
	matd<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y;
	}
	return matrix;
}

template <int r>
matd<r, 1> operator*(const matd<r, 3>& v1, const vec3d& v2) {
	matd<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y + v1[i][2] * v2.z;
	}
	return matrix;
}

template <int r>
matd<r, 1> operator*(const matd<r, 4>& v1, const vec4d& v2) {
	matd<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y + v1[i][2] * v2.z + v1[i][3] * v2.w;
	}
	return matrix;
}

template <int r, int c>
matd<r, c> operator/(const matd<r, c>& v1, double v2) {
	matd<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) / v2;
	}
	return matrix;
}

inline double mat2x2_determinant(const matd<2, 2>& m) {
	return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

inline double mat3x3_determinant(const matd<3, 3>& m) {
	double inv0 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
	double inv1 = m[1][2] * m[2][0] - m[1][0] * m[2][2];
	double inv2 = m[1][0] * m[2][1] - m[2][0] * m[1][1];
	return m[0][0] * inv0 + m[0][1] * inv1 + m[0][2] * inv2;
}

inline double mat4x4_determinant(const matd<4, 4>& m) {
	double s00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
	double s01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
	double s02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
	double s03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
	double s04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
	double s05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
	return m[0][0] * (m[1][1] * s00 - m[1][2] * s01 + m[1][3] * s02) -
		m[0][1] * (m[1][0] * s00 - m[1][2] * s03 + m[1][3] * s04) +
		m[0][2] * (m[1][0] * s01 - m[1][1] * s03 + m[1][3] * s05) -
		m[0][3] * (m[1][0] * s02 - m[1][1] * s04 + m[1][2] * s05);
}

inline matd<2, 2> mat2x2_inverse(const matd<2, 2>& m) {
	double inv_det = 1 / (m[0][0] * m[1][1] - m[0][1] * m[1][0]);
	return {
		 inv_det * m[1][1],
		-inv_det * m[0][1],
		-inv_det * m[1][0],
		 inv_det * m[0][0],
	};
}

inline matd<3, 3> mat3x3_inverse(const matd<3, 3>& m) {
	double inv0 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
	double inv1 = m[1][2] * m[2][0] - m[1][0] * m[2][2];
	double inv2 = m[1][0] * m[2][1] - m[2][0] * m[1][1];
	double inv_det = 1 / (m[0][0] * inv0 + m[0][1] * inv1 + m[0][2] * inv2);
	return {
		inv_det * inv0,
		inv_det * (m[0][2] * m[2][1] - m[0][1] * m[2][2]),
		inv_det * (m[0][1] * m[1][2] - m[0][2] * m[1][1]),
		inv_det * inv1,
		inv_det * (m[0][0] * m[2][2] - m[0][2] * m[2][0]),
		inv_det * (m[1][0] * m[0][2] - m[0][0] * m[1][2]),
		inv_det * inv2,
		inv_det * (m[2][0] * m[0][1] - m[0][0] * m[2][1]),
		inv_det * (m[0][0] * m[1][1] - m[1][0] * m[0][1]),
	};
}

inline matd<4, 4> mat4x4_inverse(const matd<4, 4>& m) {
	double s00 = m[2][2] * m[3][3] - m[2][3] * m[3][2];
	double s01 = m[2][1] * m[3][3] - m[2][3] * m[3][1];
	double s02 = m[2][1] * m[3][2] - m[2][2] * m[3][1];
	double s03 = m[2][0] * m[3][3] - m[2][3] * m[3][0];
	double s04 = m[2][0] * m[3][2] - m[2][2] * m[3][0];
	double s05 = m[2][0] * m[3][1] - m[2][1] * m[3][0];
	double s06 = m[1][2] * m[3][3] - m[1][3] * m[3][2];
	double s07 = m[1][1] * m[3][3] - m[1][3] * m[3][1];
	double s08 = m[1][1] * m[3][2] - m[1][2] * m[3][1];
	double s09 = m[1][2] * m[2][3] - m[1][3] * m[2][2];
	double s10 = m[1][1] * m[2][3] - m[1][3] * m[2][1];
	double s11 = m[1][1] * m[2][2] - m[1][2] * m[2][1];
	double s12 = m[1][0] * m[3][3] - m[1][3] * m[3][0];
	double s13 = m[1][0] * m[3][2] - m[1][2] * m[3][0];
	double s14 = m[1][0] * m[2][3] - m[1][3] * m[2][0];
	double s15 = m[1][0] * m[2][2] - m[1][2] * m[2][0];
	double s16 = m[1][0] * m[3][1] - m[1][1] * m[3][0];
	double s17 = m[1][0] * m[2][1] - m[1][1] * m[2][0];
	double inv0 = m[1][1] * s00 - m[1][2] * s01 + m[1][3] * s02;
	double inv1 = m[1][0] * s00 - m[1][2] * s03 + m[1][3] * s04;
	double inv2 = m[1][0] * s01 - m[1][1] * s03 + m[1][3] * s05;
	double inv3 = m[1][0] * s02 - m[1][1] * s04 + m[1][2] * s05;
	double inv_det = 1 / (m[0][0] * inv0 - m[0][1] * inv1 + m[0][2] * inv2 - m[0][3] * inv3);
	return {
		 inv_det * inv0,
		-inv_det * (m[0][1] * s00 - m[0][2] * s01 + m[0][3] * s02),
		 inv_det * (m[0][1] * s06 - m[0][2] * s07 + m[0][3] * s08),
		-inv_det * (m[0][1] * s09 - m[0][2] * s10 + m[0][3] * s11),
		-inv_det * inv1,
		 inv_det * (m[0][0] * s00 - m[0][2] * s03 + m[0][3] * s04),
		-inv_det * (m[0][0] * s06 - m[0][2] * s12 + m[0][3] * s13),
		 inv_det * (m[0][0] * s09 - m[0][2] * s14 + m[0][3] * s15),
		 inv_det * inv2,
		-inv_det * (m[0][0] * s01 - m[0][1] * s03 + m[0][3] * s05),
		 inv_det * (m[0][0] * s07 - m[0][1] * s12 + m[0][3] * s16),
		-inv_det * (m[0][0] * s10 - m[0][1] * s14 + m[0][3] * s17),
		-inv_det * inv3,
		 inv_det * (m[0][0] * s02 - m[0][1] * s04 + m[0][2] * s05),
		-inv_det * (m[0][0] * s08 - m[0][1] * s13 + m[0][2] * s16),
		 inv_det * (m[0][0] * s11 - m[0][1] * s15 + m[0][2] * s17),
	};
}

using vec2 = vec2f;
using vec3 = vec3f;
using vec4 = vec4f;
template<int r, int c> using mat = gl::matf<r, c>;

inline constexpr float eps = 1e-6;

struct vertexobject {
	unsigned int id = 0;					/* read only */
	int length = 0;							/* read & write */
	std::vector<int> size;					/* read & write */
	std::vector<std::string> name;			/* read & write, optional */
	std::vector<unsigned int> buffer_id;	/* read only */
	std::vector<std::vector<float> > data;	/* read & write */
};

struct texture {
	unsigned int id = 0;					/* read only */
	int width = 0;							/* read & write */
	int height = 0;							/* read & write */
	int bytes = 0;							/* read & write */
	std::string name;						/* read & write, optional */
	std::vector<uint8_t> data;				/* read & write */
};

enum {
	MESH_DIRECT,
	MESH_INDEXED,
};

class mesh {
public:
	int mode = MESH_DIRECT;
	std::string name;
	std::vector<vec3> vertex;
	std::vector<vec3> normal;
	std::vector<vec3> binormal;
	std::vector<vec3> tangent;
	std::vector<vec2> uv;
	std::vector<vec3> vertex_color;
	std::vector<unsigned int> face_vertex;
	std::vector<unsigned int> face_normal;
	std::vector<unsigned int> face_binormal;
	std::vector<unsigned int> face_tangent;
	std::vector<unsigned int> face_uv;
	std::vector<unsigned int> face_vertex_color;
	
	void load(vertexobject& vo) const {
		bool has_normal = !normal.empty();
		bool has_binormal = !binormal.empty();
		bool has_tangent = !tangent.empty();
		bool has_uv = !uv.empty();
		bool has_vertex_color = !vertex_color.empty();
		vo.length = static_cast<int>(mode == MESH_DIRECT ? vertex.size() : face_vertex.size());
		vo.size = {3};
		vo.name = {"vertex"};
		if (has_normal) {
			vo.size.push_back(3);
			vo.name.push_back("normal");
		}
		if (has_binormal) {
			vo.size.push_back(3);
			vo.name.push_back("binormal");
		}
		if (has_tangent) {
			vo.size.push_back(3);
			vo.name.push_back("tangent");
		}
		if (has_uv) {
			vo.size.push_back(2);
			vo.name.push_back("uv");
		}
		if (has_vertex_color) {
			vo.size.push_back(3);
			vo.name.push_back("vertex_color");
		}
		vo.data.resize(vo.size.size());
		for (int i = 0; i < vo.data.size(); ++i) {
			vo.data[i].resize(vo.length * vo.size[i]);
		}
		auto data_iter = vo.data.begin();
		float* data_ptr = nullptr;
		if (mode == MESH_DIRECT) {
			data_ptr = data_iter->data();
			for (int i = 0; i < vo.length; ++i) {
				*(data_ptr++) = vertex[i].x;
				*(data_ptr++) = vertex[i].y;
				*(data_ptr++) = vertex[i].z;
			}
			++data_iter;
			if (has_normal) {
				data_ptr = data_iter->data();
				for (int i = 0; i < vo.length; ++i) {
					*(data_ptr++) = normal[i].x;
					*(data_ptr++) = normal[i].y;
					*(data_ptr++) = normal[i].z;
				}
				++data_iter;
			}
			if (has_binormal) {
				data_ptr = data_iter->data();
				for (int i = 0; i < vo.length; ++i) {
					*(data_ptr++) = binormal[i].x;
					*(data_ptr++) = binormal[i].y;
					*(data_ptr++) = binormal[i].z;
				}
				++data_iter;
			}
			if (has_tangent) {
				data_ptr = data_iter->data();
				for (int i = 0; i < vo.length; ++i) {
					*(data_ptr++) = tangent[i].x;
					*(data_ptr++) = tangent[i].y;
					*(data_ptr++) = tangent[i].z;
				}
				++data_iter;
			}
			if (has_uv) {
				data_ptr = data_iter->data();
				for (int i = 0; i < vo.length; ++i) {
					*(data_ptr++) = uv[i].x;
					*(data_ptr++) = uv[i].y;
				}
				++data_iter;
			}
			if (has_vertex_color) {
				data_ptr = data_iter->data();
				for (int i = 0; i < vo.length; ++i) {
					*(data_ptr++) = vertex_color[i].x;
					*(data_ptr++) = vertex_color[i].y;
					*(data_ptr++) = vertex_color[i].z;
				}
				++data_iter;
			}
		} else {
			data_ptr = data_iter->data();
			for (int i = 0; i < vo.length; ++i) {
				*(data_ptr++) = vertex[face_vertex[i]].x;
				*(data_ptr++) = vertex[face_vertex[i]].y;
				*(data_ptr++) = vertex[face_vertex[i]].z;
			}
			++data_iter;
			if (has_normal) {
				data_ptr = data_iter->data();
				for (int i = 0; i < vo.length; ++i) {
					*(data_ptr++) = normal[face_normal[i]].x;
					*(data_ptr++) = normal[face_normal[i]].y;
					*(data_ptr++) = normal[face_normal[i]].z;
				}
				++data_iter;
			}
			if (has_binormal) {
				data_ptr = data_iter->data();
				for (int i = 0; i < vo.length; ++i) {
					*(data_ptr++) = binormal[face_binormal[i]].x;
					*(data_ptr++) = binormal[face_binormal[i]].y;
					*(data_ptr++) = binormal[face_binormal[i]].z;
				}
				++data_iter;
			}
			if (has_tangent) {
				data_ptr = data_iter->data();
				for (int i = 0; i < vo.length; ++i) {
					*(data_ptr++) = tangent[face_tangent[i]].x;
					*(data_ptr++) = tangent[face_tangent[i]].y;
					*(data_ptr++) = tangent[face_tangent[i]].z;
				}
				++data_iter;
			}
			if (has_uv) {
				data_ptr = data_iter->data();
				for (int i = 0; i < vo.length; ++i) {
					*(data_ptr++) = uv[face_uv[i]].x;
					*(data_ptr++) = uv[face_uv[i]].y;
				}
				++data_iter;
			}
			if (has_vertex_color) {
				data_ptr = data_iter->data();
				for (int i = 0; i < vo.length; ++i) {
					*(data_ptr++) = vertex_color[face_vertex_color[i]].x;
					*(data_ptr++) = vertex_color[face_vertex_color[i]].y;
					*(data_ptr++) = vertex_color[face_vertex_color[i]].z;
				}
				++data_iter;
			}
		}
	}
};

class instance {
public:
	vec3 position;
	vec3 rotation;
	vec3 scale;
	void* userdata;
	vertexobject* object;
	std::vector<texture*> textures;
	
	instance() : position({}), rotation({}), scale({1, 1, 1}) {}
	
	instance(const vec3& p, const vec3& r, const vec3& s) :
		position(p), rotation(r), scale(s) {}
	
	instance(vertexobject* o) : object(o), position({}), rotation({}), scale({1, 1, 1}) {}
	
	instance(vertexobject* o, const vec3& p, const vec3& r, const vec3& s) :
		object(o), position(p), rotation(r), scale(s) {}
	
	instance(vertexobject* o, std::vector<texture*>& ts) :
		object(o), textures(ts), position({}), rotation({}), scale({1, 1, 1}) {}
	
	instance(vertexobject* o, std::vector<texture*>& ts,
			 const vec3& p, const vec3& r, const vec3& s) :
		object(o), textures(ts), position(p), rotation(r), scale(s) {}
};

class scene {
public:
	std::vector<instance*> instances;
	
	size_t instance_size() const {
		return instances.size();
	}
	
	instance* const* get_instances() const {
		return instances.data();
	}
	
	void add_instance(instance* i) {
		instances.push_back(i);
	}
};

enum {
	NO_ERROR,			/* No errors */
	ERROR_NO_MEMORY,	/* Out of memory */
	ERROR_READ,			/* Error reading from file */
	ERROR_WRITE,		/* Error writing to file */
	ERROR_SEEK,			/* Error seeking in file */
	ERROR_UNSOPPORTED,	/* That operation is not supported */
	ERROR_UNKNOWN,		/* Unknown error */
};

class error {
public:
	/**
	 * \param c error code
	 * \param m error message
	 */
	static void set(int c, std::string m = "") {
		error_code = c;
		if (m.empty()) {
			if (c == ERROR_NO_MEMORY) {
				message = "Out of memory";
			} else if (c == ERROR_READ) {
				message = "Error reading from file";
			} else if (c == ERROR_WRITE) {
				message = "Error writing to file";
			} else if (c == ERROR_SEEK) {
				message = "Error seeking in file";
			} else if (c == ERROR_UNSOPPORTED) {
				message = "That operation is not supported";
			} else {
				message = "Unknown error";
			}
		} else {
			message = m;
		}
	}
	
	static int get_code() {
		return error_code;
	}
	
	static std::string get_message() {
		if (error_code == NO_ERROR) return "";
		return message + '\n';
	}
	
	static void clear() {
		error_code = NO_ERROR;
	}
	
private:
	static int error_code;
	static std::string message;
};

inline int error::error_code = NO_ERROR;
inline std::string error::message;

/**
 * \param s source file
 * \param c content
 */
inline int read_file(const char* s, std::string& c) {
	std::ifstream stream(s, std::fstream::in);
	if (!stream) {
		error::set(ERROR_READ);
		return -1;
	}
	stream.seekg(0, stream.end);
	size_t length = stream.tellg();
	c.resize(length);
	stream.seekg(0, stream.beg);
	stream.read(c.data(), length);
	stream.close();
	return 0;
}

/**
 * \param s source file
 * \param c content
 */
inline int read_file(const std::string& s, std::string& c) {
	std::ifstream stream(s, std::fstream::in);
	if (!stream) {
		error::set(ERROR_READ);
		return -1;
	}
	stream.seekg(0, stream.end);
	size_t length = stream.tellg();
	c.resize(length);
	stream.seekg(0, stream.beg);
	stream.read(c.data(), length);
	stream.close();
	return 0;
}

/**
 * \param s source file
 * \param c content
 */
inline int write_file(const char* s, const std::string& c) {
	std::ofstream stream(s, std::fstream::out);
	stream.write(c.data(), c.size());
	stream.close();
	if (!stream) {
		error::set(ERROR_WRITE);
		return -1;
	}
	return 0;
}

/**
 * \param s source file
 * \param c content
 */
inline int write_file(const std::string& s, const std::string& c) {
	std::ofstream stream(s, std::fstream::out);
	stream.write(c.data(), c.size());
	stream.close();
	if (!stream) {
		error::set(ERROR_WRITE);
		return -1;
	}
	return 0;
}

/*
 *	---- example.mtl -----------------------------------------------------------
 *
 *	newmtl mtl_a						|	[material]
 *
 *	Kd 1.00 0.00 0.00					|	[diffuse]
 *
 *	newmtl mtl_b						|	[material]
 *
 *	Kd 0.00 0.00 1.00					|	[diffuse]
 *
 *	---- example.obj -----------------------------------------------------------
 *
 *	mtllib example.mtl					|	[material library]
 *
 *	v -1.00 -1.00 0.00					|	[geometric vertices] Specifies a
 *	v 1.00 -1.00 0.00					|	geometric vertex and its x y z
 *	v -1.00 1.00 0.00					|	coordinates.
 *	v 1.00 1.00 0.00					|
 *
 *	vn 0.00 0.00 1.00					|	[vertex normals] Specifies a normal
 *	vn 0.00 0.00 -1.00					|	vector with components i, j, and k.
 *
 *	vt -1.00 -1.00						|	[texture vertices] Specifies a
 *	vt 1.00 -1.00						|	texture vertex and its coordinates.
 *	vt -1.00 1.00						|	1D texture ans 3D texture are not
 *	vt 1.00 1.00						|	supported.
 *
 *	g square_a							|	[group name]
 *
 *	usemtl mtl_a						|	[material name]
 *
 *	f 1/2/3 1/2/3 1/1/1					|	[face] Specifies a geometric vertex,
 *	f 3/2/4 3/2/4 1/1/1					|	texture vertex, and vertex normal.
 *
 *	g square_b							|	[group name]
 *
 *	usemtl mtl_b						|	[material name]
 *
 *	f 1//2 1//2 2//2					|	[face] Only specifies a geometric
 *	f 2//4 2//4 2//2					|	vertex and vertex normals.
 */

/**
 * \param f mtl file
 * \param m output mesh
 * \param map indexed material map
 */
inline int read_mtl(const std::string& f, gl::mesh& m,
					std::unordered_map<std::string, int>& map) {
	std::ifstream in;
	in.open(f, std::ifstream::in);
	if (!in) {
		error::set(ERROR_READ, "Failed to read from mtl");
		return -1;
	}
	std::string line;
	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream stream(line.c_str());
		std::string keyword;
		stream >> keyword;
		if (keyword == "newmtl") {
			std::string name;
			stream >> name;
			map.insert({name, map.size()});
		} else if (keyword == "Kd") {
			vec3 color;
			stream >> color.x >> color.y >> color.z;
			m.vertex_color.push_back(color);
		}
	}
	in.close();
	return 0;
}

/**
 * \param f obj file
 * \param m output mesh
 */
inline int read_obj(const std::string& f, gl::mesh& m) {
	std::ifstream in;
	in.open(f, std::ifstream::in);
	if (!in) {
		error::set(ERROR_READ, "Failed to read from obj");
		return -1;
	}
	m.mode = MESH_INDEXED;
	int status = 0;
	std::unordered_map<std::string, int> vertex_color_map;
	int vertex_color_index = -1;
	std::string line;
	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream stream(line.c_str());
		std::string keyword;
		stream >> keyword;
		if (keyword == "v") {
			vec3 vertex;
			stream >> vertex.x >> vertex.y >> vertex.z;
			m.vertex.push_back(vertex);
		} else if (keyword == "vn") {
			vec3 normal;
			stream >> normal.x >> normal.y >> normal.z;
			m.normal.push_back(normal);
		} else if (keyword == "vt") {
			vec2 uv;
			stream >> uv.x >> uv.y;
			m.uv.push_back(uv);
		} else if (keyword == "f") {
			int index;
			for (int i = 0; i < 3; ++i) {
				stream >> index;
				m.face_vertex.push_back(index - 1);
				stream.get();
				if (std::isdigit(stream.peek())) {
					stream >> index;
					m.face_uv.push_back(index - 1);
				}
				stream.get();
				if (std::isdigit(stream.peek())) {
					stream >> index;
					m.face_normal.push_back(index - 1);
				}
			}
			if (vertex_color_index != -1) {
				for (int i = 0; i < 3; ++i) {
					m.face_vertex_color.push_back(vertex_color_index);
				}
			}
		} else if (keyword == "g") {
			/* TODO */
		} else if (keyword == "usemtl") {
			std::string name;
			stream >> name;
			if (vertex_color_map.find(name) == vertex_color_map.end()) {
				error::set(ERROR_SEEK, "Failed to find material name");
			} else {
				vertex_color_index = vertex_color_map[name];
			}
		} else if (keyword == "mtllib") {
			std::string file;
			stream >> file;
			status |= read_mtl(f.substr(0, f.rfind('/') + 1) + file, m, vertex_color_map);
		}
	}
	in.close();
	return status;
}

class camera {
public:
	/* inner parameter */
	int width = 0;
	int height = 0;
	float fovy = 0;
	float znear = 0;
	float zfar = 0;
	/* outer parameter */
	vec3 position;
	vec3 direction;
	vec3 up;
	/* matrices */
	mat<4, 4> viewing;
	mat<4, 4> projection;
	
	camera() = default;
	
	/**
	 * \param w width
	 * \param h height
	 * \param f fov on y-axis
	 * \param zn znear
	 * \param zf zfar
	 */
	camera(int w, int h, float f, float zn, float zf) : width(w), height(h), fovy(f), znear(zn), zfar(zf) {
		projection = {
			1 / (tanf(fovy / 2) * width / height), 0, 0, 0,
			0, 1 / tanf(fovy / 2), 0, 0,
			0, 0, (znear + zfar) / (znear - zfar), 2 * zfar * znear / (znear - zfar),
			0, 0, -1, 0,
		};
	}
	
	/**
	 *	\param p position
	 *	\param d direction
	 *	\param u up
	 */
	void lookat(const vec3& p, const vec3& d, const vec3& u) {
		position = p;
		direction = d;
		up = u;
		vec3 r = (-d) ^ u;
		viewing = {
			r.x, r.y, r.z, -(p * r),
			u.x, u.y, u.z, -(p * u),
			d.x, d.y, d.z, -(p * d),
			0, 0, 0, 1,
		};
	}
	
	/**
	 * \param i instance
	 * \param t translation matrix
	 * \param r rotation matrix
	 * \param s scaling matrix
	 */
	static void model_transform(const instance& i, mat<4, 4>& t, mat<4, 4>& r, mat<4, 4>& s) {
		t = {
			1, 0, 0, i.position.x,
			0, 1, 0, i.position.y,
			0, 0, 1, i.position.z,
			0, 0, 0, 1,
		};
		r =
		/* rotation x */ mat<4, 4>{
			1, 0, 0, 0,
			0, cos(i.rotation.x), -sin(i.rotation.x), 0,
			0, sin(i.rotation.x), cos(i.rotation.x), 0,
			0, 0, 0, 1,
		} *
		/* rotation y */ mat<4, 4>{
			cos(i.rotation.y), 0, -sin(i.rotation.y), 0,
			0, 1, 0, 0,
			sin(i.rotation.y), 0, cos(i.rotation.y), 0,
			0, 0, 0, 1,
		} *
		/* rotation z */ mat<4, 4>{
			cos(i.rotation.z), -sin(i.rotation.z), 0, 0,
			sin(i.rotation.z), cos(i.rotation.z), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1,
		};
		s = {
			i.scale.x, 0, 0, 0,
			0, i.scale.y, 0, 0,
			0, 0, i.scale.z, 0,
			0, 0, 0, 1,
		};
	}
	
	/**
	 *	\param i instance
	 *	\param t transform matrix
	 */
	static void model_transform(const instance& i, mat<4, 4>& t) {
		mat<4, 4> translation;
		mat<4, 4> rotation;
		mat<4, 4> scaling;
		model_transform(i, translation, rotation, scaling);
		t = translation * rotation * scaling;
	}
};

}

namespace gl::soft {

struct ginfo {
	vec3 normal;
	vec4 specular;
	vec4 irradiance;
	vec3 world_coord;
};

class shader {
public:
	mat<4, 4> model;
	mat<4, 4> viewing;
	mat<4, 4> projection;
	mat<4, 4> transform;
	const texture* const* textures;
	
	virtual void vextex_shader(const float** d, int id, vec4& v) = 0;
	virtual void geometry_shader(vec4* vs) = 0;
	virtual void fragment_shader(const vec3& b, const vec2& p, ginfo& g, vec4& c) = 0;
};

inline const vec3 nearest_map(const texture& t, float u, float v) {
	int x = u * t.width;
	x = x < 0 ? 0 : x;
	x = x >= t.width ? t.width - 1 : x;
	int y = v * t.height;
	y = y < 0 ? 0 : y;
	y = y >= t.height ? t.height - 1 : y;
	const uint8_t* pointer = t.data.data() + (x + y * t.width) * t.bytes;
	return {pointer[0] / 255.f, pointer[1] / 255.f, pointer[2] / 255.f};
}

inline const vec3 nearest_map(const texture& t, const vec2& uv) {
	return nearest_map(t, uv.x, uv.y);
}

inline const vec3 linear_map(const texture& t, float u, float v) {
	int x0 = u * t.width;
	x0 = x0 < 0 ? 0 : x0;
	x0 = x0 >= t.width ? t.width - 1 : x0;
	int x1 = x0 + 1;
	x1 = x1 >= t.width ? t.width - 1 : x1;
	int y0 = v * t.height;
	y0 = y0 < 0 ? 0 : y0;
	y0 = y0 >= t.height ? t.height - 1 : y0;
	int y1 = y0 + 1;
	y1 = y1 >= t.height ? t.height - 1 : y1;
	const uint8_t* pointer00 = t.data.data() + (x0 + y0 * t.width) * t.bytes;
	const uint8_t* pointer01 = t.data.data() + (x0 + y1 * t.width) * t.bytes;
	const uint8_t* pointer10 = t.data.data() + (x1 + y0 * t.width) * t.bytes;
	const uint8_t* pointer11 = t.data.data() + (x1 + y1 * t.width) * t.bytes;
	vec3 color00 = {pointer00[0] / 255.f, pointer00[1] / 255.f, pointer00[2] / 255.f};
	vec3 color01 = {pointer01[0] / 255.f, pointer01[1] / 255.f, pointer01[2] / 255.f};
	vec3 color10 = {pointer10[0] / 255.f, pointer10[1] / 255.f, pointer10[2] / 255.f};
	vec3 color11 = {pointer11[0] / 255.f, pointer11[1] / 255.f, pointer11[2] / 255.f};
	return (color00 * (y1 - v) + color01 * (v - y0)) * (x1 - u) + (color10 * (y1 - v) + color11 * (v - y0)) * (u - x0);
}

inline const vec3 linear_map(const texture& t, const vec2& uv) {
	return linear_map(t, uv.x, uv.y);
}

inline std::vector<double> zbuffer;
inline std::vector<ginfo> gbuffer;

struct pointlist {
	size_t size = 0;
	vec4* vertices = nullptr;
	vec3* barycenters = nullptr;
};

inline void add_pointlist(const vec4& v, const vec3& b, pointlist& l) {
	l.vertices[l.size] = v;
	l.barycenters[l.size] = b;
	++l.size;
}

inline void znear_clip(const pointlist& i, float zn, pointlist& o) {
	for (int l = 0; l < i.size; ++l) {
		vec4& vertex1 = i.vertices[l];
		vec4& vertex2 = i.vertices[(l + 1) % i.size];
		/* both outside */
		if (vertex1.w < zn && vertex2.w < zn) continue;
		/* both inside */
		const vec3& barycenter1 = i.barycenters[l];
		const vec3& barycenter2 = i.barycenters[(l + 1) % i.size];
		if (vertex1.w > zn && vertex2.w > zn) {
			add_pointlist(vertex2, barycenter2, o);
			continue;
		}
		/* from inside to outside */
		float weight1 = fabsf(vertex1.w - zn);
		float weight2 = fabsf(vertex2.w - zn);
		float inverse = 1 / (weight1 + weight2);
		if (vertex1.w > zn && vertex2.w < zn) {
			add_pointlist((vertex1 * weight2 + vertex2 * weight1) * inverse,
				(barycenter1 * weight2 + barycenter2 * weight1) * inverse, o);
			continue;
		}
		/* from outside to inside */
		add_pointlist((vertex1 * weight2 + vertex2 * weight1) * inverse,
			(barycenter1 * weight2 + barycenter2 * weight1) * inverse, o);
		add_pointlist(vertex2, barycenter2, o);
	}
}

inline void zfar_clip(const pointlist& i, float zf, pointlist& o) {
	for (int l = 0; l < i.size; ++l) {
		vec4& vertex1 = i.vertices[l];
		vec4& vertex2 = i.vertices[(l + 1) % i.size];
		/* both outside */
		if (vertex1.w > zf && vertex2.w > zf) continue;
		/* both inside */
		const vec3& barycenter1 = i.barycenters[l];
		const vec3& barycenter2 = i.barycenters[(l + 1) % i.size];
		if (vertex1.w < zf && vertex2.w < zf) {
			add_pointlist(vertex2, barycenter2, o);
			continue;
		}
		/* from inside to outside */
		float weight1 = fabsf(vertex1.w - zf);
		float weight2 = fabsf(vertex2.w - zf);
		float inverse = 1 / (weight1 + weight2);
		if (vertex1.w > zf && vertex2.w < zf) {
			add_pointlist((vertex1 * weight2 + vertex2 * weight1) * inverse,
				(barycenter1 * weight2 + barycenter2 * weight1) * inverse, o);
			continue;
		}
		/* from outside to inside */
		add_pointlist((vertex1 * weight2 + vertex2 * weight1) * inverse,
			(barycenter1 * weight2 + barycenter2 * weight1) * inverse, o);
		add_pointlist(vertex2, barycenter2, o);
	}
}

inline void rasterize(const camera& c, const pointlist& p, const vec3* dvs,
					  shader& s, vec3* canvas, double* zb, ginfo* gb) {
	for (int i = 2; i < p.size; ++i) {
		const vec3& vertexa = dvs[0];
		const vec3& vertexb = dvs[i - 1];
		const vec3& vertexc = dvs[i];
		vec3 fixeda = {p.barycenters[0].x, p.barycenters[0].y, p.barycenters[0].z};
		vec3 fixedb = {p.barycenters[i - 1].x, p.barycenters[i - 1].y, p.barycenters[i - 1].z};
		vec3 fixedc = {p.barycenters[i].x, p.barycenters[i].y, p.barycenters[i].z};
		vec3 fixed_barycenter = {1 / p.vertices[0].w, 1 / p.vertices[i - 1].w, 1 / p.vertices[i].w};
		vec2d v0 = {vertexc.x - vertexa.x, vertexc.y - vertexa.y};
		vec2d v1 = {vertexb.x - vertexa.x, vertexb.y - vertexa.y};
		vec2d v2;
		double dot00 = v0 * v0;
		double dot01 = v0 * v1;
		double dot11 = v1 * v1;
		double inverse = 1 / (dot00 * dot11 - dot01 * dot01);
		vec3 barycenter;
		vec4 color;
		vec3 vertexl = vertexa;
		vec3 vertexm = vertexb;
		vec3 vertexu = vertexc;
		if (vertexl.y > vertexm.y) std::swap(vertexl, vertexm);
		if (vertexm.y > vertexu.y) std::swap(vertexm, vertexu);
		if (vertexl.y > vertexm.y) std::swap(vertexl, vertexm);
		float lower = fmaxf(floorf(vertexl.y) + 1, 0);
		float upper = fminf(floorf(vertexu.y) + 1, c.height);
		float median = vertexm.y;
		float inverseml = 1 / (vertexm.y - vertexl.y);
		float inverseum = 1 / (vertexu.y - vertexm.y);
		float inverseul = 1 / (vertexu.y - vertexl.y);
		for (float y = lower; y < upper; ++y) {
			float left = y < median ? (vertexl.x * (vertexm.y - y) + vertexm.x * (y - vertexl.y)) * inverseml :
				(vertexm.x * (vertexu.y - y) + vertexu.x * (y - vertexm.y)) * inverseum;
			float right = (vertexl.x * (vertexu.y - y) + vertexu.x * (y - vertexl.y)) * inverseul;
			if (left > right) std::swap(left, right);
			left = fmaxf(floorf(left) + 1, 0);
			right = fminf(floorf(right) + 1, c.width);
			for (float x = left; x < right; ++x) {
				v2.x = x - vertexa.x;
				v2.y = y - vertexa.y;
				double dot02 = v0 * v2;
				double dot12 = v1 * v2;
				double u = (dot11 * dot02 - dot01 * dot12) * inverse;
				double v = (dot00 * dot12 - dot01 * dot02) * inverse;
				if (UNLIKELY(u < -eps || v < -eps || u + v > 1 + eps)) continue;
				double z = vertexa.z * (1 - u - v) + vertexb.z * v + vertexc.z * u;
				int location = x + y * c.width;
				if (z > -1 && z < 1 && z < zb[location] + eps) {
					zb[location] = z;
					barycenter.x = (1 - u - v) * fixed_barycenter.x;
					barycenter.y = v * fixed_barycenter.y;
					barycenter.z = u * fixed_barycenter.z;
					barycenter /= barycenter.x + barycenter.y + barycenter.z;
					barycenter = fixeda * barycenter.x + fixedb * barycenter.y + fixedc * barycenter.z;
					s.fragment_shader(barycenter, {x / c.width, y / c.height}, gb[location], color);
					canvas[location].x = canvas[location].x * (1 - color.w) + color.x * color.w;
					canvas[location].y = canvas[location].y * (1 - color.w) + color.y * color.w;
					canvas[location].z = canvas[location].z * (1 - color.w) + color.z * color.w;
				}
			}
		}
	}
}

template <bool write = true>
inline bool rasterize(const camera& c, const pointlist& p, const vec3* dvs, double* zb) {
	for (int i = 2; i < p.size; ++i) {
		const vec3& vertexa = dvs[0];
		const vec3& vertexb = dvs[i - 1];
		const vec3& vertexc = dvs[i];
		vec2d v0 = {vertexc.x - vertexa.x, vertexc.y - vertexa.y};
		vec2d v1 = {vertexb.x - vertexa.x, vertexb.y - vertexa.y};
		vec2d v2;
		double dot00 = v0 * v0;
		double dot01 = v0 * v1;
		double dot11 = v1 * v1;
		double inverse = 1 / (dot00 * dot11 - dot01 * dot01);
		vec3 vertexl = vertexa;
		vec3 vertexm = vertexb;
		vec3 vertexu = vertexc;
		if (vertexl.y > vertexm.y) std::swap(vertexl, vertexm);
		if (vertexm.y > vertexu.y) std::swap(vertexm, vertexu);
		if (vertexl.y > vertexm.y) std::swap(vertexl, vertexm);
		float lower = fmaxf(floorf(vertexl.y) + 1, 0);
		float upper = fminf(floorf(vertexu.y) + 1, c.height);
		float median = vertexm.y;
		float inverseml = 1 / (vertexm.y - vertexl.y);
		float inverseum = 1 / (vertexu.y - vertexm.y);
		float inverseul = 1 / (vertexu.y - vertexl.y);
		for (float y = lower; y < upper; ++y) {
			float left = y < median ? (vertexl.x * (vertexm.y - y) + vertexm.x * (y - vertexl.y)) * inverseml :
				(vertexm.x * (vertexu.y - y) + vertexu.x * (y - vertexm.y)) * inverseum;
			float right = (vertexl.x * (vertexu.y - y) + vertexu.x * (y - vertexl.y)) * inverseul;
			if (left > right) std::swap(left, right);
			left = fmaxf(floorf(left) + 1, 0);
			right = fminf(floorf(right) + 1, c.width);
			for (float x = left; x < right; ++x) {
				v2.x = x - vertexa.x;
				v2.y = y - vertexa.y;
				double dot02 = v0 * v2;
				double dot12 = v1 * v2;
				double u = (dot11 * dot02 - dot01 * dot12) * inverse;
				double v = (dot00 * dot12 - dot01 * dot02) * inverse;
				if (UNLIKELY(u < -eps || v < -eps || u + v > 1 + eps)) continue;
				double z = vertexa.z * (1 - u - v) + vertexb.z * v + vertexc.z * u;
				int location = x + y * c.width;
				if (z > -1 && z < 1 && z < zb[location] + eps) {
					if (!write) return true;
					zb[location] = z;
				}
			}
		}
	}
	return false;
}

inline void draw_objects(const camera& c, shader& s, const vertexobject& vo, vec3* canvas, double* zb, ginfo* gb) {
	static vec3 barycenters[3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
	std::vector<const float*> datas(vo.data.size());
	for (int i = 0; i < vo.length; i += 3) {
		vec4 vertices[3];
		/* vertex shader */
		for (int j = 0; j < 3; ++j) {
			for (size_t k = datas.size(); k --> 0;) {
				datas[k] = vo.data[k].data() + vo.size[k] * (i + j);
			}
			s.vextex_shader(datas.data(), j, vertices[j]);
		}
		/* geometry shader */
		s.geometry_shader(vertices);
		/* clipping */
		vec4 clip_vertices[4];
		vec3 clip_barycenters[4];
		pointlist clip_primitive = {0, clip_vertices, clip_barycenters};
		znear_clip({3, vertices, barycenters}, c.znear, clip_primitive);
		vec4 primitive_vertices[5];
		vec3 primitive_barycenters[5];
		pointlist primitive = {0, primitive_vertices, primitive_barycenters};
		zfar_clip(clip_primitive, c.zfar, primitive);
		/* perspective division */
		vec3 device_vertices[5];
		for (int i = 0; i < primitive.size; ++i) {
			device_vertices[i].x = primitive.vertices[i].x / primitive.vertices[i].w;
			device_vertices[i].y = primitive.vertices[i].y / primitive.vertices[i].w;
			device_vertices[i].z = primitive.vertices[i].z / primitive.vertices[i].w;
		}
		/* viewport transform */
		for (int i = 0; i < primitive.size; ++i) {
			device_vertices[i].x = device_vertices[i].x * c.width / 2 + c.width / 2;
			device_vertices[i].y = -device_vertices[i].y * c.height / 2 + c.height / 2;
		}
		/* rasterization */
		rasterize(c, primitive, device_vertices, s, canvas, zb, gb);
	}
}

inline void ztest(const camera& c, shader& s, const vertexobject& vo, double* zb) {
	static vec3 barycenters[3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
	std::vector<const float*> datas(vo.data.size());
	for (int i = 0; i < vo.length; i += 3) {
		vec4 vertices[3];
		/* vertex shader */
		for (int j = 0; j < 3; ++j) {
			for (size_t k = datas.size(); k --> 0;) {
				datas[k] = vo.data[k].data() + vo.size[k] * (i + j);
			}
			s.vextex_shader(datas.data(), j, vertices[j]);
		}
		/* geometry shader */
		s.geometry_shader(vertices);
		/* clipping */
		vec4 clip_vertices[4];
		vec3 clip_barycenters[4];
		pointlist clip_primitive = {0, clip_vertices, clip_barycenters};
		znear_clip({3, vertices, barycenters}, c.znear, clip_primitive);
		vec4 primitive_vertices[5];
		vec3 primitive_barycenters[5];
		pointlist primitive = {0, primitive_vertices, primitive_barycenters};
		zfar_clip(clip_primitive, c.zfar, primitive);
		/* perspective division */
		vec3 device_vertices[5];
		for (int i = 0; i < primitive.size; ++i) {
			device_vertices[i].x = primitive.vertices[i].x / primitive.vertices[i].w;
			device_vertices[i].y = primitive.vertices[i].y / primitive.vertices[i].w;
			device_vertices[i].z = primitive.vertices[i].z / primitive.vertices[i].w;
		}
		/* viewport transform */
		for (int i = 0; i < primitive.size; ++i) {
			device_vertices[i].x = device_vertices[i].x * c.width / 2 + c.width / 2;
			device_vertices[i].y = -device_vertices[i].y * c.height / 2 + c.height / 2;
		}
		/* rasterization */
		rasterize(c, primitive, device_vertices, zb);
	}
}

inline bool zquery(const camera& c, shader& s, const vertexobject& vo, double* zb) {
	static vec3 barycenters[3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
	std::vector<const float*> datas(vo.data.size());
	for (int i = 0; i < vo.length; i += 3) {
		vec4 vertices[3];
		/* vertex shader */
		for (int j = 0; j < 3; ++j) {
			for (size_t k = datas.size(); k --> 0;) {
				datas[k] = vo.data[k].data() + vo.size[k] * (i + j);
			}
			s.vextex_shader(datas.data(), j, vertices[j]);
		}
		/* geometry shader */
		s.geometry_shader(vertices);
		/* clipping */
		vec4 clip_vertices[4];
		vec3 clip_barycenters[4];
		pointlist clip_primitive = {0, clip_vertices, clip_barycenters};
		znear_clip({3, vertices, barycenters}, c.znear, clip_primitive);
		vec4 primitive_vertices[5];
		vec3 primitive_barycenters[5];
		pointlist primitive = {0, primitive_vertices, primitive_barycenters};
		zfar_clip(clip_primitive, c.zfar, primitive);
		/* perspective division */
		vec3 device_vertices[5];
		for (int i = 0; i < primitive.size; ++i) {
			device_vertices[i].x = primitive.vertices[i].x / primitive.vertices[i].w;
			device_vertices[i].y = primitive.vertices[i].y / primitive.vertices[i].w;
			device_vertices[i].z = primitive.vertices[i].z / primitive.vertices[i].w;
		}
		/* viewport transform */
		for (int i = 0; i < primitive.size; ++i) {
			device_vertices[i].x = device_vertices[i].x * c.width / 2 + c.width / 2;
			device_vertices[i].y = -device_vertices[i].y * c.height / 2 + c.height / 2;
		}
		/* rasterization */
		if (rasterize<false>(c, primitive, device_vertices, zb)) return true;
	}
	return false;
}

inline void draw_instances(const camera& c, shader& s, const instance* const* is, size_t is_size, vec3* canvas) {
	size_t buffer_size = c.width * c.height;
	if (zbuffer.size() != buffer_size) zbuffer.resize(buffer_size);
	if (gbuffer.size() != buffer_size) gbuffer.resize(buffer_size);
	mat<4, 4> vptransform = c.projection * c.viewing;
	std::fill(zbuffer.begin(), zbuffer.end(), 1);
	for (int i = 0; i < is_size; ++i) {
		camera::model_transform(*is[i], s.model);
		s.viewing = c.viewing;
		s.projection = c.projection;
		s.transform = vptransform * s.model;
		s.textures = is[i]->textures.data();
		draw_objects(c, s, *is[i]->object, canvas, zbuffer.data(), gbuffer.data());
	}
}

inline void draw_scene(const camera& c, shader& s, const scene& scene_info, vec3* canvas) {
	draw_instances(c, s, scene_info.get_instances(), scene_info.instance_size(), canvas);
}

class light {
public:
	vec3 color = {1, 1, 1};
	
	static bool intersect(const vec3& a, const vec3& b, const vec3& c, const vec3& o, const vec3& d, float l) {
		vec3 ab = b - a;
		vec3 ac = c - a;
		vec3 ao = o - a;
		vec3 p = d ^ ac;
		vec3 q = ao ^ ab;
		float inverse = 1 / (ab * p);
		float u = d * q * inverse;
		float v = ao * p * inverse;
		float t = ac * q * inverse;
		return t > eps && t < l && u > -eps && v > -eps && u + v < 1 + eps;
	}
	
	static bool intersect(const vec3& a, const vec3& b, const vec3& c, const vec3& o, const vec3& d, float l, vec3& i) {
		vec3 ab = b - a;
		vec3 ac = c - a;
		vec3 ao = o - a;
		vec3 p = d ^ ac;
		vec3 q = ao ^ ab;
		float inverse = 1 / (ab * p);
		float u = d * q * inverse;
		float v = ao * p * inverse;
		float t = ac * q * inverse;
		if (t < eps || t > l || u < -eps || v < -eps || u + v > 1 + eps) return false;
		i.x = 1 - u - v;
		i.y = v;
		i.z = u;
		return true;
	}
	
	virtual vec3 lighting(const vec3& p, const vec3& n) const {
		return {};
	}
	
	virtual vec3 cast(const vec3& p, const vec3& n, const vec3* w, size_t w_size) const {
		return {};
	};
};

class pointlight : public light {
public:
	vec3 position;
	float intensity = 1;
	float decay = .1;
	
	pointlight(const vec3& p, float i = 1, float d = .1) : position(p), intensity(i), decay(d) {}
	
	vec3 lighting(const vec3& p, const vec3& n) const override {
		float distance = position.distance(p);
		vec3 direction = (position - p).normalize();
		return color * fmax(direction * n * intensity / (distance * distance * decay), 0);
	}
	
	vec3 cast(const vec3& p, const vec3& n, const vec3* w, size_t w_size) const override {
		float distance = position.distance(p);
		vec3 direction = (position - p).normalize();
		for (int f = 0; f < w_size; f += 3) {
			if (intersect(w[f], w[f + 1], w[f + 2], p, direction, distance)) return {};
		}
		return color * fmax(direction * n * intensity / (distance * distance * decay), 0);
	}
};

class directionallight : public light {
public:
	vec3 direction;
	float intensity = 1;
	
	directionallight(const vec3& d, float i = 1) : direction(-d), intensity(i) {}
	
	vec3 lighting(const vec3& p, const vec3& n) const override {
		return color * fmax(direction * n * intensity, 0);
	}
	
	vec3 cast(const vec3& p, const vec3& n, const vec3* w, size_t w_size) const override {
		for (int f = 0; f < w_size; f += 3) {
			if (intersect(w[f], w[f + 1], w[f + 2], p, direction, limit)) return {};
		}
		return color * fmax(direction * n * intensity, 0);
	}
	
private:
	static float limit;
};

inline float directionallight::limit = 1000;

class arealight : public light {
public:
	vec3 position;
	vec3 toright;
	vec3 tolower;
	vec3 normal;
	float intensity = 1;
	float area = 0;
	
	arealight(const vec3& p, const vec3& tr, const vec3& tl, float i = 1) : position(p), toright(tr), tolower(tl),
		normal((tl ^ tr).normalize()), area((tl ^ tr).magnitude()), intensity(i) {}
	
	static bool emit(const vec3& o, const vec3& d, const vec3* w, size_t w_size , vec3& p, vec3& n) {
		float minimum = limit;
		vec3 barycenter;
		bool collided = false;
		for (int f = 0; f < w_size; f += 3) {
			if (intersect(w[f], w[f + 1], w[f + 2], o, d, limit, barycenter)) {
				vec3 position = w[f] * barycenter.x + w[f + 1] * barycenter.y + w[f + 2] * barycenter.z;
				if (o.distance(position) < minimum) {
					minimum = o.distance(position);
					p = position;
					n = (w[f + 1] - w[f]) ^ (w[f + 2] - w[f]);
				}
				collided = true;
			}
		}
		if (collided) n = n.normalize();
		return collided;
	}
	
	vec3 cast(const vec3& p, const vec3& n, const vec3* w, size_t w_size) const override {
		vec3 origin = position + toright * random01f() + tolower * random01f();
		float distance = origin.distance(p);
		vec3 direction = (origin - p).normalize();
		for (int f = 0; f < w_size; f += 3) {
			if (intersect(w[f], w[f + 1], w[f + 2], p, direction, distance)) return {};
		}
		return color * fmax(intensity * (direction * n) * -(direction * normal) * area / (distance * distance), 0);
	}
	
private:
	static float limit;
};

inline float arealight::limit = 1000;

inline void lighting(const vec3& r, vec3& c) {
	c.x *= fminf(r.x, 1);
	c.y *= fminf(r.y, 1);
	c.z *= fminf(r.z, 1);
}

inline void vertex_lighting(const camera& c, const light* const* ls, size_t ls_size, vec3* canvas) {
	for (int i = c.width * c.height; i --> 0;) {
		if (zbuffer[i] == 1) continue;
		vec3 radiance;
		for (int l = 0; l < ls_size; ++l) {
			radiance += ls[l]->lighting(gbuffer[i].world_coord, gbuffer[i].normal);
		}
		lighting(radiance, canvas[i]);
	}
}

inline void classic_raytracing(const camera& c, const light* const* ls, size_t ls_size,
							   const vec3* w, size_t w_size, vec3* canvas) {
	for (int i = c.width * c.height; i --> 0;) {
		if (zbuffer[i] == 1) continue;
		vec3 radiance;
		for (int l = 0; l < ls_size; ++l) {
			radiance += ls[l]->cast(gbuffer[i].world_coord, gbuffer[i].normal, w, w_size);
		}
		lighting(radiance, canvas[i]);
	}
}

inline void path_tracing(const camera& c, const light* const* ls, size_t ls_size,
						 const vec3* w, size_t w_size, vec3* canvas) {
	static float brdf = .2f;
	std::vector<const arealight*> lights(ls_size);
	for (size_t i = ls_size; i --> 0;) {
		lights[i] = static_cast<const arealight*>(ls[i]);
	}
	for (int i = c.width * c.height; i --> 0;) {
		if (zbuffer[i] == 1) continue;
		float probability = .6f;
		float coefficient = 1;
		vec3 radiance;
		vec3 position = gbuffer[i].world_coord;
		vec3 normal = gbuffer[i].normal;
		while (true) {
			for (auto& l : lights) {
				radiance += l->cast(position, normal, w, w_size) * coefficient;
			}
			if (probability <= random01f()) break;
			vec3 direction = vec3::random();
			if (direction * normal < 0) direction = -direction;
			coefficient *= direction * normal * brdf / probability;
			if (!arealight::emit(position + direction * eps, direction, w, w_size, position, normal)) break;
		}
		lighting(radiance, canvas[i]);
	}
}

/*
 *    4______________5
 *    /             /|
 *  7/____________6/ |
 *  |  ___________|__|__ x
 *  | /0          | /1
 *  |/____________|/
 *  /3          2
 * z
 */
inline constexpr unsigned int box_triangulated[36] = {
	1, 2, 5, 2, 6, 5, /* left side */
	0, 4, 3, 3, 4, 7, /* right side */
	4, 5, 6, 4, 6, 7, /* upper side */
	0, 2, 1, 0, 3, 2, /* lower side */
	2, 3, 6, 3, 7, 6, /* front side */
	0, 1, 5, 0, 5, 4, /* back side */
};

inline bool boxquery(const camera& c, const vec4* vs, double* zb) {
	static vec3 barycenters[3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
	for (int i = 0; i < 36; i += 3) {
		vec4 vertices[3] = {vs[box_triangulated[i]], vs[box_triangulated[i + 1]], vs[box_triangulated[i + 2]]};
		/* clipping */
		vec4 clip_vertices[4];
		vec3 clip_barycenters[4];
		pointlist clip_primitive = {0, clip_vertices, clip_barycenters};
		znear_clip({3, vertices, barycenters}, c.znear, clip_primitive);
		vec4 primitive_vertices[5];
		vec3 primitive_barycenters[5];
		pointlist primitive = {0, primitive_vertices, primitive_barycenters};
		zfar_clip(clip_primitive, c.zfar, primitive);
		/* perspective division */
		vec3 device_vertices[5];
		for (int i = 0; i < primitive.size; ++i) {
			device_vertices[i].x = primitive.vertices[i].x / primitive.vertices[i].w;
			device_vertices[i].y = primitive.vertices[i].y / primitive.vertices[i].w;
			device_vertices[i].z = primitive.vertices[i].z / primitive.vertices[i].w;
		}
		/* viewport transform */
		for (int i = 0; i < primitive.size; ++i) {
			device_vertices[i].x = device_vertices[i].x * c.width / 2 + c.width / 2;
			device_vertices[i].y = -device_vertices[i].y * c.height / 2 + c.height / 2;
		}
		/* rasterization */
		if (rasterize<false>(c, primitive, device_vertices, zb)) return true;
	}
	return false;
}

}

namespace gl::gpu {

inline constexpr const char* cubemap_vertex_shader = R"(
	#version 410 core

	uniform mat4 view_proj;

	in vec3 vertex;

	out vec3 tex_coord;

	void main() {
		tex_coord = vertex;
		gl_Position = (view_proj * vec4(vertex, 1)).xyww;
	}
)";

inline constexpr const char* cubemap_fragment_shader = R"(
	#version 410 core

	uniform samplerCube box;

	in vec3 tex_coord;

	out vec4 out_color;

	void main() {
		out_color = texture(box, tex_coord);
	}
)";

inline constexpr const char* canvas_vertex_shader = R"(
	#version 410 core

	in vec2 vertex;
	
	out vec2 tex_coord;

	void main() {
		tex_coord = max(vertex, 0);
		gl_Position = vec4(vertex, 0, 1);
	}
)";

inline constexpr const char* canvas_fragment_shader = R"(
	#version 410 core

	uniform sampler2D canvas;

	in vec2 tex_coord;

	out vec4 out_color;

	void main() {
		out_color = texture(canvas, tex_coord);
	}
)";

inline constexpr const char* framebuffer_vertex_shader = R"(
	#version 410 core

	in vec2 vertex;
	
	out vec2 tex_coord;

	void main() {
		tex_coord = max(vertex, 0);
		gl_Position = vec4(vertex, 0, 1);
	}
)";

inline constexpr const char* framebuffer_fragment_shader = R"(
	#version 410 core

	uniform sampler2D framebuffer;

	in vec2 tex_coord;

	out vec4 out_color;

	void main() {
		out_color = texture(framebuffer, tex_coord);
	}
)";

/*
 *	---- pipeline --------------------------------------------------------------
 *
 *	preload_vertexobject(s, o)
 *	preload_texture(t)
 *	shader::load(vs, fs)				|	shader::read(vs, fs)
 *
 *	shader::use()						|	draw_instances(c, s, is, is_size)
 *	shader::uniform...(n, v)			|		or draw_scene(c, s, scene_info)
 *	load_texture(s, t, i)				|
 *	draw_object(vo)						|
 *
 *	unload_vertexobject(vo)
 *	unload_texture(t)
 *	shader::unload()
 *
 *	---- cubemap ---------------------------------------------------------------
 *
 *	write texture into cubemap[0 - 6]
 *	cubemap::custom_shader.load(vs, fs)	|	use default shader
 *	cubemap::preload()
 *
 *	cubemap::draw(c)
 *
 *	cubemap::unload()
 *
 *	---- canvas ----------------------------------------------------------------
 *
 *	write texture into canvas::surface
 *	canvas::custom_shader::load(vs, fs)	|	use default shader
 *	canvas::preload()
 *
 *	canvas::draw()
 *
 *	canvas::unload()
 *
 *	---- framebuffer -----------------------------------------------------------
 *
 *	framebuffer::custom_shader::load()	|	use default shader
 *	framebuffer::preload(s, b)
 *
 *	framebuffer::bind()
 *	framebuffer::clear()
 *	draw_instances(c, s, is, is_size)	|	draw(...)
 *	framebuffer::unbind()
 *	framebuffer::draw()
 *
 *	framebuffer::unload()
 */

/* texture filter type */
enum {
	TEXTURE_NEAREST = 0,
	TEXTURE_LINEAR = 1,
};

/* texture mipmap type */
enum {
	TEXTURE_NO_MIPMAP,
	TEXTURE_MIPMAP_NEAREST,
	TEXTURE_MIPMAP_LINEAR,
};

/* texture wrapping type */
enum {
	TEXTURE_REPEAT,
	TEXTURE_MIRRORED_REPEAT,
	TEXTURE_CLAMP_TO_EDGE,
	TEXTURE_CLAMP_TO_BORDER,
};

/* texture internal format */
enum {
	TEXTURE_8 = 0,
	TEXTURE_16 = 1,
	TEXTURE_16F = 2,
	TEXTURE_32F = 3,
	TEXTURE_8I = 4,
	TEXTURE_16I = 5,
	TEXTURE_32I = 6,
	TEXTURE_8UI = 7,
	TEXTURE_16UI = 8,
	TEXTURE_32UI = 9,
};

class shader {
public:
	shader() = default;
	
	bool loaded() const {
		return program != 0;
	}
	
	void unload() const {
		glDeleteProgram(program);
	}
	
	void use() const {
		glUseProgram(program);
	}
	
	/**
	 * \param vs vertex shader
	 * \param fs fragment shader
	 * \param gs geometry shader
	 */
	void load(const char* vs, const char* fs, const char* gs = nullptr) {
		if (program == 0) program = glCreateProgram();
		GLuint vertex_shader = add_shader(vs, GL_VERTEX_SHADER);
		GLuint fragment_shader = add_shader(fs, GL_FRAGMENT_SHADER);
		if (gs) {
			GLuint geometry_shader = add_shader(gs, GL_GEOMETRY_SHADER);
			glLinkProgram(program);
			glDeleteShader(vertex_shader);
			glDeleteShader(fragment_shader);
			glDeleteShader(geometry_shader);
		} else {
			glLinkProgram(program);
			glDeleteShader(vertex_shader);
			glDeleteShader(fragment_shader);
		}
		link_info();
	}
	
	/**
	 * \param vs vertex shader
	 * \param fs fragment shader
	 * \param gs geometry shader
	 */
	void load(const std::string* vs, const std::string* fs, const std::string* gs = nullptr) {
		load(vs->c_str(), fs->c_str(), gs ? gs->c_str() : nullptr);
	}
	
	/**
	 * \param vs vertex shader file
	 * \param fs fragment shader file
	 * \param gs geometry shader file
	 */
	int read(const std::string* vs, const std::string* fs, const std::string* gs = nullptr) {
		int status = 0;
		std::string vertex_source;
		std::string fragment_source;
		status |= read_file(*vs, vertex_source);
		status |= read_file(*fs, fragment_source);
		if (gs) {
			std::string geometry_source;
			read_file(*gs, geometry_source);
			load(vertex_source.c_str(), fragment_source.c_str(), geometry_source.c_str());
		} else {
			load(vertex_source.c_str(), fragment_source.c_str());
		}
		return status;
	}
	
	/**
	 * \param n variable name
	 * \param v value
	 */
	void uniform_int(const std::string& n, int v) const {
		glUniform1i(glGetUniformLocation(program, n.c_str()), v);
	}
	
	/**
	 * \param n variable name
	 * \param v value
	 */
	void uniform_uint(const std::string& n, unsigned int v) const {
		glUniform1ui(glGetUniformLocation(program, n.c_str()), v);
	}
	
	/**
	 * \param n variable name
	 * \param v value
	 */
	void uniform_float(const std::string& n, float v) const {
		glUniform1f(glGetUniformLocation(program, n.c_str()), v);
	}
	
	/**
	 * \param n variable name
	 * \param v value
	 */
	void uniform_vec2(const std::string& n, const vec2& v) const {
		glUniform2fv(glGetUniformLocation(program, n.c_str()), 1, &v.x);
	}
	
	/**
	 * \param n variable name
	 * \param v value
	 */
	void uniform_vec3(const std::string& n, const vec3& v) const {
		glUniform3fv(glGetUniformLocation(program, n.c_str()), 1, &v.x);
	}
	
	/**
	 * \param n variable name
	 * \param v value
	 */
	void uniform_vec4(const std::string& n, const vec4& v) const {
		glUniform4fv(glGetUniformLocation(program, n.c_str()), 1, &v.x);
	}

	/**
	 * \param n variable name
	 * \param v value
	 */
	void uniform_mat2(const std::string& n, const mat<2, 2>& v) const {
		glUniformMatrix2fv(glGetUniformLocation(program, n.c_str()), 1, GL_TRUE, v[0]);
	}

	/**
	 * \param n variable name
	 * \param v value
	 */
	void uniform_mat3(const std::string& n, const mat<3, 3>& v) const {
		glUniformMatrix3fv(glGetUniformLocation(program, n.c_str()), 1, GL_TRUE, v[0]);
	}
	
	/**
	 * \param n variable name
	 * \param v value
	 */
	void uniform_mat4(const std::string& n, const mat<4, 4>& v) const {
		glUniformMatrix4fv(glGetUniformLocation(program, n.c_str()), 1, GL_TRUE, v[0]);
	}
	
	friend inline void preload_vertexobject(const shader& s, vertexobject& vo);
	
protected:
	GLuint program = 0;
	
	GLuint add_shader(const char* s, GLint t) const {
		GLuint new_shader = glCreateShader(t);
		glShaderSource(new_shader, 1, &s, nullptr);
		glCompileShader(new_shader);
		compile_info(new_shader, t);
		glAttachShader(program, new_shader);
		return new_shader;
	 }
	
	void compile_info(GLuint s, GLuint t) const {
		GLint success;
		glGetShaderiv(s, GL_COMPILE_STATUS, &success);
		if (success) return;
		if (t == GL_VERTEX_SHADER) {
			std::cerr << "[Vertex shader compile error] ";
		} else if (t == GL_GEOMETRY_SHADER) {
			std::cerr << "[Geometry shader compile error] ";
		} else if (t == GL_FRAGMENT_SHADER) {
			std::cerr << "[Fragment shader compile error] ";
		}
		GLchar info[1024];
		glGetShaderInfoLog(s, 1024, nullptr, info);
		std::cerr << info;
	}
	
	void link_info() const {
		GLint success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (success) return;
		std::cerr << "[Link error] ";
		GLchar info[1024];
		glGetProgramInfoLog(program, 1024, nullptr, info);
		std::cerr << info;
	}
};

inline constexpr GLint texture_formats[4] = {GL_RED, GL_RG, GL_RGB, GL_RGBA};

inline constexpr GLint texture_internal_formats[10][4] = {
	{GL_R8,		GL_RG8,		GL_RGB8,	GL_RGBA8},		/* TEXTURE_8 */
	{GL_R16, 	GL_RG16,	GL_RGB16,	GL_RGBA16},		/* TEXTURE_16 */
	{GL_R16F,	GL_RG16F,	GL_RGB16F,	GL_RGBA16F},	/* TEXTURE_16F */
	{GL_R32F,	GL_RG32F,	GL_RGB32F,	GL_RGBA32F},	/* TEXTURE_32F */
	{GL_R8I,	GL_RG8I,	GL_RGB8I,	GL_RGBA8I},		/* TEXTURE_8I */
	{GL_R16I,	GL_RG16I,	GL_RGB16I,	GL_RGBA16I},	/* TEXTURE_16I */
	{GL_R32I,	GL_RG32I,	GL_RGB32I,	GL_RGBA32I},	/* TEXTURE_32I */
	{GL_R8UI,	GL_RG8UI,	GL_RGB8UI,	GL_RGBA8UI},	/* TEXTURE_8I */
	{GL_R16UI,	GL_RG16UI,	GL_RGB16UI,	GL_RGBA16UI},	/* TEXTURE_16I */
	{GL_R32UI,	GL_RG32UI,	GL_RGB32UI,	GL_RGBA32UI},	/* TEXTURE_32I */
};

/**
 * \param b texture bytes
 */
inline GLint texture_format(int b) {
	if (b <= 0 || b > 4) {
		error::set(ERROR_UNSOPPORTED, "Illegal argument format");
		return -1;
	}
	return texture_formats[b - 1];
}

/**
 * \param f texture internal format
 * \param b texture bytes
 */
inline GLint texture_internal_format(int f, int b) {
	if (f < 0 || f >= 10 || b <= 0 || b > 4) {
		error::set(ERROR_UNSOPPORTED, "Illegal argument format");
		return -1;
	}
	return texture_internal_formats[f][b - 1];
}

/**
 * Preload vertexobject before rendering.
 *
 * \param s target shader
 * \param vo vertexobject
 */
inline void preload_vertexobject(const shader& s, vertexobject& vo) {
	if (vo.id != 0) error::set(ERROR_UNSOPPORTED, "Vertexobject has been preloaded");
	auto& data = vo.data;
	auto& size = vo.size;
	auto& name = vo.name;
	auto& buffer = vo.buffer_id;
	int data_size = static_cast<int>(data.size());
	buffer.resize(data_size);
	glGenVertexArrays(1, &vo.id);
	glBindVertexArray(vo.id);
	glGenBuffers(data_size, buffer.data());
	for (int i = 0; i < data_size; ++i) {
		glBindBuffer(GL_ARRAY_BUFFER, buffer[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data[i].size(), data[i].data(), GL_STATIC_DRAW);
		GLint attrib = glGetAttribLocation(s.program, name[i].c_str());
		if (attrib == -1) error::set(ERROR_UNKNOWN, "Failed to find attribute name");
		glEnableVertexAttribArray(attrib);
		glVertexAttribPointer(attrib, size[i], GL_FLOAT, GL_FALSE, sizeof(float) * size[i], static_cast<void*>(0));
	}
}

/**
 * Preload vertexobject before rendering.
 *
 * \param vo vertexobject
 */
inline void preload_vertexobject(vertexobject& vo) {
	if (vo.id != 0) error::set(ERROR_UNSOPPORTED, "Vertexobject has been preloaded");
	auto& data = vo.data;
	auto& size = vo.size;
	auto& buffer = vo.buffer_id;
	int data_size = static_cast<int>(data.size());
	buffer.resize(data_size);
	glGenVertexArrays(1, &vo.id);
	glBindVertexArray(vo.id);
	glGenBuffers(data_size, &buffer.front());
	for (int i = 0; i < data_size; ++i) {
		glBindBuffer(GL_ARRAY_BUFFER, buffer[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data[i].size(), data[i].data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, size[i], GL_FLOAT, GL_FALSE, sizeof(float) * size[i], static_cast<void*>(0));
	}
}

/**
 * Unload vertexobject.
 *
 * \param vo vertexobject
 */
inline void unload_vertexobject(vertexobject& vo) {
	if (vo.id == 0) error::set(ERROR_UNSOPPORTED, "Vertexobject has not been preloaded");
	glDeleteVertexArrays(1, &vo.id);
	glDeleteBuffers(static_cast<int>(vo.buffer_id.size()), vo.buffer_id.data());
	vo.id = 0;
}

/**
 * Preload texture before rendering.
 *
 * \param t texture
 * \param w wrapping setting
 * \param mag magnifying filter
 * \param min minifying filter
 * \param mip mipmap setting
 */
inline void preload_texture(texture& t, int w = TEXTURE_CLAMP_TO_EDGE, int mag = TEXTURE_LINEAR,
							int min = TEXTURE_NEAREST, int mip = TEXTURE_NO_MIPMAP) {
	if (t.id != 0) error::set(ERROR_UNSOPPORTED, "Texture has been preloaded");
	GLint format = texture_format(t.bytes);
	glGenTextures(1, &t.id);
	glBindTexture(GL_TEXTURE_2D, t.id);
	glTexImage2D(GL_TEXTURE_2D, 0, format, t.width, t.height, 0, format, GL_UNSIGNED_BYTE, &t.data.front());
	if (w == TEXTURE_REPEAT) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	} else if (w == TEXTURE_MIRRORED_REPEAT) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	} else if (w == TEXTURE_CLAMP_TO_EDGE) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	} else if (w == TEXTURE_CLAMP_TO_BORDER) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST + mag);
	if (mip == TEXTURE_NO_MIPMAP) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST + min);
	} else if (mip == TEXTURE_MIPMAP_NEAREST) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST + min);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else if (mip == TEXTURE_MIPMAP_LINEAR) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR + min);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
}

/**
 * Unload texture.
 *
 * \param t texture
 */
inline void unload_texture(texture& t) {
	if (t.id == 0) error::set(ERROR_UNSOPPORTED, "Texture has not been preloaded");
	glDeleteTextures(1, &t.id);
	t.id = 0;
}

/**
 * Load texture when rendering.
 *
 * \param s target shader
 * \param t texture
 * \param i index
 */
inline void load_texture(const shader& s, const texture& t, int i) {
	if (t.id == 0) error::set(ERROR_UNSOPPORTED, "Texture has not been preloaded");
	/* if texture is named */
	if (!t.name.empty()) s.uniform_int(t.name, i);
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, t.id);
}

/**
 * Load texture when rendering.
 *
 * \param t texture
 * \param i index
 */
inline void load_texture(const texture& t, int i) {
	if (t.id == 0) error::set(ERROR_UNSOPPORTED, "Texture has not been preloaded");
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, t.id);
}

/**
 * \param vo vertexobject
 */
inline void draw_objects(const vertexobject& vo) {
	if (vo.id == 0) error::set(ERROR_UNSOPPORTED, "Vertexobject has not preloaded");
	glBindVertexArray(vo.id);
	glDrawArrays(GL_TRIANGLES, 0, vo.length);
}

/**
 * \param c camrea info
 * \param s shader
 * \param is instances
 * \param is_size size of instances
 */
inline void draw_instances(const camera& c, const shader& s, const instance* const* is, size_t is_size) {
	mat<4, 4> viewing_projection = c.projection * c.viewing;
	mat<4, 4> model;
	s.use();
	for (int i = 0; i < is_size; ++i) {
		camera::model_transform(*is[i], model);
		mat<4, 4> transform = viewing_projection * model;
		s.uniform_mat4("model", model);
		s.uniform_mat4("view", c.viewing);
		s.uniform_mat4("proj", c.projection);
		s.uniform_mat4("model_view_proj", transform);
		s.uniform_mat4("inv_view_proj", mat4x4_inverse(viewing_projection));
		for (int t = 0; t < is[i]->textures.size(); ++t) {
			load_texture(s, *is[i]->textures[t], t);
		}
		draw_objects(*is[i]->object);
	}
}

/**
 * \param c camrea info
 * \param s shader
 * \param scene_info scene info
 */
inline void draw_scene(const camera& c, shader& s, const scene& scene_info) {
	draw_instances(c, s, scene_info.get_instances(), scene_info.instance_size());
}

class cubemap {
public:
	/*
	 * 0: left side
	 * 1: right side
	 * 2: upper side
	 * 3: lower side
	 * 4: front side
	 * 5: back side
	 */
	texture faces[6];
	shader custom_shader;
	
	cubemap() = default;
	
	texture& operator[](size_t k) {
		return faces[k];
	}
	
	bool loaded() const {
		return texture_id != 0;
	}
	
	/**
	 * \param mag magnifying filter
	 * \param min minifying filter
	 */
	void preload(int mag = TEXTURE_LINEAR, int min = TEXTURE_NEAREST) {
		GLint format = texture_format(faces[0].bytes);
		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
		for (int i = 0; i < 6; ++i) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, faces[i].width, faces[i].height,
						 0, format, GL_UNSIGNED_BYTE, faces[i].data.data());
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST + mag); /* GL_NEAREST or GL_LINEAR */
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST + min); /* GL_NEAREST or GL_LINEAR */
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
	
	void unload() {
		glDeleteTextures(1, &texture_id);
		texture_id = 0;
	}
	
	/**
	 * \param c camera info
	 */
	void draw(const camera& c) const {
		glDepthFunc(GL_LEQUAL);
		mat<4, 4> viewing = c.viewing;
		viewing[0][3] = 0;
		viewing[1][3] = 0;
		viewing[2][3] = 0;
		const shader& available_shader = custom_shader.loaded() ? custom_shader : default_shader;
		available_shader.use();
		available_shader.uniform_mat4("view_proj", c.projection * viewing);
		available_shader.uniform_int("box", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
		draw_objects(cube_object);
		glDepthFunc(GL_LESS);
	}
	
	friend inline void init();
	
private:
	GLuint texture_id = 0;
	static vertexobject cube_object;
	static shader default_shader;
};

inline vertexobject cubemap::cube_object = {
	0,										/* id */
	36,										/* length */
	{3},									/* size */
	{{"vertex"}},							/* name */
	{0},									/* buffer_id */
	{{										/* data */
		-1,  1, -1, -1, -1, -1,  1, -1, -1,
		 1, -1, -1,  1,  1, -1, -1,  1, -1, /* back side */
		-1, -1,  1, -1, -1, -1, -1,  1, -1,
		-1,  1, -1, -1,  1,  1, -1, -1,  1,	/* left side */
		 1, -1, -1,  1, -1,  1,  1,  1,  1,
		 1,  1,  1,  1,  1, -1,  1, -1, -1,	/* right side */
		-1, -1,  1, -1,  1,  1,  1,  1,  1,
		 1,  1,  1,  1, -1,  1, -1, -1,  1,	/* front side */
		-1,  1, -1,  1,  1, -1,  1,  1,  1,
		 1,  1,  1, -1,  1,  1, -1,  1, -1,	/* upper side */
		-1, -1, -1, -1, -1,  1,  1, -1, -1,
		 1, -1, -1, -1, -1,  1,  1, -1,  1,	/* lower side */
	}},
};

inline shader cubemap::default_shader;

class canvas {
public:
	texture surface;
	shader custom_shader;
	
	canvas() = default;
	
	bool loaded() const {
		return surface.id == 0;
	}
	
	/**
	 * \param mag magnifying filter
	 * \param min minifying filter
	 */
	void preload(int mag = TEXTURE_LINEAR, int min = TEXTURE_NEAREST) {
		preload_texture(surface, TEXTURE_CLAMP_TO_EDGE, mag, min, TEXTURE_NO_MIPMAP);
	}
	
	void unload() {
		unload_texture(surface);
	}
	
	void draw() const {
		const shader& available_shader = custom_shader.loaded() ? custom_shader : default_shader;
		available_shader.use();
		available_shader.uniform_int("canvas", 0);
		load_texture(surface, 0);
		draw_objects(canvas_object);
	}
	
	friend inline void init();
	
private:
	static vertexobject canvas_object;
	static shader default_shader;
};

inline vertexobject canvas::canvas_object = {
	0,												/* id */
	6,												/* length */
	{2},											/* size */
	{{"vertex"}},									/* name */
	{0},											/* buffer_id */
	{{1, 1, -1, 1, 1, -1, -1, -1, 1, -1, -1, 1}},	/* data */
};

inline shader canvas::default_shader;

class framebuffer {
public:
	int width = 0;
	int height = 0;
	bool depth_test = true;
	bool stencil_test = true;
	bool buffer_readonly = true;
	shader custom_shader;
	
	framebuffer() = default;
	
	/**
	 * \param w render width
	 * \param h render height
	 */
	framebuffer(int w, int h) : width(w), height(h) {}
	
	static void unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	static void clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}
	
	bool loaded() const {
		return framebuffer_id != 0;
	}
	
	/**
	 * \param s texture size
	 * \param b texture bytes
	 * \param fs texture internal formats
	 */
	void preload(int s, int b, int* fs = nullptr) {
		glGenFramebuffers(1, &framebuffer_id);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
		/* color attachment */
		buffers.resize(s);
		glGenTextures(s, buffers.data());
		std::vector<GLuint> attachments(s);
		GLuint format = texture_format(b);
		for (int i = 0; i < s; ++i) {
			attachments[i] = GL_COLOR_ATTACHMENT0 + i;
			glBindTexture(GL_TEXTURE_2D, buffers[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, fs ? texture_internal_format(fs[i], b) : format,
						 width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[i], GL_TEXTURE_2D, buffers[i], 0);
		}
		glDrawBuffers(s, attachments.data());
		/* depth stencil */
		if (!depth_test) return;
		if (buffer_readonly) {
			glGenRenderbuffers(1, &depth_stencil_id);
			glBindRenderbuffer(GL_RENDERBUFFER, depth_stencil_id);
			if (stencil_test) {
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
										  GL_RENDERBUFFER, depth_stencil_id);
			} else {
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
										  GL_RENDERBUFFER, depth_stencil_id);
			}
		} else {
			glGenTextures(1, &depth_stencil_id);
			glBindTexture(GL_TEXTURE_2D, depth_stencil_id);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			if (stencil_test) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_STENCIL, width, height,
							 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth_stencil_id, 0);
			} else {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height,
							 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_stencil_id, 0);
			}
		}
	}
	
	void unload() {
		glDeleteFramebuffers(1, &framebuffer_id);
		glDeleteTextures(static_cast<int>(buffers.size()), buffers.data());
		glDeleteRenderbuffers(1, &depth_stencil_id);
	}
	
	void bind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
		depth_test ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
		stencil_test ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
	}
	
	void draw() const {
		custom_shader.loaded() ? custom_shader.use() : default_shader.use();
		for (int i = 0; i < buffers.size(); ++i) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, buffers[i]);
		}
		draw_objects(framebuffer_object);
	}
	
	friend inline void init();
	
private:
	GLuint framebuffer_id = 0;
	GLuint depth_stencil_id = 0;
	std::vector<GLuint> buffers;
	static vertexobject framebuffer_object;
	static shader default_shader;
};

inline vertexobject framebuffer::framebuffer_object = {
	0,												/* id */
	6,												/* length */
	{2},											/* size */
	{{"vertex"}},									/* name */
	{0},											/* buffer_id */
	{{1, 1, -1, 1, 1, -1, -1, -1, 1, -1, -1, 1}},	/* data */
};

inline shader framebuffer::default_shader;

inline void init() {
	cubemap::default_shader.load(cubemap_vertex_shader, cubemap_fragment_shader);
	preload_vertexobject(cubemap::default_shader, cubemap::cube_object);
	canvas::default_shader.load(canvas_vertex_shader, canvas_fragment_shader);
	preload_vertexobject(canvas::default_shader, canvas::canvas_object);
	framebuffer::default_shader.load(framebuffer_vertex_shader, framebuffer_fragment_shader);
	preload_vertexobject(framebuffer::default_shader, framebuffer::framebuffer_object);
}

}
