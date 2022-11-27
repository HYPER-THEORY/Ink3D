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

#include "Instance.h"

namespace Ink {

Instance::Instance(const std::string& n) : name(n) {}

Instance* Instance::create(const std::string& n) {
	return new Instance(n);
}

void Instance::destroy(const Instance* i) {
	delete i;
}

void Instance::add(Instance* i) {
	i->parent = this;
	children.emplace_back(i);
}

void Instance::add(const std::initializer_list<Instance*>& l) {
	for (auto& instance : l) {
		instance->parent = this;
	}
	children.insert(children.end(), l);
}

void Instance::remove(Instance* i) {
	i->parent = nullptr;
	size_t size = children.size();
	for (int c = 0; c < size; ++c) {
		if (children[c] == i) {
			children.erase(children.begin() + c);
		}
	}
}

void Instance::remove(const std::initializer_list<Instance*>& l) {
	size_t size = children.size();
	for (auto& instance : l) {
		instance->parent = nullptr;
		for (int c = 0; c < size; ++c) {
			if (children[c] == instance) {
				children.erase(children.begin() + c);
			}
		}
	}
}

void Instance::clear() {
	children.clear();
}

Instance* Instance::get_child(int i) const {
	return children[i];
}

Instance* Instance::get_child(const std::string& n) const {
	size_t size = children.size();
	for (int c = 0; c < size; ++c) {
		if (children[c]->name == n) return children[c];
	}
	return nullptr;
}

size_t Instance::get_child_count() const {
	return children.size();
}

Instance* Instance::get_parent() const {
	return parent;
}

void Instance::set_transform(const Vec3& p, const Euler& r, const Vec3& s) {
	position = p;
	rotation = r;
	scale = s;
}

void Instance::update_matrix_local() {
	matrix_local = transform();
}

void Instance::update_matrix_global() {
	matrix_global = transform_global();
}

Vec3 Instance::global_to_local(const Vec3& v) const {
	return static_cast<Vec3>(inverse_4x4(matrix_global) * Vec4(v, 1));
}

Vec3 Instance::local_to_global(const Vec3& v) const {
	return static_cast<Vec3>(matrix_global * Vec4(v, 1));
}

Mat4 Instance::transform() const {
	return transform(position, rotation, scale);
}

Mat4 Instance::transform_global() const {
	const Instance* instance = this;
	Mat4 matrix = Mat4::identity(4);
	while (instance->parent != nullptr) {
		matrix = instance->transform() * matrix;
		instance = instance->parent;
	}
	return matrix;
}

Mat4 Instance::transform(const Vec3& p, const Euler& r, const Vec3& s) {
	return
	Mat4{              /* translation scaling matrix */
		s.x       , 0         , 0         , p.x       ,
		0         , s.y       , 0         , p.y       ,
		0         , 0         , s.z       , p.z       ,
		0         , 0         , 0         , 1         ,
	} * r.to_rotation_matrix();
}

}
