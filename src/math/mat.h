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
#include <string>
#include <sstream>

#include "vec4.h"

template <int r, int c>
class fmat {
public:
	float m[r][c];
	
	float* operator[](size_t k);
	
	const float* operator[](size_t k) const;
	
	void operator+=(const fmat<r, c>& v);

	void operator-=(const fmat<r, c>& v);

	void operator*=(float v);

	void operator/=(float v);
	
	operator fvec2();
	
	operator fvec3();
	
	operator fvec4();
	
	fmat<c, r> transpose() const;
	
	std::string to_string(int p = 2) const;
	
	static fmat<r, c> identity(int v);
};

template <int r, int c>
fmat<r, c> operator+(const fmat<r, c>& v1, const fmat<r, c>& v2);

template <int r, int c>
fmat<r, c> operator-(const fmat<r, c>& v1, const fmat<r, c>& v2);

template <int r, int c>
fmat<r, c> operator*(const fmat<r, c>& v1, float v2);

template <int r, int c>
fmat<r, c> operator*(float v1, const fmat<r, c>& v2);

template <int l1, int l2, int l3>
fmat<l1, l3> operator*(const fmat<l1, l2>& v1, const fmat<l2, l3>& v2);

template <int r>
fmat<r, 1> operator*(const fmat<r, 2>& v1, const fvec2& v2);

template <int r>
fmat<r, 1> operator*(const fmat<r, 3>& v1, const fvec3& v2);

template <int r>
fmat<r, 1> operator*(const fmat<r, 4>& v1, const fvec4& v2);

template <int r, int c>
fmat<r, c> operator/(const fmat<r, c>& v1, float v2);

template<int r, int c>
using mat = fmat<r, c>;

using mat2 = fmat<2, 2>;

using mat3 = fmat<3, 3>;

using mat4 = fmat<4, 4>;

float mat2_determinant(const mat2& m);

float mat3_determinant(const mat3& m);

float mat4_determinant(const mat4& m);

mat2 mat2_inverse(const mat2& m);

mat3 mat3_inverse(const mat3& m);

mat4 mat4_inverse(const mat4& m);

template <int r, int c>
class dmat {
public:
	double m[r][c];
	
	double* operator[](size_t k);
	
	const double* operator[](size_t k) const;
	
	void operator+=(const dmat<r, c>& v);

	void operator-=(const dmat<r, c>& v);

	void operator*=(double v);

	void operator/=(double v);
	
	operator dvec2() const;
	
	operator dvec3() const;
	
	operator dvec4() const;
	
	dmat<c, r> transpose() const;
	
	std::string to_string(int p = 2) const;
	
	static dmat<r, c> identity(int v);
};

template <int r, int c>
dmat<r, c> operator+(const dmat<r, c>& v1, const dmat<r, c>& v2);

template <int r, int c>
dmat<r, c> operator-(const dmat<r, c>& v1, const dmat<r, c>& v2);

template <int r, int c>
dmat<r, c> operator*(const dmat<r, c>& v1, double v2);

template <int r, int c>
dmat<r, c> operator*(double v1, const dmat<r, c>& v2);

template <int l1, int l2, int l3>
dmat<l1, l3> operator*(const dmat<l1, l2>& v1, const dmat<l2, l3>& v2);

template <int r>
dmat<r, 1> operator*(const dmat<r, 2>& v1, const dvec2& v2);

template <int r>
dmat<r, 1> operator*(const dmat<r, 3>& v1, const dvec3& v2);

template <int r>
dmat<r, 1> operator*(const dmat<r, 4>& v1, const dvec4& v2);

template <int r, int c>
dmat<r, c> operator/(const dmat<r, c>& v1, double v2);

using dmat2 = dmat<2, 2>;

using dmat3 = dmat<3, 3>;

using dmat4 = dmat<4, 4>;

double mat2d_determinant(const dmat2& m);

double mat3d_determinant(const dmat3& m);

double mat4d_determinant(const dmat4& m);

dmat2 mat2d_inverse(const dmat2& m);

