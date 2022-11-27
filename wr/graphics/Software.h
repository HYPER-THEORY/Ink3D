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

#include "../resource/Mesh.h"
#include "Instance.h"
#include "Camera.h"

#ifndef LIKELY
#define	LIKELY(x)	__builtin_expect((x), 1)
#endif

#ifndef UNLIKELY
#define	UNLIKELY(x)	__builtin_expect((x), 0)
#endif

namespace WR {

namespace Soft {

constexpr float eps = 1e-6;

struct GInfo {
	Vec3 normal;
	Vec4 specular;
	Vec4 irradiance;
	Vec3 world_coord;
};

class Shader {
public:
	Mat<4, 4> model;
	Mat<4, 4> view;
	Mat<4, 4> proj;
	Mat<4, 4> model_view_proj;
	const Image* const* textures;
	
	/**
	 * Vertex shader.
	 *
	 * \param m mesh
	 * \param i mesh index
	 * \param id vertex id of each face
	 * \param v vertex position
	 */
	virtual void vextex_shader(const Mesh& m, int i, int id, Vec4& v) = 0;
	
	/**
	 * Geometry shader.
	 *
	 * \param vs vertex positions
	 */
	virtual void geometry_shader(Vec4* vs) = 0;
	
	/**
	 * Fragment shader.
	 *
	 * \param b barycentric coordinate
	 * \param p screen position
	 * \param g G-buffer
	 * \param c out color
	 */
	virtual void fragment_shader(const Vec3& b, const Vec2& p, GInfo& g, Vec4& c) = 0;
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

struct PointList {
	size_t size = 0;
	Vec4* vertices = nullptr;
	Vec3* barycenters = nullptr;
};

/**
 * Add point into pointlist.
 *
 * \param v vertex
 * \param b baycenter coordinate
 * \param l pointlist
 */
void add_pointlist(const Vec4& v, const Vec3& b, PointList& l);

/**
 * Clip pointlist at near clip plane.
 *
 * \param i in pointlist
 * \param zn znear
 * \param o out pointlist
 */
void znear_clip(const PointList& i, float zn, PointList& o);

/**
 * Clip pointlist at far clip plane.
 *
 * \param i in pointlist
 * \param zf zfar
 * \param o out pointlist
 */
void zfar_clip(const PointList& i, float zf, PointList& o);

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
void rasterize(const PointList& p, const Vec3* dvs, Shader& s, Vec3* canvas, double* zb, GInfo* gb);

/**
 * Rasterize pointlist without shading.
 *
 * \param p pointlist
 * \param dvs device vertices
 * \param zb Z-buffer
 */
template <bool write = true>
bool rasterize(const PointList& p, const Vec3* dvs, double* zb);

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
void draw(const Camera& c, Shader& s, const Mesh& m, Vec3* canvas, double* zb, GInfo* gb);

/**
 * Write depth infomation of object into Z-buffer.
 *
 * \param c camera infomation
 * \param s shader
 * \param m mesh
 * \param zb Z-buffer
 */
void ztest(const Camera& c, Shader& s, const Mesh& m, double* zb);

/**
 * Query whether the object can pass depth test.
 *
 * \param c camera infomation
 * \param s shader
 * \param m mesh
 * \param zb Z-buffer
 */
bool zquery(const Camera& c, Shader& s, const Mesh& m, double* zb);

/**
 * Draw instances.
 *
 * \param c camera information
 * \param s shader
 * \param is instances
 * \param size size of instances
 * \param canvas drawing canvas
 */
void draw_instances(const Camera& c, Shader& s, const Instance* const* is, size_t size, Vec3* canvas);

static std::vector<double> zbuffer;
static std::vector<GInfo> gbuffer;

class Light {
public:
	Vec3 color = {1, 1, 1};
	
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
	static bool intersect(const Vec3& a, const Vec3& b, const Vec3& c, const Vec3& o, const Vec3& d, float l);
	
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
	static bool intersect(const Vec3& a, const Vec3& b, const Vec3& c, const Vec3& o, const Vec3& d, float l, Vec3& i);
	
