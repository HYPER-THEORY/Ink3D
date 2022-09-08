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

#include "../resource/mesh.h"
#include "instance.h"
#include "camera.h"

#ifndef LIKELY
#define	LIKELY(x)	__builtin_expect((x), 1)
#endif

#ifndef UNLIKELY
#define	UNLIKELY(x)	__builtin_expect((x), 0)
#endif

namespace soft {

constexpr float eps = 1e-6;

struct ginfo {
	vec3 normal;
	vec4 specular;
	vec4 irradiance;
	vec3 world_coord;
};

class shader {
public:
	mat<4, 4> model;
	mat<4, 4> view;
	mat<4, 4> proj;
	mat<4, 4> model_view_proj;
	const image* const* textures;
	
	/**
	 * Vertex shader.
	 *
	 * \param m mesh
	 * \param i mesh index
	 * \param id vertex id of each face
	 * \param v vertex position
	 */
	virtual void vextex_shader(const mesh& m, int i, int id, vec4& v) = 0;
	
	/**
	 * Geometry shader.
	 *
	 * \param vs vertex positions
	 */
	virtual void geometry_shader(vec4* vs) = 0;
	
	/**
	 * Fragment shader.
	 *
	 * \param b barycentric coordinate
	 * \param p screen position
	 * \param g G-buffer
	 * \param c out color
	 */
	virtual void fragment_shader(const vec3& b, const vec2& p, ginfo& g, vec4& c) = 0;
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

static int viewport_x = 0;
static int viewport_y = 0;
static int viewport_width = 0;
static int viewport_height = 0;

/**
 * Set viewport of software rasterization.
 *
 * \param x lower-left corner of the viewport rectangle
 * \param y lower-left corner of the viewport rectangle
 * \param w width of the viewport
 * \param h height of the viewport
 */
void set_viewport(int x, int y, int w, int h);

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
 * Rasterize pointlist.
 *
 * \param p pointlist
 * \param dvs device vertices
 * \param s shader
 * \param canvas drawing canvas
 * \param zb Z-buffer
 * \param gb G-buffer
 */
void rasterize(const pointlist& p, const vec3* dvs, shader& s, vec3* canvas, double* zb, ginfo* gb);

/**
 * Rasterize pointlist without shading.
 *
 * \param p pointlist
 * \param dvs device vertices
 * \param zb Z-buffer
 */
template <bool write = true>
bool rasterize(const pointlist& p, const vec3* dvs, double* zb);

/**
 * Draw objects.
 *
 * \param c camera infomation
 * \param s shader
 * \param m mesh
 * \param canvas drawing canvas
 * \param zb Z-buffer
 * \param gb G-buffer
 */
void draw(const camera& c, shader& s, const mesh& m, vec3* canvas, double* zb, ginfo* gb);

/**
 * Write depth infomation of object into Z-buffer.
 *
 * \param c camera infomation
 * \param s shader
 * \param m mesh
 * \param zb Z-buffer
 */
void ztest(const camera& c, shader& s, const mesh& m, double* zb);

/**
 * Query whether the object can pass depth test.
 *
 * \param c camera infomation
 * \param s shader
 * \param m mesh
 * \param zb Z-buffer
 */
bool zquery(const camera& c, shader& s, const mesh& m, double* zb);

/**
 * Draw instances.
 *
 * \param c camera information
 * \param s shader
 * \param is instances
 * \param size size of instances
 * \param canvas drawing canvas
 */
void draw_instances(const camera& c, shader& s, const instance* const* is, size_t size, vec3* canvas);

static std::vector<double> zbuffer;
static std::vector<ginfo> gbuffer;

class light {
public:
	vec3 color = {1, 1, 1};
	
	/**
	 * Intersect with traingle.
	 *
	 * \param a triangle vertex A
	 * \param b triangle vertex B
	 * \param c triangle vertex C
	 * \param o origin point
	 * \param d light direction
	 * \param l distance limit
	 */
	static bool intersect(const vec3& a, const vec3& b, const vec3& c, const vec3& o, const vec3& d, float l);
	
	/**
	 * Intersect with traingle.
	 *
	 * \param a triangle vertex A
	 * \param b triangle vertex B
	 * \param c triangle vertex C
	 * \param o origin point
	 * \param d light direction
	 * \param l distance limit
	 * \param i intersect point
	 */
	static bool intersect(const vec3& a, const vec3& b, const vec3& c, const vec3& o, const vec3& d, float l, vec3& i);
	
