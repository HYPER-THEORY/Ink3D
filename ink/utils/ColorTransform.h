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

#include "../math/Vector.h"

namespace Ink {

class ColorTransform {
public:
	/**
	 * Transforms from hex color code to RGB color.
	 *
	 * \param c hex color code
	 */
	static Vec3 to_rgb(unsigned int c);
	
	/**
	 * Transforms from hex color code to RGBA color.
	 *
	 * \param c hex color code
	 */
	static Vec4 to_rgba(unsigned int c);
	
	/**
	 * Transforms from RGB color to hex color code.
	 *
	 * \param c RGB color
	 */
	static unsigned int to_hex(const Vec3& c);
	
	/**
	 * Transforms from RGBA color to hex color code.
	 *
	 * \param c RGBA color
	 */
	static unsigned int to_hex(const Vec4& c);
};

}
