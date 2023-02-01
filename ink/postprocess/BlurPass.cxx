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

#include "BlurPass.h"

#include "../core/Error.h"
#include "../shaders/ShaderLib.h"

namespace Ink {

const std::string TYPES[] = {
	"float", "vec2", "vec3", "vec4"
};

const std::string SWIZZLES[] = {
	".x", ".xy", ".xyz", ".xyzw"
};

BlurPass::BlurPass(int w, int h) : width(w), height(h) {}

void BlurPass::init() {
	/* get default format with channel */
	int format = Gpu::Texture::default_format(channel, 1);
	
	/* prepare blur map 1 */
	blur_map_1 = std::make_unique<Gpu::Texture>();
	blur_map_1->init_2d(width / 2, height / 2, format);
	blur_map_1->set_filters(TEXTURE_NEAREST, TEXTURE_NEAREST);
	blur_map_1->set_wrap_all(TEXTURE_CLAMP_TO_EDGE);
	
	/* prepare blur map 2 */
	blur_map_2 = std::make_unique<Gpu::Texture>();
	blur_map_2->init_2d(width / 2, height / 2, format);
	blur_map_2->set_filters(TEXTURE_LINEAR, TEXTURE_LINEAR);
	blur_map_2->set_wrap_all(TEXTURE_CLAMP_TO_EDGE);
	
	/* prepare blur render target 1 */
	blur_target_1 = std::make_unique<Gpu::RenderTarget>();
	blur_target_1->set_texture(*blur_map_1, 0);
	
	/* prepare blur render target 2 */
	blur_target_2 = std::make_unique<Gpu::RenderTarget>();
	blur_target_2->set_texture(*blur_map_2, 0);
}

void BlurPass::render() const {
	/* fetch blur shader from shader lib */
	Defines blur_defines;
	blur_defines.set("TYPE", TYPES[channel - 1]);
	blur_defines.set("SWIZZLE", SWIZZLES[channel - 1]);
	const Gpu::Shader* blur_shader = nullptr;
	if (type == BLUR_BOX) {
		blur_shader = ShaderLib::fetch("BoxBlur", blur_defines);
	} else if (type == BLUR_GAUSSIAN) {
		blur_shader = ShaderLib::fetch("GaussianBlur", blur_defines);
	} else if (type == BLUR_BILATERAL) {
		blur_shader = ShaderLib::fetch("BilateralBlur", blur_defines);
	}
	
	/* fetch copy shader from shader lib */
	auto* copy_shader = ShaderLib::fetch("Copy");
	
	/* calculate screen parameter */
	Vec2 screen_size = Vec2(width / 2, height / 2);
	
	/* change the current viewport */
	Gpu::Rect viewport = RenderPass::get_viewport();
	RenderPass::set_viewport(Gpu::Rect(width / 2, height / 2));
	
	/* 1. blur horizontally (down-sampling) */
	blur_shader->use_program();
	blur_shader->set_uniform_f("lod", 0);
	blur_shader->set_uniform_v2("direction", Vec2(1 / screen_size.x, 0));
	blur_shader->set_uniform_i("radius", radius);
	blur_shader->set_uniform_f("sigma_s", sigma_s);
	blur_shader->set_uniform_f("sigma_r", sigma_r);
	blur_shader->set_uniform_i("map", map->activate(0));
	RenderPass::render_to(blur_shader, blur_target_1.get());
	
	/* 2. blur vertically */
	blur_shader->use_program();
	blur_shader->set_uniform_f("lod", 0);
	blur_shader->set_uniform_v2("direction", Vec2(0, 1 / screen_size.y));
	blur_shader->set_uniform_i("radius", radius);
	blur_shader->set_uniform_f("sigma_s", sigma_s);
	blur_shader->set_uniform_f("sigma_r", sigma_r);
	blur_shader->set_uniform_i("map", blur_map_1->activate(0));
	RenderPass::render_to(blur_shader, blur_target_2.get());
	
	/* set back to the initial viewport */
	RenderPass::set_viewport(viewport);
	
	/* 3. render results to render target (up-sampling) */
	copy_shader->use_program();
	copy_shader->set_uniform_i("map", blur_map_2->activate(0));
	RenderPass::render_to(copy_shader, target);
}

const Gpu::Texture* BlurPass::get_texture() const {
	return map;
}

void BlurPass::set_texture(const Gpu::Texture* t) {
	map = t;
}

}
