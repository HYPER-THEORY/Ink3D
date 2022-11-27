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

class BlendPass : public RenderPass {
public:
	/**
	 * Creates a new BlendPass object.
	 */
	explicit BlendPass() = default;
	
	/**
	 * Initializes the render pass and prepare the resources for rendering.
	 */
	void init() override;
	
	/**
	 * Compiles the required shaders and renders to the render target.
	 */
	void render() const override;
	
	/**
	 * Returns the 2D texture A represents the input of rendering pass.
	 */
	const Gpu::Texture* get_texture_a() const;
	
	/**
	 * Sets the specified 2D texture A as the input of rendering pass.
	 *
	 * \param t texture A
	 */
	void set_texture_a(const Gpu::Texture* t);
	
	/**
	 * Returns the 2D texture B represents the input of rendering pass.
	 */
	const Gpu::Texture* get_texture_b() const;
	
	/**
	 * Sets the specified 2D texture B as the input of rendering pass.
	 *
	 * \param t texture B
	 */
	void set_texture_b(const Gpu::Texture* t);
	
	/**
	 * Sets the operations when applying blending. Default is "a * b".
	 *
	 * \param o operation
	 */
	void set_operation(const std::string& o);
	
	/**
	 * Sets the swizzle of texture A as the multiplication factor in blending.
	 * Default is ".xyzw".
	 *
	 * \param s swizzle A
	 */
	void set_swizzle_a(const std::string& s);
	
	/**
	 * Sets the swizzle of texture B as the multiplication factor in blending.
	 * Default is ".xyzw".
	 *
	 * \param s swizzle B
	 */
	void set_swizzle_b(const std::string& s);
	
private:
	std::string operation = "a * b";
	std::string swizzle_a = ".xyzw";
	std::string swizzle_b = ".xyzw";
	
	const Gpu::Texture* map_a = nullptr;
	const Gpu::Texture* map_b = nullptr;
};

}
