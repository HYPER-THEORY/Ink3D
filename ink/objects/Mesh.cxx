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

#include "Mesh.h"

#include <unordered_map>

namespace Ink {

Mesh::Mesh(const std::string& n) : name(n) {}

void Mesh::normalize() {
	size_t size = normal.size();
	for (int i = 0; i < size; ++i) {
		normal[i] = normal[i].normalize();
	}
}

void Mesh::create_normals() {
	size_t size = vertex.size();
	normal.resize(size);
	std::unordered_map<std::string, Vec3> normals;
	for (int i = 0; i < size; i += 3) {
		Vec3 v1 = vertex[i + 1] - vertex[i];
		Vec3 v2 = vertex[i + 2] - vertex[i];
		Vec3 face_normal = v1.cross(v2);
		if (face_normal.magnitude() < 1E-4) continue;
		for (int j = i; j < i + 3; ++j) {
			std::string hash = (vertex[j] + Vec3(.005)).to_string(2);
			if (normals.count(hash) == 0) normals.insert({hash, {}});
			normals[hash] += face_normal.normalize();
		}
	}
	for (auto& [k, v] : normals) {
		v = v.normalize();
	}
	for (int i = 0; i < size; ++i) {
		std::string hash = (vertex[i] + Vec3(.005)).to_string(2);
		normal[i] = normals[hash];
	}
}

void Mesh::create_tangents() {
	size_t size = vertex.size();
	tangent.resize(size);
	for (int i = 0; i < size; i += 3) {
		Vec3 v1 = vertex[i + 1] - vertex[i];
		Vec3 v2 = vertex[i + 2] - vertex[i];
		Vec2 uv1 = uv[i + 1] - uv[i];
		Vec2 uv2 = uv[i + 2] - uv[i];
		float r = 1 / (uv1.x * uv2.y - uv2.x * uv1.y);
		Vec3 t = (v1 * uv2.y - v2 * uv1.y) * r;
		Vec3 b = (v2 * uv1.x - v1 * uv2.x) * r;
		for (int j = i; j < i + 3; ++j) {
			Vec3& n = normal[j];
			Vec3 ortho_t = (t - n * n.dot(t)).normalize();
			float m = n.cross(ortho_t).dot(b) < 0 ? -1 : 1;
			tangent[j] = Vec4(ortho_t, m);
		}
	}
}

}
