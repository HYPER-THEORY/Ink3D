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

#include "camera.h"

class orthocamera : public camera {
public:
	float left = 0;			/* the coordinates of the left vertical clipping plane */
	float right = 0;		/* the coordinates of the right vertical clipping plane */
	float lower = 0;		/* the coordinates of the lower horizontal clipping plane */
	float upper = 0;		/* the coordinates of the upper horizontal clipping plane */
	
	/**
	 * Create a new orthocamera.
	 */
	orthocamera() = default;
	
	/**
	 * Create a new orthocamera.
	 *
	 * \param l the coordinates of the left vertical clipping plane
	 * \param r the coordinates of the right vertical clipping plane
	 * \param low the coordinates of the lower horizontal clipping plane
	 * \param up the coordinates of the upper horizontal clipping plane
	 * \param zn distance to the farther depth clipping plane
	 * \param zf distance to the nearer depth clipping plane
	 */
	orthocamera(float l, float r, float low, float up, float zn, float zf);
	
	/**
	 * Sets orthographic projection matrix of camera.
	 *
	 * \param l the coordinates of the left vertical clipping plane
	 * \param r the coordinates of the right vertical clipping plane
	 * \param low the coordinates of the lower horizontal clipping plane
	 * \param up the coordinates of the upper horizontal clipping plane
	 * \param zn distance to the farther depth clipping plane
	 * \param zf distance to the nearer depth clipping plane
	 */
	void set(float l, float r, float low, float up, float zn, float zf);
};
