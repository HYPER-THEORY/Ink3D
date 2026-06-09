/**
 * Copyright (C) 2021-2023 HYPERTHEORY
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

#include "../objects/Defines.h"
#include "../objects/Enums.h"
#include "../objects/Image.h"
#include "../objects/Material.h"
#include "../objects/Mesh.h"
#include "../objects/Uniforms.h"

namespace ink::gpu {

class Rect {
public:
	int x = 0;         /**< the X-coordinate of the lower left corner */
	int y = 0;         /**< the Y-coordinate of the lower left corner */
	int width = 0;     /**< the width of the rectangle */
	int height = 0;    /**< the height of the rectangle */
	
	/**
	 * Creates a new Rect object.
	 */
	Rect() = default;
	
	/**
	 * Creates a new Rect object and initializes it with size.
	 *
	 * \param w the width of the rectangle
	 * \param h the height of the rectangle
	 */
	Rect(int w, int h);
	
	/**
	 * Creates a new Rect object and initializes it with size and position.
	 *
	 * \param x the X-coordinate of the lower left corner
	 * \param y the Y-coordinate of the lower left corner
	 * \param w the width of the rectangle
	 * \param h the height of the rectangle
	 */
	Rect(int x, int y, int w, int h);
};

class State {
public:
	/**
	 * Returns a string describing the current device.
	 */
	static std::string get_device_info();
	
	/**
	 * Blocks until the execution of all GPU commands is complete.
	 */
	static void finish();
	
	/**
	 * Forces the execution of all GPU commands in finite time.
	 */
	static void flush();
	
	/**
	 * Returns the error information from OpenGL.
	 */
	static std::string get_error();
	
	/**
	 * Clears the color, depth or stencil buffers. Fills the color buffer with
	 * the current color clear value.
	 *
	 * \param c whether to clear the color buffer
	 * \param d whether to clear the depth buffer
	 * \param s whether to clear the stencil buffer
	 */
	static void clear(bool c = true, bool d = true, bool s = true);
	
	/**
	 * Returns the color (.xyz) and alpha (.w) for clearing.
	 */
	static Vec4 get_clear_color();
	
	/**
	 * Sets the specified color and alpha for clearing. The default is (1, 1, 1)
	 * and 1.
	 *
	 * \param c color
	 * \param a alpha
	 */
	static void set_clear_color(const Vec3& c, float a);
	
	/**
	 * Sets the specified color (.xyz) and alpha (.w) for clearing. The default
	 * is (1, 1, 1, 1).
	 *
	 * \param c color with alpha
	 */
	static void set_clear_color(const Vec4& c);
	
	/**
	 * Returns a vec4 value that represents whether the red, green, blue, and
	 * alpha components are enabled for writing.
	 */
	static Vec4 get_color_writemask();
	
	/**
	 * Determines whether the red, green, blue, and alpha components are enabled
	 * for writing. The default is true, true, true, true.
	 *
	 * \param r red writemask
	 * \param g green writemask
	 * \param b blue writemask
	 * \param a alpha writemask
	 */
	static void set_color_writemask(bool r, bool g, bool b, bool a);
	
	/**
	 * Enables depth test. If enabled, does depth comparisons and updates the
	 * depth buffer.
	 */
	static void enable_depth_test();
	
	/**
	 * Disables depth test.
	 */
	static void disable_depth_test();
	
	/**
	 * Returns the depth clear value.
	 */
	static double get_clear_depth();
	
	/**
	 * Sets the specified depth clear value. The default is 1.
	 *
	 * \param d clear depth value
	 */
	static void set_clear_depth(double d);
	
	/**
	 * Returns true if the depth buffer is enabled for writing.
	 */
	static bool get_depth_writemask();
	
	/**
	 * Determines whether the depth buffer is enabled for writing. The default
	 * is true.
	 *
	 * \param m depth writemask
	 */
	static void set_depth_writemask(bool m);
	
