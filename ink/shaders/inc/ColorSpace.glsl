#ifndef COLORSPACE_GLSL
#define COLORSPACE_GLSL

const float srgb_gamma = 1. / 2.2;
const float srgb_inv_gamma = 2.2;
const float srgb_alpha = 0.055;

const mat3 rgb_to_xyz_mat = mat3(
	0.4124564, 0.3575761, 0.1804375,
	0.2126729, 0.7151522, 0.0721750,
	0.0193339, 0.1191920, 0.9503041
);

const mat3 xyz_to_rgb_mat = mat3(
	 3.2404542, -1.5371385, -0.4985314,
	-0.9692660,  1.8760108,  0.0415560,
	 0.0556434, -0.2040259,  1.0572252
);

const vec3 hcy_vec = vec3(0.299, 0.587, 0.114);

const float color_eps = 1e-10;

/* Converts from RGB color space to SRGB color space (approximated). */
vec3 rgb_to_srgb_approx(vec3 rgb) {
	return pow(rgb, vec3(srgb_gamma));
}

/* Converts from SRGB color space to RGB color space (approximated). */
vec3 srgb_to_rgb_approx(vec3 srgb) {
	return pow(srgb, vec3(srgb_inv_gamma));
}

/* Converts from RGB color space to SRGB color space. */
float rgb_to_srgb(float r) {
	if (r <= 0.0031308) return r * 12.92;
	return pow(r, 1. / 2.4) * (1. + srgb_alpha) - srgb_alpha;
}

/* Converts from RGB color space to SRGB color space. */
vec3 rgb_to_srgb(vec3 rgb) {
	return vec3(rgb_to_srgb(rgb.r), rgb_to_srgb(rgb.g), rgb_to_srgb(rgb.b));
}

/* Converts from SRGB color space to RGB color space. */
float srgb_to_rgb(float r) {
	if (r <= 0.04045) return r / 12.92;
	return pow((r + srgb_alpha) / (1. + srgb_alpha), 2.4);
}

/* Converts from SRGB color space to RGB color space. */
vec3 srgb_to_rgb(vec3 srgb) {
	return vec3(srgb_to_rgb(srgb.r), srgb_to_rgb(srgb.g), srgb_to_rgb(srgb.b));
}

/* Converts from RGB color space to XYZ color space. */
vec3 rgb_to_xyz(vec3 rgb) {
	return rgb_to_xyz_mat * rgb;
}

/* Converts from XYZ color space to RGB color space. */
vec3 xyz_to_rgb(vec3 xyz) {
	return xyz_to_rgb_mat * xyz;
}

/* Converts from XYZ color space to xyY color space. */
vec3 xyz_to_xyy(vec3 xyz) {
	float x = xyz.x / (xyz.x + xyz.y + xyz.z);
	float y = xyz.y / (xyz.x + xyz.y + xyz.z);
	return vec3(x, y, xyz.y);
}

/* Converts from xyY color space to XYZ color space. */
vec3 xyy_to_xyz(vec3 xyy) {
	float x = xyy.z * xyy.x / xyy.y;
	float z = xyy.z * (1. - xyy.x - xyy.y) / xyy.y;
	return vec3(x, xyy.z, z);
}

/* Converts from RGB color space to HCV color space. */
vec3 rgb_to_hcv(vec3 rgb) {
	vec4 p = (rgb.g < rgb.b) ? vec4(rgb.bg, -1., 2. / 3.) : vec4(rgb.gb, 0., -1. / 3.);
	vec4 q = (rgb.r < p.x) ? vec4(p.xyw, rgb.r) : vec4(rgb.r, p.yzx);
	float c = q.x - min(q.w, q.y);
	float h = abs((q.w - q.y) / (6. * c + color_eps) + q.z);
	return vec3(h, c, q.x);
}

/* Converts from hue to RGB color space. */
vec3 hue_to_rgb(float hue) {
	float r = abs(hue * 6. - 3.) - 1.;
	float g = 2. - abs(hue * 6. - 2.);
	float b = 2. - abs(hue * 6. - 4.);
	return saturate(vec3(r, g, b));
}

/* Converts from RGB color space to HSY color space. */
vec3 rgb_to_hsv(vec3 rgb) {
	vec3 hcv = rgb_to_hcv(rgb);
	float s = hcv.y / (hcv.z + color_eps);
	return vec3(hcv.x, s, hcv.z);
}

/* Converts from HSV color space to RGB color space. */
vec3 hsv_to_rgb(vec3 hsv) {
	vec3 rgb = hue_to_rgb(hsv.x);
	return ((rgb - 1.) * hsv.y + 1.) * hsv.z;
}

/* Converts from RGB color space to HSL color space. */
vec3 rgb_to_hsl(vec3 rgb) {
	vec3 hcv = rgb_to_hcv(rgb);
	float l = hcv.z - hcv.y * 0.5;
	float s = hcv.y / (1. - abs(l * 2. - 1.) + color_eps);
	return vec3(hcv.x, s, l);
}

/* Converts from HSL color space to RGB color space. */
vec3 hsl_to_rgb(vec3 hsl) {
	vec3 rgb = hue_to_rgb(hsl.x);
	float c = (1. - abs(2. * hsl.z - 1.)) * hsl.y;
	return (rgb - 0.5) * c + hsl.z;
}

/* Converts from RGB color space to HCY color space. */
vec3 rgb_to_hcy(vec3 rgb) {
	vec3 hcv = rgb_to_hcv(rgb);
	float y = dot(rgb, hcy_vec);
	float z = dot(hue_to_rgb(hcv.x), hcy_vec);
	hcv.y *= y < z ? z / (color_eps + y) : (1. - z) / (color_eps + 1. - y);
	return vec3(hcv.x, hcv.y, y);
}

/* Converts from HCY color space to RGB color space. */
vec3 hcy_to_rgb(vec3 hcy) {
	vec3 rgb = hue_to_rgb(hcy.x);
	float z = dot(rgb, hcy_vec);
	hcy.y *= hcy.z < z ? hcy.z / z : z < 1. ? (1. - hcy.z) / (1. - z) : 1.;
	return (rgb - z) * hcy.y + hcy.z;
}

#endif
