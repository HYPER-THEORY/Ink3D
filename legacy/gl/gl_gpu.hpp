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

/*
 *	---- pipeline --------------------------------------------------------------
 *
 *	preload_vertexobject(s, o)
 *	preload_texture(t)
 *	shader::load(vs, fs)				|	shader::read(vs, fs)
 *
 *	shader::use()						|	draw_instances(c, s, is, is_size)
 *	shader::uniform...(n, v)			|		or draw_scene(c, s, scene_info)
 *	load_texture(s, t, i)				|
 *	draw_object(vo)						|
 *
 *	unload_vertexobject(vo)
 *	unload_texture(t)
 *	shader::unload()
 *
 *	---- cubemap ---------------------------------------------------------------
 *
 *	write texture into cubemap[0 - 6]
 *	cubemap::custom_shader.load(vs, fs)	|	use default shader
 *	cubemap::preload()
 *
 *	cubemap::draw(c)
 *
 *	cubemap::unload()
 *
 *	---- canvas ----------------------------------------------------------------
 *
 *	write texture into canvas::surface
 *	canvas::custom_shader::load(vs, fs)	|	use default shader
 *	canvas::preload()
 *
 *	canvas::draw()
 *
 *	canvas::unload()
 *
 *	---- framebuffer -----------------------------------------------------------
 *
 *	framebuffer::custom_shader::load()	|	use default shader
 *	framebuffer::preload(s, b)
 *
 *	framebuffer::bind()
 *	framebuffer::clear()
 *	draw_instances(c, s, is, is_size)	|	draw(...)
 *	framebuffer::unbind()
 *	framebuffer::draw()
 *
 *	framebuffer::unload()
 */

#ifndef GL_GPU_HPP
#define GL_GPU_HPP

#include "gl_stdinc.hpp"
#include "gl_file.hpp"
#include "gl_type.hpp"
#include "gl_camera.hpp"
#include "gl_shader.hpp"

namespace gl::gpu {

/* texture filter type */
enum {
	TEXTURE_NEAREST = 0,
	TEXTURE_LINEAR = 1,
};

/* texture mipmap type */
enum {
	TEXTURE_NO_MIPMAP,
	TEXTURE_MIPMAP_NEAREST,
	TEXTURE_MIPMAP_LINEAR,
};

/* texture wrapping type */
enum {
	TEXTURE_REPEAT,
	TEXTURE_MIRRORED_REPEAT,
	TEXTURE_CLAMP_TO_EDGE,
	TEXTURE_CLAMP_TO_BORDER,
};

/* texture internal format */
enum {
	TEXTURE_8 = 0,
	TEXTURE_16 = 1,
	TEXTURE_16F = 2,
	TEXTURE_32F = 3,
	TEXTURE_8I = 4,
	TEXTURE_16I = 5,
	TEXTURE_32I = 6,
	TEXTURE_8UI = 7,
	TEXTURE_16UI = 8,
	TEXTURE_32UI = 9,
};

class shader {
public:
	shader() = default;
	
	bool loaded() const {
		return program != 0;
	}
	
	void unload() const {
		glDeleteProgram(program);
	}
	
	void use() const {
		glUseProgram(program);
	}
	
	/**
	 * \param vs vertex shader
	 * \param fs fragment shader
	 * \param gs geometry shader
	 */
	void load(const char* vs, const char* fs, const char* gs = nullptr) {
		if (program == 0) program = glCreateProgram();
		GLuint vertex_shader = add_shader(vs, GL_VERTEX_SHADER);
		GLuint fragment_shader = add_shader(fs, GL_FRAGMENT_SHADER);
		if (gs) {
			GLuint geometry_shader = add_shader(gs, GL_GEOMETRY_SHADER);
			glLinkProgram(program);
			glDeleteShader(vertex_shader);
			glDeleteShader(fragment_shader);
			glDeleteShader(geometry_shader);
		} else {
			glLinkProgram(program);
			glDeleteShader(vertex_shader);
			glDeleteShader(fragment_shader);
		}
		link_info();
	}
	
