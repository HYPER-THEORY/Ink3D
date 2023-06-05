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

#pragma once

#include <string>
#include <unordered_map>

#include "../../ink/graphics/Gpu.h"

namespace Ink::Legacy {

class Uniforms {
public:
	/**
	 * Creates a new Uniforms object.
	 */
	explicit Uniforms() = default;
	
	/**
	 * Returns the value of the specified uniform variable. Use suffix in name
	 * to determine the variable type.
	 *
	 * \param n variable name
	 *
	 * ==============================
	 * |  Suffix  |  Variable Type  |
	 * |    _i    |      int        |
	 * |    _u    |      uint       |
	 * |    _f    |      float      |
	 * |    _v2   |      vec2       |
	 * |    _v3   |      vec3       |
	 * |    _v4   |      vec4       |
	 * |    _m2   |      mat2       |
	 * |    _m3   |      mat3       |
	 * |    _m4   |      mat4       |
	 * ==============================
	 */
	const void* get(const std::string& n) const;
	
	/**
	 * Sets the specified value for the specified uniform variable. These values
	 * will be passed to vertex, geometry and fragment shaders.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set(const std::string& n, const void* v);
	
	/**
	 * Returns true if there is a value for the specified uniform variable.
	 *
	 * \param n variable name
	 */
	bool has(const std::string& n) const;
	
	/**
	 * Sets the uniforms object to the specified shader. These values will be
	 * passed to vertex, geometry and fragment shaders.
	 *
	 * \param s shader
	 */
	void set_uniforms_to_shader(const Gpu::Shader& s) const;
	
private:
	std::unordered_map<std::string, const void*> vars;
};

}
