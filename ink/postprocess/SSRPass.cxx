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

#include "SSRPass.h"

#include "../shaders/ShaderLib.h"

namespace Ink {

SSRPass::SSRPass(int w, int h, float t, float i) :
width(w), height(h), thickness(t), intensity(i) {}

void SSRPass::init() {}

void SSRPass::render() {
	/* fetch SSR shader from shader lib */
	auto* ssr_shader = ShaderLib::fetch("SSR");
	
	/* calculate camera parameters */
	Gpu::Rect viewport = RenderPass::get_viewport();
	Vec2 screen_size = Vec2(viewport.width, viewport.height);
	Mat4 inv_proj = inverse_4x4(camera->projection);
	
	/* render SSR results to render target */
	ssr_shader->use_program();
	ssr_shader->set_uniform_i("max_steps", max_steps);
	ssr_shader->set_uniform_f("intensity", intensity);
	ssr_shader->set_uniform_f("thickness", thickness);
	ssr_shader->set_uniform_f("max_roughness", max_roughness);
	ssr_shader->set_uniform_f("near", camera->near);
	ssr_shader->set_uniform_f("far", camera->far);
	ssr_shader->set_uniform_v2("screen_size", screen_size);
	ssr_shader->set_uniform_m4("view", camera->viewing);
	ssr_shader->set_uniform_m4("proj", camera->projection);
	ssr_shader->set_uniform_m4("inv_proj", inv_proj);
	ssr_shader->set_uniform_i("map", map->activate(0));
	ssr_shader->set_uniform_i("g_normal", g_normal->activate(1));
	ssr_shader->set_uniform_i("g_material", g_material->activate(2));
	ssr_shader->set_uniform_i("z_map", z_map->activate(3));
	RenderPass::render_to(ssr_shader, target);
}

const Camera* SSRPass::get_camera() const {
	return camera;
}

void SSRPass::set_camera(const Camera* c) {
	camera = c;
}

const Gpu::Texture* SSRPass::get_texture() const {
	return map;
}

void SSRPass::set_texture(const Gpu::Texture* t) {
	map = t;
}

const Gpu::Texture* SSRPass::get_texture_normal() const {
	return g_normal;
}

void SSRPass::set_texture_normal(const Gpu::Texture* n) {
	g_normal = n;
}

const Gpu::Texture* SSRPass::get_texture_material() const {
	return g_material;
}

void SSRPass::set_texture_material(const Gpu::Texture* m) {
	g_material = m;
}

const Gpu::Texture* SSRPass::get_texture_depth() const {
	return z_map;
}

void SSRPass::set_texture_depth(const Gpu::Texture* t) {
	z_map = t;
}

}
