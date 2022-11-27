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

#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include "../core/File.h"
#include "../resource/Mesh.h"
#include "../resource/Image.h"
#include "Camera.h"
#include "Instance.h"

#ifdef __APPLE__
#define	GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#else
#include "../glad/glad.h"
#endif

namespace WR {

enum ImageBaseFormat {
	IMAGE_R,
	IMAGE_RG,
	IMAGE_RGB,
	IMAGE_RGBA,
	IMAGE_D,
	IMAGE_DS,
};

enum ImageSizedFormat {
	IMAGE_NONE,
	IMAGE_8,
	IMAGE_16,
	IMAGE_16F,
	IMAGE_32F,
	IMAGE_8I,
	IMAGE_16I,
	IMAGE_32I,
	IMAGE_8UI,
	IMAGE_16UI,
	IMAGE_32UI,
};

enum ImageDataType {
	IMAGE_UBYTE,
	IMAGE_BYTE,
	IMAGE_USHORT,
	IMAGE_SHORT,
	IMAGE_UINT,
	IMAGE_INT,
	IMAGE_HALF_FLOAT,
	IMAGE_FLOAT,
	IMAGE_UINT_24_8,
};

enum TextureType {
	TEXTURE_1D,
	TEXTURE_2D,
	TEXTURE_3D,
	TEXTURE_CUBE,
	TEXTURE_2D_MS,
};

enum TextureFilterType {
	TEXTURE_NEAREST,
	TEXTURE_LINEAR,
};

enum TextureMipmapType {
	TEXTURE_NO_MIPMAP,
	TEXTURE_MIPMAP_NEAREST,
	TEXTURE_MIPMAP_LINEAR,
};

enum TextureWrappingType {
	TEXTURE_REPEAT,
	TEXTURE_MIRRORED_REPEAT,
	TEXTURE_CLAMP_TO_EDGE,
	TEXTURE_CLAMP_TO_BORDER,
};

enum ComparisonFunction {
	FUNC_NEVER,
	FUNC_LESS,
	FUNC_EQUAL,
	FUNC_LEQUAL,
	FUNC_GREATER,
	FUNC_NOTEQUAL,
	FUNC_GEQUAL,
	FUNC_ALWAYS,
};

namespace Gpu {

/**
 * Clear color buffer, depth buffer and stencil buffer.
 *
 * \param c clear color buffer
 * \param d clear depth buffer
 * \param s clear stencil buffer
 */
void clear(bool c = true, bool d = true, bool s = true);

/**
 * Set clear color.
 *
 * \param c color
 */
void clear_color(const Vec3& c);

/**
 * Enable depth test.
 */
void enable_depth_test();

/**
 * Disable depth test.
 */
void disable_depth_test();

/**
 * Enable stencil test.
 */
void enable_stencil_test();

/**
 * Disable stencil test.
 */
void disable_stencil_test();

/**
 * Enable wireframe mode.
 */
void enable_wireframe();

/**
 * Disable wireframe mode.
 */
void disable_wireframe();

/**
 * Enable face culling.
 */
void enable_cull_face();

/**
 * Disable face culling.
 */
void disable_cull_face();

/**
 * Set whether front- or back-facing facets are culled.
 *
 * \param s culling side
 */
void cull_face(int s);

/**
 * Set comparison function of depth test.
 *
 * \param f comparison function
 */
void depth_func(int f);

/**
 * Enable multisample.
 */
void enable_multisample();

/**
 * Disable multisample.
 */
void disable_multisample();

/**
 * Print errors from OpenGL.
 */
void print_error();

struct Viewport {
	int x = 0;			/* lower-left corner of the viewport rectangle */
	int y = 0;			/* lower-left corner of the viewport rectangle */
	int width = 0;		/* width of the viewport rectangle */
	int height = 0;		/* height of the viewport rectangle */
};

/**
 * Set viewport of OpenGL.
 *
 * \param v viewport
 */
void set_viewport(const Viewport& v);

/**
 * Get viewport of OpenGL.
 */
Viewport get_viewport();

class Shader {
public:
	/**
	 * Create a new shader.
	 */
	Shader();
	
	~Shader();
	
	/**
	 * Load vertex shader, geometry shader (optional) and fragment shader.
	 *
	 * \param vert vertex shader
	 * \param frag fragment shader
	 * \param geom geometry shader
	 */
	void load(const char* vert, const char* frag, const char* geom = nullptr);
	
	/**
	 * Load vertex shader, geometry shader (optional) and fragment shader
	 *
	 * \param vert vertex shader
	 * \param frag fragment shader
	 * \param geom geometry shader
	 */
	void load(const std::string* vert, const std::string* frag, const std::string* geom = nullptr);
	
	/**
	 * Read vertex shader, geometry shader (optional) and fragment shader from files.
	 *
	 * \param vert vertex shader file
	 * \param frag fragment shader file
	 * \param geom geometry shader file
	 */
	int read(const char* vert, const char* frag, const char* geom = nullptr);
	
