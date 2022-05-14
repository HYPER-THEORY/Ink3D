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

#include "vec3.h"

fvec3::fvec3(float x) : x(x), y(x), z(x) {}
	
fvec3::fvec3(float x, float y, float z) : x(x), y(y), z(z) {}

fvec3::fvec3(const fvec2& v, float z) : x(v.x), y(v.y), z(z) {}

fvec3::fvec3(float x, const fvec2& v) : x(x), y(v.x), z(v.y) {}

fvec3::fvec3(const fvec3& v) : x(v.x), y(v.y), z(v.z) {}

fvec3 fvec3::operator-() const {
	return {-x, -y, -z};
}

void fvec3::operator+=(const fvec3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
}

void fvec3::operator-=(const fvec3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

void fvec3::operator*=(float v) {
	x *= v;
	y *= v;
	z *= v;
}

void fvec3::operator/=(float v) {
	x /= v;
	y /= v;
	z /= v;
}

float fvec3::magnitude() const {
	return sqrtf(x * x + y * y + z * z);
}

float fvec3::distance(const fvec3& v) const {
	return sqrtf((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
}

fvec3 fvec3::normalize() const {
	float l = sqrtf(x * x + y * y + z * z);
	return {x / l, y / l, z / l};
}

fvec3 fvec3::rotate(const fvec3& v, float a) const {
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

std::string fvec3::to_string(int p) const {
	std::stringstream stream;
	stream.precision(p);
	stream.setf(std::ios::fixed, std::ios::floatfield);
	stream << "(" << x << ", " << y << ", " << z << ")";
	return stream.str();
}

fvec3 fvec3::random() {
	float angle1 = randomf() * M_PI * 2;
	float angle2 = randomf() * M_PI * 2;
	return {cosf(angle1) * cosf(angle2), sinf(angle2), sinf(angle1) * cosf(angle2)};
}

fvec3 operator+(const fvec3& v1, const fvec3& v2) {
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

fvec3 operator-(const fvec3& v1, const fvec3& v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

fvec3 operator*(const fvec3& v1, float v2) {
	return {v1.x * v2, v1.y * v2, v1.z * v2};
}

fvec3 operator*(float v1, const fvec3& v2) {
	return {v2.x * v1, v2.y * v1, v2.z * v1};
}

fvec3 operator/(const fvec3& v1, float v2) {
	return {v1.x / v2, v1.y / v2, v1.z / v2};
}

float operator*(const fvec3& v1, const fvec3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

fvec3 operator^(const fvec3& v1, const fvec3& v2) {
	return {v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x};
}

dvec3::dvec3(double x) : x(x), y(x), z(x) {};

dvec3::dvec3(double x, double y, double z) : x(x), y(y), z(z) {}

dvec3::dvec3(const dvec2& v, double z) : x(v.x), y(v.y), z(z) {}

dvec3::dvec3(double x, const dvec2& v) : x(x), y(v.x), z(v.y) {}

dvec3::dvec3(const dvec3& v) : x(v.x), y(v.y), z(v.z) {}

dvec3 dvec3::operator-() const {
	return {-x, -y, -z};
}

void dvec3::operator+=(const dvec3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
}

void dvec3::operator-=(const dvec3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

void dvec3::operator*=(double v) {
	x *= v;
	y *= v;
	z *= v;
}

void dvec3::operator/=(double v) {
	x /= v;
	y /= v;
	z /= v;
}

double dvec3::magnitude() const {
	return sqrt(x * x + y * y + z * z);
}

double dvec3::distance(const dvec3& v) const {
	return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
}

dvec3 dvec3::normalize() const {
	double l = sqrt(x * x + y * y + z * z);
	return {x / l, y / l, z / l};
}

dvec3 dvec3::rotate(const dvec3& v, double a) const {
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

std::string dvec3::to_string(int p) const {
	std::stringstream stream;
	stream.precision(p);
	stream.setf(std::ios::fixed, std::ios::floatfield);
	stream << "(" << x << ", " << y << ", " << z << ")";
	return stream.str();
}

dvec3 dvec3::randomunit() {
	double angle1 = randomd() * M_PI * 2;
	double angle2 = randomd() * M_PI * 2;
	return {cos(angle1) * cos(angle2), sin(angle2), sin(angle1) * cos(angle2)};
}

dvec3 operator+(const dvec3& v1, const dvec3& v2) {
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

dvec3 operator-(const dvec3& v1, const dvec3& v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

dvec3 operator*(const dvec3& v1, double v2) {
	return {v1.x * v2, v1.y * v2, v1.z * v2};
}

dvec3 operator*(double v1, const dvec3& v2) {
	return {v2.x * v1, v2.y * v1, v2.z * v1};
}

dvec3 operator/(const dvec3& v1, double v2) {
	return {v1.x / v2, v1.y / v2, v1.z / v2};
}

double operator*(const dvec3& v1, const dvec3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

dvec3 operator^(const dvec3& v1, const dvec3& v2) {
	return {v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x};
}
