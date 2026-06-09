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

#include <vector>

#include "Enums.h"

#include "../math/Vector.h"

namespace ink {

class Image {
public:
	int width = 0;                /**< the width in pixels */
	int height = 0;               /**< the height in pixels */
	int channel = 0;              /**< the channel per pixel */
	int bytes = 1;                /**< the bytes per channel */
	
	std::vector<uint8_t> data;    /**< the data source */
	
	/**
	 * Creates a new Image object.
	 */
	Image() = default;
	
	/**
	 * Creates a new Image object and initializes it with size, channel, and
	 * bytes.
	 *
	 * \param w the width in pixels
	 * \param h the height in pixels
	 * \param c the channel per pixel
	 * \param b the bytes per channel
	 */
	Image(int w, int h, int c, int b = 1);
	
	/**
	 * Returns a sub-image sliced from the current image. The new image region
	 * spans from (x1, y1) to (x2, y2).
	 *
	 * \param x1 the X-coordinate of the upper left corner
	 * \param y1 the Y-coordinate of the upper left corner
	 * \param x2 the X-coordinate of the lower right corner
	 * \param y2 the Y-coordinate of the lower right corner
	 */
	Image subimage(int x1, int y1, int x2, int y2) const;
	
	/**
	 * Flips the image vertically.
	 */
	void flip_vertical();
	
	/**
	 * Flips the image horizontally.
	 */
	void flip_horizontal();
	
	/**
	 * Returns an image list split by channel.
	 */
	std::vector<Image> split() const;
	
	/**
	 * Converts this image from one color space to another color space.
	 *
	 * \param c color conversion
	 */
	void convert(ColorConversion c);
	
	/**
	 * Converts this image from the RGB color space to the BGR color space.
	 */
	template <typename Type>
	void convert_rgb_to_bgr();
	
	/**
	 * Converts this image from the BGR color space to the RGB color space.
	 */
	template <typename Type>
	void convert_bgr_to_rgb();
	
	/**
	 * Converts this image from the RGB color space to the sRGB color space.
	 */
	template <typename Type>
	void convert_rgb_to_srgb();
	
	/**
	 * Converts this image from the sRGB color space to the RGB color space.
	 */
	template <typename Type>
	void convert_srgb_to_rgb();
	
	/**
	 * Converts this image from the RGB color space to the XYZ color space.
	 */
	template <typename Type>
	void convert_rgb_to_xyz();
	
	/**
	 * Converts this image from the XYZ color space to the RGB color space.
	 */
	template <typename Type>
	void convert_xyz_to_rgb();
	
	/**
	 * Converts this image from the RGB color space to the HSV color space.
	 */
	template <typename Type>
	void convert_rgb_to_hsv();
	
	/**
	 * Converts this image from the HSV color space to the RGB color space.
	 */
	template <typename Type>
	void convert_hsv_to_rgb();
	
	/**
	 * Converts this image from the RGB color space to the HSL color space.
	 */
	template <typename Type>
	void convert_rgb_to_hsl();
	
	/**
	 * Converts this image from the HSL color space to the RGB color space.
	 */
	template <typename Type>
	void convert_hsl_to_rgb();
};

}
