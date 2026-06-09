/**
 * Copyright (C) 2021-2023 HYPERTHEORY
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

#include "Vector.h"

namespace ink {

class Color {
public:
	/**
	 * Converts the specified color from the hexadecimal color code to the RGB
	 * values.
	 *
	 * \param c color
	 */
	static Vec3 hex_to_rgb(unsigned int c);
	
	/**
	 * Converts the specified color from the RGB values to the hexadecimal color
	 * code.
	 *
	 * \param c color
	 */
	static unsigned int rgb_to_hex(const Vec3& c);
	
	/**
	 * Converts the specified color from the RGB color space to the sRGB color
	 * space.
	 *
	 * \param c color
	 */
	static Vec3 rgb_to_srgb(const Vec3& c);
	
	/**
	 * Converts the specified color from the sRGB color space to the RGB color
	 * space.
	 *
	 * \param c color
	 */
	static Vec3 srgb_to_rgb(const Vec3& c);
	
	/**
	 * Converts the specified color from the RGB color space to the XYZ color
	 * space.
	 *
	 * \param c color
	 */
	static Vec3 rgb_to_xyz(const Vec3& c);
	
	/**
	 * Converts the specified color from the XYZ color space to the RGB color
	 * space.
	 *
	 * \param c color
	 */
	static Vec3 xyz_to_rgb(const Vec3& c);
	
	/**
	 * Converts the specified color from the RGB color space to the HSV color
	 * space.
	 *
	 * \param c color
	 */
	static Vec3 rgb_to_hsv(const Vec3& c);
	
	/**
	 * Converts the specified color from the HSV color space to the RGB color
	 * space.
	 *
	 * \param c color
	 */
	static Vec3 hsv_to_rgb(const Vec3& c);
	
	/**
	 * Converts the specified color from the RGB color space to the HSL color
	 * space.
	 *
	 * \param c color
	 */
	static Vec3 rgb_to_hsl(const Vec3& c);
	
	/**
	 * Converts the specified color from the HSL color space to the RGB color
	 * space.
	 *
	 * \param c color
	 */
	static Vec3 hsl_to_rgb(const Vec3& c);
};

}
