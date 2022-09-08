#include <common>

uniform sampler2D map;

uniform vec2 direction;
uniform int radius;

in vec2 v_uv;

layout(location = 0) out TYPE out_color;

void main() {
	TYPE blur_sum = TYPE(0.);
	float weight_sum = 0;
	
	/* prepare for gaussian filtering */
	#ifdef BLUR_GAUSSIAN
		float factor_s = 1. / (SIGMA_S * SIGMA_S);
	#endif
	
	/* prepare for bilateral filtering */
	#ifdef BLUR_BILATERAL
		float factor_s = 1. / (SIGMA_S * SIGMA_S);
		float factor_r = 1. / (SIGMA_R * SIGMA_R);
		TYPE center_color = textureLod(map, v_uv, 0) SWIZZLE;
	#endif
	
	/* sampling along the direction */
	for (float i = -radius + 1; i < radius; ++i) {
		vec2 offset = direction * i;
		TYPE color = textureLod(map, v_uv + offset, 0) SWIZZLE;
		
		/* the weight of simple filtering */
		#ifdef BLUR_SIMPLE
			float weight = 1.;
		#endif
		
		/* the weight of gaussian filtering */
		#ifdef BLUR_GAUSSIAN
			float weight = exp(i * i * factor_s * -0.5);
		#endif
		
		/* the weight of bilateral filtering */
		#ifdef BLUR_BILATERAL
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
