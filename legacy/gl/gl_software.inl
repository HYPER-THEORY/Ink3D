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

#ifndef GL_SOFTWARE_INL
#define GL_SOFTWARE_INL

#include "gl_software.hpp"
#include "gl_stdinc.hpp"
#include "gl_type.hpp"
#include "gl_camera.hpp"

namespace gl::soft {

inline const vec3 nearest_map(const texture& t, float u, float v) {
	int x = u * t.width;
	x = x < 0 ? 0 : x;
	x = x >= t.width ? t.width - 1 : x;
	int y = v * t.height;
	y = y < 0 ? 0 : y;
	y = y >= t.height ? t.height - 1 : y;
	const uint8_t* pointer = t.data.data() + (x + y * t.width) * t.bytes;
	return {pointer[0] / 255.f, pointer[1] / 255.f, pointer[2] / 255.f};
}

inline const vec3 nearest_map(const texture& t, const vec2& uv) {
	return nearest_map(t, uv.x, uv.y);
}

inline const vec3 linear_map(const texture& t, float u, float v) {
	int x0 = u * t.width;
	x0 = x0 < 0 ? 0 : x0;
	x0 = x0 >= t.width ? t.width - 1 : x0;
	int x1 = x0 + 1;
	x1 = x1 >= t.width ? t.width - 1 : x1;
	int y0 = v * t.height;
	y0 = y0 < 0 ? 0 : y0;
	y0 = y0 >= t.height ? t.height - 1 : y0;
	int y1 = y0 + 1;
	y1 = y1 >= t.height ? t.height - 1 : y1;
	const uint8_t* pointer00 = t.data.data() + (x0 + y0 * t.width) * t.bytes;
	const uint8_t* pointer01 = t.data.data() + (x0 + y1 * t.width) * t.bytes;
	const uint8_t* pointer10 = t.data.data() + (x1 + y0 * t.width) * t.bytes;
	const uint8_t* pointer11 = t.data.data() + (x1 + y1 * t.width) * t.bytes;
	vec3 color00 = {pointer00[0] / 255.f, pointer00[1] / 255.f, pointer00[2] / 255.f};
	vec3 color01 = {pointer01[0] / 255.f, pointer01[1] / 255.f, pointer01[2] / 255.f};
	vec3 color10 = {pointer10[0] / 255.f, pointer10[1] / 255.f, pointer10[2] / 255.f};
	vec3 color11 = {pointer11[0] / 255.f, pointer11[1] / 255.f, pointer11[2] / 255.f};
	return (color00 * (y1 - v) + color01 * (v - y0)) * (x1 - u) + (color10 * (y1 - v) + color11 * (v - y0)) * (u - x0);
}

inline const vec3 linear_map(const texture& t, const vec2& uv) {
	return linear_map(t, uv.x, uv.y);
}

inline void add_pointlist(const vec4& v, const vec3& b, pointlist& l) {
	l.vertices[l.size] = v;
	l.barycenters[l.size] = b;
	++l.size;
}

inline void znear_clip(const pointlist& i, float zn, pointlist& o) {
	for (int l = 0; l < i.size; ++l) {
		vec4& vertex1 = i.vertices[l];
		vec4& vertex2 = i.vertices[(l + 1) % i.size];
		/* both outside */
		if (vertex1.w < zn && vertex2.w < zn) continue;
		/* both inside */
		const vec3& barycenter1 = i.barycenters[l];
		const vec3& barycenter2 = i.barycenters[(l + 1) % i.size];
		if (vertex1.w > zn && vertex2.w > zn) {
			add_pointlist(vertex2, barycenter2, o);
			continue;
		}
		/* from inside to outside */
		float weight1 = fabsf(vertex1.w - zn);
		float weight2 = fabsf(vertex2.w - zn);
		float inverse = 1 / (weight1 + weight2);
		if (vertex1.w > zn && vertex2.w < zn) {
			add_pointlist((vertex1 * weight2 + vertex2 * weight1) * inverse,
				(barycenter1 * weight2 + barycenter2 * weight1) * inverse, o);
			continue;
		}
		/* from outside to inside */
		add_pointlist((vertex1 * weight2 + vertex2 * weight1) * inverse,
			(barycenter1 * weight2 + barycenter2 * weight1) * inverse, o);
		add_pointlist(vertex2, barycenter2, o);
	}
}

inline void zfar_clip(const pointlist& i, float zf, pointlist& o) {
	for (int l = 0; l < i.size; ++l) {
		vec4& vertex1 = i.vertices[l];
		vec4& vertex2 = i.vertices[(l + 1) % i.size];
		/* both outside */
		if (vertex1.w > zf && vertex2.w > zf) continue;
		/* both inside */
		const vec3& barycenter1 = i.barycenters[l];
		const vec3& barycenter2 = i.barycenters[(l + 1) % i.size];
		if (vertex1.w < zf && vertex2.w < zf) {
			add_pointlist(vertex2, barycenter2, o);
			continue;
		}
		/* from inside to outside */
		float weight1 = fabsf(vertex1.w - zf);
		float weight2 = fabsf(vertex2.w - zf);
		float inverse = 1 / (weight1 + weight2);
		if (vertex1.w > zf && vertex2.w < zf) {
			add_pointlist((vertex1 * weight2 + vertex2 * weight1) * inverse,
				(barycenter1 * weight2 + barycenter2 * weight1) * inverse, o);
			continue;
		}
		/* from outside to inside */
		add_pointlist((vertex1 * weight2 + vertex2 * weight1) * inverse,
			(barycenter1 * weight2 + barycenter2 * weight1) * inverse, o);
		add_pointlist(vertex2, barycenter2, o);
	}
}

inline void rasterize(const camera& c, const pointlist& p, const vec3* dvs,
					  shader& s, vec3* canvas, double* zb, ginfo* gb) {
	for (int i = 2; i < p.size; ++i) {
		const vec3& vertexa = dvs[0];
		const vec3& vertexb = dvs[i - 1];
		const vec3& vertexc = dvs[i];
		vec3 fixeda = {p.barycenters[0].x, p.barycenters[0].y, p.barycenters[0].z};
		vec3 fixedb = {p.barycenters[i - 1].x, p.barycenters[i - 1].y, p.barycenters[i - 1].z};
		vec3 fixedc = {p.barycenters[i].x, p.barycenters[i].y, p.barycenters[i].z};
		vec3 fixed_barycenter = {1 / p.vertices[0].w, 1 / p.vertices[i - 1].w, 1 / p.vertices[i].w};
		vec2d v0 = {vertexc.x - vertexa.x, vertexc.y - vertexa.y};
		vec2d v1 = {vertexb.x - vertexa.x, vertexb.y - vertexa.y};
		vec2d v2;
		double dot00 = v0 * v0;
		double dot01 = v0 * v1;
		double dot11 = v1 * v1;
		double inverse = 1 / (dot00 * dot11 - dot01 * dot01);
		vec3 barycenter;
		vec4 color;
		vec3 vertexl = vertexa;
		vec3 vertexm = vertexb;
		vec3 vertexu = vertexc;
		if (vertexl.y > vertexm.y) std::swap(vertexl, vertexm);
		if (vertexm.y > vertexu.y) std::swap(vertexm, vertexu);
		if (vertexl.y > vertexm.y) std::swap(vertexl, vertexm);
		float lower = fmaxf(floorf(vertexl.y) + 1, 0);
		float upper = fminf(floorf(vertexu.y) + 1, c.height);
		float median = vertexm.y;
		float inverseml = 1 / (vertexm.y - vertexl.y);
		float inverseum = 1 / (vertexu.y - vertexm.y);
		float inverseul = 1 / (vertexu.y - vertexl.y);
		for (float y = lower; y < upper; ++y) {
			float left = y < median ? (vertexl.x * (vertexm.y - y) + vertexm.x * (y - vertexl.y)) * inverseml :
				(vertexm.x * (vertexu.y - y) + vertexu.x * (y - vertexm.y)) * inverseum;
			float right = (vertexl.x * (vertexu.y - y) + vertexu.x * (y - vertexl.y)) * inverseul;
			if (left > right) std::swap(left, right);
			left = fmaxf(floorf(left) + 1, 0);
			right = fminf(floorf(right) + 1, c.width);
			for (float x = left; x < right; ++x) {
				v2.x = x - vertexa.x;
				v2.y = y - vertexa.y;
				double dot02 = v0 * v2;
				double dot12 = v1 * v2;
				double u = (dot11 * dot02 - dot01 * dot12) * inverse;
				double v = (dot00 * dot12 - dot01 * dot02) * inverse;
				if (UNLIKELY(u < -eps || v < -eps || u + v > 1 + eps)) continue;
				double z = vertexa.z * (1 - u - v) + vertexb.z * v + vertexc.z * u;
				int location = x + y * c.width;
				if (z > -1 && z < 1 && z < zb[location] + eps) {
					zb[location] = z;
					barycenter.x = (1 - u - v) * fixed_barycenter.x;
					barycenter.y = v * fixed_barycenter.y;
					barycenter.z = u * fixed_barycenter.z;
					barycenter /= barycenter.x + barycenter.y + barycenter.z;
					barycenter = fixeda * barycenter.x + fixedb * barycenter.y + fixedc * barycenter.z;
					s.fragment_shader(barycenter, {x / c.width, y / c.height}, gb[location], color);
					canvas[location].x = canvas[location].x * (1 - color.w) + color.x * color.w;
					canvas[location].y = canvas[location].y * (1 - color.w) + color.y * color.w;
					canvas[location].z = canvas[location].z * (1 - color.w) + color.z * color.w;
				}
			}
		}
	}
}

template <bool write>
inline bool rasterize(const camera& c, const pointlist& p, const vec3* dvs, double* zb) {
	for (int i = 2; i < p.size; ++i) {
		const vec3& vertexa = dvs[0];
		const vec3& vertexb = dvs[i - 1];
		const vec3& vertexc = dvs[i];
		vec2d v0 = {vertexc.x - vertexa.x, vertexc.y - vertexa.y};
		vec2d v1 = {vertexb.x - vertexa.x, vertexb.y - vertexa.y};
		vec2d v2;
		double dot00 = v0 * v0;
		double dot01 = v0 * v1;
		double dot11 = v1 * v1;
		double inverse = 1 / (dot00 * dot11 - dot01 * dot01);
		vec3 vertexl = vertexa;
		vec3 vertexm = vertexb;
		vec3 vertexu = vertexc;
		if (vertexl.y > vertexm.y) std::swap(vertexl, vertexm);
		if (vertexm.y > vertexu.y) std::swap(vertexm, vertexu);
		if (vertexl.y > vertexm.y) std::swap(vertexl, vertexm);
		float lower = fmaxf(floorf(vertexl.y) + 1, 0);
		float upper = fminf(floorf(vertexu.y) + 1, c.height);
		float median = vertexm.y;
		float inverseml = 1 / (vertexm.y - vertexl.y);
		float inverseum = 1 / (vertexu.y - vertexm.y);
		float inverseul = 1 / (vertexu.y - vertexl.y);
		for (float y = lower; y < upper; ++y) {
			float left = y < median ? (vertexl.x * (vertexm.y - y) + vertexm.x * (y - vertexl.y)) * inverseml :
				(vertexm.x * (vertexu.y - y) + vertexu.x * (y - vertexm.y)) * inverseum;
			float right = (vertexl.x * (vertexu.y - y) + vertexu.x * (y - vertexl.y)) * inverseul;
			if (left > right) std::swap(left, right);
			left = fmaxf(floorf(left) + 1, 0);
			right = fminf(floorf(right) + 1, c.width);
			for (float x = left; x < right; ++x) {
				v2.x = x - vertexa.x;
				v2.y = y - vertexa.y;
				double dot02 = v0 * v2;
				double dot12 = v1 * v2;
				double u = (dot11 * dot02 - dot01 * dot12) * inverse;
				double v = (dot00 * dot12 - dot01 * dot02) * inverse;
				if (UNLIKELY(u < -eps || v < -eps || u + v > 1 + eps)) continue;
				double z = vertexa.z * (1 - u - v) + vertexb.z * v + vertexc.z * u;
				int location = x + y * c.width;
				if (z > -1 && z < 1 && z < zb[location] + eps) {
					if (!write) return true;
					zb[location] = z;
				}
			}
		}
	}
	return false;
}

inline void draw_objects(const camera& c, shader& s, const vertexobject& vo, vec3* canvas, double* zb, ginfo* gb) {
	static vec3 barycenters[3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
	std::vector<const float*> datas(vo.data.size());
	for (int i = 0; i < vo.length; i += 3) {
		vec4 vertices[3];
		/* vertex shader */
		for (int j = 0; j < 3; ++j) {
			for (size_t k = datas.size(); k --> 0;) {
				datas[k] = vo.data[k].data() + vo.size[k] * (i + j);
			}
			s.vextex_shader(datas.data(), j, vertices[j]);
		}
		/* geometry shader */
		s.geometry_shader(vertices);
		/* clipping */
		vec4 clip_vertices[4];
		vec3 clip_barycenters[4];
		pointlist clip_primitive = {0, clip_vertices, clip_barycenters};
		znear_clip({3, vertices, barycenters}, c.znear, clip_primitive);
		vec4 primitive_vertices[5];
		vec3 primitive_barycenters[5];
		pointlist primitive = {0, primitive_vertices, primitive_barycenters};
		zfar_clip(clip_primitive, c.zfar, primitive);
		/* perspective division */
		vec3 device_vertices[5];
		for (int i = 0; i < primitive.size; ++i) {
			device_vertices[i].x = primitive.vertices[i].x / primitive.vertices[i].w;
			device_vertices[i].y = primitive.vertices[i].y / primitive.vertices[i].w;
			device_vertices[i].z = primitive.vertices[i].z / primitive.vertices[i].w;
		}
		/* viewport transform */
		for (int i = 0; i < primitive.size; ++i) {
			device_vertices[i].x = device_vertices[i].x * c.width / 2 + c.width / 2;
			device_vertices[i].y = -device_vertices[i].y * c.height / 2 + c.height / 2;
		}
		/* rasterization */
		rasterize(c, primitive, device_vertices, s, canvas, zb, gb);
	}
}

inline void ztest(const camera& c, shader& s, const vertexobject& vo, double* zb) {
	static vec3 barycenters[3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
	std::vector<const float*> datas(vo.data.size());
	for (int i = 0; i < vo.length; i += 3) {
		vec4 vertices[3];
		/* vertex shader */
		for (int j = 0; j < 3; ++j) {
			for (size_t k = datas.size(); k --> 0;) {
				datas[k] = vo.data[k].data() + vo.size[k] * (i + j);
			}
			s.vextex_shader(datas.data(), j, vertices[j]);
		}
		/* geometry shader */
		s.geometry_shader(vertices);
		/* clipping */
		vec4 clip_vertices[4];
		vec3 clip_barycenters[4];
		pointlist clip_primitive = {0, clip_vertices, clip_barycenters};
		znear_clip({3, vertices, barycenters}, c.znear, clip_primitive);
		vec4 primitive_vertices[5];
		vec3 primitive_barycenters[5];
		pointlist primitive = {0, primitive_vertices, primitive_barycenters};
		zfar_clip(clip_primitive, c.zfar, primitive);
		/* perspective division */
		vec3 device_vertices[5];
		for (int i = 0; i < primitive.size; ++i) {
			device_vertices[i].x = primitive.vertices[i].x / primitive.vertices[i].w;
			device_vertices[i].y = primitive.vertices[i].y / primitive.vertices[i].w;
			device_vertices[i].z = primitive.vertices[i].z / primitive.vertices[i].w;
		}
		/* viewport transform */
		for (int i = 0; i < primitive.size; ++i) {
			device_vertices[i].x = device_vertices[i].x * c.width / 2 + c.width / 2;
			device_vertices[i].y = -device_vertices[i].y * c.height / 2 + c.height / 2;
		}
		/* rasterization */
		rasterize(c, primitive, device_vertices, zb);
	}
}

inline bool zquery(const camera& c, shader& s, const vertexobject& vo, double* zb) {
	static vec3 barycenters[3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
	std::vector<const float*> datas(vo.data.size());
	for (int i = 0; i < vo.length; i += 3) {
		vec4 vertices[3];
		/* vertex shader */
		for (int j = 0; j < 3; ++j) {
			for (size_t k = datas.size(); k --> 0;) {
				datas[k] = vo.data[k].data() + vo.size[k] * (i + j);
			}
			s.vextex_shader(datas.data(), j, vertices[j]);
		}
		/* geometry shader */
		s.geometry_shader(vertices);
		/* clipping */
		vec4 clip_vertices[4];
		vec3 clip_barycenters[4];
		pointlist clip_primitive = {0, clip_vertices, clip_barycenters};
		znear_clip({3, vertices, barycenters}, c.znear, clip_primitive);
		vec4 primitive_vertices[5];
		vec3 primitive_barycenters[5];
		pointlist primitive = {0, primitive_vertices, primitive_barycenters};
		zfar_clip(clip_primitive, c.zfar, primitive);
		/* perspective division */
		vec3 device_vertices[5];
		for (int i = 0; i < primitive.size; ++i) {
			device_vertices[i].x = primitive.vertices[i].x / primitive.vertices[i].w;
			device_vertices[i].y = primitive.vertices[i].y / primitive.vertices[i].w;
			device_vertices[i].z = primitive.vertices[i].z / primitive.vertices[i].w;
		}
		/* viewport transform */
		for (int i = 0; i < primitive.size; ++i) {
			device_vertices[i].x = device_vertices[i].x * c.width / 2 + c.width / 2;
			device_vertices[i].y = -device_vertices[i].y * c.height / 2 + c.height / 2;
		}
		/* rasterization */
		if (rasterize<false>(c, primitive, device_vertices, zb)) return true;
	}
	return false;
}

inline void draw_instances(const camera& c, shader& s, const instance* const* is, size_t is_size, vec3* canvas) {
	size_t buffer_size = c.width * c.height;
	if (zbuffer.size() != buffer_size) zbuffer.resize(buffer_size);
	if (gbuffer.size() != buffer_size) gbuffer.resize(buffer_size);
	mat<4, 4> vptransform = c.projection * c.viewing;
	std::fill(zbuffer.begin(), zbuffer.end(), 1);
	for (int i = 0; i < is_size; ++i) {
		camera::model_transform(*is[i], s.model);
		s.viewing = c.viewing;
		s.projection = c.projection;
		s.transform = vptransform * s.model;
		s.textures = is[i]->textures.data();
		draw_objects(c, s, *is[i]->object, canvas, zbuffer.data(), gbuffer.data());
	}
}

inline void draw_scene(const camera& c, shader& s, const scene& scene_info, vec3* canvas) {
	draw_instances(c, s, scene_info.get_instances(), scene_info.instance_size(), canvas);
}

inline void lighting(const vec3& r, vec3& c) {
	c.x *= fminf(r.x, 1);
	c.y *= fminf(r.y, 1);
	c.z *= fminf(r.z, 1);
}

inline void vertex_lighting(const camera& c, const light* const* ls, size_t ls_size, vec3* canvas) {
	for (int i = c.width * c.height; i --> 0;) {
		if (zbuffer[i] == 1) continue;
		vec3 radiance;
		for (int l = 0; l < ls_size; ++l) {
			radiance += ls[l]->lighting(gbuffer[i].world_coord, gbuffer[i].normal);
		}
		lighting(radiance, canvas[i]);
	}
}

inline void classic_raytracing(const camera& c, const light* const* ls, size_t ls_size,
							   const vec3* w, size_t w_size, vec3* canvas) {
	for (int i = c.width * c.height; i --> 0;) {
		if (zbuffer[i] == 1) continue;
		vec3 radiance;
		for (int l = 0; l < ls_size; ++l) {
			radiance += ls[l]->cast(gbuffer[i].world_coord, gbuffer[i].normal, w, w_size);
		}
		lighting(radiance, canvas[i]);
	}
}

inline void path_tracing(const camera& c, const light* const* ls, size_t ls_size,
						 const vec3* w, size_t w_size, vec3* canvas) {
	static float brdf = .2f;
	std::vector<const arealight*> lights(ls_size);
	for (size_t i = ls_size; i --> 0;) {
		lights[i] = static_cast<const arealight*>(ls[i]);
	}
	for (int i = c.width * c.height; i --> 0;) {
		if (zbuffer[i] == 1) continue;
		float probability = .6f;
		float coefficient = 1;
		vec3 radiance;
		vec3 position = gbuffer[i].world_coord;
		vec3 normal = gbuffer[i].normal;
		while (true) {
			for (auto& l : lights) {
				radiance += l->cast(position, normal, w, w_size) * coefficient;
			}
			if (probability <= random01f()) break;
			vec3 direction = vec3::random();
			if (direction * normal < 0) direction = -direction;
			coefficient *= direction * normal * brdf / probability;
			if (!arealight::emit(position + direction * eps, direction, w, w_size, position, normal)) break;
		}
		lighting(radiance, canvas[i]);
	}
}

}

#endif
