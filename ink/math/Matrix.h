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

#pragma once

#include <algorithm>
#include <sstream>

#include "Vector4.h"

namespace Ink {

template <int r, int c>
class FMat {
public:
	float m[r][c];
	
	float* operator[](size_t k);
	
	const float* operator[](size_t k) const;
	
	FMat<r, c> operator-();
	
	bool operator==(const FMat<r, c>& v) const;
	
	void operator+=(float v);
	
	void operator+=(const FMat<r, c>& v);
	
	void operator-=(float v);
	
	void operator-=(const FMat<r, c>& v);
	
	void operator*=(float v);
	
	void operator/=(float v);
	
	explicit operator FVec2();
	
	explicit operator FVec3();
	
	explicit operator FVec4();
	
	FMat<c, r> transpose() const;
	
	std::string to_string(int p = 2) const;
	
	static FMat<r, c> identity(int v);
};

template <int r, int c>
FMat<r, c> operator+(const FMat<r, c>& v1, float v2);

template <int r, int c>
FMat<r, c> operator+(float v1, const FMat<r, c>& v2);

template <int r, int c>
FMat<r, c> operator+(const FMat<r, c>& v1, const FMat<r, c>& v2);

template <int r, int c>
FMat<r, c> operator-(const FMat<r, c>& v1, float v2);

template <int r, int c>
FMat<r, c> operator-(float v1, const FMat<r, c>& v2);

template <int r, int c>
FMat<r, c> operator-(const FMat<r, c>& v1, const FMat<r, c>& v2);

template <int r, int c>
FMat<r, c> operator*(const FMat<r, c>& v1, float v2);

template <int r, int c>
FMat<r, c> operator*(float v1, const FMat<r, c>& v2);

template <int l1, int l2, int l3>
FMat<l1, l3> operator*(const FMat<l1, l2>& v1, const FMat<l2, l3>& v2);

template <int r>
FMat<r, 1> operator*(const FMat<r, 2>& v1, const FVec2& v2);

template <int r>
FMat<r, 1> operator*(const FMat<r, 3>& v1, const FVec3& v2);

template <int r>
FMat<r, 1> operator*(const FMat<r, 4>& v1, const FVec4& v2);

template <int r, int c>
FMat<r, c> operator/(const FMat<r, c>& v1, float v2);

template <int r, int c>
FMat<r, c> operator/(float v1, const FMat<r, c>& v2);

template<int r, int c>
using Mat = FMat<r, c>;

using Mat2 = FMat<2, 2>;

using Mat3 = FMat<3, 3>;

using Mat4 = FMat<4, 4>;

float determinant_2x2(const Mat2& m);

float determinant_3x3(const Mat3& m);

float determinant_4x4(const Mat4& m);

Mat2 inverse_2x2(const Mat2& m);

Mat3 inverse_3x3(const Mat3& m);

Mat4 inverse_4x4(const Mat4& m);

template <int r, int c>
class DMat {
public:
	double m[r][c];
	
	double* operator[](size_t k);
	
	const double* operator[](size_t k) const;
	
	DMat<r, c> operator-();
	
	bool operator==(const DMat<r, c>& v) const;
	
	void operator+=(double v);
	
	void operator+=(const DMat<r, c>& v);
	
	void operator-=(double v);
	
	void operator-=(const DMat<r, c>& v);
	
	void operator*=(double v);
	
	void operator/=(double v);
	
	explicit operator DVec2();
	
	explicit operator DVec3();
	
	explicit operator DVec4();
	
	DMat<c, r> transpose() const;
	
	std::string to_string(int p = 2) const;
	
