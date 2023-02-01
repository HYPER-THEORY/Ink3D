/**
 * Copyright (C) 2021-2023 Hypertheory
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

#include "../objects/Enums.h"
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
	 * Creates a new Rect object.
	 */
	explicit Rect() = default;
	
	/**
	 * Creates a new Rect object and initializes it with size.
	 *
	 * \param w the width of the rectangle
	 * \param h the height of the rectangle
	 */
	explicit Rect(int w, int h);
	
	/**
	 * Creates a new Rect object and initializes it with size and position.
	 *
	 * \param x the x-coordinate of left lower corner
	 * \param y the y-coordinate of left lower corner
	 * \param w the width of the rectangle
	 * \param h the height of the rectangle
	 */
	explicit Rect(int x, int y, int w, int h);
};

class State {
public:
	/**
	 * Returns a string describing the current device.
	 */
	static std::string get_device_info();
	
	/**
	 * Blocks until all GPU commands is complete.
	 */
	static void finish();
	
	/**
	 * Forces execution of GPU commands in finite time.
	 */
	static void flush();
	
	/**
	 * Prints all error information from OpenGL.
	 */
	static std::string get_error();
	
	/**
	 * Clears the color, depth or stencil drawing buffers. Initializes the color
	 * buffer to the current clear color value.
	 *
	 * \param c whether to clear the color buffer
	 * \param d whether to clear the depth buffer
	 * \param s whether to clear the stencil buffer
	 */
	static void clear(bool c = true, bool d = true, bool s = true);
	
	/**
	 * Returns the clear color (.xyz) and opacity (.w).
	 */
	static Vec4 get_clear_color();
	
	/**
	 * Sets the specified clear color and opacity. Default is (1, 1, 1) and 1.
	 *
	 * \param c clear color
	 * \param a clear opacity
	 */
	static void set_clear_color(const Vec3& c, float a);
	
	/**
	 * Sets the specified clear color (.xyz) and opacity (.w). Default is (1, 1,
	 * 1, 1).
	 *
	 * \param c clear color and opacity
	 */
	static void set_clear_color(const Vec4& c);
	
	/**
	 * Returns a vec4 value represents the red, green, blue and alpha components
	 * are enabled for writing.
	 */
	static Vec4 get_color_writemask();
	
	/**
	 * Determines whether the red, green, blue and alpha components are enabled
	 * for writing. Default is true, true, true, true.
	 *
	 * \param r red writemask
	 * \param g green writemask
	 * \param b blur writemask
	 * \param a alpha writemask
	 */
	static void set_color_writemask(bool r, bool g, bool b, bool a);
	
	/**
	 * Enables depth test. If enabled, do depth comparisons and update the depth
	 * buffer.
	 */
	static void enable_depth_test();
	
	/**
	 * Disables depth test. If enabled, do depth comparisons and update the
	 * depth buffer.
	 */
	static void disable_depth_test();
	
	/**
	 * Returns the clear value for depth buffer.
	 */
	static double get_clear_depth();
	
	/**
	 * Sets the specified clear value for depth buffer. Default is 1.
	 */
	static void set_clear_depth(double d);
	
	/**
	 * Returns true if the depth buffer is enabled for writing in depth test.
	 */
	static bool get_depth_writemask();
	
	/**
	 * Determines whether the depth buffer is enabled for writing in depth test.
	 * Default is true.
	 *
	 * \param m depth writemask
	 */
	static void set_depth_writemask(bool m);
	
	/**
	 * Returns the comparison function in depth test.
	 */
	static int get_depth_func();
	
	/**
	 * Sets the specified comparison function in depth test. Default is
	 * FUNC_LESS.
	 *
	 * \param f comparison function
	 */
	static void set_depth_func(int f);
	
	/**
	 * Enables stencil test. If enabled, do stencil testing and update the
	 * stencil buffer.
	 */
	static void enable_stencil_test();
	
	/**
	 * Disables stencil test. If enabled, do stencil testing and update the
	 * stencil buffer.
	 */
	static void disable_stencil_test();
	
	/**
	 * Returns the clear value for stencil buffer.
	 */
	static int get_clear_stencil();
	
	/**
	 * Sets the specified clear value for stencil buffer. Default is 0.
	 */
	static void set_clear_stencil(int s);
	
	/**
	 * Returns true if the stencil buffer bits are enabled for writing in
	 * stencil test.
	 */
	static unsigned int get_stencil_writemask();
	
