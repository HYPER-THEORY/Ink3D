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

#include "../graphics/Gpu.h"
#include "../scene/Scene.h"
#include "../probes/ReflectionProbe.h"

#include <functional>

namespace ink {

enum RenderingMode {
	FORWARD_RENDERING,
	DEFERRED_RENDERING,
};

class Renderer {
public:
	using TextureCallback = std::function<void(gpu::Texture&)>;
	
	/**
	 * Creates a new Renderer object.
	 */
	Renderer() = default;
	
	/**
	 * Returns the color (.xyz) and alpha (.w) for clearing.
	 */
	Vec4 get_clear_color() const;
	
	/**
	 * Sets the specified color (.xyz) and alpha (.w) for clearing. The default
	 * is (0, 0, 0, 0).
	 *
	 * \param c color with alpha
	 */
	void set_clear_color(const Vec4& c);
	
	/**
	 * Clears the color, depth or stencil buffers. Fills the color buffer with
	 * the current color clear value.
	 *
	 * \param c whether to clear the color buffer
	 * \param d whether to clear the depth buffer
	 * \param s whether to clear the stencil buffer
	 */
	void clear(bool c = true, bool d = true, bool s = true) const;
	
	/**
	 * Returns the viewport region.
	 */
	gpu::Rect get_viewport() const;
	
	/**
	 * Sets the viewport region to render from (x, y) to (x + width, y +
	 * height). The default is (0, 0, 0, 0).
	 *
	 * \param v viewport region
	 */
	void set_viewport(const gpu::Rect& v);
	
	/**
	 * Returns true if scissor test is enabled.
	 */
	bool get_scissor_test() const;
	
	/**
	 * Determines whether to enable scissor test. The default is false.
	 *
	 * \param t enable scissor test
	 */
	void set_scissor_test(bool t);
	
	/**
	 * Returns the scissor region in scissor test.
	 */
	gpu::Rect get_scissor() const;
	
	/**
	 * Sets the scissor region from (x, y) to (x + width, y + height) in scissor
	 * test. The default is (0, 0, 0, 0).
	 *
	 * \param s scissor region
	 */
	void set_scissor(const gpu::Rect& s);
	
	/**
	 * Returns the rendering mode. If the mode is DEFERRED_RENDERING, there
	 * should be 4 render targets.
	 */
	RenderingMode get_rendering_mode() const;
	
	/**
	 * Sets the specified rendering mode. The default is DEFERRED_RENDERING.
	 *
	 * \param m rendering mode
	 */
	void set_rendering_mode(RenderingMode m);
	
	/**
	 * Returns the current render target if there is one, returns nullptr
	 * otherwise.
	 */
	const gpu::RenderTarget* get_target() const;
	
	/**
	 * Sets the render target. If the render target is nullptr, falls back to
	 * the default render target.
	 *
	 * \param t render target
	 */
	void set_target(const gpu::RenderTarget* t);
	
	/**
	 * Sets the texture callback which will be called when an image is loaded.
	 *
	 * \param f texture callback function
	 */
	void set_texture_callback(const TextureCallback& f);
	
	/**
	 * Returns the intensity of the skybox.
	 */
	float get_skybox_intensity() const;
	
	/**
	 * Sets the intensity of the skybox. The default is 1.
	 *
	 * \param i intensity
	 */
	void set_skybox_intensity(float i);
	
	/**
	 * Loads the specified image set, one for each side of the skybox cubemap.
	 *
	 * \param px right (+X) side of the skybox cubemap
	 * \param nx left  (-X) side of the skybox cubemap
	 * \param py upper (+Y) side of the skybox cubemap
	 * \param ny lower (-Y) side of the skybox cubemap
	 * \param pz front (+Z) side of the skybox cubemap
	 * \param nz back  (-Z) side of the skybox cubemap
	 */
	void load_skybox(const Image& px, const Image& nx,
					 const Image& py, const Image& ny,
					 const Image& pz, const Image& nz);
	
	/**
	 * Loads the specified equirectangular image to the skybox.
	 *
	 * \param i equirectangular image
	 */
	void load_skybox(const Image& i);
	
	/**
	 * Renders skybox using a camera. This function should be called before
	 * the scene is rendered.
	 *
	 * \param c camera
	 */
	void render_skybox(const Camera& c) const;
	
	/**
	 * Loads the specified mesh and creates corresponding vertex object.
	 *
	 * \param m mesh
	 */
	void load_mesh(const Mesh& m);
	
	/**
	 * Unloads the specified mesh and deletes corresponding vertex object.
	 * 
	 * \param m mesh
	 */
	void unload_mesh(const Mesh& m);
	
	/**
	 * Clears the mesh cache. The caches are generated automatically when
	 * loading meshes.
	 */
	void clear_mesh_caches();
	
