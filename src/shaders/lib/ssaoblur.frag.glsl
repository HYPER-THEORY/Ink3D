#include <common>

#define SSAO_LOW	0
#define SSAO_HIGH	1

#define BLUR_RADIUS	5

uniform sampler2D ssao_map;

uniform int quality;
uniform int blending;
uniform float darkness;
uniform vec2 screen_size;

in vec2 tex_coord;

layout(location = 0) out vec4 out_color;

float blur(vec2 coord, int radius, vec2 texel_size) {
	float blur_sum = 0.;
	int half_radius = radius / 2;
	for (float x = -half_radius; x <= half_radius; ++x) {
		for (float y = -half_radius; y <= half_radius; ++y) {
			vec2 offset = texel_size * vec2(x, y);
			blur_sum += textureLod(ssao_map, coord + offset, 0).x;
		}
	}
	return blur_sum / radius / radius;
}

float bilateral_blur(vec2 coord, int radius, vec2 texel_size) {
	float blur_sum = 0.;
	float weight_sum = 0.;
	float radius_2 = radius * 2.;
	int half_radius = radius / 2;
	float mid_l = textureLod(ssao_map, coord, 0).x;
	for (float x = -half_radius; x <= half_radius; ++x) {
		for (float y = -half_radius; y <= half_radius; ++y) {
			vec2 offset = texel_size * vec2(x, y);
			float l = textureLod(ssao_map, coord + offset, 0).x;
			float similar = (mid_l - l) * (mid_l - l) * radius_2;
			float weight = exp((x * x + y * y + similar) * -0.5);
			blur_sum += l * weight;
			weight_sum += weight;
		}
	}
	return blur_sum / weight_sum;
}

void main() {
	/* calculate ssao factor */
	float ao_factor = 1.;
	vec2 texel_size = 1. / screen_size;
	if (quality == SSAO_LOW) {
		ao_factor = 1. - blur(tex_coord, BLUR_RADIUS, texel_size);
	} else if (quality == SSAO_HIGH) {
		ao_factor = 1. - bilateral_blur(tex_coord, BLUR_RADIUS, texel_size);
	}
	/* output ssao factor */
	out_color.xyz = vec3(1.) * max((1. - ao_factor * darkness), 0.);
	out_color.w = 1.;
}
