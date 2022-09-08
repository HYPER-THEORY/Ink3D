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

#include "../graphics/Gpu.h"
#include "../scene/Scene.h"
#include "../lights/EnvProbe.h"

namespace Ink {

class Renderer {
public:
	/**
	 * Create a new renderer.
	 */
	Renderer() = default;
	
	/**
	 * Returns the clear color and opacity.
	 */
	Vec4 get_clear_color() const;
	
	/**
	 * Sets the clear color and opacity.
	 *
	 * \param c clear color
	 */
	void set_clear_color(const Vec4& c);
	
	/**
	 * Clear the color, depth or stencil drawing buffers. Initialize the color
	 * buffer to the current clear color value.
	 *
	 * \param c whether to clear the color buffer
	 * \param d whether to clear the depth buffer
	 * \param s whether to clear the stencil buffer
	 */
	void clear(bool c = true, bool d = true, bool s = true) const;
	
	/**
	 * Returns the viewport.
	 */
	Gpu::Rect get_viewport() const;
	
	/**
	 * Sets the viewport.
	 *
	 * \param v viewport
	 */
	void set_viewport(const Gpu::Rect& v);
	
	/**
	 * Determines whether to enable scissor test.
	 */
	bool get_scissor_test() const;
	
	/**
	 * Determines whether to enable scissor test.
	 *
	 * \param t enable scissor test
	 */
	void set_scissor_test(bool t);
	
	/**
	 * Returns the scissor region.
	 */
	Gpu::Rect get_scissor() const;
	
	/**
	 * Sets the scissor region.
	 *
	 * \param s scissor region
	 */
	void set_scissor(const Gpu::Rect& s);
	
	/**
	 * Sets the location of G-Buffer color.
	 *
	 * \param l the location of G-Buffer
	 */
	void set_location_c(unsigned int l);
	
	/**
	 * Returns the locations of G-Buffer color.
	 */
	unsigned int get_location_c() const;
	
	/**
	 * Sets the location of G-Buffer normal.
	 *
	 * \param l the location of G-Buffer
	 */
	void set_location_n(unsigned int l);
	
	/**
	 * Returns the locations of G-Buffer normal.
	 */
	unsigned int get_location_n() const;
	
	/**
	 * Sets the location of G-Buffer material.
	 *
	 * \param l the location of G-Buffer
	 */
	void set_location_m(unsigned int l);
	
	/**
	 * Returns the locations of G-Buffer material.
	 */
	unsigned int get_location_m() const;
	
	/**
	 * Sets the location of G-Buffer emissive.
	 *
	 * \param l the location of G-Buffer
	 */
	void set_location_e(unsigned int l);
	
	/**
	 * Returns the locations of G-Buffer emissive.
	 */
	unsigned int get_location_e() const;
	
	/**
	 * Sets the location of G-Buffer indirect light.
	 *
	 * \param l the location of G-Buffer
	 */
	void set_location_i(unsigned int l);
	
	/**
	 * Returns the locations of G-Buffer indirect light.
	 */
	unsigned int get_location_i() const;
	
	/**
	 * Returns the current render target if there are, returns nullptr
	 * otherwise.
	 */
	const Gpu::FrameBuffer* get_target() const;
	
	/**
	 * Sets the render target. When nullptr is given, the default frame buffer
	 * is set as the target frame buffer.
	 *
	 * \param t target frame buffer
	 */
	void set_target(const Gpu::FrameBuffer* t);
	
	using TextureCallback = std::function<void(Gpu::Texture&)>;
	
	/**
	 * Sets the texture callback to set up texture when it's created.
	 *
	 * \param f texture callback
	 */
	void set_texture_callback(const TextureCallback& f);
	
	/**
	 * Sets the intensity of skybox.
	 *
	 * \param i intensity
	 */
	void set_skybox_intensity(float i);
	
	/**
	 * Load a set of cube images to skybox.
	 *
	 * \param i +X, -X, +Y, -Y, +Z, -Z images
	 */
	void load_skybox_cubemap(const Image* i);
	
	/**
	 * Load an equirectangular image to skybox.
	 *
	 * \param i equirectangular image
	 */
	void load_skybox_equirect(const Image& i);
	
	/**
	 * Render skybox using a camera.
	 *
	 * \param c camera
	 */
	void render_skybox(const Camera& c);
	
	/**
	 * Load mesh and create corresponding vertex object.
	 *
	 * \param m mesh
	 */
	void load_mesh(const Mesh* m);
	
	/**
	 * Load image and create corresponding texture.
	 *
	 * \param i image
	 */
	void load_image(const Image* i);
	
	/**
	 * Load material and create corresponding shader.
	 *
	 * \param m material
	 */
	void load_material(const Material* m);
	
	/**
	 * Load meshes, images and materials in scene.
	 *
	 * \param s scene
	 */
	void load_scene(const Scene& s);
	
	/**
	 * Update scene before render it.
	 */
	void update_scene(Scene& s);
	
	/**
	 * Render a scene using a camera.
	 *
	 * \param s scene
	 * \param c camera
	 */
	void render(const Scene& s, const Camera& c) const;
	
	/**
	 * Render a scene to shadow map.
	 *
	 * \param s scene
	 * \param t target shadow
	 */
	void render_shadow(const Scene& s, const Shadow& t) const;
	
	/**
	 * Update the shadow map of spot light.
	 */
	void update_shadow(const Scene& s, SpotLight& l) const;
	
	/**
	 * Update the shadow map of directional light.
	 */
	void update_shadow(const Scene& s, DirectionalLight& l) const;
	
private:
	unsigned int location_c = 0;
	
	unsigned int location_n = 1;
	
	unsigned int location_m = 2;
	
	unsigned int location_e = 3;
	
	unsigned int location_i = 4;
	
	bool scissor_test = false;
	
	Vec4 clear_color = {0, 0, 0, 0};
	
	Gpu::Rect scissor = {0, 0, 0, 0};
	
	Gpu::Rect viewport = {0, 0, 0, 0};
	
	const Gpu::FrameBuffer* target = nullptr;
	
	float skybox_intensity = 1;
	
	std::unique_ptr<Gpu::Texture> skybox_map;
	
	TextureCallback texture_callback = [](Gpu::Texture& t) -> void {
		/* generate mipmap for every texture */
		t.generate_mipmap();
	};
	
	std::unordered_map<const Mesh*, std::unique_ptr<Gpu::VertexObject[]> > mesh_buffer;
	
	std::unordered_map<const Image*, std::unique_ptr<Gpu::Texture> > image_buffer;
	
	std::unordered_map<const Material*, std::unique_ptr<Gpu::Shader> > material_buffer;
	
	static std::unique_ptr<Gpu::VertexObject> cube;
	
	static std::unique_ptr<Gpu::Shader> skybox_shader;
	
	using MaterialCallback = std::function<bool(const Instance&, const Material&)>;
	
	void render(const Scene& s, const Camera& c, const MaterialCallback& f) const;
	
	static bool init_skybox();
};

}
