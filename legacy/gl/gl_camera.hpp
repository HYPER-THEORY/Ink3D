/*
 * Copyright (C) 2021-2022 Hypertheory
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef GL_CAMERA_HPP
#define GL_CAMERA_HPP

#include "gl_math.hpp"
#include "gl_type.hpp"

namespace gl {

class camera {
public:
	/* inner parameter */
	int width = 0;
	int height = 0;
	float fovy = 0;
	float znear = 0;
	float zfar = 0;
	/* outer parameter */
	vec3 position;
	vec3 direction;
	vec3 up;
	/* matrices */
	mat<4, 4> viewing;
	mat<4, 4> projection;
	
	camera() = default;
	
	/**
	 * \param w width
	 * \param h height
	 * \param f fov on y-axis
	 * \param zn znear
	 * \param zf zfar
	 */
	camera(int w, int h, float f, float zn, float zf) : width(w), height(h), fovy(f), znear(zn), zfar(zf) {
		projection = {
			1 / (tanf(fovy / 2) * width / height), 0, 0, 0,
			0, 1 / tanf(fovy / 2), 0, 0,
			0, 0, (znear + zfar) / (znear - zfar), 2 * zfar * znear / (znear - zfar),
			0, 0, -1, 0,
		};
	}
	
	/**
	 *	\param p position
	 *	\param d direction
	 *	\param u up
	 */
	void lookat(const vec3& p, const vec3& d, const vec3& u) {
		position = p;
		direction = d;
		up = u;
		vec3 r = (-d) ^ u;
		viewing = {
			r.x, r.y, r.z, -(p * r),
			u.x, u.y, u.z, -(p * u),
			d.x, d.y, d.z, -(p * d),
			0, 0, 0, 1,
		};
	}
	
	/**
	 * \param i instance
	 * \param t translation matrix
	 * \param r rotation matrix
	 * \param s scaling matrix
	 */
	static void model_transform(const instance& i, mat<4, 4>& t, mat<4, 4>& r, mat<4, 4>& s) {
		t = {
			1, 0, 0, i.position.x,
			0, 1, 0, i.position.y,
			0, 0, 1, i.position.z,
			0, 0, 0, 1,
		};
		r =
		/* rotation x */ mat<4, 4>{
			1, 0, 0, 0,
			0, cos(i.rotation.x), -sin(i.rotation.x), 0,
			0, sin(i.rotation.x), cos(i.rotation.x), 0,
			0, 0, 0, 1,
		} *
		/* rotation y */ mat<4, 4>{
			cos(i.rotation.y), 0, -sin(i.rotation.y), 0,
			0, 1, 0, 0,
			sin(i.rotation.y), 0, cos(i.rotation.y), 0,
			0, 0, 0, 1,
		} *
		/* rotation z */ mat<4, 4>{
			cos(i.rotation.z), -sin(i.rotation.z), 0, 0,
			sin(i.rotation.z), cos(i.rotation.z), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1,
		};
		s = {
			i.scale.x, 0, 0, 0,
			0, i.scale.y, 0, 0,
			0, 0, i.scale.z, 0,
			0, 0, 0, 1,
		};
	}
	
	/**
	 *	\param i instance
	 *	\param t transform matrix
	 */
	static void model_transform(const instance& i, mat<4, 4>& t) {
		mat<4, 4> translation;
		mat<4, 4> rotation;
		mat<4, 4> scaling;
		model_transform(i, translation, rotation, scaling);
		t = translation * rotation * scaling;
	}
};

}

#endif
