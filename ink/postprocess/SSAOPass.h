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

#include "../camera/Camera.h"
#include "../renderer/RenderPass.h"

#pragma once

namespace Ink {

class SSAOPass : public RenderPass {
public:
	int width = 0;           /**< the width of screen */
	int height = 0;          /**< the height of screen */
	float radius = 0;        /**< radius to search for occluders */
	float max_radius = 0;    /**< the maximum radius from occluder to shading point */
	float darkness = 1;      /**< the darkness of ambient occlusion, range is 0 to 1 */
	float bias = 0.2;        /**< bias to avoid unrealistic effects */
	int samples = 32;        /**< sample times, must be 16, 32 or 64 */
	
	/**
	 * Create a new SSAOPass (Screen Space Ambient Occlusion).
	 */
	SSAOPass() = default;
	
	/**
	 * Create a new SSAOPass (Screen Space Ambient Occlusion) with the specified
	 * parameters.
	 *
	 * \param w the width of screen
	 * \param h the height of screen
	 * \param r radius to search for occluders
	 * \param m the maximum radius from occluder to shading point
	 * \param d the darkness of ambient occlusion, range is 0 to 1
	 */
	SSAOPass(int w, int h, float r = 0, float m = 0, float d = 1);
	
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
	 */
	void process(const Camera& c);
	
	/**
	 * Returns the G-Buffer normal texture.
	 */
	const Gpu::Texture* get_buffer_n() const;
	
	/**
	 * Sets the G-Buffer normal texture.
	 *
	 * \param n G-Buffer normal texture
	 */
	void set_buffer_n(const Gpu::Texture* n);
	
	/**
	 * Returns the G-Buffer depth texture.
	 */
	const Gpu::Texture* get_buffer_d() const;
	
	/**
	 * Sets the G-Buffer depth texture.
	 *
	 * \param d G-Buffer depth texture
	 */
	void set_buffer_d(const Gpu::Texture* d);
	
	/**
	 * Returns the texture as the input of post processing.
	 */
	const Gpu::Texture* get_texture() const;
	
	/**
	 * Sets the texture as the input of post processing.
	 *
	 * \param t input texture
	 */
	void set_texture(const Gpu::Texture* t);
	
private:
	const Camera* camera = nullptr;
	
	const Gpu::Texture* map = nullptr;
	const Gpu::Texture* buffer_n = nullptr;
	const Gpu::Texture* buffer_d = nullptr;
	
	std::unique_ptr<Gpu::Texture> blur_map_1;
	std::unique_ptr<Gpu::Texture> blur_map_2;
	
	std::unique_ptr<Gpu::FrameBuffer> blur_target_1;
	std::unique_ptr<Gpu::FrameBuffer> blur_target_2;
	
	std::unique_ptr<Gpu::Shader> ssao_shader;
	std::unique_ptr<Gpu::Shader> blur_shader;
	std::unique_ptr<Gpu::Shader> blend_shader;
	
	using RenderPass::process;
};

}
