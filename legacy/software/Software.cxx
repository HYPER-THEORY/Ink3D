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

#include <array>

namespace Ink::Legacy::Soft {

void clear(Image& b, float d) {
	float* buffer = reinterpret_cast<float*>(b.data.data());
	std::fill_n(buffer, b.width * b.height, d);
}

void clip_near_plane(const PointList& i, PointList& o) {
	for (int l = 0; l < i.size; ++l) {
		auto& vertex_1 = i.vertices[l];
		auto& vertex_2 = i.vertices[(l + 1) % i.size];
		
		/* vertices are both outside */
		if (vertex_1.z < -vertex_1.w && vertex_2.z < -vertex_2.w) {
			continue;
		}
		
		/* vertices are both inside */
		if (vertex_1.z > -vertex_1.w && vertex_2.z > -vertex_2.w) {
			o.vertices[o.size++] = vertex_2;
			continue;
		}
		
		/* traveling from inside to outside */
		float weight_1 = fabsf(vertex_1.z + vertex_1.w);
		float weight_2 = fabsf(vertex_2.z + vertex_2.w);
		float factor_1 = weight_2 / (weight_1 + weight_2);
		float factor_2 = weight_1 / (weight_1 + weight_2);
		o.vertices[o.size++] = vertex_1 * factor_1 + vertex_2 * factor_2;
		
		/* traveling from outside to inside */
		if (vertex_1.z < -vertex_1.w && vertex_2.z > -vertex_2.w) {
			o.vertices[o.size++] = vertex_2;
		}
	}
}

void rasterize(const State& s, const Vec3* v, int n, Image& b) {
	float min_x = fmaxf(0.f, s.viewport_x);
	float max_x = fminf(b.width, s.viewport_x + s.viewport_width);
	float min_y = fmaxf(0.f, s.viewport_y);
	float max_y = fminf(b.height, s.viewport_y + s.viewport_height);
	float* buffer = reinterpret_cast<float*>(b.data.data());
	for (int i = 2; i < n; ++i) {
		auto& vertex_1 = v[0];
		auto& vertex_2 = v[i - 1];
		auto& vertex_3 = v[i];
		DVec2 v1 = {vertex_3.x - vertex_1.x, vertex_3.y - vertex_1.y};
		DVec2 v2 = {vertex_2.x - vertex_1.x, vertex_2.y - vertex_1.y};
		DVec2 v3 = {};
		double v11 = v1.dot(v1);
		double v12 = v1.dot(v2);
		double v22 = v2.dot(v2);
		double inv = 1. / (v11 * v22 - v12 * v12);
		double v11_inv = v11 * inv;
		double v12_inv = v12 * inv;
		double v22_inv = v22 * inv;
		Vec3 vertex_l = vertex_1;
		Vec3 vertex_m = vertex_2;
		Vec3 vertex_u = vertex_3;
		if (vertex_l.y > vertex_m.y) std::swap(vertex_l, vertex_m);
		if (vertex_m.y > vertex_u.y) std::swap(vertex_m, vertex_u);
		if (vertex_l.y > vertex_m.y) std::swap(vertex_l, vertex_m);
		float weight_1 = vertex_l.x / (vertex_m.y - vertex_l.y);
		float weight_2 = vertex_m.x / (vertex_m.y - vertex_l.y);
		float weight_3 = vertex_m.x / (vertex_u.y - vertex_m.y);
		float weight_4 = vertex_u.x / (vertex_u.y - vertex_m.y);
		float weight_5 = vertex_l.x / (vertex_u.y - vertex_l.y);
		float weight_6 = vertex_u.x / (vertex_u.y - vertex_l.y);
		float lower = fmaxf(floorf(vertex_l.y + 0.5f) + 0.5f, min_y + 0.5f);
		float upper = fminf(floorf(vertex_u.y + 0.5f) - 0.5f, max_y + 0.5f);
		for (float y = lower; y <= upper; y += 1.f) {
			int offset_y = static_cast<int>(y) * b.width;
			float left = y < vertex_m.y ?
				(vertex_m.y - y) * weight_1 + (y - vertex_l.y) * weight_2 :
				(vertex_u.y - y) * weight_3 + (y - vertex_m.y) * weight_4;
			float right =
				(vertex_u.y - y) * weight_5 + (y - vertex_l.y) * weight_6;
			if (left > right) std::swap(left, right);
			left = fmaxf(floorf(left + 0.5f) + 0.5f, min_x + 0.5f);
			right = fminf(floorf(right + 0.5f) - 0.5f, max_x + 0.5f);
			for (float x = left; x <= right; x += 1.f) {
				v3.x = x - vertex_1.x;
				v3.y = y - vertex_1.y;
				double v13 = v1.dot(v3);
				double v23 = v2.dot(v3);
				double u = v22_inv * v13 - v12_inv * v23;
				double v = v11_inv * v23 - v12_inv * v13;
				float z = vertex_1.z * (1. - u - v) + vertex_2.z * v + vertex_3.z * u;
				float& buffer_z = buffer[offset_y + static_cast<int>(x)];
				if (z < buffer_z) buffer_z = z;
			}
		}
	}
}

void render(const State& s, const Instance& i, const Camera& c, Image& b) {
	Mesh* mesh = i.mesh;
	size_t length = mesh->vertex.size();
	
	/* prepare resources for rendering */
	Mat4 model_view_proj = c.projection * c.viewing * i.matrix_global;
	PointList primitives;
	PointList clipped;
	Vec3 device_coords[4];
	
	for (int i = 0; i < length; i += 3) {
		/* model-view-projection transform */
		primitives.size = 3;
		for (int j = 0; j < 3; ++j) {
			primitives.vertices[j] = model_view_proj * Vec4(mesh->vertex[i + j], 1);
		}
		
		/* clip near plane */
		clipped.size = 0;
		clip_near_plane(primitives, clipped);
		
		/* perspective division */
		int number = clipped.size;
		for (int i = 0; i < number; ++i) {
			clipped.vertices[i] /= clipped.vertices[i].w;
		}
		
		/* viewport transform */
		for (int i = 0; i < number; ++i) {
			device_coords[i].x = clipped.vertices[i].x * 0.5f + 0.5f;
			device_coords[i].y = clipped.vertices[i].y * 0.5f + 0.5f;
			device_coords[i].z = clipped.vertices[i].z * 0.5f + 0.5f;
			device_coords[i].x = device_coords[i].x * s.viewport_width + s.viewport_x;
			device_coords[i].y = device_coords[i].y * s.viewport_height + s.viewport_y;
		}
		
		/* rasterization */
		rasterize(s, device_coords, number, b);
	}
}

}