	/**
	 * \param vs vertex shader
	 * \param fs fragment shader
	 * \param gs geometry shader
	 */
	void load(const std::string* vs, const std::string* fs, const std::string* gs = nullptr) {
		load(vs->c_str(), fs->c_str(), gs ? gs->c_str() : nullptr);
	}
	
	/**
	 * \param vs vertex shader file
	 * \param fs fragment shader file
	 * \param gs geometry shader file
	 */
	int read(const std::string* vs, const std::string* fs, const std::string* gs = nullptr) {
		int status = 0;
		std::string vertex_source;
		std::string fragment_source;
		status |= read_file(*vs, vertex_source);
		status |= read_file(*fs, fragment_source);
		if (gs) {
			std::string geometry_source;
			read_file(*gs, geometry_source);
			load(vertex_source.c_str(), fragment_source.c_str(), geometry_source.c_str());
		} else {
			load(vertex_source.c_str(), fragment_source.c_str());
		}
		return status;
	}
	
	/**
	 * \param n variable name
	 * \param v value
	 */
	void uniform_int(const std::string& n, int v) const {
		glUniform1i(glGetUniformLocation(program, n.c_str()), v);
	}
	
	/**
	 * \param n variable name
	 * \param v value
	 */
	void uniform_uint(const std::string& n, unsigned int v) const {
		glUniform1ui(glGetUniformLocation(program, n.c_str()), v);
	}
	
	/**
	 * \param n variable name
	 * \param v value
	 */
	void uniform_float(const std::string& n, float v) const {
		glUniform1f(glGetUniformLocation(program, n.c_str()), v);
	}
	
	/**
	 * \param n variable name
	 * \param v value
	 */
	void uniform_vec2(const std::string& n, const vec2& v) const {
		glUniform2fv(glGetUniformLocation(program, n.c_str()), 1, &v.x);
	}
	
	/**
	 * \param n variable name
	 * \param v value
	 */
	void uniform_vec3(const std::string& n, const vec3& v) const {
		glUniform3fv(glGetUniformLocation(program, n.c_str()), 1, &v.x);
	}
	
	/**
	 * \param n variable name
	 * \param v value
	 */
	void uniform_vec4(const std::string& n, const vec4& v) const {
		glUniform4fv(glGetUniformLocation(program, n.c_str()), 1, &v.x);
	}

	/**
	 * \param n variable name
	 * \param v value
	 */
	void uniform_mat2(const std::string& n, const mat<2, 2>& v) const {
		glUniformMatrix2fv(glGetUniformLocation(program, n.c_str()), 1, GL_TRUE, v[0]);
	}

	/**
	 * \param n variable name
	 * \param v value
	 */
	void uniform_mat3(const std::string& n, const mat<3, 3>& v) const {
		glUniformMatrix3fv(glGetUniformLocation(program, n.c_str()), 1, GL_TRUE, v[0]);
	}
	
	/**
	 * \param n variable name
	 * \param v value
	 */
	void uniform_mat4(const std::string& n, const mat<4, 4>& v) const {
		glUniformMatrix4fv(glGetUniformLocation(program, n.c_str()), 1, GL_TRUE, v[0]);
	}
	
	friend inline void preload_vertexobject(const shader& s, vertexobject& vo);
	
protected:
	GLuint program = 0;
	
	GLuint add_shader(const char* s, GLint t) const {
		GLuint new_shader = glCreateShader(t);
		glShaderSource(new_shader, 1, &s, nullptr);
		glCompileShader(new_shader);
		compile_info(new_shader, t);
		glAttachShader(program, new_shader);
		return new_shader;
	 }
	
	void compile_info(GLuint s, GLuint t) const {
		GLint success;
		glGetShaderiv(s, GL_COMPILE_STATUS, &success);
		if (success) return;
		if (t == GL_VERTEX_SHADER) {
			std::cerr << "[Vertex shader compile error] ";
		} else if (t == GL_GEOMETRY_SHADER) {
			std::cerr << "[Geometry shader compile error] ";
		} else if (t == GL_FRAGMENT_SHADER) {
			std::cerr << "[Fragment shader compile error] ";
		}
		GLchar info[1024];
		glGetShaderInfoLog(s, 1024, nullptr, info);
		std::cerr << info;
	}
	
