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

#include "Renderer.h"

#include "../core/Error.h"
#include "../core/Format.h"
#include "../meshes/BoxMesh.h"

namespace Ink {

Vec4 Renderer::get_clear_color() const {
	return clear_color;
}

void Renderer::set_clear_color(const Vec4& c) {
	clear_color = c;
}

void Renderer::clear(bool c, bool d, bool s) const {
	Gpu::FrameBuffer::activate(target);
	Gpu::set_clear_color(clear_color);
	Gpu::clear(c, d, s);
	Gpu::FrameBuffer::activate(nullptr);
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

int Renderer::get_tone_mapping_mode() const {
	return tone_mapping_mode;
}

float Renderer::get_tone_mapping_exposure() const {
	return tone_mapping_exposure;
}

void Renderer::set_tone_mapping(int m, float e) {
	tone_mapping_mode = m;
	tone_mapping_exposure = e;
}

const Gpu::FrameBuffer* Renderer::get_target() const {
	return target;
}

void Renderer::set_target(const Gpu::FrameBuffer *t) {
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

void Renderer::load_skybox_cubemap(const Image& px, const Image& nx, const Image& py,
								   const Image& ny, const Image& pz, const Image& nz) {
	skybox_map = std::make_unique<Gpu::Texture>();
	skybox_map->init_cube(px, nx, py, ny, pz, nz, Gpu::Texture::default_format(px));
	skybox_map->set_filters(TEXTURE_LINEAR, TEXTURE_LINEAR);
}

void Renderer::load_skybox_equirect(const Image& i) {
	skybox_map = std::make_unique<Gpu::Texture>();
	skybox_map->init_2d(i, Gpu::Texture::default_format(i));
	skybox_map->set_filters(TEXTURE_LINEAR, TEXTURE_LINEAR);
}

void Renderer::render_skybox(const Camera& c) const {
	/* initialize skybox */
	[[maybe_unused]]
	static bool inited = init_skybox();
	
	/* activate the render target */
	Gpu::FrameBuffer::activate(target);
	
	/* set the depth & stencil test */
	Gpu::enable_depth_test();
	Gpu::set_depth_func(FUNC_ALWAYS);
	Gpu::disable_stencil_test();
	
	/* disable blending & wireframe */
	Gpu::disable_blending();
	Gpu::disable_wireframe();
	
	/* cull the front sides of faces */
	Gpu::enable_culling();
	Gpu::set_cull_side(FRONT_SIDE);
	
	/* render cube texture seamlessly */
	Gpu::enable_texture_cube_seamless();
	
	/* set the viewport region */
	Gpu::set_viewport(viewport);
	
	/* set the scissor test and region */
	if (scissor_test) {
		Gpu::enable_scissor_test();
		Gpu::set_scissor(scissor);
	} else {
		Gpu::disable_scissor_test();
	}
	
	/* fetch the skybox shader from cache */
	Defines defines = get_tone_map_defines(tone_mapping_mode);
	int type = skybox_map->get_type();
	defines.set_if("USE_EQUIRECT", type == TEXTURE_2D);
	defines.set_if("USE_CUBEMAP", type == TEXTURE_CUBE);
	auto* shader_ptr = fetch_shader(defines, "Skybox");
	
	/* calculate transform matrice */
	Mat4 viewing = c.viewing;
	for (int i = 0; i < 3; ++i) {
		viewing[i][3] = 0;
	}
	Mat4 view_proj = c.projection * viewing;
	
	/* render to the render target */
	shader_ptr->use_program();
	shader_ptr->set_uniform_m4("view_proj", view_proj);
	shader_ptr->set_uniform_f("exposure", tone_mapping_exposure);
	shader_ptr->set_uniform_f("intensity", skybox_intensity);
	shader_ptr->set_uniform_i("map", skybox_map->activate(0));
	cube->attach(*shader_ptr);
	cube->draw();
	
	/* set back to the default render target */
	Gpu::FrameBuffer::activate(nullptr);
}

void Renderer::load_mesh(const Mesh* m) {
	if (cache_mesh.count(m) != 0) return;
	size_t size = m->groups.size();
	auto p = cache_mesh.insert({m, std::make_unique<Gpu::VertexObject[]>(size)});
	auto* vertex_object_ptr = p.first->second.get();
	for (int i = 0; i < size; ++i) {
		vertex_object_ptr[i].load(*m, i);
	}
}

void Renderer::load_image(const Image* i) {
	if (cache_image.count(i) != 0) return;
	auto p = cache_image.insert({i, std::make_unique<Gpu::Texture>()});
	auto* texture_ptr = p.first->second.get();
	texture_ptr->init_2d(*i, Gpu::Texture::default_format(*i));
	std::invoke(texture_callback, *texture_ptr);
}

void Renderer::load_scene(const Scene& s) {
	/* load the meshes linked with instance */
	for (auto& instance_ptr : s.to_instances()) {
		auto* mesh_ptr = instance_ptr->mesh;
		if (mesh_ptr != nullptr) load_mesh(mesh_ptr);
	}
	
	/* load the images linked with instance */
	for (auto& material_ptr : s.get_materials()) {
		auto count = material_ptr->get_image_count();
		for (int i = 0; i < count; ++i) {
			load_image(material_ptr->get_image(i));
		}
		if (material_ptr->normal_map != nullptr) {
			load_image(material_ptr->normal_map);
		}
		if (material_ptr->displacement_map != nullptr) {
			load_image(material_ptr->displacement_map);
		}
		if (material_ptr->color_map != nullptr) {
			load_image(material_ptr->color_map);
		}
		if (material_ptr->alpha_map != nullptr) {
			load_image(material_ptr->alpha_map);
		}
		if (material_ptr->emissive_map != nullptr) {
			load_image(material_ptr->emissive_map);
		}
		if (material_ptr->ao_map != nullptr) {
			load_image(material_ptr->ao_map);
		}
		if (material_ptr->roughness_map != nullptr) {
			load_image(material_ptr->roughness_map);
		}
		if (material_ptr->metalness_map != nullptr) {
			load_image(material_ptr->metalness_map);
		}
		if (material_ptr->specular_map != nullptr) {
			load_image(material_ptr->specular_map);
		}
	}
}

void Renderer::clear_scene_cache() {
	cache_mesh.clear();
	cache_image.clear();
}

void Renderer::render(const Scene& s, const Camera& c) const {
	/* activate the render target */
	Gpu::FrameBuffer::activate(target);
	
	/* set the viewport region */
	Gpu::set_viewport(viewport);
	
	/* set the scissor test and region */
	if (scissor_test) {
		Gpu::enable_scissor_test();
		Gpu::set_scissor(scissor);
	} else {
		Gpu::disable_scissor_test();
	}
	
	/* render the scene to buffer */
	render_to_buffer(s, c, {false, rendering_mode, tone_mapping_mode});
	
	/* set back to the default render target */
	Gpu::FrameBuffer::activate(nullptr);
}

void Renderer::render_transparent(const Scene& s, const Camera& c) const {
	/* activate the render target */
	Gpu::FrameBuffer::activate(target);
	
	/* set the viewport region */
	Gpu::set_viewport(viewport);
	
	/* set the scissor test and region */
	if (scissor_test) {
		Gpu::enable_scissor_test();
		Gpu::set_scissor(scissor);
	} else {
		Gpu::disable_scissor_test();
	}
	
	/* render the scene to buffer */
	if (rendering_mode == FORWARD_RENDERING) {
		render_to_buffer(s, c, {true, rendering_mode, tone_mapping_mode});
	} else {
		Gpu::set_depth_writemask(false);
		render_to_buffer(s, c, {true, rendering_mode, tone_mapping_mode});
		Gpu::set_depth_writemask(true);
	}
	
	/* set back to the default render target */
	Gpu::FrameBuffer::activate(nullptr);
}

void Renderer::render_shadow(const Scene& s, const Shadow& t) const {
	/* activate the render target */
	Gpu::FrameBuffer::activate(t.get_target());
	
	/* set the viewport to fit the resolution of shadow map */
	Vec2 resolution = Shadow::get_resolution();
	Gpu::set_viewport(Gpu::Rect(resolution.x, resolution.y));
	
	/* disable the scissor test */
	Gpu::disable_scissor_test();
	
	/* clear the shadow map (depth only) */
	Gpu::clear(false, true, false);
	
	/* render the scene to shadow */
	render_to_shadow(s, t.camera);
	
	/* set back to the default render target */
	Gpu::FrameBuffer::activate(nullptr);
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

void Renderer::update_scene(Scene& s) {
	s.update_instances();
}

void Renderer::clear_shader_cache() {
	cache_shader.clear();
}

Defines Renderer::get_material_defines(const Material& m) {
	Defines defines; /* create new defines */
	
	/* check whether to use normal map */
	defines.set_if("USE_NORMAL_MAP", m.normal_map != nullptr);
	
	/* check whether to use tangent space */
	defines.set_if("IN_TANGENT_SPACE", m.normal_map != nullptr && m.tangent_space);
	
	/* check whether to use object space */
	defines.set_if("IN_OBJECT_SPACE", m.normal_map != nullptr && !m.tangent_space);
	
	/* check whether to use displacement map */
	defines.set_if("USE_DISPLACEMENT_MAP", m.displacement_map != nullptr);
	
	/* check whether to use color map */
	defines.set_if("USE_COLOR_MAP", m.color_map != nullptr);
	
	/* check whether to use alpha map */
	defines.set_if("USE_ALPHA_MAP", m.alpha_map != nullptr);
	
	/* check whether to use alpha from color map */
	defines.set_if("USE_COLOR_WITH_ALPHA", m.color_with_alpha);
	
	/* check whether to use emissive map */
	defines.set_if("USE_EMISSIVE_MAP", m.emissive_map != nullptr);
	
	/* check whether to use ambient occlusion map */
	defines.set_if("USE_AO_MAP", m.ao_map != nullptr);
	
	/* check whether to use metalness map */
	defines.set_if("USE_METALNESS_MAP", m.metalness_map != nullptr);
	
	/* check whether to use roughness map */
	defines.set_if("USE_ROUGHNESS_MAP", m.roughness_map != nullptr);
	
	/* check whether to use specular map */
	defines.set_if("USE_SPECULAR_MAP", m.specular_map != nullptr);
	
	/* check whether to use light probe */
	defines.set_if("USE_LIGHT_PROBE", m.light_probe != nullptr);
	
	/* check whether to use reflection probe */
	defines.set_if("USE_REFLECTION_PROBE", m.reflection_probe != nullptr);
	
	return defines; /* return the defines */
}

Defines Renderer::get_scene_defines(const Scene& s) {
	Defines defines; /* create new defines */
	
	/* set the sample numbers of shadow */
	defines.set_i("SHADOW_SAMPLES", Shadow::get_samples());
	
	/* set the number of point lights */
	size_t light_count = s.get_point_light_count();
	defines.set_l("NUM_POINT_LIGHT", light_count);
	
	/* set the number of spot lights */
	light_count = s.get_spot_light_count();
	defines.set_l("NUM_SPOT_LIGHT", light_count);
	
	/* set the number of directional lights */
	light_count = s.get_directional_light_count();
	defines.set_l("NUM_DIRECTIONAL_LIGHT", light_count);
	
	/* set the number of hemisphere lights */
	light_count = s.get_hemisphere_light_count();
	defines.set_l("NUM_HEMISPHERE_LIGHT", light_count);
	
	/* set whether to use linear fog */
	defines.set_if("USE_LINEAR_FOG", s.get_linear_fog() != nullptr);
	
	/* set whether to use exp square fog */
	defines.set_if("USE_EXP2_FOG", s.get_exp2_fog() != nullptr);
	
	return defines; /* return the defines */
}

Defines Renderer::get_tone_map_defines(int m) {
	Defines defines; /* create new defines */
	
	/* set which mode to use in tone mapping */
	if (m == LINEAR_TONE_MAP) {
		defines.set("TONE_MAP", "linear_tone_map");
	}
	if (m == REINHARD_TONE_MAP) {
		defines.set("TONE_MAP", "reinhard_tone_map");
	}
	if (m == OPTIMIZED_TONE_MAP) {
		defines.set("TONE_MAP", "optimized_tone_map");
	}
	if (m == ACES_FILMIC_TONE_MAP) {
		defines.set("TONE_MAP", "aces_filmic_tone_map");
	}
	
	return defines; /* return the defines */
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
		shader.set_uniform_i(shadows_i + ".map_id", shadow.get_unique_id());
		shader.set_uniform_f(shadows_i + ".bias", shadow.bias);
		shader.set_uniform_f(shadows_i + ".normal_bias", shadow.normal_bias);
		shader.set_uniform_f(shadows_i + ".radius", shadow.radius);
		shader.set_uniform_m4(shadows_i + ".proj", shadow.camera.projection);
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
		shader.set_uniform_i(shadows_i + ".map_id", shadow.get_unique_id());
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
		shader.set_uniform_i("linear_fog.visible", linear_fog->visible);
		shader.set_uniform_v3("linear_fog.color", linear_fog->color);
		shader.set_uniform_f("linear_fog.near", linear_fog->near);
		shader.set_uniform_f("linear_fog.far", linear_fog->far);
	}
	
	/* pass the exp square fog parameters to shader */
	auto* exp2_fog = s.get_exp2_fog();
	if (exp2_fog != nullptr) {
		shader.set_uniform_i("exp2_fog.visible", exp2_fog->visible);
		shader.set_uniform_v3("exp2_fog.color", exp2_fog->color);
		shader.set_uniform_f("exp2_fog.near", exp2_fog->near);
		shader.set_uniform_f("exp2_fog.density", exp2_fog->density);
	}
}

void Renderer::render_to_buffer(const Scene& s, const Camera& c, const RenderInfo& t) const {
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
	sort_instances(c, instances, t.transparent);
	
	/* render all instances in sorted vector */
	for (auto& instance_ptr : instances) {
		
		/* get matrices from instance */
		model = instance_ptr->matrix_global;
		model_view = view * model;
		model_view_proj = proj * model_view;
		normal_mat = inverse_3x3(Mat3{
			model[0][0], model[1][0], model[2][0],
			model[0][1], model[1][1], model[2][1],
			model[0][2], model[1][2], model[2][2],
		});
		
		/* get mesh from instance */
		auto* mesh_ptr = instance_ptr->mesh;
		
		/* check whether the scene is loaded */
		if (cache_mesh.count(mesh_ptr) == 0) {
			return Error::set("Renderer: Scene is not loaded");
		}
		
		/* get vertex objects from mesh cache */
		auto* vertex_object_ptr = cache_mesh.at(mesh_ptr).get();
		size_t group_size = mesh_ptr->groups.size();
		for (int i = 0; i < group_size; ++i) {
			
			/* get material from material groups */
			auto& group = mesh_ptr->groups[i];
			auto* material_ptr = s.get_material(mesh_ptr, group.name);
			if (material_ptr == nullptr) {
				material_ptr = s.get_material(group.name);
			}
			if (material_ptr == nullptr) {
				Error::set("Renderer: Material is not linked");
				continue;
			}
			
			/* check whether the material is visible */
			if (!material_ptr->visible) continue;
			
			/* check whether the material is transparent or using blending */
			bool is_transparent = material_ptr->transparent || material_ptr->blending;
			if (is_transparent != t.transparent) continue;
			
			/* fetch the standard shader from cache */
			auto* shader_ptr = static_cast<const Gpu::Shader*>(material_ptr->shader);
			if (shader_ptr == nullptr) {
				shader_ptr = fetch_shader(fetch_defines(s, *material_ptr, t), "Standard");
			}
			
			/* render vertex object with shader */
			shader_ptr->use_program();
			vertex_object_ptr[i].attach(*shader_ptr);
			
			if (is_transparent || t.rendering_mode == FORWARD_RENDERING) {
				/* pass camera parameters to shader */
				Mat4 inv_view_proj = inverse_4x4(c.projection * c.viewing);
				shader_ptr->set_uniform_m4("inv_view_proj", inv_view_proj);
				
				/* pass the tone mapping parameters to shader */
				shader_ptr->set_uniform_f("exposure", tone_mapping_exposure);
				
				/* pass the lights & fogs parameters to shader */
				set_light_uniforms(s, *shader_ptr);
			}
			
			/* pass the renderer parameters to shader */
			shader_ptr->set_uniform_m4("model"          , model          );
			shader_ptr->set_uniform_m4("view"           , view           );
			shader_ptr->set_uniform_m4("proj"           , proj           );
			shader_ptr->set_uniform_m4("model_view"     , model_view     );
			shader_ptr->set_uniform_m4("model_view_proj", model_view_proj);
			shader_ptr->set_uniform_m3("normal_mat"     , normal_mat     );
			shader_ptr->set_uniform_v3("camera_pos"     , camera_pos     );
			
			/* pass the material parameters to shader */
			shader_ptr->set_uniform_v3("color"      , material_ptr->color       );
			shader_ptr->set_uniform_f("alpha_test"  , material_ptr->alpha_test  );
			shader_ptr->set_uniform_f("alpha"       , material_ptr->alpha       );
			shader_ptr->set_uniform_f("ao_intensity", material_ptr->ao_intensity);
			shader_ptr->set_uniform_f("specular"    , material_ptr->specular    );
			shader_ptr->set_uniform_f("metalness"   , material_ptr->metalness   );
			shader_ptr->set_uniform_f("roughness"   , material_ptr->roughness   );
			
			/* pass the emissive parameter to shader */
			Vec3 emissive = material_ptr->emissive * material_ptr->emissive_intensity;
			shader_ptr->set_uniform_v3("emissive", emissive);
			
			/* pass the normal scale if use normal map */
			if (material_ptr->normal_map != nullptr) {
				shader_ptr->set_uniform_f("normal_scale", material_ptr->normal_scale);
			}
			
			/* pass the displacement scale if use displacement map */
			if (material_ptr->displacement_map != nullptr) {
				shader_ptr->set_uniform_f("displacement_scale", material_ptr->displacement_scale);
			}
			
			/* pass the images linked with material */
			size_t image_count = material_ptr->get_image_count();
			for (int j = 0; j < image_count; ++j) {
				cache_image.at(material_ptr->get_image(j))->activate(j);
			}
			if (material_ptr->normal_map != nullptr) {
				auto& map = cache_image.at(material_ptr->normal_map);
				shader_ptr->set_uniform_i("normal_map", map->activate(16));
			}
			if (material_ptr->displacement_map != nullptr) {
				auto& map = cache_image.at(material_ptr->displacement_map);
				shader_ptr->set_uniform_i("displacement_map", map->activate(17));
			}
			if (material_ptr->color_map != nullptr) {
				auto& map = cache_image.at(material_ptr->color_map);
				shader_ptr->set_uniform_i("color_map", map->activate(18));
			}
			if (material_ptr->alpha_map != nullptr) {
				auto& map = cache_image.at(material_ptr->alpha_map);
				shader_ptr->set_uniform_i("alpha_map", map->activate(19));
			}
			if (material_ptr->emissive_map != nullptr) {
				auto& map = cache_image.at(material_ptr->emissive_map);
				shader_ptr->set_uniform_i("emissive_map", map->activate(20));
			}
			if (material_ptr->ao_map != nullptr) {
				auto& map = cache_image.at(material_ptr->ao_map);
				shader_ptr->set_uniform_i("ao_map", map->activate(21));
			}
			if (material_ptr->roughness_map != nullptr) {
				auto& map = cache_image.at(material_ptr->roughness_map);
				shader_ptr->set_uniform_i("roughness_map", map->activate(22));
			}
			if (material_ptr->metalness_map != nullptr) {
				auto& map = cache_image.at(material_ptr->metalness_map);
				shader_ptr->set_uniform_i("metalness_map", map->activate(23));
			}
			if (material_ptr->specular_map != nullptr) {
				auto& map = cache_image.at(material_ptr->specular_map);
				shader_ptr->set_uniform_i("specular_map", map->activate(24));
			}
			
			/* pass the reflection probe linked with material */
			auto* ref_probe_ptr = static_cast<const ReflectionProbe*>(material_ptr->reflection_probe);
			if (ref_probe_ptr != nullptr) {
				int ref_lod = log2f(ref_probe_ptr->resolution);
				shader_ptr->set_uniform_i("ref_map", ref_probe_ptr->activate(25));
				shader_ptr->set_uniform_i("ref_lod", ref_lod);
				shader_ptr->set_uniform_f("ref_intensity", ref_probe_ptr->intensity);
			}
			
			/* apply the depth test setting of material */
			Gpu::apply_material_depth(*material_ptr);
			
			/* apply the stencil test setting of material */
			Gpu::apply_material_stencil(*material_ptr);
			
			/* apply the wireframe setting of material */
			Gpu::apply_material_wireframe(*material_ptr);
			
			/* apply the blending setting of material */
			Gpu::apply_material_blending(*material_ptr);
			
			/* apply the render side setting of material */
			Gpu::apply_material_side(*material_ptr);
			
			/* set the blendings to render transparent objects correctly */
			if (material_ptr->transparent) {
				Gpu::enable_blending();
				Gpu::set_blend_op(BLEND_ADD);
				Gpu::set_blend_factor(FACTOR_SRC_ALPHA, FACTOR_ONE_MINUS_SRC_ALPHA);
			}
			
			/* cull back side of face if side is DOUBLE_SIDE */
			if (is_transparent && material_ptr->side == DOUBLE_SIDE) {
				Gpu::enable_culling();
				Gpu::set_cull_side(BACK_SIDE);
			}
			
			/* draw the vertex object at group i */
			vertex_object_ptr[i].draw();
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
	for (auto& instance_ptr : instances) {
		
		/* check whether the instance casts shadow */
		if (!instance_ptr->cast_shadow) continue;
		
		/* get matrices from instance */
		model = instance_ptr->matrix_global;
		model_view = view * model;
		model_view_proj = proj * model_view;
		
		/* get mesh from instance */
		auto* mesh_ptr = instance_ptr->mesh;
		
		/* check whether the scene is loaded */
		if (cache_mesh.count(mesh_ptr) == 0) {
			Error::set("Renderer: Scene is not loaded");
			continue;
		}
		
		/* get vertex objects from cache */
		auto* vertex_object_ptr = cache_mesh.at(mesh_ptr).get();
		size_t group_size = mesh_ptr->groups.size();
		for (int i = 0; i < group_size; ++i) {
			
			/* get material from material groups */
			auto& group = mesh_ptr->groups[i];
			auto* material_ptr = s.get_material(mesh_ptr, group.name);
			if (material_ptr == nullptr) {
				material_ptr = s.get_material(group.name);
			}
			if (material_ptr == nullptr) {
				Error::set("Renderer: Material is not linked");
				continue;
			}
			
			/* whether to use color map and alpha map */
			bool use_color_map = material_ptr->color_map != nullptr;
			bool use_alpha_map = material_ptr->alpha_map != nullptr;
			
			/* fetch the shadow shader from cache */
			Defines defines;
			defines.set_if("USE_COLOR_MAP", use_color_map);
			defines.set_if("USE_ALPHA_MAP", use_alpha_map);
			auto* shader_ptr = fetch_shader(defines, "Shadow");
			
			/* check whether the material is visible */
			if (!material_ptr->visible) continue;
			
			/* check whether the material is transparent or using blending */
			if (material_ptr->transparent || material_ptr->blending) continue;
			
			/* render vertex object with shader */
			shader_ptr->use_program();
			vertex_object_ptr[i].attach(*shader_ptr);
			
			/* pass the renderer parameters to shader */
			shader_ptr->set_uniform_m4("model_view_proj", model_view_proj);
			
			/* pass the material parameters to shader */
			shader_ptr->set_uniform_f("alpha", material_ptr->alpha);
			shader_ptr->set_uniform_f("alpha_test", material_ptr->alpha_test);
			shader_ptr->set_uniform_i("color_map", 0);
			shader_ptr->set_uniform_i("alpha_map", 1);
			
			/* activate color map linked with material */
			if (use_color_map) {
				cache_image.at(material_ptr->color_map)->activate(0);
			}
			if (use_alpha_map) {
				cache_image.at(material_ptr->alpha_map)->activate(1);
			}
			
			/* apply the depth test setting of material */
			Gpu::apply_material_depth(*material_ptr);
			
			/* apply the stencil test setting of material */
			Gpu::apply_material_stencil(*material_ptr);
			
			/* apply the wireframe setting of material */
			Gpu::apply_material_wireframe(*material_ptr);
			
			/* apply the blending setting of material */
			Gpu::apply_material_blending(*material_ptr);
			
			/* apply the render side setting of material */
			Gpu::apply_material_shadow_side(*material_ptr);
			
			/* draw the vertex object at group i */
			vertex_object_ptr[i].draw();
		}
	}
}

bool Renderer::init_skybox() {
	cube = std::make_unique<Gpu::VertexObject>();
	cube->load(BoxMesh::create());
	return true;
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

Defines Renderer::fetch_defines(const Scene& s, const Material& m, const RenderInfo& t) {
	Defines defines = get_material_defines(m);
	if (!t.transparent && t.rendering_mode == DEFERRED_RENDERING) {
		/* use deferred rendering */
		defines.set("DEFERRED_RENDERING");
	} else {
		/* use forward rendering */
		defines.set("FORWARD_RENDERING");
		defines.set(get_scene_defines(s));
		defines.set(get_tone_map_defines(t.tone_mapping_mode));
	}
	return defines;
}

const Gpu::Shader* Renderer::fetch_shader(const Defines& d, const std::string& n) {
	/* set the define string as key */
	std::string key = n + "\n" + d.get();
	
	/* check whether exists the key */
	if (cache_shader.count(key) != 0) return cache_shader.at(key).get();
	
	/* insert key and shader to the cache */
	auto p = cache_shader.insert({key, std::make_unique<Gpu::Shader>()});
	auto* shader_ptr = p.first->second.get();
	
	/* load shader files to shader */
	std::string shader_file = "ink/shaders/lib/" + n;
	shader_ptr->load_vert_file(shader_file + ".vert.glsl");
	shader_ptr->load_frag_file(shader_file + ".frag.glsl");
	
	/* set defines to shader and compile shader */
	shader_ptr->set_defines(d);
	shader_ptr->compile();
	
	return shader_ptr; /* return the shader */
}

std::unique_ptr<Gpu::VertexObject> Renderer::cube;

std::unordered_map<std::string, std::unique_ptr<Gpu::Shader> > Renderer::cache_shader;

}
