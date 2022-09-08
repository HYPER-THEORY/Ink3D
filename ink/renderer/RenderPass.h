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

#include <functional>

#include "../graphics/Gpu.h"

namespace Ink {

class RenderPass {
public:
	/**
	 * Create a new RenderPass.
	 */
	RenderPass() = default;
	
	/**
	 * Initialize the render pass and prepare the resources for rendering.
	 */
	virtual void init() = 0;
	
	/**
	 * Compile if the shaders are not compiled yet. It will be automatically
	 * invoked by the process method.
	 */
	virtual void compile() = 0;
	
	/**
	 * Render to the render target after the shaders are compiled. It will be
	 * automatically invoked by the process method.
	 */
	virtual void render() const = 0;
	
	/**
	 * Compile the shaders and render to the render target.
	 */
	virtual void process();
	
	/**
	 * Returns the current render target if there are, returns nullptr
	 * otherwise.
	 */
	const Gpu::FrameBuffer* get_target() const;
	
	/**
	 * Sets the render target. When nullptr is given, the default frame buffer
	 * is set as the render target.
	 *
	 * \param t target frame buffer
	 */
	void set_target(const Gpu::FrameBuffer* t);
	
	/**
	 * Returns the current viewport.
	 */
	static Gpu::Rect get_viewport();
	
	/**
	 * Sets the viewport.
	 *
	 * \param v viewport
	 */
	static void set_viewport(const Gpu::Rect& v);
	
	/**
	 * Determines whether to enable scissor test.
	 */
	static bool get_scissor_test();
	
	/**
	 * Determines whether to enable scissor test.
	 *
	 * \param t enable scissor test
	 */
	static void set_scissor_test(bool t);
	
	/**
	 * Returns the current scissor region.
	 */
	static Gpu::Rect get_scissor();
	
	/**
	 * Sets the scissor region.
	 *
	 * \param s scissor region
	 */
	static void set_scissor(const Gpu::Rect& s);
	
	/**
	 * Render full screen to the specified render target.
	 *
	 * \param s shader
	 * \param t render target
	 */
	static void render_to(const Gpu::Shader* s, const Gpu::FrameBuffer* t);

protected:
	const Gpu::FrameBuffer* target = nullptr;
	
	static bool scissor_test;
	
	static Gpu::Rect scissor;
	
	static Gpu::Rect viewport;
	
	static std::unique_ptr<Gpu::VertexObject> plane;
	
	static bool init_render_to();
};

}
