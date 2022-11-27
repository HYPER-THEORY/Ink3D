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

#include "Geom.h"

namespace WR {

void Geom::init() {
	/* init cube_object */
	Mesh cube_mesh;
	cube_mesh.material_groups = {{"default", 0, 36}};
	cube_mesh.vertex = {
		{-1,  1, -1}, {-1, -1, -1}, { 1, -1, -1},
		{ 1, -1, -1}, { 1,  1, -1}, {-1,  1, -1}, /* back side */
		{-1, -1,  1}, {-1, -1, -1}, {-1,  1, -1},
		{-1,  1, -1}, {-1,  1,  1}, {-1, -1,  1}, /* left side */
		{ 1, -1, -1}, { 1, -1,  1}, { 1,  1,  1},
		{ 1,  1,  1}, { 1,  1, -1}, { 1, -1, -1}, /* right side */
		{-1, -1,  1}, {-1,  1,  1}, { 1,  1,  1},
		{ 1,  1,  1}, { 1, -1,  1}, {-1, -1,  1}, /* front side */
		{-1,  1, -1}, { 1,  1, -1}, { 1,  1,  1},
		{ 1,  1,  1}, {-1,  1,  1}, {-1,  1, -1}, /* upper side */
		{-1, -1, -1}, {-1, -1,  1}, { 1, -1, -1},
		{ 1, -1, -1}, {-1, -1,  1}, { 1, -1,  1}, /* lower side */
	};
	cube_object = std::make_unique<Gpu::VertexObject>();
	cube_object->preload(cube_mesh);
	/* init square_object */
	Mesh square_mesh;
	square_mesh.material_groups = {{"default", 0, 6}};
	square_mesh.vertex = {
		{ 1,  1, 0}, {-1,  1, 0}, { 1, -1, 0},
		{-1, -1, 0}, { 1, -1, 0}, {-1,  1, 0},
	};
	square_mesh.uv = {
		{1, 1}, {0, 1}, {1, 0},
		{0, 0}, {1, 0}, {0, 1},
	};
	square_object = std::make_unique<Gpu::VertexObject>();
	square_object->preload(square_mesh);
}

const Gpu::VertexObject& Geom::cube() {
	return *cube_object;
}

const Gpu::VertexObject& Geom::square() {
	return *square_object;
}

std::unique_ptr<Gpu::VertexObject> Geom::cube_object;
std::unique_ptr<Gpu::VertexObject> Geom::square_object;

}
