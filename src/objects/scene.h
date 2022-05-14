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

#pragma once

#include <string>
#include <initializer_list>
#include <unordered_map>

#include "mesh.h"
#include "material.h"
#include "instance.h"

class scene {
public:
	/**
	 * Create a new scene.
	 */
	scene(const std::initializer_list<instance>& i = {});
	
	/**
	 * Returns the size of instances in the scene.
	 */
	size_t instance_size() const;
	
	/**
	 * Clear instances in the scene.
	 */
	void clear_instance();
	
	/**
	 * Add an instance to the scene.
	 *
	 * \param i instance
	 */
	void add_instance(const instance& i);
	
	/**
	 * Add instances to the scene.
	 *
	 * \param i list of instances
	 */
	void add_instances(const std::initializer_list<instance>& i);
	
	/**
	 * Returns instances in the scene.
	 */
	instance* get_instances();
	
	/**
	 * Returns instances in the scene.
	 */
	const instance* get_instances() const;
	
	/**
	 * Returns an instance in the scene.
	 *
	 * \param i index of instance
	 */
	instance& get_instance(int i);
	
	/**
	 * Returns an instance in the scene.
	 *
	 * \param i index of instance
	 */
	const instance& get_instance(int i) const;
	
	/**
	 * Clear materials in the scene.
	 */
	void clear_material();
	
	/**
	 * Returns the materials linked with the specified mesh.
	 *
	 * \param s the specified mesh
	 * \param n material name
	 */
	const material* get_material(const mesh* s, const std::string& n) const;
	
	/**
	 * Returns the materials linked with all meshes.
	 *
	 * \param n material name
	 */
	const material* get_material(const std::string& n) const;
	
	/**
	 * Link materials with the specified mesh.
	 *
	 * \param s the specified mesh
	 * \param n material name
	 * \param m material
	 */
	void set_material(const mesh* s, const std::string& n, const material* m);
	
	/**
	 * Link materials with all meshes.
	 *
	 * \param n material name
	 * \param m material
	 */
	void set_material(const std::string& n, const material* m);
	
	/**
	 * Returns an iterator pointing the first element of material library.
	 */
	std::unordered_map<std::string, const material*>::const_iterator material_begin() const;
	
	/**
	 * Returns an iterator pointing the last element of material library.
	 */
	std::unordered_map<std::string, const material*>::const_iterator material_end() const;
	
private:
	std::vector<instance> instances;
	std::unordered_map<std::string, const material*> material_library;
};
