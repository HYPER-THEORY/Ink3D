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

#include "blendpass.h"

void blendpass::init() {
	blend_shader = std::make_unique<gpu::shader>();
	blend_shader->read("shaders/lib/blend.vert.glsl", "shaders/lib/blend.frag.glsl");
}

const gpu::framebuffer* blendpass::get_target() const {
	return target;
}

void blendpass::set_target(const gpu::framebuffer* t) {
	target = t;
}

const gpu::texture* blendpass::get_a() const {
	return a_map;
}

void blendpass::set_a(const gpu::texture* t, bool f) {
	a_map = t;
	a_is_factor = f;
}

const gpu::texture* blendpass::get_b() const {
	return b_map;
}

void blendpass::set_b(const gpu::texture* t, bool f) {
	b_map = t;
	b_is_factor = f;
}

const gpu::texture* blendpass::get_c() const {
	return c_map;
}

void blendpass::set_c(const gpu::texture* t, bool f) {
	c_map = t;
	c_is_factor = f;
}

const gpu::texture* blendpass::get_d() const {
	return d_map;
}

void blendpass::set_d(const gpu::texture* t, bool f) {
	d_map = t;
	d_is_factor = f;
}

void blendpass::render() const {
	blend_shader->use_program();
	blend_shader->uniform_int("use_a_map", a_map != nullptr);
	blend_shader->uniform_int("a_map", a_map != nullptr ? a_map->active(0) : 0);
	blend_shader->uniform_int("a_is_factor", a_is_factor);
	blend_shader->uniform_int("use_b_map", b_map != nullptr);
	blend_shader->uniform_int("b_map", b_map != nullptr ? b_map->active(1) : 1);
	blend_shader->uniform_int("b_is_factor", b_is_factor);
	blend_shader->uniform_int("use_c_map", c_map != nullptr);
	blend_shader->uniform_int("c_map", c_map != nullptr ? c_map->active(2) : 2);
	blend_shader->uniform_int("c_is_factor", c_is_factor);
	blend_shader->uniform_int("use_d_map", d_map != nullptr);
	blend_shader->uniform_int("d_map", d_map != nullptr ? d_map->active(3) : 3);
	blend_shader->uniform_int("d_is_factor", d_is_factor);
	renderpass::render(blend_shader.get(), target);
}

std::unique_ptr<gpu::shader> blendpass::blend_shader;
