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

#pragma once

#include "Mesh.h"

namespace Ink {

class Instance {
public:
	std::string name;              /**< instance name */
	
	bool visible = true;           /**< whether the instance will be rendered */
	
	bool cast_shadow = true;       /**< whether the instance will cast shadows */
	
	Vec3 position = {0, 0, 0};     /**< the position vector of instance */
	
	Vec3 rotation = {0, 0, 0};     /**< the rotation vector of instance */
	
	Vec3 scale = {1, 1, 1};        /**< the scaling vector of instance */
	
	Mat4 matrix_local;             /**< the local transform matrix of instance */
	
	Mat4 matrix_global;            /**< the global transform matrix of instance */
	
	const Mesh* mesh = nullptr;    /**< the linked mesh of instance */
	
	/**
	 * Create a new Instance, which is the minimum unit of rendering.
	 *
	 * \param n instance name
	 */
	static Instance* create(const std::string& n = "");
	
	/**
	 * Destroy Instance.
	 *
	 * \param i instance
	 */
	static void destroy(const Instance* i);
	
	/**
	 * Add an instance as the child of this instance.
	 *
	 * \param i instance
	 */
	void add(Instance* i);
	
	/**
	 * Add instances as the children of this instance.
	 *
	 * \param l instance list
	 */
	void add(const std::initializer_list<Instance*>& l);
	
	/**
	 * Remove the instance as the child of this instance.
	 *
	 * \param i instance
	 */
	void remove(Instance* i);
	
	/**
	 * Remove the instances as the children of this instance.
	 *
	 * \param l instance list
	 */
	void remove(const std::initializer_list<Instance*>& l);
	
	/**
	 * Remove all the children.
	 */
	void clear();
	
	/**
	 * Returns the child at the index.
	 *
	 * \param i the index of child
	 */
	Instance* get_child(int i) const;
	
	/**
	 * Returns the first child matching the name.
	 *
	 * \param n the name of child
	 */
	Instance* get_child(const std::string& n) const;
	
	/**
	 * Returns the number of its children.
	 */
	size_t get_child_count() const;
	
	/**
	 * Returns the parent of the instance.
	 */
	Instance* get_parent() const;
	
	/**
	 * Sets the transform components (position, rotation and scaling).
	 *
	 * \param p position vector
	 * \param r rotation vector
	 * \param s scaling vector
	 */
	void set_transform(const Vec3& p, const Vec3& r, const Vec3& s);
	
	/**
	 * Update the local transform matrix.
	 */
	void update_matrix_local();
	
	/**
	 * Update the global transform matrix.
	 */
	void update_matrix_global();
	
	/**
	 * Convert the vector from global space to local space.
	 *
	 * \param v vector
	 */
	Vec3 global_to_local(const Vec3& v) const;
	
	/**
	 * Convert the vector from local space to global space.
	 *
	 * \param v vector
	 */
	Vec3 local_to_global(const Vec3& v) const;
	
	/**
	 * Returns a transform matrix with the position, rotation and scale.
	 *
	 * \param p position vector
	 * \param r rotation vector
	 * \param s scale vector
	 */
	static Mat4 transform(const Vec3& p, const Vec3& r, const Vec3& s);
	
	/**
	 * Returns a transform matrix with the local position, rotation and scale
	 * of the instance.
	 */
	Mat4 transform() const;
	
	/**
	 * Returns a transform matrix with the global position, rotation and scale.
	 */
	Mat4 transform_global() const;
	
protected:
	Instance* parent = nullptr;
	
	std::vector<Instance*> children;
	
	Instance(const std::string& n);
};

}
