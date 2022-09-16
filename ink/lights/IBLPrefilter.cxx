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

#include "IBLPrefilter.h"

#include "../core/Format.h"
#include "../meshes/BoxMesh.h"

namespace Ink {

const float PHI = (1 + sqrtf(5)) / 2;
const float IPHI = 1 / PHI;

const Vec3 axises[] = {
	{ 1   , 1   ,  1   },
	{-1   , 1   ,  1   },
	{ 1   , 1   , -1   },
	{-1   , 1   , -1   },
	{ 0   , PHI ,  IPHI},
	{ 0   , PHI , -IPHI},
	{ IPHI, 0   ,  PHI },
	{-IPHI, 0   ,  PHI },
	{ PHI , IPHI,  0   },
	{-PHI , IPHI,  0   },
};

void IBLPrefilter::init() {
	/* perpare plane mesh */
	Mesh plane_mesh = {"plane"};
	plane_mesh.groups = {{"default", 0, 3}};
	plane_mesh.vertex = {{-1, 3, 0}, {-1, -1, 0}, {3, -1, 0}};
	plane_mesh.uv = {{0, 2}, {0, 0}, {2, 0}};
	
	/* perpare plane vertex object */
	plane = std::make_unique<Gpu::VertexObject>();
	plane->load(plane_mesh);
	
	/* prepare blur map */
	blur_map = std::make_unique<Gpu::Texture>();
	
	/* prepare cubemap frame buffer */
	cubemap_target = std::make_unique<Gpu::FrameBuffer>();
	cubemap_target->draw_attachments({0});
	
	/* prepare blur frame buffer */
	blur_target = std::make_unique<Gpu::FrameBuffer>();
	blur_target->draw_attachments({0});
	
	/* prepare cubemap shader */
	cubemap_shader = std::make_unique<Gpu::Shader>();
	cubemap_shader->load_vert_file("ink/shaders/lib/cubemap.vert.glsl");
	cubemap_shader->load_frag_file("ink/shaders/lib/cubemap.frag.glsl");
	
	/* prepare and compile blur shader */
	blur_shader = std::make_unique<Gpu::Shader>();
	blur_shader->load_vert_file("ink/shaders/lib/sphericalblur.vert.glsl");
	blur_shader->load_frag_file("ink/shaders/lib/sphericalblur.frag.glsl");
	blur_shader->compile();
}

void IBLPrefilter::load_cubemap(const Image& px, const Image& nx, const Image& py,
								const Image& ny, const Image& pz, const Image& nz,
								Gpu::Texture& m, int s) {
	Gpu::Texture source_map;
	source_map.init_cube(px, nx, py, ny, pz, nz, Gpu::Texture::default_format(px));
	source_map.set_filters(TEXTURE_LINEAR, TEXTURE_LINEAR);
	load_texture(source_map, m, s);
}

void IBLPrefilter::load_equirect(const Image& i, Gpu::Texture& m, int s) {
	Gpu::Texture source_map;
	source_map.init_2d(i, Gpu::Texture::default_format(i));
	source_map.set_filters(TEXTURE_LINEAR, TEXTURE_LINEAR);
	load_texture(source_map, m, s);
}

void IBLPrefilter::load_texture(const Gpu::Texture& t, Gpu::Texture& m, int s) {
	Gpu::enable_texture_cube_seamless();
	Gpu::disable_depth_test();
	Gpu::disable_stencil_test();
	Gpu::disable_blending();
	Gpu::disable_wireframe();
	Gpu::disable_cull_face();
	Gpu::disable_scissor_test();
	Gpu::set_viewport({0, 0, s, s});
	
	/* initialize cube texture */
	m.init_cube(s, s, t.get_format());
	m.set_filters(TEXTURE_LINEAR, TEXTURE_LINEAR_MIPMAP_LINEAR);
	m.generate_mipmap();
	
	/* compile the cubemap shader */
	if (t.get_type() == TEXTURE_2D) {
		cubemap_shader->set_define({{{"USE_EQUIRECT", ""}}});
	} else {
		cubemap_shader->set_define({{{"USE_CUBEMAP", ""}}});
	}
	cubemap_shader->compile();
	
	/* render to cube texture */
	for (int f = 0; f < 6; ++f) {
		cubemap_target->set_attachment(m, 0, 0, f);
		Gpu::FrameBuffer::activate(cubemap_target.get());
		cubemap_shader->use_program();
		cubemap_shader->set_uniform_i("face", f);
		cubemap_shader->set_uniform_i("map", t.activate(0));
		plane->attach(*cubemap_shader);
		plane->draw();
	}
	
	/* initialize the half blur map */
	blur_map->init_cube(s, s, t.get_format());
	blur_map->generate_mipmap();
	blur_map->set_filters(TEXTURE_LINEAR, TEXTURE_LINEAR_MIPMAP_LINEAR);
	
	/* blur cube texture latitudinally and longitudinally */
	float weights[20];
	int size_lod = s;
	int max_lod = log2f(size_lod);
	for (int lod = 1; lod <= max_lod; ++lod) {
		
		/* calculate sigma_radians */
		float sigma_radians = sqrtf(3) / size_lod;
		if (lod == 1) sigma_radians = 2. / size_lod;
		
		/* calculate blur parameters */
		Vec3 pole_axis = axises[(lod - 1) % 10];
		float d_theta = PI / (size_lod * 2 - 2);
		float sigma = sigma_radians / d_theta;
		int samples = 1 + floorf(sigma * 3);
		get_weights(sigma, 20, weights);
		
		/* blur texture latitudinally */
		Gpu::set_viewport({0, 0, size_lod / 2, size_lod / 2});
		for (int f = 0; f < 6; ++f) {
			blur_target->set_attachment(*blur_map, 0, lod, f);
			Gpu::FrameBuffer::activate(blur_target.get());
			blur_shader->use_program();
			blur_shader->set_uniform_i("face", f);
			blur_shader->set_uniform_i("samples", samples);
			blur_shader->set_uniform_i("latitudinal", 1);
			blur_shader->set_uniform_f("d_theta", d_theta);
			blur_shader->set_uniform_f("lod", lod - 1);
			blur_shader->set_uniform_v3("pole_axis", pole_axis);
			blur_shader->set_uniform_i("map", m.activate(0));
			for (int w = 0; w < 20; ++w) {
				std::string weights_i = str_format("weights[{}]", w);
				blur_shader->set_uniform_f(weights_i, weights[w]);
			}
			plane->attach(*blur_shader);
			plane->draw();
		}
		
		/* calculate blur parameters */
		size_lod /= 2;
		d_theta = PI / (size_lod * 2 - 2);
		if (std::isinf(d_theta)) d_theta = PI;
		sigma = sigma_radians / d_theta;
		samples = 1 + floor(sigma * 3);
		get_weights(sigma, 20, weights);
		
		/* blur texture longitudinally */
		for (int f = 0; f < 6; ++f) {
			blur_target->set_attachment(m, 0, lod, f);
			Gpu::FrameBuffer::activate(blur_target.get());
			blur_shader->use_program();
			blur_shader->set_uniform_i("face", f);
			blur_shader->set_uniform_i("samples", samples);
			blur_shader->set_uniform_i("latitudinal", 0);
			blur_shader->set_uniform_f("d_theta", d_theta);
			blur_shader->set_uniform_f("lod", lod);
			blur_shader->set_uniform_v3("pole_axis", pole_axis);
			blur_shader->set_uniform_i("map", blur_map->activate(0));
			for (int w = 0; w < 20; ++w) {
				std::string weights_i = str_format("weights[{}]", w);
				blur_shader->set_uniform_f(weights_i, weights[w]);
			}
			plane->attach(*blur_shader);
			plane->draw();
		}
	}
	Gpu::FrameBuffer::activate(nullptr);
}

void IBLPrefilter::get_weights(float s, int n, float* w) {
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

std::unique_ptr<Gpu::VertexObject> IBLPrefilter::plane;

std::unique_ptr<Gpu::Texture> IBLPrefilter::blur_map;

std::unique_ptr<Gpu::FrameBuffer> IBLPrefilter::cubemap_target;
std::unique_ptr<Gpu::FrameBuffer> IBLPrefilter::blur_target;

std::unique_ptr<Gpu::Shader> IBLPrefilter::cubemap_shader;
std::unique_ptr<Gpu::Shader> IBLPrefilter::blur_shader;

}
