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

#include "Vector2.h"

namespace ink {

class FVec3 {
public:
	float x = 0;    /**< the X component of the vector */
	float y = 0;    /**< the Y component of the vector */
	float z = 0;    /**< the Z component of the vector */
	
	constexpr FVec3() = default;
	
	constexpr FVec3(float x, float y, float z);
	
	constexpr FVec3(const FVec2& v, float z);
	
	constexpr FVec3(float x, const FVec2& v);
	
	constexpr explicit FVec3(float x);
	
	constexpr FVec3 operator+() const;
	
	constexpr FVec3 operator-() const;
	
	constexpr bool operator==(const FVec3& v) const;
	
	constexpr bool operator!=(const FVec3& v) const;
	
	constexpr FVec3& operator+=(float v);
	
	constexpr FVec3& operator+=(const FVec3& v);
	
	constexpr FVec3& operator-=(float v);
	
	constexpr FVec3& operator-=(const FVec3& v);
	
	constexpr FVec3& operator*=(float v);
	
	constexpr FVec3& operator*=(const FVec3& v);
	
	constexpr FVec3& operator/=(float v);
	
	constexpr FVec3& operator/=(const FVec3& v);
	
	constexpr float dot(const FVec3& v) const;
	
	constexpr FVec3 cross(const FVec3& v) const;
	
	inline float magnitude() const;
	
	inline float distance(const FVec3& v) const;
	
	inline FVec3 normalize() const;
	
	inline FVec3 rotate(const FVec3& v, float a) const;
	
	inline std::string to_string(int p = 2) const;
};

constexpr FVec3 operator+(const FVec3& v1, float v2);

constexpr FVec3 operator+(float v1, const FVec3& v2);

constexpr FVec3 operator+(const FVec3& v1, const FVec3& v2);

constexpr FVec3 operator-(const FVec3& v1, float v2);

constexpr FVec3 operator-(float v1, const FVec3& v2);

constexpr FVec3 operator-(const FVec3& v1, const FVec3& v2);

constexpr FVec3 operator*(const FVec3& v1, float v2);

constexpr FVec3 operator*(float v1, const FVec3& v2);

constexpr FVec3 operator*(const FVec3& v1, const FVec3& v2);

constexpr FVec3 operator/(const FVec3& v1, float v2);

constexpr FVec3 operator/(float v1, const FVec3& v2);

constexpr FVec3 operator/(const FVec3& v1, const FVec3& v2);

class DVec3 {
public:
	double x = 0;    /**< the X component of the vector */
	double y = 0;    /**< the Y component of the vector */
	double z = 0;    /**< the Z component of the vector */
	
	constexpr DVec3() = default;
	
	constexpr DVec3(double x, double y, double z);
	
	constexpr DVec3(const DVec2& v, double z);
	
	constexpr DVec3(double x, const DVec2& v);
	
	constexpr explicit DVec3(double x);
	
	constexpr DVec3 operator+() const;
	
	constexpr DVec3 operator-() const;
	
	constexpr bool operator==(const DVec3& v) const;
	
	constexpr bool operator!=(const DVec3& v) const;
	
	constexpr DVec3& operator+=(double v);
	
	constexpr DVec3& operator+=(const DVec3& v);
	
	constexpr DVec3& operator-=(double v);
	
	constexpr DVec3& operator-=(const DVec3& v);
	
	constexpr DVec3& operator*=(double v);
	
	constexpr DVec3& operator*=(const DVec3& v);
	
	constexpr DVec3& operator/=(double v);
	
	constexpr DVec3& operator/=(const DVec3& v);
	
	constexpr double dot(const DVec3& v) const;
	
	constexpr DVec3 cross(const DVec3& v) const;
	
	inline double magnitude() const;
	
	inline double distance(const DVec3& v) const;
	
	inline DVec3 normalize() const;
	
	inline DVec3 rotate(const DVec3& v, double a) const;
	
