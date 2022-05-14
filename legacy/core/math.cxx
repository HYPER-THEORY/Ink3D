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

#include "math.h"

vec2f::vec2f(float x) : x(x), y(x) {}

vec2f::vec2f(float x, float y) : x(x), y(y) {}

vec2f::vec2f(const vec2f& v) : x(v.x), y(v.y) {}

vec2f vec2f::operator-() const {
	return {-x, -y};
}

void vec2f::operator+=(const vec2f& v) {
	x += v.x;
	y += v.y;
}

void vec2f::operator-=(const vec2f& v) {
	x -= v.x;
	y -= v.y;
}

void vec2f::operator*=(float v) {
	x *= v;
	y *= v;
}

void vec2f::operator/=(float v) {
	x /= v;
	y /= v;
}

float vec2f::magnitude() const {
	return sqrtf(x * x + y * y);
}

float vec2f::distance(const vec2f& v) const {
	return sqrtf((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y));
}

vec2f vec2f::normalize() const {
	float l = sqrtf(x * x + y * y);
	return {x / l, y / l};
}

vec2f vec2f::rotate(float a) const {
	return {x * cosf(a) - y * sinf(a), x * sinf(a) + y * cosf(a)};
}

vec2f vec2f::random() {
	float angle = randomf() * M_PI * 2;
	return {cosf(angle), sinf(angle)};
}

vec2f operator+(const vec2f& v1, const vec2f& v2) {
	return {v1.x + v2.x, v1.y + v2.y};
}

vec2f operator-(const vec2f& v1, const vec2f& v2) {
	return {v1.x - v2.x, v1.y - v2.y};
}

vec2f operator*(const vec2f& v1, float v2) {
	return {v1.x * v2, v1.y * v2};
}

vec2f operator*(float v1, const vec2f& v2) {
	return {v2.x * v1, v2.y * v1};
}

vec2f operator/(const vec2f& v1, float v2) {
	return {v1.x / v2, v1.y / v2};
}

