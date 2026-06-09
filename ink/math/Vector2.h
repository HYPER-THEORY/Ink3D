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

#include "Constants.h"

#include <cmath>
#include <sstream>
#include <string>

namespace ink {

class FVec2 {
public:
	float x = 0;    /**< the X component of the vector */
	float y = 0;    /**< the Y component of the vector */
	
	constexpr FVec2() = default;
	
	constexpr FVec2(float x, float y);
	
	constexpr explicit FVec2(float x);
	
	constexpr FVec2 operator+() const;
	
	constexpr FVec2 operator-() const;
	
	constexpr bool operator==(const FVec2& v) const;
	
	constexpr bool operator!=(const FVec2& v) const;
	
	constexpr FVec2& operator+=(float v);
	
	constexpr FVec2& operator+=(const FVec2& v);
	
	constexpr FVec2& operator-=(float v);
	
	constexpr FVec2& operator-=(const FVec2& v);
	
	constexpr FVec2& operator*=(float v);
	
	constexpr FVec2& operator*=(const FVec2& v);
	
	constexpr FVec2& operator/=(float v);
	
	constexpr FVec2& operator/=(const FVec2& v);
	
	constexpr float dot(const FVec2& v) const;
	
	constexpr float cross(const FVec2& v) const;
	
	inline float magnitude() const;
	
	inline float distance(const FVec2& v) const;
	
	inline FVec2 normalize() const;
	
	inline FVec2 rotate(float a) const;
	
	inline std::string to_string(int p = 2) const;
};

constexpr FVec2 operator+(const FVec2& v1, float v2);

constexpr FVec2 operator+(float v1, const FVec2& v2);

constexpr FVec2 operator+(const FVec2& v1, const FVec2& v2);

constexpr FVec2 operator-(const FVec2& v1, float v2);

constexpr FVec2 operator-(float v1, const FVec2& v2);

constexpr FVec2 operator-(const FVec2& v1, const FVec2& v2);

constexpr FVec2 operator*(const FVec2& v1, float v2);

constexpr FVec2 operator*(float v1, const FVec2& v2);

constexpr FVec2 operator*(const FVec2& v1, const FVec2& v2);

constexpr FVec2 operator/(const FVec2& v1, float v2);

constexpr FVec2 operator/(float v1, const FVec2& v2);

constexpr FVec2 operator/(const FVec2& v1, const FVec2& v2);

class DVec2 {
public:
	double x = 0;    /**< the X component of the vector */
	double y = 0;    /**< the Y component of the vector */
	
	constexpr DVec2() = default;
	
	constexpr DVec2(double x, double y);
	
	constexpr explicit DVec2(double x);
	
	constexpr DVec2 operator+() const;
	
	constexpr DVec2 operator-() const;
	
	constexpr bool operator==(const DVec2& v) const;
	
	constexpr bool operator!=(const DVec2& v) const;
	
	constexpr DVec2& operator+=(double v);
	
	constexpr DVec2& operator+=(const DVec2& v);
	
	constexpr DVec2& operator-=(double v);
	
	constexpr DVec2& operator-=(const DVec2& v);
	
	constexpr DVec2& operator*=(double v);
	
	constexpr DVec2& operator*=(const DVec2& v);
	
	constexpr DVec2& operator/=(double v);
	
	constexpr DVec2& operator/=(const DVec2& v);
	
	constexpr double dot(const DVec2& v) const;
	
	constexpr double cross(const DVec2& v) const;
	
	inline double magnitude() const;
	
	inline double distance(const DVec2& v) const;
	
	inline DVec2 normalize() const;
	
	inline DVec2 rotate(double a) const;
	
