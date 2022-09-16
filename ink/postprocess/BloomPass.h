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

#include "../renderer/RenderPass.h"

namespace Ink {

class BloomPass : public RenderPass {
public:
	int width = 0;            /**< the width of screen */
	int height = 0;           /**< the height of screen */
	float threshold = 1;      /**< the threshold of luminance to affect bloom */
	float intensity = 1;      /**< the intensity of bloom effect */
	float radius = 1;         /**< the radius of bloom effect, range is 0 to 1 */
	Vec3 tint = {1, 1, 1};    /**< the tint modify the bloom color */
	
	/**
	 * Create a new BloomPass.
	 */
	BloomPass() = default;
	
	/**
	 * Create a new BloomPass with the specified parameters.
	 *
	 * \param w the width of screen
	 * \param h the height of screen
	 * \param t the threshold of luminance to affect bloom
	 * \param i the intensity of bloom effect
	 * \param r the radius of bloom effect, range is 0 to 1
	 */
	BloomPass(int w, int h, float t = 1, float i = 1, float r = 1);
	
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
	const Gpu::Texture* map = nullptr;
	
	std::unique_ptr<Gpu::Texture> bloom_map_1;
	std::unique_ptr<Gpu::Texture> bloom_map_2;
	
	std::unique_ptr<Gpu::FrameBuffer> bloom_target;
	
	std::unique_ptr<Gpu::Shader> bright_pass_shader;
	std::unique_ptr<Gpu::Shader> blur_shader;
	std::unique_ptr<Gpu::Shader> bloom_shader;
};

}
