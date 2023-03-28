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

#include "ShaderCache.h"

#include "../core/File.h"

namespace Ink {

void ShaderCache::load_vert(const std::string& n, const char* s) {
	vert_shaders.insert_or_assign(n, s);
	resolve_includes(vert_shaders[n]);
}

void ShaderCache::load_vert(const std::string& n, const std::string& s) {
	vert_shaders.insert_or_assign(n, s);
	resolve_includes(vert_shaders[n]);
}

void ShaderCache::load_geom(const std::string& n, const char* s) {
	geom_shaders.insert_or_assign(n, s);
	resolve_includes(geom_shaders[n]);
}

void ShaderCache::load_geom(const std::string& n, const std::string& s) {
	geom_shaders.insert_or_assign(n, s);
	resolve_includes(geom_shaders[n]);
}

void ShaderCache::load_frag(const std::string& n, const char* s) {
	frag_shaders.insert_or_assign(n, s);
	resolve_includes(frag_shaders[n]);
}

void ShaderCache::load_frag(const std::string& n, const std::string& s) {
	frag_shaders.insert_or_assign(n, s);
	resolve_includes(frag_shaders[n]);
}

void ShaderCache::load_vert_file(const std::string& n, const std::string& s) {
	vert_shaders.insert_or_assign(n, File::read(s));
	resolve_includes(vert_shaders[n]);
}

void ShaderCache::load_geom_file(const std::string& n, const std::string& s) {
	geom_shaders.insert_or_assign(n, File::read(s));
	resolve_includes(geom_shaders[n]);
}

void ShaderCache::load_frag_file(const std::string& n, const std::string& s) {
	frag_shaders.insert_or_assign(n, File::read(s));
	resolve_includes(frag_shaders[n]);
}

bool ShaderCache::has_vert(const std::string& n) {
	return vert_shaders.count(n) != 0;
}

bool ShaderCache::has_geom(const std::string& n) {
	return geom_shaders.count(n) != 0;
}

bool ShaderCache::has_frag(const std::string& n) {
	return frag_shaders.count(n) != 0;
}

const Gpu::Shader* ShaderCache::fetch(const std::string& n) {
	/* set the name as cache key */
	std::string key = n;
	
	/* check whether the key exists */
	if (cache.count(key) != 0) return cache.at(key).get();
	
	/* insert key and shader to the cache */
	auto p = cache.insert({key, std::make_unique<Gpu::Shader>()});
	auto* shader = p.first->second.get();
	
	/* load vertex, geometry and fragment shaders */
	if (vert_shaders.count(n) != 0) {
		shader->load_vert(vert_shaders[n]);
	} else {
		Error::set("ShaderCache", "Vertex shader is missing");
	}
	if (geom_shaders.count(n) != 0) {
		shader->load_geom(geom_shaders[n]);
	}
	if (frag_shaders.count(n) != 0) {
		shader->load_frag(frag_shaders[n]);
	} else {
		Error::set("ShaderCache", "Fragment shader is missing");
	}
	
	/* compile shader */
	shader->compile();
	
	return shader; /* return the shader */
}

const Gpu::Shader* ShaderCache::fetch(const std::string& n, const Defines& d) {
	/* set the name and defines as cache key */
	std::string key = n + '\n' + d.get();
	
	/* check whether the key exists */
	if (cache.count(key) != 0) return cache.at(key).get();
	
	/* insert key and shader to the cache */
	auto p = cache.insert({key, std::make_unique<Gpu::Shader>()});
	auto* shader = p.first->second.get();
	
	/* load vertex, geometry and fragment shaders */
	if (vert_shaders.count(n) != 0) {
		shader->load_vert(vert_shaders[n]);
	} else {
		Error::set("ShaderCache", "Vertex shader is missing");
	}
	if (geom_shaders.count(n) != 0) {
		shader->load_geom(geom_shaders[n]);
	}
	if (frag_shaders.count(n) != 0) {
		shader->load_frag(frag_shaders[n]);
	} else {
		Error::set("ShaderCache", "Fragment shader is missing");
	}
	
	/* set defines and compile shader */
	shader->set_defines(d);
	shader->compile();
	
	return shader; /* return the shader */
}

void ShaderCache::clear_cache(const std::string& n) {
	cache.erase(n);
}

void ShaderCache::clear_cache(const std::string& n, const Defines& d) {
	cache.erase(n + '\n' + d.get());
}

void ShaderCache::clear_caches() {
	cache.clear();
}

std::string ShaderCache::get_include_path() {
	return include_path;
}

void ShaderCache::set_include_path(const std::string& p) {
	include_path = p;
}

void ShaderCache::resolve_includes(std::string& s) {
	size_t line_begin = 0;
	size_t line_end = -1;
	
	/* record include times to avoid circular include dependency */
	int include_times = 0;
	constexpr int max_include_times = 256;
	
	while (line_end != s.length()) {
		/* search the string of each line */
		line_begin = line_end + 1;
		line_end = s.find('\n', line_begin);
		line_end = line_end == -1 ? s.length() : line_end;
		size_t line_length = line_end - line_begin;
		std::string line = s.substr(line_begin, line_length);
		
		/* search for the include name */
		size_t char_1 = line.find_first_not_of(" \t");
		if (char_1 == -1 || line[char_1] != '#') {
			continue;
		}
		size_t char_2 = line.find_first_not_of(" \t", char_1 + 1);
		if (char_2 == -1 || line.substr(char_2, 7) != "include") {
			continue;
		}
		size_t char_3 = line.find_first_not_of(" \t", char_2 + 7);
		if (char_3 == -1 || line[char_3] != '<') {
			continue;
		}
		size_t char_4 = line.find('>', char_3 + 1);
		if (char_4 == -1) {
			Error::set("ShaderCache", "Invalid preprocessing directive");
			continue;
		}
		std::string include = line.substr(char_3 + 1, char_4 - char_3 - 1);
		
		/* read included file into content */
		std::string content = File::read(include_path + include + ".glsl");
		
		/* replace the line with content */
		s.replace(line_begin, line_end - line_begin, content);
		line_end = line_begin - 1;
		
		/* check if there is circular include dependency */
		if (include_times++ == max_include_times) {
			return Error::set("ShaderCache", "Circular include dependency");
		}
	}
}

std::string ShaderCache::include_path = "ink/shaders/include/";

std::unordered_map<std::string, std::string> ShaderCache::vert_shaders;
std::unordered_map<std::string, std::string> ShaderCache::geom_shaders;
std::unordered_map<std::string, std::string> ShaderCache::frag_shaders;

std::unordered_map<std::string, std::unique_ptr<Gpu::Shader> > ShaderCache::cache;

}
