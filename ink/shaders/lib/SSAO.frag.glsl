#include <common>
#include <packing>
#include <transform>
#include <poisson>

#if SAMPLES == 16
#define POISSON_3D poisson_3d_16
#elif SAMPLES == 32
#define POISSON_3D poisson_3d_32
#elif SAMPLES == 64
#define POISSON_3D poisson_3d_64
#endif

uniform sampler2D buffer_n;
uniform sampler2D buffer_d;

uniform float radius;
uniform float max_radius;
uniform float bias;
uniform float intensity;
uniform float camera_near;
uniform float camera_far;
uniform mat4 view_proj;
uniform mat4 inv_view_proj;

in vec2 v_uv;

layout(location = 0) out vec4 out_color;

bool out_of_screen(vec2 coord) {
	return any(bvec4(coord.x < 0., coord.x > 1., coord.y < 0., coord.y > 1.));
}

float ssao(vec3 world_pos, vec3 normal, float linear_depth, vec3 noise) {
	float ao_sum = 0.;
	float ao_count = 0.;
	float ao_bias = radius * linear_depth * bias;
	
	/* sample from the mix of poisson disks and noise */
	for (int i = 0; i < SAMPLES; ++i) {
		
		/* calculate offset along with the normal */
		vec3 offset = mix(POISSON_3D[i], noise, 0.2) * radius;
		offset *= step(0., dot(offset, normal)) * 2. - 1.;
		
		/* transform world space to screen space */
		vec4 ndc = view_proj * vec4(world_pos + offset, 1.);
		vec3 coord = ndc.xyz / ndc.w * 0.5 + 0.5;
		
		/* discard when the coord is out of screen */
		if (out_of_screen(coord.xy)) continue;
		
		/* calcualte linear depth A and B */
		float depth = textureLod(buffer_d, coord.xy, 0).x;
		float linear_a = linearize_depth_persp(depth, camera_near, camera_far);
		float linear_b = linearize_depth_persp(coord.z, camera_near, camera_far);
		
		/* check whether the sample is valid */
		float accept = step(linear_b - linear_a, max_radius);
		
		/* compare both depths and accumulate ao factor */
		ao_sum += accept * step(linear_a, linear_b - ao_bias) *
			dot(normalize(offset), normal) * (1 - length(offset));
		ao_count += accept * 0.5 + 0.5;
	}
	return ao_sum / max(0.1, ao_count);
}

void main() {
	/* sample depth from textures */
	float depth = textureLod(buffer_d, v_uv, 0).x;
	
	/* ignore the pixels on skybox */
	out_color = vec4(1.);
	if (depth == 1.) return;
	
	/* sample world depth from textures */
	vec3 normal = textureLod(buffer_n, v_uv, 0).xyz;
	
	/* normalize normal to avoid artifacts */
	normal = normalize(unpack_normal(normal));
	
	/* transform from screen space to world space */
	vec4 ndc = vec4(vec3(v_uv.xy, depth) * 2. - 1., 1.);
	vec4 world_pos = inv_view_proj * ndc;
	world_pos /= world_pos.w;
	
	/* calculate AO factor */
	float a = rand(vec3(v_uv, 0.));
	float b = rand(vec3(v_uv, 1.));
	float c = rand(vec3(v_uv, 2.));
	float linear_depth = linearize_depth_persp(depth, camera_near, camera_far);
	float factor = ssao(world_pos.xyz, normal, linear_depth, vec3(a, b, c));
	
	/* output AO factor to map */
	out_color = vec4(1. - intensity * factor);
}
