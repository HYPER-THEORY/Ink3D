#include <common>
#include <poisson>

#define SHADOW_HARD			0
#define SHADOW_PCF			1
#define SHADOW_PCSS			2

#define MIN_FILTER_RADIUS	3
#define MAX_FILTER_RADIUS	15

uniform sampler2D normal_map;
uniform sampler2D depth_map;
uniform sampler2D shadow_map;

uniform int type;
uniform float darkness;
uniform float bias;
uniform float normal_bias;
uniform vec2 screen_size;
uniform int filter_radius;
uniform int light_weight;
uniform vec3 light_dir;
uniform mat4 proj;
uniform mat4 inv_view_proj;
uniform mat4 light_view_proj;

in vec2 tex_coord;

layout(location = 0) out vec4 out_color;

bool out_of_screen(vec2 coord) {
	return coord.x < 0. || coord.x > 1. || coord.y < 0. || coord.y > 1.;
}

float get_bias(vec2 texel_size, int radius, vec3 normal) {
	float sin_theta = 1. - dot(normal, light_dir);
	float normal_bias_factor = max(texel_size.x, texel_size.y) * sin_theta * radius * 2.;
	return normal_bias_factor * normal_bias + bias;
}

float search_blocker(vec2 uv, vec2 texel_size, float zreceiver, int radius, vec3 normal) {
	float mixed_bias = get_bias(texel_size, radius, normal);
	float biased_zreceiver = zreceiver - mixed_bias;
	float blocker_sum = 0.;
	float blocker_count = 0.;
	for (int i = 0; i < 64; ++i) {
		vec2 offset = texel_size * radius * poisson_2d_64[i];
		float shadow_dist = textureLod(shadow_map, uv + offset, 0).x;
		float accept = step(shadow_dist, biased_zreceiver);
		blocker_sum += accept * shadow_dist;
		blocker_count += accept;
	}
	return blocker_sum / max(1., blocker_count);
}

float pcf(vec2 uv, vec2 texel_size, float zreceiver, int radius, vec3 normal) {
	float mixed_bias = get_bias(texel_size, radius, normal);
	float biased_zreceiver = zreceiver - mixed_bias;
	float shadow_sum = 0.;
	for (int i = 0; i < 64; ++i) {
		vec2 offset = texel_size * radius * poisson_2d_64[i];
		float shadow_dist = textureLod(shadow_map, uv + offset, 0).x;
		shadow_sum += step(shadow_dist, biased_zreceiver);
	}
	return shadow_sum * .015625;
}

float pcss(vec2 uv, vec2 texel_size, float zreceiver, vec3 normal) {
	float zblocker = search_blocker(uv, texel_size, zreceiver, MAX_FILTER_RADIUS, normal);
	float blocker = linearize_depth(proj, zblocker);
	float receiver = linearize_depth(proj, zreceiver);
	float penumbra = (receiver - blocker) / blocker * light_weight;
	int filter_radius = clamp(int(penumbra), MIN_FILTER_RADIUS, MAX_FILTER_RADIUS);
	return pcf(uv, texel_size, zreceiver, filter_radius, normal);
}

void main() {
	out_color = vec4(1.);
	/* sample from textures */
	float depth = textureLod(depth_map, tex_coord, 0).x;
	vec3 normal = normalize(textureLod(normal_map, tex_coord, 0).xyz);
	/* ignore pixels on back face or on skybox */
	if (dot(normal, light_dir) < 0. || depth == 1.) return;
	/* transform screen space to light space */
	vec4 ndc = vec4(tex_coord.xy * 2. - 1., depth * 2. - 1., 1.);
	vec4 light_ndc = light_view_proj * (inv_view_proj * ndc);
	vec3 light_coord = light_ndc.xyz / light_ndc.w * .5 + .5;
	/* ignore pixels outside shadowmap */
	if (out_of_screen(light_coord.xy) || light_ndc.w < 0.) return;
	/* calculate shadow factor */
	vec2 texel_size = 1. / screen_size;
	float shadow_factor = 0.;
	if (type == SHADOW_HARD) {
		shadow_factor = pcf(light_coord.xy, texel_size, light_coord.z, 1, normal);
	} else if (type == SHADOW_PCF) {
		shadow_factor = pcf(light_coord.xy, texel_size, light_coord.z, filter_radius, normal);
	} else if (type == SHADOW_PCSS) {
		shadow_factor = pcss(light_coord.xy, texel_size, light_coord.z, normal);
	}
	/* output shadow factor */
	out_color.xyz *= 1. - darkness * shadow_factor;
}
