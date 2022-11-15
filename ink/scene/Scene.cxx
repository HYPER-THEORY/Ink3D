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

#include "Scene.h"

#include "../core/Format.h"

namespace Ink {

Scene::Scene(const std::string& n) : Instance(n) {}

const Material* Scene::get_material(const Mesh* s, const std::string& n) const {
	std::string name = Format::format("{}#{}", reinterpret_cast<size_t>(s), n);
	if (material_library.count(name) == 0) return nullptr;
	return material_library.at(name);
}

const Material* Scene::get_material(const std::string& n) const {
	if (material_library.count(n) == 0) return nullptr;
	return material_library.at(n);
}

std::vector<const Material*> Scene::get_materials() const {
	std::vector<const Material*> materials;
	for (auto& [name, material] : material_library) {
		materials.emplace_back(material);
	}
	return materials;
}

void Scene::set_material(const Mesh* s, const std::string& n, const Material* m) {
	std::string name = Format::format("{}#{}", reinterpret_cast<size_t>(s), n);
	material_library.insert_or_assign(name, m);
}

void Scene::set_material(const std::string& n, const Material* m) {
	material_library.insert_or_assign(n, m);
}

void Scene::clear_material() {
	material_library.clear();
}

const LinearFog* Scene::get_linear_fog() const {
	return linear_fog;
}

void Scene::set_fog(const LinearFog* f) {
	linear_fog = f;
	exp2_fog = nullptr;
}

const Exp2Fog* Scene::get_exp2_fog() const {
	return exp2_fog;
}

void Scene::set_fog(const Exp2Fog* f) {
	exp2_fog = f;
	linear_fog = nullptr;
}

void Scene::add_light(const PointLight* l) {
	point_lights.emplace_back(l);
}

void Scene::add_light(const SpotLight* l) {
	spot_lights.emplace_back(l);
}

void Scene::add_light(const DirectionalLight* l) {
	directional_lights.emplace_back(l);
}

void Scene::add_light(const HemisphereLight* l) {
	hemisphere_lights.emplace_back(l);
}

void Scene::clear_light() {
	point_lights.clear();
	spot_lights.clear();
	directional_lights.clear();
	hemisphere_lights.clear();
}

size_t Scene::get_point_light_count() const {
	return point_lights.size();
}

const PointLight* Scene::get_point_light(int i) const {
	return point_lights[i];
}

size_t Scene::get_spot_light_count() const {
	return spot_lights.size();
}

const SpotLight* Scene::get_spot_light(int i) const {
	return spot_lights[i];
}

size_t Scene::get_directional_light_count() const {
	return directional_lights.size();
}

const DirectionalLight* Scene::get_directional_light(int i) const {
	return directional_lights[i];
}

size_t Scene::get_hemisphere_light_count() const {
	return hemisphere_lights.size();
}

const HemisphereLight* Scene::get_hemisphere_light(int i) const {
	return hemisphere_lights[i];
}

void Scene::update_instances() {
	std::vector<Instance*> unvisited;
	matrix_local = transform();
	matrix_global = matrix_local;
	unvisited.emplace_back(this);
	while (!unvisited.empty()) {
		Instance* current = unvisited.back();
		unvisited.pop_back();
		size_t count = current->get_child_count();
		for (int i = 0; i < count; ++i) {
			Instance* child = current->get_child(i);
			child->matrix_local = child->transform();
			child->matrix_global = current->matrix_global * child->matrix_local;
			unvisited.emplace_back(child);
		}
	}
}

std::vector<const Instance*> Scene::to_instances() const {
	std::vector<const Instance*> unvisited;
	std::vector<const Instance*> instances;
	unvisited.emplace_back(this);
	while (!unvisited.empty()) {
		const Instance* current = unvisited.back();
		unvisited.pop_back();
		size_t count = current->get_child_count();
		for (int i = 0; i < count; ++i) {
			unvisited.emplace_back(current->get_child(i));
		}
		instances.emplace_back(current);
	}
	return instances;
}

std::vector<const Instance*> Scene::to_visible_instances() const {
	std::vector<const Instance*> unvisited;
	std::vector<const Instance*> instances;
	unvisited.emplace_back(this);
	while (!unvisited.empty()) {
		const Instance* current = unvisited.back();
		unvisited.pop_back();
		size_t count = current->get_child_count();
		for (int i = 0; i < count; ++i) {
			unvisited.emplace_back(current->get_child(i));
		}
		if (current->visible && current->mesh != nullptr) {
			instances.emplace_back(current);
		}
	}
	return instances;
}

}
