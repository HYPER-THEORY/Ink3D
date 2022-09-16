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

#include "ToneMappingPass.h"

namespace Ink {

void ToneMappingPass::init() {
	tone_map_shader = std::make_unique<Gpu::Shader>();
	tone_map_shader->load_vert_file("ink/shaders/lib/ToneMapping.vert.glsl");
	tone_map_shader->load_frag_file("ink/shaders/lib/ToneMapping.frag.glsl");
}

void ToneMappingPass::compile() {
	Defines defines;
	if (mode == TONE_MAPPING_LINEAR) {
		defines.set("TONE_MAPPING_LINEAR");
	}
	if (mode == TONE_MAPPING_REINHARD) {
		defines.set("TONE_MAPPING_REINHARD");
	}
	if (mode == TONE_MAPPING_OPTIMIZED) {
		defines.set("TONE_MAPPING_OPTIMIZED");
	}
	if (mode == TONE_MAPPING_ACES_FILMIC) {
		defines.set("TONE_MAPPING_ACES_FILMIC");
	}
	tone_map_shader->set_define(defines);
	tone_map_shader->compile();
}

void ToneMappingPass::render() const {
	tone_map_shader->use_program();
	tone_map_shader->set_uniform_f("exposure", exposure);
	tone_map_shader->set_uniform_i("map", map->activate(0));
	RenderPass::render_to(tone_map_shader.get(), target);
}

const Gpu::Texture* ToneMappingPass::get_texture() const {
	return map;
}

void ToneMappingPass::set_texture(const Gpu::Texture* t) {
	map = t;
}

}
