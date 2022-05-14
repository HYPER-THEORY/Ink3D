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

#include "vec2.h"

fvec2::fvec2(float x) : x(x), y(x) {}

fvec2::fvec2(float x, float y) : x(x), y(y) {}

fvec2::fvec2(const fvec2& v) : x(v.x), y(v.y) {}

fvec2 fvec2::operator-() const {
	return {-x, -y};
}

void fvec2::operator+=(const fvec2& v) {
	x += v.x;
	y += v.y;
}

void fvec2::operator-=(const fvec2& v) {
	x -= v.x;
	y -= v.y;
}

void fvec2::operator*=(float v) {
	x *= v;
	y *= v;
}

void fvec2::operator/=(float v) {
	x /= v;
	y /= v;
}

float fvec2::magnitude() const {
	return sqrtf(x * x + y * y);
}

float fvec2::distance(const fvec2& v) const {
	return sqrtf((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y));
}

std::string fvec2::to_string(int p) const {
	std::stringstream stream;
	stream.precision(p);
	stream.setf(std::ios::fixed, std::ios::floatfield);
	stream << "(" << x << ", " << y << ")";
	return stream.str();
}

fvec2 fvec2::normalize() const {
	float l = sqrtf(x * x + y * y);
	return {x / l, y / l};
}

fvec2 fvec2::rotate(float a) const {
	return {x * cosf(a) - y * sinf(a), x * sinf(a) + y * cosf(a)};
}

fvec2 fvec2::random() {
	float angle = randomf() * M_PI * 2;
	return {cosf(angle), sinf(angle)};
}

fvec2 operator+(const fvec2& v1, const fvec2& v2) {
	return {v1.x + v2.x, v1.y + v2.y};
}

fvec2 operator-(const fvec2& v1, const fvec2& v2) {
	return {v1.x - v2.x, v1.y - v2.y};
}

fvec2 operator*(const fvec2& v1, float v2) {
	return {v1.x * v2, v1.y * v2};
}

fvec2 operator*(float v1, const fvec2& v2) {
	return {v2.x * v1, v2.y * v1};
}

fvec2 operator/(const fvec2& v1, float v2) {
	return {v1.x / v2, v1.y / v2};
}

float operator*(const fvec2& v1, const fvec2& v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

float operator^(const fvec2& v1, const fvec2& v2) {
	return v1.x * v2.y - v1.y * v2.x;
}

dvec2::dvec2(double x) : x(x) {}

dvec2::dvec2(double x, double y) : x(x), y(y) {}

dvec2::dvec2(const dvec2& v) : x(v.x), y(v.y) {}

dvec2 dvec2::operator-() const {
	return {-x, -y};
}

void dvec2::operator+=(const dvec2& v) {
	x += v.x;
	y += v.y;
}

void dvec2::operator-=(const dvec2& v) {
	x -= v.x;
	y -= v.y;
}

void dvec2::operator*=(double v) {
	x *= v;
	y *= v;
}

void dvec2::operator/=(double v) {
	x /= v;
	y /= v;
}

double dvec2::magnitude() const {
	return sqrt(x * x + y * y);
}

double dvec2::distance(const dvec2& v) const {
	return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y));
}

dvec2 dvec2::normalize() const {
	double l = sqrt(x * x + y * y);
	return {x / l, y / l};
}

dvec2 dvec2::rotate(double a) const {
	return {x * cos(a) - y * sin(a), x * sin(a) + y * cos(a)};
}

std::string dvec2::to_string(int p) const {
	std::stringstream stream;
	stream.precision(p);
	stream.setf(std::ios::fixed, std::ios::floatfield);
	stream << "(" << x << ", " << y << ")";
	return stream.str();
}

dvec2 dvec2::randomunit() {
	double angle = randomd() * M_PI * 2;
	return {cos(angle), sin(angle)};
}

dvec2 operator+(const dvec2& v1, const dvec2& v2) {
	return {v1.x + v2.x, v1.y + v2.y};
}

dvec2 operator-(const dvec2& v1, const dvec2& v2) {
	return {v1.x - v2.x, v1.y - v2.y};
}

dvec2 operator*(const dvec2& v1, double v2) {
	return {v1.x * v2, v1.y * v2};
}

dvec2 operator*(double v1, const dvec2& v2) {
	return {v2.x * v1, v2.y * v1};
}

dvec2 operator/(const dvec2& v1, double v2) {
	return {v1.x / v2, v1.y / v2};
}

double operator*(const dvec2& v1, const dvec2& v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

double operator^(const dvec2& v1, const dvec2& v2) {
	return v1.x * v2.y - v1.y * v2.x;
}
