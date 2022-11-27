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

namespace WR {

inline float randomf();

class Vec2f {
public:
	float x = 0;
	float y = 0;
	
	Vec2f() = default;
	
	Vec2f(float x);
	
	Vec2f(float x, float y);
	
	Vec2f(const Vec2f& v);
	
	Vec2f operator-() const;
	
	void operator+=(const Vec2f& v);
	
	void operator-=(const Vec2f& v);
	
	void operator*=(float v);
	
	void operator/=(float v);
	
	float magnitude() const;
	
	float distance(const Vec2f& v) const;
	
	Vec2f normalize() const;
	
	Vec2f rotate(float a) const;
	
	static Vec2f random();
};

Vec2f operator+(const Vec2f& v1, const Vec2f& v2);

Vec2f operator-(const Vec2f& v1, const Vec2f& v2);

Vec2f operator*(const Vec2f& v1, float v2);

Vec2f operator*(float v1, const Vec2f& v2);

Vec2f operator/(const Vec2f& v1, float v2);

float operator*(const Vec2f& v1, const Vec2f& v2);

float operator^(const Vec2f& v1, const Vec2f& v2);

using Vec2 = Vec2f;

class Vec3f {
public:
	float x = 0;
	float y = 0;
	float z = 0;
	
	Vec3f() = default;
	
	Vec3f(float x);
	
	Vec3f(float x, float y, float z);
	
	Vec3f(const Vec2f& v, float z);
	
	Vec3f(float x, const Vec2f& v);
	
	Vec3f(const Vec3f& v);
	
	Vec3f operator-() const;
	
	void operator+=(const Vec3f& v);
	
	void operator-=(const Vec3f& v);
	
	void operator*=(float v);
	
	void operator/=(float v);
	
	float magnitude() const;
	
	float distance(const Vec3f& v) const;
	
	Vec3f normalize() const;
	
	Vec3f rotate(const Vec3f& v, float a) const;
	
	static Vec3f random();
};

Vec3f operator+(const Vec3f& v1, const Vec3f& v2);

Vec3f operator-(const Vec3f& v1, const Vec3f& v2);

Vec3f operator*(const Vec3f& v1, float v2);

Vec3f operator*(float v1, const Vec3f& v2);

Vec3f operator/(const Vec3f& v1, float v2);

float operator*(const Vec3f& v1, const Vec3f& v2);

Vec3f operator^(const Vec3f& v1, const Vec3f& v2);

using Vec3 = Vec3f;

class Vec4f {
public:
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 0;
	
	Vec4f() = default;
	
	Vec4f(float x);
	
	Vec4f(float x, float y, float z, float w);
	
	Vec4f(const Vec2f& v, float z, float w);
	
	Vec4f(float x, const Vec2f& v, float w);
	
	Vec4f(float x, float y, const Vec2f& v);
	
	Vec4f(const Vec2f& v1, const Vec2f& v2);
	
	Vec4f(const Vec3f& v, float w);
	
	Vec4f(float x, const Vec3f& v);
	
	Vec4f(const Vec4f& v);
	
	Vec4f operator-() const;
	
	void operator+=(const Vec4f& v);
	
	void operator-=(const Vec4f& v);
	
	void operator*=(float v);
	
	void operator/=(float v);
	
	float magnitude() const;
	
	float distance(const Vec4f& v) const;
	
	Vec4f normalize() const;
};

Vec4f operator+(const Vec4f& v1, const Vec4f& v2);

Vec4f operator-(const Vec4f& v1, const Vec4f& v2);

Vec4f operator*(const Vec4f& v1, float v2);

Vec4f operator*(float v1, const Vec4f& v2);

Vec4f operator/(const Vec4f& v1, float v2);

float operator*(const Vec4f& v1, const Vec4f& v2);

using Vec4 = Vec4f;

template <int r, int c>
class Matf {
public:
	float m[r][c];
	
	float* operator[](size_t k);
	
	const float* operator[](size_t k) const;
	
	void operator+=(const Matf<r, c>& v);

	void operator-=(const Matf<r, c>& v);

	void operator*=(float v);

	void operator/=(float v);
	
	operator Vec2f();
	
	operator Vec3f();
	
	operator Vec4f();
	
	static Matf<r, c> identity(int v);
	