	/**
	 * Determines whether the stencil buffer bits are enabled for writing in
	 * stencil test. Default is all 1's.
	 *
	 * \param m stencil writemask
	 */
	static void set_stencil_writemask(unsigned int m);
	
	/**
	 * Returns the comparison function in stencil test.
	 */
	static int get_stencil_func();
	
	/**
	 * Returns the reference value in stencil test.
	 */
	static int get_stencil_ref();
	
	/**
	 * Returns the mask in stencil test.
	 */
	static int get_stencil_mask();
	
	/**
	 * Sets the specified comparison function, reference value and mask in
	 * stencil test. Default is FUNC_ALWAYS, 0 and 1.
	 *
	 * \param f comparison function
	 * \param r reference value
	 * \param m mask
	 */
	static void set_stencil_func(int f, int r, int m);
	
	/**
	 * Returns the operation when the stencil test fails.
	 */
	static int get_stencil_fail();
	
	/**
	 * Returns the operation when the stencil test passes but depth test fails.
	 */
	static int get_stencil_zfail();
	
	/**
	 * Returns the operation when both the stencil test and depth test pass.
	 */
	static int get_stencil_zpass();
	
	/**
	 * Sets the specified operations for different cases in stencil test and
	 * depth test. Default is STENCIL_KEEP, STENCIL_KEEP, STENCIL_KEEP.
	 *
	 * \param f the operation when the stencil test fails
	 * \param zf the operation when the stencil test passes but depth test fails
	 * \param zp the operation when both the stencil test and depth test pass
	 */
	static void set_stencil_op(int f, int zf, int zp);
	
	/**
	 * Enables blending. If enabled, blend the computed fragment color values
	 * with the values in the color buffers.
	 */
	static void enable_blending();
	
	/**
	 * Disables blending. If enabled, blend the computed fragment color values
	 * with the values in the color buffers.
	 */
	static void disable_blending();
	
	/**
	 * Returns the blend operation of RGB color components in blending.
	 */
	static int get_blend_op_rgb();
	
	/**
	 * Returns the blend operation of alpha color component in blending.
	 */
	static int get_blend_op_alpha();
	
	/**
	 * Sets the specified blend operation in blending. Default is BLEND_ADD.
	 *
	 * \param o blend operation
	 */
	static void set_blend_op(int o);
	
	/**
	 * Sets the specified blend operation in blending. Default is BLEND_ADD,
	 * BLEND_ADD.
	 *
	 * \param rgb RGB blend operation
	 * \param a alpha blend operation
	 */
	static void set_blend_op(int rgb, int a);
	
	/**
	 * Returns the source blend factor of RGB color components in blending.
	 */
	static int get_blend_src_rgb();
	
	/**
	 * Returns the source blend factor of alpha color component in blending.
	 */
	static int get_blend_src_alpha();
	
	/**
	 * Returns the source destination factor of RGB color components in
	 * blending.
	 */
	static int get_blend_dst_rgb();
	
	/**
	 * Returns the source destination factor of alpha color component in
	 * blending.
	 */
	static int get_blend_dst_alpha();
	
	/**
	 * Sets the specified source and destination blend factors in blending.
	 * Default is FACTOR_ONE and FACTOR_ZERO.
	 *
	 * \param s source blend function
	 * \param d destination blend function
	 */
	static void set_blend_factor(int s, int d);
	
	/**
	 * Sets the specified source and destination blend factors in blending.
	 * Default is FACTOR_ONE, FACTOR_ZERO, FACTOR_ONE, FACTOR_ZERO.
	 *
	 * \param srgb RGB source blend function
	 * \param drgb RGB destination blend function
	 * \param sa alpha source blend function
	 * \param da alpha destination blend function
	 */
	static void set_blend_factor(int srgb, int drgb, int sa, int da);
	
	/**
	 * Returns the viewport region.
	 */
	static Rect get_viewport();
	
	/**
	 * Sets the viewport region to render from (x, y) to (x + width, y +
	 * height).
	 *
	 * \param v viewport region
	 */
	static void set_viewport(const Rect& v);
	
	/**
	 * Enables scissor test. If enabled, discard fragments that are outside the
	 * scissor rectangle.
	 */
	static void enable_scissor_test();
	
	/**
	 * Disables scissor test. If enabled, discard fragments that are outside the
	 * scissor rectangle.
	 */
	static void disable_scissor_test();
	
