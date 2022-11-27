/*
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

#include "Gpu.h"

namespace WR {

namespace Gpu {

void clear(bool c, bool d, bool s) {
	GLbitfield flag = 0;
	flag |= GL_COLOR_BUFFER_BIT * c;
	flag |= GL_DEPTH_BUFFER_BIT * d;
	flag |= GL_STENCIL_BUFFER_BIT * s;
	glClear(flag);
}

void clear_color(const Vec3& c) {
	glClearColor(c.x, c.y, c.z, 1);
}

void enable_depth_test() {
	glEnable(GL_DEPTH_TEST);
}

void disable_depth_test() {
	glDisable(GL_DEPTH_TEST);
}

void enable_stencil_test() {
	glEnable(GL_STENCIL_TEST);
}

void disable_stencil_test() {
	glDisable(GL_STENCIL_TEST);
}

void enable_wireframe() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void disable_wireframe() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void enable_cull_face() {
	glEnable(GL_CULL_FACE);
}

void disable_cull_face() {
	glDisable(GL_CULL_FACE);
}

void cull_face(int s) {
	if (s == FRONT_SIDE) {
		glCullFace(GL_FRONT);
	} else if (s == BACK_SIDE) {
		glCullFace(GL_BACK);
	} else if (s == DOUBLE_SIDE) {
		glCullFace(GL_FRONT_AND_BACK);
	}
}

void depth_func(int f) {
	glDepthFunc(GL_NEVER + f);
}

void enable_multisample() {
	glEnable(GL_MULTISAMPLE);
}

void disable_multisample() {
	glDisable(GL_MULTISAMPLE);
}

void set_viewport(const Viewport& v) {
	glViewport(v.x, v.y, v.width, v.height);
}

Viewport get_viewport() {
	Viewport view;
	glGetIntegerv(GL_VIEWPORT, &view.x);
	return view;
}

void print_error() {
	GLenum error = glGetError();
	while (error != GL_NO_ERROR) {
		std::cerr << "[OpenGL Error] " << error << ": ";
		if (error == GL_INVALID_ENUM) {
			std::cerr << "An unacceptable value is specified for an enumerated argument.\n";
		} else if (error == GL_INVALID_VALUE) {
			std::cerr << "A numeric argument is out of range.\n";
		} else if (error == GL_INVALID_OPERATION) {
			std::cerr << "The specified operation is not allowed in the current state.\n";
		} else if (error == GL_OUT_OF_MEMORY) {
			std::cerr << "There is not enough memory left to execute the function.\n";
		} else {
			std::cerr << "An unknown error has occurred.\n";
		}
		error = glGetError();
	}
}

Shader::Shader() {
	program = glCreateProgram();
}

Shader::~Shader() {
	glDeleteProgram(program);
}

void Shader::load(const char* vert, const char* frag, const char* geom) {
	std::string vert_string = {vert};
	std::string frag_string = {frag};
	if (!geom) return load(&vert_string, &frag_string, nullptr);
	std::string geom_string = {geom};
	load(&vert_string, &frag_string, &geom_string);
}

void Shader::load(const std::string* vert, const std::string* frag, const std::string* geom) {
	GLuint vertex_shader = add_shader(*vert, GL_VERTEX_SHADER);
	GLuint fragment_shader = add_shader(*frag, GL_FRAGMENT_SHADER);
	if (!geom) {
		glLinkProgram(program);
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
		return link_info();
	}
	GLuint geometry_shader = add_shader(*geom, GL_GEOMETRY_SHADER);
	glLinkProgram(program);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	glDeleteShader(geometry_shader);
	link_info();
}

int Shader::read(const char* vert, const char* frag, const char* geom) {
	int return_value = 0;
	std::string vertex_source;
	std::string fragment_source;
	return_value |= read_file(vert, vertex_source);
	return_value |= read_file(frag, fragment_source);
	if (!geom) {
		load(&vertex_source, &fragment_source);
		return return_value;
	}
	std::string geometry_source;
	return_value |= read_file(geom, geometry_source);
	load(&vertex_source, &fragment_source, &geometry_source);
	return return_value;
}

int Shader::read(const std::string* vert, const std::string* frag, const std::string* geom) {
	int return_value = 0;
	std::string vertex_source;
	std::string fragment_source;
	return_value |= read_file(*vert, vertex_source);
	return_value |= read_file(*frag, fragment_source);
	if (!geom) {
		load(&vertex_source, &fragment_source);
		return return_value;
	}
	std::string geometry_source;
	return_value |= read_file(*geom, geometry_source);
	load(&vertex_source, &fragment_source, &geometry_source);
	return return_value;
}

void Shader::use() const {
	glUseProgram(program);
}

void Shader::uniform_int(const std::string& n, int v) const {
	glUniform1i(glGetUniformLocation(program, n.c_str()), v);
}

void Shader::uniform_uint(const std::string& n, unsigned int v) const {
	glUniform1ui(glGetUniformLocation(program, n.c_str()), v);
}

void Shader::uniform_float(const std::string& n, float v) const {
	glUniform1f(glGetUniformLocation(program, n.c_str()), v);
}

void Shader::uniform_vec2(const std::string& n, const Vec2& v) const {
	glUniform2fv(glGetUniformLocation(program, n.c_str()), 1, &v.x);
}

void Shader::uniform_vec3(const std::string& n, const Vec3& v) const {
	glUniform3fv(glGetUniformLocation(program, n.c_str()), 1, &v.x);
}

void Shader::uniform_vec4(const std::string& n, const Vec4& v) const {
	glUniform4fv(glGetUniformLocation(program, n.c_str()), 1, &v.x);
}

void Shader::uniform_mat2(const std::string& n, const Mat<2, 2>& v) const {
	glUniformMatrix2fv(glGetUniformLocation(program, n.c_str()), 1, GL_TRUE, v[0]);
}

void Shader::uniform_mat3(const std::string& n, const Mat<3, 3>& v) const {
	glUniformMatrix3fv(glGetUniformLocation(program, n.c_str()), 1, GL_TRUE, v[0]);
}

void Shader::uniform_mat4(const std::string& n, const Mat<4, 4>& v) const {
	glUniformMatrix4fv(glGetUniformLocation(program, n.c_str()), 1, GL_TRUE, v[0]);
}

GLuint Shader::add_shader(const std::string& s, GLint t) const {
	GLuint new_shader = glCreateShader(t);
	std::string shader_string = s;
	resolve_include(shader_string);
	const char* shader_str = shader_string.c_str();
	glShaderSource(new_shader, 1, &shader_str, nullptr);
	glCompileShader(new_shader);
	compile_info(new_shader, t);
	glAttachShader(program, new_shader);
	return new_shader;
}

void Shader::compile_info(GLuint s, GLuint t) const {
	GLint success;
	glGetShaderiv(s, GL_COMPILE_STATUS, &success);
	if (success) return;
	if (t == GL_VERTEX_SHADER) {
		std::cerr << "[Vertex Shader Compile Error] ";
	} else if (t == GL_GEOMETRY_SHADER) {
		std::cerr << "[Geometry Shader Compile Error] ";
	} else if (t == GL_FRAGMENT_SHADER) {
		std::cerr << "[Fragment Shader Compile Error] ";
	}
	GLchar info[1024];
	glGetShaderInfoLog(s, 1024, nullptr, info);
	std::cerr << info;
}

void Shader::link_info() const {
	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success) return;
	std::cerr << "[Link Error] ";
	GLchar info[1024];
	glGetProgramInfoLog(program, 1024, nullptr, info);
	std::cerr << info;
}

void Shader::resolve_include(std::string& s) {
	int include_times = 0;
	size_t include_location = s.find("#include");
	while (include_location != -1) {
		/* search for include name */
		size_t include_start = s.find('<', include_location) + 1;
		size_t include_end = s.find('>', include_location);
		size_t include_finish = s.find('\n', include_location);
		std::string include = s.substr(include_start, include_end - include_start);
		/* read include content */
		std::string content;
		int success = read_file("shaders/inc/" + include + ".glsl", content);
		if (success == -1) {
			return static_cast<void>(set_error("[Shader Error] Included file does not exist"));
		}
		/* resolve #pragma once */
		size_t expand_location = content.find("#pragma once");
		if (expand_location != -1) {
			size_t expand_finish = content.find('\n', expand_location);
			std::string macro_name = include + "_GLSL";
			std::transform(include.begin(), include.end(), macro_name.begin(), ::toupper);
			content.replace(expand_location, expand_finish - expand_location,
							"#ifndef " + macro_name + "\n#define " + macro_name) += "\n#endif";
		}
		/* replace content */
		s.replace(include_location, include_finish - include_location, content);
		/* locate next include expression */
		include_location = s.find("#include", include_location);
		/* determine whether exist circular includes */
		if (++include_times == /* max include times */ 1024) {
			return static_cast<void>(set_error("[Shader Error] Circular includes"));
		}
	}
}

