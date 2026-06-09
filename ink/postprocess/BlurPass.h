/**
 * Copyright (C) 2021-2023 HYPERTHEORY
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

#include "RenderPass.h"

namespace ink {

enum BlurType {
	BLUR_BOX,
	BLUR_GAUSSIAN,
	BLUR_BILATERAL,
};

class BlurPass : public RenderPass {
public:
	BlurType type = BLUR_BOX;    /**< the type of the blur */
	int width = 0;               /**< the width of the screen */
	int height = 0;              /**< the height of the screen */
	int channel = 4;             /**< the channel number of the texture */
	int radius = 3;              /**< the radius of the blur */
	float sigma_s = 2.0;         /**< the spatial sigma, for Gaussian or bilateral blur */
	float sigma_r = 0.25;        /**< the range sigma, only for bilateral blur */
	
	/**
	 * Creates a new BlurPass object.
	 */
	BlurPass() = default;
	
	/**
	 * Creates a new BlurPass and initializes it with the size of the screen.
	 *
	 * \param w the width of the screen
	 * \param h the height of the screen
	 */
	BlurPass(int w, int h);
	
	/**
	 * Initializes the render pass and prepares the resources for rendering.
	 */
	void init() override;
	
	/**
	 * Compiles the required shaders and renders to the render target.
	 */
	void render() override;
	
	/**
	 * Returns the 2D texture that represents the input of the render pass.
	 */
	const gpu::Texture* get_texture() const;
	
	/**
	 * Sets the specified 2D texture as the input of the render pass.
	 *
	 * \param t input texture
	 */
	void set_texture(const gpu::Texture* t);
	
private:
	const gpu::Texture* map = nullptr;
	
	std::unique_ptr<gpu::Texture> blur_map_1;
	std::unique_ptr<gpu::Texture> blur_map_2;
	
	std::unique_ptr<gpu::RenderTarget> blur_target_1;
	std::unique_ptr<gpu::RenderTarget> blur_target_2;
};

}