	Matf<c, r> transpose() const;
};

template <int r, int c>
Matf<r, c> operator+(const Matf<r, c>& v1, const Matf<r, c>& v2);

template <int r, int c>
Matf<r, c> operator-(const Matf<r, c>& v1, const Matf<r, c>& v2);

template <int r, int c>
Matf<r, c> operator*(const Matf<r, c>& v1, float v2);

template <int r, int c>
Matf<r, c> operator*(float v1, const Matf<r, c>& v2);

template <int l1, int l2, int l3>
Matf<l1, l3> operator*(const Matf<l1, l2>& v1, const Matf<l2, l3>& v2);

template <int r>
Matf<r, 1> operator*(const Matf<r, 2>& v1, const Vec2f& v2);

template <int r>
Matf<r, 1> operator*(const Matf<r, 3>& v1, const Vec3f& v2);

template <int r>
Matf<r, 1> operator*(const Matf<r, 4>& v1, const Vec4f& v2);

template <int r, int c>
Matf<r, c> operator/(const Matf<r, c>& v1, float v2);

template<int r, int c>
using Mat = Matf<r, c>;

using Mat2 = Matf<2, 2>;

using Mat3 = Matf<3, 3>;

using Mat4 = Matf<4, 4>;

float mat2_determinant(const Mat2& m);

float mat3_determinant(const Mat3& m);

float mat4_determinant(const Mat4& m);

Mat2 mat2_inverse(const Mat2& m);

Mat3 mat3_inverse(const Mat3& m);

Mat4 mat4_inverse(const Mat4& m);

inline double randomd();

class Vec2d {
public:
	double x = 0;
	double y = 0;
	
	Vec2d() = default;
	
	Vec2d(double x);
	
	Vec2d(double x, double y);
	
	Vec2d(const Vec2d& v);
	
	Vec2d operator-() const;
	
	void operator+=(const Vec2d& v);
	
	void operator-=(const Vec2d& v);
	
	void operator*=(double v);
	
	void operator/=(double v);
	
	double magnitude() const;
	
	double distance(const Vec2d& v) const;
	
	Vec2d normalize() const;
	
	Vec2d rotate(double a) const;
	
	static Vec2d randomunit();
};

Vec2d operator+(const Vec2d& v1, const Vec2d& v2);

Vec2d operator-(const Vec2d& v1, const Vec2d& v2);

Vec2d operator*(const Vec2d& v1, double v2);

Vec2d operator*(double v1, const Vec2d& v2);

Vec2d operator/(const Vec2d& v1, double v2);

double operator*(const Vec2d& v1, const Vec2d& v2);

double operator^(const Vec2d& v1, const Vec2d& v2);

class Vec3d {
public:
	double x = 0;
	double y = 0;
	double z = 0;
	
	Vec3d() = default;
	
	Vec3d(double x);
	
	Vec3d(double x, double y, double z);
	
	Vec3d(const Vec2d& v, double z);
	
	Vec3d(double x, const Vec2d& v);
	
	Vec3d(const Vec3d& v);
	
	Vec3d operator-() const;
	
	void operator+=(const Vec3d& v);
	
	void operator-=(const Vec3d& v);
	
	void operator*=(double v);
	
	void operator/=(double v);
	
	double magnitude() const;
	
	double distance(const Vec3d& v) const;
	
	Vec3d normalize() const;
	
	Vec3d rotate(const Vec3d& v, double a) const;
	
	static Vec3d randomunit();
};

Vec3d operator+(const Vec3d& v1, const Vec3d& v2);

Vec3d operator-(const Vec3d& v1, const Vec3d& v2);

Vec3d operator*(const Vec3d& v1, double v2);

Vec3d operator*(double v1, const Vec3d& v2);

Vec3d operator/(const Vec3d& v1, double v2);

double operator*(const Vec3d& v1, const Vec3d& v2);

Vec3d operator^(const Vec3d& v1, const Vec3d& v2);

class Vec4d {
public:
	double x = 0;
	double y = 0;
	double z = 0;
	double w = 0;
	
	Vec4d() = default;
	
	Vec4d(double x);
	
	Vec4d(double x, double y, double z, double w);
	
	Vec4d(const Vec2d& v, double z, double w);
	
	Vec4d(double x, const Vec2d& v, double w);
	
	Vec4d(double x, double y, const Vec2d& v);
	
	Vec4d(const Vec2d& v1, const Vec2d& v2);
	
	Vec4d(const Vec3d& v, double w);
	
	Vec4d(double x, const Vec3d& v);
	
	Vec4d(const Vec4d& v);
	
	Vec4d operator-() const;
	
	void operator+=(const Vec4d& v);
	
	void operator-=(const Vec4d& v);
	
	void operator*=(double v);
	
	void operator/=(double v);
	
	double magnitude() const;
	
	double distance(const Vec4d& v) const;
	