	/**
	 * Returns the comparison function in depth test.
	 */
	static ComparisonFunc get_depth_func();
	
	/**
	 * Sets the specified comparison function in depth test. The default is
	 * FUNC_LESS.
	 *
	 * \param f comparison function
	 */
	static void set_depth_func(ComparisonFunc f);
	
	/**
	 * Enables stencil test. If enabled, does stencil testing and updates the
	 * stencil buffer.
	 */
	static void enable_stencil_test();
	
	/**
	 * Disables stencil test.
	 */
	static void disable_stencil_test();
	
	/**
	 * Returns the stencil clear value.
	 */
	static int get_clear_stencil();
	
	/**
	 * Sets the specified stencil clear value. The default is 0.
	 *
	 * \param s stencil clear value
	 */
	static void set_clear_stencil(int s);
	
	/**
	 * Returns the stencil bit mask to enable writing of individual bits in the
	 * stencil planes.
	 */
	static unsigned int get_stencil_writemask();
	
	/**
	 * Sets the specified stencil bit mask to enable writing of individual bits
	 * in the stencil planes. The default is all 1's.
	 *
	 * \param m stencil writemask
	 */
	static void set_stencil_writemask(unsigned int m);
	
	/**
	 * Returns the comparison function in stencil test.
	 */
	static ComparisonFunc get_stencil_func();
	
	/**
	 * Returns the reference value in stencil test.
	 */
	static int get_stencil_ref();
	
	/**
	 * Returns the mask in stencil test.
	 */
	static int get_stencil_mask();
	
	/**
	 * Sets the specified comparison function, reference value, and mask in
	 * stencil test. The default is FUNC_ALWAYS, 0 and 1.
	 *
	 * \param f comparison function
	 * \param r reference value
	 * \param m mask
	 */
	static void set_stencil_func(ComparisonFunc f, int r, int m);
	
	/**
	 * Returns the operation when stencil test fails.
	 */
	static StencilOperation get_stencil_fail();
	
	/**
	 * Returns the operation when the stencil test passes but depth test fails.
	 */
	static StencilOperation get_stencil_zfail();
	
	/**
	 * Returns the operation when both stencil test and depth test passes.
	 */
	static StencilOperation get_stencil_zpass();
	
	/**
	 * Sets the specified operations for different cases in stencil test and
	 * depth test. The default is STENCIL_KEEP, STENCIL_KEEP, STENCIL_KEEP.
	 *
	 * \param f the operation when stencil test fails
	 * \param zf the operation when stencil test passes but depth test fails
	 * \param zp the operation when both stencil test and depth test passes
	 */
	static void set_stencil_op(StencilOperation f,
							   StencilOperation zf,
							   StencilOperation zp);
	
	/**
	 * Enables blending. If enabled, blends the computed fragment color values
	 * with the values in the color buffers.
	 */
	static void enable_blending();
	
	/**
	 * Disables blending.
	 */
	static void disable_blending();
	
	/**
	 * Returns the blend operation of the RGB color components in blending.
	 */
	static BlendOperation get_blend_op_rgb();
	
	/**
	 * Returns the blend operation of the alpha color component in blending.
	 */
	static BlendOperation get_blend_op_alpha();
	
	/**
	 * Sets the specified blend operation in blending. The default is BLEND_ADD.
	 *
	 * \param o blend operation
	 */
	static void set_blend_op(BlendOperation o);
	
	/**
	 * Sets the specified blend operation in blending. The default is BLEND_ADD,
	 * BLEND_ADD.
	 *
	 * \param rgb RGB blend operation
	 * \param a alpha blend operation
	 */
	static void set_blend_op(BlendOperation rgb, BlendOperation a);
	
	/**
	 * Returns the source blend factor of the RGB color components in blending.
	 */
	static BlendFactor get_blend_src_rgb();
	
