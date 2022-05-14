/*
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

#include "renderer.h"

void renderer::clear(bool c, bool d, bool s) const {
	if (target != nullptr) target->active();
	gpu::set_clear_color(clear_color);
	gpu::clear(c, d, s);
	gpu::framebuffer::default_active();
}

vec4 renderer::get_clear_color() const {
	return clear_color;
}

void renderer::set_clear_color(const vec4& c) {
	clear_color = c;
}

const gpu::framebuffer* renderer::get_target() const {
	return target;
}

void renderer::set_target(const gpu::framebuffer *t) {
	target = t;
}

void renderer::preload(const scene &s, int wrap, int mag, int min, bool mip) {
	size_t instance_size = s.instance_size();
	auto* instances = s.get_instances();
	for (int i = 0; i < instance_size; ++i) {
		auto* mesh_ptr = instances[i].mesh_ptr;
		if (mesh_buffer.count(mesh_ptr) != 0) continue;
		size_t group_size = mesh_ptr->material_groups.size();
		mesh_buffer.insert({mesh_ptr, std::make_unique<gpu::vertexobject[]>(group_size)});
		auto* vertexobject_ptr = mesh_buffer[mesh_ptr].get();
		/* preload vertexobject */
		for (int g = 0; g < group_size; ++g) {
			vertexobject_ptr->preload(*mesh_ptr, g);
			++vertexobject_ptr;
		}
		/* preload textures */
		auto material_end = s.material_end();
		for (auto i = s.material_begin(); i != material_end; ++i) {
			auto* material_ptr = i->second;
			if (material_ptr == nullptr) {
				set_error("[Renderer Error] Material is not linked");
				continue;
			}
			size_t image_size = material_ptr->image_size();
			auto* images = material_ptr->get_images();
			for (int t = 0; t < image_size; ++t) {
				if (image_buffer.count(images[t]) != 0) continue;
				image_buffer.insert({images[t], std::make_unique<gpu::texture>()});
				auto* texture_ptr = image_buffer[images[t]].get();
				texture_ptr->preload_2d(*images[t]);
				texture_ptr->set_wrap_all(wrap);
				texture_ptr->set_filters(mag, min);
				if (mip) texture_ptr->generate_mipmap();
			}
		}
	}
}

void renderer::render(const scene& s, const camera& c) const {
	if (target != nullptr) target->active();
	mat4 view_proj = c.projection * c.viewing;
	std::unordered_map<std::string, mat4> uniform_vars;
	uniform_vars.insert_or_assign("view", c.viewing);
	uniform_vars.insert_or_assign("inv_view", mat4_inverse(c.viewing));
	uniform_vars.insert_or_assign("proj", c.projection);
	uniform_vars.insert_or_assign("inv_proj", mat4_inverse(c.projection));
	uniform_vars.insert_or_assign("view_proj", view_proj);
	uniform_vars.insert_or_assign("inv_view_proj", mat4_inverse(view_proj));
	size_t instance_size = s.instance_size();
	auto* instances = s.get_instances();
	for (int i = 0; i < instance_size; ++i) {
		mat4 model = camera::model_transform(instances[i]);
		uniform_vars.insert_or_assign("model", model);
		uniform_vars.insert_or_assign("inv_model", mat4_inverse(model));
		uniform_vars.insert_or_assign("model_view_proj", view_proj * model);
		auto* mesh_ptr = instances[i].mesh_ptr;
		if (mesh_buffer.count(mesh_ptr) == 0) {
			set_error("[Renderer Error] Scene is not preloaded");
			continue;
		}
		auto* vertexobject_ptr = mesh_buffer.at(mesh_ptr).get();
		for (auto& g : mesh_ptr->material_groups) {
			auto* material_ptr = s.get_material(mesh_ptr, g.name);
			if (material_ptr == nullptr) {
				material_ptr = s.get_material(g.name);
			}
			if (material_ptr == nullptr) {
				set_error("[Renderer Error] Material is not linked");
				continue;
			}
			auto* shader_ptr = static_cast<const gpu::shader*>(material_ptr->get_shader());
			if (shader_ptr == nullptr) {
				set_error("[Renderer Error] Shader is not linked");
				continue;
			}
			shader_ptr->use_program();
			vertexobject_ptr->attach(*shader_ptr);
			for (auto& [n, v] : uniform_vars) {
				shader_ptr->uniform_mat4(n, v);
			}
			use_material(material_ptr, shader_ptr);
			vertexobject_ptr->draw();
			++vertexobject_ptr;
		}
	}
	gpu::framebuffer::default_active();
}

void renderer::use_material(const material* m, const gpu::shader* s) const {
	/* pass informations to shader */
	s->uniform_vec3("color", m->color);
	s->uniform_int("color_map", m->color_map);
	/* active textures */
	size_t image_size = m->image_size();
	auto* image_ptr = m->get_images();
	for (int t = 0; t < image_size; ++t) {
		image_buffer.at(image_ptr[t]).get()->active(t);
	}
	/* gpu settings */
	gpu_settings(m);
}

void renderer::gpu_settings(const material* m) {
	/* depth test setting */
	m->depth_test ?
		gpu::enable_depth_test() :
		gpu::disable_depth_test();
	/* stencil test setting */
	m->stencil_test ?
		gpu::enable_stencil_test() :
		gpu::disable_stencil_test();
	/* wireframe setting */
	m->wireframe ?
		gpu::enable_wireframe() :
		gpu::disable_wireframe();
	/* side setting */
	if (m->side == FRONT_SIDE) {
		gpu::enable_cull_face();
		gpu::set_cull_face(BACK_SIDE);
	} else if (m->side == BACK_SIDE) {
		gpu::enable_cull_face();
		gpu::set_cull_face(FRONT_SIDE);
	} else if (m->side == DOUBLE_SIDE) {
		gpu::disable_cull_face();
	} else {
		set_error("[Renderer Error] Illegal material side");
	}
}
