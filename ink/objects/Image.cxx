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

#include "Image.h"

#include "../core/Error.h"
#include "../math/Color.h"

#include <algorithm>

namespace Ink {

Image::Image(int w, int h, int c, int b) :
width(w), height(h), channel(c), bytes(b) {
	data.resize(w * h * c * b);
}

Image Image::subimage(int x1, int y1, int x2, int y2) {
	if (x1 > x2) std::swap(x1, x2);
	if (y1 > y2) std::swap(y1, y2);
	
	/* check whether the sub-image region is legal */
	if (x1 < 0 || x2 > width || y1 < 0 || y2 > height) {
		Error::set("Image", "Illegal slicing region");
	}
	
	/* create a new image */
	Image image = Image(x2 - x1, y2 - y1, channel, bytes);
	
	/* copy image's data to subimage */
	int bpp = channel * bytes;
	int row_bytes = image.width * bpp;
	uint8_t* data_ptr = data.data();
	uint8_t* image_ptr = image.data.data();
	for (int r = 0; r < image.height; ++r) {
		uint8_t* ptr_1 = data_ptr + (x1 + (r + y1) * width) * bpp;
		uint8_t* ptr_2 = image_ptr + r * row_bytes;
		std::copy_n(ptr_1, row_bytes, ptr_2);
	}
	
	return image; /* return the sub-image */
}

void Image::flip_vertical() {
	int bpp = channel * bytes;
	int row_bytes = width * bpp;
	std::vector<uint8_t> temp(row_bytes);
	uint8_t* temp_ptr = temp.data();
	uint8_t* data_ptr = data.data();
	int median = height / 2;
	for (int y = 0; y < median; ++y) {
		uint8_t* ptr_1 = data_ptr + y * row_bytes;
		uint8_t* ptr_2 = data_ptr + (height - y - 1) * row_bytes;
		std::copy_n(ptr_1, row_bytes, temp_ptr);
		std::copy_n(ptr_2, row_bytes, ptr_1);
		std::copy_n(temp_ptr, row_bytes, ptr_2);
	}
}

void Image::flip_horizontal() {
	int bpp = channel * bytes;
	std::vector<uint8_t> temp(bpp);
	uint8_t* temp_ptr = temp.data();
	uint8_t* data_ptr = data.data();
	int median = width / 2;
	for (int x = 0; x < median; ++x) {
		for (int y = 0; y < height; ++y) {
			uint8_t* ptr_1 = data_ptr + (x + y * width) * bpp;
			uint8_t* ptr_2 = data_ptr + (width - x - 1 + y * width) * bpp;
			std::copy_n(ptr_1, bpp, temp_ptr);
			std::copy_n(ptr_2, bpp, ptr_1);
			std::copy_n(temp_ptr, bpp, ptr_2);
		}
	}
}

std::vector<Image> Image::split() const {
	int bpp = channel * bytes;
	std::vector<Image> images(channel);
	std::vector<uint8_t*> image_ptrs(channel);
	for (int i = 0; i < channel; ++i) {
		images[i] = Image(width, height, 1, bytes);
		image_ptrs[i] = images[i].data.data();
	}
	const uint8_t* data_ptr = data.data();
	int pixel = width * height;
	while (pixel --> 0) {
		const uint8_t* ptr_1 = data_ptr + pixel * bpp;
		for (int i = 0; i < channel; ++i) {
			uint8_t* ptr_2 = image_ptrs[i] + pixel * bytes;
			std::copy_n(ptr_1 + i * bytes, bytes, ptr_2);
		}
	}
	return images;
}

void Image::convert(ColorConversion c) {
	/* check the number of channels */
	if (channel != 3 && channel != 4) {
		return Error::set("Image", "Image's channel must be 3 or 4");
	}
	
	/* convert from RGB color space to BGR color space */
	if (c == COLOR_RGB_TO_BGR) {
		if (bytes == 1) {
			convert_rgb_to_bgr<uint8_t>();
		} else {
			convert_rgb_to_bgr<float_t>();
		}
	}
	
	/* convert from BGR color space to RGB color space */
	else if (c == COLOR_BGR_TO_RGB) {
		if (bytes == 1) {
			convert_bgr_to_rgb<uint8_t>();
		} else {
			convert_bgr_to_rgb<float_t>();
		}
	}
	
	/* convert from RGB color space to SRGB color space */
	else if (c == COLOR_RGB_TO_SRGB) {
		if (bytes == 1) {
			convert_rgb_to_srgb<uint8_t>();
		} else {
			convert_rgb_to_srgb<float_t>();
		}
	}
	
	/* convert from SRGB color space to RGB color space */
	else if (c == COLOR_SRGB_TO_RGB) {
		if (bytes == 1) {
			convert_srgb_to_rgb<uint8_t>();
		} else {
			convert_srgb_to_rgb<float_t>();
		}
	}
	
	/* convert from RGB color space to XYZ color space */
	else if (c == COLOR_RGB_TO_XYZ) {
		if (bytes == 1) {
			convert_rgb_to_xyz<uint8_t>();
		} else {
			convert_rgb_to_xyz<float_t>();
		}
	}
	
	/* convert from XYZ color space to RGB color space */
	else if (c == COLOR_XYZ_TO_RGB) {
		if (bytes == 1) {
			convert_xyz_to_rgb<uint8_t>();
		} else {
			convert_xyz_to_rgb<float_t>();
		}
	}
	
	/* convert from RGB color space to HSV color space */
	else if (c == COLOR_RGB_TO_HSV) {
		if (bytes == 1) {
			convert_rgb_to_hsv<uint8_t>();
		} else {
			convert_rgb_to_hsv<float_t>();
		}
	}
	
	/* convert from HSV color space to RGB color space */
	else if (c == COLOR_HSV_TO_RGB) {
		if (bytes == 1) {
			convert_hsv_to_rgb<uint8_t>();
		} else {
			convert_hsv_to_rgb<float_t>();
		}
	}
	
	/* convert from RGB color space to HSL color space */
	else if (c == COLOR_RGB_TO_HSL) {
		if (bytes == 1) {
			convert_rgb_to_hsl<uint8_t>();
		} else {
			convert_rgb_to_hsl<float_t>();
		}
	}
	
	/* convert from HSL color space to RGB color space */
	else if (c == COLOR_HSL_TO_RGB) {
		if (bytes == 1) {
			convert_hsl_to_rgb<uint8_t>();
		} else {
			convert_hsl_to_rgb<float_t>();
		}
	}
	
	/* convert from RGB color space to HCY color space */
	else if (c == COLOR_RGB_TO_HCY) {
		if (bytes == 1) {
			convert_rgb_to_hcy<uint8_t>();
		} else {
			convert_rgb_to_hcy<float_t>();
		}
	}
	
	/* convert from HCY color space to RGB color space */
	else if (c == COLOR_HCY_TO_RGB) {
		if (bytes == 1) {
			convert_hcy_to_rgb<uint8_t>();
		} else {
			convert_hcy_to_rgb<float_t>();
		}
	}
}

template <typename Type>
void Image::convert_rgb_to_bgr() {
	/* get pointers of the specified data type */
	Type* ptr_r = reinterpret_cast<Type*>(data.data());
	Type* ptr_b = ptr_r + 2;
	
	/* swap the red and blue channel */
	int i = width * height;
	while (i --> 0) {
		std::swap(ptr_r[channel * i], ptr_b[channel * i]);
	}
}

template <typename Type>
void Image::convert_bgr_to_rgb() {
	/* get pointers of the specified data type */
	Type* ptr_r = reinterpret_cast<Type*>(data.data());
	Type* ptr_b = ptr_r + 2;
	
	/* swap the red and blue channel */
	int i = width * height;
	while (i --> 0) {
		std::swap(ptr_r[channel * i], ptr_b[channel * i]);
	}
}

template <typename Type>
void Image::convert_rgb_to_srgb() {
	/* get pointer of the specified data type */
	Type* ptr_r = reinterpret_cast<Type*>(data.data());
	Type* ptr_g = ptr_r + 1;
	Type* ptr_b = ptr_g + 1;
	
	/* convert colorspace */
	Vec3 color;
	int i = width * height;
	while (i --> 0) {
		/* unpack RGB information from data */
		color.x = unpack<Type>(ptr_r[channel * i]);
		color.y = unpack<Type>(ptr_g[channel * i]);
		color.z = unpack<Type>(ptr_b[channel * i]);
		
		/* convert to SRGB color space */
		color = Color::rgb_to_srgb(color);
		
		/* pack SRGB color to data */
		ptr_r[channel * i] = pack<Type>(color.x);
		ptr_g[channel * i] = pack<Type>(color.y);
		ptr_b[channel * i] = pack<Type>(color.z);
	}
}

template <typename Type>
void Image::convert_srgb_to_rgb() {
	/* get pointer of the specified data type */
	Type* ptr_r = reinterpret_cast<Type*>(data.data());
	Type* ptr_g = ptr_r + 1;
	Type* ptr_b = ptr_g + 1;
	
	/* convert colorspace */
	Vec3 color;
	int i = width * height;
	while (i --> 0) {
		/* unpack SRGB information from data */
		color.x = unpack<Type>(ptr_r[channel * i]);
		color.y = unpack<Type>(ptr_g[channel * i]);
		color.z = unpack<Type>(ptr_b[channel * i]);
		
		/* convert to RGB color space */
		color = Color::srgb_to_rgb(color);
		
		/* pack RGB color to data */
		ptr_r[channel * i] = pack<Type>(color.x);
		ptr_g[channel * i] = pack<Type>(color.y);
		ptr_b[channel * i] = pack<Type>(color.z);
	}
}

template <typename Type>
void Image::convert_rgb_to_xyz() {
	/* get pointers of the specified data type */
	Type* ptr_r = reinterpret_cast<Type*>(data.data());
	Type* ptr_g = ptr_r + 1;
	Type* ptr_b = ptr_g + 1;
	
	/* convert colorspace */
	Vec3 color;
	int i = width * height;
	while (i --> 0) {
		/* unpack RGB information from data */
		color.x = unpack<Type>(ptr_r[channel * i]);
		color.y = unpack<Type>(ptr_g[channel * i]);
		color.z = unpack<Type>(ptr_b[channel * i]);
		
		/* convert to XYZ color space */
		color = Color::rgb_to_xyz(color);
		
		/* pack XYZ color to data */
		ptr_r[channel * i] = pack<Type>(color.x);
		ptr_g[channel * i] = pack<Type>(color.y);
		ptr_b[channel * i] = pack<Type>(color.z);
	}
}

template <typename Type>
void Image::convert_xyz_to_rgb() {
	/* get pointers of the specified data type */
	Type* ptr_r = reinterpret_cast<Type*>(data.data());
	Type* ptr_g = ptr_r + 1;
	Type* ptr_b = ptr_g + 1;
	
	/* convert colorspace */
	Vec3 color;
	int i = width * height;
	while (i --> 0) {
		/* unpack XYZ information from data */
		color.x = unpack<Type>(ptr_r[channel * i]);
		color.y = unpack<Type>(ptr_g[channel * i]);
		color.z = unpack<Type>(ptr_b[channel * i]);
		
		/* convert to RGB color space */
		color = Color::xyz_to_rgb(color);
		
		/* pack RGB color to data */
		ptr_r[channel * i] = pack<Type>(color.x);
		ptr_g[channel * i] = pack<Type>(color.y);
		ptr_b[channel * i] = pack<Type>(color.z);
	}
}

template <typename Type>
void Image::convert_rgb_to_hsv() {
	/* get pointers of the specified data type */
	Type* ptr_r = reinterpret_cast<Type*>(data.data());
	Type* ptr_g = ptr_r + 1;
	Type* ptr_b = ptr_g + 1;
	
	/* convert colorspace */
	Vec3 color;
	int i = width * height;
	while (i --> 0) {
		/* unpack RGB information from data */
		color.x = unpack<Type>(ptr_r[channel * i]);
		color.y = unpack<Type>(ptr_g[channel * i]);
		color.z = unpack<Type>(ptr_b[channel * i]);
		
		/* convert to HSV color space */
		color = Color::rgb_to_hsv(color);
		
		/* pack HSV color to data */
		ptr_r[channel * i] = pack<Type>(color.x);
		ptr_g[channel * i] = pack<Type>(color.y);
		ptr_b[channel * i] = pack<Type>(color.z);
	}
}

template <typename Type>
void Image::convert_hsv_to_rgb() {
	/* get pointers of the specified data type */
	Type* ptr_r = reinterpret_cast<Type*>(data.data());
	Type* ptr_g = ptr_r + 1;
	Type* ptr_b = ptr_g + 1;
	
	/* convert colorspace */
	Vec3 color;
	int i = width * height;
	while (i --> 0) {
		/* unpack HSV information from data */
		color.x = unpack<Type>(ptr_r[channel * i]);
		color.y = unpack<Type>(ptr_g[channel * i]);
		color.z = unpack<Type>(ptr_b[channel * i]);
		
		/* convert to RGB color space */
		color = Color::hsv_to_rgb(color);
		
		/* pack RGB color to data */
		ptr_r[channel * i] = pack<Type>(color.x);
		ptr_g[channel * i] = pack<Type>(color.y);
		ptr_b[channel * i] = pack<Type>(color.z);
	}
}

template <typename Type>
void Image::convert_rgb_to_hsl() {
	/* get pointers of the specified data type */
	Type* ptr_r = reinterpret_cast<Type*>(data.data());
	Type* ptr_g = ptr_r + 1;
	Type* ptr_b = ptr_g + 1;
	
	/* convert colorspace */
	Vec3 color;
	int i = width * height;
	while (i --> 0) {
		/* unpack RGB information from data */
		color.x = unpack<Type>(ptr_r[channel * i]);
		color.y = unpack<Type>(ptr_g[channel * i]);
		color.z = unpack<Type>(ptr_b[channel * i]);
		
		/* convert to HSL color space */
		color = Color::rgb_to_hsl(color);
		
		/* pack HSL color to data */
		ptr_r[channel * i] = pack<Type>(color.x);
		ptr_g[channel * i] = pack<Type>(color.y);
		ptr_b[channel * i] = pack<Type>(color.z);
	}
}

template <typename Type>
void Image::convert_hsl_to_rgb() {
	/* get pointers of the specified data type */
	Type* ptr_r = reinterpret_cast<Type*>(data.data());
	Type* ptr_g = ptr_r + 1;
	Type* ptr_b = ptr_g + 1;
	
	/* convert colorspace */
	Vec3 color;
	int i = width * height;
	while (i --> 0) {
		/* unpack HSL information from data */
		color.x = unpack<Type>(ptr_r[channel * i]);
		color.y = unpack<Type>(ptr_g[channel * i]);
		color.z = unpack<Type>(ptr_b[channel * i]);
		
		/* convert to RGB color space */
		color = Color::hsl_to_rgb(color);
		
		/* pack RGB color to data */
		ptr_r[channel * i] = pack<Type>(color.x);
		ptr_g[channel * i] = pack<Type>(color.y);
		ptr_b[channel * i] = pack<Type>(color.z);
	}
}

template <typename Type>
void Image::convert_rgb_to_hcy() {
	/* get pointers of the specified data type */
	Type* ptr_r = reinterpret_cast<Type*>(data.data());
	Type* ptr_g = ptr_r + 1;
	Type* ptr_b = ptr_g + 1;
	
	/* convert colorspace */
	Vec3 color;
	int i = width * height;
	while (i --> 0) {
		/* unpack RGB information from data */
		color.x = unpack<Type>(ptr_r[channel * i]);
		color.y = unpack<Type>(ptr_g[channel * i]);
		color.z = unpack<Type>(ptr_b[channel * i]);
		
		/* convert to HCY color space */
		color = Color::rgb_to_hcy(color);
		
		/* pack HCY color to data */
		ptr_r[channel * i] = pack<Type>(color.x);
		ptr_g[channel * i] = pack<Type>(color.y);
		ptr_b[channel * i] = pack<Type>(color.z);
	}
}

template <typename Type>
void Image::convert_hcy_to_rgb() {
	/* get pointers of the specified data type */
	Type* ptr_r = reinterpret_cast<Type*>(data.data());
	Type* ptr_g = ptr_r + 1;
	Type* ptr_b = ptr_g + 1;
	
	/* convert colorspace */
	Vec3 color;
	int i = width * height;
	while (i --> 0) {
		/* unpack HCY information from data */
		color.x = unpack<Type>(ptr_r[channel * i]);
		color.y = unpack<Type>(ptr_g[channel * i]);
		color.z = unpack<Type>(ptr_b[channel * i]);
		
		/* convert to RGB color space */
		color = Color::hcy_to_rgb(color);
		
		/* pack RGB color to data */
		ptr_r[channel * i] = pack<Type>(color.x);
		ptr_g[channel * i] = pack<Type>(color.y);
		ptr_b[channel * i] = pack<Type>(color.z);
	}
}

template <typename Type>
float Image::unpack(Type v) {
	if constexpr (std::is_same_v<Type, float>) return v;
	return v / 255.f;
}

template <typename Type>
Type Image::pack(float v) {
	if constexpr (std::is_same_v<Type, float>) return v;
	return v < 0 ? 0 : v > 1 ? 255 : roundf(v * 255.f);
}

}
