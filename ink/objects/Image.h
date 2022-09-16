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

#include <vector>

#include "../math/Maths.h"

namespace Ink {

enum ColorConversion {
	COLOR_RGB_TO_BGR,
	COLOR_BGR_TO_RGB,
	COLOR_SRGB_TO_RGB,
	COLOR_RGB_TO_SRGB,
	COLOR_RGB_TO_XYZ,
	COLOR_XYZ_TO_RGB,
	COLOR_RGB_TO_HSV,
	COLOR_HSV_TO_RGB,
	COLOR_RGB_TO_HSL,
	COLOR_HSL_TO_RGB,
	COLOR_RGB_TO_HCY,
	COLOR_HCY_TO_RGB,
};

class Image {
public:
	int width = 0;                /**< the width of image */
	int height = 0;               /**< the height of image */
	int channel = 0;              /**< the channel of image */
	int bytes = 1;                /**< the bytes of image */
	
	std::vector<uint8_t> data;    /**< the data of image */
	
	/**
	 * Create a new Image.
	 */
	Image() = default;
	
	/**
	 * Create a new Image with its size, channel and bytes.
	 *
	 * \param w the width of image
	 * \param h the height of image
	 * \param c the channel of image
	 * \param b the bytes of image
	 */
	Image(int w, int h, int c, int b = 1);
	
	/**
	 * Returns a sub-image sliced of the current image.
	 *
	 * \param x1 the x-coordinate of left upper corner
	 * \param y1 the y-coordinate of left upper corner
	 * \param x2 the x-coordinate of right lower corner
	 * \param y2 the y-coordinate of right lower corner
	 */
	Image subimage(int x1, int y1, int x2, int y2);
	
	/**
	 * Filp image vertically.
	 */
	void flip_vertical();
	
	/**
	 * Filp image horizontally.
	 */
	void flip_horizontal();
	
	/**
	 * Returns a list of image splitted by channel.
	 */
	std::vector<Image> split() const;
	
	/**
	 * Convert from one color space to another.
	 *
	 * \param c color conversion
	 */
	void convert(int c);
	
	/**
	 * Convert from RGB color space to BGR color space.
	 */
	template <typename Type>
	void convert_rgb_to_bgr();
	
	/**
	 * Convert from BGR color space to RGB color space.
	 */
	template <typename Type>
	void convert_bgr_to_rgb();
	
	/**
	 * Convert from RGB color space to SRGB color space.
	 */
	template <typename Type>
	void convert_rgb_to_srgb();
	
	/**
	 * Convert from SRGB color space to RGB color space.
	 */
	template <typename Type>
	void convert_srgb_to_rgb();
	
	/**
	 * Convert from RGB color space to XYZ color space.
	 */
	template <typename Type>
	void convert_rgb_to_xyz();
	
	/**
	 * Convert from XYZ color space to RGB color space.
	 */
	template <typename Type>
	void convert_xyz_to_rgb();
	
	/**
	 * Convert from RGB color space to HSV color space.
	 */
	template <typename Type>
	void convert_rgb_to_hsv();
	
	/**
	 * Convert from HSV color space to RGB color space.
	 */
	template <typename Type>
	void convert_hsv_to_rgb();
	
	/**
	 * Convert from RGB color space to HSL color space.
	 */
	template <typename Type>
	void convert_rgb_to_hsl();
	
	/**
	 * Convert from HSL color space to RGB color space.
	 */
	template <typename Type>
	void convert_hsl_to_rgb();
	
	/**
	 * Convert from RGB color space to HCY color space.
	 */
	template <typename Type>
	void convert_rgb_to_hcy();
	
	/**
	 * Convert from HCY color space to RGB color space.
	 */
	template <typename Type>
	void convert_hcy_to_rgb();
	
private:
	template <typename Type>
	static inline float unpack(Type v);
	
	template <typename Type>
	static inline Type pack(float v);
	
	static inline float saturate(float v);
	
	static inline Vec3 rgb_to_hcv(float r, float g, float b);
	
	static inline Vec3 hue_to_rgb(float h);
};

}
