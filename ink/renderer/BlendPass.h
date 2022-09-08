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
	 * Create a new BlendPass.
	 */
	BlendPass() = default;
	
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
	 * Returns the texture A.
	 */
	const Gpu::Texture* get_texture_a() const;
	
	/**
	 * Sets the texture A as the blending input.
	 *
	 * \param t texture A
	 */
	void set_texture_a(const Gpu::Texture* t);
	
	/**
	 * Sets the swizzle of texture A as the multiplication factor when blending.
	 *
	 * \param s swizzle A
	 */
	void set_swizzle_a(const std::string& s);
	
	/**
	 * Returns the texture B.
	 */
	const Gpu::Texture* get_texture_b() const;
	
	/**
	 * Sets the texture B as the blending input.
	 *
	 * \param t texture B
	 */
	void set_texture_b(const Gpu::Texture* t);
	
	/**
	 * Sets the swizzle of texture A as the multiplication factor when blending.
	 *
	 * \param s swizzle B
	 */
	void set_swizzle_b(const std::string& s);
	
	/**
	 * Returns the texture C.
	 */
	const Gpu::Texture* get_texture_c() const;
	
	/**
	 * Sets the texture C as the blending input.
	 *
	 * \param t texture C
	 */
	void set_texture_c(const Gpu::Texture* t);
	
	/**
	 * Sets the swizzle of texture A as the multiplication factor when blending.
	 *
	 * \param s swizzle C
	 */
	void set_swizzle_c(const std::string& s);
	
	/**
	 * Returns the texture D.
	 */
	const Gpu::Texture* get_texture_d() const;
	
	/**
	 * Sets the texture D as the blending input.
	 *
	 * \param t texture D
	 */
	void set_texture_d(const Gpu::Texture* t);
	
	/**
	 * Sets the swizzle of texture A as the multiplication factor when blending.
	 *
	 * \param s swizzle D
	 */
	void set_swizzle_d(const std::string& s);
	
	/**
	 * Sets the operations when applying blending. Default is "a * b".
	 */
	void set_operation(const std::string& o);
	
private:
	std::string swizzle_a = ".xyzw";
	std::string swizzle_b = ".xyzw";
	std::string swizzle_c = ".xyzw";
	std::string swizzle_d = ".xyzw";
	
	const Gpu::Texture* map_a = nullptr;
	const Gpu::Texture* map_b = nullptr;
	const Gpu::Texture* map_c = nullptr;
	const Gpu::Texture* map_d = nullptr;
	
	std::string operation = "a * b";
	
	std::unique_ptr<Gpu::Shader> blend_shader;
};

}