	/**
	 * Read vertex shader, geometry shader (optional) and fragment shader from files.
	 *
	 * \param vert vertex shader file
	 * \param frag fragment shader file
	 * \param geom geometry shader file
	 */
	int read(const std::string* vert, const std::string* frag, const std::string* geom = nullptr);
	
	/**
	 * Use shader during rendering.
	 */
	void use() const;

	/**
	 * Set a value for the specified uniform variable.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void uniform_int(const std::string& n, int v) const;

	/**
	 * Set a value for the specified uniform variable.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void uniform_uint(const std::string& n, unsigned int v) const;

	/**
	 * Set a value for the specified uniform variable.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void uniform_float(const std::string& n, float v) const;

	/**
	 * Set a value for the specified uniform variable.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void uniform_vec2(const std::string& n, const Vec2& v) const;

	/**
	 * Set a value for the specified uniform variable.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void uniform_vec3(const std::string& n, const Vec3& v) const;

	/**
	 * Set a value for the specified uniform variable.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void uniform_vec4(const std::string& n, const Vec4& v) const;

	/**
	 * Set a value for the specified uniform variable.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void uniform_mat2(const std::string& n, const Mat<2, 2>& v) const;

	/**
	 * Set a value for the specified uniform variable.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void uniform_mat3(const std::string& n, const Mat<3, 3>& v) const;

	/**
	 * Set a value for the specified uniform variable.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void uniform_mat4(const std::string& n, const Mat<4, 4>& v) const;

protected:
	GLuint program = 0;

	GLuint add_shader(const std::string& s, GLint t) const;

	void compile_info(GLuint s, GLuint t) const;

	void link_info() const;
	
	static void resolve_include(std::string& s);
	
	friend class VertexObject;
};

class BufferObject {
public:
	/**
	 * Create a new bufferobject.
	 */
	BufferObject();
	
	~BufferObject();
	
	BufferObject(const BufferObject&) = delete;
	
	BufferObject& operator=(const BufferObject&) = delete;
	
private:
	GLuint id = 0;
	
	friend class VertexObject;
};

class VertexObject {
public:
	/**
	 * Create a new vertexobject.
	 */
	VertexObject();
	
	~VertexObject();
	
	VertexObject(const VertexObject&) = delete;
	
	VertexObject& operator=(const VertexObject&) = delete;
	
	/**
	 * Preload vertexobject before rendering.
	 *
	 * \param m mesh
	 * \param g material group
	 */
	void preload(const Mesh& m, unsigned int g = 0);
	
	/**
	 * Attach vertexobject to target shader. If vertexobject is attached, you
	 * can call variable names directly in shader. Otherwise you have to declare
	 * the location manually.
	 *
	 * \param s target shader
	 */
	void attach(const Shader& s) const;
	
	/**
	 * Draw vertexobject.
	 */
	void draw() const;
	
private:
	GLuint id = 0;
	int length = 0;
	std::vector<int> sizes;
	std::vector<std::string> names;
	std::unique_ptr<BufferObject[]> buffers;
};

class Texture {
public:
	/**
	 * Create a new texture.
	 */
	Texture();
	
	~Texture();
	
	Texture(const Texture&) = delete;
	
	Texture& operator=(const Texture&) = delete;
	
	/**
	 * Preload 1d texture with empty data.
	 *
	 * \param w width of texture
	 * \param b image base format
	 * \param s image sized format
	 * \param t image data type
	 */
	void preload_1d(int w, int b, int s = IMAGE_8, int t = IMAGE_UBYTE);
	
	/**
	 * Preload 2d texture with empty data.
	 *
	 * \param w width of texture
	 * \param h height of texture
	 * \param b image base format
	 * \param s image sized format
	 * \param t image data type
	 */
	void preload_2d(int w, int h, int b, int s = IMAGE_8, int t = IMAGE_UBYTE);
	
	/**
	 * Preload 2d texture with image.
	 *
	 * \param i image
	 * \param s image sized format
	 * \param t image data type
	 */
	void preload_2d(const Image& i, int s = IMAGE_8, int t = IMAGE_UBYTE);
	
	/**
	 * Preload 3d texture with empty data.
	 *
	 * \param w width of texture
	 * \param h height of texture
	 * \param d depth of texture
	 * \param b image base format
	 * \param s image sized format
	 * \param t image data type
	 */
	void preload_3d(int w, int h, int d, int b, int s = IMAGE_8, int t = IMAGE_UBYTE);
	
	/**
	 * Preload cube texture with empty data.
	 *
	 * \param w width of texture
	 * \param h height of texture
	 * \param b image base format
	 * \param s image sized format
	 * \param t image data type
	 */
	void preload_cube(int w, int h, int b, int s = IMAGE_8, int t = IMAGE_UBYTE);
	
	/**
	 * Preload cube texture with images.
	 *
	 * \param is left, right, upper, lower, front, back images of cube
	 * \param s image sized format
	 * \param t image data type
	 */
	void preload_cube(const Image* is, int s = IMAGE_8, int t = IMAGE_UBYTE);
	
