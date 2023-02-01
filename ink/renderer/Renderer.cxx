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

#include "Renderer.h"

#include "../core/Error.h"
#include "../core/Format.h"
#include "../meshes/BoxMesh.h"
#include "../shaders/ShaderLib.h"

namespace Ink {

const Vec3 CUBE_DIRECTIONS[] = {
	{-1, 0, 0}, {1, 0, 0}, {0, -1, 0},
	{0, 1, 0}, {0, 0, -1}, {0, 0, 1},
};

const Vec3 CUBE_UPS[] = {
	{0, -1, 0}, {0, -1, 0}, {0, 0, 1},
	{0, 0, -1}, {0, -1, 0}, {0, -1, 0},
};

Vec4 Renderer::get_clear_color() const {
	return clear_color;
}

void Renderer::set_clear_color(const Vec4& c) {
	clear_color = c;
}

void Renderer::clear(bool c, bool d, bool s) const {
	Gpu::RenderTarget::activate(target);
	Gpu::State::set_clear_color(clear_color);
	Gpu::State::clear(c, d, s);
	Gpu::RenderTarget::activate(nullptr);
}

Gpu::Rect Renderer::get_viewport() const {
	return viewport;
}

void Renderer::set_viewport(const Gpu::Rect& v) {
	viewport = v;
}

bool Renderer::get_scissor_test() const {
	return scissor_test;
}

void Renderer::set_scissor_test(bool t) {
	scissor_test = t;
}

Gpu::Rect Renderer::get_scissor() const {
	return scissor;
}

void Renderer::set_scissor(const Gpu::Rect& s) {
	scissor = s;
}

int Renderer::get_rendering_mode() const {
	return rendering_mode;
}

void Renderer::set_rendering_mode(int m) {
	rendering_mode = m;
}

int Renderer::get_tone_map_mode() const {
	return tone_map_mode;
}

float Renderer::get_tone_map_exposure() const {
	return tone_map_exposure;
}

void Renderer::set_tone_map(int m, float e) {
	tone_map_mode = m;
	tone_map_exposure = e;
}

const Gpu::RenderTarget* Renderer::get_target() const {
	return target;
}

void Renderer::set_target(const Gpu::RenderTarget *t) {
	target = t;
}

void Renderer::set_texture_callback(const TextureCallback& f) {
	texture_callback = f;
}

float Renderer::get_skybox_intensity() const {
	return skybox_intensity;
}

void Renderer::set_skybox_intensity(float i) {
	skybox_intensity = i;
}

void Renderer::load_skybox(const Image& px, const Image& nx,
						   const Image& py, const Image& ny,
						   const Image& pz, const Image& nz) {
	skybox_map = std::make_unique<Gpu::Texture>();
	skybox_map->init_cube(px, nx, py, ny, pz, nz,
						  Gpu::Texture::default_format(px));
	skybox_map->set_filters(TEXTURE_LINEAR, TEXTURE_LINEAR);
}

void Renderer::load_skybox(const Image& i) {
	skybox_map = std::make_unique<Gpu::Texture>();
	skybox_map->init_2d(i, Gpu::Texture::default_format(i));
	skybox_map->set_filters(TEXTURE_LINEAR, TEXTURE_LINEAR);
}

void Renderer::render_skybox(const Camera& c) const {
	/* activate the render target */
	Gpu::RenderTarget::activate(target);
	
	/* set the viewport region */
	Gpu::State::set_viewport(viewport);
	
	/* set the scissor test and region */
	if (scissor_test) {
		Gpu::State::enable_scissor_test();
		Gpu::State::set_scissor(scissor);
	} else {
		Gpu::State::disable_scissor_test();
	}
	
	/* render the skybox to buffer */
	render_skybox_to_buffer(c, rendering_mode);
	
	/* set back to the default render target */
	Gpu::RenderTarget::activate(nullptr);
}

void Renderer::load_mesh(const Mesh* m) {
	if (mesh_cache.count(m) != 0) return;
	size_t size = m->groups.size();
	auto p = mesh_cache.insert({m, std::make_unique<Gpu::VertexObject[]>(size)});
	auto* vertex_object = p.first->second.get();
	for (int i = 0; i < size; ++i) {
		vertex_object[i].load(*m, m->groups[i]);
	}
}

void Renderer::unload_mesh(const Mesh* m) {
	mesh_cache.erase(m);
}

void Renderer::load_image(const Image* i) {
	if (image_cache.count(i) != 0) return;
	auto p = image_cache.insert({i, std::make_unique<Gpu::Texture>()});
	auto* texture = p.first->second.get();
	texture->init_2d(*i, Gpu::Texture::default_format(*i));
	std::invoke(texture_callback, *texture);
}

void Renderer::unload_image(const Image* i) {
	image_cache.erase(i);
}

void Renderer::load_scene(const Scene& s) {
	/* load the meshes linked with instance */
	for (auto& instance : s.to_instances()) {
		auto* mesh = instance->mesh;
		if (mesh != nullptr) load_mesh(mesh);
	}
	
	/* load the images linked with instance */
	for (auto& material : s.get_materials()) {
		for (int i = 0; i < 16; ++i) {
			auto* image = material->get_image(i);
			if (image != nullptr) load_image(image);
		}
		if (material->normal_map != nullptr) {
			load_image(material->normal_map);
		}
		if (material->displacement_map != nullptr) {
			load_image(material->displacement_map);
		}
		if (material->color_map != nullptr) {
			load_image(material->color_map);
		}
		if (material->alpha_map != nullptr) {
			load_image(material->alpha_map);
		}
		if (material->emissive_map != nullptr) {
			load_image(material->emissive_map);
		}
		if (material->ao_map != nullptr) {
			load_image(material->ao_map);
		}
		if (material->roughness_map != nullptr) {
			load_image(material->roughness_map);
		}
		if (material->metalness_map != nullptr) {
			load_image(material->metalness_map);
		}
		if (material->specular_map != nullptr) {
			load_image(material->specular_map);
		}
	}
}

void Renderer::unload_scene(const Scene& s) {
	/* unload the meshes linked with instance */
	for (auto& instance : s.to_instances()) {
		auto* mesh = instance->mesh;
		if (mesh != nullptr) unload_mesh(mesh);
	}
	
	/* unload the images linked with instance */
	for (auto& material : s.get_materials()) {
		for (int i = 0; i < 16; ++i) {
			auto* image = material->get_image(i);
			if (image != nullptr) unload_image(image);
		}
		if (material->normal_map != nullptr) {
			unload_image(material->normal_map);
		}
		if (material->displacement_map != nullptr) {
			unload_image(material->displacement_map);
		}
		if (material->color_map != nullptr) {
			unload_image(material->color_map);
		}
		if (material->alpha_map != nullptr) {
			unload_image(material->alpha_map);
		}
		if (material->emissive_map != nullptr) {
			unload_image(material->emissive_map);
		}
		if (material->ao_map != nullptr) {
			unload_image(material->ao_map);
		}
		if (material->roughness_map != nullptr) {
			unload_image(material->roughness_map);
		}
		if (material->metalness_map != nullptr) {
			unload_image(material->metalness_map);
		}
		if (material->specular_map != nullptr) {
			unload_image(material->specular_map);
		}
	}
}

void Renderer::clear_scene_caches() {
	mesh_cache.clear();
	image_cache.clear();
}

void Renderer::render(const Scene& s, const Camera& c) const {
	/* activate the render target */
	Gpu::RenderTarget::activate(target);
	
	/* set the viewport region */
	Gpu::State::set_viewport(viewport);
	
	/* set the scissor test and region */
	if (scissor_test) {
		Gpu::State::enable_scissor_test();
		Gpu::State::set_scissor(scissor);
	} else {
		Gpu::State::disable_scissor_test();
	}
	
	/* render the scene to buffer */
	render_to_buffer(s, c, rendering_mode, false);
	
	/* set back to the default render target */
	Gpu::RenderTarget::activate(nullptr);
}

void Renderer::render_transparent(const Scene& s, const Camera& c) const {
	/* activate the render target */
	Gpu::RenderTarget::activate(target);
	
	/* set the viewport region */
	Gpu::State::set_viewport(viewport);
	
	/* set the scissor test and region */
	if (scissor_test) {
		Gpu::State::enable_scissor_test();
		Gpu::State::set_scissor(scissor);
	} else {
		Gpu::State::disable_scissor_test();
	}
	
	/* render the scene to buffer */
	Gpu::State::set_depth_writemask(false);
	render_to_buffer(s, c, rendering_mode, true);
	Gpu::State::set_depth_writemask(true);
	
	/* set back to the default render target */
	Gpu::RenderTarget::activate(nullptr);
}

void Renderer::render_shadow(const Scene& s, const Shadow& t) const {
	/* activate the render target */
	Gpu::RenderTarget::activate(t.get_target());
	
	/* set the viewport to fit the resolution of shadow map */
	Vec2 resolution = Shadow::get_resolution();
	Gpu::State::set_viewport(Gpu::Rect(resolution.x, resolution.y));
	
	/* disable the scissor test */
	Gpu::State::disable_scissor_test();
	
	/* clear the shadow map (depth only) */
	Gpu::State::clear(false, true, false);
	
	/* render the scene to shadow */
	render_to_shadow(s, t.camera);
	
	/* set back to the default render target */
	Gpu::RenderTarget::activate(nullptr);
}

void Renderer::update_shadow(const Scene& s, SpotLight& l) const {
	Vec3 up = l.direction.cross({0, 0, 1});
	if (up.magnitude() < 1E-4) up = {0, 1, 0};
	l.shadow.camera.lookat(l.position, -l.direction, up);
	render_shadow(s, l.shadow);
}

void Renderer::update_shadow(const Scene& s, DirectionalLight& l) const {
	Vec3 up = l.direction.cross({0, 0, 1});
	if (up.magnitude() < 1E-4) up = {0, 1, 0};
	l.shadow.camera.lookat(l.position, -l.direction, up);
	render_shadow(s, l.shadow);
}

void Renderer::update_probe(const Scene& s, const Vec3& p, ReflectionProbe& r) const {
	/* set the viewport with the resolution of probe */
	Gpu::State::set_viewport(Gpu::Rect(r.resolution, r.resolution));
	
	/* disable the scissor test */
	Gpu::State::disable_scissor_test();
	
	/* prepare texture for probe */
	if (!probe_map) {
		probe_map = std::make_unique<Gpu::Texture>();
	}
	probe_map->init_cube(r.resolution, r.resolution, TEXTURE_R8G8B8A8_UNORM);
	probe_map->set_filters(TEXTURE_LINEAR, TEXTURE_LINEAR);
	
	/* prepare depth texture for rendering */
	if (!probe_buffer) {
		probe_buffer = std::make_unique<Gpu::RenderBuffer>();
	}
	probe_buffer->init(r.resolution, r.resolution, TEXTURE_D24_UNORM);
	
	/* prepare frame buffer for probe */
	if (!probe_target) {
		probe_target = std::make_unique<Gpu::RenderTarget>();
	}
	probe_target->set_depth_buffer(*probe_buffer);
	
	/* initialize camera for probe */
	PerspCamera camera = PerspCamera(PI_2, 1, 0.1, 100);
	
	for (int i = 0; i < 6; ++i) {
		/* update camera for each side of cube */
		camera.lookat(p, CUBE_DIRECTIONS[i], CUBE_UPS[i]);
		
		/* update and activate render target */
		probe_target->set_texture(*probe_map, 0, 0, i);
		Gpu::RenderTarget::activate(probe_target.get());
		
		/* clear depth buffer before rendering */
		Gpu::State::clear(false, true, false);
		
		/* render skybox loaded by renderer */
		render_skybox_to_buffer(camera, FORWARD_RENDERING);
		
		/* render the opaque objects in scene */
		render_to_buffer(s, camera, FORWARD_RENDERING, false);
		
		/* render the transparent objects in scene */
		Gpu::State::set_depth_writemask(false);
		render_to_buffer(s, camera, FORWARD_RENDERING, true);
		Gpu::State::set_depth_writemask(true);
	}
	
	/* set back to the default render target */
	Gpu::RenderTarget::activate(nullptr);
	
	/* update probe with probe texture */
	r.load_texture(*probe_map);
}

void Renderer::update_scene(Scene& s) {
	s.update_instances();
}

void Renderer::set_material_defines(const Material& m, Defines& d) {
	/* check whether to use vertex color */
	d.set_if("USE_VERTEX_COLOR", m.vertex_color);
	
	/* check whether to use normal map */
	d.set_if("USE_NORMAL_MAP", m.normal_map != nullptr);
	
	/* check whether to use normal map in tangent space */
	d.set_if("IN_TANGENT_SPACE", m.normal_map != nullptr && m.tangent_space);
	
	/* check whether to use normal map in object space */
	d.set_if("IN_OBJECT_SPACE", m.normal_map != nullptr && !m.tangent_space);
	
	/* check whether to use displacement map */
	d.set_if("USE_DISPLACEMENT_MAP", m.displacement_map != nullptr);
	
	/* check whether to use color map */
	d.set_if("USE_COLOR_MAP", m.color_map != nullptr && !m.map_with_alpha);
	
	/* check whether to use color map with alpha channel */
	d.set_if("USE_COLOR_ALPHA_MAP", m.color_map != nullptr && m.map_with_alpha);
	
	/* check whether to use alpha map */
	d.set_if("USE_ALPHA_MAP", m.alpha_map != nullptr);
	
	/* check whether to use emissive map */
	d.set_if("USE_EMISSIVE_MAP", m.emissive_map != nullptr);
	
	/* check whether to use ambient occlusion map */
	d.set_if("USE_AO_MAP", m.ao_map != nullptr);
	
	/* check whether to use metalness map */
	d.set_if("USE_METALNESS_MAP", m.metalness_map != nullptr);
	
	/* check whether to use roughness map */
	d.set_if("USE_ROUGHNESS_MAP", m.roughness_map != nullptr);
	
	/* check whether to use specular map */
	d.set_if("USE_SPECULAR_MAP", m.specular_map != nullptr);
	
	/* check whether to use light probe */
	d.set_if("USE_LIGHT_PROBE", m.light_probe != nullptr);
	
	/* check whether to use reflection probe */
	d.set_if("USE_REFLECTION_PROBE", m.reflection_probe != nullptr);
}

void Renderer::set_scene_defines(const Scene& s, Defines& d) {
	/* set the sample numbers of shadow */
	d.set_i("SHADOW_SAMPLES", Shadow::get_samples());
	
	/* set the number of point lights */
	size_t light_count = s.get_point_light_count();
	d.set_l("NUM_POINT_LIGHT", light_count);
	
	/* set the number of spot lights */
	light_count = s.get_spot_light_count();
	d.set_l("NUM_SPOT_LIGHT", light_count);
	
	/* set the number of directional lights */
	light_count = s.get_directional_light_count();
	d.set_l("NUM_DIRECTIONAL_LIGHT", light_count);
	
	/* set the number of hemisphere lights */
	light_count = s.get_hemisphere_light_count();
	d.set_l("NUM_HEMISPHERE_LIGHT", light_count);
	
	/* set whether to use linear fog */
	d.set_if("USE_LINEAR_FOG", s.get_linear_fog() != nullptr);
	
	/* set whether to use exp square fog */
	d.set_if("USE_EXP2_FOG", s.get_exp2_fog() != nullptr);
}

void Renderer::set_tone_map_defines(int m, Defines& d) {
	/* set which to use linear tone mapping */
	if (m == LINEAR_TONE_MAP) {
		d.set("TONE_MAP", "linear_tone_map");
	}
	
	/* set which to use reinhard tone mapping */
	if (m == REINHARD_TONE_MAP) {
		d.set("TONE_MAP", "reinhard_tone_map");
	}
	
	/* set which to use optimized tone mapping */
	if (m == OPTIMIZED_TONE_MAP) {
		d.set("TONE_MAP", "optimized_tone_map");
	}
	
	/* set which to use ACES filmic tone mapping */
	if (m == ACES_FILMIC_TONE_MAP) {
		d.set("TONE_MAP", "aces_filmic_tone_map");
	}
}

void Renderer::set_light_uniforms(const Scene& s, const Gpu::Shader& shader) {
	/* determines whether to enable shadow */
	bool enable_shadow = false;
	
	/* apply point lights in lighting */
	size_t point_light_count = s.get_point_light_count();
	for (int i = 0; i < point_light_count; ++i) {
		
		/* pass the light information to shader */
		auto& light = *s.get_point_light(i);
		auto lights_i = Format::format("point_lights[{}]", i);
		Vec3 light_color = light.color * light.intensity * PI;
		shader.set_uniform_i(lights_i + ".visible", light.visible);
		shader.set_uniform_v3(lights_i + ".position", light.position);
		shader.set_uniform_v3(lights_i + ".color", light_color);
		shader.set_uniform_f(lights_i + ".distance", light.distance);
		shader.set_uniform_f(lights_i + ".decay", light.decay);
	}
	
	/* apply spot lights in lighting */
	size_t spot_light_count = s.get_spot_light_count();
	for (int i = 0; i < spot_light_count; ++i) {
		
		/* pass the light information to shader */
		auto& light = *s.get_spot_light(i);
		std::string lights_i = Format::format("spot_lights[{}]", i);
		Vec3 light_direction = -light.direction.normalize();
		Vec3 light_color = light.color * light.intensity * PI;
		float light_angle = cosf(light.angle);
		float light_penumbra = cosf(light.angle * (1 - light.penumbra));
		shader.set_uniform_i(lights_i + ".visible", light.visible);
		shader.set_uniform_v3(lights_i + ".position", light.position);
		shader.set_uniform_v3(lights_i + ".direction", light_direction);
		shader.set_uniform_v3(lights_i + ".color", light_color);
		shader.set_uniform_f(lights_i + ".distance", light.distance);
		shader.set_uniform_f(lights_i + ".decay", light.decay);
		shader.set_uniform_f(lights_i + ".angle", light_angle);
		shader.set_uniform_f(lights_i + ".penumbra", light_penumbra);
		shader.set_uniform_i(lights_i + ".cast_shadow", light.cast_shadow);
		
		/* check whether the light cast shadow */
		if (!light.cast_shadow) continue;
		enable_shadow = true;
		
		/* pass the shadow information to shader */
		auto& shadow = light.shadow;
		std::string shadows_i = Format::format("spot_lights[{}].shadow", i);
		Mat4 view_proj = shadow.camera.projection * shadow.camera.viewing;
		shader.set_uniform_i(shadows_i + ".type", shadow.type);
		shader.set_uniform_i(shadows_i + ".map_id", shadow.map_id);
		shader.set_uniform_f(shadows_i + ".bias", shadow.bias);
		shader.set_uniform_f(shadows_i + ".normal_bias", shadow.normal_bias);
		shader.set_uniform_f(shadows_i + ".radius", shadow.radius);
		shader.set_uniform_m4(shadows_i + ".view_proj", view_proj);
	}
	
	/* apply directional lights in lighting */
	size_t directional_light_count = s.get_directional_light_count();
	for (int i = 0; i < directional_light_count; ++i) {
		
		/* pass the light information to shader */
		auto& light = *s.get_directional_light(i);
		std::string lights_i = Format::format("directional_lights[{}]", i);
		Vec3 light_direction = -light.direction.normalize();
		Vec3 light_color = light.color * light.intensity * PI;
		shader.set_uniform_i(lights_i + ".visible", light.visible);
		shader.set_uniform_v3(lights_i + ".direction", light_direction);
		shader.set_uniform_v3(lights_i + ".color", light_color);
		shader.set_uniform_i(lights_i + ".cast_shadow", light.cast_shadow);
		
		/* check whether the light cast shadow */
		if (!light.cast_shadow) continue;
		enable_shadow = true;
		
		/* pass the shadow information to shader */
		auto& shadow = light.shadow;
		std::string shadows_i = Format::format("directional_lights[{}].shadow", i);
		Mat4 view_proj = shadow.camera.projection * shadow.camera.viewing;
		shader.set_uniform_i(shadows_i + ".type", shadow.type);
		shader.set_uniform_i(shadows_i + ".map_id", shadow.map_id);
		shader.set_uniform_f(shadows_i + ".bias", shadow.bias);
		shader.set_uniform_f(shadows_i + ".normal_bias", shadow.normal_bias);
		shader.set_uniform_f(shadows_i + ".radius", shadow.radius);
		shader.set_uniform_m4(shadows_i + ".proj", shadow.camera.projection);
		shader.set_uniform_m4(shadows_i + ".view_proj", view_proj);
	}
	
	/* apply hemisphere lights in lighting */
	size_t hemisphere_light_count = s.get_hemisphere_light_count();
	for (int i = 0; i < hemisphere_light_count; ++i) {
		
		/* pass the light information to shader */
		auto& light = *s.get_hemisphere_light(i);
		std::string lights_i = Format::format("hemisphere_lights[{}]", i);
		Vec3 light_sky_color = light.color * light.intensity * PI;
		Vec3 light_ground_color = light.ground_color * light.intensity * PI;
		shader.set_uniform_i(lights_i + ".visible", light.visible);
		shader.set_uniform_v3(lights_i + ".direction", light.direction);
		shader.set_uniform_v3(lights_i + ".sky_color", light_sky_color);
		shader.set_uniform_v3(lights_i + ".ground_color", light_ground_color);
	}
	
	/* pass the shadow parameters to shader */
	if (enable_shadow) Shadow::activate_texture(26);
	shader.set_uniform_i("global_shadow.map", 26);
	shader.set_uniform_v2("global_shadow.size", Shadow::get_resolution());
	
	/* pass the linear fog parameters to shader */
	auto* linear_fog = s.get_linear_fog();
	if (linear_fog != nullptr) {
		shader.set_uniform_i("fog.visible", linear_fog->visible);
		shader.set_uniform_v3("fog.color", linear_fog->color);
		shader.set_uniform_f("fog.near", linear_fog->near);
		shader.set_uniform_f("fog.far", linear_fog->far);
	}
	
	/* pass the exp square fog parameters to shader */
	auto* exp2_fog = s.get_exp2_fog();
	if (exp2_fog != nullptr) {
		shader.set_uniform_i("fog.visible", exp2_fog->visible);
		shader.set_uniform_v3("fog.color", exp2_fog->color);
		shader.set_uniform_f("fog.near", exp2_fog->near);
		shader.set_uniform_f("fog.density", exp2_fog->density);
	}
}

void Renderer::render_skybox_to_buffer(const Camera& c, int r) const {
	/* initialize cube vertex object */
	[[maybe_unused]]
	static bool inited = init_cube();
	
	/* set the depth & stencil test */
	Gpu::State::disable_depth_test();
	Gpu::State::disable_stencil_test();
	
	/* disable blending & wireframe */
	Gpu::State::disable_blending();
	Gpu::State::disable_wireframe();
	
	/* cull the front sides of faces */
	Gpu::State::enable_culling();
	Gpu::State::set_cull_side(FRONT_SIDE);
	
	/* render cube texture seamlessly */
	Gpu::State::enable_texture_cube_seamless();
	
	/* fetch the skybox shader from shader lib */
	Defines defines;
	if (r == FORWARD_RENDERING) {
		set_tone_map_defines(tone_map_mode, defines);
	} else {
		defines.set("TONE_MAP", "linear_tone_map");
	}
	int type = skybox_map->get_type();
	defines.set_if("USE_EQUIRECT", type == TEXTURE_2D);
	defines.set_if("USE_CUBEMAP", type == TEXTURE_CUBE);
	auto* shader = ShaderLib::fetch("Skybox", defines);
	
	/* calculate transform matrice */
	Mat4 viewing = c.viewing;
	for (int i = 0; i < 3; ++i) {
		viewing[i][3] = 0;
	}
	Mat4 view_proj = c.projection * viewing;
	
	/* calculate exposure in tone mapping */
	float exposure = 1;
	if (r == FORWARD_RENDERING) exposure = tone_map_exposure;
	
	/* render to the render target */
	shader->use_program();
	shader->set_uniform_m4("view_proj", view_proj);
	shader->set_uniform_f("exposure", exposure);
	shader->set_uniform_f("intensity", skybox_intensity);
	shader->set_uniform_i("map", skybox_map->activate(0));
	cube->attach(*shader);
	cube->render();
}

void Renderer::render_to_buffer(const Scene& s, const Camera& c, int r, bool t) const {
	/* create transform matrices & vectors */
	Mat4 model;
	Mat4 view = c.viewing;
	Mat4 proj = c.projection;
	Mat4 model_view;
	Mat4 model_view_proj;
	Mat3 normal_mat;
	Vec3 camera_pos = c.position;
	
	/* sort the vector of visible instances */
	auto instances = s.to_visible_instances();
	sort_instances(c, instances, t);
	
	/* render all instances in sorted vector */
	for (auto& instance : instances) {
		
		/* get matrices from instance */
		model = instance->matrix_global;
		model_view = view * model;
		model_view_proj = proj * model_view;
		normal_mat = inverse_3x3(Mat3{
			model[0][0], model[1][0], model[2][0],
			model[0][1], model[1][1], model[2][1],
			model[0][2], model[1][2], model[2][2],
		});
		
		/* get mesh from instance */
		auto* mesh = instance->mesh;
		
		/* check whether the scene is loaded */
		if (mesh_cache.count(mesh) == 0) {
			return Error::set("Renderer: Scene is not loaded");
		}
		
		/* get vertex objects from mesh cache */
		auto* vertex_object = mesh_cache.at(mesh).get();
		size_t group_size = mesh->groups.size();
		for (int i = 0; i < group_size; ++i) {
			
			/* get material from material groups */
			auto& group = mesh->groups[i];
			auto* material = s.get_material(group.name, instance);
			if (material == nullptr) {
				material = s.get_material(group.name, mesh);
			}
			if (material == nullptr) {
				material = s.get_material(group.name);
			}
			if (material == nullptr) {
				Error::set("Renderer: Material is not linked");
				continue;
			}
			
			/* check whether the material is visible */
			if (!material->visible) continue;
			
			/* check whether the material is transparent or blending */
			bool is_transparent = material->transparent || material->blending;
			if (is_transparent != t) continue;
			
			/* fetch the standard shader from shader lib */
			auto* shader = static_cast<const Gpu::Shader*>(material->shader);
			if (shader == nullptr) {
				Defines defines;
				set_material_defines(*material, defines);
				if (!t && r == DEFERRED_RENDERING) {
					/* use defines in deferred rendering */
					defines.set("DEFERRED_RENDERING");
				} else {
					/* use defines in forward rendering */
					defines.set("FORWARD_RENDERING");
					set_scene_defines(s, defines);
					set_tone_map_defines(tone_map_mode, defines);
				}
				shader = ShaderLib::fetch("Standard", defines);
			}
			
			/* render vertex object with shader */
			shader->use_program();
			vertex_object[i].attach(*shader);
			
			if (is_transparent || r == FORWARD_RENDERING) {
				/* pass camera parameters to shader */
				Mat4 inv_view_proj = inverse_4x4(c.projection * c.viewing);
				shader->set_uniform_m4("inv_view_proj", inv_view_proj);
				
				/* pass the tone mapping parameters to shader */
				shader->set_uniform_f("exposure", tone_map_exposure);
				
				/* pass the lights & fogs parameters to shader */
				set_light_uniforms(s, *shader);
			}
			
			/* pass the renderer parameters to shader */
			shader->set_uniform_m4("model"          , model          );
			shader->set_uniform_m4("view"           , view           );
			shader->set_uniform_m4("proj"           , proj           );
			shader->set_uniform_m4("model_view"     , model_view     );
			shader->set_uniform_m4("model_view_proj", model_view_proj);
			shader->set_uniform_m3("normal_mat"     , normal_mat     );
			shader->set_uniform_v3("camera_pos"     , camera_pos     );
			
			/* pass the material parameters to shader */
			shader->set_uniform_v3("color"      , material->color       );
			shader->set_uniform_f("alpha_test"  , material->alpha_test  );
			shader->set_uniform_f("alpha"       , material->alpha       );
			shader->set_uniform_f("ao_intensity", material->ao_intensity);
			shader->set_uniform_f("specular"    , material->specular    );
			shader->set_uniform_f("metalness"   , material->metalness   );
			shader->set_uniform_f("roughness"   , material->roughness   );
			
			/* pass the emissive parameter to shader */
			Vec3 emissive = material->emissive * material->emissive_intensity;
			shader->set_uniform_v3("emissive", emissive);
			
			/* pass the normal scale if use normal map */
			if (material->normal_map != nullptr) {
				shader->set_uniform_f("normal_scale", material->normal_scale);
			}
			
			/* pass the displacement scale if use displacement map */
			if (material->displacement_map != nullptr) {
				shader->set_uniform_f("displacement_scale", material->displacement_scale);
			}
			
			/* pass the images linked with material */
			for (int j = 0; j < 16; ++j) {
				auto* image = material->get_image(j);
				if (image != nullptr) image_cache.at(image)->activate(j);
			}
			if (material->normal_map != nullptr) {
				auto& map = image_cache.at(material->normal_map);
				shader->set_uniform_i("normal_map", map->activate(16));
			}
			if (material->displacement_map != nullptr) {
				auto& map = image_cache.at(material->displacement_map);
				shader->set_uniform_i("displacement_map", map->activate(17));
			}
			if (material->color_map != nullptr) {
				auto& map = image_cache.at(material->color_map);
				shader->set_uniform_i("color_map", map->activate(18));
			}
			if (material->alpha_map != nullptr) {
				auto& map = image_cache.at(material->alpha_map);
				shader->set_uniform_i("alpha_map", map->activate(19));
			}
			if (material->emissive_map != nullptr) {
				auto& map = image_cache.at(material->emissive_map);
				shader->set_uniform_i("emissive_map", map->activate(20));
			}
			if (material->ao_map != nullptr) {
				auto& map = image_cache.at(material->ao_map);
				shader->set_uniform_i("ao_map", map->activate(21));
			}
			if (material->roughness_map != nullptr) {
				auto& map = image_cache.at(material->roughness_map);
				shader->set_uniform_i("roughness_map", map->activate(22));
			}
			if (material->metalness_map != nullptr) {
				auto& map = image_cache.at(material->metalness_map);
				shader->set_uniform_i("metalness_map", map->activate(23));
			}
			if (material->specular_map != nullptr) {
				auto& map = image_cache.at(material->specular_map);
				shader->set_uniform_i("specular_map", map->activate(24));
			}
			
			/* pass the reflection probe linked with material */
			auto* ref_probe = static_cast<const ReflectionProbe*>(material->reflection_probe);
			if (ref_probe != nullptr) {
				int ref_lod = log2f(ref_probe->resolution);
				shader->set_uniform_i("ref_map", ref_probe->activate(25));
				shader->set_uniform_f("ref_lod", ref_lod);
				shader->set_uniform_f("ref_intensity", ref_probe->intensity);
			}
			
			/* pass the custom uniforms linked with material */
			if (material->uniforms != nullptr) {
				shader->set_uniforms(*material->uniforms);
			}
			
			/* apply the depth test setting for material */
			Gpu::MaterialState::set_depth(*material);
			
			/* apply the stencil test setting for material */
			Gpu::MaterialState::set_stencil(*material);
			
			/* apply the wireframe setting for material */
			Gpu::MaterialState::set_wireframe(*material);
			
			/* apply the blending setting for material */
			Gpu::MaterialState::set_blending(*material);
			
			/* apply the render side setting for material */
			Gpu::MaterialState::set_side(*material);
			
			/* set the blendings to render transparent objects correctly */
			if (material->transparent) {
				Gpu::State::enable_blending();
				Gpu::State::set_blend_op(BLEND_ADD);
				Gpu::State::set_blend_factor(FACTOR_SRC_ALPHA, FACTOR_ONE_MINUS_SRC_ALPHA);
			}
			
			/* cull back side of face if side is DOUBLE_SIDE */
			if (is_transparent && material->side == DOUBLE_SIDE) {
				Gpu::State::enable_culling();
				Gpu::State::set_cull_side(BACK_SIDE);
			}
			
			/* render the vertex object at group i */
			vertex_object[i].render();
		}
	}
}

void Renderer::render_to_shadow(const Scene& s, const Camera& c) const {
	/* create transform matrices & vectors */
	Mat4 model;
	Mat4 view = c.viewing;
	Mat4 proj = c.projection;
	Mat4 model_view;
	Mat4 model_view_proj;
	
	/* sort the vector of visible instances */
	auto instances = s.to_visible_instances();
	sort_instances(c, instances, false);
	
	/* render all instances in sorted vector */
	for (auto& instance : instances) {
		
		/* check whether the instance casts shadow */
		if (!instance->cast_shadow) continue;
		
		/* get matrices from instance */
		model = instance->matrix_global;
		model_view = view * model;
		model_view_proj = proj * model_view;
		
		/* get mesh from instance */
		auto* mesh = instance->mesh;
		
		/* check whether the scene is loaded */
		if (mesh_cache.count(mesh) == 0) {
			Error::set("Renderer: Scene is not loaded");
			continue;
		}
		
		/* get vertex objects from cache */
		auto* vertex_object = mesh_cache.at(mesh).get();
		size_t group_size = mesh->groups.size();
		for (int i = 0; i < group_size; ++i) {
			
			/* get material from material groups */
			auto& group = mesh->groups[i];
			auto* material = s.get_material(group.name, mesh);
			if (material == nullptr) {
				material = s.get_material(group.name);
			}
			if (material == nullptr) {
				Error::set("Renderer: Material is not linked");
				continue;
			}
			
			/* whether to use color map and alpha map */
			bool use_color_map = material->color_map != nullptr && material->map_with_alpha;
			bool use_alpha_map = material->alpha_map != nullptr;
			
			/* fetch the shadow shader from shader lib */
			Defines defines;
			defines.set_if("USE_COLOR_MAP", use_color_map);
			defines.set_if("USE_ALPHA_MAP", use_alpha_map);
			auto* shader = ShaderLib::fetch("Shadow", defines);
			
			/* check whether the material is visible */
			if (!material->visible) continue;
			
			/* check whether the material is transparent or using blending */
			if (material->transparent || material->blending) continue;
			
			/* render vertex object with shader */
			shader->use_program();
			vertex_object[i].attach(*shader);
			
			/* pass the renderer parameters to shader */
			shader->set_uniform_m4("model_view_proj", model_view_proj);
			
			/* pass the material parameters to shader */
			shader->set_uniform_f("alpha", material->alpha);
			shader->set_uniform_f("alpha_test", material->alpha_test);
			shader->set_uniform_i("color_map", 0);
			shader->set_uniform_i("alpha_map", 1);
			
			/* activate color map linked with material */
			if (use_color_map) {
				image_cache.at(material->color_map)->activate(0);
			}
			if (use_alpha_map) {
				image_cache.at(material->alpha_map)->activate(1);
			}
			
			/* apply the depth test setting for material */
			Gpu::MaterialState::set_depth(*material);
			
			/* apply the stencil test setting for material */
			Gpu::MaterialState::set_stencil(*material);
			
			/* apply the wireframe setting for material */
			Gpu::MaterialState::set_wireframe(*material);
			
			/* apply the blending setting for material */
			Gpu::MaterialState::set_blending(*material);
			
			/* apply the render side setting for material */
			Gpu::MaterialState::set_shadow_side(*material);
			
			/* render the vertex object for group i */
			vertex_object[i].render();
		}
	}
}

bool Renderer::init_cube() {
	cube = std::make_unique<Gpu::VertexObject>();
	Mesh box = BoxMesh::create();
	cube->load(box, box.groups[0]);
	return true; /* finish */
}

void Renderer::sort_instances(const Camera& c, std::vector<const Instance*>& l, bool t) {
	using InstanceItem = std::pair<const Instance*, float>;
	std::vector<InstanceItem> sorted_instances;
	auto instances_size = l.size();
	sorted_instances.resize(instances_size);
	for (int i = 0; i < instances_size; ++i) {
		Vec3 position = l[i]->local_to_global({});
		sorted_instances[i].first = l[i];
		sorted_instances[i].second = (c.position - position).dot(c.direction);
	}
	auto compare = t ?
	[](const InstanceItem& a, const InstanceItem& b) -> bool {
		if (a.first->priority != b.first->priority) {
			return a.first->priority < b.first->priority;
		}
		return a.second > b.second;
	} :
	[](const InstanceItem& a, const InstanceItem& b) -> bool {
		if (a.first->priority != b.first->priority) {
			return a.first->priority < b.first->priority;
		}
		return a.second < b.second;
	};
	std::sort(sorted_instances.begin(), sorted_instances.end(), compare);
	for (int i = 0; i < instances_size; ++i) {
		l[i] = sorted_instances[i].first;
	}
}

std::unique_ptr<Gpu::VertexObject> Renderer::cube;

std::unique_ptr<Gpu::Texture> Renderer::probe_map;

std::unique_ptr<Gpu::RenderBuffer> Renderer::probe_buffer;

std::unique_ptr<Gpu::RenderTarget> Renderer::probe_target;

}
