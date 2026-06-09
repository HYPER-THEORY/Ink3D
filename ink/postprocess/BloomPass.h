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

class BloomPass : public RenderPass {
public:
	int width = 0;            /**< the width of the screen */
	int height = 0;           /**< the height of the screen */
	float threshold = 1;      /**< the threshold of the luminance to affect bloom */
	float intensity = 1;      /**< the intensity of the bloom effect */
	float radius = 0.5;       /**< the radius of the bloom effect, range is 0 to 1 */
	Vec3 tint = {1, 1, 1};    /**< the tint which modifies the bloom color */
	
	/**
	 * Creates a new BloomPass object.
	 */
	BloomPass() = default;
	
	/**
	 * Creates a new BloomPass object and initializes it with the specified
	 * parameters.
	 *
	 * \param w the width of the screen
	 * \param h the height of the screen
	 * \param t the threshold of the luminance to affect bloom
	 * \param i the intensity of the bloom effect
	 * \param r the radius of the bloom effect, range is 0 to 1
	 */
	BloomPass(int w, int h, float t = 1, float i = 1, float r = 0.5);
	
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
	
	std::unique_ptr<gpu::Texture> bloom_map_1;
	std::unique_ptr<gpu::Texture> bloom_map_2;
	
	std::unique_ptr<gpu::RenderTarget> bloom_target;
};

}
