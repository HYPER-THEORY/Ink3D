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

#include "../../ink/shaders/ShaderCache.h"
#include "../../ink/shaders/ShaderLib.h"

namespace Ink::Legacy {

SSAOPass::SSAOPass(int w, int h, float r, float m, float i) :
width(w), height(h), radius(r), max_radius(m), intensity(i) {}

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
	
	/* prepare blur render target 1 */
	blur_target_1 = std::make_unique<Gpu::RenderTarget>();
	blur_target_1->set_texture(*blur_map_1, 0);
	
	/* prepare blur render target 2 */
	blur_target_2 = std::make_unique<Gpu::RenderTarget>();
	blur_target_2->set_texture(*blur_map_2, 0);
}

void SSAOPass::render() {
	/* fetch SSAO shader from shader lib */
	if (!ShaderCache::has_vert("LegacySSAO")) {
		ShaderCache::load_vert("LegacySSAO", shader_vert);
	}
	if (!ShaderCache::has_frag("LegacySSAO")) {
		ShaderCache::load_frag("LegacySSAO", shader_frag);
	}
	Defines ssao_defines;
	ssao_defines.set("SAMPLES", std::to_string(samples));
	auto* ssao_shader = ShaderCache::fetch("LegacySSAO", ssao_defines);
	
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
	Mat4 view_proj = camera->projection * camera->viewing;
	Mat4 inv_view_proj = inverse_4x4(view_proj);
	Mat4 inv_proj = inverse_4x4(camera->projection);
	Vec2 screen_size = Vec2(width / 2, height / 2);
	
	/* change the current viewport */
	Gpu::Rect viewport = RenderPass::get_viewport();
	RenderPass::set_viewport(Gpu::Rect(width / 2, height / 2));
	
	/* 1. render SSAO to texture (down-sampling) */
	ssao_shader->use_program();
	ssao_shader->set_uniform_f("radius", radius);
	ssao_shader->set_uniform_f("max_radius", max_radius);
	ssao_shader->set_uniform_f("intensity", intensity);
	ssao_shader->set_uniform_f("bias", bias);
	ssao_shader->set_uniform_f("camera_near", camera->near);
	ssao_shader->set_uniform_f("camera_far", camera->far);
	ssao_shader->set_uniform_m4("view_proj", view_proj);
	ssao_shader->set_uniform_m4("inv_view_proj", inv_view_proj);
	ssao_shader->set_uniform_m4("inv_proj", inv_proj);
	ssao_shader->set_uniform_i("buffer_n", buffer_n->activate(0));
	ssao_shader->set_uniform_i("buffer_d", buffer_d->activate(1));
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

void SSAOPass::set(const Camera* c) {
	camera = c;
}

const Gpu::Texture* SSAOPass::get_texture() const {
	return map;
}

void SSAOPass::set_texture(const Gpu::Texture* t) {
	map = t;
}

const Gpu::Texture* SSAOPass::get_buffer_n() const {
	return buffer_n;
}

void SSAOPass::set_buffer_n(const Gpu::Texture* n) {
	buffer_n = n;
}

const Gpu::Texture* SSAOPass::get_buffer_d() const {
	return buffer_d;
}

void SSAOPass::set_buffer_d(const Gpu::Texture* d) {
	buffer_d = d;
}

std::string SSAOPass::shader_vert = R"(
#include <common>

in vec3 vertex;
in vec2 uv;

out vec2 v_uv;

void main() {
	v_uv = uv;
	gl_Position = vec4(vertex, 1.);
}
)";

std::string SSAOPass::shader_frag = R"(
#include <common>
#include <packing>
#include <transform>
#include <poisson>

#if SAMPLES == 16
#define POISSON_3D POISSON_3D_16
#elif SAMPLES == 32
#define POISSON_3D POISSON_3D_32
#elif SAMPLES == 64
#define POISSON_3D POISSON_3D_64
#endif

uniform sampler2D buffer_n;
uniform sampler2D buffer_d;

uniform float radius;
uniform float max_radius;
uniform float bias;
uniform float intensity;
uniform float camera_near;
uniform float camera_far;
uniform mat4 view_proj;
uniform mat4 inv_view_proj;

in vec2 v_uv;

layout(location = 0) out vec4 out_color;

bool out_of_screen(vec2 coord) {
	return any(bvec4(coord.x < 0., coord.x > 1., coord.y < 0., coord.y > 1.));
}

float ssao(vec3 world_pos, vec3 normal, float linear_depth, vec3 noise) {
	float ao_sum = 0.;
	float ao_count = 0.;
	float ao_bias = radius * linear_depth * bias;
	
	/* sample from the mix of poisson disks and noise */
	for (int i = 0; i < SAMPLES; ++i) {
		
		/* calculate offset along with the normal */
		vec3 offset = mix(POISSON_3D[i], noise, 0.2) * radius;
		offset *= step(0., dot(offset, normal)) * 2. - 1.;
		
		/* transform world space to screen space */
		vec4 ndc = view_proj * vec4(world_pos + offset, 1.);
		vec3 coord = ndc.xyz / ndc.w * 0.5 + 0.5;
		
		/* discard when the coord is out of screen */
		if (out_of_screen(coord.xy)) continue;
		
		/* calcualte linear depth A and B */
		float depth = textureLod(buffer_d, coord.xy, 0).x;
		float linear_a = linearize_depth_persp(depth, camera_near, camera_far);
		float linear_b = linearize_depth_persp(coord.z, camera_near, camera_far);
		
		/* check whether the sample is valid */
		float accept = step(linear_b - linear_a, max_radius);
		
		/* compare both depths and accumulate ao factor */
		ao_sum += accept * step(linear_a, linear_b - ao_bias) *
			dot(normalize(offset), normal) * (1 - length(offset));
		ao_count += accept * 0.5 + 0.5;
	}
	return ao_sum / max(0.1, ao_count);
}

void main() {
	/* sample depth from textures */
	float depth = textureLod(buffer_d, v_uv, 0).x;
	
	/* set the output with no AO */
	out_color = vec4(1.);
	
	/* ignore the pixels on skybox */
	if (depth == 1.) return;
	
	/* sample world depth from textures */
	vec3 normal = textureLod(buffer_n, v_uv, 0).xyz;
	
	/* normalize normal to avoid artifacts */
	normal = normalize(unpack_normal(normal));
	
	/* transform from screen space to world space */
	vec4 ndc = vec4(vec3(v_uv.xy, depth) * 2. - 1., 1.);
	vec4 world_pos = inv_view_proj * ndc;
	world_pos /= world_pos.w;
	
	/* calculate AO factor */
	float a = rand(vec3(v_uv, 0.));
	float b = rand(vec3(v_uv, 1.));
	float c = rand(vec3(v_uv, 2.));
	float linear_depth = linearize_depth_persp(depth, camera_near, camera_far);
	float factor = ssao(world_pos.xyz, normal, linear_depth, vec3(a, b, c));
	
	/* output AO factor to map */
	out_color = vec4(1. - intensity * factor);
}
)";

}
