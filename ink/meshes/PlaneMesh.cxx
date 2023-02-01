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

#include "PlaneMesh.h"

namespace Ink {

const std::vector<Vec3> plane_mesh_vertex = {
#include "PlaneMesh.v"
};

const std::vector<Vec3> plane_mesh_normal = {
#include "PlaneMesh.n"
};

const std::vector<Vec2> plane_mesh_uv = {
#include "PlaneMesh.u"
};

Mesh PlaneMesh::create() {
	Mesh mesh = Mesh("Plane");
	mesh.groups = {{"default", 0, 6}};
	mesh.vertex = plane_mesh_vertex;
	mesh.normal = plane_mesh_normal;
	mesh.uv = plane_mesh_uv;
	return mesh;
}

}
