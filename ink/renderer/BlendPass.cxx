/**
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

#include "BlendPass.h"

namespace Ink {

void BlendPass::init() {
	blend_shader = std::make_unique<Gpu::Shader>();
	blend_shader->load_vert_file("ink/shaders/lib/Blend.vert.glsl");
	blend_shader->load_frag_file("ink/shaders/lib/Blend.frag.glsl");
}

void BlendPass::compile() {
	Defines blend_defines;
	if (map_a != nullptr) {
		blend_defines.set("USE_A");
		blend_defines.set("A_SWIZZLE", swizzle_a);
	}
	if (map_b != nullptr) {
		blend_defines.set("USE_B");
		blend_defines.set("B_SWIZZLE", swizzle_b);
	}
	if (map_c != nullptr) {
		blend_defines.set("USE_C");
		blend_defines.set("C_SWIZZLE", swizzle_c);
	}
	if (map_d != nullptr) {
		blend_defines.set("USE_D");
		blend_defines.set("D_SWIZZLE", swizzle_d);
	}
	blend_defines.set("OP(a, b)", operation);
	blend_shader->set_defines(blend_defines);
	blend_shader->compile();
}

void BlendPass::render() const {
	blend_shader->use_program();
	if (map_a != nullptr) {
		blend_shader->set_uniform_i("map_a", map_a->activate(0));
	}
	if (map_b != nullptr) {
		blend_shader->set_uniform_i("map_b", map_b->activate(1));
	}
	if (map_c != nullptr) {
		blend_shader->set_uniform_i("map_c", map_c->activate(2));
	}
	if (map_d != nullptr) {
		blend_shader->set_uniform_i("map_d", map_d->activate(3));
	}
	blend_shader->set_uniform_v4("init_color", init_color);
	RenderPass::render_to(blend_shader.get(), target);
}

const Gpu::Texture* BlendPass::get_texture_a() const {
	return map_a;
}

void BlendPass::set_texture_a(const Gpu::Texture* t) {
	map_a = t;
}

void BlendPass::set_swizzle_a(const std::string& s) {
	swizzle_a = s;
}

const Gpu::Texture* BlendPass::get_texture_b() const {
	return map_b;
}

void BlendPass::set_texture_b(const Gpu::Texture* t) {
	map_b = t;
}

void BlendPass::set_swizzle_b(const std::string& s) {
	swizzle_b = s;
}

const Gpu::Texture* BlendPass::get_texture_c() const {
	return map_c;
}

void BlendPass::set_texture_c(const Gpu::Texture* t) {
	map_c = t;
}

void BlendPass::set_swizzle_c(const std::string& s) {
	swizzle_c = s;
}

const Gpu::Texture* BlendPass::get_texture_d() const {
	return map_d;
}

void BlendPass::set_texture_d(const Gpu::Texture* t) {
	map_d = t;
}

void BlendPass::set_swizzle_d(const std::string& s) {
	swizzle_d = s;
}

void BlendPass::set_init_color(const Vec4& c) {
	init_color = c;
}

void BlendPass::set_operation(const std::string& o) {
	operation = o;
}

}
