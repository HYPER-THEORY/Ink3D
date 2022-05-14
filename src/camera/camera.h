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

#pragma once

#include <cmath>

#include "../math/maths.h"
#include "../objects/instance.h"

class camera {
public:
	float znear = 0;		/* distance to the nearer depth clipping plane */
	float zfar = 0;			/* distance to the farther depth clipping plane */
	
	vec3 position;			/* the position of camera */
	vec3 direction;			/* the viewing direction of camera */
	vec3 up;				/* the view-up vector of camera */
	
	mat4 viewing;			/* viewing matrix */
	mat4 projection;		/* projection matrix */
	
	/**
	 * Create a new camera.
	 */
	camera() = default;
	
	/**
	 * Sets the pose of camera.
	 *
	 * \param p position
	 * \param d direction
	 * \param u up
	 */
	void lookat(const vec3& p, const vec3& d, const vec3& u);
	
	/**
	 * Returns a model transform matrix with its position, rotation and scale.
	 *
	 * \param p position
	 * \param r rotation
	 * \param s scale
	 * \param matt translation matrix
	 * \param matr rotation matrix
	 * \param mats scaling matrix
	 */
	static void model_transform(const vec3& p, const vec3& r, const vec3& s,
								mat4& matt, mat4& matr, mat4& mats);
	
	/**
	 * Returns a model transform matrix with its position, rotation and scale.
	 *
	 * \param p position
	 * \param r rotation
	 * \param s scale
	 */
	static mat4 model_transform(const vec3& p, const vec3& r, const vec3& s);
	
	/**
	 * Returns a model transform matrix with the position, rotation and scale of
	 * instance.
	 *
	 * \param i instance
	 * \param matt translation matrix
	 * \param matr rotation matrix
	 * \param mats scaling matrix
	 */
	static void model_transform(const instance& i, mat4& matt, mat4& matr, mat4& mats);
	
	/**
	 * Returns a model transform matrix with the position, rotation and scale of
	 * instance.
	 *
	 * \param i instance
	 */
	static mat4 model_transform(const instance& i);
};
