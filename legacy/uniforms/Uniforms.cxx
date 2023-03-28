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

#include "Uniforms.h"

#include "../../ink/core/Error.h"

namespace Ink::Legacy {

const void* Uniforms::get(const std::string& n) const {
	return vars.at(n);
}

void Uniforms::set(const std::string& n, const void* v) {
	vars.insert_or_assign(n, v);
}

bool Uniforms::has(const std::string& n) const {
	return vars.count(n) != 0;
}

void Uniforms::set_shader_uniforms(const Gpu::Shader& s) const {
	for (auto& [uniform_name, value] : vars) {
		/* get the suffix of variable */
		size_t length = uniform_name.size();
		char suffix_1 = uniform_name[length - 1];
		char suffix_2 = uniform_name[length - 2];
		
		/* get the name of variable */
		std::string name = uniform_name;
		name[length - (suffix_2 == '_' ? 2 : 3)] = '\0';
		
		/* convert variable into int data */
		if (suffix_1 == 'i') {
			s.set_uniform_i(name, *static_cast<const int*>(value));
		}
		
		/* convert variable into unsigned int data */
		else if (suffix_1 == 'u') {
			s.set_uniform_u(name, *static_cast<const unsigned int*>(value));
		}
		
		/* convert variable into float data */
		else if (suffix_1 == 'f') {
			s.set_uniform_f(name, *static_cast<const float*>(value));
		}
		
		/* convert variable into Vec2 data */
		else if (suffix_1 == '2' && suffix_2 == 'v') {
			s.set_uniform_v2(name, *static_cast<const Vec2*>(value));
		}
		
		/* convert variable into Vec3 data */
		else if (suffix_1 == '3' && suffix_2 == 'v') {
			s.set_uniform_v3(name, *static_cast<const Vec3*>(value));
		}
		
		/* convert variable into Vec4 data */
		else if (suffix_1 == '4' && suffix_2 == 'v') {
			s.set_uniform_v4(name, *static_cast<const Vec4*>(value));
		}
		
		/* convert variable into Mat2 data */
		else if (suffix_1 == '2' && suffix_2 == 'm') {
			s.set_uniform_m2(name, *static_cast<const Mat2*>(value));
		}
		
		/* convert variable into Mat3 data */
		else if (suffix_1 == '3' && suffix_2 == 'm') {
			s.set_uniform_m3(name, *static_cast<const Mat3*>(value));
		}
		
		/* convert variable into Mat4 data */
		else if (suffix_1 == '4' && suffix_2 == 'm') {
			s.set_uniform_m4(name, *static_cast<const Mat4*>(value));
		}
		
		/* unknown variable suffix */
		else {
			Error::set("Shader", "Unknown variable suffix");
		}
	}
}

}
