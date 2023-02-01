#include <common>

uniform sampler2D map;

uniform float lod;
uniform vec2 direction;
uniform int radius;
uniform float sigma_s;

in vec2 v_uv;

layout(location = 0) out TYPE out_color;

void main() {
	TYPE blur_sum = TYPE(0.);
	float weight_sum = 0;
	
	/* prepare for Gaussian filtering */
	float factor_s = 1. / (sigma_s * sigma_s);
	
	/* sample color along the direction */
	for (float i = -radius + 1; i < radius; ++i) {
		vec2 offset = direction * i;
		TYPE color = textureLod(map, v_uv + offset, lod) SWIZZLE;
		float weight = exp(i * i * factor_s * -0.5);
		blur_sum += color * weight;
		weight_sum += weight;
	}
	
	/* output filtered color */
	out_color = blur_sum / weight_sum;
}