	/**
	 * Returns the scissor region in scissor test.
	 */
	static Rect get_scissor();
	
	/**
	 * Sets the scissor region from (x, y) to (x + width, y + height) in scissor
	 * test.
	 *
	 * \param s scissor region
	 */
	static void set_scissor(const Rect& s);
	
	/**
	 * Enables wireframe. If enabled, boundary edges of the polygon are drawn as
	 * line segments.
	 */
	static void enable_wireframe();
	
	/**
	 * Disables wireframe. If enabled, boundary edges of the polygon are drawn
	 * as line segments.
	 */
	static void disable_wireframe();
	
	/**
	 * Enables face culling. If enabled, cull polygons based on their winding in
	 * window coordinates.
	 */
	static void enable_culling();
	
	/**
	 * Disables face culling. If enabled, cull polygons based on their winding
	 * in window coordinates.
	 */
	static void disable_culling();
	
	/**
	 * Determines which side of face will be culled.
	 */
	static int get_cull_side();
	
	/**
	 * Determines which side of face will be culled. Default is BACK_SIDE.
	 *
	 * \param s culling side
	 */
	static void set_cull_side(int s);
	
	/**
	 * Enables polygon offset. If enabled, and if the wireframe is disabled, an
	 * offset is added to depth values of a polygon's fragments before the depth
	 * comparison is performed.
	 */
	static void enable_polygon_offset();
	
	/**
	 * Disables polygon offset. If enabled, and if the wireframe is disabled, an
	 * offset is added to depth values of a polygon's fragments before the depth
	 * comparison is performed.
	 */
	static void disable_polygon_offset();
	
	/**
	 * Returns the polygon offset factor.
	 */
	static float get_polygon_offset_factor();
	
	/**
	 * Returns the polygon offset units.
	 */
	static float get_polygon_offset_units();
	
	/**
	 * Sets the specified polygon offset factor and units. Default is 0 and 0.
	 *
	 * \param f polygon offset factor
	 * \param u polygon offset units
	 */
	static void set_polygon_offset(float f, float u);
	
	/**
	 * Enables dithering. If enabled, dither color components or indices before
	 * they are written to the color buffer.
	 */
	static void enable_dithering();
	
	/**
	 * Disables dithering. If enabled, dither color components or indices before
	 * they are written to the color buffer.
	 */
	static void disable_dithering();
	
	/**
	 * Enables multisample anti-aliasing. If enabled, use multiple fragment
	 * samples in computing the final color of a pixel.
	 */
	static void enable_multisample();
	
	/**
	 * Disables multisample anti-aliasing. If enabled, use multiple fragment
	 * samples in computing the final color of a pixel.
	 */
	static void disable_multisample();
	
	/**
	 * Enables alpha to coverage. If enabled, compute a temporary coverage value
	 * where each bit is determined by the alpha value at the corresponding
	 * sample location.
	 */
	static void enable_alpha_to_coverage();
	
	/**
	 * Disables alpha to coverage. If enabled, compute a temporary coverage
	 * value where each bit is determined by the alpha value at the
	 * corresponding sample location.
	 */
	static void disable_alpha_to_coverage();
	
	/**
	 * Enables seamless cube texture accesses. If enabled, cubemap textures are
	 * sampled such that when linearly sampling from the border between two
	 * adjacent faces, texels from both faces are used to generate the final
	 * sample value.
	 */
	static void enable_texture_cube_seamless();
	
	/**
	 * Disables seamless cube texture accesses. If enabled, cubemap textures are
	 * sampled such that when linearly sampling from the border between two
	 * adjacent faces, texels from both faces are used to generate the final
	 * sample value.
	 */
	static void disable_texture_cube_seamless();
};

class MaterialState {
public:
	/**
	 * Applies the depth dependent settings for the specified material.
	 *
	 * \param m material
	 */
	static void set_depth(const Material& m);
	
	/**
	 * Applies the stencil dependent settings for the specified material.
	 *
	 * \param m material
	 */
	static void set_stencil(const Material& m);
	
	/**
	 * Applies the blending dependent settings for the specified material.
	 *
	 * \param m material
	 */
	static void set_blending(const Material& m);
	
	/**
	 * Applies the wireframe dependent settings for the specified material.
	 *
	 * \param m material
	 */
	static void set_wireframe(const Material& m);
	
