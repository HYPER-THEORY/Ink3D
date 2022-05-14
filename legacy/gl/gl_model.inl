/*
 * Copyright (C) 2021-2022 Hypertheory
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef GL_MODEL_INL
#define GL_MODEL_INL

#include "gl_model.hpp"
#include "gl_stdinc.hpp"
#include "gl_type.hpp"
#include "gl_error.hpp"

namespace gl {

inline int read_mtl(const std::string& f, gl::mesh& m,
					std::unordered_map<std::string, int>& map) {
	std::ifstream in;
	in.open(f, std::ifstream::in);
	if (!in) {
		error::set(ERROR_READ, "Failed to read from mtl");
		return -1;
	}
	std::string line;
	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream stream(line.c_str());
		std::string keyword;
		stream >> keyword;
		if (keyword == "newmtl") {
			std::string name;
			stream >> name;
			map.insert({name, map.size()});
		} else if (keyword == "Kd") {
			vec3 color;
			stream >> color.x >> color.y >> color.z;
			m.vertex_color.push_back(color);
		}
	}
	in.close();
	return 0;
}

inline int read_obj(const std::string& f, gl::mesh& m) {
	std::ifstream in;
	in.open(f, std::ifstream::in);
	if (!in) {
		error::set(ERROR_READ, "Failed to read from obj");
		return -1;
	}
	m.mode = MESH_INDEXED;
	int status = 0;
	std::unordered_map<std::string, int> vertex_color_map;
	int vertex_color_index = -1;
	std::string line;
	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream stream(line.c_str());
		std::string keyword;
		stream >> keyword;
		if (keyword == "v") {
			vec3 vertex;
			stream >> vertex.x >> vertex.y >> vertex.z;
			m.vertex.push_back(vertex);
		} else if (keyword == "vn") {
			vec3 normal;
			stream >> normal.x >> normal.y >> normal.z;
			m.normal.push_back(normal);
		} else if (keyword == "vt") {
			vec2 uv;
			stream >> uv.x >> uv.y;
			m.uv.push_back(uv);
		} else if (keyword == "f") {
			int index;
			for (int i = 0; i < 3; ++i) {
				stream >> index;
				m.face_vertex.push_back(index - 1);
				stream.get();
				if (std::isdigit(stream.peek())) {
					stream >> index;
					m.face_uv.push_back(index - 1);
				}
				stream.get();
				if (std::isdigit(stream.peek())) {
					stream >> index;
					m.face_normal.push_back(index - 1);
				}
			}
			if (vertex_color_index != -1) {
				for (int i = 0; i < 3; ++i) {
					m.face_vertex_color.push_back(vertex_color_index);
				}
			}
		} else if (keyword == "g") {
			/* TODO */
		} else if (keyword == "usemtl") {
			std::string name;
			stream >> name;
			if (vertex_color_map.find(name) == vertex_color_map.end()) {
				error::set(ERROR_SEEK, "Failed to find material name");
			} else {
				vertex_color_index = vertex_color_map[name];
			}
		} else if (keyword == "mtllib") {
			std::string file;
			stream >> file;
			status |= read_mtl(f.substr(0, f.rfind('/') + 1) + file, m, vertex_color_map);
		}
	}
	in.close();
	return status;
}

}

#endif
