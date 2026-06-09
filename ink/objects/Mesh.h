/**
 * Copyright (C) 2021-2023 HYPERTHEORY
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

#include "../math/Euler.h"
#include "../math/Vector.h"

#include <string>
#include <vector>

namespace ink {

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
	std::vector<Vec3> color;          /**< the color for each vertex */
	
	/**
	 * Creates a new Mesh object and initializes it with name.
	 *
	 * \param n mesh name
	 */
	Mesh(const std::string& n = "");
	
	/**
	 * Translates the mesh. This operation will modify the mesh data.
	 *
	 * \param x X coordinate
	 * \param y Y coordinate
	 * \param z Z coordinate
	 */
	void translate(float x, float y, float z);
	
	/**
	 * Translates the mesh. This operation will modify the mesh data.
	 *
	 * \param t translate vector
	 */
	void translate(const Vec3& t);
	
	/**
	 * Rotates the mesh about the X axis. This operation will modify the mesh
	 * data.
	 *
	 * \param a angle
	 */
	void rotate_x(float a);
	
	/**
	 * Rotates the mesh about the Y axis. This operation will modify the mesh
	 * data.
	 *
	 * \param a angle
	 */
	void rotate_y(float a);
	
	/**
	 * Rotates the mesh about the Z axis. This operation will modify the mesh
	 * data.
	 *
	 * \param a angle
	 */
	void rotate_z(float a);
	
	/**
	 * Rotates the mesh with Euler angles. This operation will modify the mesh
	 * data.
	 *
	 * \param e Euler angles
	 */
	void rotate(const Euler& e);
	
	/**
	 * Scales the mesh. This operation will modify the mesh data.
	 *
	 * \param x X coordinate
	 * \param y Y coordinate
	 * \param z Z coordinate
	 */
	void scale(float x, float y, float z);
	
	/**
	 * Scales the mesh. This operation will modify the mesh data.
	 *
	 * \param s scale vector
	 */
	void scale(const Vec3& s);
	
	/**
	 * Normalizes all the normal vectors of the mesh.
	 */
	void normalize();
	
	/**
	 * Calculates normals from the vertices, adds these normals to the mesh.
	 */
	void create_normals();
	
	/**
	 * Calculates tangents from the vertices, normals, and UVs information, adds
	 * these tangents to the mesh.
	 */
	void create_tangents();
};

}