	/**
	 * Calculate light factor of the specified position.
	 *
	 * \param p position
	 * \param n normal
	 */
	virtual Vec3 lighting(const Vec3& p, const Vec3& n) const = 0;
	
	/**
	 * Cast a ray to the world.
	 *
	 * \param p position
	 * \param n normal
	 * \param w world (triangles in world coordinates)
	 * \param ws world size (quantity of triangles)
	 */
	virtual Vec3 cast(const Vec3& p, const Vec3& n, const Vec3* w, size_t ws) const = 0;
};

class PointLight : public Light {
public:
	Vec3 position;
	float intensity = 1;
	float decay = .1;
	
	/**
	 * Create a new pointlight.
	 *
	 * \param p light position
	 * \param i light intensity
	 * \param d spread decay
	 */
	PointLight(const Vec3& p, float i = 1, float d = .1);
	
	/**
	 * Calculate light factor of the specified position.
	 *
	 * \param p position
	 * \param n normal
	 */
	Vec3 lighting(const Vec3& p, const Vec3& n) const override;
	
	/**
	 * Cast a ray to the world.
	 *
	 * \param p position
	 * \param n normal
	 * \param w world (triangles in world coordinates)
	 * \param ws world size (quantity of triangles)
	 */
	Vec3 cast(const Vec3& p, const Vec3& n, const Vec3* w, size_t ws) const override;
};

class DirectLight : public Light {
public:
	Vec3 direction;
	float intensity = 1;
	
	/**
	 * Create a new dorectionallight.
	 *
	 * \param d light direction
	 * \param i light intensity
	 */
	DirectLight(const Vec3& d, float i = 1);
	
	/**
	 * Calculate light factor of the specified position.
	 *
	 * \param p position
	 * \param n normal
	 */
	Vec3 lighting(const Vec3& p, const Vec3& n) const override;
	
	/**
	 * Cast a ray to the world.
	 *
	 * \param p position
	 * \param n normal
	 * \param w world (triangles in world coordinates)
	 * \param ws world size (quantity of triangles)
	 */
	Vec3 cast(const Vec3& p, const Vec3& n, const Vec3* w, size_t ws) const override;
	
private:
	static float limit;
};

class AreaLight : public Light {
public:
	Vec3 position;
	Vec3 toright;
	Vec3 tolower;
	Vec3 normal;
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
	AreaLight(const Vec3& p, const Vec3& tr, const Vec3& tl, float i = 1);
	
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
	static bool emit(const Vec3& o, const Vec3& d, const Vec3* w, size_t ws, Vec3& p, Vec3& n);
	
	/**
	 * Cast a ray to the world.
	 *
	 * \param p position
	 * \param n normal
	 * \param w world (triangles in world coordinates)
	 * \param ws world size (quantity of triangles)
	 */
	Vec3 cast(const Vec3& p, const Vec3& n, const Vec3* w, size_t ws) const override;
	
private:
	static float limit;
};

/**
 * Compute color with radiance.
 *
 * \param r radiance
 * \param c color
 */
void lighting(const Vec3& r, Vec3& c);

/**
 * Classic raytracing.
 *
 * \param ls lights
 * \param size size of lights
 * \param w world (triangles in world coordinates)
 * \param ws world size (quantity of triangles)
 * \param canvas drawing canvas
 */
void classic_raytracing(const Light* const* ls, size_t size, const Vec3* w, size_t ws, Vec3* canvas);

/**
 * Path raytracing.
 *
 * \param ls lights
 * \param size size of lights
 * \param w world (triangles in world coordinates)
 * \param ws world size (quantity of triangles)
 * \param canvas drawing canvas
 */
void path_tracing(const Light* const* ls, size_t size, const Vec3* w, size_t ws, Vec3* canvas);

}

}
