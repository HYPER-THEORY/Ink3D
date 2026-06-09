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

#pragma once

#include "Vector4.h"

#include <sstream>
#include <utility>

namespace ink {

template <int r, int c>
class FMat {
public:
	constexpr FMat();
	
	constexpr FMat(const FVec2& v);
	
	constexpr FMat(const FVec3& v);
	
	constexpr FMat(const FVec4& v);
	
	constexpr FMat(const std::initializer_list<float>& v);
	
	constexpr operator FVec2() const;
	
	constexpr operator FVec3() const;
	
	constexpr operator FVec4() const;
	
	constexpr float* operator[](size_t k);
	
	constexpr const float* operator[](size_t k) const;
	
	constexpr FMat<r, c> operator+() const;
	
	constexpr FMat<r, c> operator-() const;
	
	constexpr bool operator==(const FMat<r, c>& v) const;
	
	constexpr bool operator!=(const FMat<r, c>& v) const;
	
	constexpr FMat<r, c>& operator+=(float v);
	
	constexpr FMat<r, c>& operator+=(const FMat<r, c>& v);
	
	constexpr FMat<r, c>& operator-=(float v);
	
	constexpr FMat<r, c>& operator-=(const FMat<r, c>& v);
	
	constexpr FMat<r, c>& operator*=(float v);
	
	constexpr FMat<r, c>& operator/=(float v);
	
	constexpr FMat<c, r> transpose() const;
	
	std::string to_string(int p = 2) const;
	
	static constexpr FMat<r, c> identity();
	
private:
	float m[r * c];
};

template <int r, int c>
constexpr FMat<r, c> operator+(const FMat<r, c>& v1, float v2);

template <int r, int c>
constexpr FMat<r, c> operator+(float v1, const FMat<r, c>& v2);

template <int r, int c>
constexpr FMat<r, c> operator+(const FMat<r, c>& v1, const FMat<r, c>& v2);

template <int r, int c>
constexpr FMat<r, c> operator-(const FMat<r, c>& v1, float v2);

template <int r, int c>
constexpr FMat<r, c> operator-(float v1, const FMat<r, c>& v2);

template <int r, int c>
constexpr FMat<r, c> operator-(const FMat<r, c>& v1, const FMat<r, c>& v2);

template <int r, int c>
constexpr FMat<r, c> operator*(const FMat<r, c>& v1, float v2);

template <int r, int c>
constexpr FMat<r, c> operator*(float v1, const FMat<r, c>& v2);

template <int l1, int l2, int l3>
constexpr FMat<l1, l3> operator*(const FMat<l1, l2>& v1, const FMat<l2, l3>& v2);

template <int r>
constexpr FMat<r, 1> operator*(const FMat<r, 2>& v1, const FVec2& v2);

template <int r>
constexpr FMat<r, 1> operator*(const FMat<r, 3>& v1, const FVec3& v2);

template <int r>
constexpr FMat<r, 1> operator*(const FMat<r, 4>& v1, const FVec4& v2);

template <int r, int c>
constexpr FMat<r, c> operator/(const FMat<r, c>& v1, float v2);

template <int r, int c>
constexpr FMat<r, c> operator/(float v1, const FMat<r, c>& v2);

template <int r, int c>
class DMat {
public:
	constexpr DMat();
	
	constexpr DMat(const DVec2& v);
	
	constexpr DMat(const DVec3& v);
	
	constexpr DMat(const DVec4& v);
	
	constexpr DMat(const std::initializer_list<double>& v);
	
	constexpr operator DVec2() const;
	
	constexpr operator DVec3() const;
	
	constexpr operator DVec4() const;
	
	constexpr double* operator[](size_t k);
	
	constexpr const double* operator[](size_t k) const;
	
	constexpr DMat<r, c> operator+() const;
	
	constexpr DMat<r, c> operator-() const;
	
	constexpr bool operator==(const DMat<r, c>& v) const;
	
	constexpr bool operator!=(const DMat<r, c>& v) const;
	
	constexpr DMat<r, c>& operator+=(double v);
	
	constexpr DMat<r, c>& operator+=(const DMat<r, c>& v);
	
	constexpr DMat<r, c>& operator-=(double v);
	
	constexpr DMat<r, c>& operator-=(const DMat<r, c>& v);
	
	constexpr DMat<r, c>& operator*=(double v);
	
	constexpr DMat<r, c>& operator/=(double v);
	
	constexpr DMat<c, r> transpose() const;
	
	std::string to_string(int p = 2) const;
	
