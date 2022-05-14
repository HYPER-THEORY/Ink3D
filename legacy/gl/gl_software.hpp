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

#ifndef GL_SOFTWARE_HPP
#define GL_SOFTWARE_HPP

#include "gl_stdinc.hpp"
#include "gl_type.hpp"
#include "gl_camera.hpp"

namespace gl::soft {

struct ginfo {
	vec3 normal;
	vec4 specular;
	vec4 irradiance;
	vec3 world_coord;
};

class shader {
public:
	mat<4, 4> model;
	mat<4, 4> viewing;
	mat<4, 4> projection;
	mat<4, 4> transform;
	const texture* const* textures;
	
	virtual void vextex_shader(const float** d, int id, vec4& v) = 0;
	virtual void geometry_shader(vec4* vs) = 0;
	virtual void fragment_shader(const vec3& b, const vec2& p, ginfo& g, vec4& c) = 0;
};

inline const vec3 nearest_map(const texture& t, float u, float v);

inline const vec3 nearest_map(const texture& t, const vec2& uv);

inline const vec3 linear_map(const texture& t, float u, float v);

inline const vec3 linear_map(const texture& t, const vec2& uv);

inline std::vector<double> zbuffer;
inline std::vector<ginfo> gbuffer;

struct pointlist {
	size_t size = 0;
	vec4* vertices = nullptr;
	vec3* barycenters = nullptr;
};

inline void add_pointlist(const vec4& v, const vec3& b, pointlist& l);

inline void znear_clip(const pointlist& i, float zn, pointlist& o);

inline void zfar_clip(const pointlist& i, float zf, pointlist& o);

inline void rasterize(const camera& c, const pointlist& p, const vec3* dvs,
					  shader& s, vec3* canvas, double* zb, ginfo* gb);

template <bool write = true>
inline bool rasterize(const camera& c, const pointlist& p, const vec3* dvs, double* zb);

inline void draw_objects(const camera& c, shader& s, const vertexobject& vo, vec3* canvas, double* zb, ginfo* gb);

inline void ztest(const camera& c, shader& s, const vertexobject& vo, double* zb);

inline bool zquery(const camera& c, shader& s, const vertexobject& vo, double* zb);

inline void draw_instances(const camera& c, shader& s, const instance* const* is, size_t is_size, vec3* canvas);

inline void draw_scene(const camera& c, shader& s, const scene& scene_info, vec3* canvas);

class light {
public:
	vec3 color = {1, 1, 1};
	
	static bool intersect(const vec3& a, const vec3& b, const vec3& c, const vec3& o, const vec3& d, float l) {
		vec3 ab = b - a;
		vec3 ac = c - a;
		vec3 ao = o - a;
		vec3 p = d ^ ac;
		vec3 q = ao ^ ab;
		float inverse = 1 / (ab * p);
		float u = d * q * inverse;
		float v = ao * p * inverse;
		float t = ac * q * inverse;
		return t > eps && t < l && u > -eps && v > -eps && u + v < 1 + eps;
	}
	
	static bool intersect(const vec3& a, const vec3& b, const vec3& c, const vec3& o, const vec3& d, float l, vec3& i) {
		vec3 ab = b - a;
		vec3 ac = c - a;
		vec3 ao = o - a;
		vec3 p = d ^ ac;
		vec3 q = ao ^ ab;
		float inverse = 1 / (ab * p);
		float u = d * q * inverse;
		float v = ao * p * inverse;
		float t = ac * q * inverse;
		if (t < eps || t > l || u < -eps || v < -eps || u + v > 1 + eps) return false;
		i.x = 1 - u - v;
		i.y = v;
		i.z = u;
		return true;
	}
	
	virtual vec3 lighting(const vec3& p, const vec3& n) const {
		return {};
	}
	
