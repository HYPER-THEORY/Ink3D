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

#include "gpu.h"

namespace gpu {

void clear(bool c, bool d, bool s) {
	GLbitfield flag = 0;
	flag |= GL_COLOR_BUFFER_BIT * c;
	flag |= GL_DEPTH_BUFFER_BIT * d;
	flag |= GL_STENCIL_BUFFER_BIT * s;
	glClear(flag);
}

void set_clear_color(const vec3& c, float a) {
	glClearColor(c.x, c.y, c.z, a);
}

void set_clear_color(const vec4& c) {
	glClearColor(c.x, c.y, c.z, c.w);
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

void set_cull_face(int s) {
	if (s == FRONT_SIDE) {
		glCullFace(GL_FRONT);
	} else if (s == BACK_SIDE) {
		glCullFace(GL_BACK);
	} else if (s == DOUBLE_SIDE) {
		glCullFace(GL_FRONT_AND_BACK);
	}
}

void set_depth_func(int f) {
	glDepthFunc(GL_NEVER + f);
}

void enable_multisample() {
	glEnable(GL_MULTISAMPLE);
}

void disable_multisample() {
	glDisable(GL_MULTISAMPLE);
}

viewport get_viewport() {
	viewport current_viewport;
	glGetIntegerv(GL_VIEWPORT, &current_viewport.x);
	return current_viewport;
}

void set_viewport(const viewport& v) {
	glViewport(v.x, v.y, v.width, v.height);
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

shader::shader() {
	program = glCreateProgram();
}

shader::~shader() {
	glDeleteProgram(program);
}

void shader::load(const char* vert, const char* frag, const char* geom) {
	std::string vert_string = {vert};
	std::string frag_string = {frag};
	if (geom == nullptr) return load(&vert_string, &frag_string, nullptr);
	std::string geom_string = {geom};
	load(&vert_string, &frag_string, &geom_string);
}

void shader::load(const std::string* vert, const std::string* frag, const std::string* geom) {
	GLuint vertex_shader = add_shader(*vert, GL_VERTEX_SHADER);
	GLuint fragment_shader = add_shader(*frag, GL_FRAGMENT_SHADER);
	if (geom == nullptr) {
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

int shader::read(const char* vert, const char* frag, const char* geom) {
	int return_value = 0;
	std::string vertex_source;
	std::string fragment_source;
	return_value |= read_file(vert, vertex_source);
	return_value |= read_file(frag, fragment_source);
	if (geom == nullptr) {
		load(&vertex_source, &fragment_source);
		return return_value;
	}
	std::string geometry_source;
	return_value |= read_file(geom, geometry_source);
	load(&vertex_source, &fragment_source, &geometry_source);
	return return_value;
}

int shader::read(const std::string* vert, const std::string* frag, const std::string* geom) {
	int return_value = 0;
	std::string vertex_source;
	std::string fragment_source;
	return_value |= read_file(*vert, vertex_source);
	return_value |= read_file(*frag, fragment_source);
	if (geom == nullptr) {
		load(&vertex_source, &fragment_source);
		return return_value;
	}
	std::string geometry_source;
	return_value |= read_file(*geom, geometry_source);
	load(&vertex_source, &fragment_source, &geometry_source);
	return return_value;
}

void shader::use_program() const {
	glUseProgram(program);
}

void shader::uniform_int(const std::string& n, int v) const {
	glUniform1i(glGetUniformLocation(program, n.c_str()), v);
}

void shader::uniform_uint(const std::string& n, unsigned int v) const {
	glUniform1ui(glGetUniformLocation(program, n.c_str()), v);
}

void shader::uniform_float(const std::string& n, float v) const {
	glUniform1f(glGetUniformLocation(program, n.c_str()), v);
}

void shader::uniform_vec2(const std::string& n, const vec2& v) const {
	glUniform2fv(glGetUniformLocation(program, n.c_str()), 1, &v.x);
}

void shader::uniform_vec3(const std::string& n, const vec3& v) const {
	glUniform3fv(glGetUniformLocation(program, n.c_str()), 1, &v.x);
}

void shader::uniform_vec4(const std::string& n, const vec4& v) const {
	glUniform4fv(glGetUniformLocation(program, n.c_str()), 1, &v.x);
}

void shader::uniform_mat2(const std::string& n, const mat2& v) const {
	glUniformMatrix2fv(glGetUniformLocation(program, n.c_str()), 1, GL_TRUE, v[0]);
}

void shader::uniform_mat3(const std::string& n, const mat3& v) const {
	glUniformMatrix3fv(glGetUniformLocation(program, n.c_str()), 1, GL_TRUE, v[0]);
}

void shader::uniform_mat4(const std::string& n, const mat4& v) const {
	glUniformMatrix4fv(glGetUniformLocation(program, n.c_str()), 1, GL_TRUE, v[0]);
}

GLuint shader::add_shader(const std::string& s, GLint t) const {
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

void shader::compile_info(GLuint s, GLuint t) const {
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

void shader::link_info() const {
	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success) return;
	std::cerr << "[Link Error] ";
	GLchar info[1024];
	glGetProgramInfoLog(program, 1024, nullptr, info);
	std::cerr << info;
}

void shader::resolve_include(std::string& s) {
	int include_times = 0;
	size_t line_start = 0;
	size_t line_end = s.find('\n');
	while (line_end != -1) {
		std::string line = s.substr(line_start, line_end - line_start);
		while (true) {
			/* search for include name */
			size_t char_1 = line.find_first_not_of(" \t");
			if (char_1 == -1 || line[char_1] != '#') break;
			size_t char_2 = line.find_first_not_of(" \t", char_1 + 1);
			if (char_2 == -1 || line.substr(char_2, 7) != "include") break;
			size_t char_3 = line.find_first_not_of(" \t", char_2 + 7);
			if (char_3 == -1 || line[char_3] != '<') break;
			size_t char_4 = line.find('>', char_3 + 1);
			if (char_4 == -1) break;
			std::string include = line.substr(char_3 + 1, char_4 - char_3 - 1);
			/* read included file into content */
			std::string content;
			int success = read_file(include_path + include + ".glsl", content);
			if (success == -1) {
				return static_cast<void>(set_error("[Shader Error] Included file does not exist"));
			}
			/* replace content */
			s.replace(line_start, line_end - line_start, content);
			line_end = line_start - 1;
			/* check if there is circular include */
			if (++include_times == /* max include times */ 1024) {
				return static_cast<void>(set_error("[Shader Error] Circular include"));
			}
			break;
		}
		line_start = line_end + 1;
		line_end = s.find('\n', line_start);
	}
}

void shader::set_include_path(const std::string& p) {
	include_path = p;
}

std::string shader::include_path = "shaders/inc/";

bufferobject::bufferobject() {
	glGenBuffers(1, &id);
}
   
bufferobject::~bufferobject() {
	glDeleteBuffers(1, &id);
}

vertexobject::vertexobject() {
	glGenVertexArrays(1, &id);
}
   
vertexobject::~vertexobject() {
	glDeleteVertexArrays(1, &id);
}

void vertexobject::preload(const mesh& m, unsigned int g) {
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
	std::vector<float> data;
	length = material_reference.length;
	/* has_vertex */ {
		sizes = {3};
		locations = {0, 3};
		names = {"vertex"};
	}
	if (has_normal) {
		sizes.push_back(3);
		locations.push_back(locations.back() + 3);
		names.push_back("normal");
	}
	if (has_binormal) {
		sizes.push_back(3);
		locations.push_back(locations.back() + 3);
		names.push_back("binormal");
	}
	if (has_tangent) {
		sizes.push_back(3);
		locations.push_back(locations.back() + 3);
		names.push_back("tangent");
	}
	if (has_uv) {
		sizes.push_back(2);
		locations.push_back(locations.back() + 2);
		names.push_back("uv");
	}
	int stride = locations.back();
	data.resize(length * stride);
	auto* data_ptr = data.data();
	for (int i = 0; i < length; ++i) {
		int position = material_reference.position + i;
		/* has_vertex */ {
			std::copy_n(&vertex[position].x, 3, data_ptr);
			data_ptr += 3;
		}
		if (has_normal) {
			std::copy_n(&normal[position].x, 3, data_ptr);
			data_ptr += 3;
		}
		if (has_binormal) {
			std::copy_n(&binormal[position].x, 3, data_ptr);
			data_ptr += 3;
		}
		if (has_tangent) {
			std::copy_n(&tangent[position].x, 3, data_ptr);
			data_ptr += 3;
		}
		if (has_uv) {
			std::copy_n(&uv[position].x, 2, data_ptr);
			data_ptr += 2;
		}
	}
	buffer = std::make_unique<bufferobject>();
	glBindVertexArray(id);
	glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), data.data(), GL_STATIC_DRAW);
	for (int i = 0; i < sizes.size(); ++i) {
		glVertexAttribPointer(i, sizes[i], GL_FLOAT, GL_FALSE, sizeof(float) * stride,
							  reinterpret_cast<void*>(sizeof(float) * locations[i]));
		glEnableVertexAttribArray(i);
	}
}

void vertexobject::attach(const shader& s) const {
	int stride = locations.back();
	glBindVertexArray(id);
	glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
	for (int i = 0; i < sizes.size(); ++i) {
		GLint attrib = glGetAttribLocation(s.program, names[i].c_str());
		if (attrib == -1) continue;
		glVertexAttribPointer(attrib, sizes[i], GL_FLOAT, GL_FALSE, sizeof(float) * stride,
							  reinterpret_cast<void*>(sizeof(float) * locations[i]));
		glEnableVertexAttribArray(attrib);
	}
}

void vertexobject::draw() const {
	glBindVertexArray(id);
	glDrawArrays(GL_TRIANGLES, 0, length);
}

texture::texture() {
	glGenTextures(1, &id);
}

texture::~texture() {
	glDeleteTextures(1, &id);
}

void texture::preload_1d(int w, int b, int s, int t) {
	GLint sized = gl_sized_format(b, s);
	GLenum base = gl_base_format(b);
	GLenum data = gl_data_type(t);
	glBindTexture(GL_TEXTURE_1D, id);
	glTexImage1D(GL_TEXTURE_1D, 0, sized, w, 0, base, data, nullptr);
	type = TEXTURE_1D;
}

void texture::preload_2d(int w, int h, int b, int s, int t) {
	GLint sized = gl_sized_format(b, s);
	GLenum base = gl_base_format(b);
	GLenum data = gl_data_type(t);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, sized, w, h, 0, base, data, nullptr);
	type = TEXTURE_2D;
}

void texture::preload_2d(const image& i, int s, int t) {
	GLint sized = gl_sized_format(i.bytes - 1, s);
	GLenum base = gl_base_format(i.bytes - 1);
	GLenum data = gl_data_type(t);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, sized, i.width, i.height, 0, base, data, i.data.data());
	type = TEXTURE_2D;
}

void texture::preload_3d(int w, int h, int d, int b, int s, int t) {
	GLint sized = gl_sized_format(b, s);
	GLenum base = gl_base_format(b);
	GLenum data = gl_data_type(t);
	glBindTexture(GL_TEXTURE_3D, id);
	glTexImage3D(GL_TEXTURE_3D, 0, sized, w, h, d, 0, base, data, nullptr);
	type = TEXTURE_3D;
}

void texture::preload_cube(int w, int h, int b, int s, int t) {
	GLint sized = gl_sized_format(b, s);
	GLenum base = gl_base_format(b);
	GLenum data = gl_data_type(t);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	for (int i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, sized, w, h, 0, base, data, nullptr);
	}
	type = TEXTURE_CUBE;
}

void texture::preload_cube(const image* is, int s, int t) {
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

int texture::get_type() const {
	return type;
}

void texture::set_wrap_s(int m) const {
	GLenum type = gl_texture_type(this->type);
	glBindTexture(type, id);
	glTexParameteri(type, GL_TEXTURE_WRAP_S, gl_wrapping_mode(m));
}

void texture::set_wrap_t(int m) const {
	GLenum type = gl_texture_type(this->type);
	glBindTexture(type, id);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, gl_wrapping_mode(m));
}

void texture::set_wrap_r(int m) const {
	GLenum type = gl_texture_type(this->type);
	glBindTexture(type, id);
	glTexParameteri(type, GL_TEXTURE_WRAP_R, gl_wrapping_mode(m));
}

void texture::set_wrap_all(int m) const {
	GLenum type = gl_texture_type(this->type);
	glBindTexture(type, id);
	glTexParameteri(type, GL_TEXTURE_WRAP_S, gl_wrapping_mode(m));
	glTexParameteri(type, GL_TEXTURE_WRAP_T, gl_wrapping_mode(m));
	glTexParameteri(type, GL_TEXTURE_WRAP_R, gl_wrapping_mode(m));
}

void texture::set_filters(int mag, int min) const {
	GLenum type = gl_texture_type(this->type);
	glBindTexture(type, id);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, gl_filter(mag));
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, gl_filter(min));
}

