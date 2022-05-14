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

#include "copypass.h"

void copypass::init() {
	copy_shader = std::make_unique<gpu::shader>();
	copy_shader->read("shaders/lib/copy.vert.glsl", "shaders/lib/copy.frag.glsl");
}

const gpu::framebuffer* copypass::get_target() const {
	return target;
}

void copypass::set_target(const gpu::framebuffer* t) {
	target = t;
}

const gpu::texture* copypass::get_source() const {
	return source_map;
}

void copypass::set_source(const gpu::texture* s) {
	source_map = s;
}

void copypass::render() const {
	copy_shader->use_program();
	copy_shader->uniform_int("source_map", source_map->active(0));
	renderpass::render(copy_shader.get(), target);
}

std::unique_ptr<gpu::shader> copypass::copy_shader;
