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

#include "RenderPass.h"

namespace Ink {

const Gpu::RenderTarget* RenderPass::get_target() const {
	return target;
}

void RenderPass::set_target(const Gpu::RenderTarget* t) {
	target = t;
}

Gpu::Rect RenderPass::get_viewport() {
	return viewport;
}

void RenderPass::set_viewport(const Gpu::Rect& v) {
	viewport = v;
}

void RenderPass::render_to(const Gpu::Shader* s, const Gpu::RenderTarget* t) {
	/* initialize render_to */
	[[maybe_unused]]
	static bool inited = init_render_to();
	
	/* activate render target */
	Gpu::RenderTarget::activate(t);
	
	/* disable depth & stencil & scissor test */
	Gpu::State::disable_depth_test();
	Gpu::State::disable_stencil_test();
	Gpu::State::disable_scissor_test();
	
	/* disable blending & wireframe & culling */
	Gpu::State::disable_blending();
	Gpu::State::disable_wireframe();
	Gpu::State::disable_culling();
	
	/* set the viewport region */
	Gpu::State::set_viewport(viewport);
	
	/* draw the plane with shader */
	plane->attach(*s);
	plane->render();
	
	/* set to default render target */
	Gpu::RenderTarget::activate(nullptr);
}

bool RenderPass::init_render_to() {
	/* prepare plane mesh */
	Mesh plane_mesh = Mesh("plane");
	plane_mesh.groups = {{"default", 0, 3}};
	plane_mesh.vertex = {{-1, 3, 0}, {-1, -1, 0}, {3, -1, 0}};
	plane_mesh.uv = {{0, 2}, {0, 0}, {2, 0}};
	
	/* prepare plane vertex object */
	plane = std::make_unique<Gpu::VertexObject>();
	plane->load(plane_mesh, plane_mesh.groups[0]);
	
	return true; /* finish */
}

Gpu::Rect RenderPass::viewport;

std::unique_ptr<Gpu::VertexObject> RenderPass::plane;

}
