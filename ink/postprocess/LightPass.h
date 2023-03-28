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

#include "../scene/Scene.h"
#include "../renderer/Renderer.h"

#include "RenderPass.h"

namespace Ink {

class LightPass : public RenderPass {
public:
	/**
	 * Creates a new LightPass object.
	 */
	explicit LightPass() = default;
	
	/**
	 * Initializes the render pass and prepares the resources for rendering.
	 */
	void init() override;
	
	/**
	 * Compiles the required shaders and renders to the render target.
	 */
	void render() override;
	
	/**
	 * Returns the mode in tone mapping.
	 */
	int get_tone_map_mode() const;
	
	/**
	 * Returns the exposure in tone mapping.
	 */
	float get_tone_map_exposure() const;
	
	/**
	 * Sets the specified mode and exposure in tone mapping. Default is
	 * LINEAR_TONE_MAP and 1.
	 *
	 * \param m tone mapping mode
	 * \param e tone mapping exposure
	 */
	void set_tone_map(int m, float e);
	
	/**
	 * Returns the scene represents the input of render pass.
	 */
	const Scene* get_scene() const;
	
	/**
	 * Sets the specified scene as the input of render pass.
	 *
	 * \param s scene
	 */
	void set_scene(const Scene* s);
	
	/**
	 * Returns the camera represents the input of render pass.
	 */
	const Camera* get_camera() const;
	
	/**
	 * Sets the specified camera as the input of render pass.
	 *
	 * \param c camera
	 */
	void set_camera(const Camera* c);
	
	/**
	 * Returns the 2D texture represents the base color buffer in G-Buffers.
	 */
	const Gpu::Texture* get_texture_color() const;
	
	/**
	 * Sets the specified 2D texture as the base color buffer in G-Buffers.
	 *
	 * \param t base color texture
	 */
	void set_texture_color(const Gpu::Texture* t);
	
	/**
	 * Returns the 2D texture represents the world normal buffer in G-Buffers.
	 */
	const Gpu::Texture* get_texture_normal() const;
	
	/**
	 * Sets the specified 2D texture as the world normal buffer in G-Buffers.
	 *
	 * \param t world normal texture
	 */
	void set_texture_normal(const Gpu::Texture* t);
	
	/**
	 * Returns the 2D texture represents the material data buffer in G-Buffers.
	 */
	const Gpu::Texture* get_texture_material() const;
	
	/**
	 * Sets the specified 2D texture as the material data buffer in G-Buffers.
	 *
	 * \param t material data texture
	 */
	void set_texture_material(const Gpu::Texture* t);
	
	/**
	 * Returns the 2D texture represents the indirect light buffer in G-Buffers.
	 */
	const Gpu::Texture* get_texture_light() const;
	
	/**
	 * Sets the specified 2D texture as the indirect light buffer in G-Buffers.
	 *
	 * \param t indirect light texture
	 */
	void set_texture_light(const Gpu::Texture* t);
	
	/**
	 * Returns the 2D texture represents the depth buffer / Z-Buffer.
	 */
	const Gpu::Texture* get_texture_depth() const;
	
	/**
	 * Sets the specified 2D texture as the depth buffer / Z-Buffer.
	 *
	 * \param d depth texture
	 */
	void set_texture_depth(const Gpu::Texture* d);
	
private:
	int tone_map_mode = LINEAR_TONE_MAP;
	
	float tone_map_exposure = 1;
	
	const Scene* scene = nullptr;
	
	const Camera* camera = nullptr;
	
	const Gpu::Texture* g_color = nullptr;
	
	const Gpu::Texture* g_normal = nullptr;
	
	const Gpu::Texture* g_material = nullptr;
	
	const Gpu::Texture* g_light = nullptr;
	
	const Gpu::Texture* z_map = nullptr;
};

}
