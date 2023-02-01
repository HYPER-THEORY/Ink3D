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

#include "../objects/Image.h"
#include "../objects/Mesh.h"
#include "../objects/Instance.h"
#include "../camera/Camera.h"

namespace Ink::Soft {

class PointList {
public:
	size_t size = 0;                /**< the size of point list */
	Vec4* vertices = nullptr;       /**< the vertices of point list */
	Vec3* barycenters = nullptr;    /**< the baycenter coordinates of point list */
	
	/**
	 * Adds a point into this point list.
	 *
	 * \param v vertex
	 * \param b baycenter coordinates
	 */
	void add_point(const Vec4& v, const Vec3& b);
};

class Shader {
public:
	Mat4 model;                      /**< the matrix of model transform */
	Mat4 view;                       /**< the matrix of viewing transform */
	Mat4 proj;                       /**< the matrix of projection transform */
	Mat4 model_view;                 /**< the matrix of model view transform */
	Mat4 model_view_proj;            /**< the matrix of model view projection transform */
	Vec3 camera_pos;                 /**< the position of camera */
	
	/**
	 * Creates a new Shader object.
	 */
	explicit Shader() = default;
	
	/**
	 * The function to complete the task of vertex shader.
	 *
	 * \param m mesh
	 * \param i mesh index
	 * \param id vertex id
	 * \param v vertex position
	 */
	virtual void vextex(const Mesh& m, int i, int id, Vec4& v) = 0;
	
	/**
	 * The function to complete the task of geometry shader.
	 *
	 * \param v vertex positions
	 */
	virtual void geometry(Vec4* v) = 0;
	
	/**
	 * The function to complete the task of fragment shader.
	 *
	 * \param b barycentric coordinate
	 * \param p screen position
	 * \param c out color
	 */
	virtual void fragment(const Vec3& b, const Vec2& p, Vec4& c) = 0;
};

static int viewport_w = 0;
static int viewport_h = 0;

/**
 * Returns the width and height of viewport region.
 */
std::pair<int, int> get_viewport();

/**
 * Sets the viewport region to render from (0, 0) to (width, height).
 *
 * \param w the width of the viewport
 * \param h the height of the viewport
 */
void set_viewport(int w, int h);

/**
 * Clips the point list at near clip plane.
 *
 * \param i input point list
 * \param z znear plane distance
 * \param o output point list
 */
void znear_clip(const PointList& i, float z, PointList& o);

/**
 * Clips the pointlist at far clip plane.
 *
 * \param i input point list
 * \param z zfar plane distance
 * \param o output point list
 */
void zfar_clip(const PointList& i, float z, PointList& o);

/**
 * Rasterizes the triangles in pointlist. The results will be saved in Z-Buffer
 * and canvas.
 *
 * \param p pointlist
 * \param d device vertices
 * \param s shader
 * \param zb Z-buffer
 * \param canvas canvas
 */
void rasterize(const PointList& p, const Vec3* d, Shader& s, double* zb, Vec4* canvas);

/**
 * Rasterizes the triangles in pointlist without shading. The results will be
 * saved in Z-Buffer.
 *
 * \param p pointlist
 * \param d device vertices
 * \param zb Z-Buffer
 */
void rasterize(const PointList& p, const Vec3* d, double* zb);

/**
 * Renders the mesh using a camera. Drawing will only affects Z-Buffer when
 * canvas is nullptr.
 *
 * \param m mesh
 * \param c camera
 * \param s shader
 * \param zb Z-Buffer
 * \param canvas canvas
 */
void render(const Mesh& m, const Camera& c, Shader& s, double* zb, Vec4* canvas = nullptr);

/**
 * Renders the instance using a camera. The results will be saved in canvas.
 *
 * \param i instance
 * \param c camera
 * \param s shader
 * \param zb Z-Buffer
 * \param canvas canvas
 */
void render_instance(const Instance* i, const Camera& c, Shader& s, double* zb, Vec4* canvas = nullptr);

}
