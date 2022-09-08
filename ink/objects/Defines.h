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

#pragma once

#include <string>
#include <initializer_list>

namespace Ink {

class Defines {
public:
	using DefinePair = std::pair<std::string, std::string>;
	
	/**
	 * Create a new Defines.
	 *
	 * \param l define list
	 */
	Defines(const std::initializer_list<DefinePair>& l = {});
	
	/**
	 * Returns the value of the define directives.
	 */
	std::string get() const;
	
	/**
	 * Sets the define directive.
	 *
	 * \param n define name
	 */
	void set(const std::string& n);
	
	/**
	 * Sets the define directive.
	 *
	 * \param n define name
	 * \param v define value
	 */
	void set(const std::string& n, const std::string& v);
	
private:
	std::string defines;
};

}
