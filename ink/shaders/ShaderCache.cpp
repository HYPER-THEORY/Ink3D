/**
 * Copyright (C) 2021-2023 HYPERTHEORY
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

#include "../core/Error.h"
#include "../core/File.h"

#include <algorithm>

namespace ink {

void ShaderCache::load_vert(const std::string& n, const char* s) {
	std::string name = to_lower(n);
	vert_shaders.insert_or_assign(name, s);
	resolve_includes(vert_shaders[name]);
}

void ShaderCache::load_vert(const std::string& n, const std::string& s) {
	std::string name = to_lower(n);
	vert_shaders.insert_or_assign(name, s);
	resolve_includes(vert_shaders[name]);
}

void ShaderCache::load_geom(const std::string& n, const char* s) {
	std::string name = to_lower(n);
	geom_shaders.insert_or_assign(name, s);
	resolve_includes(geom_shaders[name]);
}

void ShaderCache::load_geom(const std::string& n, const std::string& s) {
	std::string name = to_lower(n);
	geom_shaders.insert_or_assign(name, s);
	resolve_includes(geom_shaders[name]);
}

void ShaderCache::load_frag(const std::string& n, const char* s) {
	std::string name = to_lower(n);
	frag_shaders.insert_or_assign(name, s);
	resolve_includes(frag_shaders[name]);
}

void ShaderCache::load_frag(const std::string& n, const std::string& s) {
	std::string name = to_lower(n);
	frag_shaders.insert_or_assign(name, s);
	resolve_includes(frag_shaders[name]);
}

void ShaderCache::load_include(const std::string& n, const char* s) {
	std::string name = to_lower(n);
	include_shaders.insert_or_assign(name, s);
	resolve_includes(include_shaders[name]);
}

void ShaderCache::load_include(const std::string& n, const std::string& s) {
	std::string name = to_lower(n);
	include_shaders.insert_or_assign(name, s);
	resolve_includes(include_shaders[name]);
}

void ShaderCache::load_vert_file(const std::string& n, const std::string& p) {
	std::string name = to_lower(n);
	vert_shaders.insert_or_assign(name, File::read(p));
	resolve_includes(vert_shaders[name]);
}

void ShaderCache::load_geom_file(const std::string& n, const std::string& p) {
	std::string name = to_lower(n);
	geom_shaders.insert_or_assign(name, File::read(p));
	resolve_includes(geom_shaders[name]);
}

void ShaderCache::load_frag_file(const std::string& n, const std::string& p) {
	std::string name = to_lower(n);
	frag_shaders.insert_or_assign(name, File::read(p));
	resolve_includes(frag_shaders[name]);
}

void ShaderCache::load_include_file(const std::string& n, const std::string& p) {
	std::string name = to_lower(n);
	include_shaders.insert_or_assign(name, File::read(p));
	resolve_includes(include_shaders[name]);
}

bool ShaderCache::has_vert(const std::string& n) {
	std::string name = to_lower(n);
	return vert_shaders.count(name) != 0;
}

bool ShaderCache::has_geom(const std::string& n) {
	std::string name = to_lower(n);
	return geom_shaders.count(name) != 0;
}

bool ShaderCache::has_frag(const std::string& n) {
	std::string name = to_lower(n);
	return frag_shaders.count(name) != 0;
}

bool ShaderCache::has_include(const std::string& n) {
	std::string name = to_lower(n);
	return include_shaders.count(name) != 0;
}

const gpu::Shader* ShaderCache::fetch(const std::string& n) {
	/* set the name as cache key */
	std::string name = to_lower(n);
	std::string key = name;
	
	/* check whether the key exists */
	if (cache.count(key) != 0) return cache.at(key).get();
	
	/* insert key and shader to the cache */
	auto p = cache.insert({key, std::make_unique<gpu::Shader>()});
	auto* shader = p.first->second.get();
	
	/* load vertex, geometry and fragment shaders */
	if (vert_shaders.count(name) != 0) {
		shader->load_vert(vert_shaders[name]);
	} else {
		Error::set("ShaderCache", "Vertex shader is missing");
	}
	if (geom_shaders.count(name) != 0) {
		shader->load_geom(geom_shaders[name]);
	}
	if (frag_shaders.count(name) != 0) {
		shader->load_frag(frag_shaders[name]);
	} else {
		Error::set("ShaderCache", "Fragment shader is missing");
	}
	
	/* compile shader */
	shader->compile();
	
	return shader; /* return the shader */
}

const gpu::Shader* ShaderCache::fetch(const std::string& n, const Defines& d) {
	/* set the name and defines as cache key */
	std::string name = to_lower(n);
	std::string key = name + '\n' + d.get();
	
	/* check whether the key exists */
	if (cache.count(key) != 0) return cache.at(key).get();
	
	/* insert key and shader to the cache */
	auto p = cache.insert({key, std::make_unique<gpu::Shader>()});
	auto* shader = p.first->second.get();
	
	/* load vertex, geometry and fragment shaders */
	if (vert_shaders.count(name) != 0) {
		shader->load_vert(vert_shaders[name]);
	} else {
		Error::set("ShaderCache", "Vertex shader is missing");
	}
	if (geom_shaders.count(name) != 0) {
		shader->load_geom(geom_shaders[name]);
	}
	if (frag_shaders.count(name) != 0) {
		shader->load_frag(frag_shaders[name]);
	} else {
		Error::set("ShaderCache", "Fragment shader is missing");
	}
	
	/* set defines and compile shader */
	shader->set_defines(d);
	shader->compile();
	
	return shader; /* return the shader */
}

void ShaderCache::clear_cache(const std::string& n) {
	std::string name = to_lower(n);
	cache.erase(name);
}

void ShaderCache::clear_cache(const std::string& n, const Defines& d) {
	std::string name = to_lower(n);
	cache.erase(name + '\n' + d.get());
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

std::string ShaderCache::to_lower(const std::string& s) {
	std::string lower = s;
	std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
	return lower;
}

void ShaderCache::resolve_includes(std::string& s) {
	size_t line_begin = 0;
	size_t line_end = -1;
	
	while (line_end != s.length()) {
		/* search every line of the shader content */
		line_begin = line_end + 1;
		line_end = s.find('\n', line_begin);
		line_end = line_end == -1 ? s.length() : line_end;
		std::string line = s.substr(line_begin, line_end - line_begin);
		
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
		std::string include_name = to_lower(line.substr(char_3 + 1, char_4 - char_3 - 1));
		
		/* read the included file into content */
		if (include_set.count(include_name) != 0) {
			s = std::string();
			return Error::set("ShaderCache", "Circular include dependency");
		}
		if (!has_include(include_name)) {
			include_set.insert(include_name);
			load_include_file(include_name, include_path + "/" + include_name + ".glsl");
			include_set.erase(include_name);
		}
		std::string content = include_shaders[include_name];
		
		/* replace the line with content */
		s.replace(line_begin, line_end - line_begin, content);
		line_end = line_begin - 1;
	}
}

std::string ShaderCache::include_path = "ink/shaders/include";

std::unordered_map<std::string, std::string> ShaderCache::vert_shaders;
std::unordered_map<std::string, std::string> ShaderCache::geom_shaders;
std::unordered_map<std::string, std::string> ShaderCache::frag_shaders;
std::unordered_map<std::string, std::string> ShaderCache::include_shaders;

std::unordered_set<std::string> ShaderCache::include_set;

std::unordered_map<std::string, std::unique_ptr<gpu::Shader>> ShaderCache::cache;

}