	void link_info() const {
		GLint success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (success) return;
		std::cerr << "[Link error] ";
		GLchar info[1024];
		glGetProgramInfoLog(program, 1024, nullptr, info);
		std::cerr << info;
	}
};

inline constexpr GLint texture_formats[4] = {GL_RED, GL_RG, GL_RGB, GL_RGBA};

inline constexpr GLint texture_internal_formats[10][4] = {
	{GL_R8,		GL_RG8,		GL_RGB8,	GL_RGBA8},		/* TEXTURE_8 */
	{GL_R16, 	GL_RG16,	GL_RGB16,	GL_RGBA16},		/* TEXTURE_16 */
	{GL_R16F,	GL_RG16F,	GL_RGB16F,	GL_RGBA16F},	/* TEXTURE_16F */
	{GL_R32F,	GL_RG32F,	GL_RGB32F,	GL_RGBA32F},	/* TEXTURE_32F */
	{GL_R8I,	GL_RG8I,	GL_RGB8I,	GL_RGBA8I},		/* TEXTURE_8I */
	{GL_R16I,	GL_RG16I,	GL_RGB16I,	GL_RGBA16I},	/* TEXTURE_16I */
	{GL_R32I,	GL_RG32I,	GL_RGB32I,	GL_RGBA32I},	/* TEXTURE_32I */
	{GL_R8UI,	GL_RG8UI,	GL_RGB8UI,	GL_RGBA8UI},	/* TEXTURE_8I */
	{GL_R16UI,	GL_RG16UI,	GL_RGB16UI,	GL_RGBA16UI},	/* TEXTURE_16I */
	{GL_R32UI,	GL_RG32UI,	GL_RGB32UI,	GL_RGBA32UI},	/* TEXTURE_32I */
};

/**
 * \param b texture bytes
 */
inline GLint texture_format(int b) {
	if (b <= 0 || b > 4) {
		error::set(ERROR_UNSOPPORTED, "Illegal argument format");
		return -1;
	}
	return texture_formats[b - 1];
}

/**
 * \param f texture internal format
 * \param b texture bytes
 */
inline GLint texture_internal_format(int f, int b) {
	if (f < 0 || f >= 10 || b <= 0 || b > 4) {
		error::set(ERROR_UNSOPPORTED, "Illegal argument format");
		return -1;
	}
	return texture_internal_formats[f][b - 1];
}

/**
 * Preload vertexobject before rendering.
 *
 * \param s target shader
 * \param vo vertexobject
 */
inline void preload_vertexobject(const shader& s, vertexobject& vo) {
	if (vo.id != 0) error::set(ERROR_UNSOPPORTED, "Vertexobject has been preloaded");
	auto& data = vo.data;
	auto& size = vo.size;
	auto& name = vo.name;
	auto& buffer = vo.buffer_id;
	int data_size = static_cast<int>(data.size());
	buffer.resize(data_size);
	glGenVertexArrays(1, &vo.id);
	glBindVertexArray(vo.id);
	glGenBuffers(data_size, buffer.data());
	for (int i = 0; i < data_size; ++i) {
		glBindBuffer(GL_ARRAY_BUFFER, buffer[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data[i].size(), data[i].data(), GL_STATIC_DRAW);
		GLint attrib = glGetAttribLocation(s.program, name[i].c_str());
		if (attrib == -1) error::set(ERROR_UNKNOWN, "Failed to find attribute name");
		glEnableVertexAttribArray(attrib);
		glVertexAttribPointer(attrib, size[i], GL_FLOAT, GL_FALSE, sizeof(float) * size[i], static_cast<void*>(0));
	}
}

/**
 * Preload vertexobject before rendering.
 *
 * \param vo vertexobject
 */
inline void preload_vertexobject(vertexobject& vo) {
	if (vo.id != 0) error::set(ERROR_UNSOPPORTED, "Vertexobject has been preloaded");
	auto& data = vo.data;
	auto& size = vo.size;
	auto& buffer = vo.buffer_id;
	int data_size = static_cast<int>(data.size());
	buffer.resize(data_size);
	glGenVertexArrays(1, &vo.id);
	glBindVertexArray(vo.id);
	glGenBuffers(data_size, &buffer.front());
	for (int i = 0; i < data_size; ++i) {
		glBindBuffer(GL_ARRAY_BUFFER, buffer[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data[i].size(), data[i].data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, size[i], GL_FLOAT, GL_FALSE, sizeof(float) * size[i], static_cast<void*>(0));
	}
}

/**
 * Unload vertexobject.
 *
 * \param vo vertexobject
 */
inline void unload_vertexobject(vertexobject& vo) {
	if (vo.id == 0) error::set(ERROR_UNSOPPORTED, "Vertexobject has not been preloaded");
	glDeleteVertexArrays(1, &vo.id);
	glDeleteBuffers(static_cast<int>(vo.buffer_id.size()), vo.buffer_id.data());
	vo.id = 0;
}

/**
 * Preload texture before rendering.
 *
 * \param t texture
 * \param w wrapping setting
 * \param mag magnifying filter
 * \param min minifying filter
 * \param mip mipmap setting
 */
inline void preload_texture(texture& t, int w = TEXTURE_CLAMP_TO_EDGE, int mag = TEXTURE_LINEAR,
							int min = TEXTURE_NEAREST, int mip = TEXTURE_NO_MIPMAP) {
	if (t.id != 0) error::set(ERROR_UNSOPPORTED, "Texture has been preloaded");
	GLint format = texture_format(t.bytes);
	glGenTextures(1, &t.id);
	glBindTexture(GL_TEXTURE_2D, t.id);
	glTexImage2D(GL_TEXTURE_2D, 0, format, t.width, t.height, 0, format, GL_UNSIGNED_BYTE, &t.data.front());
	if (w == TEXTURE_REPEAT) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	} else if (w == TEXTURE_MIRRORED_REPEAT) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	} else if (w == TEXTURE_CLAMP_TO_EDGE) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	} else if (w == TEXTURE_CLAMP_TO_BORDER) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST + mag);
	if (mip == TEXTURE_NO_MIPMAP) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST + min);
	} else if (mip == TEXTURE_MIPMAP_NEAREST) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST + min);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else if (mip == TEXTURE_MIPMAP_LINEAR) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR + min);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
}

