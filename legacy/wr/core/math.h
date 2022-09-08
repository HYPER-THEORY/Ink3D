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

#include <algorithm>
#include <cmath>

inline float randomf();

class vec2f {
public:
	float x = 0;
	float y = 0;
	
	vec2f() = default;
	
	vec2f(float x);
	
	vec2f(float x, float y);
	
	vec2f(const vec2f& v);
	
	vec2f operator-() const;
	
	void operator+=(const vec2f& v);
	
	void operator-=(const vec2f& v);
	
	void operator*=(float v);
	
	void operator/=(float v);
	
	float magnitude() const;
	
	float distance(const vec2f& v) const;
	
	vec2f normalize() const;
	
	vec2f rotate(float a) const;
	
	static vec2f random();
};

vec2f operator+(const vec2f& v1, const vec2f& v2);

vec2f operator-(const vec2f& v1, const vec2f& v2);

vec2f operator*(const vec2f& v1, float v2);

vec2f operator*(float v1, const vec2f& v2);

vec2f operator/(const vec2f& v1, float v2);

float operator*(const vec2f& v1, const vec2f& v2);

float operator^(const vec2f& v1, const vec2f& v2);

using vec2 = vec2f;

class vec3f {
public:
	float x = 0;
	float y = 0;
	float z = 0;
	
	vec3f() = default;
	
	vec3f(float x);
	
	vec3f(float x, float y, float z);
	
	vec3f(const vec2f& v, float z);
	
	vec3f(float x, const vec2f& v);
	
	vec3f(const vec3f& v);
	
	vec3f operator-() const;
	
	void operator+=(const vec3f& v);
	
	void operator-=(const vec3f& v);
	
	void operator*=(float v);
	
	void operator/=(float v);
	
	float magnitude() const;
	
	float distance(const vec3f& v) const;
	
	vec3f normalize() const;
	
	vec3f rotate(const vec3f& v, float a) const;
	
	static vec3f random();
};

vec3f operator+(const vec3f& v1, const vec3f& v2);

vec3f operator-(const vec3f& v1, const vec3f& v2);

vec3f operator*(const vec3f& v1, float v2);

vec3f operator*(float v1, const vec3f& v2);

vec3f operator/(const vec3f& v1, float v2);

float operator*(const vec3f& v1, const vec3f& v2);

vec3f operator^(const vec3f& v1, const vec3f& v2);

using vec3 = vec3f;

class vec4f {
public:
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 0;
	
	vec4f() = default;
	
	vec4f(float x);
	
	vec4f(float x, float y, float z, float w);
	
	vec4f(const vec2f& v, float z, float w);
	
	vec4f(float x, const vec2f& v, float w);
	
	vec4f(float x, float y, const vec2f& v);
	
	vec4f(const vec2f& v1, const vec2f& v2);
	
	vec4f(const vec3f& v, float w);
	
	vec4f(float x, const vec3f& v);
	
	vec4f(const vec4f& v);
	
	vec4f operator-() const;
	
	void operator+=(const vec4f& v);
	
	void operator-=(const vec4f& v);
	
	void operator*=(float v);
	
	void operator/=(float v);
	
	float magnitude() const;
	
	float distance(const vec4f& v) const;
	
	vec4f normalize() const;
};

vec4f operator+(const vec4f& v1, const vec4f& v2);

vec4f operator-(const vec4f& v1, const vec4f& v2);

vec4f operator*(const vec4f& v1, float v2);

vec4f operator*(float v1, const vec4f& v2);

vec4f operator/(const vec4f& v1, float v2);

float operator*(const vec4f& v1, const vec4f& v2);

using vec4 = vec4f;

template <int r, int c>
class matf {
public:
	float m[r][c];
	
	float* operator[](size_t k);
	
	const float* operator[](size_t k) const;
	
	void operator+=(const matf<r, c>& v);

	void operator-=(const matf<r, c>& v);

	void operator*=(float v);

	void operator/=(float v);
	
	operator vec2f();
	
	operator vec3f();
	
	operator vec4f();
	
	static matf<r, c> identity(int v);
	
