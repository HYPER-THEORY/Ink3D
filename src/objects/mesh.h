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
#include <string>
#include <unordered_map>

#include "../core/error.h"
#include "../math/maths.h"

#include "image.h"

struct meshgroup {
	std::string name;
	int position = 0;
	int length = 0;
};

class mesh {
public:
	std::string name;							/* mesh name */
	
	std::vector<meshgroup> groups;				/* mesh groups */
	std::vector<meshgroup> material_groups;		/* material groups */
	
	std::vector<vec3> vertex;					/* vertex geometry array */
	std::vector<vec3> normal;					/* normal geometry array */
	std::vector<vec3> binormal;					/* binormal geometry array */
	std::vector<vec3> tangent;					/* tangent geometry array */
	std::vector<vec2> uv;						/* uv geometry array */
	
	/**
	 * Create a new mesh.
	 */
	mesh() = default;
	
	/**
	 * Create a new mesh.
	 *
	 * \param n name
	 */
	mesh(const std::string& n);
};