	/**
	 * Returns the source blend factor of the alpha color component in blending.
	 */
	static BlendFactor get_blend_src_alpha();
	
	/**
	 * Returns the destination blend factor of the RGB color components in
	 * blending.
	 */
	static BlendFactor get_blend_dst_rgb();
	
	/**
	 * Returns the destination blend factor of the alpha color component in
	 * blending.
	 */
	static BlendFactor get_blend_dst_alpha();
	
	/**
	 * Sets the specified source and destination blend factors in blending. The
	 * default is FACTOR_ONE and FACTOR_ZERO.
	 *
	 * \param s source blend factor
	 * \param d destination blend factor
	 */
	static void set_blend_factor(BlendFactor s, BlendFactor d);
	
	/**
	 * Sets the specified source and destination blend factors in blending. The
	 * default is FACTOR_ONE, FACTOR_ZERO, FACTOR_ONE, FACTOR_ZERO.
	 *
	 * \param sr RGB source blend factor
	 * \param dr RGB destination blend factor
	 * \param sa alpha source blend factor
	 * \param da alpha destination blend factor
	 */
	static void set_blend_factor(BlendFactor sr, BlendFactor dr,
								 BlendFactor sa, BlendFactor da);
	
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
	 * Enables scissor test. If enabled, discards fragments that are outside the
	 * scissor rectangle.
	 */
	static void enable_scissor_test();
	
	/**
	 * Disables scissor test.
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
	 * Disables wireframe.
	 */
	static void disable_wireframe();
	
	/**
	 * Enables face culling. If enabled, culls polygons based on their winding
	 * in window coordinates.
	 */
	static void enable_culling();
	
	/**
	 * Disables face culling.
	 */
	static void disable_culling();
	
	/**
	 * Returns the side of faces that will be culled.
	 */
	static RenderSide get_cull_side();
	
	/**
	 * Determines which side of faces will be culled. The default is BACK_SIDE.
	 *
	 * \param s culling side
	 */
	static void set_cull_side(RenderSide s);
	
	/**
	 * Enables polygon offset. If enabled, and if the wireframe is disabled, an
	 * offset is added to depth values of a polygon's fragments before the depth
	 * comparison is performed.
	 */
	static void enable_polygon_offset();
	
	/**
	 * Disables polygon offset.
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
	 * Sets the specified polygon offset factor and units. The default is 0 and
	 * 0.
	 *
	 * \param f polygon offset factor
	 * \param u polygon offset units
	 */
	static void set_polygon_offset(float f, float u);
	
	/**
	 * Enables dithering. If enabled, dithers color components or indices before
	 * they are written to the color buffer.
	 */
	static void enable_dithering();
	
	/**
	 * Disables dithering.
	 */
	static void disable_dithering();
	
	/**
	 * Enables multisample anti-aliasing. If enabled, uses multiple fragment
	 * samples in computing the final color of a pixel.
	 */
	static void enable_multisample();
	
	/**
	 * Disables multisample anti-aliasing.
	 */
	static void disable_multisample();
	
	/**
	 * Enables alpha to coverage. If enabled, computes a temporary coverage
	 * value where each bit is determined by the alpha value at the
	 * corresponding sample location.
	 */
	static void enable_alpha_to_coverage();
	
	/**
	 * Disables alpha to coverage.
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
	 * Disables seamless cube texture accesses.
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
	Shader();
	
	/**
	 * Deletes this Shader object.
	 */
	~Shader();
	
	/**
	 * Shader is non-copyable. The copy constructor is deleted.
	 */
	Shader(const Shader&) = delete;
	
	/**
	 * Shader is non-copyable. The copy assignment operator is deleted.
	 */
	Shader& operator=(const Shader&) = delete;
	
	/**
	 * Loads the contents of the specified vertex shader to this shader.
	 *
	 * \param s vertex shader
	 */
	void load_vert(const char* s);
	
