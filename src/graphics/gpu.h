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

#include "../core/file.h"
#include "../math/maths.h"
#include "../objects/constants.h"
#include "../objects/uniforms.h"
#include "../objects/mesh.h"
#include "../objects/image.h"
#include "../objects/material.h"
#include "../objects/instance.h"
#include "../camera/camera.h"

#ifdef __APPLE__
#define	GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#else
#include "../glad/glad.h"
#endif

/* texture type */
enum {
	TEXTURE_1D,
	TEXTURE_2D,
	TEXTURE_3D,
	TEXTURE_CUBE,
};

/* texture wrapping mode */
enum {
	TEXTURE_REPEAT,
	TEXTURE_MIRRORED_REPEAT,
	TEXTURE_CLAMP_TO_EDGE,
	TEXTURE_CLAMP_TO_BORDER,
};

/* texture mag & min filter */
enum {
	TEXTURE_NEAREST,
	TEXTURE_LINEAR,
	TEXTURE_NEAREST_MIPMAP_NEAREST,
	TEXTURE_LINEAR_MIPMAP_NEAREST,
	TEXTURE_NEAREST_MIPMAP_LINEAR,
	TEXTURE_LINEAR_MIPMAP_LINEAR,
};

namespace gpu {

struct rect {
	int x = 0;			/* the lower-left corner of the rectangle */
	int y = 0;			/* the lower-left corner of the rectangle */
	int width = 0;		/* the width of the rectangle */
	int height = 0;		/* the height of the rectangle */
};

/**
 * Clear the color, depth or stencil drawing buffers. Initialize the color
 * buffer to the current clear color value.
 *
 * \param c whether to clear the color buffer
 * \param d whether to clear the depth buffer
 * \param s whether to clear the stencil buffer
 */
void clear(bool c = true, bool d = true, bool s = true);

/**
 * Returns the current clear color and opacity.
 */
vec4 get_clear_color();

/**
 * Sets the clear color and opacity.
 *
 * \param c clear color
 * \param a clear opacity
 */
void set_clear_color(const vec3& c, float a = 1);

/**
 * Sets the clear color and opacity.
 *
 * \param c clear color
 */
void set_clear_color(const vec4& c);

/**
 * Enable depth test.
 */
void enable_depth_test();

/**
 * Disable depth test.
 */
void disable_depth_test();

/**
 * Returns the current comparison function in depth test.
 */
int get_depth_func();

/**
 * Sets the comparison function in depth test.
 *
 * \param f comparison function
 */
void set_depth_func(int f);

/**
 * Enable stencil test.
 */
void enable_stencil_test();

/**
 * Disable stencil test.
 */
void disable_stencil_test();

/**
 * Returns the writemask in stencil test.
 */
int get_stencil_writemask();

/**
 * Sets the writemask in stencil test.
 *
 * \param m writemask
 */
void set_stencil_writemask(int m);

/**
 * Returns the comparison function in stencil test.
 */
int get_stencil_func();

/**
 * Returns the reference value in stencil test.
 */
int get_stencil_ref();

/**
 * Returns the mask in stencil test.
 */
int get_stencil_mask();

/**
 * Sets the comparison function, reference value and mask in stencil test.
 *
 * \param f comparison function
 * \param r reference value
 * \param m mask
 */
void set_stencil_func(int f, int r, int m);

/**
 * Sets the operations in stencil test.
 *
 * \param f the operation when the stencil test fails
 * \param zf the operation when the stencil test passes but the depth test fails
 * \param zp the operation when both the stencil test and the depth test pass
 */
void set_stencil_op(int f, int zf, int zp);

/**
 * Enable alpha test.
 */
void enable_scissor_test();

/**
 * Disable alpha test.
 */
void disable_scissor_test();

/**
 * Returns the current scissor region.
 */
rect get_scissor();

/**
 * Sets the scissor region.
 *
 * \param s scissor region
 */
void set_scissor(const rect& s);

/**
 * Enable wireframe.
 */
void enable_wireframe();

/**
 * Disable wireframe.
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
 * Determines which face to be culled.
 */
int get_cull_face();

/**
 * Determines which face to be culled.
 *
 * \param s culling side
 */
void set_cull_face(int s);

/**
 * Returns the current viewport.
 */
rect get_viewport();

/**
 * Set the viewport.
 *
 * \param v viewport
 */
void set_viewport(const rect& v);

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

/**
 * Sets the material active.
 *
 * \param m material
 */
void active_material_depth(const material& m);

/**
 * Sets the material active.
 *
 * \param m material
 */
void active_material_stencil(const material& m);

/**
 * Sets the material active.
 *
 * \param m material
 */
void active_material_wireframe(const material& m);

/**
 * Sets the material active.
 *
 * \param m material
 */
void active_material_side(const material& m);

/**
 * Sets the material active.
 *
 * \param m material
 */
void active_material(const material& m);

class shader {
public:
	/**
	 * Create a new shader.
	 */
	shader();
	