	matf<c, r> transpose() const;
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

template<int r, int c>
using mat = matf<r, c>;

using mat2 = matf<2, 2>;

using mat3 = matf<3, 3>;

using mat4 = matf<4, 4>;

float mat2_determinant(const mat2& m);

float mat3_determinant(const mat3& m);

float mat4_determinant(const mat4& m);

mat2 mat2_inverse(const mat2& m);

mat3 mat3_inverse(const mat3& m);

mat4 mat4_inverse(const mat4& m);

inline double randomd();

class vec2d {
public:
	double x = 0;
	double y = 0;
	
	vec2d() = default;
	
	vec2d(double x);
	
	vec2d(double x, double y);
	
	vec2d(const vec2d& v);
	
	vec2d operator-() const;
	
	void operator+=(const vec2d& v);
	
	void operator-=(const vec2d& v);
	
	void operator*=(double v);
	
	void operator/=(double v);
	
	double magnitude() const;
	
	double distance(const vec2d& v) const;
	
	vec2d normalize() const;
	
	vec2d rotate(double a) const;
	
	static vec2d randomunit();
};

vec2d operator+(const vec2d& v1, const vec2d& v2);

vec2d operator-(const vec2d& v1, const vec2d& v2);

vec2d operator*(const vec2d& v1, double v2);

vec2d operator*(double v1, const vec2d& v2);

vec2d operator/(const vec2d& v1, double v2);

double operator*(const vec2d& v1, const vec2d& v2);

double operator^(const vec2d& v1, const vec2d& v2);

class vec3d {
public:
	double x = 0;
	double y = 0;
	double z = 0;
	
	vec3d() = default;
	
	vec3d(double x);
	
	vec3d(double x, double y, double z);
	
	vec3d(const vec2d& v, double z);
	
	vec3d(double x, const vec2d& v);
	
	vec3d(const vec3d& v);
	
	vec3d operator-() const;
	
	void operator+=(const vec3d& v);
	
	void operator-=(const vec3d& v);
	
	void operator*=(double v);
	
	void operator/=(double v);
	
	double magnitude() const;
	
	double distance(const vec3d& v) const;
	
	vec3d normalize() const;
	
	vec3d rotate(const vec3d& v, double a) const;
	
	static vec3d randomunit();
};

vec3d operator+(const vec3d& v1, const vec3d& v2);

vec3d operator-(const vec3d& v1, const vec3d& v2);

vec3d operator*(const vec3d& v1, double v2);

vec3d operator*(double v1, const vec3d& v2);

vec3d operator/(const vec3d& v1, double v2);

double operator*(const vec3d& v1, const vec3d& v2);

vec3d operator^(const vec3d& v1, const vec3d& v2);

class vec4d {
public:
	double x = 0;
	double y = 0;
	double z = 0;
	double w = 0;
	
	vec4d() = default;
	
	vec4d(double x);
	
	vec4d(double x, double y, double z, double w);
	
	vec4d(const vec2d& v, double z, double w);
	
	vec4d(double x, const vec2d& v, double w);
	
	vec4d(double x, double y, const vec2d& v);
	
	vec4d(const vec2d& v1, const vec2d& v2);
	
	vec4d(const vec3d& v, double w);
	
	vec4d(double x, const vec3d& v);
	
	vec4d(const vec4d& v);
	
	vec4d operator-() const;
	
	void operator+=(const vec4d& v);
	
	void operator-=(const vec4d& v);
	
	void operator*=(double v);
	
	void operator/=(double v);
	
	double magnitude() const;
	
	double distance(const vec4d& v) const;
	
	vec4d normalize() const;
};

vec4d operator+(const vec4d& v1, const vec4d& v2);

vec4d operator-(const vec4d& v1, const vec4d& v2);

vec4d operator*(const vec4d& v1, double v2);

vec4d operator*(double v1, const vec4d& v2);

vec4d operator/(const vec4d& v1, double v2);

double operator*(const vec4d& v1, const vec4d& v2);

template <int r, int c>
class matd {
public:
	double m[r][c];
	
	double* operator[](size_t k);
	
	const double* operator[](size_t k) const;
	
	void operator+=(const matd<r, c>& v);

	void operator-=(const matd<r, c>& v);

	void operator*=(double v);

	void operator/=(double v);
	
	operator vec2d() const;
	
	operator vec3d() const;
	
