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

#include "Physics.h"

namespace Ink::Legacy {

CollisionBox::CollisionBox(const Vec3& v1, const Vec3& v2) :
v1(v1), v2(v2) {}

void CollisionBox::set(const Vec3& v, float w, float h, float d) {
	v1 = v;
	v2.x = v.x + w;
	v2.y = v.y + h;
	v2.z = v.z + d;
}

bool CollisionBox::contain(const Vec3& v) const {
	return v1.x < v.x && v.x < v2.x && v1.y < v.y &&
		v.y < v2.y && v1.z < v.z && v.z < v2.z;
}

bool CollisionBox::contain(float x, float y, float z) const {
	return v1.x < x && x < v2.x && v1.y < y &&
		y < v2.y && v1.z < z && z < v2.z;
}

bool CollisionBox::hittest(const CollisionBox& b) const {
	return contain(b.v1.x, b.v1.y, b.v1.z) ||
		contain(b.v1.x, b.v1.y, b.v2.z) ||
		contain(b.v1.x, b.v2.y, b.v1.z) ||
		contain(b.v1.x, b.v2.y, b.v2.z) ||
		contain(b.v2.x, b.v1.y, b.v1.z) ||
		contain(b.v2.x, b.v1.y, b.v2.z) ||
		contain(b.v2.x, b.v2.y, b.v1.z) ||
		contain(b.v2.x, b.v2.y, b.v2.z) ||
		b.contain(v1.x, v1.y, v1.z) ||
		b.contain(v1.x, v1.y, v2.z) ||
		b.contain(v1.x, v2.y, v1.z) ||
		b.contain(v1.x, v2.y, v2.z) ||
		b.contain(v2.x, v1.y, v1.z) ||
		b.contain(v2.x, v1.y, v2.z) ||
		b.contain(v2.x, v2.y, v1.z) ||
		b.contain(v2.x, v2.y, v2.z);
}

Solid::Solid(const Vec3& p, float w, float h, float d) :
	position(p), width(w), height(h), depth(d) {
	refresh();
}

void Solid::activate() {
	world.insert(this);
}

void Solid::deactivate() {
	world.erase(this);
}

void Solid::refresh() {
	box.set(position, width, height, depth);
}

void Solid::collide(Solid* s, float x, float y, float z) {
	CollisionBox& collided = s->box;
	if (s != this && box.hittest(collided)) {
		if (x > 0) {
			position.x = collided.v1.x - width - limit;
		} else if (x < 0) {
			position.x = collided.v2.x + limit;
		}
		if (y > 0) {
			position.y = collided.v1.y - height - limit;
		} else if (y < 0) {
			position.y = collided.v2.y + limit;
		}
		if (z > 0) {
			position.z = collided.v1.z - depth - limit;
		} else if (z < 0) {
			position.z = collided.v2.z + limit;
		}
		refresh();
	}
}

void Solid::move(const Vec3& d) {
	auto world_begin = world.begin();
	auto world_end = world.end();
	position.x += d.x;
	refresh();
	for (auto i = world_begin; i != world_end; ++i) {
		collide(*i, d.x, 0, 0);
	}
	position.y += d.y;
	refresh();
	for (auto i = world_begin; i != world_end; ++i) {
		collide(*i, 0, d.y, 0);
	}
	position.z += d.z;
	refresh();
	for (auto i = world_begin; i != world_end; ++i) {
		collide(*i, 0, 0, d.z);
	}
}

float Solid::limit = 1e-4;

std::unordered_set<Solid*> Solid::world;

}
