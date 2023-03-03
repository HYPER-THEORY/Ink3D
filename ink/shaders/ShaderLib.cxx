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

#include "ShaderLib.h"

#include "ShaderCache.h"

#include "../core/File.h"

namespace Ink {

const Gpu::Shader* ShaderLib::fetch(const std::string& n) {
	std::string shader_file = library_path + n;
	if (!ShaderCache::has_vert(n)) {
		ShaderCache::load_vert(n, File::read(shader_file + ".vert.glsl"));
	}
	if (!ShaderCache::has_frag(n)) {
		ShaderCache::load_frag(n, File::read(shader_file + ".frag.glsl"));
	}
	return ShaderCache::fetch(n);
}

const Gpu::Shader* ShaderLib::fetch(const std::string& n, const Defines& d) {
	std::string shader_file = library_path + n;
	if (!ShaderCache::has_vert(n)) {
		ShaderCache::load_vert(n, File::read(shader_file + ".vert.glsl"));
	}
	if (!ShaderCache::has_frag(n)) {
		ShaderCache::load_frag(n, File::read(shader_file + ".frag.glsl"));
	}
	return ShaderCache::fetch(n, d);
}

std::string ShaderLib::get_library_path() {
	return library_path;
}

void ShaderLib::set_library_path(const std::string& p) {
	library_path = p;
}

std::string ShaderLib::library_path = "ink/shaders/library/";

}
