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

#include "Vector4.h"

#include <sstream>

namespace Ink {

FVec4::FVec4(float x) : x(x), y(x), z(x), w(x) {}

FVec4::FVec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

FVec4::FVec4(const FVec2& v, float z, float w) : x(v.x), y(v.y), z(z), w(w) {}

FVec4::FVec4(float x, const FVec2& v, float w) : x(x), y(v.x), z(v.y), w(w) {}

FVec4::FVec4(float x, float y, const FVec2& v) : x(x), y(y), z(v.x), w(v.y) {}

FVec4::FVec4(const FVec2& v1, const FVec2& v2) : x(v1.x), y(v1.y), z(v2.x), w(v2.y) {}

FVec4::FVec4(const FVec3& v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}

FVec4::FVec4(float x, const FVec3& v) : x(x), y(v.x), z(v.y), w(v.z) {}

FVec4 FVec4::operator-() const {
	return {-x, -y, -z, -w};
}

bool FVec4::operator==(const FVec4& v) const {
	return x == v.x && y == v.y && z == v.z && w == v.w;
}

void FVec4::operator+=(float v) {
	x += v;
	y += v;
	z += v;
	w += v;
}

void FVec4::operator+=(const FVec4& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
}

void FVec4::operator-=(float v) {
	x -= v;
	y -= v;
	z -= v;
	w -= v;
}

void FVec4::operator-=(const FVec4& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
}

void FVec4::operator*=(float v) {
	x *= v;
	y *= v;
	z *= v;
	w *= v;
}

void FVec4::operator*=(const FVec4& v) {
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
}

void FVec4::operator/=(float v) {
	x /= v;
	y /= v;
	z /= v;
	w /= v;
}

void FVec4::operator/=(const FVec4& v) {
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;
}

float FVec4::dot(const FVec4& v) const {
	return x * v.x + y * v.y + z * v.z + w * v.w;
}

float FVec4::magnitude() const {
	return sqrtf(x * x + y * y + z * z + w * w);
}

float FVec4::distance(const FVec4& v) const {
	return sqrtf((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) +
				 (z - v.z) * (z - v.z) + (w - v.w) * (w - v.w));
}

FVec4 FVec4::normalize() const {
	float l = sqrtf(x * x + y * y + z * z + w * w);
	return {x / l, y / l, z / l, w / l};
}

std::string FVec4::to_string(int p) const {
	static std::stringstream stream;
	stream.setf(std::ios::fixed, std::ios::floatfield);
	stream.precision(p);
	stream.str(std::string());
	stream << "(" << x << ", " << y << ", " << z << ", " << w << ")";
	return stream.str();
}

FVec4 operator+(const FVec4& v1, float v2) {
	return {v1.x + v2, v1.y + v2, v1.z + v2, v1.w + v2};
}

FVec4 operator+(float v1, const FVec4& v2) {
	return {v1 + v2.x, v1 + v2.y, v1 + v2.z, v1 + v2.w};
}

FVec4 operator+(const FVec4& v1, const FVec4& v2) {
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
}

FVec4 operator-(const FVec4& v1, float v2) {
	return {v1.x - v2, v1.y - v2, v1.z - v2, v1.w - v2};
}

FVec4 operator-(float v1, const FVec4& v2) {
	return {v1 - v2.x, v1 - v2.y, v1 - v2.z, v1 - v2.w};
}

FVec4 operator-(const FVec4& v1, const FVec4& v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

FVec4 operator*(const FVec4& v1, float v2) {
	return {v1.x * v2, v1.y * v2, v1.z * v2, v1.w * v2};
}

FVec4 operator*(float v1, const FVec4& v2) {
	return {v1 * v2.x, v1 * v2.y, v1 * v2.z, v1 * v2.w};
}

FVec4 operator*(const FVec4& v1, const FVec4& v2) {
	return {v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w};
}

FVec4 operator/(const FVec4& v1, float v2) {
	return {v1.x / v2, v1.y / v2, v1.z / v2, v1.w / v2};
}

FVec4 operator/(float v1, const FVec4& v2) {
	return {v1 / v2.x, v1 / v2.y, v1 / v2.z, v1 / v2.w};
}

FVec4 operator/(const FVec4& v1, const FVec4& v2) {
	return {v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w};
}

DVec4::DVec4(double x) : x(x), y(x), z(x), w(x) {}

DVec4::DVec4(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}

DVec4::DVec4(const DVec2& v, double z, double w) : x(v.x), y(v.y), z(z), w(w) {}

DVec4::DVec4(double x, const DVec2& v, double w) : x(x), y(v.x), z(v.y), w(w) {}

DVec4::DVec4(double x, double y, const DVec2& v) : x(x), y(y), z(v.x), w(v.y) {}

DVec4::DVec4(const DVec2& v1, const DVec2& v2) : x(v1.x), y(v1.y), z(v2.x), w(v2.y) {}

DVec4::DVec4(const DVec3& v, double w) : x(v.x), y(v.y), z(v.z), w(w) {}

DVec4::DVec4(double x, const DVec3& v) : x(x), y(v.x), z(v.y), w(v.z) {}

DVec4 DVec4::operator-() const {
	return {-x, -y, -z, -w};
}

bool DVec4::operator==(const DVec4& v) const {
	return x == v.x && y == v.y && z == v.z && w == v.w;
}

void DVec4::operator+=(double v) {
	x += v;
	y += v;
	z += v;
	w += v;
}

void DVec4::operator+=(const DVec4& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
}

void DVec4::operator-=(double v) {
	x -= v;
	y -= v;
	z -= v;
	w -= v;
}

void DVec4::operator-=(const DVec4& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
}

void DVec4::operator*=(double v) {
	x *= v;
	y *= v;
	z *= v;
	w *= v;
}

void DVec4::operator*=(const DVec4& v) {
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
}

void DVec4::operator/=(double v) {
	x /= v;
	y /= v;
	z /= v;
	w /= v;
}

void DVec4::operator/=(const DVec4& v) {
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;
}

double DVec4::dot(const DVec4& v) const {
	return x * v.x + y * v.y + z * v.z + w * v.w;
}

double DVec4::magnitude() const {
	return sqrt(x * x + y * y + z * z + w * w);
}

double DVec4::distance(const DVec4& v) const {
	return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) +
				 (z - v.z) * (z - v.z) + (w - v.w) * (w - v.w));
}

