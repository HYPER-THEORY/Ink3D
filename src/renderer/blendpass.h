/*
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

#include <memory>

#include "../math/maths.h"
#include "../graphics/gpu.h"

#include "renderpass.h"

class blendpass {
public:
	/**
	 * Init blendpass.
	 */
	static void init();
	
	/**
	 * Create a new blendpass.
	 */
	blendpass() = default;
	
	/**
	 * Returns the current target framebuffer if there are, returns nullptr
	 * otherwise.
	 */
	const gpu::framebuffer* get_target() const;
	
	/**
	 * Sets the target framebuffer. When nullptr is given, the default
	 * framebuffer is set as the target framebuffer.
	 *
	 * \param t target framebuffer
	 */
	void set_target(const gpu::framebuffer* t);
	
	/**
	 * Returns A texture.
	 */
	const gpu::texture* get_a() const;
	
	/**
	 * Sets A texture as the blending source.
	 *
	 * \param t A texture
	 * \param f whether only use the red channel of the texture as factors
	 */
	void set_a(const gpu::texture* t, bool f = false);
	
	/**
	 * Returns B texture.
	 */
	const gpu::texture* get_b() const;
	
	/**
	 * Sets B texture as the blending source.
	 *
	 * \param t B texture
	 * \param f whether only use the red channel of the texture as factors
	 */
	void set_b(const gpu::texture* t, bool f = false);
	
	/**
	 * Returns B texture.
	 */
	const gpu::texture* get_c() const;
	
	/**
	 * Sets C texture as the blending source.
	 *
	 * \param t C texture
	 * \param f whether only use the red channel of the texture as factors
	 */
	void set_c(const gpu::texture* t, bool f = false);
	
	/**
	 * Returns D texture.
	 */
	const gpu::texture* get_d() const;
	
	/**
	 * Sets D texture as the blending source.
	 *
	 * \param t D texture
	 * \param f whether only use the red channel of the texture as factors
	 */
	void set_d(const gpu::texture* t, bool f = false);
	
	/**
	 * Render a blended texture of the source textures.
	 */
	void render() const;
	
private:
	bool a_is_factor = false;
	bool b_is_factor = false;
	bool c_is_factor = false;
	bool d_is_factor = false;
	
	const gpu::texture* a_map = nullptr;
	const gpu::texture* b_map = nullptr;
	const gpu::texture* c_map = nullptr;
	const gpu::texture* d_map = nullptr;
	
	const gpu::framebuffer* target = nullptr;
	
	static std::unique_ptr<gpu::shader> blend_shader;
};
