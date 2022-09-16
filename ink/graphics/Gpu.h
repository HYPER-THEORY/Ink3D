/**
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

#include "../objects/Constants.h"
#include "../objects/Uniforms.h"
#include "../objects/Defines.h"
#include "../objects/Mesh.h"
#include "../objects/Image.h"
#include "../objects/Material.h"

namespace Ink::Gpu {

class Rect {
public:
	int x = 0;         /**< the x-coordinate of left lower corner */
	int y = 0;         /**< the y-coordinate of left lower corner */
	int width = 0;     /**< the width of the rectangle */
	int height = 0;    /**< the height of the rectangle */
	
	/**
	 * Create a new Rect.
	 */
	Rect() = default;
	
	/**
	 * Create a new Rect with its size.
	 *
	 * \param w the width of the rectangle
	 * \param h the height of the rectangle
	 */
	Rect(int w, int h);
	
	/**
	 * Create a new Rect with its size and position.
	 *
	 * \param x the x-coordinate of left lower corner
	 * \param y the y-coordinate of left lower corner
	 * \param w the width of the rectangle
	 * \param h the height of the rectangle
	 */
	Rect(int x, int y, int w, int h);
};

/**
 * Block until all GPU commands is complete.
 */
void finish();

/**
 * Forces execution of GPU commands in finite time.
 */
void flush();

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
Vec4 get_clear_color();

/**
 * Sets the clear color and opacity.
 *
 * \param c clear color
 * \param a clear opacity
 */
void set_clear_color(const Vec3& c, float a = 1);

/**
 * Sets the clear color and opacity.
 *
 * \param c clear color
 */
void set_clear_color(const Vec4& c);

/**
 * Enable depth test when rendering objects.
 */
void enable_depth_test();

/**
 * Disable depth test when rendering objects.
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
 * Enable stencil test when rendering objects.
 */
void enable_stencil_test();

/**
 * Disable stencil test when rendering objects.
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
 * Returns the operation when the stencil test fails.
 */
int get_stencil_fail();

/**
 * Returns the operation when the stencil test passes but depth test fails.
 */
int get_stencil_zfail();

/**
 * Returns the operation when both the stencil test and depth test pass.
 */
int get_stencil_zpass();

/**
 * Sets the operations in stencil test.
 *
 * \param f the operation when the stencil test fails
 * \param zf the operation when the stencil test passes but depth test fails
 * \param zp the operation when both the stencil test and depth test pass
 */
void set_stencil_op(int f, int zf, int zp);

/**
 * Enable blending when rendering objects.
 */
void enable_blending();

/**
 * Disable blending when rendering objects.
 */
void disable_blending();

/**
 * Returns the RGB blend operation.
 */
int get_blend_op();

/**
 * Returns the alpha blend operation.
 */
int get_blend_op_alpha();

/**
 * Sets the blend operation when applying blending.
 *
 * \param rgb RGB blend operation
 * \param a alpha blend operation
 */
void set_blend_op(int rgb, int a);

/**
 * Returns the RGB source blend factor.
 */
int get_blend_src();

/**
 * Returns the alpha source blend factor.
 */
int get_blend_src_alpha();

/**
 * Returns the RGB source destination factor.
 */
int get_blend_dst();

/**
 * Returns the alpha source destination factor.
 */
int get_blend_dst_alpha();

/**
 * Sets the blend factors when applying blending.
 *
 * \param srgb RGB source blend function
 * \param drgb RGB source destination function
 * \param sa alpha source blend function
 * \param da alpha source destination function
 */
void set_blend_factor(int srgb, int drgb, int sa, int da);

/**
 * Enable alpha test when rendering objects.
 */
void enable_scissor_test();

/**
 * Disable alpha test when rendering objects.
 */
void disable_scissor_test();

/**
 * Returns the current scissor region.
 */
Rect get_scissor();

/**
 * Sets the current scissor region.
 *
 * \param s scissor region
 */
void set_scissor(const Rect& s);

/**
 * Enable wireframe when rendering objects.
 */