	inline std::string to_string(int p = 2) const;
};

constexpr DVec2 operator+(const DVec2& v1, double v2);

constexpr DVec2 operator+(double v1, const DVec2& v2);

constexpr DVec2 operator+(const DVec2& v1, const DVec2& v2);

constexpr DVec2 operator-(const DVec2& v1, double v2);

constexpr DVec2 operator-(double v1, const DVec2& v2);

constexpr DVec2 operator-(const DVec2& v1, const DVec2& v2);

constexpr DVec2 operator*(const DVec2& v1, double v2);

constexpr DVec2 operator*(double v1, const DVec2& v2);

constexpr DVec2 operator*(const DVec2& v1, const DVec2& v2);

constexpr DVec2 operator/(const DVec2& v1, double v2);

constexpr DVec2 operator/(double v1, const DVec2& v2);

constexpr DVec2 operator/(const DVec2& v1, const DVec2& v2);

using Vec2 = FVec2;

constexpr FVec2::FVec2(float x, float y) : x(x), y(y) {}

constexpr FVec2::FVec2(float x) : x(x), y(x) {}

constexpr FVec2 FVec2::operator+() const {
	return *this;
}

constexpr FVec2 FVec2::operator-() const {
	return {-x, -y};
}

constexpr bool FVec2::operator==(const FVec2& v) const {
	return x == v.x && y == v.y;
}

constexpr bool FVec2::operator!=(const FVec2& v) const {
	return x != v.x || y != v.y;
}

constexpr FVec2& FVec2::operator+=(float v) {
	x += v;
	y += v;
	return *this;
}

constexpr FVec2& FVec2::operator+=(const FVec2& v) {
	x += v.x;
	y += v.y;
	return *this;
}

constexpr FVec2& FVec2::operator-=(float v) {
	x -= v;
	y -= v;
	return *this;
}

constexpr FVec2& FVec2::operator-=(const FVec2& v) {
	x -= v.x;
	y -= v.y;
	return *this;
}

constexpr FVec2& FVec2::operator*=(float v) {
	x *= v;
	y *= v;
	return *this;
}

constexpr FVec2& FVec2::operator*=(const FVec2& v) {
	x *= v.x;
	y *= v.y;
	return *this;
}

constexpr FVec2& FVec2::operator/=(float v) {
	x /= v;
	y /= v;
	return *this;
}

constexpr FVec2& FVec2::operator/=(const FVec2& v) {
	x /= v.x;
	y /= v.y;
	return *this;
}

constexpr float FVec2::dot(const FVec2& v) const {
	return x * v.x + y * v.y;
}

constexpr float FVec2::cross(const FVec2& v) const {
	return x * v.y - y * v.x;
}

inline float FVec2::magnitude() const {
	return sqrtf(x * x + y * y);
}

inline float FVec2::distance(const FVec2& v) const {
	return sqrtf((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y));
}

inline FVec2 FVec2::normalize() const {
	float l = sqrtf(x * x + y * y);
	return {x / l, y / l};
}

inline FVec2 FVec2::rotate(float a) const {
	return {x * cosf(a) - y * sinf(a), x * sinf(a) + y * cosf(a)};
}

inline std::string FVec2::to_string(int p) const {
	std::stringstream stream;
	stream.setf(std::ios::fixed, std::ios::floatfield);
	stream.precision(p);
	stream << "(" << x << ", " << y << ")";
	return stream.str();
}

constexpr FVec2 operator+(const FVec2& v1, float v2) {
	return {v1.x + v2, v1.y + v2};
}

constexpr FVec2 operator+(float v1, const FVec2& v2) {
	return {v1 + v2.x, v1 + v2.y};
}

constexpr FVec2 operator+(const FVec2& v1, const FVec2& v2) {
	return {v1.x + v2.x, v1.y + v2.y};
}

constexpr FVec2 operator-(const FVec2& v1, float v2) {
	return {v1.x - v2, v1.y - v2};
}

constexpr FVec2 operator-(float v1, const FVec2& v2) {
	return {v1 - v2.x, v1 - v2.y};
}

constexpr FVec2 operator-(const FVec2& v1, const FVec2& v2) {
	return {v1.x - v2.x, v1.y - v2.y};
}

constexpr FVec2 operator*(const FVec2& v1, float v2) {
	return {v1.x * v2, v1.y * v2};
}

constexpr FVec2 operator*(float v1, const FVec2& v2) {
	return {v1 * v2.x, v1 * v2.y};
}

constexpr FVec2 operator*(const FVec2& v1, const FVec2& v2) {
	return {v1.x * v2.x, v1.y * v2.y};
}

constexpr FVec2 operator/(const FVec2& v1, float v2) {
	return {v1.x / v2, v1.y / v2};
}

constexpr FVec2 operator/(float v1, const FVec2& v2) {
	return {v1 / v2.x, v1 / v2.y};
}

constexpr FVec2 operator/(const FVec2& v1, const FVec2& v2) {
	return {v1.x / v2.x, v1.y / v2.y};
}

constexpr DVec2::DVec2(double x, double y) : x(x), y(y) {}

constexpr DVec2::DVec2(double x) : x(x), y(x) {}

constexpr DVec2 DVec2::operator+() const {
	return *this;
}

constexpr DVec2 DVec2::operator-() const {
	return {-x, -y};
}

constexpr bool DVec2::operator==(const DVec2& v) const {
	return x == v.x && y == v.y;
}

constexpr bool DVec2::operator!=(const DVec2& v) const {
	return x != v.x || y != v.y;
}

constexpr DVec2& DVec2::operator+=(double v) {
	x += v;
	y += v;
	return *this;
}

constexpr DVec2& DVec2::operator+=(const DVec2& v) {
	x += v.x;
	y += v.y;
	return *this;
}

constexpr DVec2& DVec2::operator-=(double v) {
	x -= v;
	y -= v;
	return *this;
}

constexpr DVec2& DVec2::operator-=(const DVec2& v) {
	x -= v.x;
	y -= v.y;
	return *this;
}

constexpr DVec2& DVec2::operator*=(double v) {
	x *= v;
	y *= v;
	return *this;
}

constexpr DVec2& DVec2::operator*=(const DVec2& v) {
	x *= v.x;
	y *= v.y;
	return *this;
}

constexpr DVec2& DVec2::operator/=(double v) {
	x /= v;
	y /= v;
	return *this;
}

constexpr DVec2& DVec2::operator/=(const DVec2& v) {
	x /= v.x;
	y /= v.y;
	return *this;
}

constexpr double DVec2::dot(const DVec2& v) const {
	return x * v.x + y * v.y;
}

constexpr double DVec2::cross(const DVec2& v) const {
	return x * v.y - y * v.x;
}

inline double DVec2::magnitude() const {
	return sqrt(x * x + y * y);
}

inline double DVec2::distance(const DVec2& v) const {
	return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y));
}