	static constexpr DMat<r, c> identity();
	
private:
	double m[r * c];
};

template <int r, int c>
constexpr DMat<r, c> operator+(const DMat<r, c>& v1, double v2);

template <int r, int c>
constexpr DMat<r, c> operator+(double v1, const DMat<r, c>& v2);

template <int r, int c>
constexpr DMat<r, c> operator+(const DMat<r, c>& v1, const DMat<r, c>& v2);

template <int r, int c>
constexpr DMat<r, c> operator-(const DMat<r, c>& v1, double v2);

template <int r, int c>
constexpr DMat<r, c> operator-(double v1, const DMat<r, c>& v2);

template <int r, int c>
constexpr DMat<r, c> operator-(const DMat<r, c>& v1, const DMat<r, c>& v2);

template <int r, int c>
constexpr DMat<r, c> operator*(const DMat<r, c>& v1, double v2);

template <int r, int c>
constexpr DMat<r, c> operator*(double v1, const DMat<r, c>& v2);

template <int l1, int l2, int l3>
constexpr DMat<l1, l3> operator*(const DMat<l1, l2>& v1, const DMat<l2, l3>& v2);

template <int r>
constexpr DMat<r, 1> operator*(const DMat<r, 2>& v1, const DVec2& v2);

template <int r>
constexpr DMat<r, 1> operator*(const DMat<r, 3>& v1, const DVec3& v2);

template <int r>
constexpr DMat<r, 1> operator*(const DMat<r, 4>& v1, const DVec4& v2);

template <int r, int c>
constexpr DMat<r, c> operator/(const DMat<r, c>& v1, double v2);

template <int r, int c>
constexpr DMat<r, c> operator/(double v1, const DMat<r, c>& v2);

template<int r, int c>
using Mat = FMat<r, c>;

using Mat2 = FMat<2, 2>;
using Mat3 = FMat<3, 3>;
using Mat4 = FMat<4, 4>;

using Mat2x2 = FMat<2, 2>;
using Mat2x3 = FMat<2, 3>;
using Mat2x4 = FMat<2, 4>;
using Mat3x2 = FMat<3, 2>;
using Mat3x3 = FMat<3, 3>;
using Mat3x4 = FMat<3, 4>;
using Mat4x2 = FMat<4, 2>;
using Mat4x3 = FMat<4, 3>;
using Mat4x4 = FMat<4, 4>;

constexpr float determinant_2x2(const Mat2& m);

constexpr float determinant_3x3(const Mat3& m);

constexpr float determinant_4x4(const Mat4& m);

constexpr Mat2 inverse_2x2(const Mat2& m);

constexpr Mat3 inverse_3x3(const Mat3& m);

constexpr Mat4 inverse_4x4(const Mat4& m);

using DMat2 = DMat<2, 2>;
using DMat3 = DMat<3, 3>;
using DMat4 = DMat<4, 4>;

using DMat2x2 = DMat<2, 2>;
using DMat2x3 = DMat<2, 3>;
using DMat2x4 = DMat<2, 4>;
using DMat3x2 = DMat<3, 2>;
using DMat3x3 = DMat<3, 3>;
using DMat3x4 = DMat<3, 4>;
using DMat4x2 = DMat<4, 2>;
using DMat4x3 = DMat<4, 3>;
using DMat4x4 = DMat<4, 4>;

constexpr double determinant_2x2(const DMat2& m);

constexpr double determinant_3x3(const DMat3& m);

constexpr double determinant_4x4(const DMat4& m);

constexpr DMat2 inverse_2x2(const DMat2& m);

constexpr DMat3 inverse_3x3(const DMat3& m);

constexpr DMat4 inverse_4x4(const DMat4& m);

template <int r, int c>
constexpr FMat<r, c>::FMat() {
	std::fill_n(m, r * c, 0.0f);
}

template <int r, int c>
constexpr FMat<r, c>::FMat(const FVec2& v) {
	std::fill_n(m, r * c, 0.0f);
	m[0] = v.x;
	m[1] = v.y;
}

template <int r, int c>
constexpr FMat<r, c>::FMat(const FVec3& v) {
	std::fill_n(m, r * c, 0.0f);
	m[0] = v.x;
	m[1] = v.y;
	m[2] = v.z;
}

template <int r, int c>
constexpr FMat<r, c>::FMat(const FVec4& v) {
	std::fill_n(m, r * c, 0.0f);
	m[0] = v.x;
	m[1] = v.y;
	m[2] = v.z;
	m[3] = v.w;
}

template <int r, int c>
constexpr FMat<r, c>::FMat(const std::initializer_list<float>& v) {
	std::fill_n(m, r * c, 0.0f);
	std::copy(v.begin(), v.end(), m);
}

template <int r, int c>
constexpr FMat<r, c>::operator FVec2() const {
	return {m[0], m[1]};
}

template <int r, int c>
constexpr FMat<r, c>::operator FVec3() const {
	return {m[0], m[1], m[2]};
}

template <int r, int c>
constexpr FMat<r, c>::operator FVec4() const {
	return {m[0], m[1], m[2], m[3]};
}

template <int r, int c>
constexpr float* FMat<r, c>::operator[](size_t k) {
	return m + k * c;
}

template <int r, int c>
constexpr const float* FMat<r, c>::operator[](size_t k) const {
	return m + k * c;
}

template <int r, int c>
constexpr FMat<r, c> FMat<r, c>::operator+() const {
	return *this;
}

template <int r, int c>
constexpr FMat<r, c> FMat<r, c>::operator-() const {
	FMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) matrix[0][i] = -m[i];
	return matrix;
}

template <int r, int c>
constexpr bool FMat<r, c>::operator==(const FMat<r, c>& v) const {
	int i = r * c;
	while (i --> 0) {
		if (m[i] != v[0][i]) return false;
	}
	return true;
}

template <int r, int c>
constexpr bool FMat<r, c>::operator!=(const FMat<r, c>& v) const {
	int i = r * c;
	while (i --> 0) {
		if (m[i] != v[0][i]) return true;
	}
	return false;
}