	static DMat<r, c> identity(int v);
};

template <int r, int c>
DMat<r, c> operator+(const DMat<r, c>& v1, double v2);

template <int r, int c>
DMat<r, c> operator+(double v1, const DMat<r, c>& v2);

template <int r, int c>
DMat<r, c> operator+(const DMat<r, c>& v1, const DMat<r, c>& v2);

template <int r, int c>
DMat<r, c> operator-(const DMat<r, c>& v1, double v2);

template <int r, int c>
DMat<r, c> operator-(double v1, const DMat<r, c>& v2);

template <int r, int c>
DMat<r, c> operator-(const DMat<r, c>& v1, const DMat<r, c>& v2);

template <int r, int c>
DMat<r, c> operator*(const DMat<r, c>& v1, double v2);

template <int r, int c>
DMat<r, c> operator*(double v1, const DMat<r, c>& v2);

template <int l1, int l2, int l3>
DMat<l1, l3> operator*(const DMat<l1, l2>& v1, const DMat<l2, l3>& v2);

template <int r>
DMat<r, 1> operator*(const DMat<r, 2>& v1, const DVec2& v2);

template <int r>
DMat<r, 1> operator*(const DMat<r, 3>& v1, const DVec3& v2);

template <int r>
DMat<r, 1> operator*(const DMat<r, 4>& v1, const DVec4& v2);

template <int r, int c>
DMat<r, c> operator/(const DMat<r, c>& v1, double v2);

template <int r, int c>
DMat<r, c> operator/(double v1, const DMat<r, c>& v2);

using DMat2 = DMat<2, 2>;

using DMat3 = DMat<3, 3>;

using DMat4 = DMat<4, 4>;

double determinant_2x2(const DMat2& m);

double determinant_3x3(const DMat3& m);

double determinant_4x4(const DMat4& m);

DMat2 inverse_2x2(const DMat2& m);

DMat3 inverse_3x3(const DMat3& m);

DMat4 inverse_4x4(const DMat4& m);

template <int r, int c>
float* FMat<r, c>::operator[](size_t k) {
	return m[k];
}

template <int r, int c>
const float* FMat<r, c>::operator[](size_t k) const {
	return m[k];
}

template <int r, int c>
FMat<r, c> FMat<r, c>::operator-() {
	FMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) *(*matrix.m + i) = -*(*m + i);
	return matrix;
}

template <int r, int c>
bool FMat<r, c>::operator==(const FMat<r, c>& v) const {
	int i = r * c;
	while (i --> 0) {
		if (*(*m + i) != *(*v.m + i)) return false;
	}
	return true;
}

template <int r, int c>
void FMat<r, c>::operator+=(float v) {
	int i = r * c;
	while (i --> 0) *(*m + i) += v;
}

template <int r, int c>
void FMat<r, c>::operator+=(const FMat<r, c>& v) {
	int i = r * c;
	while (i --> 0) *(*m + i) += *(*v.m + i);
}

template <int r, int c>
void FMat<r, c>::operator-=(float v) {
	int i = r * c;
	while (i --> 0) *(*m + i) -= v;
}

template <int r, int c>
void FMat<r, c>::operator-=(const FMat<r, c>& v) {
	int i = r * c;
	while (i --> 0) *(*m + i) -= *(*v.m + i);
}

template <int r, int c>
void FMat<r, c>::operator*=(float v) {
	int i = r * c;
	while (i --> 0) *(*m + i) *= v;
}

template <int r, int c>
void FMat<r, c>::operator/=(float v) {
	int i = r * c;
	while (i --> 0) *(*m + i) /= v;
}

template <int r, int c>
FMat<r, c>::operator FVec2() {
	return {m[0][0], m[1][0]};
}

template <int r, int c>
FMat<r, c>::operator FVec3() {
	return {m[0][0], m[1][0], m[2][0]};
}

template <int r, int c>
FMat<r, c>::operator FVec4() {
	return {m[0][0], m[1][0], m[2][0], m[3][0]};
}

template <int r, int c>
FMat<c, r> FMat<r, c>::transpose() const {
	FMat<c, r> matrix;
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			matrix[j][i] = m[i][j];
		}
	}
	return matrix;
}

template <int r, int c>
std::string FMat<r, c>::to_string(int p) const {
	static std::stringstream stream;
	stream.setf(std::ios::fixed, std::ios::floatfield);
	stream.precision(p);
	stream.str(std::string());
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
FMat<r, c> FMat<r, c>::identity(int v) {
	FMat<r, c> matrix;
	std::fill_n(matrix[0], r * c, 0);
	for (int i = 0; i < v; ++i) {
		matrix[i][i] = 1;
	}
	return matrix;
}

template <int r, int c>
FMat<r, c> operator+(const FMat<r, c>& v1, float v2) {
	FMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) *(*matrix.m + i) = *(*v1.m + i) + v2;
	return matrix;
}

