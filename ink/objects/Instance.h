/**
 * Copyright (C) 2021-2023 HYPERTHEORY
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

namespace ink {

class Instance {
public:
	std::string name;              /**< instance name */
	
	bool visible = true;           /**< whether the instance will be rendered */
	
	bool cast_shadow = true;       /**< whether the instance will cast shadows */
	
	int priority = 0;              /**< the sorting priority in rendering */
	
	Vec3 position = {0, 0, 0};     /**< the position vector of the instance */
	
	Vec3 scale = {1, 1, 1};        /**< the scaling vector of the instance */
	
	Euler rotation;                /**< the rotation angles of the instance */
	
	Mat4 matrix_local;             /**< the transform matrix in the local space */
	
	Mat4 matrix_global;            /**< the transform matrix in the global space */
	
	Mesh* mesh = nullptr;          /**< the linked mesh of the instance */
	
	/**
	 * Creates a new Instance object, which is the minimum unit of rendering.
	 *
	 * \param n instance name
	 */
	Instance(const std::string& n = "");
	
	/**
	 * Adds the specified instance as the child of this instance. The index
	 * starts from zero.
	 *
	 * \param i instance
	 */
	void add(Instance* i);
	
	/**
	 * Adds the specified instances as the children of this instance. The index
	 * starts from zero.
	 *
	 * \param l instance list
	 */
	void add(const std::initializer_list<Instance*>& l);
	
	/**
	 * Removes the specified instance as the child of this instance.
	 *
	 * \param i instance
	 */
	void remove(Instance* i);
	
	/**
	 * Removes the specified instances as the children of this instance.
	 *
	 * \param l instance list
	 */
	void remove(const std::initializer_list<Instance*>& l);
	
	/**
	 * Removes all the children from this instance.
	 */
	void clear();
	
	/**
	 * Returns the child at the specified index of this instance.
	 *
	 * \param i the index of the child
	 */
	Instance* get_child(int i) const;
	
	/**
	 * Returns the first child matching the specified name of this instance.
	 *
	 * \param n the name of the child
	 */
	Instance* get_child(const std::string& n) const;
	
	/**
	 * Returns the number of children.
	 */
	size_t get_child_count() const;
	
	/**
	 * Returns the parent of this instance.
	 */
	Instance* get_parent() const;
	
	/**
	 * Sets the transform (position, rotation and scale) to this instance.
	 *
	 * \param p position vector
	 * \param r rotation angles
	 * \param s scale vector
	 */
	void set_transform(const Vec3& p, const Euler& r, const Vec3& s);
	
	/**
	 * Updates the local transform matrix.
	 * This function is equivalent to "matrix_local = transform();".
	 */
	void update_matrix_local();
	
	/**
	 * Updates the global transform matrix.
	 * This function is equivalent to "matrix_global = transform_global();".
	 */
	void update_matrix_global();
	
	/**
	 * Converts the vector from the global space to the local space.
	 * This function only works when matrix_global is prepared.
	 *
	 * \param v vector
	 */
	Vec3 global_to_local(const Vec3& v) const;
	
	/**
	 * Converts the vector from the local space to the global space.
	 * This function only works when matrix_global is prepared.
	 *
	 * \param v vector
	 */
	Vec3 local_to_global(const Vec3& v) const;
	
	/**
	 * Returns a transform matrix in the local space, calculating from the
	 * position, rotation, and scale.
	 */
	Mat4 transform() const;
	
	/**
	 * Returns a transform matrix in the global space, by multiplying the
	 * transform matrices of its ancestors.
	 */
	Mat4 transform_global() const;
	
	/**
	 * Returns a transform matrix calculating from the specified position,
	 * rotation and scale.
	 *
	 * \param p position vector
	 * \param r rotation angles
	 * \param s scale vector
	 */
	static Mat4 transform(const Vec3& p, const Euler& r, const Vec3& s);
	
protected:
	Instance* parent = nullptr;
	
	std::vector<Instance*> children;
};

}
