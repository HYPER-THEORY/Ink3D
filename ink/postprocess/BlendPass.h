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

class BlendPass : public RenderPass {
public:
	/**
	 * Creates a new BlendPass object.
	 */
	BlendPass() = default;
	
	/**
	 * Initializes the render pass and prepares the resources for rendering.
	 */
	void init() override;
	
	/**
	 * Compiles the required shaders and renders to the render target.
	 */
	void render() override;
	
	/**
	 * Returns the 2D texture A that represents the input of the render pass.
	 */
	const gpu::Texture* get_texture_a() const;
	
	/**
	 * Sets the specified 2D texture A as the input of the render pass.
	 *
	 * \param t texture A
	 */
	void set_texture_a(const gpu::Texture* t);
	
	/**
	 * Returns the 2D texture B that represents the input of the render pass.
	 */
	const gpu::Texture* get_texture_b() const;
	
	/**
	 * Sets the specified 2D texture B as the input of the render pass.
	 *
	 * \param t texture B
	 */
	void set_texture_b(const gpu::Texture* t);
	
	/**
	 * Returns the operations when applying blending.
	 */
	std::string get_operation() const;
	
	/**
	 * Sets the operations when applying blending. The default is "a * b".
	 *
	 * \param o operation
	 */
	void set_operation(const std::string& o);
	
	/**
	 * Returns the swizzle of the texture A that represents the multiplication
	 * factor in blending.
	 */
	std::string get_swizzle_a() const;
	
	/**
	 * Sets the swizzle of the texture A as the multiplication factor in
	 * blending. The default is ".xyzw".
	 *
	 * \param s swizzle A
	 */
	void set_swizzle_a(const std::string& s);
	
	/**
	 * Returns the swizzle of the texture B that represents the multiplication
	 * factor in blending.
	 */
	std::string get_swizzle_b() const;
	
	/**
	 * Sets the swizzle of the texture B as the multiplication factor in
	 * blending. The default is ".xyzw".
	 *
	 * \param s swizzle B
	 */
	void set_swizzle_b(const std::string& s);
	
private:
	std::string operation = "a * b";
	std::string swizzle_a = ".xyzw";
	std::string swizzle_b = ".xyzw";
	
	const gpu::Texture* map_a = nullptr;
	const gpu::Texture* map_b = nullptr;
};

}
