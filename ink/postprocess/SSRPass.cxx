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

#include "SSRPass.h"

#include "../../libs/glad/glad.h"

namespace Ink {

SSRPass::SSRPass(int w, int h, float t, float i) :
width(w), height(h), thickness(t), intensity(i) {}

void SSRPass::init() {
	/* calculate max lod and Hierarchical-Z map size */
	Vec2 max_lod = {ceilf(log2f(width)) - 1, ceilf(log2f(height)) - 1};
	Vec2 z_map_size = {powf(2, max_lod.x), powf(2, max_lod.y)};
	
	/* prepare Hierarchical-Z map */
	z_map = std::make_unique<Gpu::Texture>();
	z_map->init_2d(z_map_size.x, z_map_size.y, TEXTURE_R32_SFLOAT);
	z_map->set_filters(TEXTURE_NEAREST, TEXTURE_NEAREST_MIPMAP_NEAREST);
	z_map->generate_mipmap();
	
	/* prepare Hierarchical-Z frame buffer */
	hi_z_target = std::make_unique<Gpu::FrameBuffer>();
	hi_z_target->draw_attachments({0});
	
	/* prepare copy shader */
	copy_shader = std::make_unique<Gpu::Shader>();
	copy_shader->load_vert_file("ink/shaders/lib/Copy.vert.glsl");
	copy_shader->load_frag_file("ink/shaders/lib/Copy.frag.glsl");
	
	/* prepare Hi-Z shader */
	hi_z_shader = std::make_unique<Gpu::Shader>();
	hi_z_shader->load_vert_file("ink/shaders/lib/MinHiZ.vert.glsl");
	hi_z_shader->load_frag_file("ink/shaders/lib/MinHiZ.frag.glsl");
	
	/* compile SSR shader */
	ssr_shader = std::make_unique<Gpu::Shader>();
	ssr_shader->load_vert_file("ink/shaders/lib/SSR.vert.glsl");
	ssr_shader->load_frag_file("ink/shaders/lib/SSR.frag.glsl");
}

void SSRPass::compile() {
	/* compile copy shader */
	copy_shader->compile();
	
	/* compile Hi-Z shader */
	hi_z_shader->compile();
	
	/* compile SSR shader */
	ssr_shader->compile();
}

void SSRPass::render() const {
	/* calculate max lod and Hierarchical-Z map size */
	Vec2 max_lod = {ceilf(log2f(width)) - 1, ceilf(log2f(height)) - 1};
	Vec2 z_map_size = {powf(2, max_lod.x), powf(2, max_lod.y)};
	
	/* the max level used in algorithm */
	int max_level = fmin(max_lod.x, max_lod.y);
	
	/* change the current viewport */
	Gpu::Rect viewport = RenderPass::get_viewport();
	RenderPass::set_viewport(Gpu::Rect(z_map_size.x, z_map_size.y));
	
	/* 1. copy depth buffer to Hierarchical-Z map */
	copy_shader->use_program();
	copy_shader->set_uniform_i("map", buffer_d->activate(0));
	hi_z_target->set_attachment(*z_map, 0, 0);
	RenderPass::render_to(copy_shader.get(), hi_z_target.get());
	
	/* 2. generate Hierarchical-Z map */
	Vec2 z_size = z_map_size / 2;
	for (int lod = 0; lod < max_level; ++lod) {
		
		/* set the viewport to Hierarchical-Z map size */
		RenderPass::set_viewport(Gpu::Rect(z_size.x, z_size.y));
		
		/* set the mipmap range of Hierarchical-Z map */
		/* to avoid reading and writing with the same texture */
		z_map->set_mipmap_range(lod, lod);
		
		/* render to lower level Hierarchical-Z map */
		hi_z_shader->use_program();
		hi_z_shader->set_uniform_v2("screen_size", z_size);
		hi_z_shader->set_uniform_i("lod", lod);
		hi_z_shader->set_uniform_i("map", z_map->activate(0));
		hi_z_target->set_attachment(*z_map, 0, lod + 1);
		RenderPass::render_to(hi_z_shader.get(), hi_z_target.get());
		
		/* update the size of Hierarchical-Z map */
		z_size.x = fmax(1, floorf(z_size.x / 2));
		z_size.y = fmax(1, floorf(z_size.y / 2));
	}
	
	/* set back to initial the mipmap range */
	z_map->set_mipmap_range(0, 1000);
	
	/* set back to the initial viewport */
	RenderPass::set_viewport(viewport);
	
	/* calculate camera parameters */
	Mat4 view_proj = camera->projection * camera->viewing;
	Mat4 inv_view_proj = inverse_4x4(view_proj);
	
	/* 3. render SSR mixing with input map to target */
	ssr_shader->use_program();
	ssr_shader->set_uniform_i("max_steps", max_steps);
	ssr_shader->set_uniform_i("max_level", max_level);
	ssr_shader->set_uniform_f("intensity", intensity);
	ssr_shader->set_uniform_f("thickness", thickness);
	ssr_shader->set_uniform_f("max_roughness", max_roughness);
	ssr_shader->set_uniform_v3("camera_pos", camera->position);
	ssr_shader->set_uniform_f("camera_near", camera->near);
	ssr_shader->set_uniform_f("camera_far", camera->far);
	ssr_shader->set_uniform_v2("z_map_size", z_map_size);
	ssr_shader->set_uniform_m4("view_proj", view_proj);
	ssr_shader->set_uniform_m4("inv_view_proj", inv_view_proj);
	ssr_shader->set_uniform_i("map", map->activate(0));
	ssr_shader->set_uniform_i("z_map", z_map->activate(1));
	ssr_shader->set_uniform_i("buffer_n", buffer_n->activate(2));
	ssr_shader->set_uniform_i("buffer_m", buffer_n->activate(3));
	RenderPass::render_to(ssr_shader.get(), target);
}

void SSRPass::process(const Camera& c) {
	camera = &c;
	process();
}

const Gpu::Texture* SSRPass::get_texture() const {
	return map;
}

void SSRPass::set_texture(const Gpu::Texture* t) {
	map = t;
}

const Gpu::Texture* SSRPass::get_buffer_n() const {
	return buffer_n;
}

void SSRPass::set_buffer_n(const Gpu::Texture* n) {
	buffer_n = n;
}

const Gpu::Texture* SSRPass::get_buffer_m() const {
	return buffer_m;
}

void SSRPass::set_buffer_m(const Gpu::Texture* m) {
	buffer_m = m;
}

const Gpu::Texture* SSRPass::get_buffer_d() const {
	return buffer_d;
}

void SSRPass::set_buffer_d(const Gpu::Texture* d) {
	buffer_d = d;
}

}
