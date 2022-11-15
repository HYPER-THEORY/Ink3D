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
	int samples = 32;        /**< sample numbers, must be 16, 32 or 64 */
	float radius = 0;        /**< radius to search for occluders */
	float max_radius = 0;    /**< the maximum radius from occluder to shading point */
	float intensity = 1;     /**< the intensity of ambient occlusion, range is 0 to 1 */
	float bias = 0.1;        /**< bias to avoid unrealistic effects */
	
	/**
	 * Creates a new SSAOPass (Screen Space Ambient Occlusion).
	 */
	explicit SSAOPass() = default;
	
	/**
	 * Creates a new SSAOPass (Screen Space Ambient Occlusion) and initializes
	 * it with the specified parameters.
	 *
	 * \param w the width of screen
	 * \param h the height of screen
	 * \param r radius to search for occluders
	 * \param m the maximum radius from occluder to shading point
	 * \param i the intensity of ambient occlusion, range is 0 to 1
	 */
	explicit SSAOPass(int w, int h, float r, float m, float i = 1);
	
	/**
	 * Initializes the render pass and prepare the resources for rendering.
	 */
	void init() override;
	
	/**
	 * Compiles if the shaders are not compiled yet. It will be automatically
	 * invoked by the process method.
	 */
	void compile() override;
	
	/**
	 * Renders to the render target after the shaders are compiled. It will be
	 * automatically invoked by the process method.
	 */
	void render() const override;
	
	/**
	 * Compiles the shaders and render to the render target.
	 */
	void process(const Camera& c);
	
	/**
	 * Returns the 2D texture represents the input of rendering pass.
	 */
	const Gpu::Texture* get_texture() const;
	
	/**
	 * Sets the specified 2D texture as the input of rendering pass.
	 *
	 * \param t input texture
	 */
	void set_texture(const Gpu::Texture* t);
	
	/**
	 * Returns the 2D texture represents the world normal buffer in G-Buffers.
	 */
	const Gpu::Texture* get_buffer_n() const;
	
	/**
	 * Sets the specified 2D texture as the world normal buffer in G-Buffers.
	 *
	 * \param n normal buffer texture
	 */
	void set_buffer_n(const Gpu::Texture* n);
	
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