void enable_wireframe();

/**
 * Disable wireframe when rendering objects.
 */
void disable_wireframe();

/**
 * Enable face culling when rendering objects.
 */
void enable_cull_face();

/**
 * Disable face culling when rendering objects.
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
 * Returns the current viewport region.
 */
Rect get_viewport();

/**
 * Sets the current viewport region.
 *
 * \param v viewport
 */
void set_viewport(const Rect& v);

/**
 * Enable multisample when rendering objects.
 */
void enable_multisample();

/**
 * Disable multisample when rendering objects.
 */
void disable_multisample();

/**
 * Enable seamless cube texture accesses.
 */
void enable_texture_cube_seamless();

/**
 * Disable seamless cube texture accesses.
 */
void disable_texture_cube_seamless();

/**
 * Apply the depth setting of material.
 *
 * \param m material
 */
void apply_material_depth(const Material& m);

/**
 * Apply the stencil setting of material.
 *
 * \param m material
 */
void apply_material_stencil(const Material& m);

/**
 * Apply the blending setting of material.
 *
 * \param m material
 */
void apply_material_blending(const Material& m);

/**
 * Apply the wireframe setting of material.
 *
 * \param m material
 */
void apply_material_wireframe(const Material& m);

/**
 * Apply the side setting of material.
 *
 * \param m material
 */
void apply_material_side(const Material& m);

/**
 * Apply the shadow side setting of material.
 *
 * \param m material
 */
void apply_material_shadow_side(const Material& m);

/**
 * Print errors from OpenGL.
 */
std::string get_error();

class Shader {
public:
	/**
	 * Create a new Shader.
	 */
	Shader();
	
	/**
	 * Delete Shader.
	 */
	~Shader();
	
	/**
	 * Load the vertex shader.
	 *
	 * \param s vertex shader
	 */
	void load_vert(const char* s);
	
	/**
	 * Load the vertex shader.
	 *
	 * \param s vertex shader
	 */
	void load_vert(const std::string& s);
	
	/**
	 * Load the geometry shader.
	 *
	 * \param s geometry shader
	 */
	void load_geom(const char* s);
	
	/**
	 * Load the geometry shader.
	 *
	 * \param s geometry shader
	 */
	void load_geom(const std::string& s);
	
	/**
	 * Load the fragment shader.
	 *
	 * \param s fragment shader
	 */
	void load_frag(const char* s);
	
	/**
	 * Load the fragment shader.
	 *
	 * \param s fragment shader
	 */
	void load_frag(const std::string& s);
	
	/**
	 * Load the vertex shader and fragment shader.
	 *
	 * \param v vertex shader
	 * \param f fragment shader
	 */
	void load(const char* v, const char* f);
	
	/**
	 * Load the vertex shader, fragment shader and geometry shader.
	 *
	 * \param v vertex shader
	 * \param f fragment shader
	 * \param g geometry shader
	 */
	void load(const char* v, const char* f, const char* g);
	
	/**
	 * Load the vertex shader and fragment shader.
	 *
	 * \param v vertex shader
	 * \param f fragment shader
	 */
	void load(const std::string& v, const std::string& f);
	
	/**
	 * Load the vertex shader, fragment shader and geometry shader.
	 *
	 * \param v vertex shader
	 * \param f fragment shader
	 * \param g geometry shader
	 */
	void load(const std::string& v, const std::string& f, const std::string& g);
	
	/**
	 * Load the vertex shader from GLSL file.
	 *
	 * \param s vertex shader file
	 */
	int load_vert_file(const std::string& s);
	
	/**
	 * Load the geometry shader from GLSL file.
	 *
	 * \param s geometry shader file
	 */
	int load_geom_file(const std::string& s);
	
	/**
	 * Load the fragment shader from GLSL file.
	 *
	 * \param s fragment shader file
	 */
	int load_frag_file(const std::string& s);
	
	/**
	 * Load the vertex shader and fragment shader from GLSL files.
	 *
	 * \param v vertex shader file
	 * \param f fragment shader file
	 */
	int load_files(const std::string& v, const std::string& f);
	
