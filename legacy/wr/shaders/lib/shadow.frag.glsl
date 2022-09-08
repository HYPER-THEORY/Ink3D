#include <common>
#include <poisson>

#define SHADOW_HARD			0
#define SHADOW_PCF			1
#define SHADOW_PCSS			2

#define MAX_FILTER_RADIUS	15

uniform sampler2D target_map;
uniform sampler2D normal_map;
uniform sampler2D depth_map;
uniform sampler2D shadow_map;

uniform int shadow_type;
uniform float darkness;
uniform float bias;
uniform float normal_bias;
uniform float texel_size;
uniform int filter_radius;
uniform int light_weight;
uniform vec3 light_dir;
uniform mat4 proj;
uniform mat4 inv_view_proj;
uniform mat4 light_view_proj;

in vec2 tex_coord;

out vec4 out_color;

float get_bias(const in vec3 normal, const in int radius) {
	float sin_theta = 1. - dot(normal, light_dir);
	float normal_bias_factor = sin_theta * texel_size * radius;
	return normal_bias_factor * normal_bias + bias;
}

float search_blocker(const in vec2 uv, const in float zreceiver,
					 const in int radius, const in vec3 normal) {
	float mixed_bias = get_bias(normal, radius);
	float biased_zreceiver = zreceiver - mixed_bias;
	float blocker_sum = 0.;
	float blocker_count = 0.;
	for (int i = 0; i < 16; ++i) {
		vec2 offset = poisson_2d_16[i] * texel_size * radius;
		float shadow_dist = texture(shadow_map, uv + offset).x;
		float accept = step(shadow_dist, biased_zreceiver);
		blocker_sum += accept * shadow_dist;
		blocker_count += accept;
	}
	return blocker_sum / max(1., blocker_count);
}

float pcf(const in vec2 uv, const in float zreceiver,
		  const in int radius, const in vec3 normal) {
	float mixed_bias = get_bias(normal, radius);
	float biased_zreceiver = zreceiver - mixed_bias;
	float shadow_sum = 0.;
	int half_radius = radius / 2;
	for (int x = -half_radius; x <= half_radius; ++x) {
		for (int y = -half_radius; y <= half_radius; ++y) {
			vec2 offset = vec2(x, y) * texel_size;
			float shadow_dist = texture(shadow_map, uv + offset).x;
			shadow_sum += step(shadow_dist, biased_zreceiver);
		}
	}
	return shadow_sum / (radius * radius);
}

float pcss(const in vec2 uv, const in float zreceiver, const in vec3 normal) {
	float zblocker = search_blocker(uv, zreceiver, MAX_FILTER_RADIUS, normal);
	float blocker = linearize_depth(proj, zblocker);
	float receiver = linearize_depth(proj, zreceiver);
	float penumbra = (receiver - blocker) / blocker * light_weight;
	int filter_radius = clamp(int(penumbra), 1, MAX_FILTER_RADIUS / 2) * 2 + 1;
	return pcf(uv, zreceiver, filter_radius, normal);
}

void main() {
	/* sample from textures */
	vec4 target = texture(target_map, tex_coord);
	float depth = texture(depth_map, tex_coord).x;
	vec3 normal = normalize(texture(normal_map, tex_coord).xyz);
	out_color = target;
	/* ignore pixels on back face or on skybox */
	if (dot(normal, light_dir) < 0. || depth == 1.) return;
	/* transform point to light space */
	vec4 ndc = vec4(tex_coord.xy * 2. - 1., depth * 2. - 1., 1.);
	vec4 light_ndc = light_view_proj * (inv_view_proj * ndc);
	vec3 light_coord = light_ndc.xyz / light_ndc.w * .5 + .5;
	/* ignore pixels outside shadowmap */
	if (light_coord.x < 0. || light_coord.y < 0. ||
		light_coord.x > 1. || light_coord.y > 1. ||
		light_ndc.w < 0.) return;
	/* calculate shadow factor */
	float shadow_factor = 0.;
	if (shadow_type == SHADOW_HARD) {
		shadow_factor = pcf(light_coord.xy, light_coord.z, 1, normal);
	} else if (shadow_type == SHADOW_PCF) {
		shadow_factor = pcf(light_coord.xy, light_coord.z, filter_radius, normal);
	} else if (shadow_type == SHADOW_PCSS) {
		shadow_factor = pcss(light_coord.xy, light_coord.z, normal);
	}
	/* apply shadow factor to target */
	out_color.xyz = target.xyz * (1. - darkness * shadow_factor);
}