template <int r, int c>
FMat<r, c> operator+(float v1, const FMat<r, c>& v2) {
	FMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) *(*matrix.m + i) = v1 + *(*v2.m + i);
	return matrix;
}

template <int r, int c>
FMat<r, c> operator+(const FMat<r, c>& v1, const FMat<r, c>& v2) {
	FMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) *(*matrix.m + i) = *(*v1.m + i) + *(*v2.m + i);
	return matrix;
}

template <int r, int c>
FMat<r, c> operator-(const FMat<r, c>& v1, float v2) {
	FMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) *(*matrix.m + i) = *(*v1.m + i) - v2;
	return matrix;
}

template <int r, int c>
FMat<r, c> operator-(float v1, const FMat<r, c>& v2) {
	FMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) *(*matrix.m + i) = v1 - *(*v2.m + i);
	return matrix;
}

template <int r, int c>
FMat<r, c> operator-(const FMat<r, c>& v1, const FMat<r, c>& v2) {
	FMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) *(*matrix.m + i) = *(*v1.m + i) - *(*v2.m + i);
	return matrix;
}

template <int r, int c>
FMat<r, c> operator*(const FMat<r, c>& v1, float v2) {
	FMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) *(*matrix.m + i) = *(*v1.m + i) * v2;
	return matrix;
}

template <int r, int c>
FMat<r, c> operator*(float v1, const FMat<r, c>& v2) {
	FMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) *(*matrix.m + i) = v1 + *(*v2.m + i);
	return matrix;
}

template <int l1, int l2, int l3>
FMat<l1, l3> operator*(const FMat<l1, l2>& v1, const FMat<l2, l3>& v2) {
	FMat<l1, l3> matrix;
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
FMat<r, 1> operator*(const FMat<r, 2>& v1, const FVec2& v2) {
	FMat<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y;
	}
	return matrix;
}

template <int r>
FMat<r, 1> operator*(const FMat<r, 3>& v1, const FVec3& v2) {
	FMat<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y + v1[i][2] * v2.z;
	}
	return matrix;
}

template <int r>
FMat<r, 1> operator*(const FMat<r, 4>& v1, const FVec4& v2) {
	FMat<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y + v1[i][2] * v2.z + v1[i][3] * v2.w;
	}
	return matrix;
}

template <int r, int c>
FMat<r, c> operator/(float v1, const FMat<r, c>& v2) {
	FMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) *(*matrix.m + i) = v1 / *(*v2.m + i);
	return matrix;
}

template <int r, int c>
FMat<r, c> operator/(const FMat<r, c>& v1, float v2) {
	FMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) *(*matrix.m + i) = *(*v1.m + i) / v2;
	return matrix;
}

template <int r, int c>
double* DMat<r, c>::operator[](size_t k) {
	return m[k];
}

template <int r, int c>
const double* DMat<r, c>::operator[](size_t k) const {
	return m[k];
}

template <int r, int c>
DMat<r, c> DMat<r, c>::operator-() {
	DMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) *(*matrix.m + i) = -*(*m + i);
	return matrix;
}

template <int r, int c>
bool DMat<r, c>::operator==(const DMat<r, c>& v) const {
	int i = r * c;
	while (i --> 0) {
		if (*(*m + i) != *(*v.m + i)) return false;
	}
	return true;
}

template <int r, int c>
void DMat<r, c>::operator+=(double v) {
	int i = r * c;
	while (i --> 0) *(*m + i) += v;
}

template <int r, int c>
void DMat<r, c>::operator+=(const DMat<r, c>& v) {
	int i = r * c;
	while (i --> 0) *(*m + i) += *(*v.m + i);
}

template <int r, int c>
void DMat<r, c>::operator-=(double v) {
	int i = r * c;
	while (i --> 0) *(*m + i) -= v;
}

template <int r, int c>
void DMat<r, c>::operator-=(const DMat<r, c>& v) {
	int i = r * c;
	while (i --> 0) *(*m + i) -= *(*v.m + i);
}

template <int r, int c>
void DMat<r, c>::operator*=(double v) {
	int i = r * c;
	while (i --> 0) *(*m + i) *= v;
}

