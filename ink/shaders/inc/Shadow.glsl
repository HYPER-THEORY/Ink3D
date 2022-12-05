#ifndef SHADOW_GLSL
#define SHADOW_GLSL

#include <poisson>
#include <transform>

#define SHADOW_HARD          0
#define SHADOW_PCF           1
#define SHADOW_PCSS          2

#define MIN_FILTER_RADIUS    1.
#define MAX_SEARCH_RADIUS    16.

#if SHADOW_SAMPLES == 16
#define POISSON_2D poisson_2d_16
#elif SHADOW_SAMPLES == 32
#define POISSON_2D poisson_2d_32
#elif SHADOW_SAMPLES == 64
#define POISSON_2D poisson_2d_64
#endif

struct GlobalShadow {
	sampler2DArray map;
	vec2 size;
};

struct Shadow {
	int type;
	int map_id;
	float bias;
	float normal_bias;
	float radius;
	mat4 proj;
	mat4 view_proj;
};

uniform GlobalShadow global_shadow;

/* Returns true if the coordinate is out of screen. */
bool out_of_screen(vec3 coord) {
	return any(bvec2(any(bvec3(coord.x < 0., coord.y < 0., coord.z < 0.)),
					 any(bvec3(coord.x > 1., coord.y > 1., coord.z > 1.))));
}

/* Searches for the depth of the neighbouring blockers. */
float search_blocker(Shadow shadow, vec3 light_pos, vec2 texel_size, float radius) {
	float blocker_accept_sum = 0.;
	float blocker_sum = 0.;
	for (int i = 0; i < SHADOW_SAMPLES; ++i) {
		vec2 offset = texel_size * radius * POISSON_2D[i];
		vec3 coord = vec3(light_pos.xy + offset, shadow.map_id);
		float shadow_distance = textureLod(global_shadow.map, coord, 0).x;
		float accept = step(shadow_distance, light_pos.z);
		blocker_accept_sum += accept;
		blocker_sum += accept * shadow_distance;
	}
	return blocker_sum / max(1., blocker_accept_sum);
}

/* Calculates the shadow with hard edge. */
float shadow_hard(Shadow shadow, vec3 light_pos) {
	vec3 coord = vec3(light_pos.xy, shadow.map_id);
	float shadow_distance = textureLod(global_shadow.map, coord, 0).x;
	return step(shadow_distance, light_pos.z);
}

/* Calculates the Percentage Closer Filtering shadow (PCF). */
float shadow_pcf(Shadow shadow, vec3 light_pos, vec2 texel_size, float radius) {
	float shadow_sum = 0.;
	for (int i = 0; i < SHADOW_SAMPLES; ++i) {
		vec2 offset = texel_size * radius * POISSON_2D[i];
		vec3 coord = vec3(light_pos.xy + offset, shadow.map_id);
		float shadow_distance = textureLod(global_shadow.map, coord, 0).x;
		shadow_sum += step(shadow_distance, light_pos.z);
	}
	return shadow_sum / SHADOW_SAMPLES;
}

/* Calculates the Percentage Closer Soft Shadow (PCSS). */
float shadow_pcss(Shadow shadow, vec3 light_pos, vec2 texel_size, mat4 proj) {
	float zblocker = search_blocker(shadow, light_pos, texel_size, MAX_SEARCH_RADIUS);
	float blocker = linearize_depth(zblocker, proj);
	float receiver = linearize_depth(light_pos.z, proj);
	float penumbra = (receiver - blocker) / blocker * shadow.radius;
	float filter_radius = max(penumbra, MIN_FILTER_RADIUS);
	return shadow_pcf(shadow, light_pos, texel_size, filter_radius);
}

/* Calculates the shadow factor by shadow mapping. */
float shadowing(Shadow shadow, vec3 world_pos, vec3 normal) {
	/* calculate light position */
	world_pos += normal * shadow.normal_bias;
	vec4 light_ndc = shadow.view_proj * vec4(world_pos, 1.);
	vec3 light_pos = light_ndc.xyz / light_ndc.w * 0.5 + 0.5;
	light_pos.z -= shadow.bias;
	
	/* ignore the pixels outside shadow map */
	if (out_of_screen(light_pos)) return 1.;
	vec2 texel_size = 1. / global_shadow.size;
	
	/* calculate shadow factor with edge fade */
	float factor = 1.;
	if (shadow.type == SHADOW_HARD) {
		factor = shadow_hard(shadow, light_pos);
	} else if (shadow.type == SHADOW_PCF) {
		factor = shadow_pcf(shadow, light_pos, texel_size, shadow.radius);
	} else if (shadow.type == SHADOW_PCSS) {
		factor = shadow_pcss(shadow, light_pos, texel_size, shadow.proj);
	}
	return 1. - factor;
}

#endif
