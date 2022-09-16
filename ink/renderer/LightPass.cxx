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
	light_shader->load_vert_file("ink/shaders/lib/lighting.vert.glsl");
	light_shader->load_frag_file("ink/shaders/lib/lighting.frag.glsl");
}

void LightPass::compile() {
	/* create light shader defines */
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
	
	/* set the whether to use linear fog */
	if (scene->get_linear_fog() != nullptr) {
		defines.set("USE_LINEAR_FOG");
	}
	
	/* set the whether to use exp fog */
	if (scene->get_exp_fog() != nullptr) {
		defines.set("USE_EXP_FOG");
	}
	
	/* set the mode of tone mapping */
	if (tone_mapping == TONE_MAPPING_LINEAR) {
		defines.set("TONE_MAPPING_LINEAR");
	}
	if (tone_mapping == TONE_MAPPING_REINHARD) {
		defines.set("TONE_MAPPING_REINHARD");
	}
	if (tone_mapping == TONE_MAPPING_OPTIMIZED) {
		defines.set("TONE_MAPPING_OPTIMIZED");
	}
	if (tone_mapping == TONE_MAPPING_ACES_FILMIC) {
		defines.set("TONE_MAPPING_ACES_FILMIC");
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
	light_shader->set_uniform_i("buffer_c", buffer_c->activate(0));
	light_shader->set_uniform_i("buffer_n", buffer_n->activate(1));
	light_shader->set_uniform_i("buffer_m", buffer_m->activate(2));
	light_shader->set_uniform_i("buffer_l", buffer_l->activate(3));
	light_shader->set_uniform_i("buffer_d", buffer_d->activate(4));
	
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
	
	/* pass linear fog parameters to shader */
	auto* linear_fog = scene->get_linear_fog();
	if (linear_fog != nullptr) {
		light_shader->set_uniform_i("linear_fog.visible", linear_fog->visible);
		light_shader->set_uniform_v3("linear_fog.color", linear_fog->color);
		light_shader->set_uniform_f("linear_fog.near", linear_fog->near);
		light_shader->set_uniform_f("linear_fog.far", linear_fog->far);
	}
	
	/* pass linear fog parameters to shader */
	auto* exp_fog = scene->get_exp_fog();
	if (exp_fog != nullptr) {
		light_shader->set_uniform_i("exp_fog.visible", exp_fog->visible);
		light_shader->set_uniform_v3("exp_fog.color", exp_fog->color);
		light_shader->set_uniform_f("exp_fog.near", exp_fog->near);
		light_shader->set_uniform_f("exp_fog.density", exp_fog->density);
	}
	
	/* pass the tone mapping parameters to shader */
	light_shader->set_uniform_f("exposure", tone_mapping_exposure);
	
	/* render to target */
	RenderPass::render_to(light_shader.get(), target);
}

void LightPass::process(const Scene& s, const Camera& c) {
	scene = &s;
	camera = &c;
	process();
}

const Gpu::Texture* LightPass::get_buffer_c() const {
	return buffer_c;
}

void LightPass::set_buffer_c(const Gpu::Texture* t) {
	buffer_c = t;
}

const Gpu::Texture* LightPass::get_buffer_n() const {
	return buffer_n;
}

void LightPass::set_buffer_n(const Gpu::Texture* t) {
	buffer_n = t;
}

const Gpu::Texture* LightPass::get_buffer_m() const {
	return buffer_m;
}

void LightPass::set_buffer_m(const Gpu::Texture* t) {
	buffer_m = t;
}

const Gpu::Texture* LightPass::get_buffer_l() const {
	return buffer_l;
}

void LightPass::set_buffer_l(const Gpu::Texture* t) {
	buffer_l = t;
}

const Gpu::Texture* LightPass::get_buffer_d() const {
	return buffer_d;
}

void LightPass::set_buffer_d(const Gpu::Texture* t) {
	buffer_d = t;
}

void LightPass::set_tone_mapping(int m, float e) {
	tone_mapping = m;
	tone_mapping_exposure = e;
}

}