/**
 * Unload texture.
 *
 * \param t texture
 */
inline void unload_texture(texture& t) {
	if (t.id == 0) error::set(ERROR_UNSOPPORTED, "Texture has not been preloaded");
	glDeleteTextures(1, &t.id);
	t.id = 0;
}

/**
 * Load texture when rendering.
 *
 * \param s target shader
 * \param t texture
 * \param i index
 */
inline void load_texture(const shader& s, const texture& t, int i) {
	if (t.id == 0) error::set(ERROR_UNSOPPORTED, "Texture has not been preloaded");
	/* if texture is named */
	if (!t.name.empty()) s.uniform_int(t.name, i);
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, t.id);
}

/**
 * Load texture when rendering.
 *
 * \param t texture
 * \param i index
 */
inline void load_texture(const texture& t, int i) {
	if (t.id == 0) error::set(ERROR_UNSOPPORTED, "Texture has not been preloaded");
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, t.id);
}

/**
 * \param vo vertexobject
 */
inline void draw_objects(const vertexobject& vo) {
	if (vo.id == 0) error::set(ERROR_UNSOPPORTED, "Vertexobject has not preloaded");
	glBindVertexArray(vo.id);
	glDrawArrays(GL_TRIANGLES, 0, vo.length);
}

/**
 * \param c camrea info
 * \param s shader
 * \param is instances
 * \param is_size size of instances
 */
inline void draw_instances(const camera& c, const shader& s, const instance* const* is, size_t is_size) {
	mat<4, 4> viewing_projection = c.projection * c.viewing;
	mat<4, 4> model;
	s.use();
	for (int i = 0; i < is_size; ++i) {
		camera::model_transform(*is[i], model);
		mat<4, 4> transform = viewing_projection * model;
		s.uniform_mat4("model", model);
		s.uniform_mat4("view", c.viewing);
		s.uniform_mat4("proj", c.projection);
		s.uniform_mat4("model_view_proj", transform);
		s.uniform_mat4("inv_view_proj", mat4x4_inverse(viewing_projection));
		for (int t = 0; t < is[i]->textures.size(); ++t) {
			load_texture(s, *is[i]->textures[t], t);
		}
		draw_objects(*is[i]->object);
	}
}