	/**
	 * Calculate light factor of the specified position.
	 *
	 * \param p position
	 * \param n normal
	 */
	virtual vec3 lighting(const vec3& p, const vec3& n) const = 0;
	
	/**
	 * Cast a ray to the world.
	 *
	 * \param p position
	 * \param n normal
	 * \param w world (triangles in world coordinates)
	 * \param ws world size (quantity of triangles)
	 */
	virtual vec3 cast(const vec3& p, const vec3& n, const vec3* w, size_t ws) const = 0;
};

class pointlight : public light {
public:
	vec3 position;
	float intensity = 1;
	float decay = .1;
	
	/**
	 * Create a new pointlight.
	 *
	 * \param p light position
	 * \param i light intensity
	 * \param d spread decay
	 */
	pointlight(const vec3& p, float i = 1, float d = .1);
	
	/**
	 * Calculate light factor of the specified position.
	 *
	 * \param p position
	 * \param n normal
	 */
	vec3 lighting(const vec3& p, const vec3& n) const override;
	
	/**
	 * Cast a ray to the world.
	 *
	 * \param p position
	 * \param n normal
	 * \param w world (triangles in world coordinates)
	 * \param ws world size (quantity of triangles)
	 */
	vec3 cast(const vec3& p, const vec3& n, const vec3* w, size_t ws) const override;
};

class directlight : public light {
public:
	vec3 direction;
	float intensity = 1;
	
	/**
	 * Create a new dorectionallight.
	 *
	 * \param d light direction
	 * \param i light intensity
	 */
	directlight(const vec3& d, float i = 1);
	
	/**
	 * Calculate light factor of the specified position.
	 *
	 * \param p position
	 * \param n normal
	 */
	vec3 lighting(const vec3& p, const vec3& n) const override;
	
	/**
	 * Cast a ray to the world.
	 *
	 * \param p position
	 * \param n normal
	 * \param w world (triangles in world coordinates)
	 * \param ws world size (quantity of triangles)
	 */
	vec3 cast(const vec3& p, const vec3& n, const vec3* w, size_t ws) const override;
	
private:
	static float limit;
};

class arealight : public light {
public:
	vec3 position;
	vec3 toright;
	vec3 tolower;
	vec3 normal;
	float intensity = 1;
	float area = 0;
	
	/**
	 * Create a new arealight.
	 *
	 * \param p light position
	 * \param tr vector from left to right
	 * \param tl vector from upper to lower
	 * \param i light intensity
	 */
	arealight(const vec3& p, const vec3& tr, const vec3& tl, float i = 1);
	
	/**
	 * Emit a ray to the world.
	 *
	 * \param o origin point
	 * \param d light direction
	 * \param w world (triangles in world coordinates)
	 * \param ws world size (quantity of triangles)
	 * \param p intersect position
	 * \param n intersect normal
	 */
	static bool emit(const vec3& o, const vec3& d, const vec3* w, size_t ws, vec3& p, vec3& n);
	
	/**
	 * Cast a ray to the world.
	 *
	 * \param p position
	 * \param n normal
	 * \param w world (triangles in world coordinates)
	 * \param ws world size (quantity of triangles)
	 */
	vec3 cast(const vec3& p, const vec3& n, const vec3* w, size_t ws) const override;
	
private:
	static float limit;
};

/**
 * Compute color with radiance.
 *
 * \param r radiance
 * \param c color
 */
void lighting(const vec3& r, vec3& c);

/**
 * Classic raytracing.
 *
 * \param ls lights
 * \param size size of lights
 * \param w world (triangles in world coordinates)
 * \param ws world size (quantity of triangles)
 * \param canvas drawing canvas
 */
void classic_raytracing(const light* const* ls, size_t size, const vec3* w, size_t ws, vec3* canvas);

/**
 * Path raytracing.
 *
 * \param ls lights
 * \param size size of lights
 * \param w world (triangles in world coordinates)
 * \param ws world size (quantity of triangles)
 * \param canvas drawing canvas
 */
void path_tracing(const light* const* ls, size_t size, const vec3* w, size_t ws, vec3* canvas);

}