BufferObject::BufferObject() {
	glGenBuffers(1, &id);
}
   
BufferObject::~BufferObject() {
	glDeleteBuffers(1, &id);
}

VertexObject::VertexObject() {
	glGenVertexArrays(1, &id);
}
   
VertexObject::~VertexObject() {
	glDeleteVertexArrays(1, &id);
}

void VertexObject::preload(const Mesh& m, unsigned int g) {
	auto& vertex = m.vertex;
	auto& normal = m.normal;
	auto& binormal = m.binormal;
	auto& tangent = m.tangent;
	auto& uv = m.uv;
	bool has_normal = !normal.empty();
	bool has_binormal = !binormal.empty();
	bool has_tangent = !tangent.empty();
	bool has_uv = !uv.empty();
	auto& material_reference = m.material_groups[g];
	std::vector<std::vector<float> > data;
	int material_position = material_reference.position;
	length = material_reference.length;
	sizes = {3};
	names = {"vertex"};
	data.push_back(std::vector<float>(length * 3));
	std::copy_n(&vertex[material_position].x, length * 3, data.back().data());
	if (has_normal) {
		sizes.push_back(3);
		names.push_back("normal");
		data.push_back(std::vector<float>(length * 3));
		std::copy_n(&normal[material_position].x, length * 3, data.back().data());
	}
	if (has_binormal) {
		sizes.push_back(3);
		names.push_back("binormal");
		data.push_back(std::vector<float>(length * 3));
		std::copy_n(&binormal[material_position].x, length * 3, data.back().data());
	}
	if (has_tangent) {
		sizes.push_back(3);
		names.push_back("tangent");
		data.push_back(std::vector<float>(length * 3));
		std::copy_n(&tangent[material_position].x, length * 3, data.back().data());
	}
	if (has_uv) {
		sizes.push_back(2);
		names.push_back("uv");
		data.push_back(std::vector<float>(length * 2));
		std::copy_n(&uv[material_position].x, length * 2, data.back().data());
	}
	size_t data_size = sizes.size();
	buffers = std::make_unique<BufferObject[]>(data_size);
	glBindVertexArray(id);
	for (int i = 0; i < data_size; ++i) {
		glBindBuffer(GL_ARRAY_BUFFER, buffers[i].id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data[i].size(), data[i].data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, sizes[i], GL_FLOAT, GL_FALSE, sizeof(float) * sizes[i], nullptr);
	}
}

