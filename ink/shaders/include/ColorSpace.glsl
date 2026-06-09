#ifndef COLORSPACE_GLSL
#define COLORSPACE_GLSL

#include <common>

const float SRGB_GAMMA = 1. / 2.2;
const float SRGB_INV_GAMMA = 2.2;

const mat3 RGB_TO_XYZ = mat3(
	vec3( 0.4124564,  0.3575761,  0.1804375),
	vec3( 0.2126729,  0.7151522,  0.0721750),
	vec3( 0.0193339,  0.1191920,  0.9503041)
);

const mat3 XYZ_TO_RGB = mat3(
	vec3( 3.2404542, -1.5371385, -0.4985314),
	vec3(-0.9692660,  1.8760108,  0.0415560),
	vec3( 0.0556434, -0.2040259,  1.0572252)
);

/* Converts from RGB color space to SRGB color space (approximated). */
vec3 rgb_to_srgb_approx(vec3 rgb) {
	return pow(rgb, vec3(SRGB_GAMMA));
}

/* Converts from SRGB color space to RGB color space (approximated). */
vec3 srgb_to_rgb_approx(vec3 srgb) {
	return pow(srgb, vec3(SRGB_INV_GAMMA));
}

/* Converts from RGB color space to SRGB color space. */
float rgb_to_srgb(float r) {
	return r <= 0.0031308 ? r * 12.92 : pow(r, 1. / 2.4) * 1.055 - 0.055;
}

/* Converts from RGB color space to SRGB color space. */
vec3 rgb_to_srgb(vec3 rgb) {
	return vec3(rgb_to_srgb(rgb.r), rgb_to_srgb(rgb.g), rgb_to_srgb(rgb.b));
}

/* Converts from SRGB color space to RGB color space. */
float srgb_to_rgb(float r) {
	return r <= 0.04045 ? r / 12.92 : pow((r + 0.055) / 1.055, 2.4);
}

/* Converts from SRGB color space to RGB color space. */
vec3 srgb_to_rgb(vec3 srgb) {
	return vec3(srgb_to_rgb(srgb.r), srgb_to_rgb(srgb.g), srgb_to_rgb(srgb.b));
}

/* Converts from RGB color space to XYZ color space. */
vec3 rgb_to_xyz(vec3 rgb) {
	return RGB_TO_XYZ * rgb;
}

/* Converts from XYZ color space to RGB color space. */
vec3 xyz_to_rgb(vec3 xyz) {
	return XYZ_TO_RGB * xyz;
}

/* Converts from hue to RGB color space. */
vec3 hue_to_rgb(float hue) {
	float r = abs(hue * 6. - 3.) - 1.;
	float g = 2. - abs(hue * 6. - 2.);
	float b = 2. - abs(hue * 6. - 4.);
	return saturate(vec3(r, g, b));
}

/* Converts from RGB color space to HCV color space. */
vec3 rgb_to_hcv(vec3 rgb) {
	vec4 p = (rgb.g < rgb.b) ? vec4(rgb.bg, -1., 2. / 3.) : vec4(rgb.gb, 0., -1. / 3.);
	vec4 q = (rgb.r < p.x) ? vec4(p.xyw, rgb.r) : vec4(rgb.r, p.yzx);
	float chr = q.x - min(q.w, q.y);
	float hue = abs((q.w - q.y) / (6. * chr + EPS) + q.z);
	return vec3(hue, chr, q.x);
}

/* Converts from RGB color space to HSV color space. */
vec3 rgb_to_hsv(vec3 rgb) {
	vec3 hcv = rgb_to_hcv(rgb);
	return vec3(hcv.x, hcv.y / (hcv.z + EPS), hcv.z);
}

/* Converts from HSV color space to RGB color space. */
vec3 hsv_to_rgb(vec3 hsv) {
	return ((hue_to_rgb(hsv.x) - 1.) * hsv.y + 1.) * hsv.z;
}

/* Converts from RGB color space to HSL color space. */
vec3 rgb_to_hsl(vec3 rgb) {
	vec3 hcv = rgb_to_hcv(rgb);
	float lum = hcv.z - hcv.y * 0.5;
	float sat = hcv.y / (1. - abs(lum * 2. - 1.) + EPS);
	return vec3(hcv.x, sat, lum);
}

/* Converts from HSL color space to RGB color space. */
vec3 hsl_to_rgb(vec3 hsl) {
	return (hue_to_rgb(hsl.x) - 0.5) * (1. - abs(2. * hsl.z - 1.)) * hsl.y + hsl.z;
}

#endif
