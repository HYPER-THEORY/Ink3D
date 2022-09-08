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

#include "BlurPass.h"

#include "../core/Error.h"

namespace Ink {

BlurPass::BlurPass(int w, int h) : width(w), height(h) {}

void BlurPass::init() {
	/* prepare blur map 1 */
	blur_map_1 = std::make_unique<Gpu::Texture>();
	blur_map_1->init_2d(width / 2, height / 2, Gpu::Texture::default_format(channel, 1));
	blur_map_1->set_filters(TEXTURE_NEAREST, TEXTURE_NEAREST);
	blur_map_1->set_wrap_all(TEXTURE_CLAMP_TO_EDGE);
	
	/* prepare blur map 2 */
	blur_map_2 = std::make_unique<Gpu::Texture>();
	blur_map_2->init_2d(width / 2, height / 2, Gpu::Texture::default_format(channel, 1));
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
	
	/* prepare blur shader */
	blur_shader = std::make_unique<Gpu::Shader>();
	blur_shader->load_vert_file("shaders/lib/Blur.vert.glsl");
	blur_shader->load_frag_file("shaders/lib/Blur.frag.glsl");
	
	/* prepare copy shader */
	copy_shader = std::make_unique<Gpu::Shader>();
	copy_shader->load_vert_file("shaders/lib/Copy.vert.glsl");
	copy_shader->load_frag_file("shaders/lib/Copy.frag.glsl");
}

void BlurPass::compile() {
	Defines defines;
	
	/* set the sigma of space */
	defines.set("SIGMA_S", std::to_string(sigma_s));
	
	/* set the sigma of range */
	defines.set("SIGMA_R", std::to_string(sigma_r));
	
	/* set the type of blur */
	if (type == BLUR_SIMPLE) {
		defines.set("BLUR_SIMPLE");
	}
	if (type == BLUR_GAUSSIAN) {
		defines.set("BLUR_GAUSSIAN");
	}
	if (type == BLUR_BILATERAL) {
		defines.set("BLUR_BILATERAL");
	}
	
	/* set the channels of texture */
	if (channel == 1) {
		defines.set("TYPE", "float");
		defines.set("SWIZZLE", ".x");
	} else if (channel == 2) {
		defines.set("TYPE", "vec2");
		defines.set("SWIZZLE", ".xy");
	} else if (channel == 3) {
		defines.set("TYPE", "vec3");
		defines.set("SWIZZLE", ".xyz");
	} else if (channel == 4) {
		defines.set("TYPE", "vec4");
		defines.set("SWIZZLE", ".xyzw");
	}
	
	/* compile blur shader */
	blur_shader->set_define(defines);
	blur_shader->compile();
	
	/* compile copy shader */
	copy_shader->compile();
}

void BlurPass::render() const {
	/* calculate screen parameter */
	Vec2 screen_size = Vec2(width / 2, height / 2);
	
	/* change the current viewport */
	Gpu::Rect viewport = RenderPass::get_viewport();
	RenderPass::set_viewport({width / 2, height / 2});
	
	/* 1. blur horizontally (downsampling) */
	blur_shader->use_program();
	blur_shader->set_uniform_v2("direction", Vec2(1 / screen_size.x, 0));
	blur_shader->set_uniform_i("radius", radius);
	blur_shader->set_uniform_i("map", map->activate(0));
	RenderPass::render_to(blur_shader.get(), blur_buffer_1.get());
	
	/* 2. blur vertically */
	blur_shader->use_program();
	blur_shader->set_uniform_v2("direction", Vec2(0, 1 / screen_size.y));
	blur_shader->set_uniform_i("radius", radius);
	blur_shader->set_uniform_i("map", blur_map_1->activate(0));
	RenderPass::render_to(blur_shader.get(), blur_buffer_2.get());
	
	/* set back to the original viewport */
	RenderPass::set_viewport(viewport);
	
	/* 3. render results to target (upsampling) */
	copy_shader->use_program();
	copy_shader->set_uniform_i("map", blur_map_2->activate(0));
	RenderPass::render_to(copy_shader.get(), target);
}

const Gpu::Texture* BlurPass::get_texture() const {
	return map;
}

void BlurPass::set_texture(const Gpu::Texture* t) {
	map = t;
}

}
