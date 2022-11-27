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

#include "Camera.h"

namespace WR {

Camera::Camera(float f, float a, float zn, float zf) {
	persp(f, a, zn, zf);
}

Camera::Camera(float l, float r, float low, float up, float zf, float zn) {
	ortho(l, r, low, up, zf, zn);
}

void Camera::persp(float f, float a, float zn, float zf) {
	fovy = f;
	aspect = a;
	znear = zn;
	zfar = zf;
	projection = {
		1 / tanf(f / 2) / a,   0,                     0,                     0,
		0,                     1 / tanf(f / 2),       0,                     0,
		0,                     0,                     (zn + zf) / (zn - zf), 2 * zf * zn / (zn - zf),
		0,                     0,                     -1,                    0,
	};
}

void Camera::ortho(float l, float r, float low, float up, float zf, float zn) {
	left = l;
	right = r;
	lower = low;
	upper = up;
	zfar = zf;
	znear = zn;
	projection = {
		2 / (r - l),           0,                     0,                     (r + l) / (l - r),
		0,                     2 / (up - low),        0,                     (up + low) / (low - up),
		0,                     0,                     2 / (zn - zf),         (zf + zn) / (zn - zf),
		0,                     0,                     0,                     1,
	};
}

void Camera::lookat(const Vec3& p, const Vec3& d, const Vec3& u) {
	position = p;
	direction = d;
	up = u;
	Vec3 r = (-d) ^u;
	viewing = {
		r.x,        r.y,        r.z,        -(p * r),
		u.x,        u.y,        u.z,        -(p * u),
		d.x,        d.y,        d.z,        -(p * d),
		0,          0,          0,          1,
	};
}
	
void Camera::model_transform(const Vec3& p, const Vec3& r, const Vec3& s,
                             Mat<4, 4>& matt, Mat<4, 4>& matr, Mat<4, 4>& mats) {
	matt = {									/* translation matrix */
		1,          0,          0,          p.x,
		0,          1,          0,          p.y,
		0,          0,          1,          p.z,
		0,          0,          0,          1,
	};
	matr =										/* rotation matrices */
	Mat<4, 4>{
		1,          0,          0,          0,	/* rotation x matrix */
		0,          cosf(r.x),  -sinf(r.x), 0,
		0,          sinf(r.x),  cosf(r.x),  0,
		0,          0,          0,          1,
	} *
    Mat<4, 4>{
		cosf(r.y),  0,          -sinf(r.y), 0,	/* rotation y matrix */
		0,          1,          0,          0,
		sinf(r.y),  0,          cosf(r.y),  0,
		0,          0,          0,          1,
	} *
    Mat<4, 4>{
		cosf(r.z),  -sinf(r.z), 0,          0,	/* rotation z matrix */
		sinf(r.z),  cosf(r.z),  0,          0,
		0,          0,          1,          0,
		0,          0,          0,          1,
	};
	mats = {									/* scaling matrix */
		s.x,        0,          0,          0,
		0,          s.y,        0,          0,
		0,          0,          s.z,        0,
		0,          0,          0,          1,
	};
}

Mat<4, 4> Camera::model_transform(const Vec3& p, const Vec3& r, const Vec3& s) {
	Mat<4, 4> matrix[3];
	model_transform(p, r, s, matrix[0], matrix[1], matrix[2]);
	return matrix[0] * matrix[1] * matrix[2];
}

void Camera::model_transform(const Instance& i, Mat<4, 4>& matt, Mat<4, 4>& matr, Mat<4, 4>& mats) {
	model_transform(i.position, i.rotation, i.scale, matt, matr, mats);
}

Mat<4, 4> Camera::model_transform(const Instance& i) {
	return model_transform(i.position, i.rotation, i.scale);
}

}
