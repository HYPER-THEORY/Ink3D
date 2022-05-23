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

#include "fxaapass.h"

void fxaapass::init() {
	fxaa_shader = std::make_unique<gpu::shader>();
	fxaa_shader->read("shaders/lib/fxaa.vert.glsl", "shaders/lib/fxaa.frag.glsl");
}

const gpu::framebuffer* fxaapass::get_target() const {
	return target;
}

void fxaapass::set_target(const gpu::framebuffer* t) {
	target = t;
}

const gpu::texture* fxaapass::get_source() const {
	return source_map;
}

void fxaapass::set_source(const gpu::texture* s) {
	source_map = s;
}

void fxaapass::render() const {
	gpu::rect view = gpu::get_viewport();
	float view_width = view.width;
	float view_height = view.height;
	vec2 screen_size = {view_width, view_height};
	fxaa_shader->use_program();
	fxaa_shader->set_uniform_v2("screen_size", screen_size);
	fxaa_shader->set_uniform_i("source_map", source_map->active(0));
	renderpass::render(fxaa_shader.get(), target);
}

std::unique_ptr<gpu::shader> fxaapass::fxaa_shader;
