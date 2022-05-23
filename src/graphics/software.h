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

#include "../math/maths.h"
#include "../objects/mesh.h"
#include "../objects/instance.h"
#include "../camera/camera.h"

#ifndef LIKELY
#define	LIKELY(x)	__builtin_expect((x), 1)
#endif

#ifndef UNLIKELY
#define	UNLIKELY(x)	__builtin_expect((x), 0)
#endif

namespace soft {

constexpr float eps = 1e-6;

class shader {
public:
	mat4 model;
	mat4 view;
	mat4 proj;
	mat4 model_view_proj;
	const image* const* textures;
	
	/**
	 * Vertex shader.
	 *
	 * \param m mesh
	 * \param i mesh index
	 * \param id vertex id
	 * \param v vertex position
	 */
	virtual void vextex(const mesh& m, int i, int id, vec4& v) = 0;
	
	/**
	 * Geometry shader.
	 *
	 * \param vs vertex positions
	 */
	virtual void geometry(vec4* vs) = 0;
	
	/**
	 * Fragment shader.
	 *
	 * \param b barycentric coordinate
	 * \param p screen position
	 * \param c out color
	 */
	virtual void fragment(const vec3& b, const vec2& p, vec4& c) = 0;
};

/**
 * Nearest texture mapping.
 *
 * \param t texture
 * \param u u-coordinate
 * \param v v-coordinate
 */
vec3 nearest_map(const image& t, float u, float v);

/**
 * Nearest texture mapping.
 *
 * \param t texture
 * \param uv uv-coordinate
 */
vec3 nearest_map(const image& t, const vec2& uv);

/**
 * Linear texture mapping.
 *
 * \param t texture
 * \param u u-coordinate
 * \param v v-coordinate
 */
vec3 linear_map(const image& t, float u, float v);

/**
 * Linear texture mapping.
 *
 * \param t texture
 * \param uv uv-coordinate
 */
vec3 linear_map(const image& t, const vec2& uv);

static int viewport_w = 0;
static int viewport_h = 0;

/**
 * Sets the viewport.
 *
 * \param w the width of the viewport
 * \param h the height of the viewport
 */
void set_viewport(int w, int h);

struct pointlist {
	size_t size = 0;
	vec4* vertices = nullptr;
	vec3* barycenters = nullptr;
};

/**
 * Add point into pointlist.
 *
 * \param v vertex
 * \param b baycenter coordinate
 * \param l pointlist
 */
void add_pointlist(const vec4& v, const vec3& b, pointlist& l);

/**
 * Clip pointlist at near clip plane.
 *
 * \param i in pointlist
 * \param zn znear
 * \param o out pointlist
 */
void znear_clip(const pointlist& i, float zn, pointlist& o);

/**
 * Clip pointlist at far clip plane.
 *
 * \param i in pointlist
 * \param zf zfar
 * \param o out pointlist
 */
void zfar_clip(const pointlist& i, float zf, pointlist& o);

/**
 * Rasterize the triangle of pointlist.
 *
 * \param p pointlist
 * \param dvs device vertices
 * \param s shader
 * \param canvas drawing canvas
 * \param zb Z-buffer
 */
void rasterize(const pointlist& p, const vec3* dvs, shader& s, vec4* canvas, double* zb);

/**
 * Rasterize the triangle of pointlist and only write depth infomation.
 *
 * \param p pointlist
 * \param dvs device vertices
 * \param zb Z-buffer
 */
void rasterize(const pointlist& p, const vec3* dvs, double* zb);

/**
 * Draw objects.
 *
 * \param c camera
 * \param s shader
 * \param m mesh
 * \param canvas drawing canvas
 * \param zb Z-buffer
 */
void draw(const camera& c, shader& s, const mesh& m, vec4* canvas, double* zb);

/**
 * Draw objects and only write depth infomation.
 *
 * \param c camera
 * \param s shader
 * \param m mesh
 * \param zb Z-buffer
 */
void draw(const camera& c, shader& s, const mesh& m, double* zb);

/**
 * Draw instances.
 *
 * \param c camera
 * \param s shader
 * \param is instances
 * \param ts textures
 * \param size size of instances
 * \param canvas drawing canvas
 */
void draw_instances(const camera& c, shader& s, const instance* const* is,
					const image* const* ts, size_t size, vec4* canvas);

static std::vector<double> zbuffer;

}