	/**
	 * Load the vertex shader, fragment shader and geometry shader from GLSL
	 * files.
	 *
	 * \param v vertex shader file
	 * \param f fragment shader file
	 * \param g geometry shader file
	 */
	int load_files(const std::string& v, const std::string& f, const std::string& g);
	
	/**
	 * Compile shader program.
	 */
	void compile();
	
	/**
	 * Use the program of shader.
	 */
	void use_program() const;
	
	/**
	 * Sets the define directives to shader.
	 *
	 * \param d define directives
	 */
	void set_define(const Defines& d);
	
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
	void set_uniform_v2(const std::string& n, const Vec2& v) const;
	
	/**
	 * Sets a value for the specified uniform variable.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_v3(const std::string& n, const Vec3& v) const;
	
	/**
	 * Sets a value for the specified uniform variable.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_v4(const std::string& n, const Vec4& v) const;
	
	/**
	 * Sets a value for the specified uniform variable.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_m2(const std::string& n, const Mat2& v) const;
	
	/**
	 * Sets a value for the specified uniform variable.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_m3(const std::string& n, const Mat3& v) const;
	
	/**
	 * Sets a value for the specified uniform variable.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_m4(const std::string& n, const Mat4& v) const;
	
	/**
	 * Sets values for the specified uniform variables.
	 *
	 * \param u uniform variables
	 */
	void set_uniforms(const Uniforms& u) const;
	
	/**
	 * Sets the path of included shaders. Default is "ink/shaders/inc/".
	 *
	 * \param p include path
	 */
	static void set_include_path(const std::string& p);
	
	/**
	 * Sets the GLSL version of shaders. Default is "410".
	 */
	static void set_glsl_version(const std::string& v);

private:
	uint32_t program = 0;
	
	std::string vert_shader;
	std::string geom_shader;
	std::string frag_shader;
	std::string defines;
	std::string recent_defines = "UNINITIALIZED";
	
	static std::string include_path;
	static std::string glsl_version;
	
	Shader(const Shader&) = delete;
	
	Shader& operator=(const Shader&) = delete;
	
	uint32_t compile_shader(const std::string& s, int32_t t) const;
	
	void compile_shaders() const;
	
	std::string get_compile_info(uint32_t s, uint32_t t) const;
	
	std::string get_link_info() const;
	
	static std::string get_error_info(const std::string& c, const std::string& s);
	
	void resolve_defines(std::string& s) const;
	
	static void resolve_include(std::string& s);
	
	static void resolve_version(std::string& s);
	
	friend class VertexObject;
};

class BufferObject {
public:
	/**
	 * Create a new BufferObject.
	 */
	BufferObject();
	
	/**
	 * Delete BufferObject.
	 */
	~BufferObject();
	
private:
	uint32_t id = 0;
	
	BufferObject(const BufferObject&) = delete;
	
	BufferObject& operator=(const BufferObject&) = delete;
	
	friend class VertexObject;
};

class VertexObject {
public:
	/**
	 * Create a new VertexObject.
	 */
	VertexObject();
	
	/**
	 * Delete VertexObject.
	 */
	~VertexObject();
	
	/**
	 * Load mesh to vertex object.
	 *
	 * \param m mesh
	 * \param g material group index
	 */
	void load(const Mesh& m, unsigned int g = 0);
	
	/**
	 * Attach vertex object to target shader in order to match the input data
	 * with the shader locations automatically.
	 *
	 * \param s target shader
	 */
	void attach(const Shader& s) const;
	
	/**
	 * Draw vertex object.
	 */
	void draw() const;
	
private:
	uint32_t id = 0;
	
	int length = 0;
	
	BufferObject buffer;
	
	std::vector<std::string> names;
	
	std::vector<int> sizes;
	
	std::vector<int> locations;
	
	VertexObject(const VertexObject&) = delete;
	
	VertexObject& operator=(const VertexObject&) = delete;
};

