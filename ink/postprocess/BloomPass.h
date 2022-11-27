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

#include "RenderPass.h"

namespace Ink {

class BloomPass : public RenderPass {
public:
	int width = 0;            /**< the width of screen */
	int height = 0;           /**< the height of screen */
	float threshold = 1;      /**< the threshold of luminance to affect bloom */
	float intensity = 1;      /**< the intensity of bloom effect */
	float radius = 0.5;       /**< the radius of bloom effect, range is 0 to 1 */
	Vec3 tint = {1, 1, 1};    /**< the tint modify the bloom color */
	
	/**
	 * Creates a new BloomPass object.
	 */
	explicit BloomPass() = default;
	
	/**
	 * Creates a new BloomPass object and initializes it with the specified
	 * parameters.
	 *
	 * \param w the width of screen
	 * \param h the height of screen
	 * \param t the threshold of luminance to affect bloom
	 * \param i the intensity of bloom effect
	 * \param r the radius of bloom effect, range is 0 to 1
	 */
	explicit BloomPass(int w, int h, float t = 1, float i = 1, float r = 1);
	
	/**
	 * Initializes the render pass and prepare the resources for rendering.
	 */
	void init() override;
	
	/**
	 * Compiles the required shaders and renders to the render target.
	 */
	void render() const override;
	
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
	
private:
	const Gpu::Texture* map = nullptr;
	
	std::unique_ptr<Gpu::Texture> bloom_map_1;
	std::unique_ptr<Gpu::Texture> bloom_map_2;
	
	std::unique_ptr<Gpu::FrameBuffer> bloom_target;
};

}
