/**
 * Copyright (C) 2021-2023 HYPERTHEORY
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

#include "../math/Constants.h"
#include "../math/Vector.h"
#include "../shaders/ShaderLib.h"

#include <format>

namespace ink {

constexpr Vec3 DODECAHEDRON_AXES[] = {
	{ 1.000000000,  1.000000000,  1.000000000},
	{-1.000000000,  1.000000000,  1.000000000},
	{ 1.000000000,  1.000000000, -1.000000000},
	{-1.000000000,  1.000000000, -1.000000000},
	{ 0.000000000,  1.618033989,  0.618033989},
	{ 0.000000000,  1.618033989, -0.618033989},
	{ 0.618033989,  0.000000000,  1.618033989},
	{-0.618033989,  0.000000000,  1.618033989},
	{ 1.618033989,  0.618033989,  0.000000000},
	{-1.618033989,  0.618033989,  0.000000000},
};

void IBLFilter::load_cubemap(const Image& px, const Image& nx,
							 const Image& py, const Image& ny,
							 const Image& pz, const Image& nz,
							 gpu::Texture& m, int s) {
	gpu::Texture source_map;
	source_map.init_cube(px, nx, py, ny, pz, nz,
						 gpu::Texture::default_format(px));
	source_map.set_filters(TEXTURE_LINEAR, TEXTURE_LINEAR);
	load_texture(source_map, m, s);
}

void IBLFilter::load_equirect(const Image& i, gpu::Texture& m, int s) {
	gpu::Texture source_map;
	source_map.init_2d(i, gpu::Texture::default_format(i));
	source_map.set_filters(TEXTURE_LINEAR, TEXTURE_LINEAR);
	load_texture(source_map, m, s);
}

void IBLFilter::load_texture(const gpu::Texture& t, gpu::Texture& m, int s) {
	/* initialize fullscreen plane vertex object */
	if (!fullscreen_plane) init_fullscreen_plane();
	
	/* set the states of GPU pipeline */
	gpu::State::disable_depth_test();
	gpu::State::disable_stencil_test();
	gpu::State::disable_scissor_test();
	gpu::State::disable_blending();
	gpu::State::disable_culling();
	gpu::State::disable_wireframe();
	gpu::State::enable_texture_cube_seamless();
	
	/* change the current viewport */
	gpu::State::set_viewport(gpu::Rect(s, s));
	
	/* initialize radiance environment map */
	m.init_cube(s, s, t.get_format());
	m.set_filters(TEXTURE_LINEAR, TEXTURE_LINEAR_MIPMAP_LINEAR);
	m.generate_mipmap();
	
	/* fetch the blur shader from shader lib */
	auto* blur_shader = ShaderLib::fetch("SphericalBlur");
	
	/* fetch the cubemap shader from shader lib */
	Defines cubemap_defines;
	int type = t.get_type();
	if (type == TEXTURE_2D) {
		cubemap_defines.set("USE_EQUIRECT");
	} else if (type == TEXTURE_CUBE) {
		cubemap_defines.set("USE_CUBEMAP");
	}
	auto* cubemap_shader = ShaderLib::fetch("Cubemap", cubemap_defines);
	
	/* prepare cubemap render target */
	if (!cubemap_target) {
		cubemap_target = std::make_unique<gpu::RenderTarget>();
	}
	
	/* render to cube texture */
	for (int i = 0; i < 6; ++i) {
		cubemap_target->set_texture(m, 0, 0, i);
		gpu::RenderTarget::activate(cubemap_target.get());
		cubemap_shader->use_program();
		cubemap_shader->set_uniform_i("face", i);
		cubemap_shader->set_uniform_i("map", t.activate(0));
		fullscreen_plane->attach(*cubemap_shader);
		fullscreen_plane->render();
	}
	
	/* prepare blur map */
	if (!blur_map) blur_map = std::make_unique<gpu::Texture>();
	blur_map->init_cube(s, s, t.get_format());
	blur_map->generate_mipmap();
	blur_map->set_filters(TEXTURE_LINEAR, TEXTURE_LINEAR_MIPMAP_LINEAR);
	
	/* prepare blur render target */
	if (!blur_target) {
		blur_target = std::make_unique<gpu::RenderTarget>();
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
		Vec3 pole_axis = DODECAHEDRON_AXES[(lod - 1) % 10];
		float d_theta = PI / (size_lod * 2 - 2);
		if (std::isinf(d_theta)) d_theta = PI;
		float sigma = sigma_radians / d_theta;
		int samples = 1 + floorf(sigma * 3);
		gaussian_weights(sigma, 20, weights);
		
		/* change the current viewport */
		gpu::State::set_viewport(gpu::Rect(size_lod / 2, size_lod / 2));
		
		/* blur texture latitudinally */
		for (int i = 0; i < 6; ++i) {
			blur_target->set_texture(*blur_map, 0, lod, i);
			gpu::RenderTarget::activate(blur_target.get());
			blur_shader->use_program();
			blur_shader->set_uniform_f("lod", lod - 1);
			blur_shader->set_uniform_i("face", i);
			blur_shader->set_uniform_i("samples", samples);
			blur_shader->set_uniform_i("latitudinal", 1);
			blur_shader->set_uniform_f("d_theta", d_theta);
			blur_shader->set_uniform_v3("pole_axis", pole_axis);
			blur_shader->set_uniform_i("map", m.activate(0));
			for (int w = 0; w < 20; ++w) {
				std::string weights_i = std::format("weights[{}]", w);
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
		samples = 1 + floorf(sigma * 3);
		gaussian_weights(sigma, 20, weights);
		
		/* blur texture longitudinally */
		for (int f = 0; f < 6; ++f) {
			blur_target->set_texture(m, 0, lod, f);
			gpu::RenderTarget::activate(blur_target.get());
			blur_shader->use_program();
			blur_shader->set_uniform_f("lod", lod);
			blur_shader->set_uniform_i("face", f);
			blur_shader->set_uniform_i("samples", samples);
			blur_shader->set_uniform_i("latitudinal", 0);
			blur_shader->set_uniform_f("d_theta", d_theta);
			blur_shader->set_uniform_v3("pole_axis", pole_axis);
			blur_shader->set_uniform_i("map", blur_map->activate(0));
			for (int w = 0; w < 20; ++w) {
				std::string weights_i = std::format("weights[{}]", w);
				blur_shader->set_uniform_f(weights_i, weights[w]);
			}
			fullscreen_plane->attach(*blur_shader);
			fullscreen_plane->render();
		}
	}
	
	/* set back to the default render target */
	gpu::RenderTarget::activate(nullptr);
}

void IBLFilter::init_fullscreen_plane() {
	/* prepare triangle mesh */
	Mesh triangle_mesh = Mesh("fullscreen");
	triangle_mesh.groups = {{"default", 0, 3}};
	triangle_mesh.vertex = {{-1, 3, 0}, {-1, -1, 0}, {3, -1, 0}};
	triangle_mesh.uv = {{0, 2}, {0, 0}, {2, 0}};
	
	/* prepare fullscreen plane vertex object */
	fullscreen_plane = std::make_unique<gpu::VertexObject>();
	fullscreen_plane->load(triangle_mesh, triangle_mesh.groups[0]);
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
	for (int i = 0; i < n; ++i) {
		w[i] /= weight_sum;
	}
}

std::unique_ptr<gpu::VertexObject> IBLFilter::fullscreen_plane;

std::unique_ptr<gpu::Texture> IBLFilter::blur_map;

std::unique_ptr<gpu::RenderTarget> IBLFilter::cubemap_target;

std::unique_ptr<gpu::RenderTarget> IBLFilter::blur_target;

}
