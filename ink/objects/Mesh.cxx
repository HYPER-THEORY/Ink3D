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

#include "Mesh.h"

#include "../core/Error.h"

#include <unordered_map>

namespace Ink {

Mesh::Mesh(const std::string& n) : name(n) {}

void Mesh::translate(float x, float y, float z) {
	for (auto& v : vertex) {
		v.x += x;
		v.y += y;
		v.z += z;
	}
}

void Mesh::translate(const Vec3& t) {
	for (auto& v : vertex) {
		v += t;
	}
}

void Mesh::rotate_x(float a) {
	for (auto& v : vertex) {
		float v_y = v.y;
		float v_z = v.z;
		v.y = cosf(a) * v_y - sinf(a) * v_z;
		v.z = sinf(a) * v_y + cosf(a) * v_z;
	}
	for (auto& n : normal) {
		float n_y = n.y;
		float n_z = n.z;
		n.y = cosf(a) * n_y - sinf(a) * n_z;
		n.z = sinf(a) * n_y + cosf(a) * n_z;
		n = n.normalize();
	}
	for (auto& t : tangent) {
		Vec3 t_xyz = {t.x, t.y, t.z};
		t_xyz.y = cosf(a) * t.y - sinf(a) * t.z;
		t_xyz.z = sinf(a) * t.y + cosf(a) * t.z;
		t = {t_xyz.normalize(), t.w};
	}
}

void Mesh::rotate_y(float a) {
	for (auto& v : vertex) {
		float v_x = v.y;
		float v_z = v.z;
		v.x = cosf(a) * v_x + sinf(a) * v_z;
		v.z = -sinf(a) * v_x + cosf(a) * v_z;
	}
	for (auto& n : normal) {
		float n_x = n.y;
		float n_z = n.z;
		n.x = cosf(a) * n_x + sinf(a) * n_z;
		n.z = -sinf(a) * n_x + cosf(a) * n_z;
		n = n.normalize();
	}
	for (auto& t : tangent) {
		Vec3 t_xyz = {t.x, t.y, t.z};
		t_xyz.x = cosf(a) * t.x + sinf(a) * t.z;
		t_xyz.z = -sinf(a) * t.x + cosf(a) * t.z;
		t = {t_xyz.normalize(), t.w};
	}
}

void Mesh::rotate_z(float a) {
	for (auto& v : vertex) {
		float v_x = v.x;
		float v_y = v.y;
		v.x = cosf(a) * v_x - sinf(a) * v_y;
		v.y = sinf(a) * v_x + cosf(a) * v_y;
	}
	for (auto& n : normal) {
		float n_x = n.x;
		float n_y = n.y;
		n.x = cosf(a) * n_x - sinf(a) * n_y;
		n.y = sinf(a) * n_x + cosf(a) * n_y;
		n = n.normalize();
	}
	for (auto& t : tangent) {
		Vec3 t_xyz = {t.x, t.y, t.z};
		t_xyz.x = cosf(a) * t.x - sinf(a) * t.y;
		t_xyz.y = sinf(a) * t.x + cosf(a) * t.y;
		t = {t_xyz.normalize(), t.w};
	}
}

void Mesh::rotate(const Euler& e) {
	Mat3 rotation_matrix = e.to_rotation_matrix();
	for (auto& v : vertex) {
		v = rotation_matrix * v;
	}
	for (auto& n : normal) {
		n = Vec3(rotation_matrix * n).normalize();
	}
	for (auto& t : tangent) {
		Vec3 t_xyz = rotation_matrix * Vec3(t.x, t.y, t.z);
		t = {t_xyz.normalize(), t.w};
	}
}

void Mesh::scale(float x, float y, float z) {
	for (auto& v : vertex) {
		v.x *= x;
		v.y *= y;
		v.z *= z;
	}
	for (auto& n : normal) {
		n.x /= x;
		n.y /= y;
		n.z /= z;
		n = n.normalize();
	}
	for (auto& t : tangent) {
		Vec3 t_xyz = {t.x * x, t.y * y, t.z * z};
		t = {t_xyz.normalize(), t.w};
	}
}

void Mesh::scale(const Vec3& s) {
	for (auto& v : vertex) {
		v *= s;
	}
	for (auto& n : normal) {
		n = (n / s).normalize();
	}
	for (auto& t : tangent) {
		Vec3 t_xyz = {t.x * s.x, t.y * s.y, t.z * s.z};
		t = {t_xyz.normalize(), t.w};
	}
}

void Mesh::normalize() {
	size_t size = normal.size();
	for (int i = 0; i < size; ++i) {
		normal[i] = normal[i].normalize();
	}
}

void Mesh::create_normals() {
	if (vertex.empty()) {
		return Error::set("Mesh", "Vertex information is missing");
	}
	size_t size = vertex.size();
	normal.resize(size);
	std::unordered_map<std::string, Vec3> normals;
	for (int i = 0; i < size; i += 3) {
		Vec3 v1 = vertex[i + 1] - vertex[i];
		Vec3 v2 = vertex[i + 2] - vertex[i];
		Vec3 face_normal = v1.cross(v2);
		for (int j = i; j < i + 3; ++j) {
			std::string hash = (vertex[j] + Vec3(0.005)).to_string(2);
			if (normals.count(hash) == 0) normals.insert({hash, {}});
			normals[hash] += face_normal.normalize();
		}
	}
	for (auto& [k, v] : normals) {
		v = v.normalize();
	}
	for (int i = 0; i < size; ++i) {
		std::string hash = (vertex[i] + Vec3(0.005)).to_string(2);
		normal[i] = normals[hash];
	}
}

void Mesh::create_tangents() {
	if (vertex.empty()) {
		return Error::set("Mesh", "Vertex information is missing");
	}
	if (uv.empty()) {
		return Error::set("Mesh", "UV information is missing");
	}
	if (normal.empty()) {
		return Error::set("Mesh", "Normal information is missing");
	}
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
