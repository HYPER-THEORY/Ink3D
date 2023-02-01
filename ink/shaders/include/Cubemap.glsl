#ifndef CUBEMAP_GLSL
#define CUBEMAP_GLSL

/* Converts face uv to unnormalized direction. */
vec3 face_to_cube(vec2 uv, int face) {
	vec3 dir = vec3(uv, 1.);
	if (face == 0) return vec3(dir.z, -dir.y, -dir.x);
	if (face == 1) return vec3(-dir.z, -dir.y, dir.x);
	if (face == 2) return vec3(dir.x, dir.z, dir.y);
	if (face == 3) return vec3(dir.x, -dir.z, -dir.y);
	if (face == 4) return vec3(dir.x, -dir.y, dir.z);
	if (face == 5) return vec3(-dir.x, -dir.y, -dir.z);
	return vec3(0., 0., 0.);
}

/* Converts normalized direction to equirectangular uv. */
vec2 cube_to_equirect(vec3 dir) {
	float u = atan(dir.z, dir.x) * INV_PI * 0.5 + 0.5;
	float v = asin(clamp(dir.y, -1., 1.)) * INV_PI + 0.5;
	return vec2(u, v);
}

#endif
