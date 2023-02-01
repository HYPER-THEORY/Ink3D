/**
 * Copyright (C) 2021-2023 Hypertheory
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

#include "Vector3.h"

#include <sstream>

#include "Constants.h"
#include "Random.h"

namespace Ink {

FVec3::FVec3(float x) : x(x), y(x), z(x) {}

FVec3::FVec3(float x, float y, float z) : x(x), y(y), z(z) {}

FVec3::FVec3(const FVec2& v, float z) : x(v.x), y(v.y), z(z) {}

FVec3::FVec3(float x, const FVec2& v) : x(x), y(v.x), z(v.y) {}

FVec3 FVec3::operator-() const {
	return {-x, -y, -z};
}

bool FVec3::operator==(const FVec3& v) const {
	return x == v.x && y == v.y && z == v.z;
}

void FVec3::operator+=(float v) {
	x += v;
	y += v;
	z += v;
}

void FVec3::operator+=(const FVec3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
}

void FVec3::operator-=(float v) {
	x -= v;
	y -= v;
	z -= v;
}

void FVec3::operator-=(const FVec3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

void FVec3::operator*=(float v) {
	x *= v;
	y *= v;
	z *= v;
}

void FVec3::operator*=(const FVec3& v) {
	x *= v.x;
	y *= v.y;
	z *= v.z;
}

void FVec3::operator/=(float v) {
	x /= v;
	y /= v;
	z /= v;
}

void FVec3::operator/=(const FVec3& v) {
	x /= v.x;
	y /= v.y;
	z /= v.z;
}

float FVec3::dot(const FVec3& v) const {
	return x * v.x + y * v.y + z * v.z;
}

FVec3 FVec3::cross(const FVec3& v) const {
	return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
}

float FVec3::magnitude() const {
	return sqrtf(x * x + y * y + z * z);
}

float FVec3::distance(const FVec3& v) const {
	return sqrtf((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
}

FVec3 FVec3::normalize() const {
	float l = sqrtf(x * x + y * y + z * z);
	return {x / l, y / l, z / l};
}

FVec3 FVec3::rotate(const FVec3& v, float a) const {
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

std::string FVec3::to_string(int p) const {
	static std::stringstream stream;
	stream.setf(std::ios::fixed, std::ios::floatfield);
	stream.precision(p);
	stream.str(std::string());
	stream << "(" << x << ", " << y << ", " << z << ")";
	return stream.str();
}

FVec3 FVec3::random() {
	float angle1 = Random::random_f() * PI * 2;
	float angle2 = Random::random_f() * PI * 2;
	return {cosf(angle1) * cosf(angle2), sinf(angle2), sinf(angle1) * cosf(angle2)};
}

FVec3 operator+(const FVec3& v1, float v2) {
	return {v1.x + v2, v1.y + v2, v1.z + v2};
}

FVec3 operator+(float v1, const FVec3& v2) {
	return {v1 + v2.x, v1 + v2.y, v1 + v2.z};
}

FVec3 operator+(const FVec3& v1, const FVec3& v2) {
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

FVec3 operator-(const FVec3& v1, float v2) {
	return {v1.x - v2, v1.y - v2, v1.z - v2};
}

FVec3 operator-(float v1, const FVec3& v2) {
	return {v1 - v2.x, v1 - v2.y, v1 - v2.z};
}

FVec3 operator-(const FVec3& v1, const FVec3& v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

FVec3 operator*(const FVec3& v1, float v2) {
	return {v1.x * v2, v1.y * v2, v1.z * v2};
}

FVec3 operator*(float v1, const FVec3& v2) {
	return {v1 * v2.x, v1 * v2.y, v1 * v2.z};
}

FVec3 operator*(const FVec3& v1, const FVec3& v2) {
	return {v1.x * v2.x, v1.y * v2.y, v1.z * v2.z};
}

FVec3 operator/(const FVec3& v1, float v2) {
	return {v1.x / v2, v1.y / v2, v1.z / v2};
}

FVec3 operator/(float v1, const FVec3& v2) {
	return {v1 / v2.x, v1 / v2.y, v1 / v2.z};
}

FVec3 operator/(const FVec3& v1, const FVec3& v2) {
	return {v1.x / v2.x, v1.y / v2.y, v1.z / v2.z};
}

DVec3::DVec3(double x) : x(x), y(x), z(x) {}
	
DVec3::DVec3(double x, double y, double z) : x(x), y(y), z(z) {}

DVec3::DVec3(const DVec2& v, double z) : x(v.x), y(v.y), z(z) {}

DVec3::DVec3(double x, const DVec2& v) : x(x), y(v.x), z(v.y) {}

DVec3 DVec3::operator-() const {
	return {-x, -y, -z};
}

bool DVec3::operator==(const DVec3& v) const {
	return x == v.x && y == v.y && z == v.z;
}

void DVec3::operator+=(double v) {
	x += v;
	y += v;
	z += v;
}

void DVec3::operator+=(const DVec3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
}

void DVec3::operator-=(double v) {
	x -= v;
	y -= v;
	z -= v;
}

void DVec3::operator-=(const DVec3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

void DVec3::operator*=(double v) {
	x *= v;
	y *= v;
	z *= v;
}

void DVec3::operator*=(const DVec3& v) {
	x *= v.x;
	y *= v.y;
	z *= v.z;
}

void DVec3::operator/=(double v) {
	x /= v;
	y /= v;
	z /= v;
}

void DVec3::operator/=(const DVec3& v) {
	x /= v.x;
	y /= v.y;
	z /= v.z;
}

double DVec3::dot(const DVec3& v) const {
	return x * v.x + y * v.y + z * v.z;
}

DVec3 DVec3::cross(const DVec3& v) const {
	return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
}

double DVec3::magnitude() const {
	return sqrt(x * x + y * y + z * z);
}

double DVec3::distance(const DVec3& v) const {
	return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
}

DVec3 DVec3::normalize() const {
	double l = sqrt(x * x + y * y + z * z);
	return {x / l, y / l, z / l};
}

DVec3 DVec3::rotate(const DVec3& v, double a) const {
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

std::string DVec3::to_string(int p) const {
	static std::stringstream stream;
	stream.setf(std::ios::fixed, std::ios::floatfield);
	stream.precision(p);
	stream.str(std::string());
	stream << "(" << x << ", " << y << ", " << z << ")";
	return stream.str();
}

DVec3 DVec3::random() {
	double angle1 = Random::random() * PI * 2;
	double angle2 = Random::random() * PI * 2;
	return {cos(angle1) * cos(angle2), sin(angle2), sin(angle1) * cos(angle2)};
}

DVec3 operator+(const DVec3& v1, double v2) {
	return {v1.x + v2, v1.y + v2, v1.z + v2};
}

DVec3 operator+(double v1, const DVec3& v2) {
	return {v1 + v2.x, v1 + v2.y, v1 + v2.z};
}

DVec3 operator+(const DVec3& v1, const DVec3& v2) {
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

DVec3 operator-(const DVec3& v1, double v2) {
	return {v1.x - v2, v1.y - v2, v1.z - v2};
}

DVec3 operator-(double v1, const DVec3& v2) {
	return {v1 - v2.x, v1 - v2.y, v1 - v2.z};
}

DVec3 operator-(const DVec3& v1, const DVec3& v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

DVec3 operator*(const DVec3& v1, double v2) {
	return {v1.x * v2, v1.y * v2, v1.z * v2};
}

DVec3 operator*(double v1, const DVec3& v2) {
	return {v1 * v2.x, v1 * v2.y, v1 * v2.z};
}

DVec3 operator*(const DVec3& v1, const DVec3& v2) {
	return {v1.x * v2.x, v1.y * v2.y, v1.z * v2.z};
}

DVec3 operator/(const DVec3& v1, double v2) {
	return {v1.x / v2, v1.y / v2, v1.z / v2};
}

DVec3 operator/(double v1, const DVec3& v2) {
	return {v1 / v2.x, v1 / v2.y, v1 / v2.z};
}

DVec3 operator/(const DVec3& v1, const DVec3& v2) {
	return {v1.x / v2.x, v1.y / v2.y, v1.z / v2.z};
}

}