	/**
	 * Loads the contents of the specified vertex shader to this shader.
	 *
	 * \param s vertex shader
	 */
	void load_vert(const std::string& s);
	
	/**
	 * Loads the contents of the specified geometry shader to this shader.
	 *
	 * \param s geometry shader
	 */
	void load_geom(const char* s);
	
	/**
	 * Loads the contents of the specified geometry shader to this shader.
	 *
	 * \param s geometry shader
	 */
	void load_geom(const std::string& s);
	
	/**
	 * Loads the contents of the specified fragment shader to this shader.
	 *
	 * \param s fragment shader
	 */
	void load_frag(const char* s);
	
	/**
	 * Loads the contents of the specified fragment shader to this shader.
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
	 * Compiles the shader program if the shader has been changed.
	 */
	void compile() const;
	
	/**
	 * Uses the program of the compiled shader.
	 */
	void use_program() const;
	
	/**
	 * Sets the preprocessor defines for the shader. These defines are then
	 * available in the vertex, geometry, and fragment shaders.
	 *
	 * \param d defines
	 */
	void set_defines(const Defines& d);
	
	/**
	 * Sets the value of the uniform variable. These uniform variables are then
	 * available in the vertex, geometry, and fragment shaders.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_i(const std::string& n, int v) const;
	
	/**
	 * Sets the value of the uniform variable. These uniform variables are then
	 * available in the vertex, geometry, and fragment shaders.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_u(const std::string& n, unsigned int v) const;
	
	/**
	 * Sets the value of the uniform variable. These uniform variables are then
	 * available in the vertex, geometry, and fragment shaders.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_f(const std::string& n, float v) const;
	
	/**
	 * Sets the value of the uniform variable. These uniform variables are then
	 * available in the vertex, geometry, and fragment shaders.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_v2(const std::string& n, const Vec2& v) const;
	
	/**
	 * Sets the value of the uniform variable. These uniform variables are then
	 * available in the vertex, geometry, and fragment shaders.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_v3(const std::string& n, const Vec3& v) const;
	
	/**
	 * Sets the value of the uniform variable. These uniform variables are then
	 * available in the vertex, geometry, and fragment shaders.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_v4(const std::string& n, const Vec4& v) const;
	
	/**
	 * Sets the value of the uniform variable. These uniform variables are then
	 * available in the vertex, geometry, and fragment shaders.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_m2(const std::string& n, const Mat2& v) const;
	
	/**
	 * Sets the value of the uniform variable. These uniform variables are then
	 * available in the vertex, geometry, and fragment shaders.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_m3(const std::string& n, const Mat3& v) const;
	
	/**
	 * Sets the value of the uniform variable. These uniform variables are then
	 * available in the vertex, geometry, and fragment shaders.
	 *
	 * \param n variable name
	 * \param v value
	 */
	void set_uniform_m4(const std::string& n, const Mat4& v) const;
	
	/**
	 * Sets the value of the uniform variable. These uniform variables are then
	 * available in the vertex, geometry, and fragment shaders.
	 *
	 * \param u uniforms object
	 */
	void set_uniforms(const Uniforms& u) const;
	
	/**
	 * Returns the GLSL version of the shading language.
	 */
	static std::string get_glsl_version();
	
	/**
	 * Sets the GLSL version of the shading language. The default is "410".
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
	VertexObject();
	
	/**
	 * Deletes this VertexObject object.
	 */
	~VertexObject();
	
	/**
	 * VertexObject is non-copyable. The copy constructor is deleted.
	 */
	VertexObject(const VertexObject&) = delete;
	
	/**
	 * VertexObject is non-copyable. The copy assignment operator is deleted.
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
	 * Attaches this vertex object to the target shader to automatically match
	 * the input data with the shader locations.
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
	Texture();
	
	/**
	 * Deletes this Texture object.
	 */
	~Texture();
	
