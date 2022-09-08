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

#include "LightPass.h"

#include "../core/Format.h"

namespace Ink {

void LightPass::init() {
	light_shader = std::make_unique<Gpu::Shader>();
	light_shader->load_vert_file("shaders/lib/lighting.vert.glsl");
	light_shader->load_frag_file("shaders/lib/lighting.frag.glsl");
}

void LightPass::compile() {
	/* create a new define */
	Defines defines;
	
	/* calculate the number of lights */
	auto num_point_light = std::to_string(scene->get_point_light_count());
	auto num_spot_light = std::to_string(scene->get_spot_light_count());
	auto num_directional_light = std::to_string(scene->get_directional_light_count());
	auto num_ambient_light = std::to_string(scene->get_ambient_light_count());
	auto num_hemisphere_light = std::to_string(scene->get_hemisphere_light_count());
	
	/* set the number of lights */
	defines.set("NUM_POINT_LIGHT"      , num_point_light      );
	defines.set("NUM_SPOT_LIGHT"       , num_spot_light       );
	defines.set("NUM_DIRECTIONAL_LIGHT", num_directional_light);
	defines.set("NUM_AMBIENT_LIGHT"    , num_ambient_light    );
	defines.set("NUM_HEMISPHERE_LIGHT" , num_hemisphere_light );
	
	/* set the mode of tone mapping */
	if (tone_map_mode == LINEAR_TONE_MAPPING) {
		defines.set("LINEAR_TONE_MAPPING");
	}
	if (tone_map_mode == REINHARD_TONE_MAPPING) {
		defines.set("REINHARD_TONE_MAPPING");
	}
	if (tone_map_mode == OPTIMIZED_TONE_MAPPING) {
		defines.set("OPTIMIZED_TONE_MAPPING");
	}
	if (tone_map_mode == ACES_FILMIC_TONE_MAPPING) {
		defines.set("ACES_FILMIC_TONE_MAPPING");
	}
	
	/* compile lighting shader */
	light_shader->set_define(defines);
	light_shader->compile();
}

void LightPass::render() const {
	/* use the program of light shader */
	light_shader->use_program();
	
	/* pass camera parameters to shader */
	Mat4 inv_view_proj = inverse_4x4(camera->projection * camera->viewing);
	light_shader->set_uniform_v3("camera_pos", camera->position);
	light_shader->set_uniform_m4("inv_view_proj", inv_view_proj);
	
	/* pass G-Buffers to shader */
	light_shader->set_uniform_i("gbuffer_c", gbuffer_c->activate(0));
	light_shader->set_uniform_i("gbuffer_n", gbuffer_n->activate(1));
	light_shader->set_uniform_i("gbuffer_m", gbuffer_m->activate(2));
	light_shader->set_uniform_i("gbuffer_d", gbuffer_d->activate(3));
	light_shader->set_uniform_i("gbuffer_e", gbuffer_e->activate(4));
	light_shader->set_uniform_i("gbuffer_i", gbuffer_i->activate(5));
	
	/* determines whether to enable shadow */
	bool enable_shadow = false;
	
	/* apply point lights on lighting */
	size_t point_light_count = scene->get_point_light_count();
	for (int i = 0; i < point_light_count; ++i) {
		
		/* pass the light informations to shader */
		auto& light = *scene->get_point_light(i);
		auto lights_i = str_format("point_lights[{}]", i);
		Vec3 light_color = light.color * light.intensity;
		light_shader->set_uniform_i(lights_i + ".visible", light.visible);
		light_shader->set_uniform_v3(lights_i + ".position", light.position);
		light_shader->set_uniform_v3(lights_i + ".color", light_color);
		light_shader->set_uniform_f(lights_i + ".distance", light.distance);
		light_shader->set_uniform_f(lights_i + ".decay", light.decay);
	}
	
	/* apply spot lights on lighting */
	size_t spot_light_count = scene->get_spot_light_count();
	for (int i = 0; i < spot_light_count; ++i) {
		
		/* pass the light informations to shader */
		auto& light = *scene->get_spot_light(i);
		std::string lights_i = str_format("spot_lights[{}]", i);
		Vec3 light_direction = -light.direction.normalize();
		Vec3 light_color = light.color * light.intensity;
		float light_angle = cosf(light.angle);
		float light_penumbra = cosf(light.angle * (1 - light.penumbra));
		light_shader->set_uniform_i(lights_i + ".visible", light.visible);
		light_shader->set_uniform_i(lights_i + ".shadow", light.cast_shadow);
		light_shader->set_uniform_v3(lights_i + ".position", light.position);
		light_shader->set_uniform_v3(lights_i + ".direction", light_direction);
		light_shader->set_uniform_v3(lights_i + ".color", light_color);
		light_shader->set_uniform_f(lights_i + ".distance", light.distance);
		light_shader->set_uniform_f(lights_i + ".decay", light.decay);
		light_shader->set_uniform_f(lights_i + ".angle", light_angle);
		light_shader->set_uniform_f(lights_i + ".penumbra", light_penumbra);
		
		/* check whether the light cast shadow */
		if (!light.cast_shadow) continue;
		enable_shadow = true;
		
		/* pass the shadow informations to shader */
		auto& shadow = light.shadow;
		std::string shadows_i = str_format("spot_shadows[{}]", i);
		Mat4 view_proj = shadow.camera.projection * shadow.camera.viewing;
		light_shader->set_uniform_i(shadows_i + ".type", shadow.type);
		light_shader->set_uniform_i(shadows_i + ".map_id", shadow.get_unique_id());
		light_shader->set_uniform_f(shadows_i + ".bias", shadow.bias);
		light_shader->set_uniform_f(shadows_i + ".normal_bias", shadow.normal_bias);
		light_shader->set_uniform_f(shadows_i + ".radius", shadow.radius);
		light_shader->set_uniform_m4(shadows_i + ".proj", shadow.camera.projection);
		light_shader->set_uniform_m4(shadows_i + ".view_proj", view_proj);
	}
	
	/* apply directional lights on lighting */
	size_t directional_light_count = scene->get_directional_light_count();
	for (int i = 0; i < directional_light_count; ++i) {
		
		/* pass the light informations to shader */
		auto& light = *scene->get_directional_light(i);
		std::string lights_i = str_format("directional_lights[{}]", i);
		Vec3 light_direction = -light.direction.normalize();
		Vec3 light_color = light.color * light.intensity;
		light_shader->set_uniform_i(lights_i + ".visible", light.visible);
		light_shader->set_uniform_i(lights_i + ".shadow", light.cast_shadow);
		light_shader->set_uniform_v3(lights_i + ".direction", light_direction);
		light_shader->set_uniform_v3(lights_i + ".color", light_color);
		
		/* check whether the light cast shadow */
		if (!light.cast_shadow) continue;
		enable_shadow = true;
		
		/* pass the shadow informations to shader */
		auto& shadow = light.shadow;
		std::string shadows_i = str_format("directional_shadows[{}]", i);
		Mat4 view_proj = shadow.camera.projection * shadow.camera.viewing;
		light_shader->set_uniform_i(shadows_i + ".type", shadow.type);
		light_shader->set_uniform_i(shadows_i + ".map_id", shadow.get_unique_id());
		light_shader->set_uniform_f(shadows_i + ".bias", shadow.bias);
		light_shader->set_uniform_f(shadows_i + ".normal_bias", shadow.normal_bias);
		light_shader->set_uniform_f(shadows_i + ".radius", shadow.radius);
		light_shader->set_uniform_m4(shadows_i + ".proj", shadow.camera.projection);
		light_shader->set_uniform_m4(shadows_i + ".view_proj", view_proj);
	}
	
	/* apply ambient lights on lighting */
	size_t ambient_light_count = scene->get_ambient_light_count();
	for (int i = 0; i < ambient_light_count; ++i) {
		
		/* pass the light informations to shader */
		auto& light = *scene->get_ambient_light(i);
		std::string lights_i = str_format("ambient_lights[{}]", i);
		Vec3 light_color = light.color * light.intensity;
		light_shader->set_uniform_i(lights_i + ".visible", light.visible);
		light_shader->set_uniform_v3(lights_i + ".color", light_color);
	}
	
	/* apply hemisphere lights on lighting */
	size_t hemisphere_light_count = scene->get_hemisphere_light_count();
	for (int i = 0; i < hemisphere_light_count; ++i) {
		
		/* pass the light informations to shader */
		auto& light = *scene->get_hemisphere_light(i);
		std::string lights_i = str_format("hemisphere_lights[{}]", i);
		Vec3 light_sky_color = light.color * light.intensity;
		Vec3 light_ground_color = light.ground_color * light.intensity;
		light_shader->set_uniform_i(lights_i + ".visible", light.visible);
		light_shader->set_uniform_v3(lights_i + ".direction", light.direction);
		light_shader->set_uniform_v3(lights_i + ".sky_color", light_sky_color);
		light_shader->set_uniform_v3(lights_i + ".ground_color", light_ground_color);
	}
	
	/* pass the shadow parameters to shader */
	if (enable_shadow) Shadow::activate_texture(6);
	light_shader->set_uniform_i("global_shadow.map", 6);
	light_shader->set_uniform_v2("global_shadow.size", Shadow::get_resolution());
	
	/* pass the tone mapping parameters to shader */
	light_shader->set_uniform_f("exposure", tone_map_exposure);
	
	/* render to target */
	RenderPass::render_to(light_shader.get(), target);
}

void LightPass::process(const Scene& s, const Camera& c) {
	scene = &s;
	camera = &c;
	process();
}

const Gpu::Texture* LightPass::get_gbuffer_c() const {
	return gbuffer_c;
}

void LightPass::set_gbuffer_c(const Gpu::Texture* t) {
	gbuffer_c = t;
}

const Gpu::Texture* LightPass::get_gbuffer_n() const {
	return gbuffer_n;
}

void LightPass::set_gbuffer_n(const Gpu::Texture* t) {
	gbuffer_n = t;
}

const Gpu::Texture* LightPass::get_gbuffer_m() const {
	return gbuffer_m;
}

void LightPass::set_gbuffer_m(const Gpu::Texture* t) {
	gbuffer_m = t;
}

const Gpu::Texture* LightPass::get_gbuffer_d() const {
	return gbuffer_d;
}

void LightPass::set_gbuffer_d(const Gpu::Texture* t) {
	gbuffer_d = t;
}

const Gpu::Texture* LightPass::get_gbuffer_e() const {
	return gbuffer_e;
}

void LightPass::set_gbuffer_e(const Gpu::Texture* t) {
	gbuffer_e = t;
}

const Gpu::Texture* LightPass::get_gbuffer_i() const {
	return gbuffer_i;
}

void LightPass::set_gbuffer_i(const Gpu::Texture* t) {
	gbuffer_i = t;
}

void LightPass::set_tone_mapping(int m, float e) {
	tone_map_mode = m;
	tone_map_exposure = e;
}

}