	Vec4d normalize() const;
};

Vec4d operator+(const Vec4d& v1, const Vec4d& v2);

Vec4d operator-(const Vec4d& v1, const Vec4d& v2);

Vec4d operator*(const Vec4d& v1, double v2);

Vec4d operator*(double v1, const Vec4d& v2);

Vec4d operator/(const Vec4d& v1, double v2);

double operator*(const Vec4d& v1, const Vec4d& v2);

template <int r, int c>
class Matd {
public:
	double m[r][c];
	
	double* operator[](size_t k);
	
	const double* operator[](size_t k) const;
	
	void operator+=(const Matd<r, c>& v);

	void operator-=(const Matd<r, c>& v);

	void operator*=(double v);

	void operator/=(double v);
	
	operator Vec2d() const;
	
	operator Vec3d() const;
	
	operator Vec4d() const;
	
	static Matd<r, c> identity(int v);
	
	Matd<c, r> transpose() const;
};

template <int r, int c>
Matd<r, c> operator+(const Matd<r, c>& v1, const Matd<r, c>& v2);

template <int r, int c>
Matd<r, c> operator-(const Matd<r, c>& v1, const Matd<r, c>& v2);

template <int r, int c>
Matd<r, c> operator*(const Matd<r, c>& v1, double v2);

template <int r, int c>
Matd<r, c> operator*(double v1, const Matd<r, c>& v2);

template <int l1, int l2, int l3>
Matd<l1, l3> operator*(const Matd<l1, l2>& v1, const Matd<l2, l3>& v2);

template <int r>
Matd<r, 1> operator*(const Matd<r, 2>& v1, const Vec2d& v2);

template <int r>
Matd<r, 1> operator*(const Matd<r, 3>& v1, const Vec3d& v2);

template <int r>
Matd<r, 1> operator*(const Matd<r, 4>& v1, const Vec4d& v2);

template <int r, int c>
Matd<r, c> operator/(const Matd<r, c>& v1, double v2);

double mat2x2_determinant(const Matd<2, 2>& m);

double mat3x3_determinant(const Matd<3, 3>& m);

double mat4x4_determinant(const Matd<4, 4>& m);

Matd<2, 2> mat2x2_inverse(const Matd<2, 2>& m);

Matd<3, 3> mat3x3_inverse(const Matd<3, 3>& m);

Matd<4, 4> mat4x4_inverse(const Matd<4, 4>& m);

/* inline implementations */

inline float randomf() {
	return rand() / (RAND_MAX + 1.f);
}

inline double randomd() {
	return rand() / (RAND_MAX + 1.);
}

/* template implementations */

template <int r, int c>
float* Matf<r, c>::operator[](size_t k) {
	return m[k];
}

template <int r, int c>
const float* Matf<r, c>::operator[](size_t k) const {
	return m[k];
}

template <int r, int c>
void Matf<r, c>::operator+=(const Matf<r, c>& v) {
	for (int i = r * c; i --> 0;) {
		*(*m + i) += *(*v.m + i);
	}
}

template <int r, int c>
void Matf<r, c>::operator-=(const Matf<r, c>& v) {
	for (int i = r * c; i --> 0;) {
		*(*m + i) -= *(*v.m + i);
	}
}

template <int r, int c>
void Matf<r, c>::operator*=(float v) {
	for (int i = r * c; i --> 0;) {
		*(*m + i) *= v;
	}
}

template <int r, int c>
void Matf<r, c>::operator/=(float v) {
	for (int i = r * c; i --> 0;) {
		*(*m + i) /= v;
	}
}

template <int r, int c>
Matf<r, c>::operator Vec2f() {
	return {m[0][0], m[1][0]};
}

template <int r, int c>
Matf<r, c>::operator Vec3f() {
	return {m[0][0], m[1][0], m[2][0]};
}

template <int r, int c>
Matf<r, c>::operator Vec4f() {
	return {m[0][0], m[1][0], m[2][0], m[3][0]};
}

template <int r, int c>
Matf<r, c> Matf<r, c>::identity(int v) {
	Matf<r, c> matrix;
	for (int i = 0; i < v; ++i) {
		matrix[i][i] = 1;
	}
	return matrix;
}

template <int r, int c>
Matf<c, r> Matf<r, c>::transpose() const {
	Matf<c, r> matrix;
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			matrix[j][i] = m[i][j];
		}
	}
	return matrix;
}

template <int r, int c>
Matf<r, c> operator+(const Matf<r, c>& v1, const Matf<r, c>& v2) {
	Matf<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) + *(*v2.m + i);
	}
	return matrix;
}

template <int r, int c>
Matf<r, c> operator-(const Matf<r, c>& v1, const Matf<r, c>& v2) {
	Matf<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) - *(*v2.m + i);
	}
	return matrix;
}

template <int r, int c>
Matf<r, c> operator*(const Matf<r, c>& v1, float v2) {
	Matf<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) * v2;
	}
	return matrix;
}

