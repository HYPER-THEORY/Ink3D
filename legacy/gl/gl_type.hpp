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

#ifndef GL_TYPE_HPP
#define GL_TYPE_HPP

#include "gl_stdinc.hpp"
#include "gl_math.hpp"

using vec2 = gl::vec2f;
using vec2d = gl::vec2d;
using vec3 = gl::vec3f;
using vec3d = gl::vec3d;
using vec4 = gl::vec4f;
using vec4d = gl::vec4d;
template<int r, int c> using mat = gl::matf<r, c>;
template<int r, int c> using matd = gl::matd<r, c>;

inline constexpr float eps = 1e-6;

namespace gl {

struct vertexobject {
	unsigned int id = 0;					/* read only */
	int length = 0;							/* read & write */
	std::vector<int> size;					/* read & write */
	std::vector<std::string> name;			/* read & write, optional */
	std::vector<unsigned int> buffer_id;	/* read only */
	std::vector<std::vector<float> > data;	/* read & write */
};

struct texture {
	unsigned int id = 0;					/* read only */
	int width = 0;							/* read & write */
	int height = 0;							/* read & write */
	int bytes = 0;							/* read & write */
	std::string name;						/* read & write, optional */
	std::vector<uint8_t> data;				/* read & write */
};

enum {
	MESH_DIRECT,
	MESH_INDEXED,
};

class mesh {
public:
	int mode = MESH_DIRECT;
	std::string name;
	std::vector<vec3> vertex;
	std::vector<vec3> normal;
	std::vector<vec3> binormal;
	std::vector<vec3> tangent;
	std::vector<vec2> uv;
	std::vector<vec3> vertex_color;
	std::vector<unsigned int> face_vertex;
	std::vector<unsigned int> face_normal;
	std::vector<unsigned int> face_binormal;
	std::vector<unsigned int> face_tangent;
	std::vector<unsigned int> face_uv;
	std::vector<unsigned int> face_vertex_color;
	
