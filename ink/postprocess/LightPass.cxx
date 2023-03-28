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

#include "LightPass.h"

#include "../core/Format.h"
#include "../shaders/ShaderLib.h"

namespace Ink {

void LightPass::init() {}

void LightPass::render() {
	/* fetch light shader from shader lib */
	Defines light_defines;
	Renderer::set_tone_map_defines(tone_map_mode, light_defines);
	Renderer::set_scene_defines(*scene, light_defines);
	auto* light_shader = ShaderLib::fetch("Lighting", light_defines);
	
	/* pass parameters and G-Buffers to shader */
	light_shader->use_program();
	light_shader->set_uniform_v3("camera_pos", camera->position);
	light_shader->set_uniform_f("exposure", tone_map_exposure);
	light_shader->set_uniform_i("g_color", g_color->activate(0));
	light_shader->set_uniform_i("g_normal", g_normal->activate(1));
	light_shader->set_uniform_i("g_material", g_material->activate(2));
	light_shader->set_uniform_i("g_light", g_light->activate(3));
	light_shader->set_uniform_i("z_map", z_map->activate(4));
	
	/* pass camera parameter to shader */
	Mat4 inv_view_proj = inverse_4x4(camera->projection * camera->viewing);
	light_shader->set_uniform_m4("inv_view_proj", inv_view_proj);
	
	/* pass the lights & fogs parameters to shader */
	Renderer::set_light_uniforms(*scene, *light_shader);
	
	/* render results to render target */
	RenderPass::render_to(light_shader, target);
}

const Scene* LightPass::get_scene() const {
	return scene;
}

void LightPass::set_scene(const Scene* s) {
	scene = s;
}

const Camera* LightPass::get_camera() const {
	return camera;
}

void LightPass::set_camera(const Camera* c) {
	camera = c;
}

int LightPass::get_tone_map_mode() const {
	return tone_map_mode;
}

float LightPass::get_tone_map_exposure() const {
	return tone_map_exposure;
}

void LightPass::set_tone_map(int m, float e) {
	tone_map_mode = m;
	tone_map_exposure = e;
}

const Gpu::Texture* LightPass::get_texture_color() const {
	return g_color;
}

void LightPass::set_texture_color(const Gpu::Texture* t) {
	g_color = t;
}

const Gpu::Texture* LightPass::get_texture_normal() const {
	return g_normal;
}

void LightPass::set_texture_normal(const Gpu::Texture* t) {
	g_normal = t;
}

const Gpu::Texture* LightPass::get_texture_material() const {
	return g_material;
}

void LightPass::set_texture_material(const Gpu::Texture* t) {
	g_material = t;
}

const Gpu::Texture* LightPass::get_texture_light() const {
	return g_light;
}

void LightPass::set_texture_light(const Gpu::Texture* t) {
	g_light = t;
}

const Gpu::Texture* LightPass::get_texture_depth() const {
	return z_map;
}

void LightPass::set_texture_depth(const Gpu::Texture* t) {
	z_map = t;
}

}