	~shader();
	
	/**
	 * Load vertex shader, fragment shader (and geometry shader).
	 *
	 * \param vert vertex shader
	 * \param frag fragment shader
	 * \param geom geometry shader
	 */
	void load(const char* vert, const char* frag, const char* geom = nullptr);
	
	/**
	 * Load vertex shader, fragment shader (and geometry shader).
	 *
	 * \param vert vertex shader
	 * \param frag fragment shader
	 * \param geom geometry shader
	 */
	void load(const std::string* vert, const std::string* frag, const std::string* geom = nullptr);
	
	/**
	 * Readvertex shader, fragment shader (and geometry shader) from GLSL files.
	 *
	 * \param vert vertex shader file
	 * \param frag fragment shader file
	 * \param geom geometry shader file
	 */
	int read(const char* vert, const char* frag, const char* geom = nullptr);
	
	/**
	 * Read vertex shader, fragment shader (and geometry shader) from GLSL files.
	 *
	 * \param vert vertex shader file
	 * \param frag fragment shader file
	 * \param geom geometry shader file
	 */
	int read(const std::string* vert, const std::string* frag, const std::string* geom = nullptr);
	
	/**
	 * Use the program of shader.
	 */
	void use_program() const;
	
	/**
	 * Sets a value for the specified uniform variable.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_i(const std::string& n, int v) const;

	/**
	 * Sets a value for the specified uniform variable.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_u(const std::string& n, unsigned int v) const;

	/**
	 * Sets a value for the specified uniform variable.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_f(const std::string& n, float v) const;

	/**
	 * Sets a value for the specified uniform variable.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_v2(const std::string& n, const vec2& v) const;

	/**
	 * Sets a value for the specified uniform variable.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_v3(const std::string& n, const vec3& v) const;

	/**
	 * Sets a value for the specified uniform variable.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_v4(const std::string& n, const vec4& v) const;

	/**
	 * Sets a value for the specified uniform variable.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_m2(const std::string& n, const mat2& v) const;

	/**
	 * Sets a value for the specified uniform variable.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_m3(const std::string& n, const mat3& v) const;

	/**
	 * Sets a value for the specified uniform variable.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_m4(const std::string& n, const mat4& v) const;
	
	/**
	 * Sets values for the specified uniform variables.
	 *
	 * \param u uniform variables
	 */
	void set_uniforms(const uniforms& u) const;
	
	/**
	 * Sets the path of included shaders.
	 *
	 * \param p include path
	 */
	static void set_include_path(const std::string& p);

protected:
	GLuint program = 0;
	
	static std::string include_path;

	GLuint add_shader(const std::string& s, GLint t) const;

	void compile_info(GLuint s, GLuint t) const;

	void link_info() const;
	
	static void resolve_include(std::string& s);
	
	friend class vertexobject;
};

class bufferobject {
public:
	/**
	 * Create a new bufferobject.
	 */
	bufferobject();
	
	~bufferobject();
	
	bufferobject(const bufferobject&) = delete;
	
	bufferobject& operator=(const bufferobject&) = delete;
	
private:
	GLuint id = 0;
	
	friend class vertexobject;
};

class vertexobject {
public:
	/**
	 * Create a new vertexobject.
	 */
	vertexobject();
	
	~vertexobject();
	
	vertexobject(const vertexobject&) = delete;
	
	vertexobject& operator=(const vertexobject&) = delete;
	
	/**
	 * Preload vertexobject.
	 *
	 * \param m mesh
	 * \param g material group index
	 */
	void preload(const mesh& m, unsigned int g = 0);
	
	/**
	 * Attach vertexobject to target shader in order to match the input datas
	 * with the shader locations automatically.
	 *
	 * \param s target shader
	 */
	void attach(const shader& s) const;
	
	/**
	 * Draw vertexobject.
	 */
	void draw() const;
	
private:
	GLuint id = 0;
	int length = 0;
	std::vector<int> sizes;
	std::vector<int> locations;
	std::vector<std::string> names;
	std::unique_ptr<bufferobject> buffer;
};

class texture {
public:
	/**
	 * Create a new texture.
	 */
	texture();
	