inline DVec2 DVec2::normalize() const {
	double l = sqrt(x * x + y * y);
	return {x / l, y / l};
}

inline DVec2 DVec2::rotate(double a) const {
	return {x * cos(a) - y * sin(a), x * sin(a) + y * cos(a)};
}

inline std::string DVec2::to_string(int p) const {
	std::stringstream stream;
	stream.setf(std::ios::fixed, std::ios::floatfield);
	stream.precision(p);
	stream << "(" << x << ", " << y << ")";
	return stream.str();
}

constexpr DVec2 operator+(const DVec2& v1, double v2) {
	return {v1.x + v2, v1.y + v2};
}

constexpr DVec2 operator+(double v1, const DVec2& v2) {
	return {v1 + v2.x, v1 + v2.y};
}

constexpr DVec2 operator+(const DVec2& v1, const DVec2& v2) {
	return {v1.x + v2.x, v1.y + v2.y};
}

constexpr DVec2 operator-(const DVec2& v1, double v2) {
	return {v1.x - v2, v1.y - v2};
}

constexpr DVec2 operator-(double v1, const DVec2& v2) {
	return {v1 - v2.x, v1 - v2.y};
}

constexpr DVec2 operator-(const DVec2& v1, const DVec2& v2) {
	return {v1.x - v2.x, v1.y - v2.y};
}

constexpr DVec2 operator*(const DVec2& v1, double v2) {
	return {v1.x * v2, v1.y * v2};
}

constexpr DVec2 operator*(double v1, const DVec2& v2) {
	return {v1 * v2.x, v1 * v2.y};
}

constexpr DVec2 operator*(const DVec2& v1, const DVec2& v2) {
	return {v1.x * v2.x, v1.y * v2.y};
}

constexpr DVec2 operator/(const DVec2& v1, double v2) {
	return {v1.x / v2, v1.y / v2};
}

constexpr DVec2 operator/(double v1, const DVec2& v2) {
	return {v1 / v2.x, v1 / v2.y};
}

constexpr DVec2 operator/(const DVec2& v1, const DVec2& v2) {
	return {v1.x / v2.x, v1.y / v2.y};
}

}
