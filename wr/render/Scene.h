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

#include <initializer_list>
#include <unordered_map>
#include "../resource/Image.h"
#include "../resource/Mesh.h"
#include "../resource/Material.h"
#include "../resource/SimpleMat.h"
#include "../graphics/Instance.h"
#include "../graphics/Camera.h"
#include "../graphics/Gpu.h"

namespace WR {

class Scene {
public:
	/**
	 * Create a new scene.
	 */
	Scene(const std::initializer_list<Instance>& i = {});
	
	/**
	 * Determine the size of scene.
	 */
	size_t size() const;
	
	/**
	 * Clear scene.
	 */
	void clear();
	
	/**
	 * Add an instance to scene.
	 *
	 * \param i instance
	 */
	void add_instance(const Instance& i);
	
	/**
	 * Add instances to scene.
	 *
	 * \param i list of instances
	 */
	void add_instances(const std::initializer_list<Instance>& i);
	
	/**
	 * Get instances of scene.
	 */
	Instance* get_instances();
	
	/**
	 * Get instances of scene.
	 */
	const Instance* get_instances() const;
	
	/**
	 * Get instance of scene.
	 *
	 * \param i index of instance
	 */
	Instance& get_instance(int i);
	
	/**
	 * Get instance of scene.
	 *
	 * \param i index of instance
	 */
	const Instance& get_instance(int i) const;
	
private:
	std::vector<Instance> instances;
};

}