void VertexObject::attach(const Shader& s) const {
	size_t data_size = sizes.size();
	glBindVertexArray(id);
	for (int i = 0; i < data_size; ++i) {
		glBindBuffer(GL_ARRAY_BUFFER, buffers[i].id);
		GLint attrib = glGetAttribLocation(s.program, names[i].c_str());
		if (attrib == -1) continue;
		glEnableVertexAttribArray(attrib);
		glVertexAttribPointer(attrib, sizes[i], GL_FLOAT, GL_FALSE, sizeof(float) * sizes[i], nullptr);
	}
}

void VertexObject::draw() const {
	glBindVertexArray(id);
	glDrawArrays(GL_TRIANGLES, 0, length);
}

Texture::Texture() {
	glGenTextures(1, &id);
}

Texture::~Texture() {
	glDeleteTextures(1, &id);
}

void Texture::preload_1d(int w, int b, int s, int t) {
	GLint sized = gl_sized_format(b, s);
	GLenum base = gl_base_format(b);
	GLenum data = gl_data_type(t);
	glBindTexture(GL_TEXTURE_1D, id);
	glTexImage1D(GL_TEXTURE_1D, 0, sized, w, 0, base, data, nullptr);
	type = TEXTURE_1D;
}

void Texture::preload_2d(int w, int h, int b, int s, int t) {
	GLint sized = gl_sized_format(b, s);
	GLenum base = gl_base_format(b);
	GLenum data = gl_data_type(t);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, sized, w, h, 0, base, data, nullptr);
	type = TEXTURE_2D;
}

void Texture::preload_2d(const Image& i, int s, int t) {
	GLint sized = gl_sized_format(i.bytes - 1, s);
	GLenum base = gl_base_format(i.bytes - 1);
	GLenum data = gl_data_type(t);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, sized, i.width, i.height, 0, base, data, i.data.data());
	type = TEXTURE_2D;
}

void Texture::preload_3d(int w, int h, int d, int b, int s, int t) {
	GLint sized = gl_sized_format(b, s);
	GLenum base = gl_base_format(b);
	GLenum data = gl_data_type(t);
	glBindTexture(GL_TEXTURE_3D, id);
	glTexImage3D(GL_TEXTURE_3D, 0, sized, w, h, d, 0, base, data, nullptr);
	type = TEXTURE_3D;
}

void Texture::preload_cube(int w, int h, int b, int s, int t) {
	GLint sized = gl_sized_format(b, s);
	GLenum base = gl_base_format(b);
	GLenum data = gl_data_type(t);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	for (int i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, sized, w, h, 0, base, data, nullptr);
	}
	type = TEXTURE_CUBE;
}

