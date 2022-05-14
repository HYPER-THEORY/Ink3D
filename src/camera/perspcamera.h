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

class perspcamera : public camera {
public:
	float fovy = 0;			/* the field of view angle in y-axis */
	float aspect = 0;		/* the aspect ratio of field of view in x-axis */
	
	/**
	 * Create a new perspcamera.
	 */
	perspcamera() = default;
	
	/**
	 * Create a new perspcamera.
	 *
	 * \param f the field of view angle in y-axis
	 * \param a the aspect ratio of field of view in x-axis
	 * \param zn distance to the nearer depth clipping plane
	 * \param zf distance to the nearer depth clipping plane
	 */
	perspcamera(float f, float a, float zn, float zf);
	
	/**
	 * Sets the perspective projection matrix of camera.
	 *
	 * \param f the field of view angle in y-axis
	 * \param a the aspect ratio of field of view in x-axis
	 * \param zn distance to the nearer depth clipping plane
	 * \param zf distance to the nearer depth clipping plane
	 */
	void set(float f, float a, float zn, float zf);
};
