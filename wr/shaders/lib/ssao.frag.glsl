#include <common>
#include <poisson>

uniform sampler2D target_map;
uniform sampler2D normal_map;
uniform sampler2D depth_map;

uniform float darkness;
uniform float radius;
uniform float max_radius;
uniform float bias;
uniform mat4 proj;
uniform mat4 view_proj;
uniform mat4 inv_view_proj;

in vec2 tex_coord;

out vec4 out_color;

float ssao(const in vec3 world_coord, const in vec3 normal,
		   const in float linear_depth, const in vec3 noise) {
	float ao_sum = 0.;
	float ao_count = 0.;
	float ao_bias = radius * linear_depth * bias;
	for (int i = 0; i < 64; ++i) {
		vec3 offset = mix(poisson_3d_64[i], noise, .5) * radius;
		offset *= step(0., dot(offset, normal)) * 2. - 1.;
		vec4 ndc = view_proj * vec4(world_coord + offset, 1.);
		vec3 coord = ndc.xyz / ndc.w * .5 + .5;
		if (coord.x < 0. || coord.y < 0. ||
			coord.x > 1. || coord.y > 1.) continue;
		float depth = texture(depth_map, coord.xy).x;
		float linear_a = linearize_depth(proj, depth);
		float linear_b = linearize_depth(proj, coord.z);
		float accept = step(linear_b - linear_a, max_radius);
		ao_sum += accept * step(linear_a, linear_b - ao_bias) *
			dot(normalize(offset), normal) * (1 - length(offset));
		ao_count += accept * .5 + .5;
	}
	return ao_sum / max(.1, ao_count);
}

void main() {
	/* sample from textures */
	vec4 target = texture(target_map, tex_coord);
	float depth = texture(depth_map, tex_coord).x;
	vec3 normal = normalize(texture(normal_map, tex_coord).xyz);
	out_color = target;
	/* ignore pixels on skybox */
	if (depth == 1.) return;
	/* transform point to light space */
	vec4 ndc = vec4(tex_coord.xy * 2. - 1., depth * 2. - 1., 1.);
	vec4 world_coord = inv_view_proj * ndc;
	world_coord /= world_coord.w;
	/* calculate ao factor */
	float a = rand(vec3(tex_coord, 0.));
	float b = rand(vec3(tex_coord, 1.));
	float c = rand(vec3(tex_coord, 2.));
	float linear_depth = linearize_depth(proj, depth);
	float ao_factor = ssao(world_coord.xyz, normal, linear_depth, vec3(a, b, c));
	/* apply ao factor to target */
	out_color.xyz = vec3(1.) * (1. - ao_factor);
//	out_color = vec4(target.xyz * (1. - darkness * ao_factor), target.w);
}
