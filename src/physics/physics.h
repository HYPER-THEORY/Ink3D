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

#include <vector>

#include "../math/maths.h"

namespace phys {

template <typename t>
struct listed {
	t object;
	bool valid;
};

template <typename t>
class list {
public:
	std::vector<listed<t> > objects;
	
	/**
	 * Add object to list.
	 *
	 * \param o object
	 */
	bool* add(t o);
	
	/**
	 * Remove invalid objects.
	 */
	void remove();
	
	/**
	 * Returns the size of list.
	 */
	size_t size() const;
	
	const t& operator[](size_t k) const;
};

class cuboid {
public:
	vec3 v1;		/* vertex 1 */
	vec3 v2;		/* vertex 2 */
	
	/**
	 * Create a new cuboid.
	 */
	cuboid();
	
	/**
	 * Create a new cuboid.
	 *
	 * \param v1 upper-left-front vertex
	 * \param v2 lower-right-back vertex
	 */
	cuboid(const vec3& v1, const vec3& v2);
	
	/**
	 * Set cuboid with position and size.
	 *
	 * \param v center point
	 * \param w width of cuboid
	 * \param h height of cuboid
	 * \param d depth of cuboid
	 */
	void set(const vec3& v, float w, float h, float d);
	
	/**
	 * Determines whether the point is contained within the cuboid region.
	 *
	 * \param v point
	 */
	bool contain(const vec3& v) const;
	
	/**
	 * Determines whether the point is contained within the cuboid region.
	 *
	 * \param x x-coordinate of point
	 * \param y y-coordinate of point
	 * \param z z-coordinate of point
	 */
	bool contain(float x, float y, float z) const;
	
	/**
	 * Determines whether the cuboid is hitted by another cuboid.
	 *
	 * \param c cuboid
	 */
	bool hittest(const cuboid& c);
};

class solid {
public:
	cuboid box;					/* collision box */
	vec3 position;				/* position of box */
	float width = 0;			/* width of box */
	float height = 0;			/* height of box */
	float depth = 0;			/* depth of box */
	bool* valid = nullptr;		/* whether solid is valid */
	
	static list<solid*> world;
	
	/**
	 * Create a new solid.
	 */
	solid();
	
	/**
	 * Create a new solid.
	 *
	 * \param p center point
	 * \param w width of cuboid
	 * \param h height of cuboid
	 * \param d depth of cuboid
	 */
	solid(const vec3& p, float w, float h, float d);
	
	~solid();
	
	/**
	 * Refresh the collision box with updated position and size.
	 */
	void refresh();
	
	/**
	 * Move cuboid in one direction and accpet the collisions.
	 *
	 * \param s soild
	 * \param x x-axis direction
	 * \param y y-axis direction
	 * \param z z-axis direction
	 */
	void collide(solid* s, float x, float y, float z);
	
	/**
	 * Move cuboid in the specified direction.
	 *
	 * \param d moving direction
	 */
	void move(const vec3& d);
	
private:
	static float buffer;
};

class fragility {
public:
	using hit_callback = void (*)(const solid&);
	
	cuboid box;						/* cullision box */
	vec3 position;					/* position of box */
	float width = 0;				/* width of box */
	float height = 0;				/* height of box */
	float depth = 0;				/* depth of box */
	hit_callback hit = nullptr;		/* collision callback */
	
	/**
	 * Create a new fragility.
	 */
	fragility() = default;
	
	/**
	 * Create a new fragility.
	 *
	 * \param p center point
	 * \param w width of cuboid
	 * \param h height of cuboid
	 * \param d depth of cuboid
	 */
	fragility(const vec3& p, float w, float h, float d);
	
	/**
	 * Refresh the collision box with updated position and size.
	 */
	void refresh();
	
	/**
	 * Update the status whether the fragility is hitted by solids.
	 */
	void update();
};

/* template implementations */

template <typename t>
bool* list<t>::add(t o) {
	objects.push_back({o, true});
	return &objects.back().valid;
}

template <typename t>
void list<t>::remove() {
	for (auto o = objects.begin(); o != objects.end();) {
		o->valid ? ++o : o = objects.erase(o);
	}
}

template <typename t>
size_t list<t>::size() const {
	return objects.size();
}

template <typename t>
const t& list<t>::operator[](size_t k) const {
	return objects[k].object;
}

}
