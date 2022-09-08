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

#include "../objects/Image.h"
#include "../objects/Mesh.h"
#include "../objects/Instance.h"
#include "../camera/Camera.h"

namespace Ink::Soft {

class Shader {
public:
	Mat4 model;                      /**< model matrix */
	Mat4 view;                       /**< view matrix */
	Mat4 proj;                       /**< projection matrix */
	Mat4 model_view;                 /**< model view matrix */
	Mat4 model_view_proj;            /**< model view projection matrix */
	Vec3 camera_pos;                 /**< camera position */
	
	const Image* const* textures;    /**< textures used by camera */
	
	/**
	 * Vertex shader function.
	 *
	 * \param m mesh
	 * \param i mesh index
	 * \param id vertex id
	 * \param v vertex position
	 */
	virtual void vextex(const Mesh& m, int i, int id, Vec4& v) = 0;
	
	/**
	 * Geometry shader function.
	 *
	 * \param v vertex positions
	 */
	virtual void geometry(Vec4* v) = 0;
	
	/**
	 * Fragment shader function.
	 *
	 * \param b barycentric coordinate
	 * \param p screen position
	 * \param c out color
	 */
	virtual void fragment(const Vec3& b, const Vec2& p, Vec4& c) = 0;
};

/**
 * Nearest texture mapping.
 *
 * \param t texture
 * \param u u-coordinate
 * \param v v-coordinate
 */
Vec3 nearest_map(const Image& t, float u, float v);

/**
 * Nearest texture mapping.
 *
 * \param t texture
 * \param uv uv-coordinate
 */
Vec3 nearest_map(const Image& t, const Vec2& uv);

/**
 * Linear texture mapping.
 *
 * \param t texture
 * \param u u-coordinate
 * \param v v-coordinate
 */
Vec3 linear_map(const Image& t, float u, float v);

/**
 * Linear texture mapping.
 *
 * \param t texture
 * \param uv uv-coordinate
 */
Vec3 linear_map(const Image& t, const Vec2& uv);

static int viewport_w = 0;
static int viewport_h = 0;

/**
 * Sets the viewport.
 *
 * \param w the width of the viewport
 * \param h the height of the viewport
 */
void set_viewport(int w, int h);

struct PointList {
	size_t size = 0;
	Vec4* vertices = nullptr;
	Vec3* barycenters = nullptr;
};

/**
 * Add point into point list.
 *
 * \param l point list
 * \param v vertex
 * \param b baycenter coordinate
 */
void add_point(PointList& l, const Vec4& v, const Vec3& b);

/**
 * Clip the point list at near clip plane.
 *
 * \param i input point list
 * \param z znear plane distance
 * \param o output point list
 */
void znear_clip(const PointList& i, float z, PointList& o);

/**
 * Clip the pointlist at far clip plane.
 *
 * \param i input point list
 * \param z zfar plane distance
 * \param o output point list
 */
void zfar_clip(const PointList& i, float z, PointList& o);

/**
 * Rasterize the triangles in pointlist.
 *
 * \param p pointlist
 * \param d device vertices
 * \param s shader
 * \param zb Z-buffer
 * \param canvas canvas
 */
void rasterize(const PointList& p, const Vec3* d, Shader& s, double* zb, Vec4* canvas);

/**
 * Rasterize the triangles in pointlist without rendering its color.
 *
 * \param p pointlist
 * \param d device vertices
 * \param zb Z-Buffer
 */
void rasterize(const PointList& p, const Vec3* d, double* zb);

/**
 * Draw the mesh using a camera. When canvas is nullptr, the drawing will only
 * affects Z-Buffer.
 *
 * \param c camera
 * \param s shader
 * \param m mesh
 * \param zb Z-Buffer
 * \param canvas canvas
 */
void draw(const Camera& c, Shader& s, const Mesh& m, double* zb, Vec4* canvas = nullptr);

/**
 * Draw the instances using a camera.
 *
 * \param c camera
 * \param s shader
 * \param i instances
 * \param t the textures of instances
 * \param size the size of instances
 * \param canvas canvas
 */
void draw_instances(const Camera& c, Shader& s, const Instance* const* i,
					const Image** const* t, size_t size, Vec4* canvas);

static std::vector<double> zbuffer;

}
