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

namespace Ink {

class Defines {
public:
	/**
	 * Creates a new Defines object.
	 */
	explicit Defines() = default;
	
	/**
	 * Returns the specific string containing all the define directives.
	 */
	std::string get() const;
	
	/**
	 * Sets the defines object. These values will be defined in vertex, geometry
	 * and fragment shaders.
	 *
	 * \param d defines
	 */
	void set(const Defines& d);
	
	/**
	 * Sets the define directive consisted of a single name. These values will
	 * be defined in vertex, geometry and fragment shaders.
	 *
	 * \param n macro name
	 */
	void set(const std::string& n);
	
	/**
	 * Sets the define directive consisted of a name and a value. These values
	 * will be defined in vertex, geometry and fragment shaders.
	 *
	 * \param n macro name
	 * \param v value
	 */
	void set(const std::string& n, const std::string& v);
	
	/**
	 * Sets the define directive consisted of a name and a value. These values
	 * will be defined in vertex, geometry and fragment shaders.
	 *
	 * \param n macro name
	 * \param v value
	 */
	void set_i(const std::string& n, int v);
	
	/**
	 * Sets the define directive consisted of a name and a value. These values
	 * will be defined in vertex, geometry and fragment shaders.
	 *
	 * \param n macro name
	 * \param v value
	 */
	void set_l(const std::string& n, long v);
	
	/**
	 * Sets the define directive consisted of a name and a value. These values
	 * will be defined in vertex, geometry and fragment shaders.
	 *
	 * \param n macro name
	 * \param v value
	 */
	void set_ll(const std::string& n, long long v);
	
	/**
	 * Sets the define directive consisted of a name if the flag is true. These
	 * values will be defined in vertex, geometry and fragment shaders.
	 *
	 * \param n macro name
	 * \param f flag
	 */
	void set_if(const std::string& n, bool f);
	
private:
	std::string defines;
};

}
