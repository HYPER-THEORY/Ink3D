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
#include <string>

#include "../math/Maths.h"

namespace Ink {

struct MeshGroup {
	std::string name;
	int position = 0;
	int length = 0;
};

class Mesh {
public:
	std::string name;                 /**< mesh name */
	
	std::vector<MeshGroup> groups;    /**< mesh groups */
	
	std::vector<Vec3> vertex;         /**< the position for each vertex */
	std::vector<Vec3> normal;         /**< the normal for each vertex */
	std::vector<Vec2> uv;             /**< the UV for each vertex */
	std::vector<Vec4> tangent;        /**< the tangent for each vertex */
	
	/**
	 * Creates a new Mesh and initializes it with a name.
	 *
	 * \param n mesh name
	 */
	explicit Mesh(const std::string& n = "");
	
	/**
	 * Normalizes all the normal vectors in the mesh.
	 */
	void normalize();
	
	/**
	 * Calculates normals and adds them to the mesh. Normals are calculated by
	 * the vertices in the mesh.
	 */
	void create_normals();
	
	/**
	 * Calculates tangents and adds them to the mesh. Tangents are calculated by
	 * the vertices, normals and UVs in the mesh.
	 */
	void create_tangents();
};

}
