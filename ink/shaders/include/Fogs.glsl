#ifndef FOGS_GLSL
#define FOGS_GLSL

#include <common>

struct LinearFog {
	bool visible;
	vec3 color;
	float near;
	float far;
};

struct Exp2Fog {
	bool visible;
	vec3 color;
	float near;
	float density;
};

/* Applies the specified linear fog to color. */
void apply_fog(LinearFog fog, float camera_distance, inout vec3 color) {
	if (!fog.visible) return;
	float factor = saturate((fog.far - camera_distance) / (fog.far - fog.near));
	color = mix(fog.color, color, factor);
}

/* Applies the specified exp square fog to color. */
void apply_fog(Exp2Fog fog, float camera_distance, inout vec3 color) {
	if (!fog.visible) return;
	float fog_distance = max(0, camera_distance - fog.near);
	float factor = exp(-square(fog_distance * fog.density));
	color = mix(fog.color, color, factor);
}

#endif