	/**
	 * Applies the side dependent settings for the specified material.
	 *
	 * \param m material
	 */
	static void set_side(const Material& m);
	
	/**
	 * Applies the shadow side dependent settings for the specified material.
	 *
	 * \param m material
	 */
	static void set_shadow_side(const Material& m);
};

class Shader {
public:
	/**
	 * Creates a new Shader object.
	 */
	explicit Shader();
	
	/**
	 * Deletes this Shader object.
	 */
	~Shader();
	
	/**
	 * The copy constructor is deleted.
	 */
	Shader(const Shader&) = delete;
	
	/**
	 * The copy assignment operator is deleted.
	 */
	Shader& operator=(const Shader&) = delete;
	
	/**
	 * Loads the specified string of vertex shader content to the shader.
	 *
	 * \param s vertex shader
	 */
	void load_vert(const char* s);
	
	/**
	 * Loads the specified string of vertex shader content to the shader.
	 *
	 * \param s vertex shader
	 */
	void load_vert(const std::string& s);
	
	/**
	 * Loads the specified string of geometry shader content to the shader.
	 *
	 * \param s geometry shader
	 */
	void load_geom(const char* s);
	
	/**
	 * Loads the specified string of geometry shader content to the shader.
	 *
	 * \param s geometry shader
	 */
	void load_geom(const std::string& s);
	
	/**
	 * Loads the specified string of fragment shader content to the shader.
	 *
	 * \param s fragment shader
	 */
	void load_frag(const char* s);
	
	/**
	 * Loads the specified string of fragment shader content to the shader.
	 *
	 * \param s fragment shader
	 */
	void load_frag(const std::string& s);
	
	/**
	 * Loads the vertex shader from the specified GLSL file to the shader.
	 *
	 * \param p the path to vertex shader file
	 */
	void load_vert_file(const std::string& p);
	
	/**
	 * Loads the geometry shader from the specified GLSL file to the shader.
	 *
	 * \param p the path to geometry shader file
	 */
	void load_geom_file(const std::string& p);
	
	/**
	 * Loads the fragment shader from the specified GLSL file to the shader.
	 *
	 * \param p the path to fragment shader file
	 */
	void load_frag_file(const std::string& p);
	
	/**
	 * Compile the program of shader if the shader has changed.
	 */
	void compile() const;
	
	/**
	 * Use the program of the compiled shader.
	 */
	void use_program() const;
	
	/**
	 * Sets the define directives to the shader. These values will be defined in
	 * vertex, geometry and fragment shaders.
	 *
	 * \param d defines
	 */
	void set_defines(const Defines& d);
	
	/**
	 * Sets the specified value for the specified uniform variable. These values
	 * will be passed to vertex, geometry and fragment shaders.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_i(const std::string& n, int v) const;
	
	/**
	 * Sets the specified value for the specified uniform variable. These values
	 * will be passed to vertex, geometry and fragment shaders.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_u(const std::string& n, unsigned int v) const;
	
	/**
	 * Sets the specified value for the specified uniform variable. These values
	 * will be passed to vertex, geometry and fragment shaders.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_f(const std::string& n, float v) const;
	
	/**
	 * Sets the specified value for the specified uniform variable. These values
	 * will be passed to vertex, geometry and fragment shaders.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_v2(const std::string& n, const Vec2& v) const;
	
	/**
	 * Sets the specified value for the specified uniform variable. These values
	 * will be passed to vertex, geometry and fragment shaders.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_v3(const std::string& n, const Vec3& v) const;
	
	/**
	 * Sets the specified value for the specified uniform variable. These values
	 * will be passed to vertex, geometry and fragment shaders.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_v4(const std::string& n, const Vec4& v) const;
	
	/**
	 * Sets the specified value for the specified uniform variable. These values
	 * will be passed to vertex, geometry and fragment shaders.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_m2(const std::string& n, const Mat2& v) const;
	
	/**
	 * Sets the specified value for the specified uniform variable. These values
	 * will be passed to vertex, geometry and fragment shaders.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_m3(const std::string& n, const Mat3& v) const;
	
	/**
	 * Sets the specified value for the specified uniform variable. These values
	 * will be passed to vertex, geometry and fragment shaders.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_m4(const std::string& n, const Mat4& v) const;
	
	/**
	 * Sets the specified uniforms object to the shader. These values will be
	 * passed to vertex, geometry and fragment shaders.
	 *
	 * \param u uniforms object
	 */
	void set_uniforms(const Uniforms& u) const;
	
