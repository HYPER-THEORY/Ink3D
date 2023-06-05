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

#include "Vector.h"

namespace Ink {

class Color {
public:
	/**
	 * Converts the specified color from RGB color space to SRGB color space.
	 *
	 * \param c color
	 */
	static Vec3 rgb_to_srgb(const Vec3& c);
	
	/**
	 * Converts the specified color from SRGB color space to RGB color space.
	 *
	 * \param c color
	 */
	static Vec3 srgb_to_rgb(const Vec3& c);
	
	/**
	 * Converts the specified color from RGB color space to XYZ color space.
	 *
	 * \param c color
	 */
	static Vec3 rgb_to_xyz(const Vec3& c);
	
	/**
	 * Converts the specified color from XYZ color space to RGB color space.
	 *
	 * \param c color
	 */
	static Vec3 xyz_to_rgb(const Vec3& c);
	
	/**
	 * Converts the specified color from RGB color space to HSV color space.
	 *
	 * \param c color
	 */
	static Vec3 rgb_to_hsv(const Vec3& c);
	
	/**
	 * Converts the specified color from HSV color space to RGB color space.
	 *
	 * \param c color
	 */
	static Vec3 hsv_to_rgb(const Vec3& c);
	
	/**
	 * Converts the specified color from RGB color space to HSL color space.
	 *
	 * \param c color
	 */
	static Vec3 rgb_to_hsl(const Vec3& c);
	
	/**
	 * Converts the specified color from HSL color space to RGB color space.
	 *
	 * \param c color
	 */
	static Vec3 hsl_to_rgb(const Vec3& c);
	
	/**
	 * Converts the specified color from RGB color space to HCY color space.
	 *
	 * \param c color
	 */
	static Vec3 rgb_to_hcy(const Vec3& c);
	
	/**
	 * Converts the specified color from HCY color space to RGB color space.
	 *
	 * \param c color
	 */
	static Vec3 hcy_to_rgb(const Vec3& c);
	
private:
	static inline float saturate(float v);
	
	static inline float rgb_to_srgb(float v);
	
	static inline float srgb_to_rgb(float v);
	
	static inline Vec3 hue_to_rgb(float h);
	
	static inline Vec3 rgb_to_hcv(const Vec3& c);
};

}