dmat3 mat3d_inverse(const dmat3& m);

dmat4 mat4d_inverse(const dmat4& m);

/* template implementations */

template <int r, int c>
float* fmat<r, c>::operator[](size_t k) {
	return m[k];
}

template <int r, int c>
const float* fmat<r, c>::operator[](size_t k) const {
	return m[k];
}

template <int r, int c>
void fmat<r, c>::operator+=(const fmat<r, c>& v) {
	for (int i = r * c; i --> 0;) {
		*(*m + i) += *(*v.m + i);
	}
}

template <int r, int c>
void fmat<r, c>::operator-=(const fmat<r, c>& v) {
	for (int i = r * c; i --> 0;) {
		*(*m + i) -= *(*v.m + i);
	}
}

template <int r, int c>
void fmat<r, c>::operator*=(float v) {
	for (int i = r * c; i --> 0;) {
		*(*m + i) *= v;
	}
}

template <int r, int c>
void fmat<r, c>::operator/=(float v) {
	for (int i = r * c; i --> 0;) {
		*(*m + i) /= v;
	}
}

template <int r, int c>
fmat<r, c>::operator fvec2() {
	return {m[0][0], m[1][0]};
}

template <int r, int c>
fmat<r, c>::operator fvec3() {
	return {m[0][0], m[1][0], m[2][0]};
}

template <int r, int c>
fmat<r, c>::operator fvec4() {
	return {m[0][0], m[1][0], m[2][0], m[3][0]};
}

template <int r, int c>
fmat<c, r> fmat<r, c>::transpose() const {
	fmat<c, r> matrix;
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			matrix[j][i] = m[i][j];
		}
	}
	return matrix;
}

template <int r, int c>
std::string fmat<r, c>::to_string(int p) const {
	std::stringstream stream;
	stream.precision(p);
	stream.setf(std::ios::fixed, std::ios::floatfield);
	for (int i = 0; i < r; ++i) {
		stream << "[ ";
		for (int j = 0; j < c - 1; ++j) {
			stream << m[i][j] << ", ";
		}
		stream << m[i][c - 1] << " ]\n";
	}
	return stream.str();
}

template <int r, int c>
fmat<r, c> fmat<r, c>::identity(int v) {
	fmat<r, c> matrix;
	for (int i = 0; i < v; ++i) {
		matrix[i][i] = 1;
	}
	return matrix;
}

template <int r, int c>
fmat<r, c> operator+(const fmat<r, c>& v1, const fmat<r, c>& v2) {
	fmat<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) + *(*v2.m + i);
	}
	return matrix;
}

template <int r, int c>
fmat<r, c> operator-(const fmat<r, c>& v1, const fmat<r, c>& v2) {
	fmat<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) - *(*v2.m + i);
	}
	return matrix;
}

template <int r, int c>
fmat<r, c> operator*(const fmat<r, c>& v1, float v2) {
	fmat<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) * v2;
	}
	return matrix;
}

template <int r, int c>
fmat<r, c> operator*(float v1, const fmat<r, c>& v2) {
	fmat<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = v1 + *(*v2.m + i);
	}
	return matrix;
}

template <int l1, int l2, int l3>
fmat<l1, l3> operator*(const fmat<l1, l2>& v1, const fmat<l2, l3>& v2) {
	fmat<l1, l3> matrix;
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
fmat<r, 1> operator*(const fmat<r, 2>& v1, const fvec2& v2) {
	fmat<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y;
	}
	return matrix;
}

template <int r>
fmat<r, 1> operator*(const fmat<r, 3>& v1, const fvec3& v2) {
	fmat<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y + v1[i][2] * v2.z;
	}
	return matrix;
}

template <int r>
fmat<r, 1> operator*(const fmat<r, 4>& v1, const fvec4& v2) {
	fmat<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y + v1[i][2] * v2.z + v1[i][3] * v2.w;
	}
	return matrix;
}

template <int r, int c>
fmat<r, c> operator/(const fmat<r, c>& v1, float v2) {
	fmat<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) / v2;
	}
	return matrix;
}