class Texture {
public:
	/**
	 * Create a new Texture.
	 */
	Texture();
	
	/**
	 * Delete Texture.
	 */
	~Texture();
	
	/**
	 * Initialize 1d texture with empty data.
	 *
	 * \param w the width of texture
	 * \param f texture format
	 * \param t image data type
	 */
	void init_1d(int w, int f, int t = IMAGE_UBYTE);
	
	/**
	 * Initialize 2d texture with empty data.
	 *
	 * \param w the width of texture
	 * \param h the height of texture
	 * \param f texture format
	 * \param t image data type
	 */
	void init_2d(int w, int h, int f, int t = IMAGE_UBYTE);
	
	/**
	 * Initialize 2d texture with image.
	 *
	 * \param i image
	 * \param f texture format
	 */
	void init_2d(const Image& i, int f);
	
	/**
	 * Initialize 3d texture with empty data.
	 *
	 * \param w the width of texture
	 * \param h the height of texture
	 * \param d the depth of texture
	 * \param f texture format
	 * \param t image data type
	 */
	void init_3d(int w, int h, int d, int f, int t = IMAGE_UBYTE);
	
	/**
	 * Initialize cube texture with empty data.
	 *
	 * \param w the width of texture
	 * \param h the height of texture
	 * \param f texture format
	 * \param t image data type
	 */
	void init_cube(int w, int h, int f, int t = IMAGE_UBYTE);
	
	/**
	 * Initialize cube texture with images.
	 *
	 * \param px right (+X) face of cube image
	 * \param nx left  (-X) face of cube image
	 * \param py upper (+Y) face of cube image
	 * \param ny lower (-Y) face of cube image
	 * \param pz front (+Z) face of cube image
	 * \param nz back  (-Z) face of cube image
	 * \param f texture format
	 */
	void init_cube(const Image& px, const Image& nx, const Image& py,
				   const Image& ny, const Image& pz, const Image& nz, int f);
	
	/**
	 * Initialize 1D array texture with empty data.
	 *
	 * \param w the width of texture
	 * \param l the layer of texture
	 * \param f texture format
	 * \param t image data type
	 */
	void init_1d_array(int w, int l, int f, int t = IMAGE_UBYTE);
	
	/**
	 * Initialize 2D array texture with empty data.
	 *
	 * \param w the width of texture
	 * \param h the height of texture
	 * \param l the layer of texture
	 * \param f image format
	 * \param t image data type
	 */
	void init_2d_array(int w, int h, int l, int f, int t = IMAGE_UBYTE);
	
	/**
	 * Initialize cube array texture with empty data.
	 *
	 * \param w the width of texture
	 * \param h the height of texture
	 * \param l the layer of texture
	 * \param f texture format
	 * \param t image data type
	 */
	void init_cube_array(int w, int h, int l, int f, int t = IMAGE_UBYTE);
	
	/**
	 * Returns the type of texture.
	 */
	int get_type() const;
	
	/**
	 * Returns the format of texture.
	 */
	int get_format() const;
	
	/**
	 * Returns the width of texture.
	 */
	int get_width() const;
	
	/**
	 * Returns the height of texture.
	 */
	int get_height() const;
	
	/**
	 * Returns the depth of texture.
	 */
	int get_depth() const;
	
	/**
	 * Returns the layer of texture.
	 */
	int get_layer() const;
	
	/**
	 * Returns the image of texture.
	 */
	Image get_image() const;
	
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
	 * Sets the magnification and minification filters of texture.
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
	void set_border_color(const Vec4& c) const;
	
	/**
	 * Sets the texture active in the specified location.
	 *
	 * \param l the location of texture
	 */
	int activate(int l) const;
	
	/**
	 * Get the default texture format for the specified image.
	 *
	 * \param i image
	 */
	static int default_format(const Image& i);
	
	/**
	 * Get the default texture format for the specified channels and bytes.
	 *
	 * \param c the number of channels
	 * \param b bytes
	 */
	static int default_format(int c, int b);
	
private:
	uint32_t id = 0;
	
