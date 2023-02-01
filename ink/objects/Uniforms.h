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
#include <vector>

#include "../math/Vector.h"
#include "../math/Matrix.h"

namespace Ink {

class Uniforms {
public:
	/**
	 * Creates a new Uniforms object.
	 */
	explicit Uniforms() = default;
	
	/**
	 * Returns the number of uniform variables.
	 */
	size_t count() const;
	
	/**
	 * Returns the name of unifrom variable at the specified index.
	 *
	 * \param i the index of uniform
	 */
	std::string get_name(int i) const;
	
	/**
	 * Returns the type of unifrom variable at the specified index.
	 *
	 * \param i the index of uniform
	 */
	int get_type(int i) const;
	
	/**
	 * Returns the location of unifrom variable at the specified index.
	 *
	 * \param i the index of uniform
	 */
	int get_location(int i) const;
	
	/**
	 * Returns the data from the uniform data block.
	 */
	const float* get_data() const;
	
	/**
	 * Sets the specified value for the specified uniform variable. The value
	 * will be passed to vertex, geometry and fragment shaders.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_i(const std::string& n, int v);
	
	/**
	 * Sets the specified value for the specified uniform variable. The value
	 * will be passed to vertex, geometry and fragment shaders.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_u(const std::string& n, unsigned int v);
	
	/**
	 * Sets the specified value for the specified uniform variable. The value
	 * will be passed to vertex, geometry and fragment shaders.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_f(const std::string& n, float v);
	
	/**
	 * Sets the specified value for the specified uniform variable. The value
	 * will be passed to vertex, geometry and fragment shaders.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_v2(const std::string& n, const Vec2& v);
	
	/**
	 * Sets the specified value for the specified uniform variable. The value
	 * will be passed to vertex, geometry and fragment shaders.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_v3(const std::string& n, const Vec3& v);
	
	/**
	 * Sets the specified value for the specified uniform variable. The value
	 * will be passed to vertex, geometry and fragment shaders.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_v4(const std::string& n, const Vec4& v);
	
	/**
	 * Sets the specified value for the specified uniform variable. The value
	 * will be passed to vertex, geometry and fragment shaders.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_m2(const std::string& n, const Mat2& v);
	
	/**
	 * Sets the specified value for the specified uniform variable. The value
	 * will be passed to vertex, geometry and fragment shaders.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_m3(const std::string& n, const Mat3& v);
	
	/**
	 * Sets the specified value for the specified uniform variable. The value
	 * will be passed to vertex, geometry and fragment shaders.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_m4(const std::string& n, const Mat4& v);
	
private:
	std::vector<float> data;
	
	std::vector<std::tuple<std::string, int, int> > unifroms;
};

}
