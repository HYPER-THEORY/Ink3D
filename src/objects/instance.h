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

#include <vector>
#include <initializer_list>

#include "mesh.h"
#include "image.h"

class instance {
public:
	vec3 position;						/* the position of instance */
	vec3 rotation;						/* the rotation of instance */
	vec3 scale;							/* the scale of instance */
	const mesh* mesh_ptr = nullptr;		/* linked mesh */
	
	/**
	 * Create a new instance, which is the minimum unit of rendering.
	 *
	 * \param m linked mesh
	 * \param p the position of instance
	 * \param r the rotation of instance
	 * \param s the scale of instance
	 */
	instance(const mesh* m = nullptr, const vec3& p = {}, const vec3& r = {}, const vec3& s = {1, 1, 1});
};
