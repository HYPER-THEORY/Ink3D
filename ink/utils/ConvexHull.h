/**
 * Copyright (C) 2021-2023 Hypertheory
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

#include "../math/Vector.h"

namespace Ink {

class ConvexHull {
public:
	/**
	 * Creates a new ConvexHull object.
	 */
	explicit ConvexHull() = default;
	
	/**
	 * Adds the specified vertex to convex hull.
	 *
	 * \param v vertex
	 */
	void add_vertex(const Vec3& v);
	
	/**
	 * Adds the specified vertex list to convex hull.
	 *
	 * \param v vertex list
	 */
	void add_vertices(const std::initializer_list<Vec3>& v);
	
	/**
	 * Returns the number of vertices in convex hull.
	 */
	size_t get_vertex_count() const;
	
	/**
	 * Returns the vertex at the specified index in convex hull.
	 *
	 * \param i vertex index
	 */
	Vec3 get_vertex(int i) const;
	
	/**
	 * Returns the number of faces in convex hull.
	 */
	size_t get_face_count() const;
	
	/**
	 * Returns the face at the specified index in convex hull.
	 *
	 * \param i face index
	 */
	std::array<int, 3> get_face(int i) const;
	
	/**
	 * Starts the execution of convex hull algorithm.
	 */
	void compute();
	
private:
	std::vector<Vec3> vertices;
	std::vector<Vec3> normals;
	std::vector<std::array<int, 3> > faces;
	
	void insert_face(int a, int b, int c);
};

}
