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

#include "Math.h"

namespace WR {

Vec2f::Vec2f(float x) : x(x), y(x) {}

Vec2f::Vec2f(float x, float y) : x(x), y(y) {}

Vec2f::Vec2f(const Vec2f& v) : x(v.x), y(v.y) {}

Vec2f Vec2f::operator-() const {
	return {-x, -y};
}

void Vec2f::operator+=(const Vec2f& v) {
	x += v.x;
	y += v.y;
}

void Vec2f::operator-=(const Vec2f& v) {
	x -= v.x;
	y -= v.y;
}

void Vec2f::operator*=(float v) {
	x *= v;
	y *= v;
}

void Vec2f::operator/=(float v) {
	x /= v;
	y /= v;
}

float Vec2f::magnitude() const {
	return sqrtf(x * x + y * y);
}

float Vec2f::distance(const Vec2f& v) const {
	return sqrtf((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y));
}

Vec2f Vec2f::normalize() const {
	float l = sqrtf(x * x + y * y);
	return {x / l, y / l};
}

Vec2f Vec2f::rotate(float a) const {
	return {x * cosf(a) - y * sinf(a), x * sinf(a) + y * cosf(a)};
}

Vec2f Vec2f::random() {
	float angle = randomf() * M_PI * 2;
	return {cosf(angle), sinf(angle)};
}

Vec2f operator+(const Vec2f& v1, const Vec2f& v2) {
	return {v1.x + v2.x, v1.y + v2.y};
}

Vec2f operator-(const Vec2f& v1, const Vec2f& v2) {
	return {v1.x - v2.x, v1.y - v2.y};
}

Vec2f operator*(const Vec2f& v1, float v2) {
	return {v1.x * v2, v1.y * v2};
}

Vec2f operator*(float v1, const Vec2f& v2) {
	return {v2.x * v1, v2.y * v1};
}

Vec2f operator/(const Vec2f& v1, float v2) {
	return {v1.x / v2, v1.y / v2};
}

float operator*(const Vec2f& v1, const Vec2f& v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

float operator^(const Vec2f& v1, const Vec2f& v2) {
	return v1.x * v2.y - v1.y * v2.x;
}

Vec3f::Vec3f(float x) : x(x), y(x), z(x) {}
	
Vec3f::Vec3f(float x, float y, float z) : x(x), y(y), z(z) {}

Vec3f::Vec3f(const Vec2f& v, float z) : x(v.x), y(v.y), z(z) {}

Vec3f::Vec3f(float x, const Vec2f& v) : x(x), y(v.x), z(v.y) {}

Vec3f::Vec3f(const Vec3f& v) : x(v.x), y(v.y), z(v.z) {}

Vec3f Vec3f::operator-() const {
	return {-x, -y, -z};
}

void Vec3f::operator+=(const Vec3f& v) {
	x += v.x;
	y += v.y;
	z += v.z;
}

void Vec3f::operator-=(const Vec3f& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

void Vec3f::operator*=(float v) {
	x *= v;
	y *= v;
	z *= v;
}

void Vec3f::operator/=(float v) {
	x /= v;
	y /= v;
	z /= v;
}

float Vec3f::magnitude() const {
	return sqrtf(x * x + y * y + z * z);
}

float Vec3f::distance(const Vec3f& v) const {
	return sqrtf((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
}

Vec3f Vec3f::normalize() const {
	float l = sqrtf(x * x + y * y + z * z);
	return {x / l, y / l, z / l};
}

Vec3f Vec3f::rotate(const Vec3f& v, float a) const {
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

Vec3f Vec3f::random() {
	float angle1 = randomf() * M_PI * 2;
	float angle2 = randomf() * M_PI * 2;
	return {cosf(angle1) * cosf(angle2), sinf(angle2), sinf(angle1) * cosf(angle2)};
}

Vec3f operator+(const Vec3f& v1, const Vec3f& v2) {
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

Vec3f operator-(const Vec3f& v1, const Vec3f& v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

Vec3f operator*(const Vec3f& v1, float v2) {
	return {v1.x * v2, v1.y * v2, v1.z * v2};
}

Vec3f operator*(float v1, const Vec3f& v2) {
	return {v2.x * v1, v2.y * v1, v2.z * v1};
}

Vec3f operator/(const Vec3f& v1, float v2) {
	return {v1.x / v2, v1.y / v2, v1.z / v2};
}

float operator*(const Vec3f& v1, const Vec3f& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vec3f operator^(const Vec3f& v1, const Vec3f& v2) {
	return {v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x};
}

Vec4f::Vec4f(float x) : x(x), y(x), z(x), w(x) {};
	
Vec4f::Vec4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	
Vec4f::Vec4f(const Vec2f& v, float z, float w) : x(v.x), y(v.y), z(z), w(w) {}
	
Vec4f::Vec4f(float x, const Vec2f& v, float w) : x(x), y(v.x), z(v.y), w(w) {}
	
Vec4f::Vec4f(float x, float y, const Vec2f& v) : x(x), y(y), z(v.x), w(v.y) {}
	
Vec4f::Vec4f(const Vec2f& v1, const Vec2f& v2) : x(v1.x), y(v1.y), z(v2.x), w(v2.y) {}
	
Vec4f::Vec4f(const Vec3f& v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}
	
Vec4f::Vec4f(float x, const Vec3f& v) : x(x), y(v.x), z(v.y), w(v.z) {}
	
Vec4f::Vec4f(const Vec4f& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

Vec4f Vec4f::operator-() const {
	return {-x, -y, -z, -w};
}

void Vec4f::operator+=(const Vec4f& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
}

void Vec4f::operator-=(const Vec4f& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
}

void Vec4f::operator*=(float v) {
	x *= v;
	y *= v;
	z *= v;
	w *= v;
}

void Vec4f::operator/=(float v) {
	x /= v;
	y /= v;
	z /= v;
	w /= v;
}

float Vec4f::magnitude() const {
	return sqrtf(x * x + y * y + z * z + w * w);
}

float Vec4f::distance(const Vec4f& v) const {
	return sqrtf((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z) + (w - v.w) * (w - v.w));
}

Vec4f Vec4f::normalize() const {
	float l = sqrtf(x * x + y * y + z * z + w * w);
	return {x / l, y / l, z / l, w / l};
}

Vec4f operator+(const Vec4f& v1, const Vec4f& v2) {
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
}

Vec4f operator-(const Vec4f& v1, const Vec4f& v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

Vec4f operator*(const Vec4f& v1, float v2) {
	return {v1.x * v2, v1.y * v2, v1.z * v2, v1.w * v2};
}

Vec4f operator*(float v1, const Vec4f& v2) {
	return {v2.x * v1, v2.y * v1, v2.z * v1, v2.w * v1};
}

Vec4f operator/(const Vec4f& v1, float v2) {
	return {v1.x / v2, v1.y / v2, v1.z / v2, v1.w / v2};
}

float mat2_determinant(const Mat2& m) {
	return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

float mat3_determinant(const Mat3& m) {
	/* sub-factor */
	float sub0 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
	float sub1 = m[1][2] * m[2][0] - m[1][0] * m[2][2];
	float sub2 = m[1][0] * m[2][1] - m[2][0] * m[1][1];
	return m[0][0] * sub0 + m[0][1] * sub1 + m[0][2] * sub2;
}

float mat4_determinant(const Mat4& m) {
	/* sub-factor */
	float sub0 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
	float sub1 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
	float sub2 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
	float sub3 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
	float sub4 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
	float sub5 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
	return m[0][0] * (m[1][1] * sub0 - m[1][2] * sub1 + m[1][3] * sub2) -
		m[0][1] * (m[1][0] * sub0 - m[1][2] * sub3 + m[1][3] * sub4) +
		m[0][2] * (m[1][0] * sub1 - m[1][1] * sub3 + m[1][3] * sub5) -
		m[0][3] * (m[1][0] * sub2 - m[1][1] * sub4 + m[1][2] * sub5);
}

Matf<2, 2> mat2_inverse(const Mat2& m) {
	/* inverse determinant */
	float inv_det = 1 / (m[0][0] * m[1][1] - m[0][1] * m[1][0]);
	return {
		 inv_det * m[1][1],
		-inv_det * m[0][1],
		-inv_det * m[1][0],
		 inv_det * m[0][0],
	};
}

Matf<3, 3> mat3_inverse(const Mat3& m) {
	/* inverse */
	float inv0 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
	float inv1 = m[1][2] * m[2][0] - m[1][0] * m[2][2];
	float inv2 = m[1][0] * m[2][1] - m[2][0] * m[1][1];
	/* inverse determinant */
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

Matf<4, 4> mat4_inverse(const Mat4& m) {
	/* sub-factor */
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
	/* inverse */
	float inv0 = m[1][1] * sub00 - m[1][2] * sub01 + m[1][3] * sub02;
	float inv1 = m[1][0] * sub00 - m[1][2] * sub03 + m[1][3] * sub04;
	float inv2 = m[1][0] * sub01 - m[1][1] * sub03 + m[1][3] * sub05;
	float inv3 = m[1][0] * sub02 - m[1][1] * sub04 + m[1][2] * sub05;
	/* inverse determinant */
	float inv_det = 1 / (m[0][0] * inv0 - m[0][1] * inv1 + m[0][2] * inv2 - m[0][3] * inv3);
	return {
		 inv_det * inv0,
		-inv_det * (m[0][1] * sub00 - m[0][2] * sub01 + m[0][3] * sub02),
		 inv_det * (m[0][1] * sub06 - m[0][2] * sub07 + m[0][3] * sub08),
		-inv_det * (m[0][1] * sub09 - m[0][2] * sub10 + m[0][3] * sub11),
		-inv_det * inv1,
		 inv_det * (m[0][0] * sub00 - m[0][2] * sub03 + m[0][3] * sub04),
		-inv_det * (m[0][0] * sub06 - m[0][2] * sub12 + m[0][3] * sub13),
		 inv_det * (m[0][0] * sub09 - m[0][2] * sub14 + m[0][3] * sub15),
		 inv_det * inv2,
		-inv_det * (m[0][0] * sub01 - m[0][1] * sub03 + m[0][3] * sub05),
		 inv_det * (m[0][0] * sub07 - m[0][1] * sub12 + m[0][3] * sub16),
		-inv_det * (m[0][0] * sub10 - m[0][1] * sub14 + m[0][3] * sub17),
		-inv_det * inv3,
		 inv_det * (m[0][0] * sub02 - m[0][1] * sub04 + m[0][2] * sub05),
		-inv_det * (m[0][0] * sub08 - m[0][1] * sub13 + m[0][2] * sub16),
		 inv_det * (m[0][0] * sub11 - m[0][1] * sub15 + m[0][2] * sub17),
	};
}

Vec2d::Vec2d(double x) : x(x) {}

Vec2d::Vec2d(double x, double y) : x(x), y(y) {}

Vec2d::Vec2d(const Vec2d& v) : x(v.x), y(v.y) {}

Vec2d Vec2d::operator-() const {
	return {-x, -y};
}

void Vec2d::operator+=(const Vec2d& v) {
	x += v.x;
	y += v.y;
}

void Vec2d::operator-=(const Vec2d& v) {
	x -= v.x;
	y -= v.y;
}

void Vec2d::operator*=(double v) {
	x *= v;
	y *= v;
}

void Vec2d::operator/=(double v) {
	x /= v;
	y /= v;
}

double Vec2d::magnitude() const {
	return sqrt(x * x + y * y);
}

double Vec2d::distance(const Vec2d& v) const {
	return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y));
}

Vec2d Vec2d::normalize() const {
	double l = sqrt(x * x + y * y);
	return {x / l, y / l};
}

Vec2d Vec2d::rotate(double a) const {
	return {x * cos(a) - y * sin(a), x * sin(a) + y * cos(a)};
}

Vec2d Vec2d::randomunit() {
	double angle = randomd() * M_PI * 2;
	return {cos(angle), sin(angle)};
}

Vec2d operator+(const Vec2d& v1, const Vec2d& v2) {
	return {v1.x + v2.x, v1.y + v2.y};
}

Vec2d operator-(const Vec2d& v1, const Vec2d& v2) {
	return {v1.x - v2.x, v1.y - v2.y};
}

Vec2d operator*(const Vec2d& v1, double v2) {
	return {v1.x * v2, v1.y * v2};
}

Vec2d operator*(double v1, const Vec2d& v2) {
	return {v2.x * v1, v2.y * v1};
}

Vec2d operator/(const Vec2d& v1, double v2) {
	return {v1.x / v2, v1.y / v2};
}

double operator*(const Vec2d& v1, const Vec2d& v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

double operator^(const Vec2d& v1, const Vec2d& v2) {
	return v1.x * v2.y - v1.y * v2.x;
}

Vec3d::Vec3d(double x) : x(x), y(x), z(x) {};

Vec3d::Vec3d(double x, double y, double z) : x(x), y(y), z(z) {}

Vec3d::Vec3d(const Vec2d& v, double z) : x(v.x), y(v.y), z(z) {}

Vec3d::Vec3d(double x, const Vec2d& v) : x(x), y(v.x), z(v.y) {}

Vec3d::Vec3d(const Vec3d& v) : x(v.x), y(v.y), z(v.z) {}

Vec3d Vec3d::operator-() const {
	return {-x, -y, -z};
}

void Vec3d::operator+=(const Vec3d& v) {
	x += v.x;
	y += v.y;
	z += v.z;
}

void Vec3d::operator-=(const Vec3d& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

void Vec3d::operator*=(double v) {
	x *= v;
	y *= v;
	z *= v;
}

void Vec3d::operator/=(double v) {
	x /= v;
	y /= v;
	z /= v;
}

double Vec3d::magnitude() const {
	return sqrt(x * x + y * y + z * z);
}

double Vec3d::distance(const Vec3d& v) const {
	return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
}

Vec3d Vec3d::normalize() const {
	double l = sqrt(x * x + y * y + z * z);
	return {x / l, y / l, z / l};
}

Vec3d Vec3d::rotate(const Vec3d& v, double a) const {
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

Vec3d Vec3d::randomunit() {
	double angle1 = randomd() * M_PI * 2;
	double angle2 = randomd() * M_PI * 2;
	return {cos(angle1) * cos(angle2), sin(angle2), sin(angle1) * cos(angle2)};
}

Vec3d operator+(const Vec3d& v1, const Vec3d& v2) {
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

Vec3d operator-(const Vec3d& v1, const Vec3d& v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

Vec3d operator*(const Vec3d& v1, double v2) {
	return {v1.x * v2, v1.y * v2, v1.z * v2};
}

Vec3d operator*(double v1, const Vec3d& v2) {
	return {v2.x * v1, v2.y * v1, v2.z * v1};
}

Vec3d operator/(const Vec3d& v1, double v2) {
	return {v1.x / v2, v1.y / v2, v1.z / v2};
}

double operator*(const Vec3d& v1, const Vec3d& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vec3d operator^(const Vec3d& v1, const Vec3d& v2) {
	return {v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x};
}

Vec4d::Vec4d(double x) : x(x), y(x), z(x), w(x) {}

Vec4d::Vec4d(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}

Vec4d::Vec4d(const Vec2d& v, double z, double w) : x(v.x), y(v.y), z(z), w(w) {}

Vec4d::Vec4d(double x, const Vec2d& v, double w) : x(x), y(v.x), z(v.y), w(w) {}

Vec4d::Vec4d(double x, double y, const Vec2d& v) : x(x), y(y), z(v.x), w(v.y) {}

Vec4d::Vec4d(const Vec2d& v1, const Vec2d& v2) : x(v1.x), y(v1.y), z(v2.x), w(v2.y) {}

Vec4d::Vec4d(const Vec3d& v, double w) : x(v.x), y(v.y), z(v.z), w(w) {}

Vec4d::Vec4d(double x, const Vec3d& v) : x(x), y(v.x), z(v.y), w(v.z) {}

Vec4d::Vec4d(const Vec4d& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

Vec4d Vec4d::operator-() const {
	return {-x, -y, -z, -w};
}

void Vec4d::operator+=(const Vec4d& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
}

void Vec4d::operator-=(const Vec4d& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
}

void Vec4d::operator*=(double v) {
	x *= v;
	y *= v;
	z *= v;
	w *= v;
}

void Vec4d::operator/=(double v) {
	x /= v;
	y /= v;
	z /= v;
	w /= v;
}

double Vec4d::magnitude() const {
	return sqrt(x * x + y * y + z * z + w * w);
}

double Vec4d::distance(const Vec4d& v) const {
	return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z) + (w - v.w) * (w - v.w));
}

Vec4d Vec4d::normalize() const {
	double l = sqrt(x * x + y * y + z * z + w * w);
	return {x / l, y / l, z / l, w / l};
}

Vec4d operator+(const Vec4d& v1, const Vec4d& v2) {
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
}

Vec4d operator-(const Vec4d& v1, const Vec4d& v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

Vec4d operator*(const Vec4d& v1, double v2) {
	return {v1.x * v2, v1.y * v2, v1.z * v2, v1.w * v2};
}

Vec4d operator*(double v1, const Vec4d& v2) {
	return {v2.x * v1, v2.y * v1, v2.z * v1, v2.w * v1};
}

Vec4d operator/(const Vec4d& v1, double v2) {
	return {v1.x / v2, v1.y / v2, v1.z / v2, v1.w / v2};
}

double operator*(const Vec4d& v1, const Vec4d& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

double mat2x2_determinant(const Matd<2, 2>& m) {
	return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

double mat3x3_determinant(const Matd<3, 3>& m) {
	/* sub-factor */
	double sub0 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
	double sub1 = m[1][2] * m[2][0] - m[1][0] * m[2][2];
	double sub2 = m[1][0] * m[2][1] - m[2][0] * m[1][1];
	return m[0][0] * sub0 + m[0][1] * sub1 + m[0][2] * sub2;
}

double mat4x4_determinant(const Matd<4, 4>& m) {
	/* sub-factor */
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

Matd<2, 2> mat2x2_inverse(const Matd<2, 2>& m) {
	/* inverse determinant */
	double inv_det = 1 / (m[0][0] * m[1][1] - m[0][1] * m[1][0]);
	return {
		 inv_det * m[1][1],
		-inv_det * m[0][1],
		-inv_det * m[1][0],
		 inv_det * m[0][0],
	};
}

Matd<3, 3> mat3x3_inverse(const Matd<3, 3>& m) {
	/* inverse */
	double inv0 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
	double inv1 = m[1][2] * m[2][0] - m[1][0] * m[2][2];
	double inv2 = m[1][0] * m[2][1] - m[2][0] * m[1][1];
	/* inverse determinant */
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

Matd<4, 4> mat4x4_inverse(const Matd<4, 4>& m) {
	/* sub-factor */
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
	/* inverse */
	double inv0 = m[1][1] * sub00 - m[1][2] * sub01 + m[1][3] * sub02;
	double inv1 = m[1][0] * sub00 - m[1][2] * sub03 + m[1][3] * sub04;
	double inv2 = m[1][0] * sub01 - m[1][1] * sub03 + m[1][3] * sub05;
	double inv3 = m[1][0] * sub02 - m[1][1] * sub04 + m[1][2] * sub05;
	/* inverse determinant */
	double inv_det = 1 / (m[0][0] * inv0 - m[0][1] * inv1 + m[0][2] * inv2 - m[0][3] * inv3);
	return {
		 inv_det * inv0,
		-inv_det * (m[0][1] * sub00 - m[0][2] * sub01 + m[0][3] * sub02),
		 inv_det * (m[0][1] * sub06 - m[0][2] * sub07 + m[0][3] * sub08),
		-inv_det * (m[0][1] * sub09 - m[0][2] * sub10 + m[0][3] * sub11),
		-inv_det * inv1,
		 inv_det * (m[0][0] * sub00 - m[0][2] * sub03 + m[0][3] * sub04),
		-inv_det * (m[0][0] * sub06 - m[0][2] * sub12 + m[0][3] * sub13),
		 inv_det * (m[0][0] * sub09 - m[0][2] * sub14 + m[0][3] * sub15),
		 inv_det * inv2,
		-inv_det * (m[0][0] * sub01 - m[0][1] * sub03 + m[0][3] * sub05),
		 inv_det * (m[0][0] * sub07 - m[0][1] * sub12 + m[0][3] * sub16),
		-inv_det * (m[0][0] * sub10 - m[0][1] * sub14 + m[0][3] * sub17),
		-inv_det * inv3,
		 inv_det * (m[0][0] * sub02 - m[0][1] * sub04 + m[0][2] * sub05),
		-inv_det * (m[0][0] * sub08 - m[0][1] * sub13 + m[0][2] * sub16),
		 inv_det * (m[0][0] * sub11 - m[0][1] * sub15 + m[0][2] * sub17),
	};
}

}