	/**
	 * Texture is non-copyable. The copy constructor is deleted.
	 */
	Texture(const Texture&) = delete;
	
	/**
	 * Texture is non-copyable. The copy assignment operator is deleted.
	 */
	Texture& operator=(const Texture&) = delete;
	
	/**
	 * Initializes the texture as an empty 1D texture.
	 *
	 * \param w the width of the texture
	 * \param f texture format
	 * \param t image data type
	 */
	void init_1d(int w, TextureFormat f, ImageType t = IMAGE_UBYTE);
	
	/**
	 * Initializes the texture as an empty 2D texture.
	 *
	 * \param w the width of the texture
	 * \param h the height of the texture
	 * \param f texture format
	 * \param t image data type
	 */
	void init_2d(int w, int h, TextureFormat f, ImageType t = IMAGE_UBYTE);
	
	/**
	 * Initializes the texture as a 2D texture with the specified image.
	 *
	 * \param i image
	 * \param f texture format
	 * \param t image data format
	 */
	void init_2d(const Image& i, TextureFormat f, ImageFormat t = IMAGE_COLOR);
	
	/**
	 * Initializes the texture as an empty 3D texture.
	 *
	 * \param w the width of the texture
	 * \param h the height of the texture
	 * \param d the depth of the texture
	 * \param f texture format
	 * \param t image data type
	 */
	void init_3d(int w, int h, int d, TextureFormat f, ImageType t = IMAGE_UBYTE);
	
	/**
	 * Initializes the texture as an empty cube texture.
	 *
	 * \param w the width of the texture
	 * \param h the height of the texture
	 * \param f texture format
	 * \param t image data type
	 */
	void init_cube(int w, int h, TextureFormat f, ImageType t = IMAGE_UBYTE);
	
	/**
	 * Initializes the texture as a cube texture with the specified images from
	 * different sides.
	 *
	 * \param px right (+X) side of the cube image
	 * \param nx left  (-X) side of the cube image
	 * \param py upper (+Y) side of the cube image
	 * \param ny lower (-Y) side of the cube image
	 * \param pz front (+Z) side of the cube image
	 * \param nz back  (-Z) side of the cube image
	 * \param f texture format
	 * \param t image data format
	 */
	void init_cube(const Image& px, const Image& nx, const Image& py, const Image& ny,
				   const Image& pz, const Image& nz, TextureFormat f, ImageFormat t = IMAGE_COLOR);
	
	/**
	 * Initializes the texture as an empty 1D texture array.
	 *
	 * \param w the width of the texture
	 * \param l the layer of the texture
	 * \param f texture format
	 * \param t image data type
	 */
	void init_1d_array(int w, int l, TextureFormat f, ImageType t = IMAGE_UBYTE);
	
	/**
	 * Initializes the texture as an empty 2D texture array.
	 *
	 * \param w the width of the texture
	 * \param h the height of the texture
	 * \param l the layer of the texture
	 * \param f texture format
	 * \param t image data type
	 */
	void init_2d_array(int w, int h, int l, TextureFormat f, ImageType t = IMAGE_UBYTE);
	
	/**
	 * Initializes the texture as an empty cube texture array.
	 *
	 * \param w the width of the texture
	 * \param h the height of the texture
	 * \param l the layer of the texture
	 * \param f texture format
	 * \param t image data type
	 */
	void init_cube_array(int w, int h, int l, TextureFormat f, ImageType t = IMAGE_UBYTE);
	
	/**
	 * Returns the width of the texture, or 0 if the texture is uninitialized.
	 */
	int get_width() const;
	
	/**
	 * Returns the height of the texture, or 0 if the texture is uninitialized.
	 */
	int get_height() const;
	
	/**
	 * Returns the depth of the texture, or 0 if the texture is uninitialized.
	 */
	int get_depth() const;
	