	/**
	 * Returns the GLSL version of the shading language.
	 */
	static std::string get_glsl_version();
	
	/**
	 * Sets the GLSL version of the shading language. Default is "410".
	 *
	 * \param v GLSL version
	 */
	static void set_glsl_version(const std::string& v);

private:
	uint32_t program = 0;
	
	std::string defines;
	std::string vert_shader;
	std::string geom_shader;
	std::string frag_shader;
	
	static std::string glsl_version;
	
	uint32_t compile_shader(const std::string& s, int32_t t) const;
	
	void compile_shaders() const;
	
	std::string get_link_info() const;
	
	void resolve_defines(std::string& s) const;
	
	static void resolve_version(std::string& s);
	
	static std::string get_compile_info(uint32_t s, uint32_t t);
	
	static std::string get_error_info(const std::string& c, const std::string& s);
	
	friend class VertexObject;
};

class VertexObject {
public:
	/**
	 * Creates a new VertexObject object.
	 */
	explicit VertexObject();
	
	/**
	 * Deletes this VertexObject object.
	 */
	~VertexObject();
	
	/**
	 * The copy constructor is deleted.
	 */
	VertexObject(const VertexObject&) = delete;
	
	/**
	 * The copy assignment operator is deleted.
	 */
	VertexObject& operator=(const VertexObject&) = delete;
	
	/**
	 * Loads the specified mesh to this vertex object.
	 *
	 * \param m mesh
	 * \param g material group
	 */
	void load(const Mesh& m, const MeshGroup& g);
	
	/**
	 * Attaches vertex object to the target shader to automatically match the
	 * input data with the shader locations.
	 *
	 * \param s target shader
	 */
	void attach(const Shader& s) const;
	
	/**
	 * Renders the vertex object to the current render target.
	 */
	void render() const;
	
private:
	uint32_t id = 0;
	
	uint32_t buffer_id = 0;
	
	int length = 0;
	
	std::vector<std::string> names;
	
	std::vector<int> sizes;
	
	std::vector<int> locations;
};

class Texture {
public:
	/**
	 * Creates a new Texture object.
	 */
	explicit Texture();
	
	/**
	 * Deletes this Texture object.
	 */
	~Texture();
	
	/**
	 * The copy constructor is deleted.
	 */
	Texture(const Texture&) = delete;
	
	/**
	 * The copy assignment operator is deleted.
	 */
	Texture& operator=(const Texture&) = delete;
	
	/**
	 * Initializes the texture as 1d texture with empty data.
	 *
	 * \param w the width of texture
	 * \param f texture format
	 * \param t image data type
	 */
	void init_1d(int w, int f, int t = IMAGE_UBYTE);
	
	/**
	 * Initializes the texture as 2d texture with empty data.
	 *
	 * \param w the width of texture
	 * \param h the height of texture
	 * \param f texture format
	 * \param t image data type
	 */
	void init_2d(int w, int h, int f, int t = IMAGE_UBYTE);
	
	/**
	 * Initializes the texture as 2d texture with the specified image.
	 *
	 * \param i image
	 * \param f texture format
	 */
	void init_2d(const Image& i, int f);
	
	/**
	 * Initializes the texture as 3d texture with empty data.
	 *
	 * \param w the width of texture
	 * \param h the height of texture
	 * \param d the depth of texture
	 * \param f texture format
	 * \param t image data type
	 */
	void init_3d(int w, int h, int d, int f, int t = IMAGE_UBYTE);
	
	/**
	 * Initializes the texture as cube texture with empty data.
	 *
	 * \param w the width of texture
	 * \param h the height of texture
	 * \param f texture format
	 * \param t image data type
	 */
	void init_cube(int w, int h, int f, int t = IMAGE_UBYTE);
	
	/**
	 * Initializes the texture as cube texture with the specified images from
	 * different perspectives.
	 *
	 * \param px right (+X) side of cube image
	 * \param nx left  (-X) side of cube image
	 * \param py upper (+Y) side of cube image
	 * \param ny lower (-Y) side of cube image
	 * \param pz front (+Z) side of cube image
	 * \param nz back  (-Z) side of cube image
	 * \param f texture format
	 */
	void init_cube(const Image& px, const Image& nx, const Image& py,
				   const Image& ny, const Image& pz, const Image& nz, int f);
	