template <int r, int c>
constexpr FMat<r, c>& FMat<r, c>::operator+=(float v) {
	int i = r * c;
	while (i --> 0) m[i] += v;
	return *this;
}

template <int r, int c>
constexpr FMat<r, c>& FMat<r, c>::operator+=(const FMat<r, c>& v) {
	int i = r * c;
	while (i --> 0) m[i] += v[0][i];
	return *this;
}

template <int r, int c>
constexpr FMat<r, c>& FMat<r, c>::operator-=(float v) {
	int i = r * c;
	while (i --> 0) m[i] -= v;
	return *this;
}

template <int r, int c>
constexpr FMat<r, c>& FMat<r, c>::operator-=(const FMat<r, c>& v) {
	int i = r * c;
	while (i --> 0) m[i] -= v[0][i];
	return *this;
}

template <int r, int c>
constexpr FMat<r, c>& FMat<r, c>::operator*=(float v) {
	int i = r * c;
	while (i --> 0) m[i] *= v;
	return *this;
}

template <int r, int c>
constexpr FMat<r, c>& FMat<r, c>::operator/=(float v) {
	int i = r * c;
	while (i --> 0) m[i] /= v;
	return *this;
}

template <int r, int c>
constexpr FMat<c, r> FMat<r, c>::transpose() const {
	FMat<c, r> matrix;
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			matrix[j][i] = m[i * c + j];
		}
	}
	return matrix;
}

template <int r, int c>
std::string FMat<r, c>::to_string(int p) const {
	std::stringstream stream;
	stream.setf(std::ios::fixed, std::ios::floatfield);
	stream.precision(p);
	for (int i = 0; i < r; ++i) {
		stream << "[ ";
		for (int j = 0; j < c - 1; ++j) {
			stream << m[i * c + j] << ", ";
		}
		stream << m[i * c + c - 1] << " ]\n";
	}
	return stream.str();
}

template <int r, int c>
constexpr FMat<r, c> FMat<r, c>::identity() {
	FMat<r, c> matrix;
	int i = std::min(r, c);
	while (i --> 0) matrix[i][i] = 1;
	return matrix;
}

template <int r, int c>
constexpr FMat<r, c> operator+(const FMat<r, c>& v1, float v2) {
	FMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) matrix[0][i] = v1[0][i] + v2;
	return matrix;
}

template <int r, int c>
constexpr FMat<r, c> operator+(float v1, const FMat<r, c>& v2) {
	FMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) matrix[0][i] = v1 + v2[0][i];
	return matrix;
}

template <int r, int c>
constexpr FMat<r, c> operator+(const FMat<r, c>& v1, const FMat<r, c>& v2) {
	FMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) matrix[0][i] = v1[0][i] + v2[0][i];
	return matrix;
}

template <int r, int c>
constexpr FMat<r, c> operator-(const FMat<r, c>& v1, float v2) {
	FMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) matrix[0][i] = v1[0][i] - v2;
	return matrix;
}

template <int r, int c>
constexpr FMat<r, c> operator-(float v1, const FMat<r, c>& v2) {
	FMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) matrix[0][i] = v1 - v2[0][i];
	return matrix;
}

template <int r, int c>
constexpr FMat<r, c> operator-(const FMat<r, c>& v1, const FMat<r, c>& v2) {
	FMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) matrix[0][i] = v1[0][i] - v2[0][i];
	return matrix;
}

template <int r, int c>
constexpr FMat<r, c> operator*(const FMat<r, c>& v1, float v2) {
	FMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) matrix[0][i] = v1[0][i] * v2;
	return matrix;
}

template <int r, int c>
constexpr FMat<r, c> operator*(float v1, const FMat<r, c>& v2) {
	FMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) matrix[0][i] = v1 * v2[0][i];
	return matrix;
}

template <int l1, int l2, int l3>
constexpr FMat<l1, l3> operator*(const FMat<l1, l2>& v1, const FMat<l2, l3>& v2) {
	FMat<l1, l3> matrix;
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
constexpr FMat<r, 1> operator*(const FMat<r, 2>& v1, const FVec2& v2) {
	FMat<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y;
	}
	return matrix;
}

template <int r>
constexpr FMat<r, 1> operator*(const FMat<r, 3>& v1, const FVec3& v2) {
	FMat<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y + v1[i][2] * v2.z;
	}
	return matrix;
}

template <int r>
constexpr FMat<r, 1> operator*(const FMat<r, 4>& v1, const FVec4& v2) {
	FMat<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y + v1[i][2] * v2.z + v1[i][3] * v2.w;
	}
	return matrix;
}

template <int r, int c>
constexpr FMat<r, c> operator/(const FMat<r, c>& v1, float v2) {
	FMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) matrix[0][i] = v1[0][i] / v2;
	return matrix;
}

template <int r, int c>
constexpr FMat<r, c> operator/(float v1, const FMat<r, c>& v2) {
	FMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) matrix[0][i] = v1 / v2[0][i];
	return matrix;
}

template <int r, int c>
constexpr DMat<r, c>::DMat() {
	std::fill_n(m, r * c, 0.0);
}

template <int r, int c>
constexpr DMat<r, c>::DMat(const DVec2& v) {
	std::fill_n(m, r * c, 0.0);
	m[0] = v.x;
	m[1] = v.y;
}

