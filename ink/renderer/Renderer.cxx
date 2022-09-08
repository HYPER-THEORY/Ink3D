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

void Renderer::set_location_c(unsigned int l) {
	location_c = l;
}

unsigned int Renderer::get_location_c() const {
	return location_c;
}

void Renderer::set_location_n(unsigned int l) {
	location_n = l;
}

unsigned int Renderer::get_location_n() const {
	return location_n;
}

void Renderer::set_location_m(unsigned int l) {
	location_m = l;
}

unsigned int Renderer::get_location_m() const {
	return location_m;
}

void Renderer::set_location_e(unsigned int l) {
	location_e = l;
}

unsigned int Renderer::get_location_e() const {
	return location_e;
}

void Renderer::set_location_i(unsigned int l) {
	location_i = l;
}

unsigned int Renderer::get_location_i() const {
	return location_i;
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

void Renderer::set_skybox_intensity(float i) {
	skybox_intensity = i;
}

void Renderer::load_skybox_cubemap(const Image* i) {
	skybox_map = std::make_unique<Gpu::Texture>();
	skybox_map->init_cube(i, Gpu::Texture::default_format(i[0]));
	skybox_map->set_filters(TEXTURE_LINEAR, TEXTURE_NEAREST);
}

void Renderer::load_skybox_equirect(const Image& i) {
	skybox_map = std::make_unique<Gpu::Texture>();
	skybox_map->init_2d(i, Gpu::Texture::default_format(i));
	skybox_map->set_filters(TEXTURE_LINEAR, TEXTURE_NEAREST);
}

void Renderer::render_skybox(const Camera& c) {
	/* initialize skybox */
	[[maybe_unused]]
	static bool inited = init_skybox();
	
	/* activate the render target */
	Gpu::FrameBuffer::activate(target);
	
	/* set the depth & stencil test */
	Gpu::enable_depth_test();
	Gpu::set_depth_func(FUNC_ALWAYS);
	Gpu::disable_stencil_test();
	
	/* disable wireframe mode */
	Gpu::disable_wireframe();
	
	/* cull the front sides of faces */
	Gpu::enable_cull_face();
	Gpu::set_cull_face(FRONT_SIDE);
	
	/* render cube texture seamlessly */
	Gpu::enable_texture_cube_seamless();
	
	/* set the viewport */
	Gpu::set_viewport(viewport);
	
	/* set the scissor test */
	if (scissor_test) {
		Gpu::enable_scissor_test();
		Gpu::set_scissor(scissor);
	} else {
		Gpu::disable_scissor_test();
	}
	
	/* compile skybox shader */
	if (skybox_map->get_type() == TEXTURE_2D) {
		skybox_shader->set_define({{"USE_EQUIRECT", ""}});
	} else if (skybox_map->get_type() == TEXTURE_CUBE) {
		skybox_shader->set_define({{"USE_CUBEMAP", ""}});
	} else {
		set_error("Renderer: Illegal skybox map type");
	}
	skybox_shader->compile();
	
	/* render to target frame buffer */
	skybox_shader->use_program();
	Mat4 viewing = c.viewing;
	for (int i = 0; i < 3; ++i) {
		viewing[i][3] = 0;
	}
	Mat4 view_proj = c.projection * viewing;
	skybox_shader->set_uniform_m4("view_proj", view_proj);
	skybox_shader->set_uniform_f("intensity", skybox_intensity);
	skybox_shader->set_uniform_i("map", skybox_map->activate(0));
	cube->attach(*skybox_shader);
	cube->draw();
	
	/* set back to the default render target */
	Gpu::FrameBuffer::activate(nullptr);
}

void Renderer::load_mesh(const Mesh* m) {
	if (mesh_buffer.count(m) != 0) return;
	size_t group_size = m->groups.size();
	mesh_buffer.insert({m, std::make_unique<Gpu::VertexObject[]>(group_size)});
	auto* vertex_object_ptr = mesh_buffer[m].get();
	for (int g = 0; g < group_size; ++g) {
		vertex_object_ptr->load(*m, g);
		++vertex_object_ptr;
	}
}

void Renderer::load_image(const Image* i) {
	if (image_buffer.count(i) != 0) return;
	image_buffer.insert({i, std::make_unique<Gpu::Texture>()});
	auto* texture_ptr = image_buffer[i].get();
	texture_ptr->init_2d(*i, Gpu::Texture::default_format(*i));
	std::invoke(texture_callback, *texture_ptr);
}

void Renderer::load_material(const Material* m) {
	if (material_buffer.count(m) != 0) return;
	material_buffer.insert({m, std::make_unique<Gpu::Shader>()});
	auto* shader_ptr = material_buffer[m].get();
	shader_ptr->load_vert_file("shaders/lib/GBuffer.vert.glsl");
	shader_ptr->load_frag_file("shaders/lib/GBuffer.frag.glsl");
	Defines material_defines = m->get_defines();
	material_defines.set("LOCATION_C", std::to_string(location_c));
	material_defines.set("LOCATION_N", std::to_string(location_n));
	material_defines.set("LOCATION_M", std::to_string(location_m));
	material_defines.set("LOCATION_E", std::to_string(location_e));
	material_defines.set("LOCATION_I", std::to_string(location_i));
	shader_ptr->set_define(material_defines);
	shader_ptr->compile();
}

void Renderer::load_scene(const Scene &s) {
	/* load all instances in the scene */
	auto instances = s.to_instances();
	for (auto& instance_ptr : instances) {
		
		/* get mesh from instance */
		auto* mesh_ptr = instance_ptr->mesh;
		
		/* chech whether mesh is linked with instance */
		if (mesh_ptr == nullptr) continue;
		
		/* load the mesh linked with instance */
		load_mesh(mesh_ptr);
	}
	
	/* load materials in the material library */
	auto materials = s.get_materials();
	for (auto& material_ptr : materials) {
		
		/* load the uncompiled material linked with instance */
		if (material_ptr->get_shader<Gpu::Shader>() == nullptr) {
			load_material(material_ptr);
		}
		
		/* load the images linked with instance */
		size_t image_count = material_ptr->get_image_count();
		for (int i = 0; i < image_count; ++i) {
			load_image(material_ptr->get_image(i));
		}
		const Image* material_maps[] = {
			material_ptr->normal_map      ,
			material_ptr->displacement_map,
			material_ptr->color_map       ,
			material_ptr->emissive_map    ,
			material_ptr->roughness_map   ,
			material_ptr->metalness_map   ,
			material_ptr->specular_map    ,
		};
		for (auto& map : material_maps) {
			if (map != nullptr) load_image(map);
		}
	}
}

void Renderer::update_scene(Scene& s) {
	s.update_instances();
}

void Renderer::render(const Scene& s, const Camera& c) const {
	/* activate the render target */
	Gpu::FrameBuffer::activate(target);
	
	/* set the viewport */
	Gpu::set_viewport(viewport);
	
	/* set the scissor test */
	if (scissor_test) {
		Gpu::enable_scissor_test();
		Gpu::set_scissor(scissor);
	} else {
		Gpu::disable_scissor_test();
	}
	
	/* render the scene using a camera */
	render(s, c, [](const Instance& i, const Material& m) -> bool {
		/* apply the depth test setting of material */
		Gpu::apply_material_depth(m);
		
		/* apply the stencil test setting of material */
		Gpu::apply_material_stencil(m);
		
		/* apply the wireframe setting of material */
		Gpu::apply_material_wireframe(m);
		
		/* apply the render side setting of material */
		Gpu::apply_material_side(m);
		
		/* always render the material */
		return true;
	});
	
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
	
	/* render the scene using the camera of shadow */
	render(s, t.camera, [](const Instance& i, const Material& m) -> bool {
		/* apply the depth test setting of material */
		Gpu::apply_material_depth(m);
		
		/* apply the stencil test setting of material */
		Gpu::apply_material_stencil(m);
		
		/* apply the wireframe setting of material */
		Gpu::apply_material_wireframe(m);
		
		/* apply the render side setting of material */
		Gpu::apply_material_shadow_side(m);
		
		/* only render the material when casting shadow */
		return i.cast_shadow;
	});
	
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

bool Renderer::init_skybox() {
	/* prepare cube object */
	cube = std::make_unique<Gpu::VertexObject>();
	cube->load(BoxMesh::create());
	
	/* prepare skybox shader */
	skybox_shader = std::make_unique<Gpu::Shader>();
	skybox_shader->load_vert_file("shaders/lib/Skybox.vert.glsl");
	skybox_shader->load_frag_file("shaders/lib/Skybox.frag.glsl");
	
	/* finish initialization */
	return true;
}

void Renderer::render(const Scene& s, const Camera& c, const MaterialCallback& f) const {
	/* create transform matrices */
	Mat4 model;
	Mat4 view = c.viewing;
	Mat4 proj = c.projection;
	Mat4 model_view;
	Mat4 model_view_proj;
	Mat3 normal_mat;
	Vec3 camera_pos = c.position;
	
	/* set uniform variables */
	Uniforms uniforms = {
		{"model_m4"          , &model          },
		{"view_m4"           , &view           },
		{"proj_m4"           , &proj           },
		{"model_view_m4"     , &model_view     },
		{"model_view_proj_m4", &model_view_proj},
		{"normal_mat_m3"     , &normal_mat     },
		{"camera_pos_v3"     , &camera_pos     },
	};
	
	/* render all instances */
	auto instances = s.to_instances();
	for (auto& instance_ptr : instances) {
		
		/* check whether the instance is visible */
		if (!instance_ptr->visible) continue;
		
		/* get mesh from instance */
		auto* mesh_ptr = instance_ptr->mesh;
		
		/* check whether the mesh is linked */
		if (mesh_ptr == nullptr) continue;
		
		/* get matrices from instance */
		model = instance_ptr->matrix_global;
		model_view = view * model;
		model_view_proj = proj * model_view;
		normal_mat = inverse_3x3(Mat3{
			model[0][0], model[1][0], model[2][0],
			model[0][1], model[1][1], model[2][1],
			model[0][2], model[1][2], model[2][2],
		});
		
		/* check whether the mesh is loaded */
		if (mesh_buffer.count(mesh_ptr) == 0) {
			set_error("Renderer: Scene is not loaded");
			continue;
		}
		
		/* get vertex objects from mesh */
		auto* vertex_object_ptr = mesh_buffer.at(mesh_ptr).get();
		for (auto& group : mesh_ptr->groups) {
			
			/* get material from material groups */
			auto* material_ptr = s.get_material(mesh_ptr, group.name);
			if (material_ptr == nullptr) {
				material_ptr = s.get_material(group.name);
			}
			if (material_ptr == nullptr) {
				set_error("Renderer: Material is not linked");
				continue;
			}
			if (!material_ptr->visible) continue;
			
			/* get shader from material */
			auto* shader_ptr = material_ptr->get_shader<Gpu::Shader>();
			if (shader_ptr == nullptr) {
				shader_ptr = material_buffer.at(material_ptr).get();
			}
			
			/* render vertex object with shader */
			shader_ptr->use_program();
			vertex_object_ptr->attach(*shader_ptr);
			
			/* set uniforms from renderer */
			shader_ptr->set_uniforms(uniforms);
			
			/* set uniforms from material */
			shader_ptr->set_uniforms(material_ptr->get_uniforms());
			shader_ptr->set_uniform_i("normal_map"      , 16);
			shader_ptr->set_uniform_i("displacement_map", 17);
			shader_ptr->set_uniform_i("color_map"       , 18);
			shader_ptr->set_uniform_i("emissive_map"    , 19);
			shader_ptr->set_uniform_i("roughness_map"   , 20);
			shader_ptr->set_uniform_i("metalness_map"   , 21);
			shader_ptr->set_uniform_i("specular_map"    , 22);
			shader_ptr->set_uniform_i("env_map"         , 23);
			
			/* activate images linked with material */
			size_t image_count = material_ptr->get_image_count();
			for (int i = 0; i < image_count; ++i) {
				image_buffer.at(material_ptr->get_image(i))->activate(i);
			}
			const Image* material_maps[] = {
				material_ptr->normal_map      ,
				material_ptr->displacement_map,
				material_ptr->color_map       ,
				material_ptr->emissive_map    ,
				material_ptr->roughness_map   ,
				material_ptr->metalness_map   ,
				material_ptr->specular_map    ,
			};
			size_t map_size = std::size(material_maps);
			for (int i = 0; i < map_size; ++i) {
				if (material_maps[i] != nullptr) {
					image_buffer.at(material_maps[i])->activate(i + 16);
				}
			}
			
			/* activate probe linked with material */
			auto* env_probe_ptr = material_ptr->get_env_probe<EnvProbe>();
			if (env_probe_ptr != nullptr) {
				env_probe_ptr->activate(23);
				int max_lod = log2f(env_probe_ptr->get_resolution());
				shader_ptr->set_uniform_i("env_max_lod", max_lod);
				shader_ptr->set_uniform_f("env_intensity", env_probe_ptr->intensity);
			}
			
			/* invoke material callback function */
			if (f && !std::invoke(f, *instance_ptr, *material_ptr)) continue;
			
			/* draw vertex object */
			vertex_object_ptr->draw();
			++vertex_object_ptr;
		}
	}
}

std::unique_ptr<Gpu::VertexObject> Renderer::cube;

std::unique_ptr<Gpu::Shader> Renderer::skybox_shader;

}
