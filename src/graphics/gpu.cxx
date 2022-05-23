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

constexpr GLenum gl_stencil_operations[] = {
	GL_ZERO,											/* STENCIL_ZERO */
	GL_KEEP,											/* STENCIL_KEEP */
	GL_REPLACE,											/* STENCIL_REPLACE */
	GL_INCR,											/* STENCIL_INCR */
	GL_DECR,											/* STENCIL_DECR */
	GL_INCR_WRAP,										/* STENCIL_INCR_WRAP */
	GL_DECR_WRAP,										/* STENCIL_DECR_WRAP */
	GL_INVERT,											/* STENCIL_INVERT */
};

constexpr GLenum gl_texture_types[] = {
	GL_TEXTURE_1D,										/* TEXTURE_1D */
	GL_TEXTURE_2D,										/* TEXTURE_2D */
	GL_TEXTURE_3D,										/* TEXTURE_3D */
	GL_TEXTURE_CUBE_MAP,								/* TEXTURE_CUBE */
};

constexpr GLenum gl_texture_base_formats[] = {
	GL_RED,												/* IMAGE_R */
	GL_RG,												/* IMAGE_RG */
	GL_RGB,												/* IMAGE_RGB */
	GL_RGBA,											/* IMAGE_RGBA */
	GL_DEPTH_COMPONENT,									/* IMAGE_D */
	GL_DEPTH_STENCIL,									/* IMAGE_DS */
};

constexpr GLint gl_texture_sized_formats[][4] = {
	{GL_RED,	GL_RG, 		GL_RGB,		GL_RGBA},		/* IMAGE_NONE */
	{GL_R8,		GL_RG8,		GL_RGB8,	GL_RGBA8},		/* IMAGE_8 */
	{GL_R16, 	GL_RG16,	GL_RGB16,	GL_RGBA16},		/* IMAGE_16 */
	{GL_R16F,	GL_RG16F,	GL_RGB16F,	GL_RGBA16F},	/* IMAGE_16F */
	{GL_R32F,	GL_RG32F,	GL_RGB32F,	GL_RGBA32F},	/* IMAGE_32F */
	{GL_R8I,	GL_RG8I,	GL_RGB8I,	GL_RGBA8I},		/* IMAGE_8I */
	{GL_R16I,	GL_RG16I,	GL_RGB16I,	GL_RGBA16I},	/* IMAGE_16I */
	{GL_R32I,	GL_RG32I,	GL_RGB32I,	GL_RGBA32I},	/* IMAGE_32I */
	{GL_R8UI,	GL_RG8UI,	GL_RGB8UI,	GL_RGBA8UI},	/* IMAGE_8I */
	{GL_R16UI,	GL_RG16UI,	GL_RGB16UI,	GL_RGBA16UI},	/* IMAGE_16I */
	{GL_R32UI,	GL_RG32UI,	GL_RGB32UI,	GL_RGBA32UI},	/* IMAGE_32I */
};

constexpr GLenum gl_texture_data_types[] = {
	GL_UNSIGNED_BYTE,									/* IMAGE_UBYTE */
	GL_BYTE,											/* IMAGE_BYTE */
	GL_UNSIGNED_SHORT,									/* IMAGE_USHORT */
	GL_SHORT,											/* IMAGE_SHORT */
	GL_UNSIGNED_INT,									/* IMAGE_UINT */
	GL_INT,												/* IMAGE_INT */
	GL_HALF_FLOAT,										/* IMAGE_HALF_FLOAT */
	GL_FLOAT,											/* IMAGE_FLOAT */
	GL_UNSIGNED_INT_24_8,								/* IMAGE_UINT_24_8 */
};

constexpr GLint gl_texture_wrapping_modes[] = {
	GL_REPEAT,											/* TEXTURE_REPEAT */
	GL_MIRRORED_REPEAT,									/* TEXTURE_MIRRORED_REPEAT */
	GL_CLAMP_TO_EDGE,									/* TEXTURE_CLAMP_TO_EDGE */
	GL_CLAMP_TO_BORDER,									/* TEXTURE_CLAMP_TO_BORDER */
};

constexpr GLint gl_texture_filters[] = {
	GL_NEAREST,											/* TEXTURE_NEAREST */
	GL_LINEAR,											/* TEXTURE_LINEAR */
	GL_NEAREST_MIPMAP_NEAREST,							/* TEXTURE_NEAREST_MIPMAP_NEAREST */
	GL_LINEAR_MIPMAP_NEAREST,							/* TEXTURE_LINEAR_MIPMAP_NEAREST */
	GL_NEAREST_MIPMAP_LINEAR,							/* TEXTURE_NEAREST_MIPMAP_LINEAR */
	GL_LINEAR_MIPMAP_LINEAR,							/* TEXTURE_LINEAR_MIPMAP_LINEAR */
};

