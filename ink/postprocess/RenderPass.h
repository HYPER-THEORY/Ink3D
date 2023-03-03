/**
 * Copyright (C) 2021-2023 Hypertheory
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

#include "../graphics/Gpu.h"

#include <functional>
#include <memory>

namespace Ink {

class RenderPass {
public:
	/**
	 * Creates a new RenderPass object.
	 */
	explicit RenderPass() = default;
	
	/**
	 * Initializes the render pass and prepare the resources for rendering.
	 */
	virtual void init() = 0;
	
	/**
	 * Compiles the required shaders and renders to the render target.
	 */
	virtual void render() const = 0;
	
	/**
	 * Returns the current render target if there is, returns nullptr otherwise.
	 */
	const Gpu::RenderTarget* get_target() const;
	
	/**
	 * Sets the render target. When nullptr is given, the default render target
	 * is set as the render target.
	 *
	 * \param t render target
	 */
	void set_target(const Gpu::RenderTarget* t);
	
	/**
	 * Returns the viewport region.
	 */
	static Gpu::Rect get_viewport();
	
	/**
	 * Sets the viewport region to render from (x, y) to (x + width, y +
	 * height).
	 *
	 * \param v viewport region
	 */
	static void set_viewport(const Gpu::Rect& v);
	
	/**
	 * Renders the full screen triangle with shader. The result will be rendered
	 * to the specified render target.
	 *
	 * \param s shader
	 * \param t render target
	 */
	static void render_to(const Gpu::Shader* s, const Gpu::RenderTarget* t);
	
protected:
	const Gpu::RenderTarget* target = nullptr;
	
	static Gpu::Rect viewport;
	
	static std::unique_ptr<Gpu::VertexObject> fullscreen_plane;
	
	static bool init_fullscreen_plane();
};

}
