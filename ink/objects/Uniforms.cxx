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

#include "Uniforms.h"

namespace Ink {

size_t Uniforms::get_count() const {
	return unifroms.size();
}

std::string Uniforms::get_name(int i) const {
	return std::get<0>(unifroms[i]);
}

int Uniforms::get_type(int i) const {
	return std::get<1>(unifroms[i]);
}

int Uniforms::get_location(int i) const {
	return std::get<2>(unifroms[i]);
}

float* Uniforms::get_data() {
	return data.data();
}

const float* Uniforms::get_data() const {
	return data.data();
}

void Uniforms::set_i(const std::string& n, int v) {
	size_t size = data.size();
	unifroms.emplace_back(std::make_tuple(n, 0, size));
	data.resize(size + 1);
	std::copy_n(&v, 1, data.data() + size);
}

void Uniforms::set_u(const std::string& n, unsigned int v) {
	size_t size = data.size();
	unifroms.emplace_back(std::make_tuple(n, 1, size));
	data.resize(size + 1);
	std::copy_n(&v, 1, data.data() + size);
}

void Uniforms::set_f(const std::string& n, float v) {
	size_t size = data.size();
	unifroms.emplace_back(std::make_tuple(n, 2, size));
	data.resize(size + 1);
	std::copy_n(&v, 1, data.data() + size);
}

void Uniforms::set_v2(const std::string& n, const Vec2& v) {
	size_t size = data.size();
	unifroms.emplace_back(std::make_tuple(n, 3, size));
	data.resize(size + 2);
	std::copy_n(&v.x, 2, data.data() + size);
}

void Uniforms::set_v3(const std::string& n, const Vec3& v) {
	size_t size = data.size();
	unifroms.emplace_back(std::make_tuple(n, 4, size));
	data.resize(size + 3);
	std::copy_n(&v.x, 3, data.data() + size);
}

void Uniforms::set_v4(const std::string& n, const Vec4& v) {
	size_t size = data.size();
	unifroms.emplace_back(std::make_tuple(n, 5, size));
	data.resize(size + 4);
	std::copy_n(&v.x, 4, data.data() + size);
}

void Uniforms::set_m2(const std::string& n, const Mat2& v) {
	size_t size = data.size();
	unifroms.emplace_back(std::make_tuple(n, 6, size));
	data.resize(size + 4);
	std::copy_n(&v[0][0], 4, data.data() + size);
}

void Uniforms::set_m3(const std::string& n, const Mat3& v) {
	size_t size = data.size();
	unifroms.emplace_back(std::make_tuple(n, 7, size));
	data.resize(size + 9);
	std::copy_n(&v[0][0], 9, data.data() + size);
}

void Uniforms::set_m4(const std::string& n, const Mat4& v) {
	size_t size = data.size();
	unifroms.emplace_back(std::make_tuple(n, 8, size));
	data.resize(size + 16);
	std::copy_n(&v[0][0], 16, data.data() + size);
}

void Uniforms::clear() {
	unifroms.clear();
	data.clear();
}

}