void texture::generate_mipmap() const {
	GLenum type = gl_texture_type(this->type);
	glGenerateMipmap(type);
}

void texture::set_border_color(const vec4& c) const {
	GLenum type = gl_texture_type(this->type);
	glTexParameterfv(type, GL_TEXTURE_BORDER_COLOR, &c.x);
}

int texture::active(int i) const {
	GLenum type = gl_texture_type(this->type);
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(type, id);
	return i;
}

GLenum texture::gl_texture_type(int t) {
	if (t < 0 || t >= 5) return set_error("[Texture Error] Illegal texture type");
	return gl_texture_types[t];
}

GLint texture::gl_base_format(int b) {
	if (b < 0 || b >= 6) return set_error("[Texture Error] Illegal texture format");
	return gl_base_formats[b];
}

GLenum texture::gl_sized_format(int b, int s) {
	if (b < 0 || b >= 6) return set_error("[Texture Error] Illegal texture format");
	if (s < 0 || s >= 11) return set_error("[Texture Error] Illegal texture format");
	if (b == IMAGE_D) return GL_DEPTH_COMPONENT24;
	if (b == IMAGE_DS) return GL_DEPTH24_STENCIL8;
	return gl_sized_formats[s][b];
}

GLenum texture::gl_data_type(int t) {
	if (t < 0 || t >= 9) return set_error("[Texture Error] Illegal texture data type");
	return gl_data_types[t];
}

