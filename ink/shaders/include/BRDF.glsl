#ifndef BRDF_GLSL
#define BRDF_GLSL

#include <common>

/* Returns the value of Lambert BRDF. */
vec3 brdf_lambert(vec3 diffuse) {
	return diffuse * INV_PI;
}

/* Calculates Fresnel with Schlick's approximation. */
vec3 f_schlick(vec3 f0, float voh) {
	float fresnel = exp2((-5.55473 * voh - 6.98316) * voh);
	return f0 * (1. - fresnel) + fresnel;
}

/* Calculates Geometry with GGX-Smith model. */
float v_ggx(float a, float nol, float nov) {
	float a2 = a * a;
	float gv = nol * sqrt(a2 + (1. - a2) * nov * nov);
	float gl = nov * sqrt(a2 + (1. - a2) * nol * nol);
	return 0.5 / max(gv + gl, EPS);
}

/* Calculates NDF with GGX model. */
float d_ggx(float a, float noh) {
	float a2 = a * a;
	float d = noh * noh * (a2 - 1.) + 1.;
	return INV_PI * a2 / (d * d);
}

/* Returns the value of GGX BRDF (Schlick Fresnel, GGX-Smith Geometry, GGX NDF). */
vec3 brdf_ggx(vec3 light_dir, vec3 view_dir, vec3 normal, vec3 f0, float roughness) {
	float a = roughness * roughness;
	vec3 half_dir = normalize(light_dir + view_dir);
	float nol = saturate(dot(normal, light_dir));
	float nov = saturate(dot(normal, view_dir));
	float noh = saturate(dot(normal, half_dir));
	float voh = saturate(dot(view_dir, half_dir));
	return v_ggx(a, nol, nov) * d_ggx(a, noh) * f_schlick(f0, voh);
}

/* Returns the approximation of the DFG LUT. */
vec2 lut_dfg(vec3 normal, vec3 view_dir, float roughness) {
	const vec4 c0 = vec4(-1., -0.0275, -0.572, 0.022);
	const vec4 c1 = vec4(1. , 0.0425 , 1.04  , -0.04);
	float nov = saturate(dot(normal, view_dir));
	vec4 r = roughness * c0 + c1;
	float a004 = min(r.x * r.x, exp2(-9.28 * nov)) * r.x + r.y;
	return vec2(-1.04, 1.04) * a004 + r.zw;
}

/* Returns the value of multiple-scattering microfacet model. */
void scattering(vec3 normal, vec3 view_dir, vec3 f0, float roughness,
				inout vec3 single_scatter, inout vec3 multi_scatter) {
	vec2 f_ab = lut_dfg(normal, view_dir, roughness);
	vec3 fss_ess = f0 * f_ab.x + f_ab.y;
	float ess = f_ab.x + f_ab.y;
	float ems = 1. - ess;
	vec3 favg = f0 + (1. - f0) * 0.047619;
	vec3 fms = fss_ess * favg / (1. - ems * favg);
	single_scatter += fss_ess;
	multi_scatter += fms * ems;
}

#endif
