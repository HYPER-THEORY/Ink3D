#ifndef PACKING_GLSL
#define PACKING_GLSL

const float pack_upscale = 256. / 255.;
const float unpack_downscale = 255. / 256.;
const vec4 pack_factors = vec4(16777216., 65536., 256., 1.);
const vec4 unpack_factors = unpack_downscale / pack_factors;

/* Pack normal to rgb. */
vec3 pack_normal(vec3 normal) {
	return normal * .5 + .5;
}

/* Unpack rgb to normal. */
vec3 unpack_normal(vec3 rgb) {
	return rgb * 2. - 1.;
}

/* Pack float to rgba. */
vec4 pack_float(float v) {
	const float one_by_256 = 1. / 256.;
	vec4 r = fract(v * pack_factors);
	r.yzw -= r.xyz * one_by_256;
	return r * pack_upscale;
}

/* Unpack rgba to float. */
float unpack_float(vec4 v) {
	return dot(v, unpack_factors);
}

/* Pack vec2 to rgba. */
vec4 pack_vec2(vec2 v) {
	vec4 r = vec4(v.x, fract(v.x * 255.), v.y, fract(v.y * 255.));
	return vec4(r.x - r.y / 255., r.y, r.z - r.w / 255., r.w);
}

/* Unpack rgba to vec2. */
vec2 unpack_vec2(vec4 v) {
	return vec2(v.x + (v.y / 255.), v.z + (v.w / 255.));
}

#endif
