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

#include <vector>
#include <unordered_set>

#include "../math/Maths.h"

namespace Ink {

class CollisionBox {
public:
	Vec3 v1;    /**< vertex 1 */
	Vec3 v2;    /**< vertex 2 */
	
	/**
	 * Create a new CollisionBox.
	 */
	CollisionBox() = default;
	
	/**
	 * Create a new CollisionBox.
	 *
	 * \param v1 upper-left-front vertex
	 * \param v2 lower-right-back vertex
	 */
	CollisionBox(const Vec3& v1, const Vec3& v2);
	
	/**
	 * Sets position and size to the box.
	 *
	 * \param v the center of box
	 * \param w the width of box
	 * \param h the height of box
	 * \param d the depth of box
	 */
	void set(const Vec3& v, float w, float h, float d);
	
	/**
	 * Determines whether the point is contained within the box region.
	 *
	 * \param v point
	 */
	bool contain(const Vec3& v) const;
	
	/**
	 * Determines whether the point is contained within the box region.
	 *
	 * \param x the x-coordinate of point
	 * \param y the y-coordinate of point
	 * \param z the z-coordinate of point
	 */
	bool contain(float x, float y, float z) const;
	
	/**
	 * Determines whether the box is hitted by another box.
	 *
	 * \param b collision box
	 */
	bool hittest(const CollisionBox& b);
};

class Solid {
public:
	CollisionBox box;      /**< collision box */
	
	Vec3 position;         /**< the position of box */
	
	float width = 0;       /**< the width of box */
	float height = 0;      /**< the height of box */
	float depth = 0;       /**< the depth of box */
	
	/**
	 * Create a new Solid.
	 */
	Solid() = default;
	
	/**
	 * Create a new Solid.
	 *
	 * \param p center point
	 * \param w the width of cuboid
	 * \param h the height of cuboid
	 * \param d the depth of cuboid
	 */
	Solid(const Vec3& p, float w, float h, float d);
	
	/**
	 * Activate solid.
	 */
	void activate();
	
	/**
	 * Deactivate solid.
	 */
	void deactivate();
	
	/**
	 * Refresh the collision box with updated position and size.
	 */
	void refresh();
	
	/**
	 * Move the collision box in one direction.
	 *
	 * \param s soild
	 * \param x x-axis direction
	 * \param y y-axis direction
	 * \param z z-axis direction
	 */
	void collide(Solid* s, float x, float y, float z);
	
	/**
	 * Move the collision box in the specified direction.
	 *
	 * \param d direction
	 */
	void move(const Vec3& d);
	
private:
	static float limit;
	
	static std::unordered_set<Solid*> world;
};

}