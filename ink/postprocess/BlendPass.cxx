/**
 * Copyright (C) 2021-2023 Hypertheory
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

#include "../shaders/ShaderLib.h"

namespace Ink {

void BlendPass::init() {}

void BlendPass::render() const {
	Defines blend_defines;
	blend_defines.set("BLEND_OP(a, b)", operation);
	blend_defines.set("A_SWIZZLE", swizzle_a);
	blend_defines.set("B_SWIZZLE", swizzle_b);
	auto* blend_shader = ShaderLib::fetch("Blend", blend_defines);
	blend_shader->use_program();
	blend_shader->set_uniform_i("map_a", map_a->activate(0));
	blend_shader->set_uniform_i("map_b", map_b->activate(1));
	RenderPass::render_to(blend_shader, target);
}

const Gpu::Texture* BlendPass::get_texture_a() const {
	return map_a;
}

void BlendPass::set_texture_a(const Gpu::Texture* t) {
	map_a = t;
}

const Gpu::Texture* BlendPass::get_texture_b() const {
	return map_b;
}

void BlendPass::set_texture_b(const Gpu::Texture* t) {
	map_b = t;
}

std::string BlendPass::get_operation() const {
	return operation;
}

void BlendPass::set_operation(const std::string& o) {
	operation = o;
}

std::string BlendPass::get_swizzle_a() const {
	return swizzle_a;
}

void BlendPass::set_swizzle_a(const std::string& s) {
	swizzle_a = s;
}

std::string BlendPass::get_swizzle_b() const {
	return swizzle_b;
}

void BlendPass::set_swizzle_b(const std::string& s) {
	swizzle_b = s;
}

}