	/**
	 * Initializes the texture as 1D array texture with empty data.
	 *
	 * \param w the width of texture
	 * \param l the layer of texture
	 * \param f texture format
	 * \param t image data type
	 */
	void init_1d_array(int w, int l, int f, int t = IMAGE_UBYTE);
	
	/**
	 * Initializes the texture as 2D array texture with empty data.
	 *
	 * \param w the width of texture
	 * \param h the height of texture
	 * \param l the layer of texture
	 * \param f image format
	 * \param t image data type
	 */
	void init_2d_array(int w, int h, int l, int f, int t = IMAGE_UBYTE);
	
	/**
	 * Initializes the texture as cube array texture with empty data.
	 *
	 * \param w the width of texture
	 * \param h the height of texture
	 * \param l the layer of texture
	 * \param f texture format
	 * \param t image data type
	 */
	void init_cube_array(int w, int h, int l, int f, int t = IMAGE_UBYTE);
	
	/**
	 * Returns the type of texture if the texture is legal, returns -1
	 * otherwise.
	 */
	int get_type() const;
	
	/**
	 * Returns the format of texture if the texture is legal, returns -1
	 * otherwise.
	 */
	int get_format() const;
	
	/**
	 * Returns the width of texture if the texture is legal, returns 0
	 * otherwise.
	 */
	int get_width() const;
	
	/**
	 * Returns the height of texture if the texture is legal, returns 0
	 * otherwise.
	 */
	int get_height() const;
	
	/**
	 * Returns the depth of texture if the texture is legal, returns 0
	 * otherwise.
	 */
	int get_depth() const;
	
	/**
	 * Returns the layer of texture if the texture is legal, returns 0
	 * otherwise. The type of texture must be one of TEXTURE_1D_ARRAY,
	 * TEXTURE_2D_ARRAY, TEXTURE_CUBE_ARRAY.
	 */
	int get_layer() const;
	
	/**
	 * Copys texture to the specified image. The type of texture must be
	 * TEXTURE_2D.
	 */
	void copy_to_image(Image& i) const;
	
	/**
	 * Generates mipmap for the texture.
	 */
	void generate_mipmap() const;
	
	/**
	 * Sets the wrapping mode on S coordinate. Default is TEXTURE_REPEAT.
	 *
	 * \param m wrapping mode
	 */
	void set_wrap_s(int m) const;
	
	/**
	 * Sets the wrapping mode on T coordinate. Default is TEXTURE_REPEAT.
	 *
	 * \param m wrapping mode
	 */
	void set_wrap_t(int m) const;
	
	/**
	 * Sets the wrapping mode on R coordinate. Default is TEXTURE_REPEAT.
	 *
	 * \param m wrapping mode
	 */
	void set_wrap_r(int m) const;
	
	/**
	 * Sets the wrapping modes on S, T and R coordinates.
	 *
	 * \param m wrapping mode
	 */
	void set_wrap_all(int m) const;
	
	/**
	 * Sets the magnification and minification filters of texture. Default is
	 * TEXTURE_LINEAR and TEXTURE_NEAREST_MIPMAP_LINEAR.
	 *
	 * \param mag magnification filter
	 * \param min minification filter
	 */
	void set_filters(int mag, int min) const;
	
	/**
	 * Sets the border color used for border texels. Default is (0, 0, 0, 0).
	 *
	 * \param c border color
	 */
	void set_border_color(const Vec4& c) const;
	
	/**
	 * Sets the range of LOD for mipmap texture. Default is 0 and 1000.
	 *
	 * \param min lowest defined level
	 * \param max highest defines level
	 */
	void set_lod_range(int min, int max) const;
	
	/**
	 * Sets the bias of LOD for mipmap texture. Default is 0.
	 *
	 * \param b bias level
	 */
	void set_lod_bias(int b) const;
	
	/**
	 * Sets the texture active in the specified location.
	 *
	 * \param l the location of texture
	 */
	int activate(int l) const;
	
	/**
	 * Returns the default texture format for the specified image.
	 *
	 * \param i image
	 */
	static int default_format(const Image& i);
	
	/**
	 * Returns the default texture format for the specified channel and byte.
	 *
	 * \param c channel
	 * \param b byte
	 */
	static int default_format(int c, int b);
	
private:
	uint32_t id = 0;
	
	int type = -1;
	
	int format = -1;
	
	int width = 0;
	
	int height = 0;
	
	int depth = 0;
	
