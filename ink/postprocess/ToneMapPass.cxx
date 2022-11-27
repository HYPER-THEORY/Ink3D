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

#include "ToneMapPass.h"

#include "../renderer/Renderer.h"

namespace Ink {

void ToneMapPass::init() {}

void ToneMapPass::render() const {
	Defines tone_map_defines = Renderer::define_tone_map(mode);
	auto* tone_map_shader = ShaderLib::fetch("ToneMapping", tone_map_defines);
	tone_map_shader->use_program();
	tone_map_shader->set_uniform_f("exposure", exposure);
	tone_map_shader->set_uniform_i("map", map->activate(0));
	RenderPass::render_to(tone_map_shader, target);
}

const Gpu::Texture* ToneMapPass::get_texture() const {
	return map;
}

void ToneMapPass::set_texture(const Gpu::Texture* t) {
	map = t;
}

}
