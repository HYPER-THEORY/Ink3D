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

#include "Image.h"

#include <algorithm>

#include "../core/Error.h"

namespace Ink {

constexpr float ONE_BY_255 = 1.f / 255.f;

Image::Image(int w, int h, int c, int b) :
width(w), height(h), channel(c), bytes(b) {
	data.resize(w * h * c * b);
}

Image Image::subimage(int x1, int y1, int x2, int y2) {
	if (x1 > x2) std::swap(x1, x2);
	if (y1 > y2) std::swap(y1, y2);
	
	/* check whether the sub-image region is legal */
	if (x1 < 0 || x2 > width || y1 < 0 || y2 > height) {
		Error::set("Image: Illegal slicing region");
		return Image();
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

void Image::convert(int c) {
	/* check the number of channels */
	if (channel != 3 && channel != 4) {
		return Error::set("Image: Image's channel must be 3 or 4");
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
	Type* ptr = reinterpret_cast<Type*>(data.data());
	Type* ptr_r = ptr + 0;
	Type* ptr_b = ptr + 2;
	
	/* swap the red channel and blur channel */
	int i = width * height;
	while (i --> 0) {
		std::swap(*ptr_r, *ptr_b);
		ptr_r += channel;
		ptr_b += channel;
	}
}

template <typename Type>
void Image::convert_bgr_to_rgb() {
	/* get pointers of the specified data type */
	Type* ptr = reinterpret_cast<Type*>(data.data());
	Type* ptr_r = ptr + 0;
	Type* ptr_b = ptr + 2;
	
	/* swap the red channel and blur channel */
	int i = width * height;
	while (i --> 0) {
		std::swap(*ptr_r, *ptr_b);
		ptr_r += channel;
		ptr_b += channel;
	}
}

template <typename Type>
void Image::convert_rgb_to_srgb() {
	/* get pointer of the specified data type */
	Type* ptr = reinterpret_cast<Type*>(data.data());
	
	/* convert colorspace */
	int i = width * height * channel;
	while (i --> 0) {
		/* unpack color information from data */
		float color = unpack<Type>(*ptr);
		
		/* convert to SRGB color space */
		if (color <= 0.0031308f) {
			color *= 12.92f;
		} else {
			color = powf(color, 1.f / 2.4f) * 1.055f - 0.055f;
		}
		*ptr = pack<Type>(color);
		
		/* update pointers to next color */
		++ptr;
	}
}

template <typename Type>
void Image::convert_srgb_to_rgb() {
	/* get pointer of the specified data type */
	Type* ptr = reinterpret_cast<Type*>(data.data());
	
	/* convert colorspace */
	int i = width * height * channel;
	while (i --> 0) {
		/* unpack color information from data */
		float color = unpack<Type>(*ptr);
		
		/* convert to SRGB color space */
		if (color <= 0.04045f) {
			color /= 12.92f;
		} else {
			color = powf((color + 0.055f) / 1.055f, 2.4f);
		}
		*ptr = pack<Type>(color);
		
		/* update pointers to next color */
		++ptr;
	}
}

template <typename Type>
void Image::convert_rgb_to_xyz() {
	/* get pointers of the specified data type */
	Type* ptr = reinterpret_cast<Type*>(data.data());
	Type* ptr_r = ptr + 0;
	Type* ptr_g = ptr + 1;
	Type* ptr_b = ptr + 2;
	
	/* convert colorspace */
	int i = width * height;
	while (i --> 0) {
		/* unpack RGB information from data */
		float r = unpack<Type>(*ptr_r);
		float g = unpack<Type>(*ptr_g);
		float b = unpack<Type>(*ptr_b);
		
		/* convert to XYZ color space */
		*ptr_r = pack<Type>(r * 0.4124564f + g * 0.3575761f + b * 0.1804375f);
		*ptr_g = pack<Type>(r * 0.2126729f + g * 0.7151522f + b * 0.0721750f);
		*ptr_b = pack<Type>(r * 0.0193339f + g * 0.1191920f + b * 0.9503041f);
		
		/* update pointers to next color */
		ptr_r += channel;
		ptr_g += channel;
		ptr_b += channel;
	}
}

template <typename Type>
void Image::convert_xyz_to_rgb() {
	/* get pointers of the specified data type */
	Type* ptr = reinterpret_cast<Type*>(data.data());
	Type* ptr_r = ptr + 0;
	Type* ptr_g = ptr + 1;
	Type* ptr_b = ptr + 2;
	
	/* convert colorspace */
	int i = width * height;
	while (i --> 0) {
		/* unpack XYZ information from data */
		float x = unpack<Type>(*ptr_r);
		float y = unpack<Type>(*ptr_g);
		float z = unpack<Type>(*ptr_b);
		
		/* convert to RGB color space */
		*ptr_r = pack<Type>(x * 3.2404542f + y * -1.5371385f + z * -0.4985314f);
		*ptr_g = pack<Type>(x * -0.9692660f + y * 1.8760108f + z * 0.0415560f);
		*ptr_b = pack<Type>(x * 0.0556434f + y * -0.2040259f + z * 1.0572252f);
		
		/* update pointers to next color */
		ptr_r += channel;
		ptr_g += channel;
		ptr_b += channel;
	}
}

template <typename Type>
void Image::convert_rgb_to_hsv() {
	/* get pointers of the specified data type */
	Type* ptr = reinterpret_cast<Type*>(data.data());
	Type* ptr_r = ptr + 0;
	Type* ptr_g = ptr + 1;
	Type* ptr_b = ptr + 2;
	
	/* convert colorspace */
	int i = width * height;
	while (i --> 0) {
		/* unpack RGB information from data */
		float r = unpack<Type>(*ptr_r);
		float g = unpack<Type>(*ptr_g);
		float b = unpack<Type>(*ptr_b);
		
		/* convert to HSV color space */
		Vec3 hcv = rgb_to_hcv(r, g, b);
		*ptr_r = pack<Type>(hcv.x);
		*ptr_g = pack<Type>(hcv.y / hcv.z);
		*ptr_b = pack<Type>(hcv.z);
		
		/* update pointers to next color */
		ptr_r += channel;
		ptr_g += channel;
		ptr_b += channel;
	}
}

template <typename Type>
void Image::convert_hsv_to_rgb() {
	/* get pointers of the specified data type */
	Type* ptr = reinterpret_cast<Type*>(data.data());
	Type* ptr_r = ptr + 0;
	Type* ptr_g = ptr + 1;
	Type* ptr_b = ptr + 2;
	
	/* convert colorspace */
	int i = width * height;
	while (i --> 0) {
		/* unpack HSV information from data */
		float x = unpack<Type>(*ptr_r);
		float y = unpack<Type>(*ptr_g);
		float z = unpack<Type>(*ptr_b);
		
		/* convert to RGB color space */
		Vec3 rgb = ((hue_to_rgb(x) - 1.f) * y + 1.f) * z;
		*ptr_r = pack<Type>(rgb.x);
		*ptr_g = pack<Type>(rgb.y);
		*ptr_b = pack<Type>(rgb.z);
		
		/* update pointers to next color */
		ptr_r += channel;
		ptr_g += channel;
		ptr_b += channel;
	}
}

template <typename Type>
void Image::convert_rgb_to_hsl() {
	/* get pointers of the specified data type */
	Type* ptr = reinterpret_cast<Type*>(data.data());
	Type* ptr_r = ptr + 0;
	Type* ptr_g = ptr + 1;
	Type* ptr_b = ptr + 2;
	
	/* convert colorspace */
	int i = width * height;
	while (i --> 0) {
		/* unpack RGB information from data */
		float r = unpack<Type>(*ptr_r);
		float g = unpack<Type>(*ptr_g);
		float b = unpack<Type>(*ptr_b);
		
		/* convert to HSL color space */
		Vec3 hcv = rgb_to_hcv(r, g, b);
		float l = hcv.z - hcv.y * 0.5f;
		float s = hcv.y / (1.f - fabsf(l * 2.f - 1.f));
		*ptr_r = pack<Type>(hcv.x);
		*ptr_g = pack<Type>(s);
		*ptr_b = pack<Type>(l);
		
		/* update pointers to next color */
		ptr_r += channel;
		ptr_g += channel;
		ptr_b += channel;
	}
}

template <typename Type>
void Image::convert_hsl_to_rgb() {
	/* get pointers of the specified data type */
	Type* ptr_r = reinterpret_cast<Type*>(data.data());
	Type* ptr_g = reinterpret_cast<Type*>(data.data() + 1 * bytes);
	Type* ptr_b = reinterpret_cast<Type*>(data.data() + 2 * bytes);
	
	/* convert colorspace */
	int i = width * height;
	while (i --> 0) {
		/* unpack HSL information from data */
		float x = unpack<Type>(*ptr_r);
		float y = unpack<Type>(*ptr_g);
		float z = unpack<Type>(*ptr_b);
		
		/* convert to RGB color space */
		float c = (1.f - fabsf(2.f * z - 1.f)) * y;
		Vec3 rgb = (hue_to_rgb(x) - 0.5f) * c + z;
		*ptr_r = pack<Type>(rgb.x);
		*ptr_g = pack<Type>(rgb.y);
		*ptr_b = pack<Type>(rgb.z);
		
		/* update pointers to next color */
		ptr_r += channel;
		ptr_g += channel;
		ptr_b += channel;
	}
}

template <typename Type>
void Image::convert_rgb_to_hcy() {
	const Vec3 w_hcy = {0.299, 0.587, 0.114};
	
	/* get pointers of the specified data type */
	Type* ptr_r = reinterpret_cast<Type*>(data.data());
	Type* ptr_g = reinterpret_cast<Type*>(data.data() + 1 * bytes);
	Type* ptr_b = reinterpret_cast<Type*>(data.data() + 2 * bytes);
	
	/* convert colorspace */
	int i = width * height;
	while (i --> 0) {
		/* unpack RGB information from data */
		float r = unpack<Type>(*ptr_r);
		float g = unpack<Type>(*ptr_g);
		float b = unpack<Type>(*ptr_b);
		
		/* convert to HCY color space */
		Vec3 hcv = rgb_to_hcv(r, g, b);
		float y = w_hcy.dot({r, g, b});
		float z = w_hcy.dot(hue_to_rgb(hcv.x));
		hcv.y *= y < z ? z / y : (1.f - z) / (1.f - y);
		*ptr_r = pack<Type>(hcv.x);
		*ptr_g = pack<Type>(hcv.y);
		*ptr_b = pack<Type>(y);
		
		/* update pointers to next color */
		ptr_r += channel;
		ptr_g += channel;
		ptr_b += channel;
	}
}

template <typename Type>
void Image::convert_hcy_to_rgb() {
	const Vec3 w_hcy = {0.299, 0.587, 0.114};
	
	/* get pointers of the specified data type */
	Type* ptr_r = reinterpret_cast<Type*>(data.data());
	Type* ptr_g = reinterpret_cast<Type*>(data.data() + 1 * bytes);
	Type* ptr_b = reinterpret_cast<Type*>(data.data() + 2 * bytes);
	
	/* convert colorspace */
	int i = width * height;
	while (i --> 0) {
		/* unpack HSL information from data */
		float x = unpack<Type>(*ptr_r);
		float y = unpack<Type>(*ptr_g);
		float z = unpack<Type>(*ptr_b);
		
		/* convert to RGB color space */
		Vec3 rgb = hue_to_rgb(x);
		float d = w_hcy.dot(rgb);
		y *= z < d ? z / d : d < 1.f ? (1.f - z) / (1.f - d) : 1.f;
		rgb = (rgb - d) * y + z;
		*ptr_r = pack<Type>(rgb.x);
		*ptr_g = pack<Type>(rgb.y);
		*ptr_b = pack<Type>(rgb.z);
		
		/* update pointers to next color */
		ptr_r += channel;
		ptr_g += channel;
		ptr_b += channel;
	}
}

template <typename Type>
float Image::unpack(Type v) {
	if (typeid(Type) == typeid(float)) return v;
	return v * ONE_BY_255;
}

template <typename Type>
Type Image::pack(float v) {
	if (typeid(Type) == typeid(float)) return v;
	return roundf(saturate(v) * 255.f);
}

float Image::saturate(float v) {
	return v < 0 ? 0 : v > 1 ? 1 : v;
}

Vec3 Image::rgb_to_hcv(float r, float g, float b) {
	Vec4 p = g < b ? Vec4{b, g, -1.f, 2.f / 3.f} : Vec4{g, b, 0.f, -1.f / 3.f};
	Vec4 q = r < p.x ? Vec4{p.x, p.y, p.w, r} : Vec4{r, p.y, p.z, p.x};
	float c = q.x - fminf(q.w, q.y);
	float h = fabsf((q.w - q.y) / (6.f * c) + q.z);
	return {h, c, q.x};
}

Vec3 Image::hue_to_rgb(float h) {
	float r = fabsf(h * 6.f - 3.f) - 1.f;
	float g = 2.f - fabsf(h * 6.f - 2.f);
	float b = 2.f - fabsf(h * 6.f - 4.f);
	return {saturate(r), saturate(g), saturate(b)};
}

}
