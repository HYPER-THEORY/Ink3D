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
#include "../core/math.h"
#include "instance.h"

class camera {
public:
	float fovy = 0;				/* field of view angle in y-axis */
	float aspect = 0;			/* aspect ratio of field of view in x-axis */
	float left = 0;				/* coordinates of the left vertical clipping plane */
	float right = 0;			/* coordinates of the right vertical clipping plane */
	float lower = 0;			/* coordinates of the lower horizontal clipping plane */
	float upper = 0;			/* coordinates of the upper horizontal clipping plane */
	float znear = 0;			/* distance to the nearer depth clipping plane */
	float zfar = 0;				/* distance to the farther depth clipping plane */
	
	vec3 position;				/* position of camera */
	vec3 direction;				/* view direction of camera */
	vec3 up;					/* view-up vector of camera */
	
	mat<4, 4> viewing;			/* viewing matrix */
	mat<4, 4> projection;		/* projection matrix */
	
	/**
	 * Create a new camera.
	 */
	camera() = default;
	
	/**
	 * Create a new perspective camera.
	 *
	 * \param f field of view angle in y-axis
	 * \param a aspect ratio of field of view in x-axis
	 * \param zn distance to the nearer depth clipping plane
	 * \param zf distance to the nearer depth clipping plane
	 */
	camera(float f, float a, float zn, float zf);
	
	/**
	 * Create a new orthographic camera.
	 *
	 * \param l coordinates of the left vertical clipping plane
	 * \param r coordinates of the right vertical clipping plane
	 * \param low coordinates of the lower horizontal clipping plane
	 * \param up coordinates of the upper horizontal clipping plane
	 * \param zf distance to the nearer depth clipping plane
	 * \param zn distance to the farther depth clipping plane
	 */
	camera(float l, float r, float low, float up, float zf, float zn);
	
	/**
	 * Set perspective projection matrix of camera.
	 *
	 * \param f field of view angle in y-axis
	 * \param a aspect ratio of field of view in x-axis
	 * \param zn distance to the nearer depth clipping plane
	 * \param zf distance to the nearer depth clipping plane
	 */
	void persp(float f, float a, float zn, float zf);
	
	/**
	 * Set orthographic projection matrix of camera.
	 *
	 * \param l coordinates of the left vertical clipping plane
	 * \param r coordinates of the right vertical clipping plane
	 * \param low coordinates of the lower horizontal clipping plane
	 * \param up coordinates of the upper horizontal clipping plane
	 * \param zn distance to the farther depth clipping plane
	 * \param zf distance to the nearer depth clipping plane
	 */
	void ortho(float l, float r, float low, float up, float zn, float zf);
	
	/**
	 * Set camera pose.
	 *
	 *	\param p position
	 *	\param d direction
	 *	\param u up
	 */
	void lookat(const vec3& p, const vec3& d, const vec3& u);
	
	/**
	 * Get model transform matrix by its position, rotation and scale.
	 *
	 * \param p position
	 * \param r rotation
	 * \param s scale
	 * \param matt translation matrix
	 * \param matr rotation matrix
	 * \param mats scaling matrix
	 */
	static void model_transform(const vec3& p, const vec3& r, const vec3& s,
								mat<4, 4>& matt, mat<4, 4>& matr, mat<4, 4>& mats);
	
	/**
	 * Get model transform matrix by its position, rotation and scale.
	 *
	 * \param p position
	 * \param r rotation
	 * \param s scale
	 */
	static mat<4, 4> model_transform(const vec3& p, const vec3& r, const vec3& s);
	
	/**
	 * Get model transform matrix by instance.
	 *
	 * \param i instance
	 * \param matt translation matrix
	 * \param matr rotation matrix
	 * \param mats scaling matrix
	 */
	static void model_transform(const instance& i, mat<4, 4>& matt, mat<4, 4>& matr, mat<4, 4>& mats);
	
	/**
	 * Get model transform matrix by instance.
	 *
	 * \param i instance
	 */
	static mat<4, 4> model_transform(const instance& i);
};