GLint texture::gl_wrapping_mode(int m) {
	if (m < 0 || m >= 4) return set_error("[Texture Error] Illegal wrapping mode");
	return gl_wrapping_modes[m];
}

GLint texture::gl_filter(int f) {
	if (f < 0 || f >= 6) return set_error("[Texture Error] Illegal filter");
	return gl_filters[f];
}

renderbuffer::renderbuffer() {
	glGenRenderbuffers(1, &id);
}

renderbuffer::renderbuffer(int w, int h, int b, int s) {
	glGenRenderbuffers(1, &id);
	preload(w, h, b, s);
}

renderbuffer::~renderbuffer() {
	glDeleteRenderbuffers(1, &id);
}

void renderbuffer::preload(int w, int h, int b, int s) const {
	GLint sized = texture::gl_sized_format(b, s);
	glBindRenderbuffer(GL_RENDERBUFFER, id);
	glRenderbufferStorage(GL_RENDERBUFFER, sized, w, h);
}

framebuffer::framebuffer() {
	glGenFramebuffers(1, &id);
}

framebuffer::~framebuffer() {
	glDeleteFramebuffers(1, &id);
}

void framebuffer::active() const {
	glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void framebuffer::default_active() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void framebuffer::draw_attachments(const std::initializer_list<int>& l) const {
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	std::vector<GLenum> buffers;
	for (auto& i : l) {
		buffers.push_back(GL_COLOR_ATTACHMENT0 + i);
	}
	glDrawBuffers(static_cast<int>(buffers.size()), buffers.data());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void framebuffer::disable_draw() const {
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void framebuffer::set_attachment(const texture& t, unsigned int i) const {
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	GLenum attachment = GL_COLOR_ATTACHMENT0 + i;
	GLenum type = texture::gl_texture_type(t.type);
	if (t.type == TEXTURE_2D) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, type, t.id, 0);
	} else {
		set_error("[Framebuffer Error] Texture type is not supported");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void framebuffer::set_attachment(const renderbuffer& r, unsigned int i) const {
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	GLenum attachment = GL_COLOR_ATTACHMENT0 + i;
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, r.id);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void framebuffer::set_depth_attachment(const texture& t, bool ds) const {
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	GLenum attachment = ds ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT;
	GLenum type = texture::gl_texture_type(t.type);
	if (t.type == TEXTURE_2D) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, type, t.id, 0);
	} else {
		set_error("[Framebuffer Error] Texture type is not supported");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void framebuffer::set_depth_attachment(const renderbuffer& r, bool ds) const {
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	GLenum attachment = ds ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT;
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, r.id);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}
