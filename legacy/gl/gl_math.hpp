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

#ifndef GL_MATH_HPP
#define GL_MATH_HPP

#include "gl_stdinc.hpp"

namespace gl {

inline float random01f();

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

inline vec2f operator+(const vec2f& v1, const vec2f& v2);

inline vec2f operator-(const vec2f& v1, const vec2f& v2);

inline vec2f operator*(const vec2f& v1, float v2);

inline vec2f operator*(float v1, const vec2f& v2);

inline vec2f operator/(const vec2f& v1, float v2);

inline float operator*(const vec2f& v1, const vec2f& v2);

inline float operator^(const vec2f& v1, const vec2f& v2);

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

inline vec3f operator+(const vec3f& v1, const vec3f& v2);

inline vec3f operator-(const vec3f& v1, const vec3f& v2);

inline vec3f operator*(const vec3f& v1, float v2);

inline vec3f operator*(float v1, const vec3f& v2);

inline vec3f operator/(const vec3f& v1, float v2);

inline float operator*(const vec3f& v1, const vec3f& v2);

inline vec3f operator^(const vec3f& v1, const vec3f& v2);

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

inline vec4f operator+(const vec4f& v1, const vec4f& v2);

inline vec4f operator-(const vec4f& v1, const vec4f& v2);

inline vec4f operator*(const vec4f& v1, float v2);

inline vec4f operator*(float v1, const vec4f& v2);

inline vec4f operator/(const vec4f& v1, float v2);

inline float operator*(const vec4f& v1, const vec4f& v2);

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
matf<r, c> operator+(const matf<r, c>& v1, const matf<r, c>& v2);

template <int r, int c>
matf<r, c> operator-(const matf<r, c>& v1, const matf<r, c>& v2);

template <int r, int c>
matf<r, c> operator*(const matf<r, c>& v1, float v2);

template <int r, int c>
matf<r, c> operator*(float v1, const matf<r, c>& v2);

template <int l1, int l2, int l3>
matf<l1, l3> operator*(const matf<l1, l2>& v1, const matf<l2, l3>& v2);

template <int r>
matf<r, 1> operator*(const matf<r, 2>& v1, const vec2f& v2);

template <int r>
matf<r, 1> operator*(const matf<r, 3>& v1, const vec3f& v2);

template <int r>
matf<r, 1> operator*(const matf<r, 4>& v1, const vec4f& v2);

template <int r, int c>
matf<r, c> operator/(const matf<r, c>& v1, float v2);

inline float mat2x2_determinant(const matf<2, 2>& m);

inline float mat3x3_determinant(const matf<3, 3>& m);

inline float mat4x4_determinant(const matf<4, 4>& m);

inline matf<2, 2> mat2x2_inverse(const matf<2, 2>& m);

inline matf<3, 3> mat3x3_inverse(const matf<3, 3>& m);

inline matf<4, 4> mat4x4_inverse(const matf<4, 4>& m);

inline double random01d();

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

inline vec2d operator+(const vec2d& v1, const vec2d& v2);

inline vec2d operator-(const vec2d& v1, const vec2d& v2);

inline vec2d operator*(const vec2d& v1, double v2);

inline vec2d operator*(double v1, const vec2d& v2);

inline vec2d operator/(const vec2d& v1, double v2);

inline double operator*(const vec2d& v1, const vec2d& v2);

inline double operator^(const vec2d& v1, const vec2d& v2);

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

inline vec3d operator+(const vec3d& v1, const vec3d& v2);

inline vec3d operator-(const vec3d& v1, const vec3d& v2);

inline vec3d operator*(const vec3d& v1, double v2);

inline vec3d operator*(double v1, const vec3d& v2);

inline vec3d operator/(const vec3d& v1, double v2);

inline double operator*(const vec3d& v1, const vec3d& v2);

inline vec3d operator^(const vec3d& v1, const vec3d& v2);

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

inline vec4d operator+(const vec4d& v1, const vec4d& v2);

inline vec4d operator-(const vec4d& v1, const vec4d& v2);

inline vec4d operator*(const vec4d& v1, double v2);

inline vec4d operator*(double v1, const vec4d& v2);

inline vec4d operator/(const vec4d& v1, double v2);

inline double operator*(const vec4d& v1, const vec4d& v2);

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
matd<r, c> operator+(const matd<r, c>& v1, const matd<r, c>& v2);

template <int r, int c>
matd<r, c> operator-(const matd<r, c>& v1, const matd<r, c>& v2);

template <int r, int c>
matd<r, c> operator*(const matd<r, c>& v1, double v2);

template <int r, int c>
matd<r, c> operator*(double v1, const matd<r, c>& v2);

template <int l1, int l2, int l3>
matd<l1, l3> operator*(const matd<l1, l2>& v1, const matd<l2, l3>& v2);

template <int r>
matd<r, 1> operator*(const matd<r, 2>& v1, const vec2d& v2);

template <int r>
matd<r, 1> operator*(const matd<r, 3>& v1, const vec3d& v2);

template <int r>
matd<r, 1> operator*(const matd<r, 4>& v1, const vec4d& v2);

template <int r, int c>
matd<r, c> operator/(const matd<r, c>& v1, double v2);

inline double mat2x2_determinant(const matd<2, 2>& m);

inline double mat3x3_determinant(const matd<3, 3>& m);

inline double mat4x4_determinant(const matd<4, 4>& m);

inline matd<2, 2> mat2x2_inverse(const matd<2, 2>& m);

inline matd<3, 3> mat3x3_inverse(const matd<3, 3>& m);

inline matd<4, 4> mat4x4_inverse(const matd<4, 4>& m);

}

#include "gl_math.inl"

#endif