template <int r, int c>
void DMat<r, c>::operator/=(double v) {
	int i = r * c;
	while (i --> 0) *(*m + i) /= v;
}

template <int r, int c>
DMat<r, c>::operator DVec2() {
	return {m[0][0], m[1][0]};
}

template <int r, int c>
DMat<r, c>::operator DVec3() {
	return {m[0][0], m[1][0], m[2][0]};
}

template <int r, int c>
DMat<r, c>::operator DVec4() {
	return {m[0][0], m[1][0], m[2][0], m[3][0]};
}

template <int r, int c>
DMat<c, r> DMat<r, c>::transpose() const {
	DMat<c, r> matrix;
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			matrix[j][i] = m[i][j];
		}
	}
	return matrix;
}

template <int r, int c>
std::string DMat<r, c>::to_string(int p) const {
	static std::stringstream stream;
	stream.setf(std::ios::fixed, std::ios::floatfield);
	stream.precision(p);
	stream.str(std::string());
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
DMat<r, c> DMat<r, c>::identity(int v) {
	DMat<r, c> matrix;
	std::fill_n(matrix[0], r * c, 0);
	for (int i = 0; i < v; ++i) {
		matrix[i][i] = 1;
	}
	return matrix;
}

template <int r, int c>
DMat<r, c> operator+(const DMat<r, c>& v1, double v2) {
	DMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) *(*matrix.m + i) = *(*v1.m + i) + v2;
	return matrix;
}

template <int r, int c>
DMat<r, c> operator+(double v1, const DMat<r, c>& v2) {
	DMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) *(*matrix.m + i) = v1 + *(*v2.m + i);
	return matrix;
}

template <int r, int c>
DMat<r, c> operator+(const DMat<r, c>& v1, const DMat<r, c>& v2) {
	DMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) *(*matrix.m + i) = *(*v1.m + i) + *(*v2.m + i);
	return matrix;
}

template <int r, int c>
DMat<r, c> operator-(const DMat<r, c>& v1, double v2) {
	DMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) *(*matrix.m + i) = *(*v1.m + i) - v2;
	return matrix;
}

template <int r, int c>
DMat<r, c> operator-(double v1, const DMat<r, c>& v2) {
	DMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) *(*matrix.m + i) = v1 - *(*v2.m + i);
	return matrix;
}

template <int r, int c>
DMat<r, c> operator-(const DMat<r, c>& v1, const DMat<r, c>& v2) {
	DMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) *(*matrix.m + i) = *(*v1.m + i) - *(*v2.m + i);
	return matrix;
}

template <int r, int c>
DMat<r, c> operator*(const DMat<r, c>& v1, double v2) {
	DMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) *(*matrix.m + i) = *(*v1.m + i) * v2;
	return matrix;
}

template <int r, int c>
DMat<r, c> operator*(double v1, const DMat<r, c>& v2) {
	DMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) *(*matrix.m + i) = v1 + *(*v2.m + i);
	return matrix;
}

template <int l1, int l2, int l3>
DMat<l1, l3> operator*(const DMat<l1, l2>& v1, const DMat<l2, l3>& v2) {
	DMat<l1, l3> matrix;
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
DMat<r, 1> operator*(const DMat<r, 2>& v1, const DVec2& v2) {
	DMat<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y;
	}
	return matrix;
}

template <int r>
DMat<r, 1> operator*(const DMat<r, 3>& v1, const DVec3& v2) {
	DMat<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y + v1[i][2] * v2.z;
	}
	return matrix;
}

template <int r>
DMat<r, 1> operator*(const DMat<r, 4>& v1, const DVec4& v2) {
	DMat<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y + v1[i][2] * v2.z + v1[i][3] * v2.w;
	}
	return matrix;
}

template <int r, int c>
DMat<r, c> operator/(double v1, const DMat<r, c>& v2) {
	DMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) *(*matrix.m + i) = v1 / *(*v2.m + i);
	return matrix;
}

template <int r, int c>
DMat<r, c> operator/(const DMat<r, c>& v1, double v2) {
	DMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) *(*matrix.m + i) = *(*v1.m + i) / v2;
	return matrix;
}

}