DVec4 DVec4::normalize() const {
	double l = sqrt(x * x + y * y + z * z + w * w);
	return {x / l, y / l, z / l, w / l};
}

std::string DVec4::to_string(int p) const {
	static std::stringstream stream;
	stream.setf(std::ios::fixed, std::ios::floatfield);
	stream.precision(p);
	stream.str(std::string());
	stream << "(" << x << ", " << y << ", " << z << ", " << w << ")";
	return stream.str();
}

DVec4 operator+(const DVec4& v1, double v2) {
	return {v1.x + v2, v1.y + v2, v1.z + v2, v1.w + v2};
}

DVec4 operator+(double v1, const DVec4& v2) {
	return {v1 + v2.x, v1 + v2.y, v1 + v2.z, v1 + v2.w};
}

DVec4 operator+(const DVec4& v1, const DVec4& v2) {
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
}

DVec4 operator-(const DVec4& v1, double v2) {
	return {v1.x - v2, v1.y - v2, v1.z - v2, v1.w - v2};
}

DVec4 operator-(double v1, const DVec4& v2) {
	return {v1 - v2.x, v1 - v2.y, v1 - v2.z, v1 - v2.w};
}

DVec4 operator-(const DVec4& v1, const DVec4& v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

DVec4 operator*(const DVec4& v1, double v2) {
	return {v1.x * v2, v1.y * v2, v1.z * v2, v1.w * v2};
}

DVec4 operator*(double v1, const DVec4& v2) {
	return {v1 * v2.x, v1 * v2.y, v1 * v2.z, v1 * v2.w};
}

DVec4 operator*(const DVec4& v1, const DVec4& v2) {
	return {v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w};
}

DVec4 operator/(const DVec4& v1, double v2) {
	return {v1.x / v2, v1.y / v2, v1.z / v2, v1.w / v2};
}

DVec4 operator/(double v1, const DVec4& v2) {
	return {v1 / v2.x, v1 / v2.y, v1 / v2.z, v1 / v2.w};
}

DVec4 operator/(const DVec4& v1, const DVec4& v2) {
	return {v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w};
}

}
