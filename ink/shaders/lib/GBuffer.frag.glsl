#include <common>
#include <packing>
#include <brdf>
#include <iblfilter>

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 model_view;
uniform mat4 model_view_proj;
uniform mat3 normal_mat;
uniform vec3 camera_pos;

uniform float alpha_test;

#ifdef USE_NORMAL_MAP
uniform sampler2D normal_map;
uniform float normal_scale;
#endif

uniform vec3 color;
uniform float alpha;

#ifdef USE_COLOR_MAP
uniform sampler2D color_map;
#endif

uniform float metalness;

#ifdef USE_METALNESS_MAP
uniform sampler2D metalness_map;
#endif

uniform float roughness;

#ifdef USE_ROUGHNESS_MAP
uniform sampler2D roughness_map;
#endif

uniform float specular;

#ifdef USE_SPECULAR_MAP
uniform sampler2D specular_map;
#endif

uniform vec3 emissive;
uniform float emissive_intensity;

#ifdef USE_EMISSIVE_MAP
uniform sampler2D emissive_map;
#endif

#ifdef USE_ENV_PROBE
uniform samplerCube env_map;
uniform int env_max_lod;
uniform float env_intensity;
#endif

in vec2 v_uv;
in vec3 v_normal;
in vec3 v_world_pos;

#ifdef USE_TANGENT
in vec3 v_tangent;
in vec3 v_bitangent;
#endif

layout(location = LOCATION_C) out vec4 out_gbuffer_c;
layout(location = LOCATION_N) out vec4 out_gbuffer_n;
layout(location = LOCATION_M) out vec4 out_gbuffer_m;
layout(location = LOCATION_E) out vec4 out_gbuffer_e;
layout(location = LOCATION_I) out vec4 out_gbuffer_i;

void main() {
	/* calculate color */
	vec4 t_color = vec4(color, alpha);
	#ifdef USE_COLOR_MAP
		t_color *= texture(color_map, v_uv);
	#endif
	
	/* alpha test */
	if (t_color.w < alpha_test) discard;
	
	/* calculate normal */
	vec3 t_normal = normalize(v_normal);
	#ifdef USE_TANGENT
		vec3 tangent = normalize(v_tangent);
		vec3 bitangent = normalize(v_bitangent);
		mat3 tbn_mat = mat3(tangent, bitangent, t_normal);
	#endif
	#ifdef USE_NORMAL_MAP
		vec3 normal = unpack_normal(texture(normal_map, v_uv).xyz);
		normal.xy *= normal_scale;
		#ifdef USE_TANGENT
			t_normal = normalize(tbn_mat * normal);
		#else
			t_normal = normalize(normal_mat * normal);
		#endif
	#endif
	
	/* calculate metalness */
	float t_metalness = metalness;
	#ifdef USE_METALNESS_MAP
		t_metalness *= texture(metalness_map, v_uv).x;
	#endif
	
	/* calculate roughness */
	float t_roughness = roughness;
	#ifdef USE_METALNESS_MAP
		t_roughness *= texture(roughness_map, v_uv).x;
	#endif
	t_roughness = max(t_roughness, 0.0525);
	
	/* calculate specular */
	float t_specular = specular;
	#ifdef USE_SPECULAR_MAP
		t_specular *= texture(specular_map, v_uv).x;
	#endif
	
	/* calculate emissive */
	vec4 t_emissive = vec4(emissive, emissive_intensity);
	#ifdef USE_EMISSIVE_MAP
		t_emissive *= texture(emissive_map, v_uv);
	#endif
	
	/* calculate diffuse color */
	vec3 diffuse = t_color.xyz * (1 - t_metalness);
	
	/* calculate specular F0 */
	float non_metal = min(square((t_specular - 1.) / (t_specular + 1.)), 1.);
	vec3 specular_f0 = mix(vec3(non_metal), t_color.xyz, t_metalness);
	
	/* calculate indirect diffuse & specular light */
	vec3 view_dir = normalize(camera_pos - v_world_pos);
	vec3 in_diffuse = vec3(0.);
	vec3 in_specular = vec3(0.);
	
	/* apply environment probe */
	#ifdef USE_ENV_PROBE
		/* calculate single and multi scattering */
		vec3 single_scattering = vec3(0.);
		vec3 multi_scattering = vec3(0.);
		multiscatter(t_normal, view_dir, t_color.xyz, t_roughness, single_scattering, multi_scattering);
		
		/* calculate diffuse and specular from environment map */
		vec3 env_diffuse = ibl_diffuse(env_map, env_max_lod, t_normal) * ONE_BY_PI * env_intensity;
		vec3 env_specular = ibl_specular(env_map, env_max_lod, view_dir, t_normal, t_roughness) * env_intensity;
		
		/* add contributions to indirect light */
		in_diffuse += diffuse * (1 - (single_scattering + multi_scattering)) * env_diffuse;
		in_specular += single_scattering * env_specular;
		in_specular += multi_scattering * env_diffuse;
	#endif
	
	/* calculate indirect light */
	vec3 in_light = in_diffuse + in_specular;
	
	/* output G-Buffers */
	out_gbuffer_c = vec4(diffuse, t_color.w);
	out_gbuffer_n = vec4(pack_normal(t_normal), 0.);
	out_gbuffer_m = vec4(specular_f0, t_roughness);
	out_gbuffer_e = t_emissive;
	out_gbuffer_i = vec4(in_light, 0.);
}