template <int r, int c>
double* dmat<r, c>::operator[](size_t k) {
	return m[k];
}

template <int r, int c>
const double* dmat<r, c>::operator[](size_t k) const {
	return m[k];
}

template <int r, int c>
void dmat<r, c>::operator+=(const dmat<r, c>& v) {
	for (int i = r * c; i --> 0;) {
		*(*m + i) += *(*v.m + i);
	}
}

template <int r, int c>
void dmat<r, c>::operator-=(const dmat<r, c>& v) {
	for (int i = r * c; i --> 0;) {
		*(*m + i) -= *(*v.m + i);
	}
}

template <int r, int c>
void dmat<r, c>::operator*=(double v) {
	for (int i = r * c; i --> 0;) {
		*(*m + i) *= v;
	}
}

template <int r, int c>
void dmat<r, c>::operator/=(double v) {
	for (int i = r * c; i --> 0;) {
		*(*m + i) /= v;
	}
}

template <int r, int c>
dmat<r, c>::operator dvec2() const {
	return {m[0][0], m[1][0]};
}

template <int r, int c>
dmat<r, c>::operator dvec3() const {
	return {m[0][0], m[1][0], m[2][0]};
}

template <int r, int c>
dmat<r, c>::operator dvec4() const {
	return {m[0][0], m[1][0], m[2][0], m[3][0]};
}

template <int r, int c>
dmat<c, r> dmat<r, c>::transpose() const {
	dmat<c, r> matrix;
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			matrix[j][i] = m[i][j];
		}
	}
	return matrix;
}

template <int r, int c>
std::string dmat<r, c>::to_string(int p) const {
	std::stringstream stream;
	stream.precision(p);
	stream.setf(std::ios::fixed, std::ios::floatfield);
	for (int i = 0; i < r; ++i) {
		stream << "[ ";
		for (int j = 0; j < c - 1; ++j) {
			stream << m[i][j] << ", ";
		}
		stream << m[i][c - 1] << " ]\n";
	}
	return stream.str();
}

template <int r, int c>
dmat<r, c> dmat<r, c>::identity(int v) {
	dmat<r, c> matrix;
	for (int i = 0; i < v; ++i) {
		matrix[i][i] = 1;
	}
	return matrix;
}

template <int r, int c>
dmat<r, c> operator+(const dmat<r, c>& v1, const dmat<r, c>& v2) {
	dmat<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) + *(*v2.m + i);
	}
	return matrix;
}

template <int r, int c>
dmat<r, c> operator-(const dmat<r, c>& v1, const dmat<r, c>& v2) {
	dmat<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) - *(*v2.m + i);
	}
	return matrix;
}

template <int r, int c>
dmat<r, c> operator*(const dmat<r, c>& v1, double v2) {
	dmat<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) * v2;
	}
	return matrix;
}

template <int r, int c>
dmat<r, c> operator*(double v1, const dmat<r, c>& v2) {
	dmat<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = v1 + *(*v2.m + i);
	}
	return matrix;
}

template <int l1, int l2, int l3>
dmat<l1, l3> operator*(const dmat<l1, l2>& v1, const dmat<l2, l3>& v2) {
	dmat<l1, l3> matrix;
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
dmat<r, 1> operator*(const dmat<r, 2>& v1, const dvec2& v2) {
	dmat<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y;
	}
	return matrix;
}

template <int r>
dmat<r, 1> operator*(const dmat<r, 3>& v1, const dvec3& v2) {
	dmat<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y + v1[i][2] * v2.z;
	}
	return matrix;
}

template <int r>
dmat<r, 1> operator*(const dmat<r, 4>& v1, const dvec4& v2) {
	dmat<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y + v1[i][2] * v2.z + v1[i][3] * v2.w;
	}
	return matrix;
}

template <int r, int c>
dmat<r, c> operator/(const dmat<r, c>& v1, double v2) {
	dmat<r, c> matrix;
	for (int i = r * c; i --> 0;) {
		*(*matrix.m + i) = *(*v1.m + i) / v2;
	}
	return matrix;
}
