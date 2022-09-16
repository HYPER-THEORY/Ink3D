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

#include "../scene/Scene.h"

#include "RenderPass.h"

namespace Ink {

enum ToneMappingMode {
	TONE_MAPPING_LINEAR,
	TONE_MAPPING_REINHARD,
	TONE_MAPPING_OPTIMIZED,
	TONE_MAPPING_ACES_FILMIC,
};

class LightPass : public RenderPass {
public:
	/**
	 * Create a new LightPass.
	 */
	LightPass() = default;
	
	/**
	 * Initialize the render pass and prepare the resources for rendering.
	 */
	void init() override;
	
	/**
	 * Compile if the shaders are not compiled yet. It will be automatically
	 * invoked by the process method.
	 */
	void compile() override;
	
	/**
	 * Render to the render target after the shaders are compiled. It will be
	 * automatically invoked by the process method.
	 */
	void render() const override;
	
	/**
	 * Compile the shaders and render to the render target.
	 *
	 * \param s scene
	 * \param c camera
	 */
	void process(const Scene& s, const Camera& c);
	
	/**
	 * Returns the G-Buffer diffuse color texture.
	 */
	const Gpu::Texture* get_buffer_c() const;
	
	/**
	 * Sets the G-Buffer diffuse color texture.
	 *
	 * \param t diffuse color texture
	 */
	void set_buffer_c(const Gpu::Texture* t);
	
	/**
	 * Returns the G-Buffer world normal texture.
	 */
	const Gpu::Texture* get_buffer_n() const;
	
	/**
	 * Sets the G-Buffer world normal texture.
	 *
	 * \param t world normal texture
	 */
	void set_buffer_n(const Gpu::Texture* t);
	
	/**
	 * Returns the G-Buffer material texture.
	 */
	const Gpu::Texture* get_buffer_m() const;
	
	/**
	 * Sets the G-Buffer material texture.
	 *
	 * \param t material texture
	 */
	void set_buffer_m(const Gpu::Texture* t);
	
	/**
	 * Returns the G-Buffer light color texture.
	 */
	const Gpu::Texture* get_buffer_l() const;
	
	/**
	 * Sets the G-Buffer light color texture.
	 *
	 * \param t light color texture
	 */
	void set_buffer_l(const Gpu::Texture* t);
	
	/**
	 * Returns the G-Buffer depth texture.
	 */
	const Gpu::Texture* get_buffer_d() const;
	
	/**
	 * Sets the G-Buffer depth texture.
	 *
	 * \param d depth texture
	 */
	void set_buffer_d(const Gpu::Texture* d);
	
	/**
	 * Sets the mode and exposure of tone mapping.
	 *
	 * \param m tone mapping mode
	 * \param e tone mapping exposure
	 */
	void set_tone_mapping(int m, float e);
	
private:
	int tone_mapping = TONE_MAPPING_LINEAR;
	
	float tone_mapping_exposure = 1;
	
	const Scene* scene = nullptr;
	
	const Camera* camera = nullptr;
	
	const Gpu::Texture* buffer_c = nullptr;
	
	const Gpu::Texture* buffer_n = nullptr;
	
	const Gpu::Texture* buffer_m = nullptr;
	
	const Gpu::Texture* buffer_l = nullptr;
	
	const Gpu::Texture* buffer_d = nullptr;
	
	std::unique_ptr<Gpu::Shader> light_shader;
	
	using RenderPass::process;
};

}