	/**
	 * Returns the layer of the texture, or 0 if the texture is uninitialized.
	 * The type must be one of TEXTURE_1D_ARRAY, TEXTURE_2D_ARRAY,
	 * TEXTURE_CUBE_ARRAY.
	 */
	int get_layer() const;
	
	/**
	 * Returns the type of the texture.
	 */
	TextureType get_type() const;
	
	/**
	 * Returns the format of the texture.
	 */
	TextureFormat get_format() const;
	
	/**
	 * Copies the texture to the specified image. The type must be TEXTURE_2D.
	 */
	void copy_to_image(Image& i) const;
	
	/**
	 * Generates mipmaps for the texture.
	 */
	void generate_mipmap() const;
	
	/**
	 * Sets the wrapping mode on S coordinate. The default is TEXTURE_REPEAT.
	 *
	 * \param m wrapping mode
	 */
	void set_wrap_s(TextureWrappingMode m) const;
	
	/**
	 * Sets the wrapping mode on T coordinate. The default is TEXTURE_REPEAT.
	 *
	 * \param m wrapping mode
	 */
	void set_wrap_t(TextureWrappingMode m) const;
	
	/**
	 * Sets the wrapping mode on R coordinate. The default is TEXTURE_REPEAT.
	 *
	 * \param m wrapping mode
	 */
	void set_wrap_r(TextureWrappingMode m) const;
	
	/**
	 * Sets the wrapping modes on S, T and R coordinates.
	 *
	 * \param m wrapping mode
	 */
	void set_wrap_all(TextureWrappingMode m) const;
	
	/**
	 * Sets the magnification and minification filters of the texture. The
	 * default is TEXTURE_LINEAR and TEXTURE_NEAREST_MIPMAP_LINEAR.
	 *
	 * \param mag magnification filter
	 * \param min minification filter
	 */
	void set_filters(TextureFilter mag, TextureFilter min) const;
	
	/**
	 * Sets the border color for border texels. The default is (0, 0, 0, 0).
	 *
	 * \param c border color
	 */
	void set_border_color(const Vec4& c) const;
	
	/**
	 * Sets the range of LODs for the texture with mipmaps. The default is 0 and
	 * 1000.
	 *
	 * \param min minimal defined level
	 * \param max maximal defined level
	 */
	void set_lod_range(int min, int max) const;
	
	/**
	 * Sets the bias of LOD for the texture with mipmaps. The default is 0.
	 *
	 * \param b bias level
	 */
	void set_lod_bias(float b) const;
	
	/**
	 * Activates this texture at the specified location.
	 *
	 * \param l the location of the texture
	 */
	int activate(int l) const;
	
	/**
	 * Returns the default texture format for the specified channel and byte.
	 *
	 * \param c channel
	 * \param b byte
	 */
	static TextureFormat default_format(int c, int b);
	
	/**
	 * Returns the default texture format for the specified image.
	 *
	 * \param i image
	 */
	static TextureFormat default_format(const Image& i);
	
private:
	uint32_t id = 0;
	
	int width = 0;
	int height = 0;
	int depth = 0;
	
	TextureType type = TEXTURE_2D;
	
	TextureFormat format = TEXTURE_R8G8B8A8_UNORM;
	
	void set_dimensions(int w, int h, int d);
	
	void set_parameters(TextureType t, TextureFormat f);
	
	friend class RenderTarget;
};

class Renderbuffer {
public:
	/**
	 * Creates a new Renderbuffer object.
	 */
	Renderbuffer();
	
	/**
	 * Deletes this Renderbuffer object.
	 */
	~Renderbuffer();
	
	/**
	 * Renderbuffer is non-copyable. The copy constructor is deleted.
	 */
	Renderbuffer(const Renderbuffer&) = delete;
	
	/**
	 * Renderbuffer is non-copyable. The copy assignment operator is deleted.
	 */
	Renderbuffer& operator=(const Renderbuffer&) = delete;
	
