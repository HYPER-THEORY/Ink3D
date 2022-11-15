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

#include "Viewer.h"

namespace Ink {

Viewer::Viewer(const Camera& c, float s) : camera(c), speed(s) {}

void Viewer::update(float dt) {
	/* receive keyboard event */
	Vec3 move = {0, 0, 0};
	if (Window::is_down(key_up)) ++move.z;
	if (Window::is_down(key_down)) --move.z;
	if (Window::is_down(key_left)) --move.x;
	if (Window::is_down(key_right)) ++move.x;
	if (move.magnitude() != 0) move = speed * dt * move.normalize();
	
	/* get the center of window */
	auto size = Window::get_size();
	Vec2 center = Vec2(size.first, size.second) * .5;
	
	/* get the position of window's cursor */
	auto cursor_position = Window::get_cursor_position();
	Vec2 cursor = Vec2(cursor_position.first, cursor_position.second);
	Vec2 delta = center - cursor;
	
	/* update angle along Y- and Z-axis */
	axis_y += delta.x * sensitivity;
	axis_z += delta.y * sensitivity;
	if (axis_z > PI_2) axis_z = PI_2;
	if (axis_z < -PI_2) axis_z = -PI_2;
	
	/* update the viewing direction of camera */
	camera.direction.x = sinf(axis_y) * cosf(axis_z);
	camera.direction.y = sinf(axis_z);
	camera.direction.z = cosf(axis_y) * cosf(axis_z);
	
	/* update the view-up vector of camera */
	camera.up.x = -sinf(axis_y) * sinf(axis_z);
	camera.up.y = cosf(axis_z);
	camera.up.z = -cosf(axis_y) * sinf(axis_z);
	
	/* update the movement of camera */
	if (mode == VIEWER_WALK) {
		camera.position.x += move.z * sinf(axis_y) - move.x * cosf(axis_y);
		camera.position.z += move.z * cosf(axis_y) + move.x * sinf(axis_y);
	} else if (mode == VIEWER_FLY) {
		camera.position += move.z * camera.direction;
		camera.position += move.x * (camera.direction.cross(camera.up));
	}
	camera.lookat(camera.position, -camera.direction, camera.up);
}

const Camera& Viewer::get_camera() const {
	return camera;
}

void Viewer::set_camera(const Camera& c) {
	camera = c;
	set_direction(c.direction);
}

void Viewer::set_position(const Vec3& p) {
	camera.position = p;
}

void Viewer::set_direction(const Vec3& d) {
	Vec3 direction = -d.normalize();
	axis_z = asinf(direction.y);
	axis_y = asinf(direction.x / cosf(axis_z));
	if (std::isnan(axis_y)) axis_y = 0;
	if (cosf(axis_y) * direction.z < 0) axis_y = -axis_y + PI;
}

}
