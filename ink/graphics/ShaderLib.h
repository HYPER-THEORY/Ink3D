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

#include "Gpu.h"

namespace Ink {

class ShaderLib {
public:
	/**
	 * Returns the shader with the specified name from shader cache.
	 *
	 * \param n name
	 */
	static const Gpu::Shader* fetch(const std::string& n);
	
	/**
	 * Returns the shader with the specified name and defines from shader cache.
	 *
	 * \param n name
	 * \param d defines
	 */
	static const Gpu::Shader* fetch(const std::string& n, const Defines& d);
	
	/**
	 * Clears all values from the shader cache.
	 */
	static void clear_caches();
	
	/**
	 * Sets the path to find shaders by names. Default is "ink/shaders/lib/".
	 *
	 * \param p library path
	 */
	static void set_library_path(const std::string& p);
	
private:
	static std::string library_path;
	
	static std::unordered_map<std::string, std::unique_ptr<Gpu::Shader> > cache;
};

}