	/**
	 * Initializes the empty render buffer with width, height, format.
	 *
	 * \param w the width of the render buffer
	 * \param h the height of the render buffer
	 * \param f texture format
	 */
	void init(int w, int h, TextureFormat f) const;
	
private:
	uint32_t id = 0;
	
	friend class RenderTarget;
};

class RenderTarget {
public:
	/**
	 * Creates a new RenderTarget object.
	 */
	RenderTarget();
	
	/**
	 * Deletes this RenderTarget object.
	 */
	~RenderTarget();
	
	/**
	 * RenderTarget is non-copyable. The copy constructor is deleted.
	 */
	RenderTarget(const RenderTarget&) = delete;
	
	/**
	 * RenderTarget is non-copyable. The copy assignment operator is deleted.
	 */
	RenderTarget& operator=(const RenderTarget&) = delete;
	
	/**
	 * Sets the 2D texture, the cube texture, the 2D texture array or the 3D
	 * texture to the render target.
	 *
	 * \param t 2D texture, cube texture, 2D texture array or 3D texture
	 * \param i the index of the texture
	 * \param l mipmap level
	 * \param p if there is a cube texture, specifies the face of the cube texture
	 *          if there is a 2D texture array, specifies the layer of the texture
	 *          if there is a 3D texture, specifies the layer of the texture
	 */
	void set_texture(const Texture& t, unsigned int i, int l = 0, int p = 0) const;
	
	/**
	 * Sets the 2D texture or the cube texture as the depth texture of render
	 * target.
	 *
	 * \param t 2D texture or cube texture
	 * \param l mipmap level
	 * \param p if there is a cube texture, specifies the face of the cube texture
	 *          if there is a 2D texture array, specifies the layer of the texture
	 */
	void set_depth_texture(const Texture& t, int l = 0, int p = 0) const;
	
	/**
	 * Sets the 2D texture or the cube texture as the stencil texture of render
	 * target.
	 *
	 * \param t 2D texture or cube texture
	 * \param l mipmap level
	 * \param p if there is a cube texture, specifies the face of the cube texture
	 *          if there is a 2D texture array, specifies the layer of the texture
	 */
	void set_stencil_texture(const Texture& t, int l = 0, int p = 0) const;
	
	/**
	 * Sets the 2D texture or the cube texture as the depth stencil texture of
	 * render target.
	 *
	 * \param t 2D texture or cube texture
	 * \param l mipmap level
	 * \param p if there is a cube texture, specifies the face of the cube texture
	 *          if there is a 2D texture array, specifies the layer of the texture
	 */
	void set_depth_stencil_texture(const Texture& t, int l = 0, int p = 0) const;
	
	/**
	 * Sets the render buffer as the depth buffer of the render target.
	 *
	 * \param r render buffer
	 */
	void set_depth_buffer(const Renderbuffer& r) const;
	
	/**
	 * Sets the render buffer as the stencil buffer of the render target.
	 *
	 * \param r render buffer
	 */
	void set_stencil_buffer(const Renderbuffer& r) const;
	
	/**
	 * Sets the render buffer as the depth stencil buffer of the render target.
	 *
	 * \param r render buffer
	 */
	void set_depth_stencil_buffer(const Renderbuffer& r) const;
	
	/**
	 * Sets the number of targets to be rendered into. The default is 1.
	 *
	 * \param n the number of targets
	 */
	void set_target_number(int n) const;
	
	/**
	 * Activates this render target.
	 */
	void activate() const;
	
	/**
	 * Activates the specified render target. If the render target is nullptr,
	 * falls back to the default render target.
	 *
	 * \param t render target
	 */
	static void activate(const RenderTarget* t);
	
private:
	uint32_t id = 0;
	
	std::vector<uint32_t> attachments;
	
	void set_framebuffer(const Texture& t, uint32_t a, int l, int p) const;
	
	void set_framebuffer(const Renderbuffer& r, uint32_t a) const;
};

}