void Texture::preload_cube(const Image* is, int s, int t) {
	GLint sized = gl_sized_format(is[0].bytes - 1, s);
	GLenum base = gl_base_format(is[0].bytes - 1);
	GLenum data = gl_data_type(t);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	for (int i = 0; i < 6; ++i) {
		GLenum target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
		glTexImage2D(target, 0, sized, is[i].width, is[i].height, 0, base, data, is[i].data.data());
	}
	type = TEXTURE_CUBE;
}

void Texture::preload_2d_ms(int w, int h, int b, int s, int samples) {
	GLint sized = gl_sized_format(b, s);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, sized, w, h, GL_TRUE);
	type = TEXTURE_2D_MS;
}

int Texture::get_type() const {
	return type;
}

void Texture::set_name(const std::string& n) {
	name = n;
}

std::string Texture::get_name() const {
	return name;
}

void Texture::set_parameters(int w, int mag, int min, int mip) {
	if (type == TEXTURE_2D_MS) {
		return static_cast<void>(set_error("[Texture Error] Operator is invalid"));
	}
	GLenum type = gl_texture_type(this->type);
	glBindTexture(type, id);
	if (w == TEXTURE_REPEAT) {
		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
	} else if (w == TEXTURE_MIRRORED_REPEAT) {
		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	} else if (w == TEXTURE_CLAMP_TO_EDGE) {
		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	} else if (w == TEXTURE_CLAMP_TO_BORDER) {
		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	}
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST + mag);
	if (mip == TEXTURE_NO_MIPMAP) {
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST + min);
	} else if (mip == TEXTURE_MIPMAP_NEAREST) {
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST + min);
		glGenerateMipmap(type);
	} else if (mip == TEXTURE_MIPMAP_LINEAR) {
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR + min);
		glGenerateMipmap(type);
	}
}

void Texture::set_border_color(const Vec4& c) {
	GLenum type = gl_texture_type(this->type);
	glTexParameterfv(type, GL_TEXTURE_BORDER_COLOR, &c.x);
}

int Texture::use(int i) const {
	GLenum type = gl_texture_type(this->type);
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(type, id);
	return i;
}

GLint Texture::gl_base_format(int b) {
	if (b < 0 || b >= 6) return set_error("[Texture Error] Illegal texture format");
	return gl_base_formats[b];
}

GLenum Texture::gl_sized_format(int b, int s) {
	if (b < 0 || b >= 6) return set_error("[Texture Error] Illegal texture format");
	if (s < 0 || s >= 11) return set_error("[Texture Error] Illegal texture format");
	if (b == IMAGE_D) return GL_DEPTH_COMPONENT24;
	if (b == IMAGE_DS) return GL_DEPTH24_STENCIL8;
	return gl_sized_formats[s][b];
}

GLenum Texture::gl_data_type(int t) {
	if (t < 0 || t >= 9) return set_error("[Texture Error] Illegal texture data type");
	return gl_data_types[t];
}

GLenum Texture::gl_texture_type(int t) {
	if (t < 0 || t >= 5) return set_error("[Texture Error] Illegal texture type");
	return gl_texture_types[t];
}

RenderBuffer::RenderBuffer() {
	glGenRenderbuffers(1, &id);
}

RenderBuffer::RenderBuffer(int w, int h, int b, int s) {
	glGenRenderbuffers(1, &id);
	preload(w, h, b, s);
}

RenderBuffer::~RenderBuffer() {
	glDeleteRenderbuffers(1, &id);
}

void RenderBuffer::preload(int w, int h, int b, int s) {
	GLint sized = Texture::gl_sized_format(b, s);
	glBindRenderbuffer(GL_RENDERBUFFER, id);
	glRenderbufferStorage(GL_RENDERBUFFER, sized, w, h);
}

FrameBuffer::FrameBuffer() {
	glGenFramebuffers(1, &id);
}

FrameBuffer::~FrameBuffer() {
	glDeleteFramebuffers(1, &id);
}

void FrameBuffer::bind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void FrameBuffer::unbind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::set_draw(const std::initializer_list<int>& b) {
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	std::vector<GLenum> buffers;
	for (auto& i : b) {
		buffers.push_back(GL_COLOR_ATTACHMENT0 + i);
	}
	glDrawBuffers(static_cast<int>(buffers.size()), buffers.data());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::disable_draw() const {
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::set_texture(const Texture& t, unsigned int i, bool d, bool ds) {
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	GLenum attachment = ds ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT;
	if (!d) attachment = GL_COLOR_ATTACHMENT0 + i;
	GLenum type = Texture::gl_texture_type(t.type);
	if (t.type == TEXTURE_2D || t.type == TEXTURE_2D_MS) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, type, t.id, 0);
	} else {
		set_error("[Framebuffer Error] Texture type is not supported");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::set_renderbuffer(const RenderBuffer& r, bool ds) {
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	GLenum attachment = ds ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT;
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, r.id);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}

}
