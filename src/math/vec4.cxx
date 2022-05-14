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

#include "vec4.h"

fvec4::fvec4(float x) : x(x), y(x), z(x), w(x) {}
	
fvec4::fvec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	
fvec4::fvec4(const fvec2& v, float z, float w) : x(v.x), y(v.y), z(z), w(w) {}
	
fvec4::fvec4(float x, const fvec2& v, float w) : x(x), y(v.x), z(v.y), w(w) {}
	
fvec4::fvec4(float x, float y, const fvec2& v) : x(x), y(y), z(v.x), w(v.y) {}
	
fvec4::fvec4(const fvec2& v1, const fvec2& v2) : x(v1.x), y(v1.y), z(v2.x), w(v2.y) {}
	
fvec4::fvec4(const fvec3& v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}
	
fvec4::fvec4(float x, const fvec3& v) : x(x), y(v.x), z(v.y), w(v.z) {}
	
fvec4::fvec4(const fvec4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

fvec4 fvec4::operator-() const {
	return {-x, -y, -z, -w};
}

void fvec4::operator+=(const fvec4& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
}

void fvec4::operator-=(const fvec4& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
}

void fvec4::operator*=(float v) {
	x *= v;
	y *= v;
	z *= v;
	w *= v;
}

void fvec4::operator/=(float v) {
	x /= v;
	y /= v;
	z /= v;
	w /= v;
}

float fvec4::magnitude() const {
	return sqrtf(x * x + y * y + z * z + w * w);
}

float fvec4::distance(const fvec4& v) const {
	return sqrtf((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z) + (w - v.w) * (w - v.w));
}

fvec4 fvec4::normalize() const {
	float l = sqrtf(x * x + y * y + z * z + w * w);
	return {x / l, y / l, z / l, w / l};
}

std::string fvec4::to_string(int p) const {
	std::stringstream stream;
	stream.precision(p);
	stream.setf(std::ios::fixed, std::ios::floatfield);
	stream << "(" << x << ", " << y << ", " << z << ", " << w << ")";
	return stream.str();
}

fvec4 operator+(const fvec4& v1, const fvec4& v2) {
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
}

fvec4 operator-(const fvec4& v1, const fvec4& v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

fvec4 operator*(const fvec4& v1, float v2) {
	return {v1.x * v2, v1.y * v2, v1.z * v2, v1.w * v2};
}

fvec4 operator*(float v1, const fvec4& v2) {
	return {v2.x * v1, v2.y * v1, v2.z * v1, v2.w * v1};
}

fvec4 operator/(const fvec4& v1, float v2) {
	return {v1.x / v2, v1.y / v2, v1.z / v2, v1.w / v2};
}

float operator*(const fvec4& v1, const fvec4& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

dvec4::dvec4(double x) : x(x), y(x), z(x), w(x) {}

dvec4::dvec4(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}

dvec4::dvec4(const dvec2& v, double z, double w) : x(v.x), y(v.y), z(z), w(w) {}

dvec4::dvec4(double x, const dvec2& v, double w) : x(x), y(v.x), z(v.y), w(w) {}

dvec4::dvec4(double x, double y, const dvec2& v) : x(x), y(y), z(v.x), w(v.y) {}

dvec4::dvec4(const dvec2& v1, const dvec2& v2) : x(v1.x), y(v1.y), z(v2.x), w(v2.y) {}

dvec4::dvec4(const dvec3& v, double w) : x(v.x), y(v.y), z(v.z), w(w) {}

dvec4::dvec4(double x, const dvec3& v) : x(x), y(v.x), z(v.y), w(v.z) {}

dvec4::dvec4(const dvec4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

dvec4 dvec4::operator-() const {
	return {-x, -y, -z, -w};
}

void dvec4::operator+=(const dvec4& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
}

void dvec4::operator-=(const dvec4& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
}

void dvec4::operator*=(double v) {
	x *= v;
	y *= v;
	z *= v;
	w *= v;
}

void dvec4::operator/=(double v) {
	x /= v;
	y /= v;
	z /= v;
	w /= v;
}

double dvec4::magnitude() const {
	return sqrt(x * x + y * y + z * z + w * w);
}

double dvec4::distance(const dvec4& v) const {
	return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z) + (w - v.w) * (w - v.w));
}

dvec4 dvec4::normalize() const {
	double l = sqrt(x * x + y * y + z * z + w * w);
	return {x / l, y / l, z / l, w / l};
}

std::string dvec4::to_string(int p) const {
	std::stringstream stream;
	stream.precision(p);
	stream.setf(std::ios::fixed, std::ios::floatfield);
	stream << "(" << x << ", " << y << ", " << z << ", " << w << ")";
	return stream.str();
}

dvec4 operator+(const dvec4& v1, const dvec4& v2) {
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
}

dvec4 operator-(const dvec4& v1, const dvec4& v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

dvec4 operator*(const dvec4& v1, double v2) {
	return {v1.x * v2, v1.y * v2, v1.z * v2, v1.w * v2};
}

dvec4 operator*(double v1, const dvec4& v2) {
	return {v2.x * v1, v2.y * v1, v2.z * v1, v2.w * v1};
}

dvec4 operator/(const dvec4& v1, double v2) {
	return {v1.x / v2, v1.y / v2, v1.z / v2, v1.w / v2};
}

double operator*(const dvec4& v1, const dvec4& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}
