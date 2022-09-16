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

#include "../renderer/RenderPass.h"

#pragma once

namespace Ink {

class ColorGradingPass : public RenderPass {
public:
	Vec3 saturation = {1, 1, 1};    /**< the color purity in color adjustments */
	Vec3 contrast = {1, 1, 1};      /**< the tonal range in color adjustments */
	Vec3 gamma = {1, 1, 1};         /**< the midtones in color adjustments */
	Vec3 gain = {1, 1, 1};          /**< the highlights in color adjustments */
	Vec3 offset = {0, 0, 0};        /**< the shadows in color adjustments */
	
	/**
	 * Create a new ColorGradingPass.
	 */
	ColorGradingPass() = default;
	
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
	
	std::unique_ptr<Gpu::Shader> color_grade_shader;
};

}

