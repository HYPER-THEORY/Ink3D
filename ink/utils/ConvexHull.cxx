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

#include "ConvexHull.h"

namespace Ink {

void ConvexHull::add_vertex(const Vec3& v) {
	vertices.emplace_back(v);
}

void ConvexHull::add_vertices(const std::initializer_list<Vec3>& v) {
	vertices.insert(vertices.end(), v);
}

size_t ConvexHull::get_vertex_count() const {
	return vertices.size();
}

Vec3 ConvexHull::get_vertex(int i) const {
	return vertices[i];
}

size_t ConvexHull::get_face_count() const {
	return faces.size();
}

std::array<int, 3> ConvexHull::get_face(int i) const {
	return faces[i];
}

void ConvexHull::compute() {
	insert_face(0, 1, 2);
	insert_face(2, 1, 0);
	size_t size = vertices.size();
	for (int i = 3; i < size; ++i) {
		std::unordered_set<long long> new_faces;
		auto face_iter = faces.begin();
		auto normal_iter = normals.begin();
		while (face_iter != faces.end()) {
			Vec3 dir = vertices[i] - vertices[(*face_iter)[0]];
			if (normal_iter->dot(dir) <= 0) {
				++face_iter;
				++normal_iter;
				continue;
			}
			for (int k = 0; k < 3; ++k) {
				long long u = (*face_iter)[k];
				long long v = (*face_iter)[(k + 1) % 3];
				long long uv = v << 32 | u;
				if (new_faces.count(uv) != 0) {
					new_faces.erase(uv);
				} else {
					new_faces.insert(u << 32 | v);
				}
			}
			face_iter = faces.erase(face_iter);
			normal_iter = normals.erase(normal_iter);
		}
		for (auto& f : new_faces) {
			insert_face(i, f >> 32, f & 0xFFFFFFFFll);
		}
	}
}

void ConvexHull::insert_face(int a, int b, int c) {
	faces.emplace_back(std::array<int, 3>({a, b, c}));
	normals.emplace_back((vertices[b] - vertices[a]).cross(vertices[c] - vertices[a]));
}

}
