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

#include "Camera.h"

namespace Ink {

bool Camera::is_perspective() const {
	return projection[3][2] == -1;
}

void Camera::lookat(const Vec3& p, const Vec3& d, const Vec3& u) {
	position = p;
	direction = d.normalize();
	up = u.normalize();
	Vec3& dn = direction;
	Vec3& un = up;
	Vec3 rn = (-direction).cross(up);
	viewing = {
		rn.x      , rn.y      , rn.z      , -p.dot(rn),
		un.x      , un.y      , un.z      , -p.dot(un),
		dn.x      , dn.y      , dn.z      , -p.dot(dn),
		0         , 0         , 0         , 1         ,
	};
}

}