	operator vec4d() const;
	
	static matd<r, c> identity(int v);
	
	matd<c, r> transpose() const;
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

double mat2x2_determinant(const matd<2, 2>& m);

double mat3x3_determinant(const matd<3, 3>& m);

double mat4x4_determinant(const matd<4, 4>& m);

matd<2, 2> mat2x2_inverse(const matd<2, 2>& m);

matd<3, 3> mat3x3_inverse(const matd<3, 3>& m);

matd<4, 4> mat4x4_inverse(const matd<4, 4>& m);

/* inline implementations */

inline float randomf() {
	return rand() / (RAND_MAX + 1.f);
}

inline double randomd() {
	return rand() / (RAND_MAX + 1.);
}

/* template implementations */

template <int r, int c>
float* matf<r, c>::operator[](size_t k) {
	return m[k];
}

template <int r, int c>
const float* matf<r, c>::operator[](size_t k) const {
	return m[k];
}

template <int r, int c>
void matf<r, c>::operator+=(const matf<r, c>& v) {
	for (int i = r * c; i --> 0;) {
		*(*m + i) += *(*v.m + i);
	}
}

template <int r, int c>
void matf<r, c>::operator-=(const matf<r, c>& v) {
	for (int i = r * c; i --> 0;) {
		*(*m + i) -= *(*v.m + i);
	}
}

template <int r, int c>
void matf<r, c>::operator*=(float v) {
	for (int i = r * c; i --> 0;) {
		*(*m + i) *= v;
	}
}

template <int r, int c>
void matf<r, c>::operator/=(float v) {
	for (int i = r * c; i --> 0;) {
		*(*m + i) /= v;
	}
}

template <int r, int c>
matf<r, c>::operator vec2f() {
	return {m[0][0], m[1][0]};
}

template <int r, int c>
matf<r, c>::operator vec3f() {
	return {m[0][0], m[1][0], m[2][0]};
}

template <int r, int c>
matf<r, c>::operator vec4f() {
	return {m[0][0], m[1][0], m[2][0], m[3][0]};
}

template <int r, int c>
matf<r, c> matf<r, c>::identity(int v) {
	matf<r, c> matrix;
	for (int i = 0; i < v; ++i) {
		matrix[i][i] = 1;
	}
	return matrix;
}

template <int r, int c>
matf<c, r> matf<r, c>::transpose() const {
	matf<c, r> matrix;
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			matrix[j][i] = m[i][j];
		}
	}
	return matrix;
}

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

template <int r, int c>
double* matd<r, c>::operator[](size_t k) {
	return m[k];
}

template <int r, int c>
const double* matd<r, c>::operator[](size_t k) const {
	return m[k];
}

template <int r, int c>
void matd<r, c>::operator+=(const matd<r, c>& v) {
	for (int i = r * c; i --> 0;) {
		*(*m + i) += *(*v.m + i);
	}
}

template <int r, int c>
void matd<r, c>::operator-=(const matd<r, c>& v) {
	for (int i = r * c; i --> 0;) {
		*(*m + i) -= *(*v.m + i);
	}
}

template <int r, int c>
void matd<r, c>::operator*=(double v) {
	for (int i = r * c; i --> 0;) {
		*(*m + i) *= v;
	}
}

template <int r, int c>
void matd<r, c>::operator/=(double v) {
	for (int i = r * c; i --> 0;) {
		*(*m + i) /= v;
	}
}

template <int r, int c>
matd<r, c>::operator vec2d() const {
	return {m[0][0], m[1][0]};
}

template <int r, int c>
matd<r, c>::operator vec3d() const {
	return {m[0][0], m[1][0], m[2][0]};
}

template <int r, int c>
matd<r, c>::operator vec4d() const {
	return {m[0][0], m[1][0], m[2][0], m[3][0]};
}

template <int r, int c>
matd<r, c> matd<r, c>::identity(int v) {
	matd<r, c> matrix;
	for (int i = 0; i < v; ++i) {
		matrix[i][i] = 1;
	}
	return matrix;
}

template <int r, int c>
matd<c, r> matd<r, c>::transpose() const {
	matd<c, r> matrix;
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			matrix[j][i] = m[i][j];
		}
	}
	return matrix;
}

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