template <int r, int c>
constexpr DMat<r, c>::DMat(const DVec3& v) {
	std::fill_n(m, r * c, 0.0);
	m[0] = v.x;
	m[1] = v.y;
	m[2] = v.z;
}

template <int r, int c>
constexpr DMat<r, c>::DMat(const DVec4& v) {
	std::fill_n(m, r * c, 0.0);
	m[0] = v.x;
	m[1] = v.y;
	m[2] = v.z;
	m[3] = v.w;
}

template <int r, int c>
constexpr DMat<r, c>::DMat(const std::initializer_list<double>& v) {
	std::fill_n(m, r * c, 0.0);
	std::copy(v.begin(), v.end(), m);
}

template <int r, int c>
constexpr DMat<r, c>::operator DVec2() const {
	return {m[0], m[1]};
}

template <int r, int c>
constexpr DMat<r, c>::operator DVec3() const {
	return {m[0], m[1], m[2]};
}

template <int r, int c>
constexpr DMat<r, c>::operator DVec4() const {
	return {m[0], m[1], m[2], m[3]};
}

template <int r, int c>
constexpr double* DMat<r, c>::operator[](size_t k) {
	return m + k * c;
}

template <int r, int c>
constexpr const double* DMat<r, c>::operator[](size_t k) const {
	return m + k * c;
}

template <int r, int c>
constexpr DMat<r, c> DMat<r, c>::operator+() const {
	return *this;
}

template <int r, int c>
constexpr DMat<r, c> DMat<r, c>::operator-() const {
	DMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) matrix[0][i] = -m[i];
	return matrix;
}

template <int r, int c>
constexpr bool DMat<r, c>::operator==(const DMat<r, c>& v) const {
	int i = r * c;
	while (i --> 0) {
		if (m[i] != v[0][i]) return false;
	}
	return true;
}

template <int r, int c>
constexpr bool DMat<r, c>::operator!=(const DMat<r, c>& v) const {
	int i = r * c;
	while (i --> 0) {
		if (m[i] != v[0][i]) return true;
	}
	return false;
}

template <int r, int c>
constexpr DMat<r, c>& DMat<r, c>::operator+=(double v) {
	int i = r * c;
	while (i --> 0) m[i] += v;
	return *this;
}

template <int r, int c>
constexpr DMat<r, c>& DMat<r, c>::operator+=(const DMat<r, c>& v) {
	int i = r * c;
	while (i --> 0) m[i] += v[0][i];
	return *this;
}

template <int r, int c>
constexpr DMat<r, c>& DMat<r, c>::operator-=(double v) {
	int i = r * c;
	while (i --> 0) m[i] -= v;
	return *this;
}

template <int r, int c>
constexpr DMat<r, c>& DMat<r, c>::operator-=(const DMat<r, c>& v) {
	int i = r * c;
	while (i --> 0) m[i] -= v[0][i];
	return *this;
}

template <int r, int c>
constexpr DMat<r, c>& DMat<r, c>::operator*=(double v) {
	int i = r * c;
	while (i --> 0) m[i] *= v;
	return *this;
}

template <int r, int c>
constexpr DMat<r, c>& DMat<r, c>::operator/=(double v) {
	int i = r * c;
	while (i --> 0) m[i] /= v;
	return *this;
}

template <int r, int c>
constexpr DMat<c, r> DMat<r, c>::transpose() const {
	DMat<c, r> matrix;
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			matrix[j][i] = m[i * c + j];
		}
	}
	return matrix;
}

template <int r, int c>
std::string DMat<r, c>::to_string(int p) const {
	std::stringstream stream;
	stream.setf(std::ios::fixed, std::ios::floatfield);
	stream.precision(p);
	for (int i = 0; i < r; ++i) {
		stream << "[ ";
		for (int j = 0; j < c - 1; ++j) {
			stream << m[i * c + j] << ", ";
		}
		stream << m[i * c + c - 1] << " ]\n";
	}
	return stream.str();
}

template <int r, int c>
constexpr DMat<r, c> DMat<r, c>::identity() {
	DMat<r, c> matrix;
	int i = std::min(r, c);
	while (i --> 0) matrix[i][i] = 1;
	return matrix;
}

template <int r, int c>
constexpr DMat<r, c> operator+(const DMat<r, c>& v1, double v2) {
	DMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) matrix[0][i] = v1[0][i] + v2;
	return matrix;
}

template <int r, int c>
constexpr DMat<r, c> operator+(double v1, const DMat<r, c>& v2) {
	DMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) matrix[0][i] = v1 + v2[0][i];
	return matrix;
}

template <int r, int c>
constexpr DMat<r, c> operator+(const DMat<r, c>& v1, const DMat<r, c>& v2) {
	DMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) matrix[0][i] = v1[0][i] + v2[0][i];
	return matrix;
}

template <int r, int c>
constexpr DMat<r, c> operator-(const DMat<r, c>& v1, double v2) {
	DMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) matrix[0][i] = v1[0][i] - v2;
	return matrix;
}

template <int r, int c>
constexpr DMat<r, c> operator-(double v1, const DMat<r, c>& v2) {
	DMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) matrix[0][i] = v1 - v2[0][i];
	return matrix;
}

template <int r, int c>
constexpr DMat<r, c> operator-(const DMat<r, c>& v1, const DMat<r, c>& v2) {
	DMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) matrix[0][i] = v1[0][i] - v2[0][i];
	return matrix;
}

