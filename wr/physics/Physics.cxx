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

#include "Physics.h"

namespace WR {

Cuboid::Cuboid() : v1(Vec3()), v2(Vec3()) {}

Cuboid::Cuboid(const Vec3& v1, const Vec3& v2) : v1(v1), v2(v2) {}

void Cuboid::set(const Vec3& v, float w, float h, float d) {
	v1 = v;
	v2.x = v.x + w;
	v2.y = v.y + h;
	v2.z = v.z + d;
}

bool Cuboid::contain(const Vec3& v) const {
	return v1.x < v.x && v.x < v2.x && v1.y < v.y && v.y < v2.y && v1.z < v.z && v.z < v2.z;
}

bool Cuboid::contain(float x, float y, float z) const {
	return v1.x < x && x < v2.x && v1.y < y && y < v2.y && v1.z < z && z < v2.z;
}

bool Cuboid::hittest(const Cuboid& c) {
	return contain(c.v1.x, c.v1.y, c.v1.z) || contain(c.v1.x, c.v1.y, c.v2.z) ||
		contain(c.v1.x, c.v2.y, c.v1.z) || contain(c.v1.x, c.v2.y, c.v2.z) ||
		contain(c.v2.x, c.v1.y, c.v1.z) || contain(c.v2.x, c.v1.y, c.v2.z) ||
		contain(c.v2.x, c.v2.y, c.v1.z) || contain(c.v2.x, c.v2.y, c.v2.z) ||
		c.contain(v1.x, v1.y, v1.z) || c.contain(v1.x, v1.y, v2.z) ||
		c.contain(v1.x, v2.y, v1.z) || c.contain(v1.x, v2.y, v2.z) ||
		c.contain(v2.x, v1.y, v1.z) || c.contain(v2.x, v1.y, v2.z) ||
		c.contain(v2.x, v2.y, v1.z) || c.contain(v2.x, v2.y, v2.z);
}

Solid::Solid() {
	valid = world.add(this);
}

Solid::Solid(const Vec3& p, float w, float h, float d) : position(p), width(w), height(h), depth(d) {
	valid = world.add(this);
	refresh();
}

Solid::~Solid() {
	*valid = false;
}

void Solid::refresh() {
	box.set(position, width, height, depth);
}

void Solid::collide(Solid* s, float x, float y, float z) {
	Cuboid& collided = s->box;
	if (s != this && box.hittest(collided)) {
		if (x != 0) position.x = x > 0 ? collided.v1.x - width - buffer : collided.v2.x + buffer;
		if (y != 0) position.y = y > 0 ? collided.v1.y - height - buffer : collided.v2.y + buffer;
		if (z != 0) position.z = z > 0 ? collided.v1.z - depth - buffer : collided.v2.z + buffer;
		refresh();
	}
}

void Solid::move(const Vec3& d) {
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

float Solid::buffer = 1e-4;
List<Solid*> Solid::world;

Fragility::Fragility(const Vec3& p, float w, float h, float d) : position(p), width(w), height(h), depth(d) {
	refresh();
}

void Fragility::refresh() {
	box.set(position, width, height, depth);
}

void Fragility::update() {
	Solid::world.remove();
	for (size_t i = Solid::world.size(); i -- > 0;) {
		if (box.hittest(Solid::world[i]->box) && hit) hit(*Solid::world[i]);
	}
}

}
