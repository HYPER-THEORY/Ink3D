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

#include "Vector3.h"

namespace ink {

class FVec4 {
public:
	float x = 0;    /**< the X component of the vector */
	float y = 0;    /**< the Y component of the vector */
	float z = 0;    /**< the Z component of the vector */
	float w = 0;    /**< the W component of the vector */
	
	constexpr FVec4() = default;
	
	constexpr FVec4(float x, float y, float z, float w);
	
	constexpr FVec4(const FVec3& v, float w);
	
	constexpr FVec4(float x, const FVec3& v);
	
	constexpr FVec4(const FVec2& v, float z, float w);
	
	constexpr FVec4(float x, const FVec2& v, float w);
	
	constexpr FVec4(float x, float y, const FVec2& v);
	
	constexpr FVec4(const FVec2& v1, const FVec2& v2);
	
	constexpr explicit FVec4(float x);
	
	constexpr FVec4 operator+() const;
	
	constexpr FVec4 operator-() const;
	
	constexpr bool operator==(const FVec4& v) const;
	
	constexpr bool operator!=(const FVec4& v) const;
	
	constexpr FVec4& operator+=(float v);
	
	constexpr FVec4& operator+=(const FVec4& v);
	
	constexpr FVec4& operator-=(float v);
	
	constexpr FVec4& operator-=(const FVec4& v);
	
	constexpr FVec4& operator*=(float v);
	
	constexpr FVec4& operator*=(const FVec4& v);
	
	constexpr FVec4& operator/=(float v);
	
	constexpr FVec4& operator/=(const FVec4& v);
	
	constexpr float dot(const FVec4& v) const;
	
	inline float magnitude() const;
	
	inline float distance(const FVec4& v) const;
	
	inline FVec4 normalize() const;
	
	inline std::string to_string(int p = 2) const;
};

constexpr FVec4 operator+(const FVec4& v1, float v2);

constexpr FVec4 operator+(float v1, const FVec4& v2);

constexpr FVec4 operator+(const FVec4& v1, const FVec4& v2);

constexpr FVec4 operator-(const FVec4& v1, float v2);

constexpr FVec4 operator-(float v1, const FVec4& v2);

constexpr FVec4 operator-(const FVec4& v1, const FVec4& v2);

constexpr FVec4 operator*(const FVec4& v1, float v2);

constexpr FVec4 operator*(float v1, const FVec4& v2);

constexpr FVec4 operator*(const FVec4& v1, const FVec4& v2);

constexpr FVec4 operator/(const FVec4& v1, float v2);

constexpr FVec4 operator/(float v1, const FVec4& v2);

constexpr FVec4 operator/(const FVec4& v1, const FVec4& v2);

class DVec4 {
public:
	double x = 0;    /**< the X component of the vector */
	double y = 0;    /**< the Y component of the vector */
	double z = 0;    /**< the Z component of the vector */
	double w = 0;    /**< the W component of the vector */
	
	constexpr DVec4() = default;
	
	constexpr DVec4(double x, double y, double z, double w);
	
	constexpr DVec4(const DVec3& v, double w);
	
	constexpr DVec4(double x, const DVec3& v);
	
	constexpr DVec4(const DVec2& v, double z, double w);
	
	constexpr DVec4(double x, const DVec2& v, double w);
	
	constexpr DVec4(double x, double y, const DVec2& v);
	
	constexpr DVec4(const DVec2& v1, const DVec2& v2);
	
	constexpr explicit DVec4(double x);
	
	constexpr DVec4 operator+() const;
	
	constexpr DVec4 operator-() const;
	
	constexpr bool operator==(const DVec4& v) const;
	
	constexpr bool operator!=(const DVec4& v) const;
	
	constexpr DVec4& operator+=(double v);
	
	constexpr DVec4& operator+=(const DVec4& v);
	
	constexpr DVec4& operator-=(double v);
	
	constexpr DVec4& operator-=(const DVec4& v);
	
	constexpr DVec4& operator*=(double v);
	
	constexpr DVec4& operator*=(const DVec4& v);
	
	constexpr DVec4& operator/=(double v);
	
	constexpr DVec4& operator/=(const DVec4& v);
	
	constexpr double dot(const DVec4& v) const;
	
	inline double magnitude() const;
	
	inline double distance(const DVec4& v) const;
	
	inline DVec4 normalize() const;
	
