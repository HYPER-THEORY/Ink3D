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

#include "renderpass.h"
#include "../meshes/quadmesh.h"

void renderpass::init() {
	mesh plane_mesh;
	plane_mesh.groups = {{"default", 0, 3}};
	plane_mesh.material_groups = {{"default", 0, 3}};
	plane_mesh.vertex = {{-1, 3, 0}, {-1, -1, 0}, {3, -1, 0}};
	plane_mesh.uv = {{0, 2}, {0, 0}, {2, 0}};
	quad = std::make_unique<gpu::vertexobject>();
	quad->preload(plane_mesh);
}

gpu::rect renderpass::get_viewport() {
	return viewport;
}

void renderpass::set_viewport(const gpu::rect& v) {
	viewport = v;
}

bool renderpass::get_scissor_test() {
	return scissor_test;
}

void renderpass::set_scissor_test(bool t) {
	scissor_test = t;
}

gpu::rect renderpass::get_scissor() {
	return scissor;
}

void renderpass::set_scissor(const gpu::rect& s) {
	scissor = s;
}

void renderpass::render(const gpu::shader* s, const gpu::framebuffer* t) {
	if (t != nullptr) t->active();
	gpu::disable_depth_test();
	gpu::disable_stencil_test();
	gpu::disable_wireframe();
	gpu::set_viewport(viewport);
	if (scissor_test) {
		gpu::enable_scissor_test();
		gpu::set_scissor(scissor);
	} else {
		gpu::disable_scissor_test();
	}
	quad->attach(*s);
	quad->draw();
	gpu::framebuffer::default_active();
}

bool renderpass::scissor_test = false;
gpu::rect renderpass::scissor = {0, 0, 0, 0};
gpu::rect renderpass::viewport = {0, 0, 0, 0};
std::unique_ptr<gpu::vertexobject> renderpass::quad;
