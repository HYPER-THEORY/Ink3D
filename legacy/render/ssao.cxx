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

#include "ssao.h"

void ssao::init() {
	ssao_shader = std::make_unique<gpu::shader>();
	ssao_shader->read("shaders/lib/ssao.vert.glsl",
					  "shaders/lib/ssao.frag.glsl");
}

ssao::ssao(float r, float m, float d, float b) :
	radius(r), max_radius(m), darkness(d), bias(b) {}

void ssao::preload() {
	gpu::viewport viewport = gpu::get_viewport();
	ssao_map = std::make_unique<gpu::texture>();
	ssao_map->preload_2d(viewport.width, viewport.height, IMAGE_R, IMAGE_16F, IMAGE_FLOAT);
	ssao_buffer = std::make_unique<gpu::framebuffer>();
	ssao_buffer->set_texture(*ssao_map, 0);
	ssao_buffer->set_draw({0});
}

void ssao::render(const camera& c, const gpu::texture& t,
				  const gpu::texture& n, const gpu::texture& d) {
	auto& square = geom::square();
	ssao_buffer->bind();
	gpu::clear_color({1});
	gpu::clear();
	square.attach(*ssao_shader);
	mat<4, 4> view_proj = c.projection * c.viewing;
	ssao_shader->use();
	ssao_shader->uniform_float("darkness", darkness);
	ssao_shader->uniform_float("radius", radius);
	ssao_shader->uniform_float("max_radius", max_radius);
	ssao_shader->uniform_float("bias", bias);
	ssao_shader->uniform_mat4("proj", c.projection);
	ssao_shader->uniform_mat4("view_proj", view_proj);
	ssao_shader->uniform_mat4("inv_view_proj", mat4_inverse(view_proj));
	ssao_shader->uniform_int("target_map", t.use(0));
	ssao_shader->uniform_int("normal_map", n.use(1));
	ssao_shader->uniform_int("depth_map", d.use(2));
	square.draw();
	ssao_buffer->unbind();
	
}

std::unique_ptr<gpu::shader> ssao::ssao_shader;
