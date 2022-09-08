/**
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
#include <array>
#include <unordered_set>

#include "../math/Maths.h"

namespace Ink {

class ConvexHull {
public:
	/**
	 * Create a new ConvexHull.
	 */
	ConvexHull() = default;
	
	/**
	 * Add vertex to convex hull.
	 *
	 * \param v vertex
	 */
	void add_vertex(const Vec3& v);
	
	/**
	 * Add vertices to convex hull.
	 *
	 * \param v vertex list
	 */
	void add_vertices(const std::initializer_list<Vec3>& v);
	
	/**
	 * Returns the number of vertices.
	 */
	size_t get_vertex_count() const;
	
	/**
	 * Returns the vertices of convex hull.
	 */
	const Vec3* get_vertices() const;
	
	/**
	 * Returns the number of faces.
	 */
	size_t get_face_count() const;
	
	/**
	 * Returns the faces of convex hull.
	 */
	const std::array<int, 3>* get_faces() const;
	
	/**
	 * Start the execution of convex hull algorithm.
	 */
	void compute();
	
private:
	std::vector<Vec3> vertices;
	std::vector<Vec3> normals;
	std::vector<std::array<int, 3> > faces;
	
	void insert_face(int a, int b, int c);
};

}
