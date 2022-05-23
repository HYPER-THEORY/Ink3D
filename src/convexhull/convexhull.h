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
#include <vector>
#include <array>
#include <list>
#include <unordered_set>

#include "../math/maths.h"

class convexhull {
public:
	std::vector<vec3> vertexs;
	
	/**
	 * Create a new convexhull.
	 */
	convexhull() = default;
	
	/**
	 * Add vertex to convexhull.
	 *
	 * \param v vertex
	 */
	void add_vertex(const vec3& v);
	
	/**
	 * Add vertices to convexhull.
	 *
	 * \param v vertex list
	 */
	void add_vertices(const std::initializer_list<vec3>& v);
	
	/**
	 * Start the execution of convex hull algorithm.
	 */
	void compute();
	
private:
	std::list<std::array<int, 3> > faces;
	std::list<vec3> normals;
	
	void insert_face(int a, int b, int c);
};