	inline std::string to_string(int p = 2) const;
};

constexpr DVec3 operator+(const DVec3& v1, double v2);

constexpr DVec3 operator+(double v1, const DVec3& v2);

constexpr DVec3 operator+(const DVec3& v1, const DVec3& v2);

constexpr DVec3 operator-(const DVec3& v1, double v2);

constexpr DVec3 operator-(double v1, const DVec3& v2);

constexpr DVec3 operator-(const DVec3& v1, const DVec3& v2);

constexpr DVec3 operator*(const DVec3& v1, double v2);

constexpr DVec3 operator*(double v1, const DVec3& v2);

constexpr DVec3 operator*(const DVec3& v1, const DVec3& v2);

constexpr DVec3 operator/(const DVec3& v1, double v2);

constexpr DVec3 operator/(double v1, const DVec3& v2);

constexpr DVec3 operator/(const DVec3& v1, const DVec3& v2);

using Vec3 = FVec3;

constexpr FVec3::FVec3(float x, float y, float z) : x(x), y(y), z(z) {}

constexpr FVec3::FVec3(const FVec2& v, float z) : x(v.x), y(v.y), z(z) {}

constexpr FVec3::FVec3(float x, const FVec2& v) : x(x), y(v.x), z(v.y) {}

constexpr FVec3::FVec3(float x) : x(x), y(x), z(x) {}

constexpr FVec3 FVec3::operator+() const {
	return *this;
}

constexpr FVec3 FVec3::operator-() const {
	return {-x, -y, -z};
}

constexpr bool FVec3::operator==(const FVec3& v) const {
	return x == v.x && y == v.y && z == v.z;
}

constexpr bool FVec3::operator!=(const FVec3& v) const {
	return x != v.x || y != v.y || z != v.z;
}

constexpr FVec3& FVec3::operator+=(float v) {
	x += v;
	y += v;
	z += v;
	return *this;
}

constexpr FVec3& FVec3::operator+=(const FVec3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

constexpr FVec3& FVec3::operator-=(float v) {
	x -= v;
	y -= v;
	z -= v;
	return *this;
}

constexpr FVec3& FVec3::operator-=(const FVec3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

constexpr FVec3& FVec3::operator*=(float v) {
	x *= v;
	y *= v;
	z *= v;
	return *this;
}

constexpr FVec3& FVec3::operator*=(const FVec3& v) {
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

constexpr FVec3& FVec3::operator/=(float v) {
	x /= v;
	y /= v;
	z /= v;
	return *this;
}

constexpr FVec3& FVec3::operator/=(const FVec3& v) {
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

constexpr float FVec3::dot(const FVec3& v) const {
	return x * v.x + y * v.y + z * v.z;
}

constexpr FVec3 FVec3::cross(const FVec3& v) const {
	return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
}

inline float FVec3::magnitude() const {
	return sqrtf(x * x + y * y + z * z);
}

inline float FVec3::distance(const FVec3& v) const {
	return sqrtf((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
}

inline FVec3 FVec3::normalize() const {
	float l = sqrtf(x * x + y * y + z * z);
	return {x / l, y / l, z / l};
}

inline FVec3 FVec3::rotate(const FVec3& v, float a) const {
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

inline std::string FVec3::to_string(int p) const {
	std::stringstream stream;
	stream.setf(std::ios::fixed, std::ios::floatfield);
	stream.precision(p);
	stream << "(" << x << ", " << y << ", " << z << ")";
	return stream.str();
}

constexpr FVec3 operator+(const FVec3& v1, float v2) {
	return {v1.x + v2, v1.y + v2, v1.z + v2};
}

constexpr FVec3 operator+(float v1, const FVec3& v2) {
	return {v1 + v2.x, v1 + v2.y, v1 + v2.z};
}

constexpr FVec3 operator+(const FVec3& v1, const FVec3& v2) {
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

constexpr FVec3 operator-(const FVec3& v1, float v2) {
	return {v1.x - v2, v1.y - v2, v1.z - v2};
}

constexpr FVec3 operator-(float v1, const FVec3& v2) {
	return {v1 - v2.x, v1 - v2.y, v1 - v2.z};
}

constexpr FVec3 operator-(const FVec3& v1, const FVec3& v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

constexpr FVec3 operator*(const FVec3& v1, float v2) {
	return {v1.x * v2, v1.y * v2, v1.z * v2};
}

constexpr FVec3 operator*(float v1, const FVec3& v2) {
	return {v1 * v2.x, v1 * v2.y, v1 * v2.z};
}

constexpr FVec3 operator*(const FVec3& v1, const FVec3& v2) {
	return {v1.x * v2.x, v1.y * v2.y, v1.z * v2.z};
}

constexpr FVec3 operator/(const FVec3& v1, float v2) {
	return {v1.x / v2, v1.y / v2, v1.z / v2};
}

constexpr FVec3 operator/(float v1, const FVec3& v2) {
	return {v1 / v2.x, v1 / v2.y, v1 / v2.z};
}

constexpr FVec3 operator/(const FVec3& v1, const FVec3& v2) {
	return {v1.x / v2.x, v1.y / v2.y, v1.z / v2.z};
}

constexpr DVec3::DVec3(double x, double y, double z) : x(x), y(y), z(z) {}

constexpr DVec3::DVec3(const DVec2& v, double z) : x(v.x), y(v.y), z(z) {}

constexpr DVec3::DVec3(double x, const DVec2& v) : x(x), y(v.x), z(v.y) {}

constexpr DVec3::DVec3(double x) : x(x), y(x), z(x) {}

constexpr DVec3 DVec3::operator+() const {
	return *this;
}

constexpr DVec3 DVec3::operator-() const {
	return {-x, -y, -z};
}

constexpr bool DVec3::operator==(const DVec3& v) const {
	return x == v.x && y == v.y && z == v.z;
}

constexpr bool DVec3::operator!=(const DVec3& v) const {
	return x != v.x || y != v.y || z != v.z;
}

constexpr DVec3& DVec3::operator+=(double v) {
	x += v;
	y += v;
	z += v;
	return *this;
}

constexpr DVec3& DVec3::operator+=(const DVec3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

constexpr DVec3& DVec3::operator-=(double v) {
	x -= v;
	y -= v;
	z -= v;
	return *this;
}

constexpr DVec3& DVec3::operator-=(const DVec3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

constexpr DVec3& DVec3::operator*=(double v) {
	x *= v;
	y *= v;
	z *= v;
	return *this;
}

constexpr DVec3& DVec3::operator*=(const DVec3& v) {
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

constexpr DVec3& DVec3::operator/=(double v) {
	x /= v;
	y /= v;
	z /= v;
	return *this;
}

constexpr DVec3& DVec3::operator/=(const DVec3& v) {
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

constexpr double DVec3::dot(const DVec3& v) const {
	return x * v.x + y * v.y + z * v.z;
}

constexpr DVec3 DVec3::cross(const DVec3& v) const {
	return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
}

inline double DVec3::magnitude() const {
	return sqrt(x * x + y * y + z * z);
}

inline double DVec3::distance(const DVec3& v) const {
	return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
}

inline DVec3 DVec3::normalize() const {
	double l = sqrt(x * x + y * y + z * z);
	return {x / l, y / l, z / l};
}

inline DVec3 DVec3::rotate(const DVec3& v, double a) const {
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

inline std::string DVec3::to_string(int p) const {
	std::stringstream stream;
	stream.setf(std::ios::fixed, std::ios::floatfield);
	stream.precision(p);
	stream << "(" << x << ", " << y << ", " << z << ")";
	return stream.str();
}

constexpr DVec3 operator+(const DVec3& v1, double v2) {
	return {v1.x + v2, v1.y + v2, v1.z + v2};
}

constexpr DVec3 operator+(double v1, const DVec3& v2) {
	return {v1 + v2.x, v1 + v2.y, v1 + v2.z};
}

constexpr DVec3 operator+(const DVec3& v1, const DVec3& v2) {
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

constexpr DVec3 operator-(const DVec3& v1, double v2) {
	return {v1.x - v2, v1.y - v2, v1.z - v2};
}

constexpr DVec3 operator-(double v1, const DVec3& v2) {
	return {v1 - v2.x, v1 - v2.y, v1 - v2.z};
}

constexpr DVec3 operator-(const DVec3& v1, const DVec3& v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

constexpr DVec3 operator*(const DVec3& v1, double v2) {
	return {v1.x * v2, v1.y * v2, v1.z * v2};
}

constexpr DVec3 operator*(double v1, const DVec3& v2) {
	return {v1 * v2.x, v1 * v2.y, v1 * v2.z};
}

constexpr DVec3 operator*(const DVec3& v1, const DVec3& v2) {
	return {v1.x * v2.x, v1.y * v2.y, v1.z * v2.z};
}

constexpr DVec3 operator/(const DVec3& v1, double v2) {
	return {v1.x / v2, v1.y / v2, v1.z / v2};
}

constexpr DVec3 operator/(double v1, const DVec3& v2) {
	return {v1 / v2.x, v1 / v2.y, v1 / v2.z};
}

constexpr DVec3 operator/(const DVec3& v1, const DVec3& v2) {
	return {v1.x / v2.x, v1.y / v2.y, v1.z / v2.z};
}

}
