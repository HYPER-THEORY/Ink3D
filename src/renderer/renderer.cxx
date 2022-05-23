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

gpu::rect renderer::get_viewport() const {
	return viewport;
}

void renderer::set_viewport(const gpu::rect& v) {
	viewport = v;
}

bool renderer::get_scissor_test() const {
	return scissor_test;
}

void renderer::set_scissor_test(bool t) {
	scissor_test = t;
}

gpu::rect renderer::get_scissor() const {
	return scissor;
}

void renderer::set_scissor(const gpu::rect& s) {
	scissor = s;
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
	/* viewport setting */
	gpu::set_viewport(viewport);
	/* scissor setting */
	if (scissor_test) {
		gpu::enable_scissor_test();
		gpu::set_scissor(scissor);
	} else {
		gpu::disable_scissor_test();
	}
	/* create transform matrices */
	mat4 model;
	mat4 inv_model;
	mat4 model_view_proj;
	mat4 inv_view = mat4_inverse(c.viewing);
	mat4 inv_proj = mat4_inverse(c.projection);
	mat4 view_proj = c.projection * c.viewing;
	mat4 inv_view_proj = mat4_inverse(view_proj);
	/* set uniform variables */
	uniforms uniform_vars;
	uniform_vars.set("view_m4", &c.viewing);
	uniform_vars.set("inv_view_m4", &inv_view);
	uniform_vars.set("proj_m4", &c.projection);
	uniform_vars.set("inv_proj_m4", &inv_proj);
	uniform_vars.set("view_proj_m4", &view_proj);
	uniform_vars.set("inv_view_proj_m4", &inv_view_proj);
	uniform_vars.set("model_m4", &model);
	uniform_vars.set("inv_model_m4", &inv_model);
	uniform_vars.set("model_view_proj_m4", &model_view_proj);
	/* render instances */
	size_t instance_size = s.instance_size();
	auto* instances = s.get_instances();
	for (int i = 0; i < instance_size; ++i) {
		model = camera::model_transform(instances[i]);
		inv_model = mat4_inverse(model);
		model_view_proj = view_proj * model;
		/* get mesh from instance */
		auto* mesh_ptr = instances[i].mesh_ptr;
		if (mesh_buffer.count(mesh_ptr) == 0) {
			set_error("[Renderer Error] Scene is not preloaded");
			continue;
		}
		/* get vertexobjects from mesh */
		auto* vertexobject_ptr = mesh_buffer.at(mesh_ptr).get();
		for (auto& g : mesh_ptr->material_groups) {
			/* get material from material groups */
			auto* material_ptr = s.get_material(mesh_ptr, g.name);
			if (material_ptr == nullptr) {
				material_ptr = s.get_material(g.name);
			}
			if (material_ptr == nullptr) {
				set_error("[Renderer Error] Material is not linked");
				continue;
			}
			if (!material_ptr->visible) continue;
			/* get shader from material */
			auto* shader_ptr = static_cast<const gpu::shader*>(material_ptr->get_shader());
			if (shader_ptr == nullptr) {
				set_error("[Renderer Error] Shader is not linked");
				continue;
			}
			/* render vertexobject with shader */
			shader_ptr->use_program();
			vertexobject_ptr->attach(*shader_ptr);
			shader_ptr->set_uniforms(uniform_vars);
			shader_ptr->set_uniforms(material_ptr->get_uniforms());
			size_t image_size = material_ptr->image_size();
			auto* image_ptr = material_ptr->get_images();
			for (int t = 0; t < image_size; ++t) {
				image_buffer.at(image_ptr[t]).get()->active(t);
			}
			gpu::active_material(*material_ptr);
			vertexobject_ptr->draw();
			++vertexobject_ptr;
		}
	}
	gpu::framebuffer::default_active();
}