/**
 * \param c camrea info
 * \param s shader
 * \param scene_info scene info
 */
inline void draw_scene(const camera& c, shader& s, const scene& scene_info) {
	draw_instances(c, s, scene_info.get_instances(), scene_info.instance_size());
}

class cubemap {
public:
	/*
	 * 0: left side
	 * 1: right side
	 * 2: upper side
	 * 3: lower side
	 * 4: front side
	 * 5: back side
	 */
	texture faces[6];
	shader custom_shader;
	
	cubemap() = default;
	
	texture& operator[](size_t k) {
		return faces[k];
	}
	
	bool loaded() const {
		return texture_id != 0;
	}
	
	/**
	 * \param mag magnifying filter
	 * \param min minifying filter
	 */
	void preload(int mag = TEXTURE_LINEAR, int min = TEXTURE_NEAREST) {
		GLint format = texture_format(faces[0].bytes);
		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
		for (int i = 0; i < 6; ++i) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, faces[i].width, faces[i].height,
						 0, format, GL_UNSIGNED_BYTE, faces[i].data.data());
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST + mag); /* GL_NEAREST or GL_LINEAR */
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST + min); /* GL_NEAREST or GL_LINEAR */
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
	
	void unload() {
		glDeleteTextures(1, &texture_id);
		texture_id = 0;
	}
	
	/**
	 * \param c camera info
	 */
	void draw(const camera& c) const {
		glDepthFunc(GL_LEQUAL);
		mat<4, 4> viewing = c.viewing;
		viewing[0][3] = 0;
		viewing[1][3] = 0;
		viewing[2][3] = 0;
		const shader& available_shader = custom_shader.loaded() ? custom_shader : default_shader;
		available_shader.use();
		available_shader.uniform_mat4("view_proj", c.projection * viewing);
		available_shader.uniform_int("box", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
		draw_objects(cube_object);
		glDepthFunc(GL_LESS);
	}
	
	friend inline void init();
	
private:
	GLuint texture_id = 0;
	static vertexobject cube_object;
	static shader default_shader;
};

inline vertexobject cubemap::cube_object = {
	0,										/* id */
	36,										/* length */
	{3},									/* size */
	{{"vertex"}},							/* name */
	{0},									/* buffer_id */
	{{										/* data */
		-1,  1, -1, -1, -1, -1,  1, -1, -1,
		 1, -1, -1,  1,  1, -1, -1,  1, -1, /* back side */
		-1, -1,  1, -1, -1, -1, -1,  1, -1,
		-1,  1, -1, -1,  1,  1, -1, -1,  1,	/* left side */
		 1, -1, -1,  1, -1,  1,  1,  1,  1,
		 1,  1,  1,  1,  1, -1,  1, -1, -1,	/* right side */
		-1, -1,  1, -1,  1,  1,  1,  1,  1,
		 1,  1,  1,  1, -1,  1, -1, -1,  1,	/* front side */
		-1,  1, -1,  1,  1, -1,  1,  1,  1,
		 1,  1,  1, -1,  1,  1, -1,  1, -1,	/* upper side */
		-1, -1, -1, -1, -1,  1,  1, -1, -1,
		 1, -1, -1, -1, -1,  1,  1, -1,  1,	/* lower side */
	}},
};

inline shader cubemap::default_shader;

class canvas {
public:
	texture surface;
	shader custom_shader;
	
	canvas() = default;
	
	bool loaded() const {
		return surface.id == 0;
	}
	
	/**
	 * \param mag magnifying filter
	 * \param min minifying filter
	 */
	void preload(int mag = TEXTURE_LINEAR, int min = TEXTURE_NEAREST) {
		preload_texture(surface, TEXTURE_CLAMP_TO_EDGE, mag, min, TEXTURE_NO_MIPMAP);
	}
	
	void unload() {
		unload_texture(surface);
	}
	
	void draw() const {
		const shader& available_shader = custom_shader.loaded() ? custom_shader : default_shader;
		available_shader.use();
		available_shader.uniform_int("canvas", 0);
		load_texture(surface, 0);
		draw_objects(canvas_object);
	}
	
