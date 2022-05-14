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

#include "ssaopass.h"

void ssaopass::init() {
	ssao_shader = std::make_unique<gpu::shader>();
	ssao_shader->read("shaders/lib/ssao.vert.glsl", "shaders/lib/ssao.frag.glsl");
	blur_shader = std::make_unique<gpu::shader>();
	blur_shader->read("shaders/lib/ssaoblur.vert.glsl", "shaders/lib/ssaoblur.frag.glsl");
}

ssaopass::ssaopass(float r, float m, float d, float b) :
	radius(r), max_radius(m), darkness(d), bias(b) {}

void ssaopass::preload() {
	gpu::viewport viewport = gpu::get_viewport();
	ssao_map = std::make_unique<gpu::texture>();
	ssao_map->preload_2d(viewport.width, viewport.height, IMAGE_R);
	ssao_map->set_wrap_all(TEXTURE_CLAMP_TO_EDGE);
	ssao_map->set_filters(TEXTURE_NEAREST, TEXTURE_NEAREST);
	ssao_buffer = std::make_unique<gpu::framebuffer>();
	ssao_buffer->set_attachment(*ssao_map, 0);
	ssao_buffer->draw_attachments({0});
}

const gpu::framebuffer* ssaopass::get_target() const {
	return target;
}

void ssaopass::set_target(const gpu::framebuffer* t) {
	target = t;
}

const gpu::texture* ssaopass::get_normal() const {
	return normal_map;
}

void ssaopass::set_normal(const gpu::texture* n) {
	normal_map = n;
}

const gpu::texture* ssaopass::get_depth() const {
	return depth_map;
}

void ssaopass::set_depth(const gpu::texture* d) {
	depth_map = d;
}

void ssaopass::render(const camera& c) const {
	gpu::viewport view = gpu::get_viewport();
	mat4 view_proj = c.projection * c.viewing;
	mat4 inv_view_proj = mat4_inverse(view_proj);
	vec2 screen_size = {1.f * view.width, 1.f * view.height};
	/* 1. render ssao to map */
	ssao_shader->use_program();
	ssao_shader->uniform_float("radius", radius);
	ssao_shader->uniform_float("max_radius", max_radius);
	ssao_shader->uniform_float("bias", bias);
	ssao_shader->uniform_mat4("proj", c.projection);
	ssao_shader->uniform_mat4("view_proj", view_proj);
	ssao_shader->uniform_mat4("inv_view_proj", inv_view_proj);
	ssao_shader->uniform_int("normal_map", normal_map->active(0));
	ssao_shader->uniform_int("depth_map", depth_map->active(1));
	renderpass::render(ssao_shader.get(), ssao_buffer.get());
	/* 2. render blurred ssao to screen */
	blur_shader->use_program();
	blur_shader->uniform_int("quality", quality);
	blur_shader->uniform_float("darkness", darkness);
	blur_shader->uniform_vec2("screen_size", screen_size);
	blur_shader->uniform_int("ssao_map", ssao_map->active(0));
	renderpass::render(blur_shader.get(), target);
}

std::unique_ptr<gpu::shader> ssaopass::ssao_shader;
std::unique_ptr<gpu::shader> ssaopass::blur_shader;
