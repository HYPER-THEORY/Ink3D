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

#include "camera.h"

void camera::lookat(const vec3& p, const vec3& d, const vec3& u) {
	position = p;
	direction = d;
	up = u;
	vec3 r = (-d) ^ u;
	viewing = {
		r.x,        r.y,        r.z,        -(p * r),
		u.x,        u.y,        u.z,        -(p * u),
		d.x,        d.y,        d.z,        -(p * d),
		0,          0,          0,          1,
	};
}
	
void camera::model_transform(const vec3& p, const vec3& r, const vec3& s,
							 mat4& matt, mat4& matr, mat4& mats) {
	matt = {									/* translation matrix */
		1,          0,          0,          p.x,
		0,          1,          0,          p.y,
		0,          0,          1,          p.z,
		0,          0,          0,          1,
	};
	matr =										/* rotation matrices */
	mat4{
		1,          0,          0,          0,		/* rotation x */
		0,          cosf(r.x),  -sinf(r.x), 0,
		0,          sinf(r.x),  cosf(r.x),  0,
		0,          0,          0,          1,
	} *
	mat4{
		cosf(r.y),  0,          -sinf(r.y), 0,		/* rotation y */
		0,          1,          0,          0,
		sinf(r.y),  0,          cosf(r.y),  0,
		0,          0,          0,          1,
	} *
	mat4{
		cosf(r.z),  -sinf(r.z), 0,          0,		/* rotation z */
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

mat4 camera::model_transform(const vec3& p, const vec3& r, const vec3& s) {
	mat<4, 4> matrix[3];
	model_transform(p, r, s, matrix[0], matrix[1], matrix[2]);
	return matrix[0] * matrix[1] * matrix[2];
}

void camera::model_transform(const instance& i, mat4& matt, mat4& matr, mat4& mats) {
	model_transform(i.position, i.rotation, i.scale, matt, matr, mats);
}

mat4 camera::model_transform(const instance& i) {
	return model_transform(i.position, i.rotation, i.scale);
}