	/**
	 * Preload 2d multisample texture with empty data.
	 *
	 * \param w width of texture
	 * \param h height of texture
	 * \param b image base format
	 * \param s image sized format
	 * \param samples number of samples
	 */
	void preload_2d_ms(int w, int h, int b, int s = IMAGE_8, int samples = 4);
	
	/**
	 * Get type of texture.
	 */
	int get_type() const;
	
	/**
	 * Set name of texture.
	 *
	 * \param n name
	 */
	void set_name(const std::string& n);
	
	/**
	 * Get name of texture.
	 */
	std::string get_name() const;
	
	/**
	 * Set parameters of texture.
	 *
	 * \param w wrapping type
	 * \param mag magnifying filter type
	 * \param min minifying filter type
	 * \param mip mipmap type
	*/
	void set_parameters(int w = TEXTURE_REPEAT, int mag = TEXTURE_LINEAR,
						int min = TEXTURE_NEAREST, int mip = TEXTURE_NO_MIPMAP);
	
	/**
	 * Set border color.
	 *
	 * \param c border color
	 */
	void set_border_color(const Vec4& c);
	
	/**
	 * Use texture during rendering.
	 *
	 * \param i index of texture
	 */
	int use(int i) const;
	
	/**
	 * Get image base format in OpenGL.
	 *
	 * \param b image base format
	 */
	static GLint gl_base_format(int b);
	
	/**
	 * Get image sized format in OpenGL.
	 *
	 * \param b image base format
	 * \param s image sized format
	 */
	static GLenum gl_sized_format(int b, int s);
	
	/**
	 * Get image data type in OpenGL.
	 *
	 * \param t image data type
	 */
	static GLenum gl_data_type(int t);
	
	/**
	 * Get texture type in OpenGL.
	 */
	static GLenum gl_texture_type(int t);
	
private:
	GLuint id = 0;
	int type = -1;
	std::string name;
	
	static constexpr GLint gl_base_formats[] = {
		GL_RED,												/* IMAGE_R */
		GL_RG,												/* IMAGE_RG */
		GL_RGB,												/* IMAGE_RGB */
		GL_RGBA,											/* IMAGE_RGBA */
		GL_DEPTH_COMPONENT,									/* IMAGE_D */
		GL_DEPTH_STENCIL,									/* IMAGE_DS */
	};
	
	static constexpr GLenum gl_sized_formats[][4] = {
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
	
	static constexpr GLenum gl_data_types[] = {
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
	
	static constexpr GLenum gl_texture_types[] = {
		GL_TEXTURE_1D,										/* TEXTURE_1D */
		GL_TEXTURE_2D,										/* TEXTURE_2D */
		GL_TEXTURE_3D,										/* TEXTURE_3D */
		GL_TEXTURE_CUBE_MAP,								/* TEXTURE_CUBE */
		GL_TEXTURE_2D_MULTISAMPLE,							/* TEXTURE_2D_MS */
	};
	
	friend class FrameBuffer;
};

class RenderBuffer {
public:
	/**
	 * Create a new renderbuffer.
	 */
	RenderBuffer();
	
	/**
	 * Create a new renderbuffer.
	 *
	 * \param w width of texture
	 * \param h height of texture
	 * \param b image base format
	 * \param s image sized format
	 */
	RenderBuffer(int w, int h, int b, int s);
	
	~RenderBuffer();
	
	/**
	 * Preload texturecube before rendering.
	 *
	 * \param w width of texture
	 * \param h height of texture
	 * \param b image base format
	 * \param s image sized format
	 */
	void preload(int w, int h, int b, int s = IMAGE_NONE);
	
private:
	GLuint id = 0;
	
	friend class FrameBuffer;
};

class FrameBuffer {
public:
	/**
	 * Create a new framebuffer.
	 */
	FrameBuffer();
	
	~FrameBuffer();
	
	FrameBuffer(const FrameBuffer&) = delete;
	
	FrameBuffer& operator=(const FrameBuffer&) = delete;
	
	/**
	 * Bind framebuffer.
	 */
	void bind() const;
	
	/**
	 * Bind default framebuffer.
	 */
	void unbind() const;
	
	/**
	 * Set which color buffers are to be drawn into.
	 *
	 * \param b color buffer list
	 */
	void set_draw(const std::initializer_list<int>& b);
	
	/**
	 * Disable draw into any buffer.
	 */
	void disable_draw() const;
	
	/**
	 * Set texture as buffer object.
	 *
	 * \param t texture
	 * \param i index of attachment
	 * \param d whether texture is used as depth buffer
	 * \param ds whether texture is used as depth stencil buffer
	 */
	void set_texture(const Texture& t, unsigned int i, bool d = false, bool ds = false);
	
	/**
	 * Set renderbuffer as depth / stencil buffer object.
	 *
	 * \param r renderobject
	 * \param ds whether renderbuffer is used as depth stencil buffer
	 */
	void set_renderbuffer(const RenderBuffer& r, bool ds = false);
	
private:
	GLuint id = 0;
};

}

}
