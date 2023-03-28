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

#include "../graphics/Gpu.h"
#include "../scene/Scene.h"
#include "../probes/ReflectionProbe.h"

#include <functional>

namespace Ink {

enum RenderingMode {
	FORWARD_RENDERING,
	DEFERRED_RENDERING,
};

enum ToneMapMode {
	LINEAR_TONE_MAP,
	REINHARD_TONE_MAP,
	OPTIMIZED_TONE_MAP,
	ACES_FILMIC_TONE_MAP,
};

class Renderer {
public:
	using TextureCallback = std::function<void(Gpu::Texture&)>;
	
	/**
	 * Creates a new Renderer object.
	 */
	explicit Renderer() = default;
	
	/**
	 * Returns the clear color (.xyz) and opacity (.w).
	 */
	Vec4 get_clear_color() const;
	
	/**
	 * Sets the specified clear color (.xyz) and opacity (.w). Default is (0, 0,
	 * 0, 0).
	 *
	 * \param c clear color and opacity
	 */
	void set_clear_color(const Vec4& c);
	
	/**
	 * Clears the color, depth or stencil drawing buffers. Initializes the color
	 * buffer to the current clear color value.
	 *
	 * \param c whether to clear the color buffer
	 * \param d whether to clear the depth buffer
	 * \param s whether to clear the stencil buffer
	 */
	void clear(bool c = true, bool d = true, bool s = true) const;
	
	/**
	 * Returns the viewport region.
	 */
	Gpu::Rect get_viewport() const;
	
	/**
	 * Sets the viewport region to render from (x, y) to (x + width, y +
	 * height). Default is (0, 0, 0, 0).
	 *
	 * \param v viewport region
	 */
	void set_viewport(const Gpu::Rect& v);
	
	/**
	 * Returns true if scissor test is enabled.
	 */
	bool get_scissor_test() const;
	
	/**
	 * Determines whether to enable scissor test. Default is false.
	 *
	 * \param t enable scissor test
	 */
	void set_scissor_test(bool t);
	
	/**
	 * Returns the scissor region in scissor test.
	 */
	Gpu::Rect get_scissor() const;
	
	/**
	 * Sets the scissor region from (x, y) to (x + width, y + height) in scissor
	 * test. Default is (0, 0, 0, 0).
	 *
	 * \param s scissor region
	 */
	void set_scissor(const Gpu::Rect& s);
	
	/**
	 * Returns the mode in rendering. If mode is DEFERRED_RENDERING, the number
	 * of render targets should be 4.
	 */
	RenderingMode get_rendering_mode() const;
	
	/**
	 * Sets the specified mode in rendering. If mode is DEFERRED_RENDERING, the
	 * number of render targets should be 4. Default is DEFERRED_RENDERING.
	 *
	 * \param m rendering mode
	 */
	void set_rendering_mode(RenderingMode m);
	
	/**
	 * Returns the mode in tone mapping. This term will only be used when the
	 * rendering mode is FORWARD_RENDERING.
	 */
	ToneMapMode get_tone_map_mode() const;
	
	/**
	 * Returns the exposure in tone mapping. This term will only be used when
	 * the rendering mode is FORWARD_RENDERING.
	 */
	float get_tone_map_exposure() const;
	
	/**
	 * Sets the specified mode and exposure in tone mapping. This term will only
	 * be used in forward rendering or transparent object rendering. Default is
	 * LINEAR_TONE_MAP and 1.
	 *
	 * \param m tone mapping mode
	 * \param e tone mapping exposure
	 */
	void set_tone_map(ToneMapMode m, float e);
	
	/**
	 * Returns the current render target if there is, returns nullptr otherwise.
	 */
	const Gpu::RenderTarget* get_target() const;
	
	/**
	 * Sets the render target. When nullptr is given, the default render target
	 * is set as the render target.
	 *
	 * \param t render target
	 */
	void set_target(const Gpu::RenderTarget* t);
	
	/**
	 * Sets the texture callback which will be called when texture is created.
	 *
	 * \param f texture callback function
	 */
	void set_texture_callback(const TextureCallback& f);
	
	/**
	 * Returns the intensity of skybox.
	 */
	float get_skybox_intensity() const;
	
	/**
	 * Sets the intensity of skybox. Default is 1.
	 *
	 * \param i intensity
	 */
	void set_skybox_intensity(float i);
	
	/**
	 * Loads a set of specified images, one for each side of the skybox cubemap.
	 *
	 * \param px right (+X) side of skybox cubemap
	 * \param nx left  (-X) side of skybox cubemap
	 * \param py upper (+Y) side of skybox cubemap
	 * \param ny lower (-Y) side of skybox cubemap
	 * \param pz front (+Z) side of skybox cubemap
	 * \param nz back  (-Z) side of skybox cubemap
	 */
	void load_skybox(const Image& px, const Image& nx,
					 const Image& py, const Image& ny,
					 const Image& pz, const Image& nz);
	