template <int r, int c>
constexpr DMat<r, c> operator*(const DMat<r, c>& v1, double v2) {
	DMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) matrix[0][i] = v1[0][i] * v2;
	return matrix;
}

template <int r, int c>
constexpr DMat<r, c> operator*(double v1, const DMat<r, c>& v2) {
	DMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) matrix[0][i] = v1 * v2[0][i];
	return matrix;
}

template <int l1, int l2, int l3>
constexpr DMat<l1, l3> operator*(const DMat<l1, l2>& v1, const DMat<l2, l3>& v2) {
	DMat<l1, l3> matrix;
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
constexpr DMat<r, 1> operator*(const DMat<r, 2>& v1, const DVec2& v2) {
	DMat<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y;
	}
	return matrix;
}

template <int r>
constexpr DMat<r, 1> operator*(const DMat<r, 3>& v1, const DVec3& v2) {
	DMat<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y + v1[i][2] * v2.z;
	}
	return matrix;
}

template <int r>
constexpr DMat<r, 1> operator*(const DMat<r, 4>& v1, const DVec4& v2) {
	DMat<r, 1> matrix;
	for (int i = 0; i < r; ++i) {
		matrix[i][0] = v1[i][0] * v2.x + v1[i][1] * v2.y + v1[i][2] * v2.z + v1[i][3] * v2.w;
	}
	return matrix;
}

template <int r, int c>
constexpr DMat<r, c> operator/(const DMat<r, c>& v1, double v2) {
	DMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) matrix[0][i] = v1[0][i] / v2;
	return matrix;
}

template <int r, int c>
constexpr DMat<r, c> operator/(double v1, const DMat<r, c>& v2) {
	DMat<r, c> matrix;
	int i = r * c;
	while (i --> 0) matrix[0][i] = v1 / v2[0][i];
	return matrix;
}

