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

#include "LightPass.h"

#include "../core/Format.h"

namespace Ink {

void LightPass::init() {}

void LightPass::render() const {
	/* fetch light shader from shader lib */
	Defines light_defines = Renderer::define_scene(*scene);
	light_defines.set(Renderer::define_tone_map(tone_map_mode));
	auto* light_shader = ShaderLib::fetch("Lighting", light_defines);
	
	/* pass parameters and G-Buffers to shader */
	light_shader->use_program();
	light_shader->set_uniform_v3("camera_pos", camera->position);
	light_shader->set_uniform_f("exposure", tone_map_exposure);
	light_shader->set_uniform_i("buffer_c", buffer_c->activate(0));
	light_shader->set_uniform_i("buffer_n", buffer_n->activate(1));
	light_shader->set_uniform_i("buffer_m", buffer_m->activate(2));
	light_shader->set_uniform_i("buffer_a", buffer_a->activate(3));
	light_shader->set_uniform_i("buffer_d", buffer_d->activate(4));
	
	/* pass camera parameter to shader */
	Mat4 inv_view_proj = inverse_4x4(camera->projection * camera->viewing);
	light_shader->set_uniform_m4("inv_view_proj", inv_view_proj);
	
	/* pass the lights & fogs parameters to shader */
	Renderer::set_light_uniforms(*scene, *light_shader);
	
	/* render results to render target */
	RenderPass::render_to(light_shader, target);
}

void LightPass::set(const Scene* s, const Camera* c) {
	scene = s;
	camera = c;
}

const Gpu::Texture* LightPass::get_buffer_c() const {
	return buffer_c;
}

void LightPass::set_buffer_c(const Gpu::Texture* t) {
	buffer_c = t;
}

const Gpu::Texture* LightPass::get_buffer_n() const {
	return buffer_n;
}

void LightPass::set_buffer_n(const Gpu::Texture* t) {
	buffer_n = t;
}

const Gpu::Texture* LightPass::get_buffer_m() const {
	return buffer_m;
}

void LightPass::set_buffer_m(const Gpu::Texture* t) {
	buffer_m = t;
}

const Gpu::Texture* LightPass::get_buffer_a() const {
	return buffer_a;
}

void LightPass::set_buffer_a(const Gpu::Texture* t) {
	buffer_a = t;
}

const Gpu::Texture* LightPass::get_buffer_d() const {
	return buffer_d;
}

void LightPass::set_buffer_d(const Gpu::Texture* t) {
	buffer_d = t;
}

int LightPass::get_tone_mapping_mode() const {
	return tone_map_mode;
}

float LightPass::get_tone_mapping_exposure() const {
	return tone_map_exposure;
}

void LightPass::set_tone_mapping(int m, float e) {
	tone_map_mode = m;
	tone_map_exposure = e;
}

}