	friend inline void init();
	
private:
	static vertexobject canvas_object;
	static shader default_shader;
};

inline vertexobject canvas::canvas_object = {
	0,												/* id */
	6,												/* length */
	{2},											/* size */
	{{"vertex"}},									/* name */
	{0},											/* buffer_id */
	{{1, 1, -1, 1, 1, -1, -1, -1, 1, -1, -1, 1}},	/* data */
};

inline shader canvas::default_shader;

class framebuffer {
public:
	int width = 0;
	int height = 0;
	bool depth_test = true;
	bool stencil_test = true;
	bool buffer_readonly = true;
	shader custom_shader;
	
	framebuffer() = default;
	
	/**
	 * \param w render width
	 * \param h render height
	 */
	framebuffer(int w, int h) : width(w), height(h) {}
	
	static void unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	static void clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}
	
	bool loaded() const {
		return framebuffer_id != 0;
	}
	
	/**
	 * \param s texture size
	 * \param b texture bytes
	 * \param fs texture internal formats
	 */
	void preload(int s, int b, int* fs = nullptr) {
		glGenFramebuffers(1, &framebuffer_id);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
		/* color attachment */
		buffers.resize(s);
		glGenTextures(s, buffers.data());
		std::vector<GLuint> attachments(s);
		GLuint format = texture_format(b);
		for (int i = 0; i < s; ++i) {
			attachments[i] = GL_COLOR_ATTACHMENT0 + i;
			glBindTexture(GL_TEXTURE_2D, buffers[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, fs ? texture_internal_format(fs[i], b) : format,
						 width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[i], GL_TEXTURE_2D, buffers[i], 0);
		}
		glDrawBuffers(s, attachments.data());
		/* depth stencil */
		if (!depth_test) return;
		if (buffer_readonly) {
			glGenRenderbuffers(1, &depth_stencil_id);
			glBindRenderbuffer(GL_RENDERBUFFER, depth_stencil_id);
			if (stencil_test) {
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
										  GL_RENDERBUFFER, depth_stencil_id);
			} else {
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
										  GL_RENDERBUFFER, depth_stencil_id);
			}
		} else {
			glGenTextures(1, &depth_stencil_id);
			glBindTexture(GL_TEXTURE_2D, depth_stencil_id);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			if (stencil_test) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_STENCIL, width, height,
							 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth_stencil_id, 0);
			} else {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height,
							 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_stencil_id, 0);
			}
		}
	}
	
	void unload() {
		glDeleteFramebuffers(1, &framebuffer_id);
		glDeleteTextures(static_cast<int>(buffers.size()), buffers.data());
		glDeleteRenderbuffers(1, &depth_stencil_id);
	}
	
	void bind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
		depth_test ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
		stencil_test ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
	}
	
	void draw() const {
		custom_shader.loaded() ? custom_shader.use() : default_shader.use();
		for (int i = 0; i < buffers.size(); ++i) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, buffers[i]);
		}
		draw_objects(framebuffer_object);
	}
	
	friend inline void init();
	
private:
	GLuint framebuffer_id = 0;
	GLuint depth_stencil_id = 0;
	std::vector<GLuint> buffers;
	static vertexobject framebuffer_object;
	static shader default_shader;
};

inline vertexobject framebuffer::framebuffer_object = {
	0,												/* id */
	6,												/* length */
	{2},											/* size */
	{{"vertex"}},									/* name */
	{0},											/* buffer_id */
	{{1, 1, -1, 1, 1, -1, -1, -1, 1, -1, -1, 1}},	/* data */
};

inline shader framebuffer::default_shader;

inline void init() {
	cubemap::default_shader.load(cubemap_vertex_shader, cubemap_fragment_shader);
	preload_vertexobject(cubemap::default_shader, cubemap::cube_object);
	canvas::default_shader.load(canvas_vertex_shader, canvas_fragment_shader);
	preload_vertexobject(canvas::default_shader, canvas::canvas_object);
	framebuffer::default_shader.load(framebuffer_vertex_shader, framebuffer_fragment_shader);
	preload_vertexobject(framebuffer::default_shader, framebuffer::framebuffer_object);
}

}

#endif
