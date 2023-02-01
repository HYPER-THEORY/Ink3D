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

#include "Software.h"

#ifndef LIKELY
#define	LIKELY(x)   __builtin_expect((x), 1)
#endif

#ifndef UNLIKELY
#define	UNLIKELY(x) __builtin_expect((x), 0)
#endif

namespace Ink::Soft {

constexpr float EPS = 1E-6;

void PointList::add_point(const Vec4& v, const Vec3& b) {
	vertices[size] = v;
	barycenters[size] = b;
	++size;
}

std::pair<int, int> get_viewport() {
	return {viewport_w, viewport_h};
}

void set_viewport(int w, int h) {
	viewport_w = w;
	viewport_h = h;
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
			o.add_point(vertex2, barycenter2);
			continue;
		}
		
		/* traveling from inside to outside */
		float weight1 = fabsf(vertex1.w - z);
		float weight2 = fabsf(vertex2.w - z);
		float inverse = 1 / (weight1 + weight2);
		if (vertex1.w > z && vertex2.w < z) {
			o.add_point((vertex1 * weight2 + vertex2 * weight1) * inverse,
						(barycenter1 * weight2 + barycenter2 * weight1) * inverse);
			continue;
		}
		
		/* traveling from outside to inside */
		o.add_point((vertex1 * weight2 + vertex2 * weight1) * inverse,
					(barycenter1 * weight2 + barycenter2 * weight1) * inverse);
		o.add_point(vertex2, barycenter2);
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
			o.add_point(vertex2, barycenter2);
			continue;
		}
		
		/* traveling from inside to outside */
		float weight1 = fabsf(vertex1.w - z);
		float weight2 = fabsf(vertex2.w - z);
		float inverse = 1 / (weight1 + weight2);
		if (vertex1.w > z && vertex2.w < z) {
			o.add_point((vertex1 * weight2 + vertex2 * weight1) * inverse,
						(barycenter1 * weight2 + barycenter2 * weight1) * inverse);
			continue;
		}
		
		/* traveling from outside to inside */
		o.add_point((vertex1 * weight2 + vertex2 * weight1) * inverse,
					(barycenter1 * weight2 + barycenter2 * weight1) * inverse);
		o.add_point(vertex2, barycenter2);
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

void render(const Mesh& m, const Camera& c, Shader& s, double* zb, Vec4* canvas) {
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
		int half_w = viewport_w / 2;
		int half_h = viewport_h / 2;
		for (int i = 0; i < primitive.size; ++i) {
			device_vertices[i].x = device_vertices[i].x * half_w + half_w;
			device_vertices[i].y = -device_vertices[i].y * half_h + half_h;
		}
		
		/* rasterization */
		if (canvas == nullptr) {
			return rasterize(primitive, device_vertices, zb);
		}
		rasterize(primitive, device_vertices, s, zb, canvas);
	}
}

void render_instance(const Instance* i, const Camera& c, Shader& s, double* zb, Vec4* canvas) {
	s.model = i->matrix_global;
	s.view = c.viewing;
	s.proj = c.projection;
	s.model_view = s.view * s.model;
	s.model_view_proj = s.proj * s.model_view;
	s.camera_pos = c.position;
	render(*i->mesh, c, s, zb, canvas);
}

}
