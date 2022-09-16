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

class FXAAPass : public RenderPass {
public:
	/**
	 * Create a new FXAAPass (Fast Approximate Anti-Aliasing).
	 */
	FXAAPass() = default;
	
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
	 * Sets the texture as the input of post processing. Insure the texture is
	 * set to linear filtering.
	 *
	 * \param t input texture
	 */
	void set_texture(const Gpu::Texture* t);
	
private:
	const Gpu::Texture* map = nullptr;
	
	std::unique_ptr<Gpu::Shader> fxaa_shader;
};

}