float operator*(const vec2f& v1, const vec2f& v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

float operator^(const vec2f& v1, const vec2f& v2) {
	return v1.x * v2.y - v1.y * v2.x;
}

vec3f::vec3f(float x) : x(x), y(x), z(x) {}
	
vec3f::vec3f(float x, float y, float z) : x(x), y(y), z(z) {}

vec3f::vec3f(const vec2f& v, float z) : x(v.x), y(v.y), z(z) {}

vec3f::vec3f(float x, const vec2f& v) : x(x), y(v.x), z(v.y) {}

vec3f::vec3f(const vec3f& v) : x(v.x), y(v.y), z(v.z) {}

vec3f vec3f::operator-() const {
	return {-x, -y, -z};
}

void vec3f::operator+=(const vec3f& v) {
	x += v.x;
	y += v.y;
	z += v.z;
}

void vec3f::operator-=(const vec3f& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

void vec3f::operator*=(float v) {
	x *= v;
	y *= v;
	z *= v;
}

void vec3f::operator/=(float v) {
	x /= v;
	y /= v;
	z /= v;
}

float vec3f::magnitude() const {
	return sqrtf(x * x + y * y + z * z);
}

float vec3f::distance(const vec3f& v) const {
	return sqrtf((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
}

vec3f vec3f::normalize() const {
	float l = sqrtf(x * x + y * y + z * z);
	return {x / l, y / l, z / l};
}

vec3f vec3f::rotate(const vec3f& v, float a) const {
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

vec3f vec3f::random() {
	float angle1 = randomf() * M_PI * 2;
	float angle2 = randomf() * M_PI * 2;
	return {cosf(angle1) * cosf(angle2), sinf(angle2), sinf(angle1) * cosf(angle2)};
}

vec3f operator+(const vec3f& v1, const vec3f& v2) {
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

vec3f operator-(const vec3f& v1, const vec3f& v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

vec3f operator*(const vec3f& v1, float v2) {
	return {v1.x * v2, v1.y * v2, v1.z * v2};
}

vec3f operator*(float v1, const vec3f& v2) {
	return {v2.x * v1, v2.y * v1, v2.z * v1};
}

vec3f operator/(const vec3f& v1, float v2) {
	return {v1.x / v2, v1.y / v2, v1.z / v2};
}

float operator*(const vec3f& v1, const vec3f& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

vec3f operator^(const vec3f& v1, const vec3f& v2) {
	return {v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x};
}

vec4f::vec4f(float x) : x(x), y(x), z(x), w(x) {};
	
vec4f::vec4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	
vec4f::vec4f(const vec2f& v, float z, float w) : x(v.x), y(v.y), z(z), w(w) {}
	
vec4f::vec4f(float x, const vec2f& v, float w) : x(x), y(v.x), z(v.y), w(w) {}
	
vec4f::vec4f(float x, float y, const vec2f& v) : x(x), y(y), z(v.x), w(v.y) {}
	
vec4f::vec4f(const vec2f& v1, const vec2f& v2) : x(v1.x), y(v1.y), z(v2.x), w(v2.y) {}
	
vec4f::vec4f(const vec3f& v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}
	
vec4f::vec4f(float x, const vec3f& v) : x(x), y(v.x), z(v.y), w(v.z) {}
	
vec4f::vec4f(const vec4f& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

vec4f vec4f::operator-() const {
	return {-x, -y, -z, -w};
}

void vec4f::operator+=(const vec4f& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
}

void vec4f::operator-=(const vec4f& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
}

void vec4f::operator*=(float v) {
	x *= v;
	y *= v;
	z *= v;
	w *= v;
}

void vec4f::operator/=(float v) {
	x /= v;
	y /= v;
	z /= v;
	w /= v;
}

float vec4f::magnitude() const {
	return sqrtf(x * x + y * y + z * z + w * w);
}

float vec4f::distance(const vec4f& v) const {
	return sqrtf((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z) + (w - v.w) * (w - v.w));
}

vec4f vec4f::normalize() const {
	float l = sqrtf(x * x + y * y + z * z + w * w);
	return {x / l, y / l, z / l, w / l};
}

vec4f operator+(const vec4f& v1, const vec4f& v2) {
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
}

vec4f operator-(const vec4f& v1, const vec4f& v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

vec4f operator*(const vec4f& v1, float v2) {
	return {v1.x * v2, v1.y * v2, v1.z * v2, v1.w * v2};
}

vec4f operator*(float v1, const vec4f& v2) {
	return {v2.x * v1, v2.y * v1, v2.z * v1, v2.w * v1};
}

vec4f operator/(const vec4f& v1, float v2) {
	return {v1.x / v2, v1.y / v2, v1.z / v2, v1.w / v2};
}

float mat2_determinant(const mat2& m) {
	return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

float mat3_determinant(const mat3& m) {
	/* sub-factor */
	float sub0 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
	float sub1 = m[1][2] * m[2][0] - m[1][0] * m[2][2];
	float sub2 = m[1][0] * m[2][1] - m[2][0] * m[1][1];
	return m[0][0] * sub0 + m[0][1] * sub1 + m[0][2] * sub2;
}

float mat4_determinant(const mat4& m) {
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

matf<2, 2> mat2_inverse(const mat2& m) {
	/* inverse determinant */
	float inv_det = 1 / (m[0][0] * m[1][1] - m[0][1] * m[1][0]);
	return {
		 inv_det * m[1][1],
		-inv_det * m[0][1],
		-inv_det * m[1][0],
		 inv_det * m[0][0],
	};
}

matf<3, 3> mat3_inverse(const mat3& m) {
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

matf<4, 4> mat4_inverse(const mat4& m) {
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

vec2d::vec2d(double x) : x(x) {}

vec2d::vec2d(double x, double y) : x(x), y(y) {}

vec2d::vec2d(const vec2d& v) : x(v.x), y(v.y) {}

vec2d vec2d::operator-() const {
	return {-x, -y};
}

void vec2d::operator+=(const vec2d& v) {
	x += v.x;
	y += v.y;
}

void vec2d::operator-=(const vec2d& v) {
	x -= v.x;
	y -= v.y;
}

void vec2d::operator*=(double v) {
	x *= v;
	y *= v;
}

void vec2d::operator/=(double v) {
	x /= v;
	y /= v;
}

double vec2d::magnitude() const {
	return sqrt(x * x + y * y);
}

double vec2d::distance(const vec2d& v) const {
	return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y));
}

vec2d vec2d::normalize() const {
	double l = sqrt(x * x + y * y);
	return {x / l, y / l};
}

vec2d vec2d::rotate(double a) const {
	return {x * cos(a) - y * sin(a), x * sin(a) + y * cos(a)};
}

vec2d vec2d::randomunit() {
	double angle = randomd() * M_PI * 2;
	return {cos(angle), sin(angle)};
}

vec2d operator+(const vec2d& v1, const vec2d& v2) {
	return {v1.x + v2.x, v1.y + v2.y};
}

vec2d operator-(const vec2d& v1, const vec2d& v2) {
	return {v1.x - v2.x, v1.y - v2.y};
}

vec2d operator*(const vec2d& v1, double v2) {
	return {v1.x * v2, v1.y * v2};
}

vec2d operator*(double v1, const vec2d& v2) {
	return {v2.x * v1, v2.y * v1};
}

vec2d operator/(const vec2d& v1, double v2) {
	return {v1.x / v2, v1.y / v2};
}

double operator*(const vec2d& v1, const vec2d& v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

double operator^(const vec2d& v1, const vec2d& v2) {
	return v1.x * v2.y - v1.y * v2.x;
}

vec3d::vec3d(double x) : x(x), y(x), z(x) {};

vec3d::vec3d(double x, double y, double z) : x(x), y(y), z(z) {}

vec3d::vec3d(const vec2d& v, double z) : x(v.x), y(v.y), z(z) {}

vec3d::vec3d(double x, const vec2d& v) : x(x), y(v.x), z(v.y) {}

vec3d::vec3d(const vec3d& v) : x(v.x), y(v.y), z(v.z) {}

vec3d vec3d::operator-() const {
	return {-x, -y, -z};
}

void vec3d::operator+=(const vec3d& v) {
	x += v.x;
	y += v.y;
	z += v.z;
}

void vec3d::operator-=(const vec3d& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

void vec3d::operator*=(double v) {
	x *= v;
	y *= v;
	z *= v;
}

void vec3d::operator/=(double v) {
	x /= v;
	y /= v;
	z /= v;
}

double vec3d::magnitude() const {
	return sqrt(x * x + y * y + z * z);
}

double vec3d::distance(const vec3d& v) const {
	return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
}

vec3d vec3d::normalize() const {
	double l = sqrt(x * x + y * y + z * z);
	return {x / l, y / l, z / l};
}

vec3d vec3d::rotate(const vec3d& v, double a) const {
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

vec3d vec3d::randomunit() {
	double angle1 = randomd() * M_PI * 2;
	double angle2 = randomd() * M_PI * 2;
	return {cos(angle1) * cos(angle2), sin(angle2), sin(angle1) * cos(angle2)};
}

vec3d operator+(const vec3d& v1, const vec3d& v2) {
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

vec3d operator-(const vec3d& v1, const vec3d& v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

vec3d operator*(const vec3d& v1, double v2) {
	return {v1.x * v2, v1.y * v2, v1.z * v2};
}

vec3d operator*(double v1, const vec3d& v2) {
	return {v2.x * v1, v2.y * v1, v2.z * v1};
}

vec3d operator/(const vec3d& v1, double v2) {
	return {v1.x / v2, v1.y / v2, v1.z / v2};
}

double operator*(const vec3d& v1, const vec3d& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

vec3d operator^(const vec3d& v1, const vec3d& v2) {
	return {v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x};
}

vec4d::vec4d(double x) : x(x), y(x), z(x), w(x) {}

vec4d::vec4d(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}

vec4d::vec4d(const vec2d& v, double z, double w) : x(v.x), y(v.y), z(z), w(w) {}

vec4d::vec4d(double x, const vec2d& v, double w) : x(x), y(v.x), z(v.y), w(w) {}

vec4d::vec4d(double x, double y, const vec2d& v) : x(x), y(y), z(v.x), w(v.y) {}

vec4d::vec4d(const vec2d& v1, const vec2d& v2) : x(v1.x), y(v1.y), z(v2.x), w(v2.y) {}

vec4d::vec4d(const vec3d& v, double w) : x(v.x), y(v.y), z(v.z), w(w) {}

vec4d::vec4d(double x, const vec3d& v) : x(x), y(v.x), z(v.y), w(v.z) {}

vec4d::vec4d(const vec4d& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

vec4d vec4d::operator-() const {
	return {-x, -y, -z, -w};
}

void vec4d::operator+=(const vec4d& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
}

void vec4d::operator-=(const vec4d& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
}

void vec4d::operator*=(double v) {
	x *= v;
	y *= v;
	z *= v;
	w *= v;
}

void vec4d::operator/=(double v) {
	x /= v;
	y /= v;
	z /= v;
	w /= v;
}

double vec4d::magnitude() const {
	return sqrt(x * x + y * y + z * z + w * w);
}

double vec4d::distance(const vec4d& v) const {
	return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z) + (w - v.w) * (w - v.w));
}

vec4d vec4d::normalize() const {
	double l = sqrt(x * x + y * y + z * z + w * w);
	return {x / l, y / l, z / l, w / l};
}

vec4d operator+(const vec4d& v1, const vec4d& v2) {
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
}

vec4d operator-(const vec4d& v1, const vec4d& v2) {
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

vec4d operator*(const vec4d& v1, double v2) {
	return {v1.x * v2, v1.y * v2, v1.z * v2, v1.w * v2};
}

vec4d operator*(double v1, const vec4d& v2) {
	return {v2.x * v1, v2.y * v1, v2.z * v1, v2.w * v1};
}

vec4d operator/(const vec4d& v1, double v2) {
	return {v1.x / v2, v1.y / v2, v1.z / v2, v1.w / v2};
}

double operator*(const vec4d& v1, const vec4d& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

double mat2x2_determinant(const matd<2, 2>& m) {
	return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

double mat3x3_determinant(const matd<3, 3>& m) {
	/* sub-factor */
	double sub0 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
	double sub1 = m[1][2] * m[2][0] - m[1][0] * m[2][2];
	double sub2 = m[1][0] * m[2][1] - m[2][0] * m[1][1];
	return m[0][0] * sub0 + m[0][1] * sub1 + m[0][2] * sub2;
}

double mat4x4_determinant(const matd<4, 4>& m) {
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

matd<2, 2> mat2x2_inverse(const matd<2, 2>& m) {
	/* inverse determinant */
	double inv_det = 1 / (m[0][0] * m[1][1] - m[0][1] * m[1][0]);
	return {
		 inv_det * m[1][1],
		-inv_det * m[0][1],
		-inv_det * m[1][0],
		 inv_det * m[0][0],
	};
}

matd<3, 3> mat3x3_inverse(const matd<3, 3>& m) {
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

matd<4, 4> mat4x4_inverse(const matd<4, 4>& m) {
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
