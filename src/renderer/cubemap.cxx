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

#include "cubemap.h"

void cubemap::init() {
	cube = std::make_unique<gpu::vertexobject>();
	cube->preload(boxmesh::create());
	default_shader = std::make_unique<gpu::shader>();
	default_shader->read("shaders/lib/cubemap.vert.glsl", "shaders/lib/cubemap.frag.glsl");
}

void cubemap::preload(const image* is) {
	cube_map = std::make_unique<gpu::texture>();
	cube_map->preload_cube(is);
	cube_map->set_wrap_all(TEXTURE_CLAMP_TO_EDGE);
	cube_map->set_filters(TEXTURE_LINEAR, TEXTURE_NEAREST);
}

const gpu::framebuffer* cubemap::get_target() const {
	return target;
}

void cubemap::set_target(const gpu::framebuffer* t) {
	target = t;
}

const gpu::shader* cubemap::get_shader() const {
	return custom_shader;
}

void cubemap::set_shader(const gpu::shader* s) {
	custom_shader = s;
}

void cubemap::render(const camera& c) const {
	if (target != nullptr) target->active();
	gpu::enable_depth_test();
	gpu::disable_stencil_test();
	gpu::disable_wireframe();
	auto* shader = custom_shader;
	if (shader == nullptr) shader = default_shader.get();
	mat4 viewing = c.viewing;
	for (int i = 0; i < 3; ++i) {
		viewing[i][3] = 0;
	}
	shader->use_program();
	cube->attach(*shader);
	shader->set_uniform_m4("view_proj", c.projection * viewing);
	shader->set_uniform_i("cube_map", cube_map->active(0));
	gpu::set_depth_func(FUNC_LEQUAL);
	cube->draw();
	gpu::set_depth_func(FUNC_LESS);
	gpu::framebuffer::default_active();
}

std::unique_ptr<gpu::vertexobject> cubemap::cube;
std::unique_ptr<gpu::shader> cubemap::default_shader;