	int type = -1;
	int format = -1;
	int width = 0;
	int height = 0;
	int depth = 0;
	
	Texture(const Texture&) = delete;
	
	Texture& operator=(const Texture&) = delete;
	
	void set_parameters(int t, int f, int w, int h, int d);
	
	friend class RenderBuffer;
	
	friend class FrameBuffer;
};

class RenderBuffer {
public:
	/**
	 * Create a new RenderBuffer.
	 */
	RenderBuffer();
	
	/**
	 * Delete RenderBuffer.
	 */
	~RenderBuffer();
	
	/**
	 * Preload render buffer.
	 *
	 * \param w the width of texture
	 * \param h the height of texture
	 * \param f image format
	 */
	void init(int w, int h, int f) const;
	
private:
	uint32_t id = 0;
	
	RenderBuffer(const RenderBuffer&) = delete;
	
	RenderBuffer& operator=(const RenderBuffer&) = delete;
	
	friend class FrameBuffer;
};

class FrameBuffer {
public:
	/**
	 * Create a new FrameBuffer.
	 */
	FrameBuffer();
	
	/**
	 * Delete FrameBuffer.
	 */
	~FrameBuffer();
	
	/**
	 * Sets the frame buffer active.
	 */
	void activate() const;
	
	/**
	 * Sets the frame buffer active.
	 */
	static void activate(const FrameBuffer* f);
	
	/**
	 * Sets which attachments can be drawn into if there is more than one
	 * attachment.
	 *
	 * \param l attachments list
	 */
	void draw_attachments(const std::initializer_list<int>& l) const;
	
	/**
	 * Disable draw into color buffers.
	 */
	void disable_draw() const;
	
	/**
	 * Sets the texture as attachment.
	 *
	 * \param t 2D texture, cube texture, 2D texture array or 3D texture
	 * \param i the index of attachment
	 * \param l mipmap level
	 * \param p if texture is cube texture, specifies the face of cube texture
	 *          if texture is 2D texture array, specifies the layer of texture
	 *          if texture is 3D texture, specifies the layer of texture
	 */
	void set_attachment(const Texture& t, unsigned int i, int l = 0, int p = 0) const;
	
	/**
	 * Sets the render buffer as attachment.
	 *
	 * \param r render buffer
	 * \param i the index of attachment
	 */
	void set_attachment(const RenderBuffer& r, unsigned int i) const;
	
	/**
	 * Sets the 2D texture as depth attachment.
	 *
	 * \param t 2D texture or cube texture
	 * \param l mipmap level
	 * \param p if texture is cube texture, specifies the face of cube texture
	 *          if texture is 2D texture array, specifies the layer of texture
	 *          if texture is 3D texture, specifies the layer of texture
	 */
	void set_depth_attachment(const Texture& t, int l = 0, int p = 0) const;
	
	/**
	 * Sets the render buffer as depth attachment.
	 *
	 * \param r render buffer
	 */
	void set_depth_attachment(const RenderBuffer& r) const;
	
	/**
	 * Sets the texture as depth stencil attachment.
	 *
	 * \param t 2D texture or cube texture
	 * \param l mipmap level
	 * \param p if texture is cube texture, specifies the face of cube texture
	 *          if texture is 2D texture array, specifies the layer of texture
	 *          if texture is 3D texture, specifies the layer of texture
	 */
	void set_depth_stencil_attachment(const Texture& t, int l = 0, int p = 0) const;
	
	/**
	 * Sets the render buffer as depth stencil attachment.
	 *
	 * \param r render buffer
	 */
	void set_depth_stencil_attachment(const RenderBuffer& r) const;
	
private:
	uint32_t id = 0;
	
	FrameBuffer(const FrameBuffer&) = delete;
	
	FrameBuffer& operator=(const FrameBuffer&) = delete;
	
	void set_texture_attachment(const Texture& t, uint32_t a, int l, int p) const;
	
	void set_render_buffer_attachment(const RenderBuffer& r, uint32_t a) const;
};

}
