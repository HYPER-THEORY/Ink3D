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

#include "physics.h"

cuboid::cuboid() : v1(vec3()), v2(vec3()) {}

cuboid::cuboid(const vec3& v1, const vec3& v2) : v1(v1), v2(v2) {}

void cuboid::set(const vec3& v, float w, float h, float d) {
	v1 = v;
	v2.x = v.x + w;
	v2.y = v.y + h;
	v2.z = v.z + d;
}

bool cuboid::contain(const vec3& v) const {
	return v1.x < v.x && v.x < v2.x && v1.y < v.y && v.y < v2.y && v1.z < v.z && v.z < v2.z;
}

bool cuboid::contain(float x, float y, float z) const {
	return v1.x < x && x < v2.x && v1.y < y && y < v2.y && v1.z < z && z < v2.z;
}

bool cuboid::hittest(const cuboid& c) {
	return contain(c.v1.x, c.v1.y, c.v1.z) || contain(c.v1.x, c.v1.y, c.v2.z) ||
		contain(c.v1.x, c.v2.y, c.v1.z) || contain(c.v1.x, c.v2.y, c.v2.z) ||
		contain(c.v2.x, c.v1.y, c.v1.z) || contain(c.v2.x, c.v1.y, c.v2.z) ||
		contain(c.v2.x, c.v2.y, c.v1.z) || contain(c.v2.x, c.v2.y, c.v2.z) ||
		c.contain(v1.x, v1.y, v1.z) || c.contain(v1.x, v1.y, v2.z) ||
		c.contain(v1.x, v2.y, v1.z) || c.contain(v1.x, v2.y, v2.z) ||
		c.contain(v2.x, v1.y, v1.z) || c.contain(v2.x, v1.y, v2.z) ||
		c.contain(v2.x, v2.y, v1.z) || c.contain(v2.x, v2.y, v2.z);
}

solid::solid() {
	valid = world.add(this);
}

solid::solid(const vec3& p, float w, float h, float d) : position(p), width(w), height(h), depth(d) {
	valid = world.add(this);
	refresh();
}

solid::~solid() {
	*valid = false;
}

void solid::refresh() {
	box.set(position, width, height, depth);
}

void solid::collide(solid* s, float x, float y, float z) {
	cuboid& collided = s->box;
	if (s != this && box.hittest(collided)) {
		if (x != 0) position.x = x > 0 ? collided.v1.x - width - buffer : collided.v2.x + buffer;
		if (y != 0) position.y = y > 0 ? collided.v1.y - height - buffer : collided.v2.y + buffer;
		if (z != 0) position.z = z > 0 ? collided.v1.z - depth - buffer : collided.v2.z + buffer;
		refresh();
	}
}

void solid::move(const vec3& d) {
	world.remove();
	position.x += d.x;
	refresh();
	for (size_t i = world.size(); i --> 0;) {
		collide(world[i], d.x, 0, 0);
	}
	position.y += d.y;
	refresh();
	for (size_t i = world.size(); i --> 0;) {
		collide(world[i], 0, d.y, 0);
	}
	position.z += d.z;
	refresh();
	for (size_t i = world.size(); i --> 0;) {
		collide(world[i], 0, 0, d.z);
	}
}

float solid::buffer = 1e-4;
list<solid*> solid::world;

fragility::fragility(const vec3& p, float w, float h, float d) : position(p), width(w), height(h), depth(d) {
	refresh();
}

void fragility::refresh() {
	box.set(position, width, height, depth);
}

void fragility::update() {
	solid::world.remove();
	for (size_t i = solid::world.size(); i --> 0;) {
		if (box.hittest(solid::world[i]->box) && hit) hit(*solid::world[i]);
	}
}
