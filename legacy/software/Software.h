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

#pragma once

#include "../objects/Image.h"
#include "../objects/Mesh.h"
#include "../objects/Instance.h"
#include "../camera/Camera.h"

namespace Ink::Legacy::Soft {

struct State {
	int viewport_x = 0;         /**< the x-coordinate of the viewport */
	int viewport_y = 0;         /**< the y-coordinate of the viewport */
	int viewport_width = 0;     /**< the width of the viewport */
	int viewport_height = 0;    /**< the height of the viewport */
};

struct PointList {
	int size = 0;        /**< the size of point list */
	Vec4 vertices[4];    /**< the vertices of point list */
};

/**
 * Clears the depth buffer to the specified clear value.
 *
 * \param b Z-Buffer
 * \param d clear depth
 */
void clear(Image& b, float d = 1);

/**
 * Clips the point list at the near clip plane.
 *
 * \param i input point list
 * \param o output point list
 */
void clip_near_plane(const PointList& i, PointList& o);

/**
 * Rasterizes the triangles in pointlist with depth test. The results will be
 * writen to Z-Buffer.
 *
 * \param s state
 * \param v vertex list
 * \param n vertex number
 * \param b Z-Buffer
 */
void rasterize(const State& s, const Vec4* v, int n, Image& b);

/**
 * Renders the instance using a camera. The results will be writen to Z-Buffer.
 *
 * \param s state
 * \param i instance
 * \param c camera
 * \param b Z-Buffer
 */
void render(const State& s, const Instance& i, const Camera& c, Image& b);

}
