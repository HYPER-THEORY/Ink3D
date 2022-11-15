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

#include "FXAAPass.h"

namespace Ink {

void FXAAPass::init() {
	fxaa_shader = std::make_unique<Gpu::Shader>();
	fxaa_shader->load_vert_file("ink/shaders/lib/FXAA.vert.glsl");
	fxaa_shader->load_frag_file("ink/shaders/lib/FXAA.frag.glsl");
}

void FXAAPass::compile() {
	fxaa_shader->compile();
}

void FXAAPass::render() const {
	Gpu::Rect viewport = RenderPass::get_viewport();
	Vec2 screen_size = Vec2(viewport.width, viewport.height);
	fxaa_shader->use_program();
	fxaa_shader->set_uniform_v2("screen_size", screen_size);
	fxaa_shader->set_uniform_i("map", map->activate(0));
	RenderPass::render_to(fxaa_shader.get(), target);
}

const Gpu::Texture* FXAAPass::get_texture() const {
	return map;
}

void FXAAPass::set_texture(const Gpu::Texture* t) {
	map = t;
}

}
