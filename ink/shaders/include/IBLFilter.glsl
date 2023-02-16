#ifndef IBLFILTER_GLSL
#define IBLFILTER_GLSL

/* Sample the environment map with roughness. */
vec4 texture_env(samplerCube map, float max_lod, vec3 dir, float roughness) {
	float mip = 0.;
	if (roughness >= 0.4) {
		mip = (0.8 - roughness) * 7.5 - 1.;
	} else if (roughness >= 0.305) {
		mip = (0.4 - roughness) / 0.095 + 2.;
	} else if (roughness >= 0.21) {
		mip = (0.305 - roughness) / 0.095 + 3.;
	} else {
		mip = -2. * log2(1.16 * roughness);
	}
	mip = clamp(max_lod - mip, 0., max_lod);
	return textureLod(map, dir, mip);
}

/* Calcualte specular light from the environment map. */
vec3 ibl_specular(samplerCube map, float max_lod, vec3 view_dir, vec3 normal, float roughness) {
	vec3 reflect_vec = reflect(-view_dir, normal);
	reflect_vec = normalize(mix(reflect_vec, normal, roughness * roughness));
	return texture_env(map, max_lod, reflect_vec, roughness).xyz;
}

/* Calcualte diffuse light from the environment map. */
vec3 ibl_diffuse(samplerCube map, float max_lod, vec3 normal) {
	return PI * texture_env(map, max_lod, normal, 1.).xyz;
}

#endif
