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

void renderer::preload(const scene &s) {
	size_t instance_size = s.size();
	auto* instances = s.get_instances();
	for (int i = 0; i < instance_size; ++i) {
		auto* mesh_ptr = instances[i].mesh_ptr;
		if (mesh_buffer.count(mesh_ptr) != 0) continue;
		size_t group_size = mesh_ptr->material_groups.size();
		mesh_buffer.insert({mesh_ptr, std::make_unique<gpu::vertexobject[]>(group_size)});
		auto* vertexobject = mesh_buffer[mesh_ptr].get();
		/* preload vertexobject */
		for (int g = 0; g < group_size; ++g) {
			(vertexobject++)->preload(*mesh_ptr, g);
		}
		/* preload textures */
		for (auto& m : mesh_ptr->material_library) {
			if (!m.second) {
				set_error("[Renderer Error] Material is not linked");
				continue;
			}
			size_t image_size = m.second->image_size();
			auto* images = m.second->get_images();
			for (int t = 0; t < image_size; ++t) {
				if (image_buffer.count(images[t]) != 0) continue;
				image_buffer.insert({images[t], std::make_unique<gpu::texture>()});
				image_buffer[images[t]].get()->preload_2d(*images[t]);
			}
		}
	}
}

void renderer::clear() {
	mesh_buffer.clear();
	image_buffer.clear();
}

void renderer::render(const scene& s, const camera& c) const {
	mat<4, 4> view_proj = c.projection * c.viewing;
	std::unordered_map<std::string, mat<4, 4> > matrices;
	matrices.insert_or_assign("view", c.viewing);
	matrices.insert_or_assign("inv_view", mat4_inverse(c.viewing));
	matrices.insert_or_assign("proj", c.projection);
	matrices.insert_or_assign("inv_proj", mat4_inverse(c.projection));
	matrices.insert_or_assign("view_proj", view_proj);
	matrices.insert_or_assign("inv_view_proj", mat4_inverse(view_proj));
	size_t instance_size = s.size();
	auto* instances = s.get_instances();
	for (int i = 0; i < instance_size; ++i) {
		mat<4, 4> model = camera::model_transform(instances[i]);
		matrices.insert_or_assign("model", model);
		matrices.insert_or_assign("inv_model", mat4_inverse(model));
		matrices.insert_or_assign("model_view_proj", view_proj * model);
		auto* mesh_ptr = instances[i].mesh_ptr;
		auto* vertexobject = mesh_buffer.at(mesh_ptr).get();
		for (auto& g : mesh_ptr->material_groups) {
			auto* material_ptr = mesh_ptr->get_material(g.name);
			if (material_ptr) {
				use_material(*material_ptr, *(vertexobject++), matrices);
			} else {
				set_error("[Renderer Error] Material is not linked");
			}
		}
	}
}

void renderer::texture_parameter(int w, int mag, int min, int mip) {
	for (auto& t : image_buffer) {
		t.second->set_parameters(w, mag, min, mip);
	}
}

void renderer::use_material(const material& m, const gpu::vertexobject& o,
							const std::unordered_map<std::string, mat<4, 4> >& ms) const {
	/* get shader */
	auto* shader = static_cast<const gpu::shader*>(m.get_shader());
	if (!shader) return static_cast<void>(set_error("[Renderer Error] Shader is not linked"));
	/* use shader */
	shader->use();
	/* attach vertexobject to shader */
	o.attach(*shader);
	/* pass informations to shader */
	for (auto& kv : ms) {
		shader->uniform_mat4(kv.first, kv.second);
	}
	material_shading(m);
	/* use textures */
	size_t image_size = m.image_size();
	auto* image_ptr = m.get_images();
	for (int t = 0; t < image_size; ++t) {
		image_buffer.at(image_ptr[t]).get()->use(t);
	}
	/* material settings */
	material_depth_test(m);
	material_stencil_test(m);
	material_wireframe(m);
	material_side(m);
	/* draw vertexobject */
	o.draw();
}

void renderer::material_depth_test(const material &m) {
	if (m.depth_test) {
		gpu::enable_depth_test();
	} else {
		gpu::disable_depth_test();
	}
}

void renderer::material_stencil_test(const material &m) {
	if (m.stencil_test) {
		gpu::enable_stencil_test();
	} else {
		gpu::disable_stencil_test();
	}
}

void renderer::material_wireframe(const material &m) {
	if (m.wireframe) {
		gpu::enable_wireframe();
	} else {
		gpu::disable_wireframe();
	}
}

void renderer::material_side(const material &m) {
	if (m.side == FRONT_SIDE) {
		gpu::enable_cull_face();
		gpu::cull_face(BACK_SIDE);
	} else if (m.side == BACK_SIDE) {
		gpu::enable_cull_face();
		gpu::cull_face(FRONT_SIDE);
	} else if (m.side == DOUBLE_SIDE) {
		gpu::disable_cull_face();
	} else {
		set_error("[Renderer Error] Illegal material side");
	}
}

void renderer::material_shading(const material& m) {
	int material_type = m.type();
	if (material_type == MATERIAL_DEFAULT) {
		/* do nothing */
	} else if (material_type == MATERIAL_SIMPLE) {
		simple_shading(static_cast<const simplemat&>(m));
	} else {
		set_error("[Renderer Error] Invalid material type");
	}
}

void renderer::simple_shading(const simplemat& m) {
	auto* shader = static_cast<const gpu::shader*>(m.get_shader());
	shader->uniform_vec3("color", m.color);
	shader->uniform_int("int", m.color_map);
}