	/**
	 * Loads the specified image and creates corresponding texture. This
	 * function will invoke the texture callback.
	 *
	 * \param i image
	 */
	void load_image(const Image& i);
	
	/**
	 * Unloads the specified image and deletes corresponding texture.
	 *
	 * \param i image
	 */
	void unload_image(const Image& i);
	
	/**
	 * Clears the image cache. The caches are generated automatically when
	 * loading images.
	 */
	void clear_image_caches();
	
	/**
	 * Loads all the meshes and images in the scene.
	 *
	 * \param s scene
	 */
	void load_scene(const Scene& s);
	
	/**
	 * Unloads all the meshes and images in the scene.
	 *
	 * \param s scene
	 */
	void unload_scene(const Scene& s);
	
	/**
	 * Clears the scene cache. The caches are generated automatically when
	 * loading meshes, images or scenes.
	 */
	void clear_scene_caches();
	
	/**
	 * Renders a scene using a camera. The results will be rendered to the
	 * current render target.
	 *
	 * \param s scene
	 * \param c camera
	 */
	void render(const Scene& s, const Camera& c) const;
	
	/**
	 * Renders the transparent objects in a scene using a camera. The results
	 * will be rendered to the current render target.
	 *
	 * \param s scene
	 * \param c camera
	 */
	void render_transparent(const Scene& s, const Camera& c) const;
	
	/**
	 * Renders a scene using the shadow camera. The results will be rendered to
	 * the shadow map.
	 *
	 * \param s scene
	 * \param t target shadow
	 */
	void render_shadow(const Scene& s, const Shadow& t) const;
	
	/**
	 * Updates the shadow of the spot light. This function will update the
	 * shadow camera and render the results to shadow map.
	 *
	 * \param s scene
	 * \param l spot light
	 */
	void update_shadow(const Scene& s, SpotLight& l) const;
	
	/**
	 * Updates the shadow of the directional light. This function will update
	 * the shadow camera and render the results to shadow map.
	 *
	 * \param s scene
	 * \param l directional light
	 */
	void update_shadow(const Scene& s, DirectionalLight& l) const;
	
	/**
	 * Updates the reflection probe at its position. The scene will be rendered
	 * in forward rendering mode.
	 *
	 * \param s scene
	 * \param r reflection probe
	 */
	void update_probe(const Scene& s, ReflectionProbe& r) const;
	
	/**
	 * Updates all the instances in the scene before the scene is rendered.
	 *
	 * \param s scene
	 */
	static void update_scene(Scene& s);
	
	/**
	 * Sets the defines object for the material.
	 *
	 * \param m material
	 * \param d defines
	 */
	static void set_material_defines(const Material& m, Defines& d);
	
	/**
	 * Sets the defines object for the scene.
	 *
	 * \param s scene
	 * \param d defines
	 */
	static void set_scene_defines(const Scene& s, Defines& d);
	
	/**
	 * Sets the uniforms object for the lights.
	 *
	 * \param s scene
	 * \param shader shader
	 */
	static void set_light_uniforms(const Scene& s, const gpu::Shader& shader);
	
private:
	Vec4 clear_color = {0, 0, 0, 0};
	
	bool scissor_test = false;
	
	gpu::Rect scissor = gpu::Rect(0, 0, 0, 0);
	
	gpu::Rect viewport = gpu::Rect(0, 0, 0, 0);
	
	const gpu::RenderTarget* target = nullptr;
	
	TextureCallback texture_callback = [](gpu::Texture& t) -> void {
		t.generate_mipmap(); /* generate mipmap for every texture */
	};
	
	RenderingMode rendering_mode = DEFERRED_RENDERING;
	
	float skybox_intensity = 1;
	
	std::unique_ptr<gpu::Texture> skybox_map;
	
	std::unordered_map<const Mesh*, std::unique_ptr<gpu::VertexObject[]>> mesh_cache;
	
	std::unordered_map<const Image*, std::unique_ptr<gpu::Texture>> image_cache;
	
	static std::unique_ptr<gpu::VertexObject> cube;
	
	static std::unique_ptr<gpu::Texture> probe_map;
	
	static std::unique_ptr<gpu::Renderbuffer> probe_buffer;
	
	static std::unique_ptr<gpu::RenderTarget> probe_target;
	
	void render_skybox_to_buffer(const Camera& c, RenderingMode r) const;
	
	void render_to_buffer(const Scene& s, const Camera& c, RenderingMode r, bool t) const;
	
	void render_to_shadow(const Scene& s, const Camera& c) const;
	
	static void init_cube();
	
	static void sort_instances(const Camera& c, std::vector<const Instance*>& l, bool t);
};

}
