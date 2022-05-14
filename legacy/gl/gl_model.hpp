/*
 * Copyright (C) 2021-2022 Hypertheory
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 *	---- example.mtl -----------------------------------------------------------
 *
 *	newmtl mtl_a						|	[material]
 *
 *	Kd 1.00 0.00 0.00					|	[diffuse]
 *
 *	newmtl mtl_b						|	[material]
 *
 *	Kd 0.00 0.00 1.00					|	[diffuse]
 *
 *	---- example.obj -----------------------------------------------------------
 *
 *	mtllib example.mtl					|	[material library]
 *
 *	v -1.00 -1.00 0.00					|	[geometric vertices] Specifies a
 *	v 1.00 -1.00 0.00					|	geometric vertex and its x y z
 *	v -1.00 1.00 0.00					|	coordinates.
 *	v 1.00 1.00 0.00					|
 *
 *	vn 0.00 0.00 1.00					|	[vertex normals] Specifies a normal
 *	vn 0.00 0.00 -1.00					|	vector with components i, j, and k.
 *
 *	vt -1.00 -1.00						|	[texture vertices] Specifies a
 *	vt 1.00 -1.00						|	texture vertex and its coordinates.
 *	vt -1.00 1.00						|	1D texture ans 3D texture are not
 *	vt 1.00 1.00						|	supported.
 *
 *	g square_a							|	[group name]
 *
 *	usemtl mtl_a						|	[material name]
 *
 *	f 1/2/3 1/2/3 1/1/1					|	[face] Specifies a geometric vertex,
 *	f 3/2/4 3/2/4 1/1/1					|	texture vertex, and vertex normal.
 *
 *	g square_b							|	[group name]
 *
 *	usemtl mtl_b						|	[material name]
 *
 *	f 1//2 1//2 2//2					|	[face] Only specifies a geometric
 *	f 2//4 2//4 2//2					|	vertex and vertex normals.
 */

#ifndef GL_MODEL_HPP
#define GL_MODEL_HPP

#include "gl_stdinc.hpp"
#include "gl_type.hpp"

namespace gl {

/**
 * \param f mtl file
 * \param m output mesh
 * \param map indexed material map
 */
inline int read_mtl(const std::string& f, gl::mesh& m,
					std::unordered_map<std::string, int>& map);

/**
 * \param f obj file
 * \param m output mesh
 */
inline int read_obj(const std::string& f, gl::mesh& m);

}

#include "gl_model.inl"

#endif