	void load(vertexobject& vo) const {
		bool has_normal = !normal.empty();
		bool has_binormal = !binormal.empty();
		bool has_tangent = !tangent.empty();
		bool has_uv = !uv.empty();
		bool has_vertex_color = !vertex_color.empty();
		vo.length = static_cast<int>(mode == MESH_DIRECT ? vertex.size() : face_vertex.size());
		vo.size = {3};
		vo.name = {"vertex"};
		if (has_normal) {
			vo.size.push_back(3);
			vo.name.push_back("normal");
		}
		if (has_binormal) {
			vo.size.push_back(3);
			vo.name.push_back("binormal");
		}
		if (has_tangent) {
			vo.size.push_back(3);
			vo.name.push_back("tangent");
		}
		if (has_uv) {
			vo.size.push_back(2);
			vo.name.push_back("uv");
		}
		if (has_vertex_color) {
			vo.size.push_back(3);
			vo.name.push_back("vertex_color");
		}
		vo.data.resize(vo.size.size());
		for (int i = 0; i < vo.data.size(); ++i) {
			vo.data[i].resize(vo.length * vo.size[i]);
		}
		auto data_iter = vo.data.begin();
		float* data_ptr = nullptr;
		if (mode == MESH_DIRECT) {
			data_ptr = data_iter->data();
			for (int i = 0; i < vo.length; ++i) {
				*(data_ptr++) = vertex[i].x;
				*(data_ptr++) = vertex[i].y;
				*(data_ptr++) = vertex[i].z;
			}
			++data_iter;
			if (has_normal) {
				data_ptr = data_iter->data();
				for (int i = 0; i < vo.length; ++i) {
					*(data_ptr++) = normal[i].x;
					*(data_ptr++) = normal[i].y;
					*(data_ptr++) = normal[i].z;
				}
				++data_iter;
			}
			if (has_binormal) {
				data_ptr = data_iter->data();
				for (int i = 0; i < vo.length; ++i) {
					*(data_ptr++) = binormal[i].x;
					*(data_ptr++) = binormal[i].y;
					*(data_ptr++) = binormal[i].z;
				}
				++data_iter;
			}
			if (has_tangent) {
				data_ptr = data_iter->data();
				for (int i = 0; i < vo.length; ++i) {
					*(data_ptr++) = tangent[i].x;
					*(data_ptr++) = tangent[i].y;
					*(data_ptr++) = tangent[i].z;
				}
				++data_iter;
			}
			if (has_uv) {
				data_ptr = data_iter->data();
				for (int i = 0; i < vo.length; ++i) {
					*(data_ptr++) = uv[i].x;
					*(data_ptr++) = uv[i].y;
				}
				++data_iter;
			}
			if (has_vertex_color) {
				data_ptr = data_iter->data();
				for (int i = 0; i < vo.length; ++i) {
					*(data_ptr++) = vertex_color[i].x;
					*(data_ptr++) = vertex_color[i].y;
					*(data_ptr++) = vertex_color[i].z;
				}
				++data_iter;
			}
		} else {
			data_ptr = data_iter->data();
			for (int i = 0; i < vo.length; ++i) {
				*(data_ptr++) = vertex[face_vertex[i]].x;
				*(data_ptr++) = vertex[face_vertex[i]].y;
				*(data_ptr++) = vertex[face_vertex[i]].z;
			}
			++data_iter;
			if (has_normal) {
				data_ptr = data_iter->data();
				for (int i = 0; i < vo.length; ++i) {
					*(data_ptr++) = normal[face_normal[i]].x;
					*(data_ptr++) = normal[face_normal[i]].y;
					*(data_ptr++) = normal[face_normal[i]].z;
				}
				++data_iter;
			}
			if (has_binormal) {
				data_ptr = data_iter->data();
				for (int i = 0; i < vo.length; ++i) {
					*(data_ptr++) = binormal[face_binormal[i]].x;
					*(data_ptr++) = binormal[face_binormal[i]].y;
					*(data_ptr++) = binormal[face_binormal[i]].z;
				}
				++data_iter;
			}
			if (has_tangent) {
				data_ptr = data_iter->data();
				for (int i = 0; i < vo.length; ++i) {
					*(data_ptr++) = tangent[face_tangent[i]].x;
					*(data_ptr++) = tangent[face_tangent[i]].y;
					*(data_ptr++) = tangent[face_tangent[i]].z;
				}
				++data_iter;
			}
			if (has_uv) {
				data_ptr = data_iter->data();
				for (int i = 0; i < vo.length; ++i) {
					*(data_ptr++) = uv[face_uv[i]].x;
					*(data_ptr++) = uv[face_uv[i]].y;
				}
				++data_iter;
			}
			if (has_vertex_color) {
				data_ptr = data_iter->data();
				for (int i = 0; i < vo.length; ++i) {
					*(data_ptr++) = vertex_color[face_vertex_color[i]].x;
					*(data_ptr++) = vertex_color[face_vertex_color[i]].y;
					*(data_ptr++) = vertex_color[face_vertex_color[i]].z;
				}
				++data_iter;
			}
		}
	}
};

class instance {
public:
	vec3 position;
	vec3 rotation;
	vec3 scale;
	void* userdata;
	vertexobject* object;
	std::vector<texture*> textures;
	
	instance() : position({}), rotation({}), scale({1, 1, 1}) {}
	
	instance(const vec3& p, const vec3& r, const vec3& s) :
		position(p), rotation(r), scale(s) {}
	
	instance(vertexobject* o) : object(o), position({}), rotation({}), scale({1, 1, 1}) {}
	
	instance(vertexobject* o, const vec3& p, const vec3& r, const vec3& s) :
		object(o), position(p), rotation(r), scale(s) {}
	
	instance(vertexobject* o, std::vector<texture*>& ts) :
		object(o), textures(ts), position({}), rotation({}), scale({1, 1, 1}) {}
	
	instance(vertexobject* o, std::vector<texture*>& ts,
			 const vec3& p, const vec3& r, const vec3& s) :
		object(o), textures(ts), position(p), rotation(r), scale(s) {}
};

class scene {
public:
	std::vector<instance*> instances;
	
	size_t instance_size() const {
		return instances.size();
	}
	
	instance* const* get_instances() const {
		return instances.data();
	}
	
	void add_instance(instance* i) {
		instances.push_back(i);
	}
};

}

#endif
