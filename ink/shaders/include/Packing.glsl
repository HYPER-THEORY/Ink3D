#ifndef PACKING_GLSL
#define PACKING_GLSL

/* Packs normal to RGB. */
vec3 pack_normal(vec3 normal) {
	return normal * 0.5 + 0.5;
}

/* Unpacks RGB to normal. */
vec3 unpack_normal(vec3 rgb) {
	return rgb * 2. - 1.;
}

/* Packs vec2 to RGBA. */
vec4 pack_vec2(vec2 v) {
	vec4 r = vec4(v.x, fract(v.x * 255.), v.y, fract(v.y * 255.));
	return vec4(r.x - r.y / 255., r.y, r.z - r.w / 255., r.w);
}

/* Unpacks RGBA to vec2. */
vec2 unpack_vec2(vec4 v) {
	return vec2(v.x + (v.y / 255.), v.z + (v.w / 255.));
}

#endif
