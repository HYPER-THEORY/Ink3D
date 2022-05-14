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

#include "shadow.h"

shadow::shadow(int r, float d, float b, float nb) :
	resolution(r), darkness(d), bias(b), normal_bias(nb) {}

void shadow::init() {
	shadow_shader = std::make_unique<gpu::shader>();
	shadow_shader->read("shaders/lib/shadow.vert.glsl",
						"shaders/lib/shadow.frag.glsl");
}

void shadow::set_light_view(const camera& v) {
	light_view = v;
}

void shadow::preload() {
	shadow_buffer = std::make_unique<gpu::framebuffer>();
	shadow_map = std::make_unique<gpu::texture>();
	shadow_map->preload_2d(resolution, resolution, IMAGE_D);
	discard_map = std::make_unique<gpu::texture>();
	discard_map->preload_2d(resolution, resolution, IMAGE_R);
	shadow_map->set_parameters(TEXTURE_CLAMP_TO_BORDER, TEXTURE_NEAREST, TEXTURE_NEAREST);
	shadow_map->set_border_color({1, 1, 1, 1});
	shadow_buffer->set_texture(*shadow_map, 0, true);
	shadow_buffer->set_texture(*discard_map, 0);
	shadow_buffer->disable_draw();
}

void shadow::create(const renderer& r, const scene& s) const {
	gpu::viewport viewport = gpu::get_viewport();
	shadow_buffer->bind();
	gpu::enable_depth_test();
	gpu::set_viewport({0, 0, resolution, resolution});
	gpu::clear(false, true, false);
	r.render(s, light_view);
	shadow_buffer->unbind();
	gpu::set_viewport(viewport);
}

void shadow::render(const camera& c, const gpu::texture& t,
					const gpu::texture& n, const gpu::texture& d) {
	auto& square = geom::square();
	square.attach(*shadow_shader);
	shadow_shader->use();
	shadow_shader->uniform_int("shadow_type", type);
	shadow_shader->uniform_float("darkness", darkness);
	shadow_shader->uniform_float("bias", bias);
	shadow_shader->uniform_float("normal_bias", normal_bias);
	shadow_shader->uniform_float("texel_size", 1.f / resolution);
	shadow_shader->uniform_int("filter_radius", radius);
	shadow_shader->uniform_int("light_weight", weight);
	shadow_shader->uniform_vec3("light_dir", light_view.direction);
	shadow_shader->uniform_mat4("proj", light_view.projection);
	shadow_shader->uniform_mat4("inv_view_proj", mat4_inverse(c.projection * c.viewing));
	shadow_shader->uniform_mat4("light_view_proj", light_view.projection * light_view.viewing);
	shadow_shader->uniform_int("target_map", t.use(0));
	shadow_shader->uniform_int("normal_map", n.use(1));
	shadow_shader->uniform_int("depth_map", d.use(2));
	shadow_shader->uniform_int("shadow_map", shadow_map->use(3));
	square.draw();
}

std::unique_ptr<gpu::shader> shadow::shadow_shader;
