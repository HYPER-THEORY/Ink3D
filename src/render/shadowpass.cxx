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

#include "shadowpass.h"

shadowpass::shadowpass(int r, float d, float b, float nb) :
	resolution(r), darkness(d), bias(b), normal_bias(nb) {}

void shadowpass::init() {
	shadow_shader = std::make_unique<gpu::shader>();
	shadow_shader->read("shaders/lib/shadow.vert.glsl", "shaders/lib/shadow.frag.glsl");
}

camera shadowpass::get_light_view() const {
	return light_view;
}

void shadowpass::set_light_view(const camera& v) {
	light_view = v;
}

void shadowpass::preload() {
	shadow_map = std::make_unique<gpu::texture>();
	shadow_map->preload_2d(resolution, resolution, IMAGE_D);
	shadow_map->set_wrap_all(TEXTURE_CLAMP_TO_BORDER);
	shadow_map->set_filters(TEXTURE_LINEAR, TEXTURE_NEAREST);
	shadow_map->set_border_color({1, 1, 1, 1});
	shadow_buffer = std::make_unique<gpu::framebuffer>();
	shadow_buffer->set_depth_attachment(*shadow_map, false);
	shadow_buffer->disable_draw();
}

void shadowpass::create(renderer& r, const scene& s) const {
	gpu::viewport view = gpu::get_viewport();
	gpu::set_viewport({0, 0, resolution, resolution});
	auto* renderer_target = r.get_target();
	r.set_target(shadow_buffer.get());
	r.clear(false, true, false);
	r.render(s, light_view);
	gpu::set_viewport(view);
	r.set_target(renderer_target);
}

const gpu::framebuffer* shadowpass::get_target() const {
	return target;
}

void shadowpass::set_target(const gpu::framebuffer* t) {
	target = t;
}

const gpu::texture* shadowpass::get_normal() const {
	return normal_map;
}

void shadowpass::set_normal(const gpu::texture* n) {
	normal_map = n;
}

const gpu::texture* shadowpass::get_depth() const {
	return depth_map;
}

void shadowpass::set_depth(const gpu::texture* d) {
	depth_map = d;
}

void shadowpass::render(const camera& c) const {
	vec2 screen_size = {1.f * resolution, 1.f * resolution};
	mat4 inv_view_proj = mat4_inverse(c.projection * c.viewing);
	mat4 light_view_proj = light_view.projection * light_view.viewing;
	shadow_shader->use_program();
	shadow_shader->uniform_int("type", type);
	shadow_shader->uniform_float("darkness", darkness);
	shadow_shader->uniform_float("bias", bias);
	shadow_shader->uniform_float("normal_bias", normal_bias);
	shadow_shader->uniform_vec2("screen_size", screen_size);
	shadow_shader->uniform_int("filter_radius", radius);
	shadow_shader->uniform_int("light_weight", light_weight);
	shadow_shader->uniform_vec3("light_dir", light_view.direction);
	shadow_shader->uniform_mat4("proj", light_view.projection);
	shadow_shader->uniform_mat4("inv_view_proj", inv_view_proj);
	shadow_shader->uniform_mat4("light_view_proj", light_view_proj);
	shadow_shader->uniform_int("normal_map", normal_map->active(0));
	shadow_shader->uniform_int("depth_map", depth_map->active(1));
	shadow_shader->uniform_int("shadow_map", shadow_map->active(2));
	renderpass::render(shadow_shader.get(), target);
}

std::unique_ptr<gpu::shader> shadowpass::shadow_shader;
