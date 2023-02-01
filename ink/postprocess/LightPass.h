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
	void render() const override;
	
	/**
	 * Sets the specified parameters to render pass before the rendering starts.
	 *
	 * \param s scene
	 * \param c camera
	 */
	void set(const Scene* s, const Camera* c);
	
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
	 * Returns the 2D texture represents the diffuse color buffer in G-Buffers.
	 */
	const Gpu::Texture* get_buffer_c() const;
	
	/**
	 * Sets the specified 2D texture as the diffuse color buffer in G-Buffers.
	 *
	 * \param t color buffer texture
	 */
	void set_buffer_c(const Gpu::Texture* t);
	
	/**
	 * Returns the 2D texture represents the world normal buffer in G-Buffers.
	 */
	const Gpu::Texture* get_buffer_n() const;
	
	/**
	 * Sets the specified 2D texture as the world normal buffer in G-Buffers.
	 *
	 * \param t normal buffer texture
	 */
	void set_buffer_n(const Gpu::Texture* t);
	
	/**
	 * Returns the 2D texture represents the material buffer in G-Buffers.
	 */
	const Gpu::Texture* get_buffer_m() const;
	
	/**
	 * Sets the specified 2D texture as the material buffer in G-Buffers.
	 *
	 * \param t material buffer texture
	 */
	void set_buffer_m(const Gpu::Texture* t);
	
	/**
	 * Returns the 2D texture represents the additional buffer in G-Buffers.
	 */
	const Gpu::Texture* get_buffer_a() const;
	
	/**
	 * Sets the specified 2D texture as the additional buffer in G-Buffers.
	 *
	 * \param t additional buffer texture
	 */
	void set_buffer_a(const Gpu::Texture* t);
	
	/**
	 * Returns the 2D texture represents the depth buffer in G-Buffers.
	 */
	const Gpu::Texture* get_buffer_d() const;
	
	/**
	 * Sets the specified 2D texture as the depth buffer in G-Buffers.
	 *
	 * \param d depth buffer texture
	 */
	void set_buffer_d(const Gpu::Texture* d);
	
private:
	int tone_map_mode = LINEAR_TONE_MAP;
	
	float tone_map_exposure = 1;
	
	const Scene* scene = nullptr;
	
	const Camera* camera = nullptr;
	
	const Gpu::Texture* buffer_c = nullptr;
	
	const Gpu::Texture* buffer_n = nullptr;
	
	const Gpu::Texture* buffer_m = nullptr;
	
	const Gpu::Texture* buffer_a = nullptr;
	
	const Gpu::Texture* buffer_d = nullptr;
};

}
