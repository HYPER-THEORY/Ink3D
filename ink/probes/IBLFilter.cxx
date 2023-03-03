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

#include "IBLFilter.h"

#include "../core/Format.h"
#include "../math/Constants.h"
#include "../math/Vector.h"
#include "../shaders/ShaderLib.h"

namespace Ink {

constexpr float PHI = 1.618033989;
constexpr float INV_PHI = 0.618033989;

void IBLFilter::load_cubemap(const Image& px, const Image& nx,
							 const Image& py, const Image& ny,
							 const Image& pz, const Image& nz,
							 Gpu::Texture& m, int s) {
	Gpu::Texture source_map;
	source_map.init_cube(px, nx, py, ny, pz, nz,
						 Gpu::Texture::default_format(px));
	source_map.set_filters(TEXTURE_LINEAR, TEXTURE_LINEAR);
	load_texture(source_map, m, s);
}

void IBLFilter::load_equirect(const Image& i, Gpu::Texture& m, int s) {
	Gpu::Texture source_map;
	source_map.init_2d(i, Gpu::Texture::default_format(i));
	source_map.set_filters(TEXTURE_LINEAR, TEXTURE_LINEAR);
	load_texture(source_map, m, s);
}

void IBLFilter::load_texture(const Gpu::Texture& t, Gpu::Texture& m, int s) {
	/* initialize fullscreen plane */
	[[maybe_unused]]
	static bool inited = init_fullscreen_plane();
	
	/* disable depth & stencil & scissor test */
	Gpu::State::disable_depth_test();
	Gpu::State::disable_stencil_test();
	Gpu::State::disable_scissor_test();
	
	/* disable blending & wireframe & culling */
	Gpu::State::disable_blending();
	Gpu::State::disable_wireframe();
	Gpu::State::disable_culling();
	
	/* enable texture cube seamless */
	Gpu::State::enable_texture_cube_seamless();
	
	/* change the current viewport */
	Gpu::State::set_viewport(Gpu::Rect(s, s));
	
	/* initialize radiance environment map */
	m.init_cube(s, s, t.get_format());
	m.set_filters(TEXTURE_LINEAR, TEXTURE_LINEAR_MIPMAP_LINEAR);
	m.generate_mipmap();
	
	/* fetch the blur shader from shader lib */
	auto* blur_shader = ShaderLib::fetch("SphericalBlur");
	
	/* fetch the cubemap shader from shader lib */
	Defines cubemap_defines;
	int type = t.get_type();
	cubemap_defines.set_if("USE_EQUIRECT", type == TEXTURE_2D);
	cubemap_defines.set_if("USE_CUBEMAP", type == TEXTURE_CUBE);
	auto* cubemap_shader = ShaderLib::fetch("Cubemap", cubemap_defines);
	
	/* prepare cubemap render target */
	if (!cubemap_target) {
		cubemap_target = std::make_unique<Gpu::RenderTarget>();
	}
	
	/* render to cube texture */
	for (int i = 0; i < 6; ++i) {
		cubemap_target->set_texture(m, 0, 0, i);
		Gpu::RenderTarget::activate(cubemap_target.get());
		cubemap_shader->use_program();
		cubemap_shader->set_uniform_i("face", i);
		cubemap_shader->set_uniform_i("map", t.activate(0));
		fullscreen_plane->attach(*cubemap_shader);
		fullscreen_plane->render();
	}
	
	/* prepare blur map */
	if (!blur_map) blur_map = std::make_unique<Gpu::Texture>();
	blur_map->init_cube(s, s, t.get_format());
	blur_map->generate_mipmap();
	blur_map->set_filters(TEXTURE_LINEAR, TEXTURE_LINEAR_MIPMAP_LINEAR);
	
	/* prepare blur render target */
	if (!blur_target) {
		blur_target = std::make_unique<Gpu::RenderTarget>();
	}
	
	/* blur cube texture latitudinally and longitudinally */
	float weights[20];
	int size_lod = s;
	int max_lod = log2f(size_lod);
	for (int lod = 1; lod <= max_lod; ++lod) {
		
		/* calculate sigma radians */
		float sigma_radians = sqrtf(3) / size_lod;
		if (lod == 1) sigma_radians = 2.f / size_lod;
		
		/* calculate blur parameters */
		Vec3 pole_axis = axes[(lod - 1) % 10];
		float d_theta = PI / (size_lod * 2 - 2);
		float sigma = sigma_radians / d_theta;
		int samples = 1 + floorf(sigma * 3);
		gaussian_weights(sigma, 20, weights);
		
		/* change the current viewport */
		Gpu::State::set_viewport(Gpu::Rect(size_lod / 2, size_lod / 2));
		
		/* blur texture latitudinally */
		for (int i = 0; i < 6; ++i) {
			blur_target->set_texture(*blur_map, 0, lod, i);
			Gpu::RenderTarget::activate(blur_target.get());
			blur_shader->use_program();
			blur_shader->set_uniform_f("lod", lod - 1);
			blur_shader->set_uniform_i("face", i);
			blur_shader->set_uniform_i("samples", samples);
			blur_shader->set_uniform_i("latitudinal", 1);
			blur_shader->set_uniform_f("d_theta", d_theta);
			blur_shader->set_uniform_v3("pole_axis", pole_axis);
			blur_shader->set_uniform_i("map", m.activate(0));
			for (int w = 0; w < 20; ++w) {
				std::string weights_i = Format::format("weights[{}]", w);
				blur_shader->set_uniform_f(weights_i, weights[w]);
			}
			fullscreen_plane->attach(*blur_shader);
			fullscreen_plane->render();
		}
		
		/* calculate blur parameters */
		size_lod /= 2;
		d_theta = PI / (size_lod * 2 - 2);
		if (std::isinf(d_theta)) d_theta = PI;
		sigma = sigma_radians / d_theta;
		samples = 1 + floor(sigma * 3);
		gaussian_weights(sigma, 20, weights);
		
		/* blur texture longitudinally */
		for (int f = 0; f < 6; ++f) {
			blur_target->set_texture(m, 0, lod, f);
			Gpu::RenderTarget::activate(blur_target.get());
			blur_shader->use_program();
			blur_shader->set_uniform_f("lod", lod);
			blur_shader->set_uniform_i("face", f);
			blur_shader->set_uniform_i("samples", samples);
			blur_shader->set_uniform_i("latitudinal", 0);
			blur_shader->set_uniform_f("d_theta", d_theta);
			blur_shader->set_uniform_v3("pole_axis", pole_axis);
			blur_shader->set_uniform_i("map", blur_map->activate(0));
			for (int w = 0; w < 20; ++w) {
				std::string weights_i = Format::format("weights[{}]", w);
				blur_shader->set_uniform_f(weights_i, weights[w]);
			}
			fullscreen_plane->attach(*blur_shader);
			fullscreen_plane->render();
		}
	}
	
	/* set back to the default render target */
	Gpu::RenderTarget::activate(nullptr);
}

bool IBLFilter::init_fullscreen_plane() {
	/* prepare triangle mesh */
	Mesh triangle_mesh = Mesh("fullscreen");
	triangle_mesh.groups = {{"default", 0, 3}};
	triangle_mesh.vertex = {{-1, 3, 0}, {-1, -1, 0}, {3, -1, 0}};
	triangle_mesh.uv = {{0, 2}, {0, 0}, {2, 0}};
	
	/* prepare fullscreen plane vertex object */
	fullscreen_plane = std::make_unique<Gpu::VertexObject>();
	fullscreen_plane->load(triangle_mesh, triangle_mesh.groups[0]);
	
	return true; /* finish */
}

void IBLFilter::gaussian_weights(float s, int n, float* w) {
	float weight_sum = 0;
	for (int i = 0; i < n; ++i) {
		float x = i / s;
		float weight = expf(-x * x * .5f);
		w[i] = weight;
		if (i == 0) {
			weight_sum += weight;
		} else {
			weight_sum += weight * 2;
		}
	}
	for (int i = 0; i < 20; ++i) {
		w[i] /= weight_sum;
	}
}

Vec3 IBLFilter::axes[] = {
	{ 1,     1,      1},
	{-1,     1,      1},
	{ 1,     1,     -1},
	{-1,     1,     -1},
	{ 0, PHI,  INV_PHI},
	{ 0, PHI, -INV_PHI},
	{ INV_PHI, 0,  PHI},
	{-INV_PHI, 0,  PHI},
	{ PHI, INV_PHI,  0},
	{-PHI, INV_PHI,  0},
};

std::unique_ptr<Gpu::VertexObject> IBLFilter::fullscreen_plane;

std::unique_ptr<Gpu::Texture> IBLFilter::blur_map;

std::unique_ptr<Gpu::RenderTarget> IBLFilter::cubemap_target;

std::unique_ptr<Gpu::RenderTarget> IBLFilter::blur_target;

}
