#include <common>
#include <poisson>

uniform sampler2D normal_map;
uniform sampler2D depth_map;

uniform float radius;
uniform float max_radius;
uniform float bias;
uniform mat4 proj;
uniform mat4 view_proj;
uniform mat4 inv_view_proj;

in vec2 tex_coord;

layout(location = 0) out vec4 out_color;

bool out_of_screen(vec2 coord) {
	return coord.x < 0. || coord.x > 1. || coord.y < 0. || coord.y > 1.;
}

float ssao(vec3 world_coord, vec3 normal, float linear_depth, vec3 noise) {
	float ao_sum = 0.;
	float ao_count = 0.;
	float ao_bias = radius * linear_depth * bias;
	for (int i = 0; i < 64; ++i) {
		/* calculate offset along with the normal */
		vec3 offset = mix(poisson_3d_64[i], noise, .2) * radius;
		offset *= step(0., dot(offset, normal)) * 2. - 1.;
		/* transform world space to screen space */
		vec4 ndc = view_proj * vec4(world_coord + offset, 1.);
		vec3 coord = ndc.xyz / ndc.w * .5 + .5;
		/* discard when the coord is out of screen */
		if (out_of_screen(coord.xy)) continue;
		/* calcualte linear depths */
		float depth = textureLod(depth_map, coord.xy, 0).x;
		float linear_a = linearize_depth(proj, depth);
		float linear_b = linearize_depth(proj, coord.z);
		/* check if the sample is valid */
		float accept = step(linear_b - linear_a, max_radius);
		/* compare both depths and accumulate ao factor */
		ao_sum += accept * step(linear_a, linear_b - ao_bias) *
			dot(normalize(offset), normal) * (1 - length(offset));
		ao_count += accept * .5 + .5;
	}
	return ao_sum / max(.1, ao_count);
}

void main() {
	/* sample from textures */
	float depth = textureLod(depth_map, tex_coord, 0).x;
	vec3 normal = normalize(textureLod(normal_map, tex_coord, 0).xyz);
	out_color.x = 1.;
	/* ignore pixels on skybox */
	if (depth == 1.) return;
	/* transform screen space to world space */
	vec4 ndc = vec4(tex_coord.xy * 2. - 1., depth * 2. - 1., 1.);
	vec4 world_coord = inv_view_proj * ndc;
	world_coord /= world_coord.w;
	/* calculate ao factor */
	float a = rand(vec3(tex_coord, 0.));
	float b = rand(vec3(tex_coord, 1.));
	float c = rand(vec3(tex_coord, 2.));
	float linear_depth = linearize_depth(proj, depth);
	float ao_factor = ssao(world_coord.xyz, normal, linear_depth, vec3(a, b, c));
	/* output ao factor to map */
	out_color.x = 1. - ao_factor;
}