	~texture();
	
	texture(const texture&) = delete;
	
	texture& operator=(const texture&) = delete;
	
	/**
	 * Preload 1d texture with empty data.
	 *
	 * \param w the width of texture
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
	void preload_2d(const image& i, int s = IMAGE_8, int t = IMAGE_UBYTE);
	
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
	void preload_cube(const image* is, int s = IMAGE_8, int t = IMAGE_UBYTE);
	
	/**
	 * Returns the type of texture.
	 */
	int get_type() const;
	
	/**
	 * Sets the wrapping mode on S coordinate.
	 *
	 * \param m wrapping mode
	 */
	void set_wrap_s(int m) const;
	
	/**
	 * Sets the wrapping mode on T coordinate.
	 *
	 * \param m wrapping mode
	 */
	void set_wrap_t(int m) const;
	
	/**
	 * Sets the wrapping mode on R coordinate.
	 *
	 * \param m wrapping mode
	 */
	void set_wrap_r(int m) const;
	
	/**
	 * Sets the wrapping modes on all coordinates.
	 *
	 * \param m wrapping mode
	 */
	void set_wrap_all(int m) const;
	
	/**
	 * Set the magnification and minification filters of texture.
	 *
	 * \param mag magnification filter
	 * \param min minification filter
	 */
	void set_filters(int mag, int min) const;
	
	/**
	 * Generate mipmap for the texture.
	 */
	void generate_mipmap() const;
	
	/**
	 * Sets the border color.
	 *
	 * \param c border color
	 */
	void set_border_color(const vec4& c) const;
	
	/**
	 * Sets the texture active.
	 *
	 * \param i index of texture
	 */
	int active(int i) const;
	
private:
	GLuint id = 0;
	int type = -1;
	
	static GLenum gl_type(int t);
	
	static GLint gl_base_format(int b);
	
	static GLenum gl_sized_format(int b, int s);
	
	static GLenum gl_data_type(int t);
	
	static GLint gl_wrapping_mode(int m);
	
	static GLint gl_filter(int f);
	
	friend class renderbuffer;
	
	friend class framebuffer;
};

class renderbuffer {
public:
	/**
	 * Create a new renderbuffer.
	 */
	renderbuffer();
	
	/**
	 * Create a new renderbuffer.
	 *
	 * \param w the width of texture
	 * \param h the height of texture
	 * \param b image base format
	 * \param s image sized format
	 */
	renderbuffer(int w, int h, int b, int s);
	
	~renderbuffer();
	
	/**
	 * Preload renderbuffer.
	 *
	 * \param w the width of texture
	 * \param h the height of texture
	 * \param b image base format
	 * \param s image sized format
	 */
	void preload(int w, int h, int b, int s = IMAGE_8) const;
	
private:
	GLuint id = 0;
	
	friend class framebuffer;
};

class framebuffer {
public:
	/**
	 * Create a new framebuffer.
	 */
	framebuffer();
	
	~framebuffer();
	
	framebuffer(const framebuffer&) = delete;
	
	framebuffer& operator=(const framebuffer&) = delete;
	
	/**
	 * Sets framebuffer active.
	 */
	void active() const;
	
	/**
	 * Sets default framebuffer active.
	 */
	static void default_active();
	
	/**
	 * Sets which color attachments can be drawn into.
	 *
	 * \param l color attachments list
	 */
	void draw_attachments(const std::initializer_list<int>& l) const;
	
	/**
	 * Disable draw into color buffers.
	 */
	void disable_draw() const;
	
	/**
	 * Sets the texture as attachment.
	 *
	 * \param t texture
	 * \param i index of attachment
	 */
	void set_attachment(const texture& t, unsigned int i) const;
	
	/**
	 * Sets the renderbuffer as attachment.
	 *
	 * \param r renderbuffer
	 * \param i index of attachment
	 */
	void set_attachment(const renderbuffer& r, unsigned int i) const;
	
	/**
	 * Sets the texture as depth / stencil attachment.
	 *
	 * \param t texture
	 * \param ds whether texture is used as depth stencil buffer
	 */
	void set_depth_attachment(const texture& t, bool ds = false) const;
	
	/**
	 * Sets the renderbuffer as depth / stencil attachment.
	 *
	 * \param r renderbuffer
	 * \param ds whether renderbuffer is used as depth stencil buffer
	 */
	void set_depth_attachment(const renderbuffer& r, bool ds = false) const;
	
private:
	GLuint id = 0;
};

}
