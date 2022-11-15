#include <common>

uniform sampler2D map;

uniform vec2 direction;
uniform int radius;
uniform float lod = 0;
uniform float sigma_s;
uniform float sigma_r;

in vec2 v_uv;

layout(location = 0) out TYPE out_color;

void main() {
	TYPE blur_sum = TYPE(0.);
	float weight_sum = 0;
	
	#ifdef BLUR_GAUSSIAN
		/* prepare for gaussian filtering */
		float factor_s = 1. / (sigma_s * sigma_s);
	#endif
	
	#ifdef BLUR_BILATERAL
		/* prepare for bilateral filtering */
		float factor_s = 1. / (sigma_s * sigma_s);
		float factor_r = 1. / (sigma_r * sigma_r);
		TYPE center_color = textureLod(map, v_uv, lod) SWIZZLE;
	#endif
	
	/* sampling along the direction */
	for (float i = -radius + 1; i < radius; ++i) {
		vec2 offset = direction * i;
		TYPE color = textureLod(map, v_uv + offset, lod) SWIZZLE;
		
		#ifdef BLUR_SIMPLE
			/* the weight of simple filtering */
			float weight = 1.;
		#endif
		
		#ifdef BLUR_GAUSSIAN
			/* the weight of gaussian filtering */
			float weight = exp(i * i * factor_s * -0.5);
		#endif
		
		#ifdef BLUR_BILATERAL
			/* the weight of bilateral filtering */
			TYPE delta_color = center_color - color;
			float weight_r = dot(delta_color, delta_color);
			float weight = exp((i * i * factor_s + weight_r * factor_r) * -0.5);
		#endif
		blur_sum += color * weight;
		weight_sum += weight;
	}
	
	/* output final color */
	out_color = blur_sum / weight_sum;
}
