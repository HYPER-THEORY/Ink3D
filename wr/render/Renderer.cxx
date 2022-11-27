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

#include "Renderer.h"

namespace WR {

void Renderer::preload(const Scene &s) {
	size_t instance_size = s.size();
	auto* instances = s.get_instances();
	for (int i = 0; i < instance_size; ++i) {
		auto* mesh_ptr = instances[i].mesh_ptr;
		if (mesh_buffer.count(mesh_ptr) != 0) continue;
		size_t group_size = mesh_ptr->material_groups.size();
		mesh_buffer.insert({mesh_ptr, std::make_unique<Gpu::VertexObject[]>(group_size)});
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
				image_buffer.insert({images[t], std::make_unique<Gpu::Texture>()});
				image_buffer[images[t]].get()->preload_2d(*images[t]);
			}
		}
	}
}

void Renderer::clear() {
	mesh_buffer.clear();
	image_buffer.clear();
}

void Renderer::render(const Scene& s, const Camera& c) const {
	Mat<4, 4> view_proj = c.projection * c.viewing;
	std::unordered_map<std::string, Mat<4, 4> > matrices;
	matrices.insert_or_assign("view", c.viewing);
	matrices.insert_or_assign("inv_view", mat4_inverse(c.viewing));
	matrices.insert_or_assign("proj", c.projection);
	matrices.insert_or_assign("inv_proj", mat4_inverse(c.projection));
	matrices.insert_or_assign("view_proj", view_proj);
	matrices.insert_or_assign("inv_view_proj", mat4_inverse(view_proj));
	size_t instance_size = s.size();
	auto* instances = s.get_instances();
	for (int i = 0; i < instance_size; ++i) {
		Mat<4, 4> model = Camera::model_transform(instances[i]);
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

void Renderer::texture_parameter(int w, int mag, int min, int mip) {
	for (auto& t : image_buffer) {
		t.second->set_parameters(w, mag, min, mip);
	}
}

void Renderer::use_material(const Material& m, const Gpu::VertexObject& o,
                            const std::unordered_map<std::string, Mat<4, 4> >& ms) const {
	/* get shader */
	auto* shader = static_cast<const Gpu::Shader*>(m.get_shader());
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

void Renderer::material_depth_test(const Material &m) {
	if (m.depth_test) {
		Gpu::enable_depth_test();
	} else {
		Gpu::disable_depth_test();
	}
}

void Renderer::material_stencil_test(const Material &m) {
	if (m.stencil_test) {
		Gpu::enable_stencil_test();
	} else {
		Gpu::disable_stencil_test();
	}
}

void Renderer::material_wireframe(const Material &m) {
	if (m.wireframe) {
		Gpu::enable_wireframe();
	} else {
		Gpu::disable_wireframe();
	}
}

void Renderer::material_side(const Material &m) {
	if (m.side == FRONT_SIDE) {
		Gpu::enable_cull_face();
		Gpu::cull_face(BACK_SIDE);
	} else if (m.side == BACK_SIDE) {
		Gpu::enable_cull_face();
		Gpu::cull_face(FRONT_SIDE);
	} else if (m.side == DOUBLE_SIDE) {
		Gpu::disable_cull_face();
	} else {
		set_error("[Renderer Error] Illegal material side");
	}
}

void Renderer::material_shading(const Material& m) {
	int material_type = m.type();
	if (material_type == MATERIAL_DEFAULT) {
		/* do nothing */
	} else if (material_type == MATERIAL_SIMPLE) {
		simple_shading(static_cast<const SimpleMat&>(m));
	} else {
		set_error("[Renderer Error] Invalid material type");
	}
}

void Renderer::simple_shading(const SimpleMat& m) {
	auto* shader = static_cast<const Gpu::Shader*>(m.get_shader());
	shader->uniform_vec3("color", m.color);
	shader->uniform_int("int", m.color_map);
}

}