constexpr float determinant_2x2(const Mat2& m) {
	/*
	 * d = M00 * M11 - M01 * M10
	 */
	return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

constexpr float determinant_3x3(const Mat3& m) {
	/*
	 * s0 = M11 * M22 - M21 * M12
	 * s1 = M12 * M20 - M10 * M22
	 * s2 = M10 * M21 - M20 * M11
	 * d = M00 * s0 + M01 * s1 + M02 * s2
	 */
	float sub0 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
	float sub1 = m[1][2] * m[2][0] - m[1][0] * m[2][2];
	float sub2 = m[1][0] * m[2][1] - m[2][0] * m[1][1];
	return m[0][0] * sub0 + m[0][1] * sub1 + m[0][2] * sub2;
}

constexpr float determinant_4x4(const Mat4& m) {
	/*
	 * s0 = M22 * M33 - M32 * M23
	 * s1 = M21 * M33 - M31 * M23
	 * s2 = M21 * M32 - M31 * M22
	 * s3 = M20 * M33 - M30 * M23
	 * s4 = M20 * M32 - M30 * M22
	 * s5 = M20 * M31 - M30 * M21
	 * d = M00 * (M11 * s0 - M12 * s1 + M13 * s2) -
	 *     M01 * (M10 * s0 - M12 * s3 + M13 * s4) +
	 *     M02 * (M10 * s1 - M11 * s3 + M13 * s5) -
	 *     M03 * (M10 * s2 - M11 * s4 + M12 * s5)
	 */
	float sub0 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
	float sub1 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
	float sub2 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
	float sub3 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
	float sub4 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
	float sub5 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
	return (m[0][0] * (m[1][1] * sub0 - m[1][2] * sub1 + m[1][3] * sub2) -
			m[0][1] * (m[1][0] * sub0 - m[1][2] * sub3 + m[1][3] * sub4) +
			m[0][2] * (m[1][0] * sub1 - m[1][1] * sub3 + m[1][3] * sub5) -
			m[0][3] * (m[1][0] * sub2 - m[1][1] * sub4 + m[1][2] * sub5));
}

constexpr Mat2 inverse_2x2(const Mat2& m) {
	/*
	 * d = M00 * M11 - M01 * M10
	 * M = [
	 *      M11 / d, -M01 / d,
	 *     -M10 / d,  M00 / d,
	 * ]
	 */
	float inv_det = 1 / (m[0][0] * m[1][1] - m[0][1] * m[1][0]);
	return {
		 inv_det * m[1][1],
		-inv_det * m[0][1],
		-inv_det * m[1][0],
		 inv_det * m[0][0],
	};
}

constexpr Mat3 inverse_3x3(const Mat3& m) {
	/*
	 * i0 = M11 * M22 - M21 * M12
	 * i1 = M12 * M20 - M10 * M22
	 * i2 = M10 * M21 - M20 * M11
	 * d = M00 * i0 + M01 * i1 + M02 * i2
	 * M = [
	 *     M11 * M22 - M21 * M12, M02 * M21 - M01 * M22, M01 * M12 - M02 * M11,
	 *     M12 * M20 - M10 * M22, M00 * M22 - M02 * M20, M10 * M02 - M00 * M12,
	 *     M10 * M21 - M20 * M11, M20 * M01 - M00 * M21, M00 * M11 - M10 * M01,
	 * ]
	 * M = M / d
	 */
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

constexpr Mat4 inverse_4x4(const Mat4& m) {
	/*
	 * s00 = M22 * M33 - M23 * M32
	 * s01 = M21 * M33 - M23 * M31
	 * s02 = M21 * M32 - M22 * M31
	 * s03 = M20 * M33 - M23 * M30
	 * s04 = M20 * M32 - M22 * M30
	 * s05 = M20 * M31 - M21 * M30
	 * s06 = M12 * M33 - M13 * M32
	 * s07 = M11 * M33 - M13 * M31
	 * s08 = M11 * M32 - M12 * M31
	 * s09 = M12 * M23 - M13 * M22
	 * s10 = M11 * M23 - M13 * M21
	 * s11 = M11 * M22 - M12 * M21
	 * s12 = M10 * M33 - M13 * M30
	 * s13 = M10 * M32 - M12 * M30
	 * s14 = M10 * M23 - M13 * M20
	 * s15 = M10 * M22 - M12 * M20
	 * s16 = M10 * M31 - M11 * M30
	 * s17 = M10 * M21 - M11 * M20
	 * i00 = M11 * s00 - M12 * s01 + M13 * s02
	 * i01 = M10 * s00 - M12 * s03 + M13 * s04
	 * i02 = M10 * s01 - M11 * s03 + M13 * s05
	 * i03 = M10 * s02 - M11 * s04 + M12 * s05
	 * d = M00 * i00 - M01 * i01 + M02 * i02 - M03 * i03
	 * M = [
	 *      (M11 * s00 - M12 * s01 + M13 * s02),
	 *     -(M01 * s00 - M02 * s01 + M03 * s02),
	 *      (M01 * s06 - M02 * s07 + M03 * s08),
	 *     -(M01 * s09 - M02 * s10 + M03 * s11),
	 *     -(M10 * s00 - M12 * s03 + M13 * s04),
	 *      (M00 * s00 - M02 * s03 + M03 * s04),
	 *     -(M00 * s06 - M02 * s12 + M03 * s13),
	 *      (M00 * s09 - M02 * s14 + M03 * s15),
	 *      (M10 * s01 - M11 * s03 + M13 * s05),
	 *     -(M00 * s01 - M01 * s03 + M03 * s05),
	 *      (M00 * s07 - M01 * s12 + M03 * s16),
	 *     -(M00 * s10 - M01 * s14 + M03 * s17),
	 *     -(M10 * s02 - M11 * s04 + M12 * s05),
	 *      (M00 * s02 - M01 * s04 + M02 * s05),
	 *     -(M00 * s08 - M01 * s13 + M02 * s16),
	 *      (M00 * s11 - M01 * s15 + M02 * s17),
	 * ]
	 * M = M / d
	 */
	float sub00 = m[2][2] * m[3][3] - m[2][3] * m[3][2];
	float sub01 = m[2][1] * m[3][3] - m[2][3] * m[3][1];
	float sub02 = m[2][1] * m[3][2] - m[2][2] * m[3][1];
	float sub03 = m[2][0] * m[3][3] - m[2][3] * m[3][0];
	float sub04 = m[2][0] * m[3][2] - m[2][2] * m[3][0];
	float sub05 = m[2][0] * m[3][1] - m[2][1] * m[3][0];
	float sub06 = m[1][2] * m[3][3] - m[1][3] * m[3][2];
	float sub07 = m[1][1] * m[3][3] - m[1][3] * m[3][1];
	float sub08 = m[1][1] * m[3][2] - m[1][2] * m[3][1];
	float sub09 = m[1][2] * m[2][3] - m[1][3] * m[2][2];
	float sub10 = m[1][1] * m[2][3] - m[1][3] * m[2][1];
	float sub11 = m[1][1] * m[2][2] - m[1][2] * m[2][1];
	float sub12 = m[1][0] * m[3][3] - m[1][3] * m[3][0];
	float sub13 = m[1][0] * m[3][2] - m[1][2] * m[3][0];
	float sub14 = m[1][0] * m[2][3] - m[1][3] * m[2][0];
	float sub15 = m[1][0] * m[2][2] - m[1][2] * m[2][0];
	float sub16 = m[1][0] * m[3][1] - m[1][1] * m[3][0];
	float sub17 = m[1][0] * m[2][1] - m[1][1] * m[2][0];
	float inv00 = m[1][1] * sub00 - m[1][2] * sub01 + m[1][3] * sub02;
	float inv01 = m[1][0] * sub00 - m[1][2] * sub03 + m[1][3] * sub04;
	float inv02 = m[1][0] * sub01 - m[1][1] * sub03 + m[1][3] * sub05;
	float inv03 = m[1][0] * sub02 - m[1][1] * sub04 + m[1][2] * sub05;
	float inv_det = 1 / (m[0][0] * inv00 - m[0][1] * inv01 + m[0][2] * inv02 - m[0][3] * inv03);
	return {
		 inv_det * inv00,
		-inv_det * (m[0][1] * sub00 - m[0][2] * sub01 + m[0][3] * sub02),
		 inv_det * (m[0][1] * sub06 - m[0][2] * sub07 + m[0][3] * sub08),
		-inv_det * (m[0][1] * sub09 - m[0][2] * sub10 + m[0][3] * sub11),
		-inv_det * inv01,
		 inv_det * (m[0][0] * sub00 - m[0][2] * sub03 + m[0][3] * sub04),
		-inv_det * (m[0][0] * sub06 - m[0][2] * sub12 + m[0][3] * sub13),
		 inv_det * (m[0][0] * sub09 - m[0][2] * sub14 + m[0][3] * sub15),
		 inv_det * inv02,
		-inv_det * (m[0][0] * sub01 - m[0][1] * sub03 + m[0][3] * sub05),
		 inv_det * (m[0][0] * sub07 - m[0][1] * sub12 + m[0][3] * sub16),
		-inv_det * (m[0][0] * sub10 - m[0][1] * sub14 + m[0][3] * sub17),
		-inv_det * inv03,
		 inv_det * (m[0][0] * sub02 - m[0][1] * sub04 + m[0][2] * sub05),
		-inv_det * (m[0][0] * sub08 - m[0][1] * sub13 + m[0][2] * sub16),
		 inv_det * (m[0][0] * sub11 - m[0][1] * sub15 + m[0][2] * sub17),
	};
}

constexpr double determinant_2x2(const DMat2& m) {
	/*
	 * d = M00 * M11 - M01 * M10
	 */
	return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

constexpr double determinant_3x3(const DMat3& m) {
	/*
	 * s0 = M11 * M22 - M21 * M12
	 * s1 = M12 * M20 - M10 * M22
	 * s2 = M10 * M21 - M20 * M11
	 * d = M00 * s0 + M01 * s1 + M02 * s2
	 */
	double sub0 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
	double sub1 = m[1][2] * m[2][0] - m[1][0] * m[2][2];
	double sub2 = m[1][0] * m[2][1] - m[2][0] * m[1][1];
	return m[0][0] * sub0 + m[0][1] * sub1 + m[0][2] * sub2;
}

constexpr double determinant_4x4(const DMat4& m) {
	/*
	 * s0 = M22 * M33 - M32 * M23
	 * s1 = M21 * M33 - M31 * M23
	 * s2 = M21 * M32 - M31 * M22
	 * s3 = M20 * M33 - M30 * M23
	 * s4 = M20 * M32 - M30 * M22
	 * s5 = M20 * M31 - M30 * M21
	 * d = M00 * (M11 * s0 - M12 * s1 + M13 * s2) -
	 *     M01 * (M10 * s0 - M12 * s3 + M13 * s4) +
	 *     M02 * (M10 * s1 - M11 * s3 + M13 * s5) -
	 *     M03 * (M10 * s2 - M11 * s4 + M12 * s5)
	 */
	double sub0 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
	double sub1 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
	double sub2 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
	double sub3 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
	double sub4 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
	double sub5 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
	return m[0][0] * (m[1][1] * sub0 - m[1][2] * sub1 + m[1][3] * sub2) -
		   m[0][1] * (m[1][0] * sub0 - m[1][2] * sub3 + m[1][3] * sub4) +
		   m[0][2] * (m[1][0] * sub1 - m[1][1] * sub3 + m[1][3] * sub5) -
		   m[0][3] * (m[1][0] * sub2 - m[1][1] * sub4 + m[1][2] * sub5);
}

constexpr DMat2 inverse_2x2(const DMat2& m) {
	/*
	 * d = M00 * M11 - M01 * M10
	 * M = [
	 *      M11 / d, -M01 / d,
	 *     -M10 / d,  M00 / d,
	 * ]
	 */
	double inv_det = 1 / (m[0][0] * m[1][1] - m[0][1] * m[1][0]);
	return {
		 inv_det * m[1][1],
		-inv_det * m[0][1],
		-inv_det * m[1][0],
		 inv_det * m[0][0],
	};
}

constexpr DMat3 inverse_3x3(const DMat3& m) {
	/*
	 * i0 = M11 * M22 - M21 * M12
	 * i1 = M12 * M20 - M10 * M22
	 * i2 = M10 * M21 - M20 * M11
	 * d = M00 * i0 + M01 * i1 + M02 * i2
	 * M = [
	 *     M11 * M22 - M21 * M12, M02 * M21 - M01 * M22, M01 * M12 - M02 * M11,
	 *     M12 * M20 - M10 * M22, M00 * M22 - M02 * M20, M10 * M02 - M00 * M12,
	 *     M10 * M21 - M20 * M11, M20 * M01 - M00 * M21, M00 * M11 - M10 * M01,
	 * ]
	 * M = M / d
	 */
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

constexpr DMat4 inverse_4x4(const DMat4& m) {
	/*
	 * s00 = M22 * M33 - M23 * M32
	 * s01 = M21 * M33 - M23 * M31
	 * s02 = M21 * M32 - M22 * M31
	 * s03 = M20 * M33 - M23 * M30
	 * s04 = M20 * M32 - M22 * M30
	 * s05 = M20 * M31 - M21 * M30
	 * s06 = M12 * M33 - M13 * M32
	 * s07 = M11 * M33 - M13 * M31
	 * s08 = M11 * M32 - M12 * M31
	 * s09 = M12 * M23 - M13 * M22
	 * s10 = M11 * M23 - M13 * M21
	 * s11 = M11 * M22 - M12 * M21
	 * s12 = M10 * M33 - M13 * M30
	 * s13 = M10 * M32 - M12 * M30
	 * s14 = M10 * M23 - M13 * M20
	 * s15 = M10 * M22 - M12 * M20
	 * s16 = M10 * M31 - M11 * M30
	 * s17 = M10 * M21 - M11 * M20
	 * i00 = M11 * s00 - M12 * s01 + M13 * s02
	 * i01 = M10 * s00 - M12 * s03 + M13 * s04
	 * i02 = M10 * s01 - M11 * s03 + M13 * s05
	 * i03 = M10 * s02 - M11 * s04 + M12 * s05
	 * d = M00 * i00 - M01 * i01 + M02 * i02 - M03 * i03
	 * M = [
	 *      (M11 * s00 - M12 * s01 + M13 * s02),
	 *     -(M01 * s00 - M02 * s01 + M03 * s02),
	 *      (M01 * s06 - M02 * s07 + M03 * s08),
	 *     -(M01 * s09 - M02 * s10 + M03 * s11),
	 *     -(M10 * s00 - M12 * s03 + M13 * s04),
	 *      (M00 * s00 - M02 * s03 + M03 * s04),
	 *     -(M00 * s06 - M02 * s12 + M03 * s13),
	 *      (M00 * s09 - M02 * s14 + M03 * s15),
	 *      (M10 * s01 - M11 * s03 + M13 * s05),
	 *     -(M00 * s01 - M01 * s03 + M03 * s05),
	 *      (M00 * s07 - M01 * s12 + M03 * s16),
	 *     -(M00 * s10 - M01 * s14 + M03 * s17),
	 *     -(M10 * s02 - M11 * s04 + M12 * s05),
	 *      (M00 * s02 - M01 * s04 + M02 * s05),
	 *     -(M00 * s08 - M01 * s13 + M02 * s16),
	 *      (M00 * s11 - M01 * s15 + M02 * s17),
	 * ]
	 * M = M / d
	 */
	double sub00 = m[2][2] * m[3][3] - m[2][3] * m[3][2];
	double sub01 = m[2][1] * m[3][3] - m[2][3] * m[3][1];
	double sub02 = m[2][1] * m[3][2] - m[2][2] * m[3][1];
	double sub03 = m[2][0] * m[3][3] - m[2][3] * m[3][0];
	double sub04 = m[2][0] * m[3][2] - m[2][2] * m[3][0];
	double sub05 = m[2][0] * m[3][1] - m[2][1] * m[3][0];
	double sub06 = m[1][2] * m[3][3] - m[1][3] * m[3][2];
	double sub07 = m[1][1] * m[3][3] - m[1][3] * m[3][1];
	double sub08 = m[1][1] * m[3][2] - m[1][2] * m[3][1];
	double sub09 = m[1][2] * m[2][3] - m[1][3] * m[2][2];
	double sub10 = m[1][1] * m[2][3] - m[1][3] * m[2][1];
	double sub11 = m[1][1] * m[2][2] - m[1][2] * m[2][1];
	double sub12 = m[1][0] * m[3][3] - m[1][3] * m[3][0];
	double sub13 = m[1][0] * m[3][2] - m[1][2] * m[3][0];
	double sub14 = m[1][0] * m[2][3] - m[1][3] * m[2][0];
	double sub15 = m[1][0] * m[2][2] - m[1][2] * m[2][0];
	double sub16 = m[1][0] * m[3][1] - m[1][1] * m[3][0];
	double sub17 = m[1][0] * m[2][1] - m[1][1] * m[2][0];
	double inv00 = m[1][1] * sub00 - m[1][2] * sub01 + m[1][3] * sub02;
	double inv01 = m[1][0] * sub00 - m[1][2] * sub03 + m[1][3] * sub04;
	double inv02 = m[1][0] * sub01 - m[1][1] * sub03 + m[1][3] * sub05;
	double inv03 = m[1][0] * sub02 - m[1][1] * sub04 + m[1][2] * sub05;
	double inv_det = 1 / (m[0][0] * inv00 - m[0][1] * inv01 + m[0][2] * inv02 - m[0][3] * inv03);
	return {
		 inv_det * inv00,
		-inv_det * (m[0][1] * sub00 - m[0][2] * sub01 + m[0][3] * sub02),
		 inv_det * (m[0][1] * sub06 - m[0][2] * sub07 + m[0][3] * sub08),
		-inv_det * (m[0][1] * sub09 - m[0][2] * sub10 + m[0][3] * sub11),
		-inv_det * inv01,
		 inv_det * (m[0][0] * sub00 - m[0][2] * sub03 + m[0][3] * sub04),
		-inv_det * (m[0][0] * sub06 - m[0][2] * sub12 + m[0][3] * sub13),
		 inv_det * (m[0][0] * sub09 - m[0][2] * sub14 + m[0][3] * sub15),
		 inv_det * inv02,
		-inv_det * (m[0][0] * sub01 - m[0][1] * sub03 + m[0][3] * sub05),
		 inv_det * (m[0][0] * sub07 - m[0][1] * sub12 + m[0][3] * sub16),
		-inv_det * (m[0][0] * sub10 - m[0][1] * sub14 + m[0][3] * sub17),
		-inv_det * inv03,
		 inv_det * (m[0][0] * sub02 - m[0][1] * sub04 + m[0][2] * sub05),
		-inv_det * (m[0][0] * sub08 - m[0][1] * sub13 + m[0][2] * sub16),
		 inv_det * (m[0][0] * sub11 - m[0][1] * sub15 + m[0][2] * sub17),
	};
}

}
