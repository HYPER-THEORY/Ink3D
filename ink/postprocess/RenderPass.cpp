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

#include "RenderPass.h"

namespace ink {

const gpu::RenderTarget* RenderPass::get_target() const {
	return target;
}

void RenderPass::set_target(const gpu::RenderTarget* t) {
	target = t;
}

gpu::Rect RenderPass::get_viewport() {
	return viewport;
}

void RenderPass::set_viewport(const gpu::Rect& v) {
	viewport = v;
}

void RenderPass::render_to(const gpu::Shader* s, const gpu::RenderTarget* t) {
	/* initialize fullscreen plane vertex object */
	if (!fullscreen_plane) init_fullscreen_plane();
	
	/* activate render target */
	gpu::RenderTarget::activate(t);
	
	/* set the states of GPU pipeline */
	gpu::State::disable_depth_test();
	gpu::State::disable_stencil_test();
	gpu::State::disable_scissor_test();
	gpu::State::disable_blending();
	gpu::State::disable_culling();
	gpu::State::disable_wireframe();
	
	/* set the viewport region */
	gpu::State::set_viewport(viewport);
	
	/* draw the fullscreen plane with shader */
	fullscreen_plane->attach(*s);
	fullscreen_plane->render();
	
	/* set to default render target */
	gpu::RenderTarget::activate(nullptr);
}

void RenderPass::init_fullscreen_plane() {
	/* prepare triangle mesh */
	Mesh triangle_mesh = Mesh("fullscreen");
	triangle_mesh.groups = {{"default", 0, 3}};
	triangle_mesh.vertex = {{-1, 3, 0}, {-1, -1, 0}, {3, -1, 0}};
	triangle_mesh.uv = {{0, 2}, {0, 0}, {2, 0}};
	
	/* prepare fullscreen plane vertex object */
	fullscreen_plane = std::make_unique<gpu::VertexObject>();
	fullscreen_plane->load(triangle_mesh, triangle_mesh.groups[0]);
}

gpu::Rect RenderPass::viewport;

std::unique_ptr<gpu::VertexObject> RenderPass::fullscreen_plane;

}
