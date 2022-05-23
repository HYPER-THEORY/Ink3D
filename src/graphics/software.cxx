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

#include "software.h"

namespace soft {

vec3 nearest_map(const image& t, float u, float v) {
	int x = u * t.width;
	x = x < 0 ? 0 : x;
	x = x >= t.width ? t.width - 1 : x;
	int y = v * t.height;
	y = y < 0 ? 0 : y;
	y = y >= t.height ? t.height - 1 : y;
	const uint8_t* pointer = t.data.data() + (x + y * t.width) * t.bytes;
	return {pointer[0] / 255.f, pointer[1] / 255.f, pointer[2] / 255.f};
}

vec3 nearest_map(const image& t, const vec2& uv) {
	return nearest_map(t, uv.x, uv.y);
}

vec3 linear_map(const image& t, float u, float v) {
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

vec3 linear_map(const image& t, const vec2& uv) {
	return linear_map(t, uv.x, uv.y);
}

void set_viewport(int w, int h) {
	viewport_w = w;
	viewport_h = h;
}

void add_pointlist(const vec4& v, const vec3& b, pointlist& l) {
	l.vertices[l.size] = v;
	l.barycenters[l.size] = b;
	++l.size;
}

void znear_clip(const pointlist& i, float zn, pointlist& o) {
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

void zfar_clip(const pointlist& i, float zf, pointlist& o) {
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

void rasterize(const pointlist& p, const vec3* dvs, shader& s, vec4* canvas, double* zb) {
	for (int i = 2; i < p.size; ++i) {
		const vec3& vertex_a = dvs[0];
		const vec3& vertex_b = dvs[i - 1];
		const vec3& vertex_c = dvs[i];
		vec3 fixed_a = {p.barycenters[0].x, p.barycenters[0].y, p.barycenters[0].z};
		vec3 fixed_b = {p.barycenters[i - 1].x, p.barycenters[i - 1].y, p.barycenters[i - 1].z};
		vec3 fixed_c = {p.barycenters[i].x, p.barycenters[i].y, p.barycenters[i].z};
		vec3 fixed_barycenter = {1 / p.vertices[0].w, 1 / p.vertices[i - 1].w, 1 / p.vertices[i].w};
		dvec2 v0 = {vertex_c.x - vertex_a.x, vertex_c.y - vertex_a.y};
		dvec2 v1 = {vertex_b.x - vertex_a.x, vertex_b.y - vertex_a.y};
		dvec2 v2 = {};
		double dot00 = v0 * v0;
		double dot01 = v0 * v1;
		double dot11 = v1 * v1;
		double inverse = 1 / (dot00 * dot11 - dot01 * dot01);
		vec3 barycenter;
		vec3 vertex_l = vertex_a;
		vec3 vertex_m = vertex_b;
		vec3 vertex_u = vertex_c;
		if (vertex_l.y > vertex_m.y) std::swap(vertex_l, vertex_m);
		if (vertex_m.y > vertex_u.y) std::swap(vertex_m, vertex_u);
		if (vertex_l.y > vertex_m.y) std::swap(vertex_l, vertex_m);
		float lower = fmaxf(floorf(vertex_l.y) + 1, 0);
		float upper = fminf(floorf(vertex_u.y) + 1, viewport_h);
		float median = vertex_m.y;
		float inverseml = 1 / (vertex_m.y - vertex_l.y);
		float inverseum = 1 / (vertex_u.y - vertex_m.y);
		float inverseul = 1 / (vertex_u.y - vertex_l.y);
		for (float y = lower; y < upper; ++y) {
			float left = y < median ?
				(vertex_l.x * (vertex_m.y - y) + vertex_m.x * (y - vertex_l.y)) * inverseml :
				(vertex_m.x * (vertex_u.y - y) + vertex_u.x * (y - vertex_m.y)) * inverseum;
			float right = (vertex_l.x * (vertex_u.y - y) + vertex_u.x * (y - vertex_l.y)) * inverseul;
			if (left > right) std::swap(left, right);
			left = fmaxf(floorf(left) + 1, 0);
			right = fminf(floorf(right) + 1, viewport_w);
			for (float x = left; x < right; ++x) {
				v2.x = x - vertex_a.x;
				v2.y = y - vertex_a.y;
				double dot02 = v0 * v2;
				double dot12 = v1 * v2;
				double u = (dot11 * dot02 - dot01 * dot12) * inverse;
				double v = (dot00 * dot12 - dot01 * dot02) * inverse;
				if (UNLIKELY(u < -eps || v < -eps || u + v > 1 + eps)) continue;
				double z = vertex_a.z * (1 - u - v) + vertex_b.z * v + vertex_c.z * u;
				int location = x + y * viewport_w;
				if (z > -1 && z < 1 && z < zb[location] + eps) {
					zb[location] = z;
					barycenter.x = (1 - u - v) * fixed_barycenter.x;
					barycenter.y = v * fixed_barycenter.y;
					barycenter.z = u * fixed_barycenter.z;
					barycenter /= barycenter.x + barycenter.y + barycenter.z;
					barycenter = fixed_a * barycenter.x + fixed_b * barycenter.y + fixed_c * barycenter.z;
					s.fragment(barycenter, {x / viewport_w, y / viewport_h}, canvas[location]);
				}
			}
		}
	}
}

void rasterize(const pointlist& p, const vec3* dvs, double* zb) {
	for (int i = 2; i < p.size; ++i) {
		const vec3& vertex_a = dvs[0];
		const vec3& vertex_b = dvs[i - 1];
		const vec3& vertex_c = dvs[i];
		dvec2 v0 = {vertex_c.x - vertex_a.x, vertex_c.y - vertex_a.y};
		dvec2 v1 = {vertex_b.x - vertex_a.x, vertex_b.y - vertex_a.y};
		dvec2 v2 = {};
		double dot00 = v0 * v0;
		double dot01 = v0 * v1;
		double dot11 = v1 * v1;
		double inverse = 1 / (dot00 * dot11 - dot01 * dot01);
		vec3 vertex_l = vertex_a;
		vec3 vertex_m = vertex_b;
		vec3 vertex_u = vertex_c;
		if (vertex_l.y > vertex_m.y) std::swap(vertex_l, vertex_m);
		if (vertex_m.y > vertex_u.y) std::swap(vertex_m, vertex_u);
		if (vertex_l.y > vertex_m.y) std::swap(vertex_l, vertex_m);
		float lower = fmaxf(floorf(vertex_l.y) + 1, 0);
		float upper = fminf(floorf(vertex_u.y) + 1, viewport_h);
		float median = vertex_m.y;
		float inverseml = 1 / (vertex_m.y - vertex_l.y);
		float inverseum = 1 / (vertex_u.y - vertex_m.y);
		float inverseul = 1 / (vertex_u.y - vertex_l.y);
		for (float y = lower; y < upper; ++y) {
			float left = y < median ?
				(vertex_l.x * (vertex_m.y - y) + vertex_m.x * (y - vertex_l.y)) * inverseml :
				(vertex_m.x * (vertex_u.y - y) + vertex_u.x * (y - vertex_m.y)) * inverseum;
			float right = (vertex_l.x * (vertex_u.y - y) + vertex_u.x * (y - vertex_l.y)) * inverseul;
			if (left > right) std::swap(left, right);
			left = fmaxf(floorf(left) + 1, 0);
			right = fminf(floorf(right) + 1, viewport_w);
			for (float x = left; x < right; ++x) {
				v2.x = x - vertex_a.x;
				v2.y = y - vertex_a.y;
				double dot02 = v0 * v2;
				double dot12 = v1 * v2;
				double u = (dot11 * dot02 - dot01 * dot12) * inverse;
				double v = (dot00 * dot12 - dot01 * dot02) * inverse;
				if (UNLIKELY(u < -eps || v < -eps || u + v > 1 + eps)) continue;
				double z = vertex_a.z * (1 - u - v) + vertex_b.z * v + vertex_c.z * u;
				int location = x + y * viewport_w;
				if (z > -1 && z < 1 && z < zb[location] + eps) zb[location] = z;
			}
		}
	}
}

void draw(const camera& c, shader& s, const mesh& m, vec4* canvas, double* zb) {
	vec3 barycenters[3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
	size_t length = m.vertex.size();
	for (int i = 0; i < length; i += 3) {
		vec4 vertices[3];
		/* vertex shader */
		for (int j = 0; j < 3; ++j) {
			s.vextex(m, i + j, j, vertices[j]);
		}
		/* geometry shader */
		s.geometry(vertices);
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
			device_vertices[i].x = device_vertices[i].x * viewport_w / 2 + viewport_w / 2;
			device_vertices[i].y = -device_vertices[i].y * viewport_h / 2 + viewport_h / 2;
		}
		/* rasterization */
		rasterize(primitive, device_vertices, s, canvas, zb);
	}
}

void draw(const camera& c, shader& s, const mesh& m, double* zb) {
	vec3 barycenters[3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
	size_t length = m.vertex.size();
	for (int i = 0; i < length; i += 3) {
		vec4 vertices[3];
		/* vertex shader */
		for (int j = 0; j < 3; ++j) {
			s.vextex(m, i + j, j, vertices[j]);
		}
		/* geometry shader */
		s.geometry(vertices);
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
			device_vertices[i].x = device_vertices[i].x * viewport_w / 2 + viewport_w / 2;
			device_vertices[i].y = -device_vertices[i].y * viewport_h / 2 + viewport_h / 2;
		}
		/* rasterization */
		rasterize(primitive, device_vertices, zb);
	}
}

void draw_instances(const camera& c, shader& s, const instance* const* is,
					const image** const* ts, size_t size, vec4* canvas) {
	size_t buffer_size = viewport_w * viewport_h;
	if (zbuffer.size() != buffer_size) zbuffer.resize(buffer_size);
	std::fill(zbuffer.begin(), zbuffer.end(), 1);
	mat4 view_proj = c.projection * c.viewing;
	for (int i = 0; i < size; ++i) {
		s.model = camera::model_transform(*is[i]);
		s.view = c.viewing;
		s.proj = c.projection;
		s.model_view_proj = view_proj * s.model;
		s.textures = ts[i];
		draw(c, s, *is[i]->mesh_ptr, canvas, zbuffer.data());
	}
}

}
