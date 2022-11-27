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

#include "ShaderLib.h"

#include <memory>

#include "../core/File.h"

namespace Ink {

const Gpu::Shader* ShaderLib::fetch(const std::string& n) {
	/* set the name as key */
	std::string key = n;
	
	/* check whether the key exists */
	if (cache.count(key) != 0) return cache.at(key).get();
	
	/* insert key and shader to the cache */
	auto p = cache.insert({key, std::make_unique<Gpu::Shader>()});
	auto* shader = p.first->second.get();
	
	/* load shader files to shader */
	std::string shader_file = library_path + n;
	shader->load_vert_file(shader_file + ".vert.glsl");
	shader->load_frag_file(shader_file + ".frag.glsl");
	
	/* compile shader */
	shader->compile();
	
	return shader; /* return the shader */
}

const Gpu::Shader* ShaderLib::fetch(const std::string& n, const Defines& d) {
	/* set the name and define string as key */
	std::string key = n + "\n" + d.get();
	
	/* check whether the key exists */
	if (cache.count(key) != 0) return cache.at(key).get();
	
	/* insert key and shader to the cache */
	auto p = cache.insert({key, std::make_unique<Gpu::Shader>()});
	auto* shader = p.first->second.get();
	
	/* load shader files to shader */
	std::string shader_file = library_path + n;
	shader->load_vert_file(shader_file + ".vert.glsl");
	shader->load_frag_file(shader_file + ".frag.glsl");
	
	/* set defines to shader and compile shader */
	shader->set_defines(d);
	shader->compile();
	
	return shader; /* return the shader */
}

void ShaderLib::clear_caches() {
	cache.clear();
}

void ShaderLib::set_library_path(const std::string& p) {
	library_path = p;
}

std::string ShaderLib::library_path = "ink/shaders/lib/";

std::unordered_map<std::string, std::unique_ptr<Gpu::Shader> > ShaderLib::cache;

}
