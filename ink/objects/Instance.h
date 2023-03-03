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

#pragma once

#include "Mesh.h"

#include "../math/Euler.h"

namespace Ink {

class Instance {
public:
	std::string name;              /**< instance name */
	
	bool visible = true;           /**< whether the instance will be rendered */
	
	bool cast_shadow = true;       /**< whether the instance will cast shadows */
	
	int priority = 0;              /**< the sorting priority in rendering */
	
	Vec3 position = {0, 0, 0};     /**< the position vector of instance */
	
	Vec3 scale = {1, 1, 1};        /**< the scaling vector of instance */
	
	Euler rotation;                /**< the rotation angles of instance */
	
	Mat4 matrix_local;             /**< the local transform matrix of instance */
	
	Mat4 matrix_global;            /**< the global transform matrix of instance */
	
	Mesh* mesh = nullptr;          /**< the linked mesh of instance */
	
	/**
	 * Creates a new Instance object, which is the minimum unit of rendering.
	 *
	 * \param n instance name
	 */
	static Instance* create(const std::string& n = "");
	
	/**
	 * Destroys the specified Instance object.
	 *
	 * \param i instance
	 */
	static void destroy(const Instance* i);
	
	/**
	 * Adds the specified instance as the child of the instance. The index
	 * starts from zero.
	 *
	 * \param i instance
	 */
	void add(Instance* i);
	
	/**
	 * Adds the specified instances as the children of the instance. The index
	 * starts from zero.
	 *
	 * \param l instance list
	 */
	void add(const std::initializer_list<Instance*>& l);
	
	/**
	 * Removes the specified instance as the child of the instance.
	 *
	 * \param i instance
	 */
	void remove(Instance* i);
	
	/**
	 * Removes the specified instances as the children of the instance.
	 *
	 * \param l instance list
	 */
	void remove(const std::initializer_list<Instance*>& l);
	
	/**
	 * Removes all the children from the instance.
	 */
	void clear();
	
	/**
	 * Returns the child at the specified index in the instance.
	 *
	 * \param i the index of child
	 */
	Instance* get_child(int i) const;
	
	/**
	 * Returns the first child matching the specified name in the instance.
	 *
	 * \param n the name of child
	 */
	Instance* get_child(const std::string& n) const;
	
	/**
	 * Returns the number of its children in the instance.
	 */
	size_t get_child_count() const;
	
	/**
	 * Returns the parent of the instance.
	 */
	Instance* get_parent() const;
	
	/**
	 * Sets the transform vectors (position, rotation and scaling) of the
	 * instance.
	 *
	 * \param p position vector
	 * \param r rotation angles
	 * \param s scaling vector
	 */
	void set_transform(const Vec3& p, const Euler& r, const Vec3& s);
	
	/**
	 * Updates the local transform matrix. This function is equivalent to
	 * "matrix_local = transform();".
	 */
	void update_matrix_local();
	
	/**
	 * Updates the global transform matrix. This function is equivalent to
	 * "matrix_global = transform_global();".
	 */
	void update_matrix_global();
	
	/**
	 * Converts the specified vector from global space to local space. This
	 * function only works when matrix_global is correct.
	 *
	 * \param v vector
	 */
	Vec3 global_to_local(const Vec3& v) const;
	
	/**
	 * Converts the specified vector from local space to global space. This
	 * function only works when matrix_global is correct.
	 *
	 * \param v vector
	 */
	Vec3 local_to_global(const Vec3& v) const;
	
	/**
	 * Returns a local transform matrix by calculating the position, rotation
	 * and scale of instance.
	 */
	Mat4 transform() const;
	
	/**
	 * Returns a global transform matrix by multiplying its ancestors' transform
	 * matrices recursively.
	 */
	Mat4 transform_global() const;
	
	/**
	 * Returns a transform matrix by calculating position, rotation and scale.
	 *
	 * \param p position vector
	 * \param r rotation angles
	 * \param s scale vector
	 */
	static Mat4 transform(const Vec3& p, const Euler& r, const Vec3& s);
	
protected:
	Instance* parent = nullptr;
	
	std::vector<Instance*> children;
	
	explicit Instance(const std::string& n);
};

}