	inline std::string to_string(int p = 2) const;
};

constexpr DVec4 operator+(const DVec4& v1, double v2);

constexpr DVec4 operator+(double v1, const DVec4& v2);

constexpr DVec4 operator+(const DVec4& v1, const DVec4& v2);

constexpr DVec4 operator-(const DVec4& v1, double v2);

constexpr DVec4 operator-(double v1, const DVec4& v2);

constexpr DVec4 operator-(const DVec4& v1, const DVec4& v2);

constexpr DVec4 operator*(const DVec4& v1, double v2);

constexpr DVec4 operator*(double v1, const DVec4& v2);

constexpr DVec4 operator*(const DVec4& v1, const DVec4& v2);

constexpr DVec4 operator/(const DVec4& v1, double v2);

constexpr DVec4 operator/(double v1, const DVec4& v2);

constexpr DVec4 operator/(const DVec4& v1, const DVec4& v2);

using Vec4 = FVec4;

constexpr FVec4::FVec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

constexpr FVec4::FVec4(const FVec3& v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}

constexpr FVec4::FVec4(float x, const FVec3& v) : x(x), y(v.x), z(v.y), w(v.z) {}

constexpr FVec4::FVec4(const FVec2& v, float z, float w) : x(v.x), y(v.y), z(z), w(w) {}

constexpr FVec4::FVec4(float x, const FVec2& v, float w) : x(x), y(v.x), z(v.y), w(w) {}

constexpr FVec4::FVec4(float x, float y, const FVec2& v) : x(x), y(y), z(v.x), w(v.y) {}

constexpr FVec4::FVec4(const FVec2& v1, const FVec2& v2) : x(v1.x), y(v1.y), z(v2.x), w(v2.y) {}

constexpr FVec4::FVec4(float x) : x(x), y(x), z(x), w(x) {}

constexpr FVec4 FVec4::operator+() const {
	return *this;
}

constexpr FVec4 FVec4::operator-() const {
	return {-x, -y, -z, -w};
}

constexpr bool FVec4::operator==(const FVec4& v) const {
	return x == v.x && y == v.y && z == v.z && w == v.w;
}

constexpr bool FVec4::operator!=(const FVec4& v) const {
	return x != v.x || y != v.y || z != v.z || w != v.w;
}

constexpr FVec4& FVec4::operator+=(float v) {
	x += v;
	y += v;
	z += v;
	w += v;
	return *this;
}

constexpr FVec4& FVec4::operator+=(const FVec4& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

constexpr FVec4& FVec4::operator-=(float v) {
	x -= v;
	y -= v;
	z -= v;
	w -= v;
	return *this;
}

constexpr FVec4& FVec4::operator-=(const FVec4& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

constexpr FVec4& FVec4::operator*=(float v) {
	x *= v;
	y *= v;
	z *= v;
	w *= v;
	return *this;
}

constexpr FVec4& FVec4::operator*=(const FVec4& v) {
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
	return *this;
}

constexpr FVec4& FVec4::operator/=(float v) {
	x /= v;
	y /= v;
	z /= v;
	w /= v;
	return *this;
}

constexpr FVec4& FVec4::operator/=(const FVec4& v) {
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;
	return *this;
}

constexpr float FVec4::dot(const FVec4& v) const {
	return x * v.x + y * v.y + z * v.z + w * v.w;
}

inline float FVec4::magnitude() const {
	return sqrtf(x * x + y * y + z * z + w * w);
}

inline float FVec4::distance(const FVec4& v) const {
	return sqrtf((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) +
				 (z - v.z) * (z - v.z) + (w - v.w) * (w - v.w));
}

inline FVec4 FVec4::normalize() const {
	float l = sqrtf(x * x + y * y + z * z + w * w);
	return {x / l, y / l, z / l, w / l};
}

inline std::string FVec4::to_string(int p) const {
	std::stringstream stream;
	stream.setf(std::ios::fixed, std::ios::floatfield);
	stream.precision(p);
	stream << "(" << x << ", " << y << ", " << z << ", " << w << ")";
	return stream.str();
}

constexpr FVec4 operator+(const FVec4& v1, float v2) {
	return {v1.x + v2, v1.y + v2, v1.z + v2, v1.w + v2};
}

constexpr FVec4 operator+(float v1, const FVec4& v2) {
	return {v1 + v2.x, v1 + v2.y, v1 + v2.z, v1 + v2.w};
}

constexpr FVec4 operator+(const FVec4& v1, const FVec4& v2) {
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
}

constexpr FVec4 operator-(const FVec4& v1, float v2) {
	return {v1.x - v2, v1.y - v2, v1.z - v2, v1.w - v2};
}

constexpr FVec4 operator-(float v1, const FVec4& v2) {
	return {v1 - v2.x, v1 - v2.y, v1 - v2.z, v1 - v2.w};
}

constexpr FVec4 operator-(const FVec4& v1, const FVec4& v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

constexpr FVec4 operator*(const FVec4& v1, float v2) {
	return {v1.x * v2, v1.y * v2, v1.z * v2, v1.w * v2};
}

constexpr FVec4 operator*(float v1, const FVec4& v2) {
	return {v1 * v2.x, v1 * v2.y, v1 * v2.z, v1 * v2.w};
}

constexpr FVec4 operator*(const FVec4& v1, const FVec4& v2) {
	return {v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w};
}

constexpr FVec4 operator/(const FVec4& v1, float v2) {
	return {v1.x / v2, v1.y / v2, v1.z / v2, v1.w / v2};
}

constexpr FVec4 operator/(float v1, const FVec4& v2) {
	return {v1 / v2.x, v1 / v2.y, v1 / v2.z, v1 / v2.w};
}

constexpr FVec4 operator/(const FVec4& v1, const FVec4& v2) {
	return {v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w};
}

constexpr DVec4::DVec4(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}

constexpr DVec4::DVec4(const DVec3& v, double w) : x(v.x), y(v.y), z(v.z), w(w) {}

constexpr DVec4::DVec4(double x, const DVec3& v) : x(x), y(v.x), z(v.y), w(v.z) {}

constexpr DVec4::DVec4(const DVec2& v, double z, double w) : x(v.x), y(v.y), z(z), w(w) {}

constexpr DVec4::DVec4(double x, const DVec2& v, double w) : x(x), y(v.x), z(v.y), w(w) {}

constexpr DVec4::DVec4(double x, double y, const DVec2& v) : x(x), y(y), z(v.x), w(v.y) {}

constexpr DVec4::DVec4(const DVec2& v1, const DVec2& v2) : x(v1.x), y(v1.y), z(v2.x), w(v2.y) {}

constexpr DVec4::DVec4(double x) : x(x), y(x), z(x), w(x) {}

constexpr DVec4 DVec4::operator+() const {
	return *this;
}

constexpr DVec4 DVec4::operator-() const {
	return {-x, -y, -z, -w};
}

constexpr bool DVec4::operator==(const DVec4& v) const {
	return x == v.x && y == v.y && z == v.z && w == v.w;
}

constexpr bool DVec4::operator!=(const DVec4& v) const {
	return x != v.x || y != v.y || z != v.z || w != v.w;
}

constexpr DVec4& DVec4::operator+=(double v) {
	x += v;
	y += v;
	z += v;
	w += v;
	return *this;
}

constexpr DVec4& DVec4::operator+=(const DVec4& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

constexpr DVec4& DVec4::operator-=(double v) {
	x -= v;
	y -= v;
	z -= v;
	w -= v;
	return *this;
}

constexpr DVec4& DVec4::operator-=(const DVec4& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

constexpr DVec4& DVec4::operator*=(double v) {
	x *= v;
	y *= v;
	z *= v;
	w *= v;
	return *this;
}

constexpr DVec4& DVec4::operator*=(const DVec4& v) {
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
	return *this;
}

constexpr DVec4& DVec4::operator/=(double v) {
	x /= v;
	y /= v;
	z /= v;
	w /= v;
	return *this;
}

constexpr DVec4& DVec4::operator/=(const DVec4& v) {
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;
	return *this;
}

constexpr double DVec4::dot(const DVec4& v) const {
	return x * v.x + y * v.y + z * v.z + w * v.w;
}

inline double DVec4::magnitude() const {
	return sqrt(x * x + y * y + z * z + w * w);
}

inline double DVec4::distance(const DVec4& v) const {
	return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) +
				 (z - v.z) * (z - v.z) + (w - v.w) * (w - v.w));
}

inline DVec4 DVec4::normalize() const {
	double l = sqrt(x * x + y * y + z * z + w * w);
	return {x / l, y / l, z / l, w / l};
}

inline std::string DVec4::to_string(int p) const {
	std::stringstream stream;
	stream.setf(std::ios::fixed, std::ios::floatfield);
	stream.precision(p);
	stream << "(" << x << ", " << y << ", " << z << ", " << w << ")";
	return stream.str();
}

constexpr DVec4 operator+(const DVec4& v1, double v2) {
	return {v1.x + v2, v1.y + v2, v1.z + v2, v1.w + v2};
}

constexpr DVec4 operator+(double v1, const DVec4& v2) {
	return {v1 + v2.x, v1 + v2.y, v1 + v2.z, v1 + v2.w};
}

constexpr DVec4 operator+(const DVec4& v1, const DVec4& v2) {
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
}

constexpr DVec4 operator-(const DVec4& v1, double v2) {
	return {v1.x - v2, v1.y - v2, v1.z - v2, v1.w - v2};
}

constexpr DVec4 operator-(double v1, const DVec4& v2) {
	return {v1 - v2.x, v1 - v2.y, v1 - v2.z, v1 - v2.w};
}

constexpr DVec4 operator-(const DVec4& v1, const DVec4& v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

constexpr DVec4 operator*(const DVec4& v1, double v2) {
	return {v1.x * v2, v1.y * v2, v1.z * v2, v1.w * v2};
}

constexpr DVec4 operator*(double v1, const DVec4& v2) {
	return {v1 * v2.x, v1 * v2.y, v1 * v2.z, v1 * v2.w};
}

constexpr DVec4 operator*(const DVec4& v1, const DVec4& v2) {
	return {v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w};
}

constexpr DVec4 operator/(const DVec4& v1, double v2) {
	return {v1.x / v2, v1.y / v2, v1.z / v2, v1.w / v2};
}

constexpr DVec4 operator/(double v1, const DVec4& v2) {
	return {v1 / v2.x, v1 / v2.y, v1 / v2.z, v1 / v2.w};
}

constexpr DVec4 operator/(const DVec4& v1, const DVec4& v2) {
	return {v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w};
}

}