	/**
	 * Loads the specified equirectangular image to skybox.
	 *
	 * \param i equirectangular image
	 */
	void load_skybox(const Image& i);
	
	/**
	 * Renders skybox using a camera. This function should be called in the
	 * first place. Otherwise it will overwrite the current render target.
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
	 */
	void unload_mesh(const Mesh& m);
	
	/**
	 * Clears all values from the image cache. The caches will be generated
	 * automatically when loading meshes.
	 */
	void clear_mesh_caches();
	
	/**
	 * Loads the specified image and creates corresponding texture. This function
	 * will invoke the texture callback.
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
	 * Clears all values from the image cache. The caches will be generated
	 * automatically when loading images.
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
	 * Clears all values from the scene cache. The caches will be generated
	 * automatically when loading meshes, images or scenes.
	 */
	void clear_scene_caches();
	
	/**
	 * Renders a scene using a camera. The results will be rendered to the
	 * specified render target.
	 *
	 * \param s scene
	 * \param c camera
	 */
	void render(const Scene& s, const Camera& c) const;
	
	/**
	 * Renders the transparent objects in a scene using a camera. The results
	 * will be rendered to the specified render target.
	 *
	 * \param s scene
	 * \param c camera
	 */
	void render_transparent(const Scene& s, const Camera& c) const;
	
	/**
	 * Renders a scene using the camera of shadow. The results will be rendered
	 * to the shadow map.
	 *
	 * \param s scene
	 * \param t target shadow
	 */
	void render_shadow(const Scene& s, const Shadow& t) const;
	
	/**
	 * Updates the shadow of spot light. This function will update the
	 * parameters of shadow's camera and render shadow map.
	 *
	 * \param s scene
	 * \param l spot light
	 */
	void update_shadow(const Scene& s, SpotLight& l) const;
	
	/**
	 * Updates the shadow of directional light. This function will update the
	 * parameters shadow'camera and render shadow map.
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
	 * Updates all the descendant instances in the scene before rendering the
	 * scene.
	 *
	 * \param s scene
	 */
	static void update_scene(Scene& s);
	
	/**
	 * Sets the defines object for material. The object will be defined in
	 * vertex, geometry and fragment shaders.
	 *
	 * \param m material
	 * \param d defines
	 */
	static void set_material_defines(const Material& m, Defines& d);
	
	/**
	 * Sets the defines object for scene. The object will be defined in vertex,
	 * geometry and fragment shaders.
	 *
	 * \param s scene
	 * \param d defines
	 */
	static void set_scene_defines(const Scene& s, Defines& d);
	
	/**
	 * Sets the defines object for tone mapping. The object will be defined in
	 * vertex, geometry and fragment shaders.
	 *
	 * \param m tone mapping mode
	 * \param d defines
	 */
	static void set_tone_map_defines(int m, Defines& d);
	
	/**
	 * Returns the uniforms object of material. The object will be passed to
	 * vertex, geometry and fragment shaders.
	 *
	 * \param s scene
	 * \param shader shader
	 */
	static void set_light_uniforms(const Scene& s, const Gpu::Shader& shader);
	
private:
	Vec4 clear_color = {0, 0, 0, 0};
	
	bool scissor_test = false;
	
	Gpu::Rect scissor = Gpu::Rect(0, 0, 0, 0);
	
	Gpu::Rect viewport = Gpu::Rect(0, 0, 0, 0);
	
	const Gpu::RenderTarget* target = nullptr;
	
	TextureCallback texture_callback = [](Gpu::Texture& t) -> void {
		t.generate_mipmap(); /* generate mipmap for every texture */
	};
	
	RenderingMode rendering_mode = DEFERRED_RENDERING;
	
	ToneMapMode tone_map_mode = LINEAR_TONE_MAP;
	
	float tone_map_exposure = 1;
	
	float skybox_intensity = 1;
	
	std::unique_ptr<Gpu::Texture> skybox_map;
	
	std::unordered_map<const Mesh*, std::unique_ptr<Gpu::VertexObject[]> > mesh_cache;
	
	std::unordered_map<const Image*, std::unique_ptr<Gpu::Texture> > image_cache;
	
	static std::unique_ptr<Gpu::VertexObject> cube;
	
	static std::unique_ptr<Gpu::Texture> probe_map;
	
	static std::unique_ptr<Gpu::RenderBuffer> probe_buffer;
	
	static std::unique_ptr<Gpu::RenderTarget> probe_target;
	
	void render_skybox_to_buffer(const Camera& c, RenderingMode r) const;
	
	void render_to_buffer(const Scene& s, const Camera& c, RenderingMode r, bool t) const;
	
	void render_to_shadow(const Scene& s, const Camera& c) const;
	
	static bool init_cube();
	
	static void sort_instances(const Camera& c, std::vector<const Instance*>& l, bool t);
};

}