	void set_parameters(int t, int f, int w, int h, int d);
	
	friend class RenderTarget;
};

class RenderBuffer {
public:
	/**
	 * Creates a new RenderBuffer object.
	 */
	explicit RenderBuffer();
	
	/**
	 * Deletes this RenderBuffer object.
	 */
	~RenderBuffer();
	
	/**
	 * The copy constructor is deleted.
	 */
	RenderBuffer(const RenderBuffer&) = delete;
	
	/**
	 * The copy assignment operator is deleted.
	 */
	RenderBuffer& operator=(const RenderBuffer&) = delete;
	
	/**
	 * Initializes the render buffer with empty data, which is similar to 2D
	 * texture.
	 *
	 * \param w the width of render buffer
	 * \param h the height of render buffer
	 * \param f image format
	 */
	void init(int w, int h, int f) const;
	
private:
	uint32_t id = 0;
	
	friend class RenderTarget;
};

class RenderTarget {
public:
	/**
	 * Creates a new RenderTarget object.
	 */
	explicit RenderTarget();
	
	/**
	 * Deletes this RenderTarget object.
	 */
	~RenderTarget();
	
	/**
	 * The copy constructor is deleted.
	 */
	RenderTarget(const RenderTarget&) = delete;
	
	/**
	 * The copy assignment operator is deleted.
	 */
	RenderTarget& operator=(const RenderTarget&) = delete;
	
	/**
	 * Sets the 2D texture, cube texture, 2D texture array or 3D texture to the
	 * render target.
	 *
	 * \param t 2D texture, cube texture, 2D texture array or 3D texture
	 * \param i the index of texture
	 * \param l mipmap level
	 * \param p if texture is cube texture, specifies the face of cube texture
	 *          if texture is 2D texture array, specifies the layer of texture
	 *          if texture is 3D texture, specifies the layer of texture
	 */
	void set_texture(const Texture& t, unsigned int i, int l = 0, int p = 0) const;
	
	/**
	 * Sets the 2D texture or cube texture as the depth texture of render
	 * target.
	 *
	 * \param t 2D texture or cube texture
	 * \param l mipmap level
	 * \param p if texture is cube texture, specifies the face of cube texture
	 *          if texture is 2D texture array, specifies the layer of texture
	 */
	void set_depth_texture(const Texture& t, int l = 0, int p = 0) const;
	
	/**
	 * Sets the 2D texture or cube texture as the stencil texture of render
	 * target.
	 *
	 * \param t 2D texture or cube texture
	 * \param l mipmap level
	 * \param p if texture is cube texture, specifies the face of cube texture
	 *          if texture is 2D texture array, specifies the layer of texture
	 */
	void set_stencil_texture(const Texture& t, int l = 0, int p = 0) const;
	
	/**
	 * Sets the 2D texture or cube texture as the depth stencil te texture of
	 * render target.
	 *
	 * \param t 2D texture or cube texture
	 * \param l mipmap level
	 * \param p if texture is cube texture, specifies the face of cube texture
	 *          if texture is 2D texture array, specifies the layer of texture
	 */
	void set_depth_stencil_texture(const Texture& t, int l = 0, int p = 0) const;
	
	/**
	 * Sets the render buffer as the depth buffer of render target.
	 *
	 * \param r render buffer
	 */
	void set_depth_buffer(const RenderBuffer& r) const;
	
	/**
	 * Sets the render buffer as the stencil buffer of render target.
	 *
	 * \param r render buffer
	 */
	void set_stencil_buffer(const RenderBuffer& r) const;
	
	/**
	 * Sets the render buffer as the depth stencil buffer of render target.
	 *
	 * \param r render buffer
	 */
	void set_depth_stencil_buffer(const RenderBuffer& r) const;
	
	/**
	 * Sets the number of targets to be rendered into. Default is 1.
	 *
	 * \param n target number
	 */
	void set_target_number(int n) const;
	
	/**
	 * Activates the render target.
	 */
	void activate() const;
	
	/**
	 * Activates the specified render target. If render target is nullptr,
	 * activates the default render target.
	 *
	 * \param t render target
	 */
	static void activate(const RenderTarget* t);
	
private:
	uint32_t id = 0;
	
	std::vector<uint32_t> attachments;
	
	void set_framebuffer(const Texture& t, uint32_t a, int l, int p) const;
	
	void set_framebuffer(const RenderBuffer& r, uint32_t a) const;
};

}
