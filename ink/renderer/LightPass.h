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
	LINEAR_TONE_MAPPING,
	REINHARD_TONE_MAPPING,
	OPTIMIZED_TONE_MAPPING,
	ACES_FILMIC_TONE_MAPPING,
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
	 * Returns the G-Buffer color texture.
	 */
	const Gpu::Texture* get_gbuffer_c() const;
	
	/**
	 * Sets the G-Buffer color texture.
	 *
	 * \param t G-Buffer color texture
	 */
	void set_gbuffer_c(const Gpu::Texture* t);
	
	/**
	 * Returns the G-Buffer normal texture.
	 */
	const Gpu::Texture* get_gbuffer_n() const;
	
	/**
	 * Sets the G-Buffer normal texture.
	 *
	 * \param t G-Buffer normal texture
	 */
	void set_gbuffer_n(const Gpu::Texture* t);
	
	/**
	 * Returns the G-Buffer material texture.
	 */
	const Gpu::Texture* get_gbuffer_m() const;
	
	/**
	 * Sets the G-Buffer material texture.
	 *
	 * \param t G-Buffer material texture
	 */
	void set_gbuffer_m(const Gpu::Texture* t);
	
	/**
	 * Returns the G-Buffer depth texture.
	 */
	const Gpu::Texture* get_gbuffer_d() const;
	
	/**
	 * Sets the G-Buffer depth texture.
	 *
	 * \param d G-Buffer depth texture
	 */
	void set_gbuffer_d(const Gpu::Texture* d);
	
	/**
	 * Returns the G-Buffer emissive texture.
	 */
	const Gpu::Texture* get_gbuffer_e() const;
	
	/**
	 * Sets the G-Buffer emissive texture.
	 *
	 * \param t G-Buffer emissive texture
	 */
	void set_gbuffer_e(const Gpu::Texture* t);
	
	/**
	 * Returns the G-Buffer indirect light texture.
	 */
	const Gpu::Texture* get_gbuffer_i() const;
	
	/**
	 * Sets the G-Buffer indirect light texture.
	 *
	 * \param t G-Buffer indirect light texture
	 */
	void set_gbuffer_i(const Gpu::Texture* t);
	
	/**
	 * Sets the mode and exposure of tone mapping.
	 *
	 * \param m tone mapping mode
	 * \param e tone mapping exposure
	 */
	void set_tone_mapping(int m, float e);
	
private:
	int tone_map_mode = LINEAR_TONE_MAPPING;
	
	float tone_map_exposure = 1;
	
	const Scene* scene = nullptr;
	
	const Camera* camera = nullptr;
	
	const Gpu::Texture* gbuffer_c = nullptr;
	
	const Gpu::Texture* gbuffer_n = nullptr;
	
	const Gpu::Texture* gbuffer_m = nullptr;
	
	const Gpu::Texture* gbuffer_d = nullptr;
	
	const Gpu::Texture* gbuffer_e = nullptr;
	
	const Gpu::Texture* gbuffer_i = nullptr;
	
	std::unique_ptr<Gpu::Shader> light_shader;
	
	using RenderPass::process;
};

}