	virtual vec3 cast(const vec3& p, const vec3& n, const vec3* w, size_t w_size) const {
		return {};
	};
};

class pointlight : public light {
public:
	vec3 position;
	float intensity = 1;
	float decay = .1;
	
	pointlight(const vec3& p, float i = 1, float d = .1) : position(p), intensity(i), decay(d) {}
	
	vec3 lighting(const vec3& p, const vec3& n) const override {
		float distance = position.distance(p);
		vec3 direction = (position - p).normalize();
		return color * fmax(direction * n * intensity / (distance * distance * decay), 0);
	}
	
	vec3 cast(const vec3& p, const vec3& n, const vec3* w, size_t w_size) const override {
		float distance = position.distance(p);
		vec3 direction = (position - p).normalize();
		for (int f = 0; f < w_size; f += 3) {
			if (intersect(w[f], w[f + 1], w[f + 2], p, direction, distance)) return {};
		}
		return color * fmax(direction * n * intensity / (distance * distance * decay), 0);
	}
};

class directionallight : public light {
public:
	vec3 direction;
	float intensity = 1;
	
	directionallight(const vec3& d, float i = 1) : direction(-d), intensity(i) {}
	
	vec3 lighting(const vec3& p, const vec3& n) const override {
		return color * fmax(direction * n * intensity, 0);
	}
	
	vec3 cast(const vec3& p, const vec3& n, const vec3* w, size_t w_size) const override {
		for (int f = 0; f < w_size; f += 3) {
			if (intersect(w[f], w[f + 1], w[f + 2], p, direction, limit)) return {};
		}
		return color * fmax(direction * n * intensity, 0);
	}
	
private:
	static float limit;
};

inline float directionallight::limit = 1000;

class arealight : public light {
public:
	vec3 position;
	vec3 toright;
	vec3 tolower;
	vec3 normal;
	float intensity = 1;
	float area = 0;
	
	arealight(const vec3& p, const vec3& tr, const vec3& tl, float i = 1) : position(p), toright(tr), tolower(tl),
		normal((tl ^ tr).normalize()), area((tl ^ tr).magnitude()), intensity(i) {}
	
	static bool emit(const vec3& o, const vec3& d, const vec3* w, size_t w_size , vec3& p, vec3& n) {
		float minimum = limit;
		vec3 barycenter;
		bool collided = false;
		for (int f = 0; f < w_size; f += 3) {
			if (intersect(w[f], w[f + 1], w[f + 2], o, d, limit, barycenter)) {
				vec3 position = w[f] * barycenter.x + w[f + 1] * barycenter.y + w[f + 2] * barycenter.z;
				if (o.distance(position) < minimum) {
					minimum = o.distance(position);
					p = position;
					n = (w[f + 1] - w[f]) ^ (w[f + 2] - w[f]);
				}
				collided = true;
			}
		}
		if (collided) n = n.normalize();
		return collided;
	}
	
	vec3 cast(const vec3& p, const vec3& n, const vec3* w, size_t w_size) const override {
		vec3 origin = position + toright * random01f() + tolower * random01f();
		float distance = origin.distance(p);
		vec3 direction = (origin - p).normalize();
		for (int f = 0; f < w_size; f += 3) {
			if (intersect(w[f], w[f + 1], w[f + 2], p, direction, distance)) return {};
		}
		return color * fmax(intensity * (direction * n) * -(direction * normal) * area / (distance * distance), 0);
	}
	
private:
	static float limit;
};

inline float arealight::limit = 1000;

inline void lighting(const vec3& r, vec3& c);

inline void vertex_lighting(const camera& c, const light* const* ls, size_t ls_size, vec3* canvas);

inline void classic_raytracing(const camera& c, const light* const* ls, size_t ls_size,
							   const vec3* w, size_t w_size, vec3* canvas);

inline void path_tracing(const camera& c, const light* const* ls, size_t ls_size,
						 const vec3* w, size_t w_size, vec3* canvas);

}

#include "gl_software.inl"

#endif
