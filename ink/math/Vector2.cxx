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

#include "Vector2.h"

#include <sstream>

namespace Ink {

FVec2::FVec2(float x) : x(x), y(x) {}

FVec2::FVec2(float x, float y) : x(x), y(y) {}

FVec2 FVec2::operator-() const {
	return {-x, -y};
}

bool FVec2::operator==(const FVec2& v) const {
	return x == v.x && y == v.y;
}

void FVec2::operator+=(float v) {
	x += v;
	y += v;
}

void FVec2::operator+=(const FVec2& v) {
	x += v.x;
	y += v.y;
}

void FVec2::operator-=(float v) {
	x -= v;
	y -= v;
}

void FVec2::operator-=(const FVec2& v) {
	x -= v.x;
	y -= v.y;
}

void FVec2::operator*=(float v) {
	x *= v;
	y *= v;
}

void FVec2::operator*=(const FVec2& v) {
	x *= v.x;
	y *= v.y;
}

void FVec2::operator/=(float v) {
	x /= v;
	y /= v;
}

void FVec2::operator/=(const FVec2& v) {
	x /= v.x;
	y /= v.y;
}

float FVec2::dot(const FVec2& v) const {
	return x * v.x + y * v.y;
}

float FVec2::cross(const FVec2& v) const {
	return x * v.y - y * v.x;
}

float FVec2::magnitude() const {
	return sqrtf(x * x + y * y);
}

float FVec2::distance(const FVec2& v) const {
	return sqrtf((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y));
}

FVec2 FVec2::normalize() const {
	float l = sqrtf(x * x + y * y);
	return {x / l, y / l};
}

FVec2 FVec2::rotate(float a) const {
	return {x * cosf(a) - y * sinf(a), x * sinf(a) + y * cosf(a)};
}

std::string FVec2::to_string(int p) const {
	std::stringstream stream;
	stream.precision(p);
	stream.setf(std::ios::fixed, std::ios::floatfield);
	stream << "(" << x << ", " << y << ")";
	
	return stream.str();
}

FVec2 FVec2::random() {
	float angle = random_float() * PI * 2;
	return {cosf(angle), sinf(angle)};
}

FVec2 operator+(const FVec2& v1, float v2) {
	return {v1.x + v2, v1.y + v2};
}

FVec2 operator+(float v1, const FVec2& v2) {
	return {v1 + v2.x, v1 + v2.y};
}

FVec2 operator+(const FVec2& v1, const FVec2& v2) {
	return {v1.x + v2.x, v1.y + v2.y};
}

FVec2 operator-(const FVec2& v1, float v2) {
	return {v1.x - v2, v1.y - v2};
}

FVec2 operator-(float v1, const FVec2& v2) {
	return {v1 - v2.x, v1 - v2.y};
}

FVec2 operator-(const FVec2& v1, const FVec2& v2) {
	return {v1.x - v2.x, v1.y - v2.y};
}

FVec2 operator*(const FVec2& v1, float v2) {
	return {v1.x * v2, v1.y * v2};
}

FVec2 operator*(float v1, const FVec2& v2) {
	return {v1 * v2.x, v1 * v2.y};
}

FVec2 operator*(const FVec2& v1, const FVec2& v2) {
	return {v1.x * v2.x, v1.y * v2.y};
}

FVec2 operator/(const FVec2& v1, float v2) {
	return {v1.x / v2, v1.y / v2};
}

FVec2 operator/(float v1, const FVec2& v2) {
	return {v1 / v2.x, v1 / v2.y};
}

FVec2 operator/(const FVec2& v1, const FVec2& v2) {
	return {v1.x / v2.x, v1.y / v2.y};
}

DVec2::DVec2(double x) : x(x), y(x) {}

DVec2::DVec2(double x, double y) : x(x), y(y) {}

DVec2 DVec2::operator-() const {
	return {-x, -y};
}

bool DVec2::operator==(const DVec2& v) const {
	return x == v.x && y == v.y;
}

void DVec2::operator+=(double v) {
	x += v;
	y += v;
}

void DVec2::operator+=(const DVec2& v) {
	x += v.x;
	y += v.y;
}

void DVec2::operator-=(double v) {
	x -= v;
	y -= v;
}

void DVec2::operator-=(const DVec2& v) {
	x -= v.x;
	y -= v.y;
}

void DVec2::operator*=(double v) {
	x *= v;
	y *= v;
}

void DVec2::operator*=(const DVec2& v) {
	x *= v.x;
	y *= v.y;
}

void DVec2::operator/=(double v) {
	x /= v;
	y /= v;
}

void DVec2::operator/=(const DVec2& v) {
	x /= v.x;
	y /= v.y;
}

double DVec2::dot(const DVec2& v) const {
	return x * v.x + y * v.y;
}

double DVec2::cross(const DVec2& v) const {
	return x * v.y - y * v.x;
}

double DVec2::magnitude() const {
	return sqrt(x * x + y * y);
}

double DVec2::distance(const DVec2& v) const {
	return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y));
}

DVec2 DVec2::normalize() const {
	double l = sqrt(x * x + y * y);
	return {x / l, y / l};
}

DVec2 DVec2::rotate(double a) const {
	return {x * cos(a) - y * sin(a), x * sin(a) + y * cos(a)};
}

std::string DVec2::to_string(int p) const {
	std::stringstream stream;
	stream.precision(p);
	stream.setf(std::ios::fixed, std::ios::floatfield);
	stream << "(" << x << ", " << y << ")";
	return stream.str();
}

DVec2 DVec2::random() {
	double angle = random_double() * PI * 2;
	return {cos(angle), sin(angle)};
}

DVec2 operator+(const DVec2& v1, double v2) {
	return {v1.x + v2, v1.y + v2};
}

DVec2 operator+(double v1, const DVec2& v2) {
	return {v1 + v2.x, v1 + v2.y};
}

DVec2 operator+(const DVec2& v1, const DVec2& v2) {
	return {v1.x + v2.x, v1.y + v2.y};
}

DVec2 operator-(const DVec2& v1, double v2) {
	return {v1.x - v2, v1.y - v2};
}

DVec2 operator-(double v1, const DVec2& v2) {
	return {v1 - v2.x, v1 - v2.y};
}

DVec2 operator-(const DVec2& v1, const DVec2& v2) {
	return {v1.x - v2.x, v1.y - v2.y};
}

DVec2 operator*(const DVec2& v1, double v2) {
	return {v1.x * v2, v1.y * v2};
}

DVec2 operator*(double v1, const DVec2& v2) {
	return {v1 * v2.x, v1 * v2.y};
}

DVec2 operator*(const DVec2& v1, const DVec2& v2) {
	return {v1.x * v2.x, v1.y * v2.y};
}

DVec2 operator/(const DVec2& v1, double v2) {
	return {v1.x / v2, v1.y / v2};
}

DVec2 operator/(double v1, const DVec2& v2) {
	return {v1 / v2.x, v1 / v2.y};
}

DVec2 operator/(const DVec2& v1, const DVec2& v2) {
	return {v1.x / v2.x, v1.y / v2.y};
}

}
