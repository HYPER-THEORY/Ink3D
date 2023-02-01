#include <common>
#include <packing>
#include <transform>
#include <poisson>

#if SAMPLES == 16
#define POISSON_3D POISSON_3D_16
#elif SAMPLES == 32
#define POISSON_3D POISSON_3D_32
#elif SAMPLES == 64
#define POISSON_3D POISSON_3D_64
#endif

uniform sampler2D buffer_n;
uniform sampler2D buffer_d;

uniform float intensity;
uniform float radius;
uniform float max_radius;
uniform float max_depth;
uniform float near;
uniform float far;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 inv_proj;

in vec2 v_uv;

layout(location = 0) out vec4 out_color;

bool out_of_screen(vec2 coord) {
	return any(bvec4(coord.x < 0., coord.x > 1., coord.y < 0., coord.y > 1.));
}

void main() {
	/* sample depth from texture */
	float depth = textureLod(buffer_d, v_uv, 0).x;
	float linear_depth = linearize_depth_persp(depth, near, far);
	
	/* ignore the pixels on skybox */
	out_color = vec4(1., 1., 1., 1.);
	if (linear_depth > max_depth) return;
	
	/* sample world normal from texture */
	vec3 normal = textureLod(buffer_n, v_uv, 0).xyz;
	normal = normalize(unpack_normal(normal));
	normal = mat3(view) * normal;
	
	/* transform from screen space to world space */
	vec4 ndc = vec4(vec3(v_uv.xy, depth) * 2. - 1., 1.);
	vec4 view_pos = inv_proj * ndc;
	view_pos /= view_pos.w;
	
	/* generate noise from -1 to 1 */
	vec3 noise;
	noise.x = rand(vec3(v_uv, 0.));
	noise.y = rand(vec3(v_uv, 1.));
	noise.z = rand(vec3(v_uv, 2.));
	noise = noise * 2. - 1.;
	
	/* calculate ambient occlusion */
	float occlusion = 0.;
	for (int i = 0; i < SAMPLES; ++i) {
		
		/* calculate offset along with the normal */
		vec3 offset = mix(POISSON_3D[i], noise, 0.2) * radius;
		offset *= step(0., dot(offset, normal)) * 2. - 1.;
		vec3 sample_pos = view_pos.xyz + offset;
		
		/* transform view space to screen space */
		vec4 sample_ndc = proj * vec4(sample_pos, 1.);
		vec2 sample_uv = sample_ndc.xy / sample_ndc.w * 0.5 + 0.5;
		
		/* discard when sample UV is out of screen */
		if (out_of_screen(sample_uv)) continue;
		
		/* calcualte difference between linear depths */
		float sample_depth = textureLod(buffer_d, sample_uv, 0).x;
		float sample_z = linearize_depth_persp(sample_depth, near, far);
		float delta = -sample_pos.z - sample_z;
		
		/* compare depths and accumulate occlusion */
		float range_fade = 1. - smoothstep(max_radius * 0.75, max_radius, delta);
		occlusion += step(0., delta) * range_fade;
	}
	
	/* output ambient occlusion */
	occlusion *= 1. - smoothstep(max_depth * 0.75, max_depth, linear_depth);
	out_color = vec4(1. - intensity * occlusion / SAMPLES);
}
