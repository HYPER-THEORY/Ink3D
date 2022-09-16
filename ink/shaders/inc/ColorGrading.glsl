#ifndef COLORGRADING_GLSL
#define COLORGRADING_GLSL

vec3 color_grading(vec3 color, vec3 saturation, vec3 contrast, vec3 gamma, vec3 gain, vec3 offset) {
	float luma = relative_luminance(color);
	color = max(vec3(0.), mix(vec3(luma), color, saturation));
	color = pow(color * 5.55555555, contrast) * 0.18;
	color = pow(color, 1. / gamma);
	return color * gain + offset;
}

#endif