template <int r, int c>
Matf<r, c> operator*(float v1, const Matf<r, c>& v2) {
	Matf<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = v1 + *(*v2.m + i);
	}
	return matrix;
}

template <int l1, int l2, int l3>
Matf<l1, l3> operator*(const Matf<l1, l2>& v1, const Matf<l2, l3>& v2) {
	Matf<l1, l3> matrix;
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
Matf<r, 1> operator*(const Matf<r, 2>& v1, const Vec2f& v2) {
	Matf<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y;
	}
	return matrix;
}

template <int r>
Matf<r, 1> operator*(const Matf<r, 3>& v1, const Vec3f& v2) {
	Matf<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y + v1[i][2] * v2.z;
	}
	return matrix;
}

template <int r>
Matf<r, 1> operator*(const Matf<r, 4>& v1, const Vec4f& v2) {
	Matf<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y + v1[i][2] * v2.z + v1[i][3] * v2.w;
	}
	return matrix;
}

template <int r, int c>
Matf<r, c> operator/(const Matf<r, c>& v1, float v2) {
	Matf<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) / v2;
	}
	return matrix;
}

template <int r, int c>
double* Matd<r, c>::operator[](size_t k) {
	return m[k];
}

template <int r, int c>
const double* Matd<r, c>::operator[](size_t k) const {
	return m[k];
}

template <int r, int c>
void Matd<r, c>::operator+=(const Matd<r, c>& v) {
	for (int i = r * c; i --> 0;) {
		*(*m + i) += *(*v.m + i);
	}
}

template <int r, int c>
void Matd<r, c>::operator-=(const Matd<r, c>& v) {
	for (int i = r * c; i --> 0;) {
		*(*m + i) -= *(*v.m + i);
	}
}

template <int r, int c>
void Matd<r, c>::operator*=(double v) {
	for (int i = r * c; i --> 0;) {
		*(*m + i) *= v;
	}
}

template <int r, int c>
void Matd<r, c>::operator/=(double v) {
	for (int i = r * c; i --> 0;) {
		*(*m + i) /= v;
	}
}

template <int r, int c>
Matd<r, c>::operator Vec2d() const {
	return {m[0][0], m[1][0]};
}

template <int r, int c>
Matd<r, c>::operator Vec3d() const {
	return {m[0][0], m[1][0], m[2][0]};
}

template <int r, int c>
Matd<r, c>::operator Vec4d() const {
	return {m[0][0], m[1][0], m[2][0], m[3][0]};
}

template <int r, int c>
Matd<r, c> Matd<r, c>::identity(int v) {
	Matd<r, c> matrix;
	for (int i = 0; i < v; ++i) {
		matrix[i][i] = 1;
	}
	return matrix;
}

template <int r, int c>
Matd<c, r> Matd<r, c>::transpose() const {
	Matd<c, r> matrix;
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			matrix[j][i] = m[i][j];
		}
	}
	return matrix;
}

template <int r, int c>
Matd<r, c> operator+(const Matd<r, c>& v1, const Matd<r, c>& v2) {
	Matd<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) + *(*v2.m + i);
	}
	return matrix;
}

template <int r, int c>
Matd<r, c> operator-(const Matd<r, c>& v1, const Matd<r, c>& v2) {
	Matd<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) - *(*v2.m + i);
	}
	return matrix;
}

template <int r, int c>
Matd<r, c> operator*(const Matd<r, c>& v1, double v2) {
	Matd<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) * v2;
	}
	return matrix;
}

template <int r, int c>
Matd<r, c> operator*(double v1, const Matd<r, c>& v2) {
	Matd<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = v1 + *(*v2.m + i);
	}
	return matrix;
}

template <int l1, int l2, int l3>
Matd<l1, l3> operator*(const Matd<l1, l2>& v1, const Matd<l2, l3>& v2) {
	Matd<l1, l3> matrix;
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
Matd<r, 1> operator*(const Matd<r, 2>& v1, const Vec2d& v2) {
	Matd<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y;
	}
	return matrix;
}

template <int r>
Matd<r, 1> operator*(const Matd<r, 3>& v1, const Vec3d& v2) {
	Matd<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y + v1[i][2] * v2.z;
	}
	return matrix;
}

template <int r>
Matd<r, 1> operator*(const Matd<r, 4>& v1, const Vec4d& v2) {
	Matd<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y + v1[i][2] * v2.z + v1[i][3] * v2.w;
	}
	return matrix;
}

template <int r, int c>
Matd<r, c> operator/(const Matd<r, c>& v1, double v2) {
	Matd<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) / v2;
	}
	return matrix;
}

}
