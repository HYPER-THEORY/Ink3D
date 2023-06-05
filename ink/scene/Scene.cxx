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

#include "Scene.h"

#define FMT_HEADER_ONLY
#include "fmt/format.h"

namespace Ink {

Scene::Scene(const std::string& n) : Instance(n) {}

Material* Scene::get_material(const std::string& n) const {
	if (material_library.count(n) == 0) {
		return nullptr;
	}
	return material_library.at(n);
}

Material* Scene::get_material(const std::string& n, const Mesh& s) const {
	auto name = fmt::format("M{}#{}", reinterpret_cast<size_t>(&s), n);
	if (material_library.count(name) == 0) {
		return nullptr;
	}
	return material_library.at(name);
}

Material* Scene::get_material(const std::string& n, const Instance& s) const {
	auto name = fmt::format("I{}#{}", reinterpret_cast<size_t>(&s), n);
	if (material_library.count(name) == 0) {
		return nullptr;
	}
	return material_library.at(name);
}

void Scene::set_material(const std::string& n, Material* m) {
	material_library.insert_or_assign(n, m);
}

void Scene::set_material(const std::string& n, const Mesh& s, Material* m) {
	auto name = fmt::format("M{}#{}", reinterpret_cast<size_t>(&s), n);
	material_library.insert_or_assign(name, m);
}

void Scene::set_material(const std::string& n, const Instance& s, Material* m) {
	auto name = fmt::format("I{}#{}", reinterpret_cast<size_t>(&s), n);
	material_library.insert_or_assign(name, m);
}

void Scene::remove_material(const std::string& n) {
	material_library.erase(n);
}

void Scene::remove_material(const std::string& n, const Mesh& s) {
	auto name = fmt::format("M{}#{}", reinterpret_cast<size_t>(&s), n);
	material_library.erase(name);
}

void Scene::remove_material(const std::string& n, const Instance& s) {
	auto name = fmt::format("I{}#{}", reinterpret_cast<size_t>(&s), n);
	material_library.erase(name);
}

void Scene::clear_materials() {
	material_library.clear();
}

std::vector<Material*> Scene::get_materials() const {
	std::vector<Material*> materials;
	for (auto& [name, material] : material_library) {
		materials.emplace_back(material);
	}
	return materials;
}

LinearFog* Scene::get_linear_fog() const {
	return linear_fog;
}

void Scene::set_fog(LinearFog* f) {
	linear_fog = f;
	exp2_fog = nullptr;
}

Exp2Fog* Scene::get_exp2_fog() const {
	return exp2_fog;
}

void Scene::set_fog(Exp2Fog* f) {
	exp2_fog = f;
	linear_fog = nullptr;
}

void Scene::add_light(PointLight* l) {
	point_lights.emplace_back(l);
}

void Scene::remove_light(PointLight* l) {
	size_t size = point_lights.size();
	for (int i = 0; i < size; ++i) {
		if (point_lights[i] == l) {
			point_lights.erase(point_lights.begin() + i);
		}
	}
}

size_t Scene::get_point_light_count() const {
	return point_lights.size();
}

PointLight* Scene::get_point_light(int i) const {
	return point_lights[i];
}

void Scene::add_light(SpotLight* l) {
	spot_lights.emplace_back(l);
}

void Scene::remove_light(SpotLight* l) {
	size_t size = spot_lights.size();
	for (int i = 0; i < size; ++i) {
		if (spot_lights[i] == l) {
			spot_lights.erase(spot_lights.begin() + i);
		}
	}
}

size_t Scene::get_spot_light_count() const {
	return spot_lights.size();
}

SpotLight* Scene::get_spot_light(int i) const {
	return spot_lights[i];
}

void Scene::add_light(DirectionalLight* l) {
	directional_lights.emplace_back(l);
}

void Scene::remove_light(DirectionalLight* l) {
	size_t size = directional_lights.size();
	for (int i = 0; i < size; ++i) {
		if (directional_lights[i] == l) {
			directional_lights.erase(directional_lights.begin() + i);
		}
	}
}

size_t Scene::get_directional_light_count() const {
	return directional_lights.size();
}

DirectionalLight* Scene::get_directional_light(int i) const {
	return directional_lights[i];
}

void Scene::add_light(HemisphereLight* l) {
	hemisphere_lights.emplace_back(l);
}

void Scene::remove_light(HemisphereLight* l) {
	size_t size = hemisphere_lights.size();
	for (int i = 0; i < size; ++i) {
		if (hemisphere_lights[i] == l) {
			hemisphere_lights.erase(hemisphere_lights.begin() + i);
		}
	}
}

size_t Scene::get_hemisphere_light_count() const {
	return hemisphere_lights.size();
}

HemisphereLight* Scene::get_hemisphere_light(int i) const {
	return hemisphere_lights[i];
}

void Scene::clear_lights() {
	point_lights.clear();
	spot_lights.clear();
	directional_lights.clear();
	hemisphere_lights.clear();
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
		if (!current->visible) continue;
		size_t count = current->get_child_count();
		for (int i = 0; i < count; ++i) {
			unvisited.emplace_back(current->get_child(i));
		}
		if (current->mesh != nullptr) {
			instances.emplace_back(current);
		}
	}
	return instances;
}

}
