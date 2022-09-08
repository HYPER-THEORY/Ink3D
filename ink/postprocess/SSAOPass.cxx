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

#include "SSAOPass.h"

namespace Ink {

SSAOPass::SSAOPass(int w, int h, float r, float m, float d) :
width(w), height(h), radius(r), max_radius(m), darkness(d) {}

void SSAOPass::init() {
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
	
	/* prepare blur frame buffer 1 */
	blur_buffer_1 = std::make_unique<Gpu::FrameBuffer>();
	blur_buffer_1->set_attachment(*blur_map_1, 0);
	blur_buffer_1->draw_attachments({0});
	
	/* prepare blur frame buffer 2 */
	blur_buffer_2 = std::make_unique<Gpu::FrameBuffer>();
	blur_buffer_2->set_attachment(*blur_map_2, 0);
	blur_buffer_2->draw_attachments({0});
	
	/* prepare SSAO shader */
	ssao_shader = std::make_unique<Gpu::Shader>();
	ssao_shader->load_vert_file("shaders/lib/SSAO.vert.glsl");
	ssao_shader->load_frag_file("shaders/lib/SSAO.frag.glsl");
	
	/* prepare blur shader */
	blur_shader = std::make_unique<Gpu::Shader>();
	blur_shader->load_vert_file("shaders/lib/Blur.vert.glsl");
	blur_shader->load_frag_file("shaders/lib/Blur.frag.glsl");
	
	/* prepare copy shader */
	copy_shader = std::make_unique<Gpu::Shader>();
	copy_shader->load_vert_file("shaders/lib/Copy.vert.glsl");
	copy_shader->load_frag_file("shaders/lib/Copy.frag.glsl");
}

void SSAOPass::compile() {
	/* get the parameters of SSAO */
	std::string samples_str = std::to_string(samples);
	std::string camera_type = "ORTHO_CAMERA";
	if (camera->is_perspective()) camera_type = "PERSP_CAMERA";
	
	/* compile SSAO shader */
	ssao_shader->set_define({
		{camera_type, ""         },
		{"SAMPLES"  , samples_str},
	});
	ssao_shader->compile();
	
	/* compile blur shader */
	blur_shader->set_define({
		{"BLUR_BILATERAL", ""     },
		{"TYPE"          , "float"},
		{"SWIZZLE"       , ".x"   },
		{"SIGMA_S"       , "2"    },
		{"SIGMA_R"       , "0.25" },
	});
	blur_shader->compile();
	
	/* compile copy shader */
	copy_shader->compile();
}

void SSAOPass::render() const {
	/* calculate camera & screen parameters */
	Mat4 view_proj = camera->projection * camera->viewing;
	Mat4 inv_view_proj = inverse_4x4(view_proj);
	Vec2 screen_size = Vec2(width / 2, height / 2);
	
	/* change the current viewport */
	Gpu::Rect viewport = RenderPass::get_viewport();
	RenderPass::set_viewport({width / 2, height / 2});
	
	/* 1. render SSAO to texture (downsampling) */
	ssao_shader->use_program();
	ssao_shader->set_uniform_f("radius", radius);
	ssao_shader->set_uniform_f("max_radius", max_radius);
	ssao_shader->set_uniform_f("bias", bias);
	ssao_shader->set_uniform_f("darkness", darkness);
	ssao_shader->set_uniform_f("camera_near", camera->near);
	ssao_shader->set_uniform_f("camera_far", camera->far);
	ssao_shader->set_uniform_m4("view_proj", view_proj);
	ssao_shader->set_uniform_m4("inv_view_proj", inv_view_proj);
	ssao_shader->set_uniform_i("gbuffer_n", gbuffer_n->activate(0));
	ssao_shader->set_uniform_i("gbuffer_d", gbuffer_d->activate(1));
	RenderPass::render_to(ssao_shader.get(), blur_buffer_1.get());
	
	/* blur texture for two times */
	for (int i = 0; i < 2; ++i) {
		
		/* 2. blur texture horizontally */
		blur_shader->use_program();
		blur_shader->set_uniform_v2("direction", Vec2(1 / screen_size.x, 0));
		blur_shader->set_uniform_i("radius", 7);
		blur_shader->set_uniform_i("map", blur_map_1->activate(0));
		RenderPass::render_to(blur_shader.get(), blur_buffer_2.get());
		
		/* 3. blur texture vertically */
		blur_shader->use_program();
		blur_shader->set_uniform_v2("direction", Vec2(0, 1 / screen_size.y));
		blur_shader->set_uniform_i("radius", 7);
		blur_shader->set_uniform_i("map", blur_map_2->activate(0));
		RenderPass::render_to(blur_shader.get(), blur_buffer_1.get());
	}
	
	/* set back to the original viewport */
	RenderPass::set_viewport(viewport);
	
	/* 4. render results to target (upsampling) */
	copy_shader->use_program();
	copy_shader->set_uniform_i("map", blur_map_1->activate(0));
	RenderPass::render_to(copy_shader.get(), target);
}

void SSAOPass::process(const Camera& c) {
	camera = &c;
	process();
}

const Gpu::Texture* SSAOPass::get_gbuffer_n() const {
	return gbuffer_n;
}

void SSAOPass::set_gbuffer_n(const Gpu::Texture* n) {
	gbuffer_n = n;
}

const Gpu::Texture* SSAOPass::get_gbuffer_d() const {
	return gbuffer_d;
}

void SSAOPass::set_gbuffer_d(const Gpu::Texture* d) {
	gbuffer_d = d;
}

}
