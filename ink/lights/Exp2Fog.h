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

#include "../math/Maths.h"

namespace Ink {

class Exp2Fog {
public:
	bool visible = true;       /**< whether the fog is visible */
	Vec3 color = {1, 1, 1};    /**< the color of fog */
	float near = 1;            /**< the nearest distance of fog */
	float density = 0.001;     /**< the density of fog */
	
	/**
	 * Creates a new Exp2Fog object and initializes it with color, distance and
	 * density.
	 *
	 * \param c the color of fog
	 * \param n the nearest distance of fog
	 * \param d the density of fog
	 */
	explicit Exp2Fog(const Vec3& c = {1, 1, 1}, float n = 1, float d = 0.001);
};

}