void clear(bool c, bool d, bool s) {
	GLbitfield flag = 0;
	flag |= GL_COLOR_BUFFER_BIT * c;
	flag |= GL_DEPTH_BUFFER_BIT * d;
	flag |= GL_STENCIL_BUFFER_BIT * s;
	glClear(flag);
}

vec4 get_clear_color() {
	vec4 clear_color;
	glGetFloatv(GL_COLOR_CLEAR_VALUE, &clear_color.x);
	return clear_color;
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

int get_depth_func() {
	int depth_func = 0;
	glGetIntegerv(GL_DEPTH_FUNC, &depth_func);
	return depth_func - GL_NEVER;
}

void set_depth_func(int f) {
	glDepthFunc(GL_NEVER + f);
}

void enable_stencil_test() {
	glEnable(GL_STENCIL_TEST);
}

void disable_stencil_test() {
	glDisable(GL_STENCIL_TEST);
}

int get_stencil_writemask() {
	int stencil_writemask = 0;
	glGetIntegerv(GL_STENCIL_WRITEMASK, &stencil_writemask);
	return stencil_writemask;
}

void set_stencil_writemask(int m) {
	glStencilMask(m);
}

int get_stencil_func() {
	int stencil_func = 0;
	glGetIntegerv(GL_STENCIL_FUNC, &stencil_func);
	return stencil_func - GL_NEVER;
}

int get_stencil_ref() {
	int stencil_ref = 0;
	glGetIntegerv(GL_STENCIL_REF, &stencil_ref);
	return stencil_ref;
}

int get_stencil_mask() {
	int stencil_mask = 0;
	glGetIntegerv(GL_STENCIL_VALUE_MASK, &stencil_mask);
	return stencil_mask;
}

void set_stencil_func(int f, int r, int m) {
	glStencilFunc(GL_NEVER + f, r, m);
}

void set_stencil_op(int f, int zf, int zp) {
	glStencilOp(gl_stencil_operations[f],
				gl_stencil_operations[zf],
				gl_stencil_operations[zp]);
}

void enable_scissor_test() {
	glEnable(GL_SCISSOR_TEST);
}

void disable_scissor_test() {
	glDisable(GL_SCISSOR_TEST);
}

rect get_scissor() {
	rect scissor_rect;
	glGetIntegerv(GL_SCISSOR_BOX, &scissor_rect.x);
	return scissor_rect;
}

void set_scissor(const rect& s) {
	glScissor(s.x, s.y, s.width, s.height);
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

int get_cull_face() {
	int cull_face = 0;
	glGetIntegerv(GL_CULL_FACE, &cull_face);
	if (cull_face == GL_FRONT) return FRONT_SIDE;
	if (cull_face == GL_BACK) return BACK_SIDE;
	return DOUBLE_SIDE;
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

rect get_viewport() {
	rect viewport_rect;
	glGetIntegerv(GL_VIEWPORT, &viewport_rect.x);
	return viewport_rect;
}

void set_viewport(const rect& v) {
	glViewport(v.x, v.y, v.width, v.height);
}

void enable_multisample() {
	glEnable(GL_MULTISAMPLE);
}

void disable_multisample() {
	glDisable(GL_MULTISAMPLE);
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

void active_material_depth(const material& m) {
	if (m.depth_test) {
		enable_depth_test();
		set_depth_func(m.depth_func);
	} else {
		disable_depth_test();
	}
}

void active_material_stencil(const material& m) {
	if (m.stencil_test) {
		enable_stencil_test();
		set_stencil_writemask(m.stencil_writemask);
		set_stencil_func(m.stencil_func, m.stencil_ref, m.stencil_mask);
		set_stencil_op(m.stencil_fail, m.stencil_zfail, m.stencil_zpass);
	} else {
		disable_stencil_test();
	}
}

void active_material_wireframe(const material& m) {
	if (m.wireframe) {
		enable_wireframe();
	} else {
		disable_wireframe();
	}
}

void active_material_side(const material& m) {
	if (m.side == FRONT_SIDE) {
		enable_cull_face();
		set_cull_face(BACK_SIDE);
	} else if (m.side == BACK_SIDE) {
		enable_cull_face();
		set_cull_face(FRONT_SIDE);
	} else if (m.side == DOUBLE_SIDE) {
		disable_cull_face();
	}
}

void active_material(const material& m) {
	active_material_depth(m);
	active_material_stencil(m);
	active_material_wireframe(m);
	active_material_side(m);
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

void shader::set_uniform_i(const std::string& n, int v) const {
	glUniform1i(glGetUniformLocation(program, n.c_str()), v);
}

void shader::set_uniform_u(const std::string& n, unsigned int v) const {
	glUniform1ui(glGetUniformLocation(program, n.c_str()), v);
}

void shader::set_uniform_f(const std::string& n, float v) const {
	glUniform1f(glGetUniformLocation(program, n.c_str()), v);
}

void shader::set_uniform_v2(const std::string& n, const vec2& v) const {
	glUniform2fv(glGetUniformLocation(program, n.c_str()), 1, &v.x);
}

void shader::set_uniform_v3(const std::string& n, const vec3& v) const {
	glUniform3fv(glGetUniformLocation(program, n.c_str()), 1, &v.x);
}

void shader::set_uniform_v4(const std::string& n, const vec4& v) const {
	glUniform4fv(glGetUniformLocation(program, n.c_str()), 1, &v.x);
}

void shader::set_uniform_m2(const std::string& n, const mat2& v) const {
	glUniformMatrix2fv(glGetUniformLocation(program, n.c_str()), 1, GL_TRUE, v[0]);
}

void shader::set_uniform_m3(const std::string& n, const mat3& v) const {
	glUniformMatrix3fv(glGetUniformLocation(program, n.c_str()), 1, GL_TRUE, v[0]);
}

void shader::set_uniform_m4(const std::string& n, const mat4& v) const {
	glUniformMatrix4fv(glGetUniformLocation(program, n.c_str()), 1, GL_TRUE, v[0]);
}

void shader::set_uniforms(const uniforms& u) const {
	auto uniforms_end = u.end();
	for (auto i = u.begin(); i != uniforms_end; ++i) {
		const std::string& name_with_suffix = i->first;
		const void* value = i->second;
		size_t last_index = name_with_suffix.rfind('_');
		if (last_index == -1) {
			set_error("[Shader Error] Variable suffix not found");
			continue;
		}
		std::string suffix = name_with_suffix.substr(last_index + 1);
		std::string name = name_with_suffix.substr(0, last_index);
		if (suffix == "i") {
			set_uniform_i(name, *static_cast<const int*>(value));
		} else if (suffix == "u") {
			set_uniform_u(name, *static_cast<const unsigned int*>(value));
		} else if (suffix == "f") {
			set_uniform_f(name, *static_cast<const float*>(value));
		} else if (suffix == "v2") {
			set_uniform_v2(name, *static_cast<const vec2*>(value));
		} else if (suffix == "v3") {
			set_uniform_v3(name, *static_cast<const vec3*>(value));
		} else if (suffix == "v4") {
			set_uniform_v4(name, *static_cast<const vec4*>(value));
		} else if (suffix == "m2") {
			set_uniform_m2(name, *static_cast<const mat2*>(value));
		} else if (suffix == "m3") {
			set_uniform_m3(name, *static_cast<const mat3*>(value));
		} else if (suffix == "m4") {
			set_uniform_m4(name, *static_cast<const mat4*>(value));
		} else {
			set_error("[Shader Error] Unknown variable suffix");
		}
	}
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
	GLenum type = gl_type(this->type);
	glBindTexture(type, id);
	glTexParameteri(type, GL_TEXTURE_WRAP_S, gl_wrapping_mode(m));
}

void texture::set_wrap_t(int m) const {
	GLenum type = gl_type(this->type);
	glBindTexture(type, id);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, gl_wrapping_mode(m));
}

void texture::set_wrap_r(int m) const {
	GLenum type = gl_type(this->type);
	glBindTexture(type, id);
	glTexParameteri(type, GL_TEXTURE_WRAP_R, gl_wrapping_mode(m));
}

void texture::set_wrap_all(int m) const {
	GLenum type = gl_type(this->type);
	glBindTexture(type, id);
	glTexParameteri(type, GL_TEXTURE_WRAP_S, gl_wrapping_mode(m));
	glTexParameteri(type, GL_TEXTURE_WRAP_T, gl_wrapping_mode(m));
	glTexParameteri(type, GL_TEXTURE_WRAP_R, gl_wrapping_mode(m));
}

void texture::set_filters(int mag, int min) const {
	GLenum type = gl_type(this->type);
	glBindTexture(type, id);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, gl_filter(mag));
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, gl_filter(min));
}

void texture::generate_mipmap() const {
	GLenum type = gl_type(this->type);
	glGenerateMipmap(type);
}

void texture::set_border_color(const vec4& c) const {
	GLenum type = gl_type(this->type);
	glTexParameterfv(type, GL_TEXTURE_BORDER_COLOR, &c.x);
}

int texture::active(int i) const {
	GLenum type = gl_type(this->type);
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(type, id);
	return i;
}

GLenum texture::gl_type(int t) {
	return gl_texture_types[t];
}

GLint texture::gl_base_format(int b) {
	return gl_texture_base_formats[b];
}

GLenum texture::gl_sized_format(int b, int s) {
	if (b == IMAGE_D) return GL_DEPTH_COMPONENT24;
	if (b == IMAGE_DS) return GL_DEPTH24_STENCIL8;
	return gl_texture_sized_formats[s][b];
}

GLenum texture::gl_data_type(int t) {
	return gl_texture_data_types[t];
}

GLint texture::gl_wrapping_mode(int m) {
	return gl_texture_wrapping_modes[m];
}

GLint texture::gl_filter(int f) {
	return gl_texture_filters[f];
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
	GLenum type = texture::gl_type(t.type);
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
	GLenum type = texture::gl_type(t.type);
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
