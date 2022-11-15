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

#include "Software.h"

#ifndef LIKELY
#define	LIKELY(x)   __builtin_expect((x), 1)
#endif

#ifndef UNLIKELY
#define	UNLIKELY(x) __builtin_expect((x), 0)
#endif

namespace Ink::Soft {

constexpr float EPS = 1E-6;

Vec3 nearest_map(const Image& t, float u, float v) {
	int x = u * t.width;
	x = x < 0 ? 0 : x;
	x = x >= t.width ? t.width - 1 : x;
	int y = v * t.height;
	y = y < 0 ? 0 : y;
	y = y >= t.height ? t.height - 1 : y;
	int bpp = t.channel * t.bytes;
	const uint8_t* ptr = t.data.data() + (x + y * t.width) * bpp;
	return Vec3(ptr[0], ptr[1], ptr[2]) / 255.f;
}

Vec3 nearest_map(const Image& t, const Vec2& uv) {
	return nearest_map(t, uv.x, uv.y);
}

Vec3 linear_map(const Image& t, float u, float v) {
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
	int bpp = t.channel * t.bytes;
	const uint8_t* data = t.data.data();
	const uint8_t* ptr_00 = data + (x0 + y0 * t.width) * bpp;
	const uint8_t* ptr_01 = data + (x0 + y1 * t.width) * bpp;
	const uint8_t* ptr_10 = data + (x1 + y0 * t.width) * bpp;
	const uint8_t* ptr_11 = data + (x1 + y1 * t.width) * bpp;
	Vec3 color00 = Vec3(ptr_00[0], ptr_00[1], ptr_00[2]) / 255.f;
	Vec3 color01 = Vec3(ptr_01[0], ptr_01[1], ptr_01[2]) / 255.f;
	Vec3 color10 = Vec3(ptr_10[0], ptr_10[1], ptr_10[2]) / 255.f;
	Vec3 color11 = Vec3(ptr_11[0], ptr_11[1], ptr_11[2]) / 255.f;
	return (color00 * (y1 - v) + color01 * (v - y0)) * (x1 - u) +
		(color10 * (y1 - v) + color11 * (v - y0)) * (u - x0);
}

Vec3 linear_map(const Image& t, const Vec2& uv) {
	return linear_map(t, uv.x, uv.y);
}

void set_viewport(int w, int h) {
	viewport_w = w;
	viewport_h = h;
}

void add_point(PointList& l, const Vec4& v, const Vec3& b) {
	l.vertices[l.size] = v;
	l.barycenters[l.size] = b;
	++l.size;
}

void znear_clip(const PointList& i, float z, PointList& o) {
	for (int l = 0; l < i.size; ++l) {
		Vec4& vertex1 = i.vertices[l];
		Vec4& vertex2 = i.vertices[(l + 1) % i.size];
		
		/* vertices are both outside */
		if (vertex1.w < z && vertex2.w < z) continue;
		
		/* vertices are both inside */
		const Vec3& barycenter1 = i.barycenters[l];
		const Vec3& barycenter2 = i.barycenters[(l + 1) % i.size];
		if (vertex1.w > z && vertex2.w > z) {
			add_point(o, vertex2, barycenter2);
			continue;
		}
		
		/* traveling from inside to outside */
		float weight1 = fabsf(vertex1.w - z);
		float weight2 = fabsf(vertex2.w - z);
		float inverse = 1 / (weight1 + weight2);
		if (vertex1.w > z && vertex2.w < z) {
			add_point(o, (vertex1 * weight2 + vertex2 * weight1) * inverse,
					  (barycenter1 * weight2 + barycenter2 * weight1) * inverse);
			continue;
		}
		
		/* traveling from outside to inside */
		add_point(o, (vertex1 * weight2 + vertex2 * weight1) * inverse,
				  (barycenter1 * weight2 + barycenter2 * weight1) * inverse);
		add_point(o, vertex2, barycenter2);
	}
}

void zfar_clip(const PointList& i, float z, PointList& o) {
	for (int l = 0; l < i.size; ++l) {
		Vec4& vertex1 = i.vertices[l];
		Vec4& vertex2 = i.vertices[(l + 1) % i.size];
		
		/* vertices are both outside */
		if (vertex1.w > z && vertex2.w > z) continue;
		
		/* vertices are both inside */
		const Vec3& barycenter1 = i.barycenters[l];
		const Vec3& barycenter2 = i.barycenters[(l + 1) % i.size];
		if (vertex1.w < z && vertex2.w < z) {
			add_point(o, vertex2, barycenter2);
			continue;
		}
		
		/* traveling from inside to outside */
		float weight1 = fabsf(vertex1.w - z);
		float weight2 = fabsf(vertex2.w - z);
		float inverse = 1 / (weight1 + weight2);
		if (vertex1.w > z && vertex2.w < z) {
			add_point(o, (vertex1 * weight2 + vertex2 * weight1) * inverse,
					  (barycenter1 * weight2 + barycenter2 * weight1) * inverse);
			continue;
		}
		
		/* traveling from outside to inside */
		add_point(o, (vertex1 * weight2 + vertex2 * weight1) * inverse,
				  (barycenter1 * weight2 + barycenter2 * weight1) * inverse);
		add_point(o, vertex2, barycenter2);
	}
}

void rasterize(const PointList& p, const Vec3* d, Shader& s, double* zb, Vec4* canvas) {
	for (int i = 2; i < p.size; ++i) {
		const Vec3& vertex_a = d[0];
		const Vec3& vertex_b = d[i - 1];
		const Vec3& vertex_c = d[i];
		Vec3 fixed_a = {p.barycenters[0].x, p.barycenters[0].y, p.barycenters[0].z};
		Vec3 fixed_b = {p.barycenters[i - 1].x, p.barycenters[i - 1].y, p.barycenters[i - 1].z};
		Vec3 fixed_c = {p.barycenters[i].x, p.barycenters[i].y, p.barycenters[i].z};
		Vec3 fixed_barycenter = {1 / p.vertices[0].w, 1 / p.vertices[i - 1].w, 1 / p.vertices[i].w};
		DVec2 v0 = {vertex_c.x - vertex_a.x, vertex_c.y - vertex_a.y};
		DVec2 v1 = {vertex_b.x - vertex_a.x, vertex_b.y - vertex_a.y};
		DVec2 v2 = {};
		double dot00 = v0.dot(v0);
		double dot01 = v0.dot(v1);
		double dot11 = v1.dot(v1);
		double inverse = 1 / (dot00 * dot11 - dot01 * dot01);
		Vec3 barycenter;
		Vec3 vertex_l = vertex_a;
		Vec3 vertex_m = vertex_b;
		Vec3 vertex_u = vertex_c;
		if (vertex_l.y > vertex_m.y) std::swap(vertex_l, vertex_m);
		if (vertex_m.y > vertex_u.y) std::swap(vertex_m, vertex_u);
		if (vertex_l.y > vertex_m.y) std::swap(vertex_l, vertex_m);
		float lower = fmaxf(floorf(vertex_l.y) + 1, 0);
		float upper = fminf(floorf(vertex_u.y) + 1, viewport_h);
		float median = vertex_m.y;
		float inverse_ml = 1 / (vertex_m.y - vertex_l.y);
		float inverse_um = 1 / (vertex_u.y - vertex_m.y);
		float inverse_ul = 1 / (vertex_u.y - vertex_l.y);
		for (float y = lower; y < upper; ++y) {
			float left = y < median ?
				(vertex_l.x * (vertex_m.y - y) + vertex_m.x * (y - vertex_l.y)) * inverse_ml :
				(vertex_m.x * (vertex_u.y - y) + vertex_u.x * (y - vertex_m.y)) * inverse_um;
			float right = (vertex_l.x * (vertex_u.y - y) + vertex_u.x * (y - vertex_l.y)) * inverse_ul;
			if (left > right) std::swap(left, right);
			left = fmaxf(floorf(left) + 1, 0);
			right = fminf(floorf(right) + 1, viewport_w);
			for (float x = left; x < right; ++x) {
				v2.x = x - vertex_a.x;
				v2.y = y - vertex_a.y;
				double dot02 = v0.dot(v2);
				double dot12 = v1.dot(v2);
				double u = (dot11 * dot02 - dot01 * dot12) * inverse;
				double v = (dot00 * dot12 - dot01 * dot02) * inverse;
				if (UNLIKELY(u < -EPS || v < -EPS || u + v > 1 + EPS)) continue;
				double z = vertex_a.z * (1 - u - v) + vertex_b.z * v + vertex_c.z * u;
				int location = x + y * viewport_w;
				if (z > -1 && z < 1 && z < zb[location] + EPS) {
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

void rasterize(const PointList& p, const Vec3* d, double* zb) {
	for (int i = 2; i < p.size; ++i) {
		const Vec3& vertex_a = d[0];
		const Vec3& vertex_b = d[i - 1];
		const Vec3& vertex_c = d[i];
		DVec2 v0 = {vertex_c.x - vertex_a.x, vertex_c.y - vertex_a.y};
		DVec2 v1 = {vertex_b.x - vertex_a.x, vertex_b.y - vertex_a.y};
		DVec2 v2 = {};
		double dot00 = v0.dot(v0);
		double dot01 = v0.dot(v1);
		double dot11 = v1.dot(v1);
		double inverse = 1 / (dot00 * dot11 - dot01 * dot01);
		Vec3 vertex_l = vertex_a;
		Vec3 vertex_m = vertex_b;
		Vec3 vertex_u = vertex_c;
		if (vertex_l.y > vertex_m.y) std::swap(vertex_l, vertex_m);
		if (vertex_m.y > vertex_u.y) std::swap(vertex_m, vertex_u);
		if (vertex_l.y > vertex_m.y) std::swap(vertex_l, vertex_m);
		float lower = fmaxf(floorf(vertex_l.y) + 1, 0);
		float upper = fminf(floorf(vertex_u.y) + 1, viewport_h);
		float median = vertex_m.y;
		float inverse_ml = 1 / (vertex_m.y - vertex_l.y);
		float inverse_um = 1 / (vertex_u.y - vertex_m.y);
		float inverse_ul = 1 / (vertex_u.y - vertex_l.y);
		for (float y = lower; y < upper; ++y) {
			float left = y < median ?
				(vertex_l.x * (vertex_m.y - y) + vertex_m.x * (y - vertex_l.y)) * inverse_ml :
				(vertex_m.x * (vertex_u.y - y) + vertex_u.x * (y - vertex_m.y)) * inverse_um;
			float right = (vertex_l.x * (vertex_u.y - y) + vertex_u.x * (y - vertex_l.y)) * inverse_ul;
			if (left > right) std::swap(left, right);
			left = fmaxf(floorf(left) + 1, 0);
			right = fminf(floorf(right) + 1, viewport_w);
			for (float x = left; x < right; ++x) {
				v2.x = x - vertex_a.x;
				v2.y = y - vertex_a.y;
				double dot02 = v0.dot(v2);
				double dot12 = v1.dot(v2);
				double u = (dot11 * dot02 - dot01 * dot12) * inverse;
				double v = (dot00 * dot12 - dot01 * dot02) * inverse;
				if (UNLIKELY(u < -EPS || v < -EPS || u + v > 1 + EPS)) continue;
				double z = vertex_a.z * (1 - u - v) + vertex_b.z * v + vertex_c.z * u;
				int location = x + y * viewport_w;
				if (z > -1 && z < 1 && z < zb[location] + EPS) zb[location] = z;
			}
		}
	}
}

void draw(const Camera& c, Shader& s, const Mesh& m, double* zb, Vec4* canvas) {
	Vec3 barycenters[3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
	size_t length = m.vertex.size();
	for (int i = 0; i < length; i += 3) {
		
		/* vertex shader & geometry shader */
		Vec4 vertices[3];
		for (int j = 0; j < 3; ++j) {
			s.vextex(m, i + j, j, vertices[j]);
		}
		s.geometry(vertices);
		
		/* do Z-near clipping */
		Vec4 clip_vertices[4];
		Vec3 clip_barycenters[4];
		PointList clip_primitive = {0, clip_vertices, clip_barycenters};
		znear_clip({3, vertices, barycenters}, c.near, clip_primitive);
		
		/* do Z-far clipping */
		Vec4 primitive_vertices[5];
		Vec3 primitive_barycenters[5];
		PointList primitive = {0, primitive_vertices, primitive_barycenters};
		zfar_clip(clip_primitive, c.far, primitive);
		
		/* perspective division */
		Vec3 device_vertices[5];
		for (int i = 0; i < primitive.size; ++i) {
			device_vertices[i] = Vec3(
				primitive.vertices[i].x,
				primitive.vertices[i].y,
				primitive.vertices[i].z
			) / primitive.vertices[i].w;
		}
		
		/* viewport transform */
		int width_2 = viewport_w / 2;
		int height_2 = viewport_h / 2;
		for (int i = 0; i < primitive.size; ++i) {
			device_vertices[i].x = device_vertices[i].x * width_2 + width_2;
			device_vertices[i].y = -device_vertices[i].y * height_2 + height_2;
		}
		
		/* rasterization */
		if (canvas == nullptr) {
			return rasterize(primitive, device_vertices, zb);
		}
		rasterize(primitive, device_vertices, s, zb, canvas);
	}
}

void draw_instances(const Camera& c, Shader& s, const Instance* const* i, size_t size, Vec4* canvas) {
	size_t buffer_size = viewport_w * viewport_h;
	if (z_buffer.size() != buffer_size) {
		z_buffer.resize(buffer_size);
	}
	std::fill(z_buffer.begin(), z_buffer.end(), 1);
	auto& instances = i;
	for (int i = 0; i < size; ++i) {
		s.model = instances[i]->matrix_global;
		s.view = c.viewing;
		s.proj = c.projection;
		s.model_view = s.view * s.model;
		s.model_view_proj = s.proj * s.model_view;
		s.camera_pos = c.position;
		draw(c, s, *instances[i]->mesh, z_buffer.data(), canvas);
	}
}

}
