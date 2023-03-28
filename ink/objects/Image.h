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

#include <vector>

#include "Enums.h"

#include "../math/Vector.h"

namespace Ink {

class Image {
public:
	int width = 0;                /**< the width of image */
	int height = 0;               /**< the height of image */
	int channel = 0;              /**< the channel of image */
	int bytes = 1;                /**< the bytes of image */
	
	std::vector<uint8_t> data;    /**< the data of image */
	
	/**
	 * Creates a new Image object.
	 */
	explicit Image() = default;
	
	/**
	 * Creates a new Image object and initializes it with the size, channel and
	 * bytes.
	 *
	 * \param w the width of image
	 * \param h the height of image
	 * \param c the channel of image
	 * \param b the bytes of image
	 */
	explicit Image(int w, int h, int c, int b = 1);
	
	/**
	 * Returns a sub-image sliced of the current image. The new image region is
	 * set from (x1, y1) to (x2, y2).
	 *
	 * \param x1 the x-coordinate of left upper corner
	 * \param y1 the y-coordinate of left upper corner
	 * \param x2 the x-coordinate of right lower corner
	 * \param y2 the y-coordinate of right lower corner
	 */
	Image subimage(int x1, int y1, int x2, int y2);
	
	/**
	 * Flips the image vertically.
	 */
	void flip_vertical();
	
	/**
	 * Flips the image horizontally. Notes that this function is slower than
	 * flip_vertical.
	 */
	void flip_horizontal();
	
	/**
	 * Returns a vector of the images split by channel.
	 */
	std::vector<Image> split() const;
	
	/**
	 * Converts colors in the image from one color space to another color space.
	 *
	 * \param c color conversion
	 */
	void convert(ColorConversion c);
	
	/**
	 * Converts colors in the image from RGB color space to BGR color space.
	 */
	template <typename Type>
	void convert_rgb_to_bgr();
	
	/**
	 * Converts colors in the image from BGR color space to RGB color space.
	 */
	template <typename Type>
	void convert_bgr_to_rgb();
	
	/**
	 * Converts colors in the image from RGB color space to SRGB color space.
	 */
	template <typename Type>
	void convert_rgb_to_srgb();
	
	/**
	 * Converts colors in the image from SRGB color space to RGB color space.
	 */
	template <typename Type>
	void convert_srgb_to_rgb();
	
	/**
	 * Converts colors in the image from RGB color space to XYZ color space.
	 */
	template <typename Type>
	void convert_rgb_to_xyz();
	
	/**
	 * Converts colors in the image from XYZ color space to RGB color space.
	 */
	template <typename Type>
	void convert_xyz_to_rgb();
	
	/**
	 * Converts colors in the image from RGB color space to HSV color space.
	 */
	template <typename Type>
	void convert_rgb_to_hsv();
	
	/**
	 * Converts colors in the image from HSV color space to RGB color space.
	 */
	template <typename Type>
	void convert_hsv_to_rgb();
	
	/**
	 * Converts colors in the image from RGB color space to HSL color space.
	 */
	template <typename Type>
	void convert_rgb_to_hsl();
	
	/**
	 * Converts colors in the image from HSL color space to RGB color space.
	 */
	template <typename Type>
	void convert_hsl_to_rgb();
	
	/**
	 * Converts colors in the image from RGB color space to HCY color space.
	 */
	template <typename Type>
	void convert_rgb_to_hcy();
	
	/**
	 * Converts colors in the image from HCY color space to RGB color space.
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
