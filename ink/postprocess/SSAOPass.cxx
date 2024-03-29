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

#include "SSAOPass.h"

#include "../core/Error.h"
#include "../shaders/ShaderLib.h"

namespace Ink {

SSAOPass::SSAOPass(int w, int h, float r, float m, float i) :
width(w), height(h), radius(r), max_radius(m), intensity(i) {}

void SSAOPass::init() {
	/* check the width and height */
	if (width == 0 || height == 0) {
		return Error::set("SSAOPass", "Width and height should be greater than 0");
	}
	
	/* prepare blur map 1 */
	blur_map_1 = std::make_unique<Gpu::Texture>();
	blur_map_1->init_2d(width / 2, height / 2, TEXTURE_R8_UNORM);
	blur_map_1->set_filters(TEXTURE_NEAREST, TEXTURE_NEAREST);
	blur_map_1->set_wrap_all(TEXTURE_CLAMP_TO_EDGE);
	
	/* prepare blur map 2 */
	blur_map_2 = std::make_unique<Gpu::Texture>();
	blur_map_2->init_2d(width / 2, height / 2, TEXTURE_R8_UNORM);
	blur_map_2->set_filters(TEXTURE_LINEAR, TEXTURE_LINEAR);
	blur_map_2->set_wrap_all(TEXTURE_CLAMP_TO_EDGE);
	
	/* prepare blur render target 1 */
	blur_target_1 = std::make_unique<Gpu::RenderTarget>();
	blur_target_1->set_texture(*blur_map_1, 0);
	
	/* prepare blur render target 2 */
	blur_target_2 = std::make_unique<Gpu::RenderTarget>();
	blur_target_2->set_texture(*blur_map_2, 0);
}

void SSAOPass::render() {
	/* fetch SSAO shader from shader lib */
	Defines ssao_defines;
	ssao_defines.set("SAMPLES", std::to_string(samples));
	auto* ssao_shader = ShaderLib::fetch("SSAO", ssao_defines);
	
	/* fetch blur shader from shader lib */
	Defines blur_defines;
	blur_defines.set("TYPE", "float");
	blur_defines.set("SWIZZLE", ".x");
	auto* blur_shader = ShaderLib::fetch("BilateralBlur", blur_defines);
	
	/* fetch blend shader from shader lib */
	Defines blend_defines;
	blend_defines.set("BLEND_OP(a, b)", "a * b");
	blend_defines.set("A_SWIZZLE", ".xyzw");
	blend_defines.set("B_SWIZZLE", ".xxxx");
	auto* blend_shader = ShaderLib::fetch("Blend", blend_defines);
	
	/* calculate camera & screen parameters */
	Mat4 inv_proj = inverse_4x4(camera->projection);
	Vec2 screen_size = Vec2(width / 2, height / 2);
	
	/* change the current viewport */
	Gpu::Rect viewport = RenderPass::get_viewport();
	RenderPass::set_viewport(Gpu::Rect(width / 2, height / 2));
	
	/* 1. render SSAO to texture (down-sampling) */
	ssao_shader->use_program();
	ssao_shader->set_uniform_f("intensity", intensity);
	ssao_shader->set_uniform_f("radius", radius);
	ssao_shader->set_uniform_f("max_radius", max_radius);
	ssao_shader->set_uniform_f("max_z", max_z);
	ssao_shader->set_uniform_f("near", camera->near);
	ssao_shader->set_uniform_f("far", camera->far);
	ssao_shader->set_uniform_m4("view", camera->viewing);
	ssao_shader->set_uniform_m4("proj", camera->projection);
	ssao_shader->set_uniform_m4("inv_proj", inv_proj);
	ssao_shader->set_uniform_i("g_normal", g_normal->activate(0));
	ssao_shader->set_uniform_i("z_map", z_map->activate(1));
	RenderPass::render_to(ssao_shader, blur_target_1.get());
	
	/* 2. blur texture for two times */
	for (int i = 0; i < 2; ++i) {
		
		/* blur texture horizontally */
		blur_shader->use_program();
		blur_shader->set_uniform_f("lod", 0);
		blur_shader->set_uniform_v2("direction", Vec2(1 / screen_size.x, 0));
		blur_shader->set_uniform_i("radius", 7);
		blur_shader->set_uniform_f("sigma_s", 2);
		blur_shader->set_uniform_f("sigma_r", 0.25);
		blur_shader->set_uniform_i("map", blur_map_1->activate(0));
		RenderPass::render_to(blur_shader, blur_target_2.get());
		
		/* blur texture vertically */
		blur_shader->use_program();
		blur_shader->set_uniform_f("lod", 0);
		blur_shader->set_uniform_v2("direction", Vec2(0, 1 / screen_size.y));
		blur_shader->set_uniform_i("radius", 7);
		blur_shader->set_uniform_f("sigma_s", 2);
		blur_shader->set_uniform_f("sigma_r", 0.25);
		blur_shader->set_uniform_i("map", blur_map_2->activate(0));
		RenderPass::render_to(blur_shader, blur_target_1.get());
	}
	
	/* set back to the initial viewport */
	RenderPass::set_viewport(viewport);
	
	/* 3. render results to render target (up-sampling) */
	blend_shader->use_program();
	blend_shader->set_uniform_i("map_a", map->activate(1));
	blend_shader->set_uniform_i("map_b", blur_map_1->activate(0));
	RenderPass::render_to(blend_shader, target);
}

const Camera* SSAOPass::get_camera() const {
	return camera;
}

void SSAOPass::set_camera(const Camera* c) {
	camera = c;
}

const Gpu::Texture* SSAOPass::get_texture() const {
	return map;
}

void SSAOPass::set_texture(const Gpu::Texture* t) {
	map = t;
}

const Gpu::Texture* SSAOPass::get_texture_normal() const {
	return g_normal;
}

void SSAOPass::set_texture_normal(const Gpu::Texture* t) {
	g_normal = t;
}

const Gpu::Texture* SSAOPass::get_texture_depth() const {
	return z_map;
}

void SSAOPass::set_texture_depth(const Gpu::Texture* t) {
	z_map = t;
}

}
