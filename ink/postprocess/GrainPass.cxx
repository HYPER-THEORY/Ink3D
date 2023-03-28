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

#include "GrainPass.h"

#include "../math/Random.h"
#include "../shaders/ShaderLib.h"

namespace Ink {

void GrainPass::init() {}

void GrainPass::render() {
	auto* grain_shader = ShaderLib::fetch("Grain");
	Gpu::Rect viewport = RenderPass::get_viewport();
	Vec2 screen_size = Vec2(viewport.width, viewport.height);
	grain_shader->use_program();
	grain_shader->set_uniform_f("intensity", intensity);
	grain_shader->set_uniform_f("seed", Random::random_f() + 1);
	grain_shader->set_uniform_i("map", map->activate(0));
	RenderPass::render_to(grain_shader, target);
}

const Gpu::Texture* GrainPass::get_texture() const {
	return map;
}

void GrainPass::set_texture(const Gpu::Texture* t) {
	map = t;
}

}
